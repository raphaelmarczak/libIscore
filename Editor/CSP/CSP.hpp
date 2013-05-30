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

#ifndef CSP_H
#define CSP_H

#define NO_MAX_MODIFICATION 0

/*!
 * \file CSP.h
 * \author Raphael Marczak (LaBRI), based on Bruno Valeze (LaBRI) code
 * \date 2008-2009
 */

#include "XMLInclude.hpp"

#include <string>
#include <vector>
#include <map>

#include "../../Includes/CSPTypes.hpp"
#include "ConstrainedBox.hpp"
#include "AntPostRelation.hpp"
#include "TriggerPoint.hpp"

#include "OutOfBoundException.hpp"
#include "IllegalArgumentException.hpp"

class ConstrainedTemporalEntity;
class Solver;
class CSPConstrainedVariable;
class CSPLinearConstraint;
class BinaryTemporalRelation;
class AllenRelation;
//class IntervalRelation;
class BoundingRelation;
class StoryLine;

/*!
 * \class CSP
 * \author Raphael Marczak (LaBRI), based on Bruno Valeze (LaBRI) code
 * \date 2008-2009
 *
 * This class is the top-level interface of the CSP package.
 * The Constraint Solving Problem is composed of several time-defined
 * entities, linked by binary tempolal relations.
 * This class manages the adding/removing of entities/relations and
 * ensures that the temporal configuration always respects the system
 * relations defined by the user
 */
class CSP
{
public :


	/*!
	 * Default constructor.
	 */
	CSP();

	/*!
	 * Destructor.
	 */
	~CSP();

	/*!
	 * Gets the name of an AntPostRelation according to its type.
	 *
	 * \param t : the type of the AntPostRelation
	 *
	 * \return the name as a string
	 */
	static string getNameFromRelation(TemporalRelationType t);

	/*!
	 * Adds a new box in the CSP.
	 *
	 * Throws an IllegalArgumentException if the boxId already exist.
	 *
	 * \param boxId : the ID to give to the box. Must not already exist.
	 * \param boxBeginPos : the begin value in ms
	 * \param boxLength : the length value in ms
	 * \param motherId : mother box ID if any, NO_ID if the box to create has no mother
	 * \param maxSceneWidth : the max scene width
	 *
	 * \return the newly created box ID
	 */
	unsigned int addBox(unsigned int boxId, int boxBeginPos, int boxLength, unsigned int motherId, int maxSceneWidth);

	/*!
	 * Removes a box from the CSP : removes the relation implicating it and the
	 * box's variables.
	 *
	 * \param boxId : the ID of the box to remove
	 */
	void removeBox(unsigned int boxId, std::vector<unsigned int>& relationsRemoved, std::vector<unsigned int>& triggersRemoved);

	/*!
	 * Adds a AntPostRelation between two controlPoints.
	 *
	 * \param boxId1 : the ID of the first box
	 * \param controlPoint1 : the index of the point in the first box to put in relation
	 * \param boxId2 : the ID of the second box
	 * \param controlPoint2 : the index of the point in the second box to put in relation
	 * \param type : the relation type
	 * \param movedBoxes : empty vector, will be filled with the ID of the boxes moved by this new relation
	 *
	 * \return the newly created relation id (NO_ID if the creation is impossible).
	 */
	unsigned int addAntPostRelation(unsigned int relationId, unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, int minBound, int maxBound, vector<unsigned int>& movedBoxes, bool mustCallSolver = true);

	/*!
	 * Removes the temporal relation using given id.
	 *
	 * \param relationId : the ID of the relation to remove
	 */
	void removeTemporalRelation(unsigned int relationId);

//	IntervalRelation* addInterval(ConstrainedTemporalEntity *box1, ConstrainedTemporalEntity *box2, int value, int tolerance);
//	BoundingRelation* addBoundingRelation(ConstrainedTemporalEntity *box1, ConstrainedTemporalEntity *box2);

	void removeTemporalRelation(BinaryTemporalRelation *rel);
//	void removeBoundingRelation(BoundingRelation *rel);

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
	 *
	 * \return true if the move is allowed or false if the move is forbidden
	 */
	bool performMoving(unsigned int boxId, int x, int y, vector<unsigned int>& movedBoxes);
	bool performMoving(unsigned int boxId, int x, int y, vector<unsigned int>& movedBoxes, unsigned int maxModification);

	// Get the links implicating one particular entity
	vector<BinaryTemporalRelation*> *links(ConstrainedTemporalEntity *ent) const;
	// Get the whole links
	vector<BinaryTemporalRelation*> *links() const;

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
	 * \param boxId : the ID of the box
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
	 * \param boxId : the ID of the box containing this controlPoint
	 * \param controlPointIndex : the index of the point to remove message
	 */
	void removeControlPointMessageToSend(unsigned int boxId, unsigned int controlPointIndex);

	void getTriggerPointMap(std::map<unsigned int, TriggerPoint *>& triggerPoints);

	/*!
	 * Gets the triggerPoint matching the given ID.
	 *
	 * Throws OutOfBoundException if the ID is not matching any triggerPoint.
	 *
	 * \param triggerId : the ID of the trigger
	 *
	 * \return the trigger matching the ID
	 */
	TriggerPoint* getTriggerPoint(unsigned int triggerId);

	/*!
	 * Adds a new triggerPoint in CSP.
	 *
	 * \param triggerId : the ID to give to the Trigger.
	 *
	 * \return the created trigger ID
	 */
	unsigned int addTriggerPoint(unsigned int triggerId);

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
	 * Fills the given vector with all the boxes ID in this CSP.
	 *
	 * \param boxesId : the vector to fill with all the boxes ID in this CSP.
	 */
	void getAllBoxesId(vector<unsigned int>& boxesID);
	void getAllAntPostRelationsId(vector<unsigned int>& relationsID);
	void getAllTriggersId(vector<unsigned int>& triggersID);

	void getAllAntPostRelations(vector<AntPostRelation*>& antPostRelations);
	AntPostRelation* getAntPostRelationById(unsigned int relationID);

	bool areControlPointsInRelation(ControlPoint* cp1, ControlPoint* cp2);

	void store(xmlNodePtr father);
	void load(xmlNodePtr root, std::map<unsigned int, CSP*>& boxesMap,
			std::map<unsigned int, CSP*>& relationsMap,
			std::map<unsigned int, CSP*>& triggersMap,
			ConstrainedBox* motherBox);
	void loadBoxes(xmlNodePtr root, std::map<unsigned int, CSP*>& boxesMap,
			std::map<unsigned int, CSP*>& relationsMap,
			std::map<unsigned int, CSP*>& triggersMap,
			ConstrainedBox* motherBox);
	void loadRelations(xmlNodePtr root, std::map<unsigned int, CSP*>& relationsMap);
	void loadTriggerPoints(xmlNodePtr root, std::map<unsigned int, CSP*>& triggersMap);

	unsigned int getMaxBoxId();
	unsigned int getMaxRelationId();
	unsigned int getMaxTriggerId();

	void changeAllBoxMaxSceneWidth(int newValue);
	Solver *getSolver();

private :

	Solver *_solver;

	std::map<unsigned int, ConstrainedTemporalEntity *> *_cedEntities;

	vector<BinaryTemporalRelation *> *_temporalRelations;

	std::map<unsigned int, TriggerPoint *> *_triggerPoints;

	// Add/remove linear contraints to/from the solver, when adding/removing a relation
	CSPLinearConstraint* addConstraint(vector<int> *varsIDs, vector<int> *varsCoeffs, BinaryRelationType type, int value, bool mustCallSolver);
	bool removeConstraint(CSPLinearConstraint *cst);

	AllenRelation* addAllenRelation(ConstrainedTemporalEntity *box1, ConstrainedTemporalEntity *box2, AllenType type, bool mustCallSolver = true);

	// Update variables values from the solver
	void updateFromSolver();
};




#endif
