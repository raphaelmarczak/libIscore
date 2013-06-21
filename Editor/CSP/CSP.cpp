/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak, Bruno Valeze
Last modification: 08/03/2010

Adviser(s): Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)

This software is a computer program whose purpose is to propose
a library for interactive scores edition and execution.

This software is governed by the CeCILL-C license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL-C
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-C license and that you accept its terms.
*/

/*!
 * \file CSP.cpp
 * \author Raphael Marczak (LaBRI), based on Bruno Valeze (LaBRI) code
 * \date 2008-2009
 */

#include "../gecode-solver/solver.hpp"
#include "../../StoryLine/StoryLine.hpp"

#include <iostream>
using namespace std;

#include "CSP.hpp"

#include "AllenRelation.hpp"
#include "ConstrainedBox.hpp"
#include "ConstrainedTemporalEntity.hpp"
#include "CSPConstrainedVariable.hpp"
#include "CSPLinearConstraint.hpp"
#include "ControlPoint.hpp"
#include "IntervalRelation.hpp"

string
CSP::getNameFromRelation(TemporalRelationType t)
{
	switch(t)
	{
	case ANTPOST_ANTERIORITY :
		return ANTPOST_ANTERIORITY_STR;
	case ANTPOST_POSTERIORITY :
		return ANTPOST_POSTERIORITY_STR;
	}

	return "";
}

CSP::CSP()
{
	_solver = new Solver();

	_cedEntities = new map<unsigned int, ConstrainedTemporalEntity*>;
	_temporalRelations = new vector<BinaryTemporalRelation*>;
	_triggerPoints = new map<unsigned int, TriggerPoint*>;
}

CSP::~CSP()
{
	delete _triggerPoints;
	delete _temporalRelations;
	delete _cedEntities;

	delete _solver;
}

unsigned int
CSP::addBox(unsigned int boxId, int boxBeginPos, int boxLength, unsigned int motherId, int maxSceneWidth)
{
	map<unsigned int, ConstrainedTemporalEntity *>::iterator iter = (*_cedEntities).find(boxId);
	if(iter != (*_cedEntities).end())  {
		throw IllegalArgumentException();
	}

	CSPConstrainedVariable *begin = new CSPConstrainedVariable(_solver->addIntVar(1, maxSceneWidth, boxBeginPos, (int)BEGIN_VAR_TYPE),
			1, maxSceneWidth, boxBeginPos, BEGIN_VAR_TYPE);
//	CSPConstrainedVariable *length = new CSPConstrainedVariable(_solver->addIntVar(10, maxSceneWidth, (int)boxLength, (int)LENGTH_VAR_TYPE),
//			10, maxSceneWidth, (int)boxLength, LENGTH_VAR_TYPE);

	// *******************************************************************************************************************************
	// *******************************************************************************************************************************
	// Author : Nicolas Hincker
	//  Pour palier au problème de compression/étirement indésirable de boîtes lors de simples déplacements
	// -> on interdit par défaut toute modification de durée si on est dans le cas d'un simple déplacement :
	//    on réduit le domaine de la variable contrainte length à [boxLength,boxLength] ainsi la durée est quoiqu'il arrive non modifiable.
	//
	// -> Cependant la boîte sera unlocked dans le cas d'un resize et non d'un déplacement.
	//
	// Voir performMoving().
	// *******************************************************************************************************************************

	CSPConstrainedVariable *length = new CSPConstrainedVariable(_solver->addIntVar((int)boxLength, (int)boxLength, (int)boxLength, (int)LENGTH_VAR_TYPE),
								    (int)boxLength, (int)boxLength, (int)boxLength, LENGTH_VAR_TYPE);


	// *******************************************************************************************************************************

	ConstrainedBox *newBox = new ConstrainedBox(begin, length);

	CSPConstrainedVariable *CP2begin = new CSPConstrainedVariable(_solver->addIntVar(1, maxSceneWidth, boxBeginPos + boxLength, (int)BEGIN_VAR_TYPE),
			1, maxSceneWidth, boxBeginPos + boxLength, BEGIN_VAR_TYPE);
	CSPConstrainedVariable *CP2length = new CSPConstrainedVariable(_solver->addIntVar(0, maxSceneWidth, 0, (int)LENGTH_VAR_TYPE),
			0, maxSceneWidth, 0, LENGTH_VAR_TYPE);


	newBox->addControlPoint(new ControlPoint(begin, length, boxId), BEGIN_CONTROL_POINT_INDEX);
	newBox->addControlPoint(new ControlPoint(CP2begin, CP2length, boxId), END_CONTROL_POINT_INDEX);

	addAllenRelation(newBox->getLastControlPoint(), newBox, ALLEN_FINISHES, false);

	newBox->getFirstControlPoint()->setProcessStepId(1);
	newBox->getLastControlPoint()->setProcessStepId(2);


	if (motherId != NO_ID) {
//		ConstrainedBox* mother = getBoxById(motherId);
//		newBox->setMother(mother);
//		mother->addChild(newBox);
	}

	(*_cedEntities)[boxId] = newBox;
	newBox->setId(boxId);

	//changeAllBoxMaxSceneWidth(10000);

	return newBox->getId();
}


void
CSP::removeBox(unsigned int boxId, std::vector<unsigned int>& relationsRemoved, std::vector<unsigned int>& triggersRemoved)
{
	ConstrainedBox* cedBox = getBoxById(boxId);

	relationsRemoved.clear();
	triggersRemoved.clear();

	/*
     remove cedBox from the boxes hierarchy
	 */

//	for (vector<ConstrainedBox*>::iterator it = cedBox->children()->begin() ; it != cedBox->children()->end() ; it++)
//	{
//		removeBoundingRelation((*it)->boundingRelation());
//		(*it)->setMother(cedBox->mother(), addBoundingRelation(cedBox->mother(), (*it)));
//	}

	if (cedBox->mother())
	{
//		removeBoundingRelation(cedBox->boundingRelation());
//		cedBox->mother()->removeChild(cedBox);
	}

	/*
     remove temporal constraints implicating cedBox
	 */

	vector<BinaryTemporalRelation*> *relToRemove = links(cedBox);
	for (vector<BinaryTemporalRelation*>::iterator it = relToRemove->begin() ; it != relToRemove->end() ; it++)
	{
		unsigned int relationId = (*it)->getId();
		relationsRemoved.push_back(relationId);
		removeTemporalRelation((*it));
		delete (*it);
	}
	relToRemove->clear();
	delete relToRemove;

	vector<unsigned int>* controlPointID = new vector <unsigned int>;
	cedBox->getAllControlPointsId(controlPointID);

	for (unsigned int i = 0 ; i < controlPointID->size() ; ++i) {
		ControlPoint* currentControlPoint = cedBox->getControlPoint(controlPointID->at(i));

		if(currentControlPoint->getTriggerPoint() != NULL) {
			TriggerPoint* currentTriggerPoint = currentControlPoint->getTriggerPoint();
			triggersRemoved.push_back(currentTriggerPoint->getTriggerId());
			currentTriggerPoint->removeRelatedControlPoint();
		}

		relToRemove = links(currentControlPoint);
		for (vector<BinaryTemporalRelation*>::iterator it = relToRemove->begin() ; it != relToRemove->end() ; it++)
		{
			removeTemporalRelation((*it));
			delete (*it);
		}

		_solver->removeIntVar(currentControlPoint->beginID());
		_solver->removeIntVar(currentControlPoint->lengthID());
	}

	/*
    remove related variables from the solver
	 */

	_solver->removeIntVar(cedBox->beginID());
	_solver->removeIntVar(cedBox->lengthID());

	/*
    remove cedBox from the constrained objects' list of the CSP
	 */

	_cedEntities->erase(boxId);
}

unsigned int
CSP::addAntPostRelation(unsigned int relationId, unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, int minBound, int maxBound, vector<unsigned int>& movedBoxes, bool mustCallSolver)
{
	if ((minBound != NO_BOUND) && (maxBound != NO_BOUND) && (minBound>maxBound)) {
		throw std::invalid_argument("CSP: minBound must be inferior to maxBound");
	}

	ConstrainedBox* constrainedBox1 = (ConstrainedBox*)getBoxById(boxId1);
	ConstrainedBox* constrainedBox2 = (ConstrainedBox*)getBoxById(boxId2);

	ConstrainedTemporalEntity *ent1 = constrainedBox1->getControlPoint(controlPoint1);
	ConstrainedTemporalEntity *ent2 = constrainedBox2->getControlPoint(controlPoint2);

	AntPostRelation *newAntPost = new AntPostRelation(ent1, ent2, type, minBound, maxBound);

	vector<int> *varsIDs = new vector<int>;
	vector<int> *varsCoeffs = new vector<int>;



	switch(type) {
	case ANTPOST_ANTERIORITY :
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(-1);

		if (minBound != NO_BOUND) {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GQ_RELATION, minBound, mustCallSolver) );
		} else {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GQ_RELATION, 0, mustCallSolver) );
		}

		if (maxBound != NO_BOUND) {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LQ_RELATION, maxBound, mustCallSolver) );
		}

		break;

	case ANTPOST_POSTERIORITY :
		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);

		if (minBound != NO_BOUND) {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GQ_RELATION, minBound, mustCallSolver) );
		} else {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GQ_RELATION, 0, mustCallSolver) );
		}

		if (maxBound != NO_BOUND) {
			newAntPost->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LQ_RELATION, maxBound, mustCallSolver) );
		}


		break;
	default : break;
	}

	if (mustCallSolver) {
		if (newAntPost->validate()) {
			_temporalRelations->push_back(newAntPost);
			newAntPost->setId(relationId);

			ent1->addRelatedEntity(ent2);
			ent2->addRelatedEntity(ent1);

			updateFromSolver();
		}
		else {
			for (vector<CSPLinearConstraint*>::iterator it = newAntPost->constraints()->begin() ; it != newAntPost->constraints()->end() ; it++) {
				if ( *it )
					removeConstraint(*it);
			}

			delete newAntPost;
			newAntPost = NULL;
		}

		delete varsIDs;
		delete varsCoeffs;


		movedBoxes.clear();

		map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();
		while (it != _cedEntities->end())
		{
			movedBoxes.push_back(it->first);
			it++;
		}


		if (newAntPost == NULL) {
			return NO_ID;
		} else {
			return newAntPost->getId();
		}
	} else {
		delete varsIDs;
		delete varsCoeffs;

		_temporalRelations->push_back(newAntPost);
		ent1->addRelatedEntity(ent2);
		ent2->addRelatedEntity(ent1);

		newAntPost->setId(relationId);

		return newAntPost->getId();
	}


}

//IntervalRelation*
//CSP::addInterval(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2, int value, int tolerance)
//{
//	CSPConstrainedVariable *var = new CSPConstrainedVariable(_solver->addIntVar(value - tolerance, value + tolerance, value, (int)INTERVAL_VAR_TYPE),
//			value - tolerance, value + tolerance, value, INTERVAL_VAR_TYPE);
//
//	IntervalRelation *newInter = new IntervalRelation(ent1, ent2, var, value, tolerance);
//
//	vector<int> *varsIDs = new vector<int>;
//	vector<int> *varsCoeffs = new vector<int>;
//
//	varsIDs->push_back(ent1->beginID());
//	varsIDs->push_back(ent1->lengthID());
//	varsIDs->push_back(ent2->beginID());
//	varsIDs->push_back(var->getID());
//	varsCoeffs->push_back(1);
//	varsCoeffs->push_back(1);
//	varsCoeffs->push_back(-1);
//	varsCoeffs->push_back(1);
//
//	CSPLinearConstraint *cst = addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, true);
//
//	if (cst) {
//		newInter->addLinearConstraint( cst );
//
//		ent1->addRelatedEntity(ent2);
//		ent2->addRelatedEntity(ent1);
//
//		updateFromSolver();
//	}
//	else {
//		_solver->removeIntVar(newInter->varID());
//
//		delete newInter;
//		newInter = NULL;
//	}
//
//	delete varsIDs;
//	delete varsCoeffs;
//
//	return newInter;
//}


//BoundingRelation*
//CSP::addBoundingRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2)
//{
//	BoundingRelation *newBounding = new BoundingRelation(ent1, ent2);
//
//	return newBounding;
//}

void
CSP::removeTemporalRelation(BinaryTemporalRelation *rel)
{
	for (vector<CSPLinearConstraint*>::iterator it = rel->constraints()->begin() ; it != rel->constraints()->end() ; it++)
		removeConstraint(*it);

	vector<BinaryTemporalRelation*>::iterator it  = _temporalRelations->begin();
	while (it != _temporalRelations->end()) {
		if (*it == rel) {
			_temporalRelations->erase(it);
			break;
		}
		else it++;
	}
}


void
CSP::removeTemporalRelation(unsigned int relationId)
{
	vector<BinaryTemporalRelation*>::iterator it  = _temporalRelations->begin();

	while (it != _temporalRelations->end()) {
		if ((*it)->getId() == relationId) {
			BinaryTemporalRelation* rel = *it;

			for (vector<CSPLinearConstraint*>::iterator it2 = rel->constraints()->begin() ; it2 != rel->constraints()->end() ; it2++) {
				removeConstraint(*it2);
			}

			ConstrainedTemporalEntity *ent1 = (*it)->entity1();
			ConstrainedTemporalEntity *ent2 = (*it)->entity2();

			ent1->removeRelatedEntity(ent2);
			ent2->removeRelatedEntity(ent1);

			_temporalRelations->erase(it);

			break;
		}
		else it++;
	}
}

//void
//CSP::removeBoundingRelation(BoundingRelation *rel)
//{
//	for (vector<CSPLinearConstraint*>::iterator it = rel->constraints()->begin() ; it != rel->constraints()->end() ; it++)
//		removeConstraint(*it);
//}

bool
CSP::performMoving(unsigned int boxesId, int x, int y, vector<unsigned int>& movedBoxes, unsigned int maxModification)
{
	int *varsIDs = new int[3];
	int *values = new int[3];
	int newLength = y - x;
	bool durationLocked = true;

	//ConstrainedBox* box = (ConstrainedBox*)(*_cedEntities)[boxesId];
	ConstrainedBox* box = getBoxById(boxesId);
	int oldLength = box->getLengthVar()->getValue();

	varsIDs[0] = box->getFirstControlPoint()->beginID();
	values[0] = x;

	int maxSceneWidth = box->getFirstControlPoint()->getBeginMax();

	varsIDs[1] = box->getLastControlPoint()->beginID();
	values[1] = y;

	varsIDs[2] = box->lengthID();
	values[2] = newLength;


	// *******************************************************************************************************************************
	// *******************************************************************************************************************************
	//
	// Author : Nicolas Hincker
	//  Pour palier au problème de compression/étirement indésirable de boîtes lors de simples déplacements.
	//
	// *******************************************************************************************************************************

        if(oldLength != newLength){
            //Cas d'un resize :
            //  > on "unlock" la boîte après avoir mis à jour la nouvelle valeur de durée (car pour réouvrir le domaine on doit mettre le min/max à boxLength)
            durationLocked = false;

            // ----------------------- unlocklockDuration ---------------------
            //solver
            _solver->setIntVar(box->getLengthVar()->getID(),10, maxSceneWidth, values[2], (int)LENGTH_VAR_TYPE);

            //CSP
            box->getLengthVar()->setMin(10);
            box->getLengthVar()->setMax(maxSceneWidth);
            //-----------------------------------------------------------------

            //TODO
//            box->unlockDuration();
//          au lieu du bout de code ci-dessus (mais crash pour le moment)

	  }

	// *******************************************************************************************************************************
	// *******************************************************************************************************************************

	for (int i = 0; i < 3 ; ++i) {
		if (values[i] < 1) {
			values[i] = 1;
		} else if (values[i] > maxSceneWidth) {
			values[i] = maxSceneWidth;
		}
	}

	bool validSolution = _solver->suggestValues(varsIDs, values, 3, maxModification);//crash

	delete[] varsIDs;
	delete[] values;

	// -------------------------- lockDuration ------------------------
	if(!durationLocked){

	    //solver
	    _solver->setIntVar(box->getLengthVar()->getID(),newLength, newLength, newLength, (int)LENGTH_VAR_TYPE);

	    //CSP
	    box->getLengthVar()->setMin(newLength);
	    box->getLengthVar()->setMax(newLength);

	    //TODO : box->lockDuration(newLength); au lieu du bout de code ci-dessous
	  }
	   //-----------------------------------------------------------------

	movedBoxes.clear();
	if (validSolution) {
		updateFromSolver(); //TODO: la clef est ici !!!
		map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();
		while (it != _cedEntities->end())
		{
			movedBoxes.push_back(it->first);
			it++;
		}

		return true;
	} else {
		return false;
	}
}

vector<BinaryTemporalRelation*> *
CSP::links() const
{
	return _temporalRelations;
}

vector<BinaryTemporalRelation*> *
CSP::links(ConstrainedTemporalEntity *ent) const
{
	vector<BinaryTemporalRelation*> *links = new vector<BinaryTemporalRelation*>;
	vector<BinaryTemporalRelation*>::iterator it;

	for (it = _temporalRelations->begin() ; it != _temporalRelations->end() ; it++)
		if ((*it)->implicates(ent)) {
			links->push_back(*it);
		}

	return links;
}

ConstrainedBox*
CSP::getBoxById(unsigned int boxId) {
	map<unsigned int, ConstrainedTemporalEntity *>::iterator iter = (*_cedEntities).find(boxId);

	if(iter == (*_cedEntities).end())  {
		throw OutOfBoundException();
	}

	return (ConstrainedBox*)(*_cedEntities)[boxId];
}


int
CSP::getBeginValue(unsigned int boxId)
{
	return getBoxById(boxId)->getFirstControlPoint()->beginValue();
}

int
CSP::getEndValue(unsigned int boxId)
{
	return getBoxById(boxId)->getLastControlPoint()->beginValue();
}

int
CSP::nbControlPoint(unsigned int boxId)
{
	return getBoxById(boxId)->nbControlPoint();
}

int
CSP::getFirstControlPointIndex(unsigned int boxId)
{
	// useful to check if the box exist.
	getBoxById(boxId);

	return BEGIN_CONTROL_POINT_INDEX;
}


int
CSP::getLastControlPointIndex(unsigned int boxId)
{
	// useful to check if the box exist.
	getBoxById(boxId);

	return END_CONTROL_POINT_INDEX;
}

void
CSP::getTriggerPointMap(map<unsigned int, TriggerPoint *>& triggerPoints) {
	triggerPoints = *_triggerPoints;
}

TriggerPoint*
CSP::getTriggerPoint(unsigned int triggerId) {
	map<unsigned int, TriggerPoint *>::iterator iter = (*_triggerPoints).find(triggerId);

	if(iter == (*_triggerPoints).end())  {
		throw OutOfBoundException();
	}

	return (*_triggerPoints)[triggerId];
}

unsigned int
CSP::addTriggerPoint(unsigned int triggerId)
{
	map<unsigned int, TriggerPoint *>::iterator iter = (*_triggerPoints).find(triggerId);

	if(iter != (*_triggerPoints).end())  {
		throw IllegalArgumentException();
	}

	(*_triggerPoints)[triggerId] = new TriggerPoint(triggerId);

	return (triggerId);
}

void
CSP::removeTriggerPoint(unsigned int triggerId)
{
	if (_triggerPoints->find(triggerId) != _triggerPoints->end()) {
		removeTriggerPointRelatedControlPoint(triggerId);
		_triggerPoints->erase(triggerId);
	}
}

bool
CSP::setTriggerPointRelatedControlPoint(unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex)
{
	return(getTriggerPoint(triggerId)->setRelatedControlPoint(getBoxById(boxId)->getControlPoint(controlPointIndex)));
}

void
CSP::removeTriggerPointRelatedControlPoint(unsigned int triggerId)
{
	getTriggerPoint(triggerId)->removeRelatedControlPoint();
}

void
CSP::setTriggerPointMessage(unsigned int triggerId, std::string triggerMessage)
{
	getTriggerPoint(triggerId)->setTriggerMessage(triggerMessage);
}

std::string
CSP::getTriggerPointMessage(unsigned int triggerId)
{
	return getTriggerPoint(triggerId)->getTriggerMessage();
}


void
CSP::getAllBoxesId(vector<unsigned int>& boxesID)
{
	boxesID.clear();

	map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();
	while (it != _cedEntities->end())
	{
		boxesID.push_back(it->first);
		it++;
	}
}

void
CSP::getAllAntPostRelationsId(vector<unsigned int>& relationsID)
{
	relationsID.clear();

	for (unsigned int i = 0 ; i < _temporalRelations->size() ; ++i) {
		if (dynamic_cast<AntPostRelation*>(_temporalRelations->at(i))) {
			relationsID.push_back(_temporalRelations->at(i)->getId());
		}
	}
}

void
CSP::getAllTriggersId(vector<unsigned int>& triggersID)
{
	triggersID.clear();

	map<unsigned int, TriggerPoint*>::iterator it  = _triggerPoints->begin();
	while (it != _triggerPoints->end())
	{
		triggersID.push_back(it->first);
		it++;
	}
}


void
CSP::getAllAntPostRelations(vector<AntPostRelation*>& antPostRelations)
{
	antPostRelations.clear();

	for (unsigned int i = 0 ; i < _temporalRelations->size() ; ++i) {
		if (dynamic_cast<AntPostRelation*>(_temporalRelations->at(i))) {
			antPostRelations.push_back((AntPostRelation*)_temporalRelations->at(i));
		}
	}
}

AntPostRelation*
CSP::getAntPostRelationById(unsigned int relationID)
{
	vector<AntPostRelation*> antPostRelations;

	getAllAntPostRelations(antPostRelations);

	for (unsigned int i = 0; i < antPostRelations.size(); ++i) {
		if (antPostRelations[i]->getId() == relationID) {
			AntPostRelation* transitionToReturn = antPostRelations[i];

			return transitionToReturn;
		}
	}

	return NULL;
}

bool
CSP::areControlPointsInRelation(ControlPoint* cp1, ControlPoint* cp2)
{
	return cp1->isRelatedWith(cp2);
}

unsigned int
CSP::getMaxBoxId()
{
	unsigned int maxID = 0;

	map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();
	while (it != _cedEntities->end())
	{
		if(it->first > maxID) {
			maxID = it->first;
		}
		it++;
	}

	return maxID;
}

unsigned int
CSP::getMaxRelationId()
{
	unsigned int maxID = 0;

	for (unsigned int i = 0 ; i < _temporalRelations->size() ; ++i) {
		if (_temporalRelations->at(i)->getId() > maxID) {
			maxID = _temporalRelations->at(i)->getId();
		}
	}

	return maxID;
}

unsigned int
CSP::getMaxTriggerId()
{
	unsigned int maxID = 0;

	map<unsigned int, TriggerPoint*>::iterator it  = _triggerPoints->begin();

	while (it != _triggerPoints->end())
	{
		if (it->first > maxID) {
			maxID = it->first;
		}
		it++;
	}

	return maxID;
}

void
CSP::store(xmlNodePtr father)
{
	xmlNodePtr boxesNode = NULL;
	xmlNodePtr relationsNode = NULL;
	xmlNodePtr triggerNode = NULL;

	vector<unsigned int> boxesId;
	vector<AntPostRelation*> antPostRelations;

	getAllBoxesId(boxesId);

	boxesNode = xmlNewNode(NULL, BAD_CAST "BOXES");
	xmlAddChild(father, boxesNode);
	for (unsigned int i = 0 ; i < boxesId.size() ; ++i) {
		getBoxById(boxesId[i])->store(boxesNode);
	}

	getAllAntPostRelations(antPostRelations);

	relationsNode = xmlNewNode(NULL, BAD_CAST "RELATIONS");
	xmlAddChild(father, relationsNode);

	for (unsigned int i = 0 ; i < antPostRelations.size() ; ++i) {
		antPostRelations[i]->store(relationsNode);
	}

	triggerNode = xmlNewNode(NULL, BAD_CAST "TRIGGERPOINTS");
	xmlAddChild(father, triggerNode);

	map<unsigned int, TriggerPoint*>::iterator it  = _triggerPoints->begin();
	while (it != _triggerPoints->end())
	{
		it->second->store(triggerNode);
		++it;
	}
}

void
CSP::load(xmlNodePtr root, std::map<unsigned int, CSP*>& boxesMap,
		std::map<unsigned int, CSP*>& relationsMap,
		std::map<unsigned int, CSP*>& triggersMap,
		ConstrainedBox* motherBox)
{
	xmlNodePtr xmlTemporalObject = NULL;

	for (xmlTemporalObject = root->children; xmlTemporalObject != NULL; xmlTemporalObject = xmlTemporalObject->next) {
		if (xmlTemporalObject->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(xmlTemporalObject->name, BAD_CAST "BOXES")) {
				loadBoxes(xmlTemporalObject, boxesMap, relationsMap, triggersMap, motherBox);
			} else if (xmlStrEqual(xmlTemporalObject->name, BAD_CAST "RELATIONS")) {
				loadRelations(xmlTemporalObject, relationsMap);
			} else if (xmlStrEqual(xmlTemporalObject->name, BAD_CAST "TRIGGERPOINTS")) {
				loadTriggerPoints(xmlTemporalObject, triggersMap);
			}
		}
	}
}

void
CSP::loadBoxes(xmlNodePtr root, std::map<unsigned int, CSP*>& boxesMap,
		std::map<unsigned int, CSP*>& relationsMap,
		std::map<unsigned int, CSP*>& triggersMap,
		ConstrainedBox* motherBox)
{
	std::cout << "-> BOXES ";
	std::cout.flush();

	xmlNodePtr xmlBox = NULL;

	for (xmlBox = root->children; xmlBox != NULL; xmlBox = xmlBox->next) {
		if (xmlBox->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(xmlBox->name, BAD_CAST "BOX")) {
				xmlChar* xmlBoxId = xmlGetProp(xmlBox, BAD_CAST "id");
				xmlChar* xmlBoxBegin = xmlGetProp(xmlBox, BAD_CAST "begin");
				xmlChar* xmlBoxLength = xmlGetProp(xmlBox, BAD_CAST "length");
			//	xmlChar* xmlBoxMaxSceneWidth = xmlGetProp(xmlBox, BAD_CAST "maxSceneWidth");
				//xmlChar* xmlBoxProcessId = xmlGetProp(xmlBox, BAD_CAST "processid");

				if ((xmlBoxId != NULL) && (xmlBoxBegin != NULL) && (xmlBoxLength != NULL) /*&& (xmlBoxMaxSceneWidth != NULL)*/) {
					unsigned int m_boxId = XMLConversion::xmlCharToUnsignedInt(xmlBoxId);
					unsigned int boxBegin = XMLConversion::xmlCharToUnsignedInt(xmlBoxBegin);
					unsigned int boxLength = XMLConversion::xmlCharToUnsignedInt(xmlBoxLength);
					//unsigned int maxSceneWidth = XMLConversion::xmlCharToUnsignedInt(xmlBoxMaxSceneWidth);
					if (m_boxId != ROOT_BOX_ID) {
						addBox(m_boxId, boxBegin, boxLength, motherBox->getId(), motherBox->lengthValue());
					}

					std::cout << ".";
					std::cout.flush();

					ConstrainedBox* currentBox = getBoxById(m_boxId);

					if (m_boxId != ROOT_BOX_ID) {
						currentBox->setMother(motherBox);
					}

					xmlNodePtr xmlOptionalArgument = NULL;

					for (xmlOptionalArgument = xmlBox->children; xmlOptionalArgument != NULL; xmlOptionalArgument = xmlOptionalArgument->next) {
						if (xmlOptionalArgument->type == XML_ELEMENT_NODE) {
							if (xmlStrEqual(xmlOptionalArgument->name, BAD_CAST "OPTIONALARGUMENT")) {
								xmlChar* xmlBoxArgKey = xmlGetProp(xmlOptionalArgument, BAD_CAST "key");
								xmlChar* xmlBoxArgValue= xmlGetProp(xmlOptionalArgument, BAD_CAST "value");

								currentBox->setOptionalArgument(XMLConversion::xmlCharToString(xmlBoxArgKey),XMLConversion::xmlCharToString(xmlBoxArgValue));
							}
						}
					}

					boxesMap[m_boxId] = this;

					currentBox->getCSP()->load(xmlBox, boxesMap, relationsMap, triggersMap, currentBox);
					//TODO: quand il y aura plusieurs points de control, penser ˆ les charger !
					// (le 1er et le dernier sont automatiquement crŽŽ avec la boite)
				} else {
					throw InvalidFileFormatException();
				}
			}
		}
	}
	std::cout << std::endl;
}

void
CSP::loadRelations(xmlNodePtr root, std::map<unsigned int, CSP*>& relationsMap)
{
	std::cout << "-> RELATIONS ";
	std::cout.flush();

	xmlNodePtr xmlRelation = NULL;

	for (xmlRelation = root->children; xmlRelation != NULL; xmlRelation = xmlRelation->next) {
		if (xmlRelation->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(xmlRelation->name, BAD_CAST "RELATION")) {
				xmlChar* xmlRelationId = xmlGetProp(xmlRelation, BAD_CAST "id");
				xmlChar* xmlRelationType = xmlGetProp(xmlRelation, BAD_CAST "type");
				xmlChar* xmlRelationFirstBoxId = xmlGetProp(xmlRelation, BAD_CAST "firstBoxId");
				xmlChar* xmlRelationFirstPointId = xmlGetProp(xmlRelation, BAD_CAST "firstPointId");
				xmlChar* xmlRelationSecondBoxId = xmlGetProp(xmlRelation, BAD_CAST "secondBoxId");
				xmlChar* xmlRelationSecondPointId = xmlGetProp(xmlRelation, BAD_CAST "secondPointId");

				xmlChar* xmlRelationMinBound = xmlGetProp(xmlRelation, BAD_CAST "minBound");
				xmlChar* xmlRelationMaxBound = xmlGetProp(xmlRelation, BAD_CAST "maxBound");

				if ((xmlRelationId != NULL) && (xmlRelationType != NULL) && (xmlRelationFirstBoxId != NULL) &&
						(xmlRelationFirstPointId != NULL) && (xmlRelationSecondBoxId != NULL) && (xmlRelationSecondPointId != NULL)) {
					unsigned int relationId = XMLConversion::xmlCharToUnsignedInt(xmlRelationId);
					std::string relationType = XMLConversion::xmlCharToString(xmlRelationType);
					unsigned int relationFirstBoxId = XMLConversion::xmlCharToUnsignedInt(xmlRelationFirstBoxId);
					unsigned int relationFirstControlPointId = XMLConversion::xmlCharToUnsignedInt(xmlRelationFirstPointId);
					unsigned int relationSecondBoxId = XMLConversion::xmlCharToUnsignedInt(xmlRelationSecondBoxId);
					unsigned int relationSecondControlPoinId = XMLConversion::xmlCharToUnsignedInt(xmlRelationSecondPointId);

					int relationMinBound = NO_BOUND;
					int relationMaxBound = NO_BOUND;

					if (xmlRelationMinBound != NULL) {
						relationMinBound = XMLConversion::xmlCharToInt(xmlRelationMinBound);
					}

					if (xmlRelationMaxBound != NULL) {
						relationMaxBound = XMLConversion::xmlCharToInt(xmlRelationMaxBound);
					}

					std::vector<unsigned int> movedBoxes;

					if (relationType == "anteriority") {
						addAntPostRelation(relationId, relationFirstBoxId, relationFirstControlPointId, relationSecondBoxId, relationSecondControlPoinId, ANTPOST_ANTERIORITY, relationMinBound, relationMaxBound, movedBoxes, false);
					} else {
						addAntPostRelation(relationId, relationFirstBoxId, relationFirstControlPointId, relationSecondBoxId, relationSecondControlPoinId, ANTPOST_POSTERIORITY, relationMinBound, relationMaxBound, movedBoxes, false);
					}

					relationsMap[relationId] = this;

					std::cout << ".";
					std::cout.flush();
				} else {
					throw InvalidFileFormatException();
				}
			}
		}
	}
	std::cout << std::endl;
}

void
CSP::loadTriggerPoints(xmlNodePtr root, std::map<unsigned int, CSP*>& triggersMap)
{
	std::cout << "-> TRIGGER POINTS ";
	std::cout.flush();

	xmlNodePtr xmlTrigger = NULL;

	for (xmlTrigger = root->children; xmlTrigger != NULL; xmlTrigger = xmlTrigger->next) {
		if (xmlTrigger->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(xmlTrigger->name, BAD_CAST "TRIGGERPOINT")) {
				xmlChar* xmlTriggerId = xmlGetProp(xmlTrigger, BAD_CAST "id");
				xmlChar* xmlTriggerMessage = xmlGetProp(xmlTrigger, BAD_CAST "message");
				xmlChar* xmlTriggerRelatedBoxId = xmlGetProp(xmlTrigger, BAD_CAST "relatedBoxId");
				xmlChar* xmlTriggerRelatedControlPointId = xmlGetProp(xmlTrigger, BAD_CAST "relatedControlPointId");
				xmlChar* xmlTriggerMuteState = xmlGetProp(xmlTrigger, BAD_CAST "mute");
				xmlChar* xmlTriggerType = xmlGetProp(xmlTrigger, BAD_CAST "type");
				xmlChar* xmlTriggerPreviousId = xmlGetProp(xmlTrigger, BAD_CAST "previousTriggerId");

				if (xmlTriggerId == NULL) {
					throw InvalidFileFormatException();
				}

				unsigned int triggerId = addTriggerPoint(XMLConversion::xmlCharToUnsignedInt(xmlTriggerId));

				if (xmlTriggerMessage != NULL) {
					setTriggerPointMessage(triggerId, XMLConversion::xmlCharToString(xmlTriggerMessage));
				}

				if ((xmlTriggerRelatedBoxId != NULL) && (xmlTriggerRelatedControlPointId != NULL)) {
					setTriggerPointRelatedControlPoint(triggerId,
							XMLConversion::xmlCharToUnsignedInt(xmlTriggerRelatedBoxId),
							XMLConversion::xmlCharToUnsignedInt(xmlTriggerRelatedControlPointId));
				}

				if (xmlTriggerType != NULL) {
					int triggerType = XMLConversion::xmlCharToInt(xmlTriggerType);
					(*_triggerPoints)[triggerId]->setType(triggerType);
				}

				if (xmlTriggerPreviousId != NULL) {
					unsigned int triggerPreviousId = XMLConversion::xmlCharToUnsignedInt(xmlTriggerPreviousId);
					(*_triggerPoints)[triggerId]->setPreviousTriggerId(triggerPreviousId);
				}

				if (xmlTriggerMuteState != NULL) {
					string muteState = XMLConversion::xmlCharToString(xmlTriggerMuteState);

					if (muteState=="true") {
						getTriggerPoint(triggerId)->mute(true);
					} else {
						getTriggerPoint(triggerId)->mute(false);
					}
				}

				triggersMap[triggerId] = this;

				std::cout << ".";
				std::cout.flush();

			}
		}
	}

	std::cout << std::endl;
}

void CSP::changeAllBoxMaxSceneWidth(int newValue)
{
	map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();
	while (it != _cedEntities->end())
	{
		ConstrainedBox* currentBox = (ConstrainedBox*) it->second;

		currentBox->changeMax(_solver, newValue);
		currentBox->getFirstControlPoint()->changeMax(_solver, newValue);
		currentBox->getLastControlPoint()->changeMax(_solver, newValue);

		it++;
	}
}

/*** PRIVATE ***/

CSPLinearConstraint*
CSP::addConstraint(vector<int> *varsIDs, vector<int> *varsCoeffs, BinaryRelationType type, int value, bool mustCallSolver)
{
	int *iDs = new int[varsIDs->size()],  *coeffs = new int[varsIDs->size()];
	for (unsigned int i=0; i<varsIDs->size(); i++) {
		iDs[i] = varsIDs->at(i);
		coeffs[i] = varsCoeffs->at(i);
	}

	CSPLinearConstraint *newConstraint = NULL;
	int newID = _solver->addConstraint(iDs, coeffs, varsIDs->size(), (int)type, value, mustCallSolver);
	if (newID >= 0)
		newConstraint = new CSPLinearConstraint(newID, varsIDs, varsCoeffs, type, value);

	delete[] iDs;
	delete[] coeffs;

	return newConstraint;
}

bool
CSP::removeConstraint(CSPLinearConstraint *cst)
{
	return _solver->removeConstraint(cst->getID());
}

void
CSP::updateFromSolver()
{
	map<unsigned int, ConstrainedTemporalEntity*>::iterator it  = _cedEntities->begin();

	while (it != _cedEntities->end())
	{
		ConstrainedBox* constrainedBox = (ConstrainedBox*) it->second;

		vector<CSPConstrainedVariable*> *vars = constrainedBox->variables();
		for (unsigned int v = 0; v<vars->size(); v++) {
			vars->at(v)->setValue(_solver->getVariableValue(vars->at(v)->getID()));
		}

		vector<unsigned int>* controlPointID = new vector <unsigned int>;
		constrainedBox->getAllControlPointsId(controlPointID);

		for (unsigned int j = 0 ; j < controlPointID->size() ; ++j)
		{
			ControlPoint* controlPoint = constrainedBox->getControlPoint(controlPointID->at(j));

			vector<CSPConstrainedVariable*> *varsControl = controlPoint->variables();

			for (unsigned int v = 0; v<varsControl->size(); v++) {
				varsControl->at(v)->setValue(_solver->getVariableValue(varsControl->at(v)->getID()));

			}
		}
		it++;
		delete vars;
	}

//	for (unsigned int i=0; i<_temporalRelations->size(); i++) {
//		if (dynamic_cast<IntervalRelation*>(_temporalRelations->at(i))) {
//			_solver->getVariableValue( dynamic_cast<IntervalRelation*>(_temporalRelations->at(i))->varID());
//			_solver->getVariableValue(_temporalRelations->at(i)->entity2()->beginID());
//			_solver->getVariableValue(_temporalRelations->at(i)->entity1()->beginID());
//			_solver->getVariableValue(_temporalRelations->at(i)->entity1()->lengthID());
//		}
//	}
}

AllenRelation*
CSP::addAllenRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2, AllenType type, bool mustCallSolver)
{
	AllenRelation *newAllen = new AllenRelation(ent1, ent2, type);

	vector<int> *varsIDs = new vector<int>;
	vector<int> *varsCoeffs = new vector<int>;


	switch(type) {
	case ALLEN_BEFORE :

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LE_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_AFTER :

		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LE_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_EQUALS :

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, mustCallSolver) );

		varsIDs->clear();
		varsCoeffs->clear();

		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->lengthID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_MEETS :

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_OVERLAPS :

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LE_RELATION, 0, mustCallSolver) );

		varsIDs->clear();
		varsCoeffs->clear();

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GR_RELATION, 0, mustCallSolver) );

		varsIDs->clear();
		varsCoeffs->clear();

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		varsIDs->push_back(ent2->lengthID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LE_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_DURING :

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, GQ_RELATION, 0, mustCallSolver) );

		varsIDs->clear();
		varsCoeffs->clear();

		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);
		varsIDs->push_back(ent2->lengthID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, LQ_RELATION, 0, mustCallSolver) );

		break;

	case ALLEN_STARTS :
		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(-1);

		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, mustCallSolver) );
		break;

	case ALLEN_FINISHES :

		varsIDs->push_back(ent2->beginID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent2->lengthID());
		varsCoeffs->push_back(1);
		varsIDs->push_back(ent1->beginID());
		varsCoeffs->push_back(-1);
		varsIDs->push_back(ent1->lengthID());
		varsCoeffs->push_back(-1);
		newAllen->addLinearConstraint( addConstraint(varsIDs, varsCoeffs, EQ_RELATION, 0, mustCallSolver) );

		break;

	default : break;
	}

	if (mustCallSolver) {
		if (newAllen->validate()) {
			_temporalRelations->push_back(newAllen);

			ent1->addRelatedEntity(ent2);
			ent2->addRelatedEntity(ent1);

			updateFromSolver();
		}
		else {
			for (vector<CSPLinearConstraint*>::iterator it = newAllen->constraints()->begin() ; it != newAllen->constraints()->end() ; it++) {
				if ( *it )
					removeConstraint(*it);
			}

			delete newAllen;
			newAllen = NULL;
		}

		delete varsIDs;
		delete varsCoeffs;

		return newAllen;
	} else {
		delete varsIDs;
		delete varsCoeffs;

		_temporalRelations->push_back(newAllen);

		ent1->addRelatedEntity(ent2);
		ent2->addRelatedEntity(ent1);

		return newAllen;
	}
}

Solver *
CSP::getSolver(){
  return _solver;
}


