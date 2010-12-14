/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak
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
 * \file Editor.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "Editor.hpp"

Editor::Editor(unsigned int maxSceneWidth)
{
	m_mainCSP = new CSP();
	m_nextBoxId = ROOT_BOX_ID;
	m_nextRelationId = 1;
	m_nextTriggerId = 1;

	addBox(0, maxSceneWidth, NO_ID);
}

Editor::~Editor()
{
	delete m_mainCSP;
}

string Editor::getNameFromRelation(TemporalRelationType t)
{
	return m_mainCSP->getNameFromRelation(t);
}

int Editor::scenarioSize()
{
	return getBoxById(ROOT_BOX_ID)->getBeginMax();
}

unsigned int Editor::addBox(int boxBeginPos, int boxLength, unsigned int motherId)
{
	if (motherId == NO_ID) {
		if (m_boxIdToContainingCSP.find(ROOT_BOX_ID) == m_boxIdToContainingCSP.end()) {
			m_mainCSP->addBox(m_nextBoxId, boxBeginPos, boxLength, motherId, boxLength);
			m_boxIdToContainingCSP[m_nextBoxId] = m_mainCSP;
		}
		else
		{
			return NO_ID;
		}
	} else {
		ConstrainedBox* motherBox = getBoxById(motherId);

		if ((boxBeginPos < motherBox->lengthValue()) && (boxBeginPos + boxLength) < motherBox->lengthValue()) {
			CSP* motherCSP = motherBox->getCSP();

			motherCSP->addBox(m_nextBoxId, boxBeginPos, boxLength, motherId, motherBox->lengthValue());
			m_boxIdToContainingCSP[m_nextBoxId] = motherCSP;

			motherCSP->getBoxById(m_nextBoxId)->setMother(motherBox);

			if (motherId != ROOT_BOX_ID) {
				if (motherBox->getHierarchyRelationId() == NO_ID) {
					std::vector<unsigned int> movedBox;

					unsigned int hierarchyRelation = addAntPostRelation(motherId, BEGIN_CONTROL_POINT_INDEX,
																		motherId, END_CONTROL_POINT_INDEX,
																		ANTPOST_ANTERIORITY,
																		motherBox->lengthValue(), motherBox->lengthValue(),
																		movedBox);

					motherBox->setHierarchyInformations(hierarchyRelation);
				}
			}
		} else {
			return NO_ID;
		}
	}

	++m_nextBoxId;

	return (m_nextBoxId - 1);
}

void Editor::removeBox(unsigned int boxId)
{
	if (m_boxIdToContainingCSP.find(boxId) != m_boxIdToContainingCSP.end()) {
		CSP* containingCSP = m_boxIdToContainingCSP[boxId];
		ConstrainedBox* currentBox = containingCSP->getBoxById(boxId);
		ConstrainedBox* mother = currentBox->mother();
		std::vector<unsigned int> boxesId;
		std::vector<unsigned int> relationId;
		std::vector<unsigned int> triggerId;


		CSP* boxCSP = currentBox->getCSP();

		if (boxCSP != NULL) {
			boxCSP->getAllBoxesId(boxesId);

			for (unsigned int i = 0; i < boxesId.size(); ++i) {
				removeBox(boxesId[i]);
			}
		}

		containingCSP->getAllBoxesId(boxesId);

		if (boxesId.size() == 0) {
			unsigned int motherHierarchyRelationId = mother->getHierarchyRelationId();

			if (motherHierarchyRelationId != NO_ID) {
				if (m_boxIdToContainingCSP.find(mother->getId()) != m_boxIdToContainingCSP.end()) {
					CSP* motherCSP = m_boxIdToContainingCSP[mother->getId()];
					motherCSP->removeTemporalRelation(motherHierarchyRelationId);
				}
			}
		}

		containingCSP->removeBox(boxId, relationId, triggerId);

		for (unsigned int i = 0; i < relationId.size(); ++i) {
			removeTemporalRelation(relationId[i]);
		}

		for (unsigned int i = 0; i < triggerId.size(); ++i) {
			removeTriggerPoint(triggerId[i]);
		}

		m_boxIdToContainingCSP.erase(boxId);
	}
}

unsigned int Editor::addAntPostRelation(unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, int minBound, int maxBound, vector<unsigned int>& movedBoxes)
{
	CSP* containingCSP1 = m_boxIdToContainingCSP[boxId1];
	CSP* containingCSP2 = m_boxIdToContainingCSP[boxId2];

	if (containingCSP1 != containingCSP2) {
		return NO_ID;
	}

	if (isAntPostRelationAlreadyExist(boxId1, controlPoint1, boxId2, controlPoint2)) {
		return NO_ID;
	} else {

		containingCSP1->addAntPostRelation(m_nextRelationId, boxId1, controlPoint1, boxId2, controlPoint2, type, minBound, maxBound, movedBoxes);
		m_relationIdToContainingCSP[m_nextRelationId]=containingCSP1;

		++m_nextRelationId;

		return (m_nextRelationId - 1);
	}
}

void Editor::changeAntPostRelationBounds(unsigned int relationId, int minBound, int maxBound, vector<unsigned int>& movedBoxes)
{
    unsigned int box1Id = getRelationFirstBoxId(relationId);
	unsigned int cp1 = getRelationFirstControlPointIndex(relationId);
	unsigned int box2Id = getRelationSecondBoxId(relationId);
	unsigned int cp2 = getRelationSecondControlPointIndex(relationId);

	if (m_relationIdToContainingCSP.find(relationId) != m_relationIdToContainingCSP.end()) {
		CSP* containingCSP = m_relationIdToContainingCSP[relationId];

		AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

		if (relation != NULL) {
			TemporalRelationType type = containingCSP->getAntPostRelationById(relationId)->antPostType();

			removeTemporalRelation(relationId, false);

			containingCSP->addAntPostRelation(relationId, box1Id, cp1, box2Id, cp2, type, minBound, maxBound, movedBoxes);
		}
	}
}

void Editor::removeTemporalRelation(unsigned int relationId, bool removeFromCSPMap)
{
	if (m_relationIdToContainingCSP.find(relationId) != m_relationIdToContainingCSP.end()) {
		CSP* containingCSP = m_relationIdToContainingCSP[relationId];

		containingCSP->removeTemporalRelation(relationId);

		if (removeFromCSPMap) {
			m_relationIdToContainingCSP.erase(relationId);
		}
	}
}

bool Editor::isAntPostRelationAlreadyExist(unsigned int boxId1, unsigned int controlPoint1Index, unsigned int boxId2, unsigned int controlPoint2Index)
{
	CSP* containingCSP1 = m_boxIdToContainingCSP[boxId1];
	CSP* containingCSP2 = m_boxIdToContainingCSP[boxId2];

	if (containingCSP1 != containingCSP2) {
		return false;
	}

	ControlPoint* cp1 = containingCSP1->getBoxById(boxId1)->getControlPoint(controlPoint1Index);
	ControlPoint* cp2 = containingCSP2->getBoxById(boxId2)->getControlPoint(controlPoint2Index);

	return containingCSP1->areControlPointsInRelation(cp1, cp2);
}

unsigned int Editor::getRelationFirstBoxId(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_ID;
	}

	ControlPoint* currentControlPoint = (ControlPoint*) relation->entity1();

	return currentControlPoint->getContainingBoxId();
}

unsigned int Editor::getRelationFirstControlPointIndex(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_ID;
	}

	ControlPoint* currentControlPoint = (ControlPoint*) relation->entity1();

	return currentControlPoint->getId();
}

unsigned int Editor::getRelationSecondBoxId(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_ID;
	}

	ControlPoint* currentControlPoint = (ControlPoint*) relation->entity2();

		return currentControlPoint->getContainingBoxId();
}

unsigned int Editor::getRelationSecondControlPointIndex(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_ID;
	}

	ControlPoint* currentControlPoint = (ControlPoint*) relation->entity2();

	return currentControlPoint->getId();
}

int Editor::getRelationMinBound(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_BOUND;
	}

	return relation->minBound();
}

int Editor::getRelationMaxBound(unsigned int relationId)
{
	CSP* containingCSP = m_relationIdToContainingCSP[relationId];
	AntPostRelation* relation = containingCSP->getAntPostRelationById(relationId);

	if (relation == NULL) {
		return NO_BOUND;
	}

	return relation->maxBound();
}

bool Editor::performMoving(unsigned int boxId, int x, int y, vector<unsigned int>& movedBoxes, int maxModification)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];
	ConstrainedBox* currentBox = containingCSP->getBoxById(boxId);

	ConstrainedBox* motherBox = currentBox->mother();

//	if ((x > motherBox->lengthValue()) || (y > motherBox->lengthValue())) {
//		return false;
//	}

	containingCSP->changeAllBoxMaxSceneWidth(motherBox->lengthValue());

	CSP* boxCSP = currentBox->getCSP();

	int previousX = getBeginValue(boxId);
	int previousY = getEndValue(boxId);

	if (boxCSP != NULL) {
		std::vector<unsigned int> boxesId;
		boxCSP->getAllBoxesId(boxesId);

		if (boxesId.size() != 0) {
			unsigned int maxValue = 0;

			for (unsigned int i = 0; i < boxesId.size(); ++i) {
				unsigned int currentChildBoxEnd = boxCSP->getEndValue(boxesId[i]);

				if(currentChildBoxEnd > maxValue) {
					maxValue = currentChildBoxEnd;
				}
			}

			if ((unsigned int)(y - x) < maxValue) {
				return false;
			}
		}
	}


	int modificationX = abs(previousX - x);
	int modificationY = abs(previousY - y);

	if (maxModification < modificationX) {
		maxModification = modificationX;
	}

	if (maxModification < modificationY) {
		maxModification = modificationY;
	}


	unsigned int hierarchyRelation = currentBox->getHierarchyRelationId();

	if (hierarchyRelation != NO_ID) {
		containingCSP->removeTemporalRelation(hierarchyRelation);
	}

	std::vector<unsigned int> hierarchyRelationMovedBoxes;

	if(containingCSP->performMoving(boxId, x, y, movedBoxes, maxModification)) {
		if (hierarchyRelation != NO_ID) {
			containingCSP->addAntPostRelation(hierarchyRelation,
											  boxId, BEGIN_CONTROL_POINT_INDEX,
											  boxId, END_CONTROL_POINT_INDEX,
											  ANTPOST_ANTERIORITY,
											  currentBox->lengthValue(),
											  currentBox->lengthValue(),
											  hierarchyRelationMovedBoxes,
											  false);
		}
		movedBoxes.clear();
		getAllBoxesId(movedBoxes);
		return true;
	} else {
		// put back the CSP into its previous state.
		containingCSP->performMoving(boxId, previousX, previousY, movedBoxes, maxModification);
		movedBoxes.clear();
		getAllBoxesId(movedBoxes);
		return false;
	}
}

ConstrainedBox* Editor::getBoxById(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->getBoxById(boxId);
}

int Editor::getBeginValue(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->getBeginValue(boxId);
}

int Editor::getEndValue(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->getEndValue(boxId);
}

int Editor::nbControlPoint(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->nbControlPoint(boxId);
}

int Editor::getFirstControlPointIndex(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->getFirstControlPointIndex(boxId);
}

int Editor::getLastControlPointIndex(unsigned int boxId)
{
	CSP* containingCSP = m_boxIdToContainingCSP[boxId];

	return containingCSP->getLastControlPointIndex(boxId);
}

void Editor::setOptionalArgument(unsigned int boxId, std::string key, std::string value)
{
	ConstrainedBox* currentBox = getBoxById(boxId);
	currentBox->setOptionalArgument(key, value);
}

void Editor::removeOptionalArgument(unsigned int boxId, std::string key)
{
	ConstrainedBox* currentBox = getBoxById(boxId);
	currentBox->removeOptionalArgument(key);
}

std::map<std::string, std::string> Editor::getOptionalArguments(unsigned int boxId)
{
	ConstrainedBox* currentBox = getBoxById(boxId);
	return currentBox->getOptionalArguments();
}

unsigned int
Editor::addTriggerPoint(unsigned int containingBoxId)
{
	CSP* containingCSP = getBoxById(containingBoxId)->getCSP();

	containingCSP->addTriggerPoint(m_nextTriggerId);
	m_triggerIdToContainingCSP[m_nextTriggerId] = containingCSP;

	++m_nextTriggerId;

	return (m_nextTriggerId - 1);
}

void
Editor::removeTriggerPoint(unsigned int triggerId)
{
	if (m_triggerIdToContainingCSP.find(triggerId) != m_triggerIdToContainingCSP.end()) {
		CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

		containingCSP->removeTriggerPoint(triggerId);
		m_triggerIdToContainingCSP.erase(triggerId);
	}
}

bool
Editor::setTriggerPointRelatedControlPoint(unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	if (containingCSP != m_boxIdToContainingCSP[boxId]) {
		throw new IllegalArgumentException();
	}

	return containingCSP->setTriggerPointRelatedControlPoint(triggerId, boxId, controlPointIndex);
}

void
Editor::removeTriggerPointRelatedControlPoint(unsigned int triggerId)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	containingCSP->removeTriggerPointRelatedControlPoint(triggerId);
}

void
Editor::setTriggerPointMessage(unsigned int triggerId, std::string triggerMessage)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	containingCSP->setTriggerPointMessage(triggerId, triggerMessage);
}

std::string
Editor::getTriggerPointMessage(unsigned int triggerId)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	return containingCSP->getTriggerPointMessage(triggerId);
}

unsigned int Editor::getTriggerPointRelatedBoxId(unsigned int triggerId)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	ControlPoint* cp = containingCSP->getTriggerPoint(triggerId)->getRelatedControlPoint();

	if (cp != NULL) {
		return containingCSP->getTriggerPoint(triggerId)->getRelatedControlPoint()->getContainingBoxId();
	} else {
		return NO_ID;
	}

}

unsigned int Editor::getTriggerPointRelatedControlPointIndex(unsigned int triggerId)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	ControlPoint* cp = containingCSP->getTriggerPoint(triggerId)->getRelatedControlPoint();

	if (cp != NULL) {
		return containingCSP->getTriggerPoint(triggerId)->getRelatedControlPoint()->getId();
	} else {
		return NO_ID;
	}

}

void Editor::setTriggerPointMuteState(unsigned int triggerId, bool muteState)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	containingCSP->getTriggerPoint(triggerId)->mute(muteState);
}

bool Editor::getTriggerPointMuteState(unsigned int triggerId)
{
	CSP* containingCSP = m_triggerIdToContainingCSP[triggerId];

	return containingCSP->getTriggerPoint(triggerId)->isMute();
}

void Editor::getAllBoxesId(vector<unsigned int>& boxesID)
{
	std::map<unsigned int, CSP*>::iterator it = m_boxIdToContainingCSP.begin();

	while (it != m_boxIdToContainingCSP.end()) {
		if (it->first != ROOT_BOX_ID) {
			boxesID.push_back(it->first);
		}
		it ++;
	}


//	m_mainCSP->getAllBoxesId(boxesID);
}

void Editor::getAllAntPostRelationsId(vector<unsigned int>& relationsID)
{
	std::map<unsigned int, CSP*>::iterator it = m_relationIdToContainingCSP.begin();

	while (it != m_relationIdToContainingCSP.end()) {
		relationsID.push_back(it->first);

		it++;
	}
	//m_mainCSP->getAllAntPostRelationsId(relationsID);
}

void Editor::getAllTriggersId(vector<unsigned int>& triggersID)
{
	std::map<unsigned int, CSP*>::iterator it = m_triggerIdToContainingCSP.begin();

	while (it != m_triggerIdToContainingCSP.end()) {
		triggersID.push_back(it->first);

		it++;
	}

	//m_mainCSP->getAllTriggersId(triggersID);
}

void
Editor::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;

	node = xmlNewNode(NULL, BAD_CAST "EDITOR");
	xmlAddChild(father, node);

	m_mainCSP->store(node);
}

void
Editor::load(xmlNodePtr root)
{
	m_mainCSP->load(root, m_boxIdToContainingCSP, m_relationIdToContainingCSP, m_triggerIdToContainingCSP, getBoxById(ROOT_BOX_ID));

	std::vector<unsigned int> boxesId;
	std::vector<unsigned int> relationsId;
	std::vector<unsigned int> triggersId;

	getAllBoxesId(boxesId);
	getAllAntPostRelationsId(relationsId);
	getAllTriggersId(triggersId);

	std::vector<unsigned int>::iterator maxBoxId = std::max_element(boxesId.begin(), boxesId.end());
	std::vector<unsigned int>::iterator maxRelationId = std::max_element(relationsId.begin(), relationsId.end());
	std::vector<unsigned int>::iterator maxTriggerId = std::max_element(triggersId.begin(), triggersId.end());

	if (maxBoxId != boxesId.end()) {
		m_nextBoxId = *maxBoxId + 1;
	}

	if (maxRelationId != relationsId.end()) {
		m_nextRelationId = *maxRelationId + 1;
	}

	if (maxTriggerId != triggersId.end()) {
		m_nextTriggerId = *maxTriggerId + 1;
	}
}

void Editor::displayMapContent()
{
	std::map<unsigned int, CSP*>::iterator itBox = m_boxIdToContainingCSP.begin();
	std::map<unsigned int, CSP*>::iterator itRelation = m_relationIdToContainingCSP.begin();
	std::map<unsigned int, CSP*>::iterator itTrigger = m_triggerIdToContainingCSP.begin();

	std::cout << "BOX <=> CSP" << std::endl;
	while (itBox != m_boxIdToContainingCSP.end()) {
		std::cout << itBox->first << " " << itBox->second << std::endl;
		itBox++;
	}

	std::cout << "RELATION <=> CSP" << std::endl;
	while (itRelation != m_relationIdToContainingCSP.end()) {
		std::cout << itRelation->first << " " << itRelation->second << std::endl;
		itRelation++;
	}

	std::cout << "TRIGGER <=> CSP" << std::endl;
	while (itTrigger != m_triggerIdToContainingCSP.end()) {
		std::cout << itTrigger->first << " " << itTrigger->second << std::endl;
		itTrigger++;
	}
}


CSP*
Editor::getCSP()
{
	//return m_mainCSP;
	return getBoxById(ROOT_BOX_ID)->getCSP();
}
