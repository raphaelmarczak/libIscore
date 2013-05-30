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
 * \file ConstrainedBox.cpp
 * \author Bruno Valeze (LaBRI) and Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "ConstrainedBox.hpp"
#include "ConstrainedTemporalEntity.hpp"
#include "CSP.hpp"
#include "CSPConstrainedVariable.hpp"

ConstrainedBox::ConstrainedBox(CSPConstrainedVariable *beg,  CSPConstrainedVariable *length)
: ConstrainedTemporalEntity(beg, length)
{
	m_childrenBoxes = new std::vector<ConstrainedBox*>;
	m_controlPoints = new std::map<unsigned int, ControlPoint*>;
	m_motherBox = NULL;

	m_csp = new CSP();

	m_flexiblityRelationId = NO_ID;
	m_minBound = NO_BOUND;
	m_maxBound = NO_BOUND;

	m_hierarchyRelationId = NO_ID;


}

void
ConstrainedBox::addChild(ConstrainedBox *box)
{
	m_childrenBoxes->push_back(box);
}

void
ConstrainedBox::removeChild(ConstrainedBox *box)
{
	vector<ConstrainedBox*>::iterator it;
	for (it = m_childrenBoxes->begin() ; it != m_childrenBoxes->end() ; it++)
		if (*it == box) {
			m_childrenBoxes->erase(it) ;
			return;
		}
}

ConstrainedBox*
ConstrainedBox::mother() const
{
	return m_motherBox;
}

//BoundingRelation*
//ConstrainedBox::boundingRelation() const
//{
//	return m_boundingRel;
//}

vector<ConstrainedBox*> *
ConstrainedBox::children() const
{
	return m_childrenBoxes;
}

void
ConstrainedBox::setMother(ConstrainedBox *box)
{
	m_motherBox = box;
}

bool
ConstrainedBox::isDescendantOf(ConstrainedBox *box)
{
	if (m_motherBox == NULL)
		return false;
	else if (box == m_motherBox)
		return true;
	else
		return m_motherBox->isDescendantOf(box);
}

bool
ConstrainedBox::isAncestorOf(ConstrainedBox *box)
{
	return box->isDescendantOf(this);
}

vector<ConstrainedBox*> *
ConstrainedBox::descendants() const
{
	vector<ConstrainedBox*> *desc = new vector<ConstrainedBox*>;

	for (vector<ConstrainedBox*>::iterator it = m_childrenBoxes->begin() ; it != m_childrenBoxes->end() ; it++) {
		desc->push_back(*it);
		vector<ConstrainedBox*> *desc2 = (*it)->descendants();
		for (vector<ConstrainedBox*>::iterator it2 = desc2->begin() ; it2 != desc2->end() ; it2++)
			desc->push_back(*it2);
	}

	return desc;
}

void
ConstrainedBox::addControlPoint(ControlPoint* controlPoint, unsigned int index)
{
	(*m_controlPoints)[index] = controlPoint;
	controlPoint->setId(index);
}

int
ConstrainedBox::nbControlPoint()
{
	return m_controlPoints->size();
}

ControlPoint*
ConstrainedBox::getControlPoint(unsigned int i)
{
	return (*m_controlPoints)[i];
}

ControlPoint*
ConstrainedBox::getFirstControlPoint()
{
	return (*m_controlPoints)[BEGIN_CONTROL_POINT_INDEX];
}

ControlPoint*
ConstrainedBox::getLastControlPoint()
{
	return (*m_controlPoints)[END_CONTROL_POINT_INDEX];
}

void
ConstrainedBox::getAllControlPointsId(vector<unsigned int>* controlPointID)
{
	controlPointID->clear();

	std::map<unsigned int, ControlPoint*>::iterator it  = m_controlPoints->begin();
	while (it != m_controlPoints->end())
	{
		controlPointID->push_back(it->first);
		it++;
	}
}

int
ConstrainedBox::minBound() const
{
	return m_minBound;
}

int
ConstrainedBox::maxBound() const
{
	return m_maxBound;
}

CSP* ConstrainedBox::getCSP() const
{
	return m_csp;
}

unsigned int
ConstrainedBox::getFlexibilityRelationId()
{
	return m_flexiblityRelationId;
}

unsigned int
ConstrainedBox::getHierarchyRelationId()
{
	return m_hierarchyRelationId;
}


void
ConstrainedBox::setFlexibilityInformations(unsigned int flexibilityRelationId, int minBound, int maxBound)
{
	m_flexiblityRelationId = flexibilityRelationId;

	m_minBound = minBound;
	m_maxBound = maxBound;
}

void
ConstrainedBox::setHierarchyInformations(unsigned int hierarchyRelationId)
{
	m_hierarchyRelationId = hierarchyRelationId;
}

std::ostream &operator<<(std::ostream &stream, ConstrainedBox& ob)
{
	stream << "ID : " << ob._id << std::endl;
	stream << "Nb control points : " << ob.nbControlPoint() << std::endl;
	stream << "Dates : " << ob.getFirstControlPoint()->beginValue() << " - " << ob.getLastControlPoint()->beginValue() << std::endl;

	return stream;
}

void ConstrainedBox::setOptionalArgument(std::string key, std::string value)
{
	m_optionalArguments[key] = value;
}

void ConstrainedBox::removeOptionalArgument(std::string key)
{
	std::map<std::string, std::string>::iterator it = m_optionalArguments.find(key);

	if (it != m_optionalArguments.end()) {
		m_optionalArguments.erase(it);
	}
}

std::map<std::string, std::string> ConstrainedBox::getOptionalArguments()
{
	return m_optionalArguments;
}

void
ConstrainedBox::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;
	xmlNodePtr controlPointsNode = NULL;
	xmlNodePtr optionalArgumentNode = NULL;

	std::ostringstream oss;

	node = xmlNewNode(NULL, BAD_CAST "BOX");

	oss << getId();
	xmlSetProp(node, BAD_CAST "id", BAD_CAST oss.str().data());

	oss.str("");
	oss << beginValue();

	xmlSetProp(node, BAD_CAST "begin", BAD_CAST oss.str().data());

	oss.str("");
	oss << lengthValue();

	xmlSetProp(node, BAD_CAST "length", BAD_CAST oss.str().data());

	//oss.str("");
	//oss << getBeginMax();

	//xmlSetProp(node, BAD_CAST "maxSceneWidth", BAD_CAST oss.str().data());

	oss.str("");
	oss << getId();
	xmlSetProp(node, BAD_CAST "processid", BAD_CAST oss.str().data());

	m_csp->store(node);

	xmlAddChild(father, node);

	controlPointsNode = xmlNewNode(NULL, BAD_CAST "CONTROLPOINTS");
	xmlAddChild(node, controlPointsNode);

	vector<unsigned int>* controlPointID = new vector <unsigned int>;
	getAllControlPointsId(controlPointID);

	for (unsigned int i = 0 ; i < controlPointID->size() ; ++i) {
		getControlPoint(controlPointID->at(i))->store(controlPointsNode);
	}

	std::map<std::string, std::string>::iterator it = m_optionalArguments.begin();

	while (it != m_optionalArguments.end()) {
		optionalArgumentNode = xmlNewNode(NULL, BAD_CAST "OPTIONALARGUMENT");
		xmlSetProp(optionalArgumentNode, BAD_CAST "key", BAD_CAST it->first.data());
		xmlSetProp(optionalArgumentNode, BAD_CAST "value", BAD_CAST it->second.data());
		xmlAddChild(node, optionalArgumentNode);
		++it;
	}
}

void
ConstrainedBox::lockDuration(){
  //Pour empêcher toute modification de durée de la boîte :
  //On réduit le domaine de la variable contrainte length à [value,value] (la même valeur) ainsi la durée est inmodifiable.

  int maxSceneWidth = getFirstControlPoint()->getBeginMax();

  //Solver
  getCSP()->getSolver()->setIntVar(getLengthVar()->getID(),lengthValue(), lengthValue(), lengthValue(), (int)LENGTH_VAR_TYPE);

  //CSP
  getLengthVar()->setID(getCSP()->getSolver()->addIntVar(10, maxSceneWidth, lengthValue(), (int)LENGTH_VAR_TYPE));
  getLengthVar()->setMin(lengthValue());
  getLengthVar()->setMax(lengthValue());
}

void
ConstrainedBox::unlockDuration(){
  //Le domaine de la variable constrainte length est alors ouvert.
  //A l'inverse de la fonction précédente, on réouvre ce domaine pour autoriser la modification de durée.

  int maxSceneWidth = getFirstControlPoint()->getBeginMax();

  //Solver
  getCSP()->getSolver()->setIntVar(getLengthVar()->getID(),10, maxSceneWidth, lengthValue(), (int)LENGTH_VAR_TYPE);

  //CSP
//  getLengthVar()->setID(getCSP()->getSolver()->addIntVar(10, maxSceneWidth, lengthValue(), (int)LENGTH_VAR_TYPE));
  getLengthVar()->setMin(10);
  getLengthVar()->setMax(maxSceneWidth);
}
