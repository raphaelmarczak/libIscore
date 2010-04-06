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

#ifndef PETRINET_H_
#define PETRINET_H_

/*!
 * \file PetriNet.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "Place.hpp"
#include "Transition.hpp"
#include "Arc.hpp"
#include "PriorityTransitionAction.hpp"
#include "ThreadSafeList.hpp"

#include "IllegalArgumentException.hpp"

#include <vector>
#include <list>
#include <queue>
#include <string>

#define STATIC_EVENT "event0"
#define MY_MIN_STEP_TIME_IN_MS 100

struct priorityTransitionActionComparison {
  bool operator() (const PriorityTransitionAction* left, const PriorityTransitionAction* right) {
	  return (left->getDate() > right->getDate());
  }
};

typedef std::vector<Transition*> transitionList;
typedef std::vector<Place*> placeList;
typedef std::list<std::string> stringList;

typedef std::priority_queue<PriorityTransitionAction*,
	 						std::vector<PriorityTransitionAction *,
	 								    std::allocator<PriorityTransitionAction*> >,
	 					    priorityTransitionActionComparison >
		priorityTransitionActionQueue;

/*!
 * \class PetriNet
 *
 * PetriNet represents an arcs and tokens colored Petri Network with actions as well as
 * time-flushed and synchronized transitions.
 *
 * To create a Petri Network, a number of colors could be provided. If not, the Petri Network
 * will be created with one color.
 *
 * This class allows to create/delete Places, create/delete Transitions,
 * create/delete Arcs, add/reset events, change the number of colors.
 *
 * Places, Transitions, Arcs ... must not be created outside a PetriNet (useless).
 */
class PetriNet
{
public:
	// constructor

	/*!
	 * Default constructor / Constructor specifying the number of color.
	 *
	 * Creates a Petri Network with the number of colors provided. If no
	 * number of color is provided, the Petri Network will be created with
	 * one color.
	 *
	 * \param nbColors : (optional) The Petri Network number of colors.
	 */
	PetriNet(unsigned int nbColors = 1);

	/*!
	 * Destructor.
	 */
	~PetriNet();

	/*!
	 * Updates the PetriNet in order to cross transition, update timer, ...
	 *
	 * Throws incoherentStateException if we need to cross a transition
	 * impossible to cross.
	 */
	void update();

	/*!
	 * Modifies the PetriNet according to the current time.
	 */
	void makeOneStep();

	/*!
	 * Increments the current time of this petriNet.
	 *
	 * \param dt : time to add to the current time.
	 */
	void addTime(unsigned int dt);

	/*!
	 * Gets the PetriNet current time.
	 *
	 * \return the PetriNet current time.
	 */
	unsigned int getCurrentTimeInMs();

	/*!
	 * Tests the validity of a color (>1 and <= number of colors)
	 *
	 * \param color : color to test validity
	 * \return true if the given color number is valid
	 */
	bool isColorValid(unsigned int color);

	/*!
	 * Gets the number of colors in this Petri Network
	 *
	 * \return the number of colors
	 */
	unsigned int nbOfPossibleColors();

	/*!
	 * Changes the number of colors.
	 * The new number must be greater.
	 *
	 * Throws a IllegalArgumentException if NewNbColors
	 * is lower or equal than current number of color
	 * in this Petri Network.
	 *
	 * \param newNbColors : new number of colors
	 */
	void changeNbOfColors(unsigned int newNbColors);

	/*!
	 * Empties the events list.
	 * The list will only contain the static event.
	 */
	void resetEvents();

	/*!
	 * Adds an event in this Petri Network.
	 *
	 * \param event : event to add.
	 */
	void putAnEvent(std::string event);


	/*!
	 * Tests if an particular event was received by this Petri Network.
	 *
	 * \param event : event to test the reception.
	 * \return true if the event was received by this PetriNetwork
	 */
	bool isAnEvent(std::string event);

	/*!
	 * Creates a new place.
	 *
	 * \return the new place created.
	 */
	Place* createPlace();

	/*!
	 * Creates a new transition.
	 *
	 * \return the new transition created.
	 */
	Transition* createTransition();

	/*!
	 * Creates an arc between from place to a transition.
	 * A color could be specified (1 if not).
	 *
	 * \param from : place.
	 * \param to : transition.
	 * \param color : (optional) arc color.
	 * \return the new arc created or matching arc if already exist.
	 */
	Arc* createArc(Place* from, Transition* to, int color = 1);

	/*!
	 * Creates an arc between from a transition to a place.
	 * A color could be specified (1 if not).
	 *
	 * \param from : transition.
	 * \param to : place.
	 * \param color : (optional) arc color.
	 * \return the new arc created or matching arc if already exist.
	 */
	Arc* createArc(Transition* from, Place* to, int color = 1);

	/*!
	 * Deletes all arcs between the givens nodes.
	 *
	 * \param from : PetriNetNode.
	 * \param to : PetriNetNode.
	 */
	void deleteArc(PetriNetNode* from, PetriNetNode* to);

	/*!
	 * Deletes colored arc between the givens nodes.
	 *
	 * \param from : PetriNetNode.
	 * \param to : PetriNetNode.
	 * \param color : arc color to delete.
	 */
	void deleteArc(PetriNetNode* from, PetriNetNode* to, int color);

	/*!
	 * Removes and deletes a Place.
	 *
	 * \param placeToDelete : place to delete.
	 */
	void deleteItem(Place* & placeToDelete);

	/*!
	 * Removes and deletes a Transition.
	 *
	 * \param transitionToDelete : transition to delete.
	 */
	void deleteItem(Transition* & transitionToDelete);

	/*!
	 * Specifies the given transition as sensitized.
	 *
	 * \param t : transition to set as sensitized.
	 */
	void turnIntoSensitized(Transition* t);
//
//
//	/*!
//	 * Specifies the given transition as unsensitized.
//	 * If it still has token in one of its predecessors,
//	 * specifies it as active.
//	 *
//	 * \param t : transition to set as unsensitized.
//	 */
//	void turnIntoUnsensitized(Transition* t);

	/*!
	 * Gets the places in this PetriNet
	 *
	 * \return the Petri Net Places
	 */
	placeList getPlaces();

	/*!
	 * Gets the transitions in this PetriNet
	 *
	 * \return the Petri Net Transitions
	 */
	transitionList getTransitions();

	/*!
	 * Gets the events in this PetriNet
	 *
	 * \return the Petri Net Events
	 */
	stringList getEvents();

//	/*!
//	 * Gets the active transitions in this PetriNet
//	 *
//	 * \return the Petri Net Active Transitions
//	 */
//	transitionList getActiveTransitions();

	/*!
	 * Gets the sensitized transitions in this PetriNet
	 *
	 * \return the Petri Net Sensitized Transitions
	 */
	transitionList getSensitizedTransitions();

	/*!
	 * Adds an action (START/END) to the priority queue.
	 *
	 * \param action : the action to add.
	 */
	void addActionToPriorityQueue(PriorityTransitionAction* action);

	/*!
	 * Gets the top action (the most closer in time).
	 *
	 * Throws IncoherentStateException if the priority queue is empty.
	 *
	 * \return the top action.
	 */
	PriorityTransitionAction* getTopActionOnPriorityQueue();

	priorityTransitionActionQueue getPriorityQueue();

	/*!
	 * Removes the top action (the most closer in time).
	 *
	 * Throws IncoherentStateException if the priority queue is empty.
	 */
	void removeTopActionOnPriorityQueue();

	/*!
	 * Tests if the priority queue is empty.
	 *
	 * \return true if the priority queue is empty.
	 */
	bool isEmptyPriorityQueue();
	
	void setUpdateFactor(float updateFactor);
	float getUpdateFactor();

	void ignoreEventsForOneStep();

	void addWaitedTriggerPointMessageAction(void* arg, void(*pt2Func)(void *, bool, Transition*));
	void removeWaitedTriggerPointMessageAction();

	void pushTransitionToCrossWhenAcceleration(Transition* t);

	void print();

private:
	unsigned int m_currentTime; // current time of this petriNet in microsecondes.

	unsigned int m_nbColors; // number of colors.
	
	float m_updateFactor;

	placeList m_places; // list of places.
	transitionList m_transitions; // list of transitions.

	ThreadSafeList m_incomingEvents; // list of events (strings).

	long long m_previousCallOfMakeOneStepInMs;

//	/*
//	 * A transition is active when it has at least one predecessor with
//	 * enought tokens.
//	 *
//	 * A transition is sensitized when all predecessors have enought tokens,
//	 * and all in-going arcs timers are between min and max value.
//	 * Only the matching event is waiting for crossing this transition.
//	 *
//	 */
//	transitionList m_activeTransitions; // list of actives transitions.
	transitionList m_sensitizedTransitions;	// list of sensitized transitions.
	transitionList m_transitionsToCrossWhenAcceleration;

	priorityTransitionActionQueue m_priorityTransitionsActionQueue;

	bool m_mustCrossAllTransitionWithoutWaitingEvent;

	void (*m_waitedTriggerPointMessageAction)(void*, bool, Transition*);
	void* m_waitedTriggerPointMessageArgument;

	// Private function only used to factorize the program.
	Arc* newArc(PetriNetNode* from, PetriNetNode* to, int color);
	void deleteItem(PetriNetNode* nodeToDelete);

	// Calculates the delta time.
	long long computeDt();
};

#endif /*PETRINET_H_*/
