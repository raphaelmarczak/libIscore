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

#ifndef EDITOR_HPP_
#define EDITOR_HPP_

/*!
 * \file Editor.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 *
 * \brief This file contains the editor (all functions to communicate with the CSP).
 */

#include "CSP/CSP.hpp"

#include "CSP/XMLInclude.hpp"

/*!
 * \class Editor
 *
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 *
 * \brief Class which contains all functions for edition.
 *
 * This class contains all functions to communicate with the CSP.
 */
class Editor
{
public:
	/*!
	 * Editor constructor.
	 */
	Editor(unsigned int maxSceneWidth);

	/*!
	 * Editor destructor.
	 */
	virtual ~Editor();

	/*!
	 * Gets the name of an AntPostRelation according to its type.
	 *
	 * \param t : the type of the AntPostRelation
	 *
	 * \return the name as a string
	 */
	string getNameFromRelation(TemporalRelationType t);

	int scenarioSize();
	/*!
	 * Adds a new box in the CSP.
	 *
	 * \param boxBeginPos : the begin value in ms
	 * \param boxLength : the length value in ms
	 * \param motherId : mother box ID if any, NO_ID if the box to create has no mother
	 * \param maxSceneWidth : the max scene width
	 *
	 * \return the newly created box ID
	 */
	unsigned int addBox(int boxBeginPos, int boxLength, unsigned int motherId);

	/*!
	 * Removes a box from the CSP : removes the relation implicating it and the
	 * box's variables.
	 *
	 * \param boxId : the ID of the box to remove
	 */
	void removeBox(unsigned int boxId);

	/*!
	 * Adds a AntPostRelation between two controlPoints.
	 *
	 * \param boxId1 : the ID of the first box
	 * \param controlPoint1 : the index of the point in the first box to put in relation
	 * \param boxId2 : the ID of the second box
	 * \param controlPoint2 : the index of the point in the second box to put in relation
	 * \param type : the relation type
	 * \param minBound : the min bound for the relation length in ms.
	 * \param maxBound : the max bound for the relation length in ms. NO_BOUND if the max bound is not used (+infinity).
	 * \param movedBoxes : empty vector, will be filled with the ID of the boxes moved by this new relation
	 *
	 * \return the newly created relation id (NO_ID if the creation is impossible).
	 */
	unsigned int addAntPostRelation(unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, int minBound, int maxBound, vector<unsigned int>* movedBoxes);

	/*!
	 * Changes min bound and max bound for the relation length.
	 *
	 * \param relationId : the relation to change the bounds.
	 * \param minBound : the min bound for the box relation in ms.
	 * \param maxBound : the max bound for the box relation in ms. NO_BOUND if the max bound is not used (+infinity).
	 * \param movedBoxes : empty vector, will be filled with the ID of the boxes moved by this new relation.
	 */
	void changeAntPostRelationBounds(unsigned int relationId, int minBound, int maxBound, vector<unsigned int>* movedBoxes = NULL);

	/*!
	 * Checks if a relation exists between the two given control points.
	 *
	 * \param boxId1 : the id of the first box.
	 * \param controlPoint1Index : the index of the control point in the first box.
	 * \param boxId2 : the id of the second box.
	 * \param controlPoint2Index : the index of the control point in the second box.
	 *
	 * \return true if a relation exists between the two given control points.
	 */
	bool isAntPostRelationAlreadyExist(unsigned int boxId1, unsigned int controlPoint1Index, unsigned int boxId2, unsigned int controlPoint2Index);

	/*!
	 * Removes the temporal relation using given id.
	 *
	 * \param relationId : the ID of the relation to remove
	 */
	void removeTemporalRelation(unsigned int relationId);

	/*!
	 * Gets the Id of the first box linked with the given relation id.
	 *
	 * \param relationId : the id of the relation.
	 *
	 * \return the id of the first box.
	 */
	unsigned int getRelationFirstBoxId(unsigned int relationId);

	/*!
	 * Gets the index of the control point linked with the given relation id in the first box.
	 *
	 * \param relationId : the id of the relation.
	 *
	 * \return the index of the control point.
	 */
	unsigned int getRelationFirstControlPointIndex(unsigned int relationId);

	/*!
	 * Gets the Id of the second box linked with the given relation id.
	 *
	 * \param relationId : the id of the relation.
	 *
	 * \return the id of the second box.
	 */
	unsigned int getRelationSecondBoxId(unsigned int relationId);

	/*!
	 * Gets the index of the control point linked with the given relation id in the second box.
	 *
	 * \param relationId : the id of the relation.
	 *
	 * \return the index of the control point.
	 */
	unsigned int getRelationSecondControlPointIndex(unsigned int relationId);

	/*!
	 * Calculates the optimal system after an editing operation
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box edited
	 * \param x : new begin value of the box
	 * \param y : new end value of the box
	 * \param maxSceneWidth : the max scene width
	 * \param movedBoxes : empty vector, will be filled with the ID of the boxes moved by this resolution
	 * \param maxModification : (optional) if provided, the system will refuse modification (in length or date) greater than this value.
	 *
	 * \return true if the move is allowed or false if the move is forbidden
	 */
	bool performMoving(unsigned int boxId, int x, int y, vector<unsigned int>* movedBoxes, int maxModification = NO_MAX_MODIFICATION);

	/*!
	 * Gets the box matching the given ID
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box
	 *
	 * \return the box matching the given ID
	 */
	ConstrainedBox* getBoxById(unsigned int boxId);

	/*!
	 * Gets the begin value of the box matching the given ID
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box
	 *
	 * \return the begin value of the box matching the given ID
	 */
	int getBeginValue(unsigned int boxId);

	/*!
	 * Gets the end value of the box matching the given ID
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box
	 *
	 * \return the end value of the box matching the given ID
	 */
	int getEndValue(unsigned int boxId);

	/*!
	 * Gets the number of controlPoints in the box matching the given ID
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box
	 *
	 * \return the number of controlPoints in the box matching the given ID
	 */
	int nbControlPoint(unsigned int boxId);

	/*!
	 * Gets the index of the first control point (the left one).
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param id : the ID of the box
	 *
	 * \return the index of the first control point (the left one)
	 */
	int getFirstControlPointIndex(unsigned int boxId);

	/*!
	 * Gets the index of the first control point (the right one).
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box
	 *
	 * \return the index of the first control point (the right one)
	 */
	int getLastControlPointIndex(unsigned int boxId);

	void setOptionalArgument(unsigned int boxId, std::string key, std::string value);
	void removeOptionalArgument(unsigned int boxId, std::string key);
	std::map<std::string, std::string> getOptionalArguments(unsigned int boxId);

	/*!
	 * Sets the message to send (via Network) when the given controlPoint is reached.
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param boxId : the ID of the box containing this controlPoint
	 * \param controlPointIndex : the index of the point to set message
	 */
	void setControlPointMessageToSend(unsigned int boxId, unsigned int controlPointIndex, string messageToSend);

	/*!
	 * Removes the message to send (via Network) when the given controlPoint is reached.
	 *
	 * Throws OutOfBoundException if the ID is not matching any box.
	 *
	 * \param id : the ID of the box
	 * \param controlPointIndex : the index of the point to remove message
	 */
	void removeControlPointMessageToSend(unsigned int boxId, unsigned int controlPointIndex);

	/*!
	 * Adds a new triggerPoint in CSP.
	 *
	 * \return the created trigger ID
	 */
	unsigned int addTriggerPoint(unsigned int containingBoxId = ROOT_BOX_ID);

	/*!
	 * Removes the triggerPoint from the CSP.
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger to be removed.
	 */
	void removeTriggerPoint(unsigned int triggerId);

	/*!
	 * Sets the controlPoint to link with triggerPoint (given by ID).
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger to be linked with the controlPoint.
	 * \param boxId : the ID of the box containing the controlPoint to link.
	 * \param controlPointIndex : the controlPoint index in the box.
	 *
	 * \return false if the controlPoint is already linked to another triggerPoint.
	 */
	bool setTriggerPointRelatedControlPoint(unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex);

	/*!
	 * Removes the controlPoint linked with the triggerPoint (given by ID).
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger to be unlinked with its controlPoint.
	 */
	void removeTriggerPointRelatedControlPoint(unsigned int triggerId);

	/*!
	 * Sets the triggerPoint (given by ID) message.
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger.
	 * \param triggerMessage : the trigger message
	 */
	void setTriggerPointMessage(unsigned int triggerId, std::string triggerMessage);

	/*!
	 * Gets the triggerPoint (given by ID) message.
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger.
	 *
	 * \return the trigger message
	 */
	std::string getTriggerPointMessage(unsigned int triggerId);

	/*!
	 * Gets the id of the box linked to the given trigger point.
	 *
	 * \param triggerId : the trigger Id.
	 *
	 * \return the id of the box linked to the trigger point,
	 * NO_ID if the trigger point is not linked to a box.
	 */
	unsigned int getTriggerPointRelatedBoxId(unsigned int triggerId);

	/*!
	 * Gets the index of the control point linked to the given trigger point.
	 *
	 * \param triggerId : the trigger Id.
	 *
	 * \return the index of the control point linked to the trigger point,
	 * NO_ID if the trigger point is not linked to a control point.
	 */
	unsigned int getTriggerPointRelatedControlPointIndex(unsigned int triggerId);

	/*!
	 * Sets the triggerPoint mute state.
	 *
	 * \param triggerId : the ID of the trigger to set the mute state.
	 * \param muteState : mute state ; true if the trigger must be mutable, false if not.
	 */
	void setTriggerPointMuteState(unsigned int triggerId, bool muteState);

	/*!
	 * Gets the triggerPoint mute state.
	 *
	 * \param triggerId : the ID of the trigger to test the mute state.
	 *
	 * \return true if the trigger is muted.
	 */
	bool getTriggerPointMuteState(unsigned int triggerId);

	/*!
	 * Fills the given vector with all the boxes ID used in the editor.
	 * Useful after a load.
	 *
	 * \param boxesID : the vector to fill with all boxes ID used.
	 */
	void getAllBoxesId(vector<unsigned int>* boxesID);

	/*!
	 * Fills the given vector with all the antPost relations ID used in the editor.
	 * Useful after a load.
	 *
	 * \param relationsId : the vector to fill with all relations ID used.
	 */
	void getAllAntPostRelationsId(vector<unsigned int>* relationsID);

	/*!
	 * Fills the given vector with all the triggers ID used in the editor.
	 * Useful after a load.
	 *
	 * \param triggersID : the vector to fill with all triggers ID used.
	 */
	void getAllTriggersId(vector<unsigned int>* triggersID);


	/*!
	 * Stores the editor.
	 *
	 * \param father : xmlNode.
	 */
	void store(xmlNodePtr father);

	/*!
	 * Loads the editor.
	 *
	 * \param root : xmlNode.
	 */
	void load(xmlNodePtr root);

	/*!
	 * Gets the CSP. Must not be called outside the library. This function is public only for debug purpose.
	 *
	 * \return : the CSP.
	 */
	CSP* getCSP();

private:
	unsigned int m_nextBoxId;
	unsigned int m_nextRelationId;
	unsigned int m_nextTriggerId;

	CSP* m_mainCSP;
	std::map<unsigned int, CSP*> m_boxIdToContainingCSP;
	std::map<unsigned int, CSP*> m_relationIdToContainingCSP;
	std::map<unsigned int, CSP*> m_triggerIdToContainingCSP;

	void displayMapContent();
};

#endif /*EDITOR_HPP_*/
