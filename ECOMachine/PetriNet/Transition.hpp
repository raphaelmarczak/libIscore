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

#ifndef TRANSITION_H_
#define TRANSITION_H_

/*!
 * \file Transition.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "Arc.hpp"
#include "PetriNetNode.hpp"
#include "TransitionBitArray.hpp"
#include "PriorityTransitionAction.hpp"
#include "TransitionAction.hpp"

#include "IncoherentStateException.hpp"

#include <vector>
#include <string>

typedef std::vector<std::string> stringVector;
typedef std::vector<TransitionAction*> transitionActionList;

/*!
 * \class Transition
 *
 * A transition is a link between two places.
 */
class Transition : public PetriNetNode
{
public:
	friend class PetriNet;

	/*!
	 * Sets the event to cross the transition.
	 *
	 * \param s : string representing the waited event to cross the transition.
	 */
	void setEvent(std::string s);

	/*!
	 * Correcty creates the BitArray (representing actives arcs).
	 */
	void createBitArray();

	/*!
	 * Resets the BitArray (representing actives arcs).
	 * All is set to 0.
	 */
	void resetBitArray();

	/*!
	 * Resets the arcs state (active or not).
	 */
	void resetIncomingArcsState();
	
	void merge(Transition* transitionToMerge);

	/*!
	 * Adds an action to call when the transition is crossed.
	 *
	 * \param pt2Func : function with one void* parameter.
	 * \param argPt2Func : argument for pt2Func.
	 */
	void addExternAction(void(*pt2Func)(void*), void* argPt2Func);

	/*!
	 * Removes actions called when the transition is crossed.
	 */
	void removeExternActions();

	/*!
	 * Sets the arc (with number matching the index in bitArray) as active.
	 *
	 * Throws OutOfBoundException if arcNumber is greater or equal the number of ingoing arcs.
	 *
	 * \param arcNumber : the number of arc to be set as active.
	 */
	void setArcAsActiveByNumber(int arcNumber);

	/*!
	 * Sets the arc (with number matching the index in bitArray) as inactive.
	 *
	 * Throws OutOfBoundException if arcNumber is greater or equal the number of ingoing arcs.
	 *
	 * \param arcNumber : the number of arc to be set as inactive.
	 */
	void setArcAsInactiveByNumber(int arcNumber);

	/*!
	 * Sets the arc as active.
	 *
	 * Throws OutOfBoundException if arcNumber is greater or equal the number of ingoing arcs.
	 *
	 * \param arcNumber : the arc to be set as active.
	 * \param recalculateArcTime : True if the arc time must be recalculate. False if we want to use
	 * 							   the time previously stored into this arc.
	 */
	void setArcAsActive(Arc* arc, unsigned int timeOffset, bool recalculateArcTime);

	/*!
	 * Sets the arc as inactive.
	 *
	 * Throws OutOfBoundException if arcNumber is greater or equal the number of ingoing arcs.
	 *
	 * \param arcNumber : the arc to be set as inactive.
	 */
	void setArcAsInactive(Arc* arc);

	/*!
	 * Tests if all ingoing arcs are actives.
	 *
	 * Throws IncoherentStateException if this transition does not have in going arcs
	 * (should not happen)
	 *
	 * \return true if all ingoing arcs are actives.
	 */
	bool areAllInGoingArcsActive();

	/*!
	 * Gets the event to cross the transition.
	 *
	 * \return string representing the waited event to cross the transition.
	 */
	std::string getEvent();

	/*!
	 * Tests if the transition is static (crossable without external event).
	 *
	 * \return true if the transition is static.
	 */
	bool isStatic();

	/*!
	 * Sets the transition temporally as static.
	 *
	 * The transition could regain its original status by calling
	 * turnIntoOriginalStatus().
	 */
	void turnIntoStatic();

	/*!
	 * Sets the transition in its original status if it was
	 * previously turned as static.
	 */
	void turnIntoOriginalStatus();

//	/*!
//	 * Tests if one of the in-going arc has reach its min value.
//	 *
//	 * \return true if one of the in-going arc has reach its min value.
//	 */
//	bool isPureAndOnMinLimit();
//
//	/*!
//	 * Tests if one of the in-going arc has reach its max value.
//	 *
//	 * \return the in-going arc which has reach its max value. NULL if not.
//	 */
//	Arc* isPureAndOnMaxLimit();

	/**
	 * Cross the transition.
	 */
	void crossTransition(bool mustSetTokenValue, unsigned int newTokenValue = 0);

	/*!
	 * Tests if one of the in-going arc comes from a place with
	 * enough token to be crossable.
	 *
	 * \return the in-going arc which comes from a place with enough token to be crossable. NULL if not.
	 */
	Arc* hasATokenInPredecessors();

	void print();

	ExtendedInt getStartDate();
	ExtendedInt getEndDate();

	/*!
	 * Destructor.
	 */
	~Transition();

private:
	/*!
	 * Default constructor.
	 *
	 * Must not be called (a Transition is a PetriNetItem, so it must belong to a Petri Network).
	 */
	Transition();

	/*!
	 * Constructor specifying the Petri Network.
	 *
	 * Must be called only by a PetriNet object.
	 *
	 * \param petriNet : the PetriNet in which the Transition is.
	 */
	Transition(PetriNet* petriNet);

	TransitionBitArray* m_activeArcsBitArray;

	stringVector m_events; // Waited event to cross the transition (STATIC_EVENT if no external event is waited).

//	void (*m_externAction)(void*);
//	void* m_externActionArgument;

	transitionActionList m_externActions;

	ExtendedInt m_startDate;
	ExtendedInt m_endDate;

	PriorityTransitionAction* m_startAction;
	PriorityTransitionAction* m_endAction;


};

#endif /*TRANSITION_H_*/
