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

#ifndef ARC_H_
#define ARC_H_

/*!
 * \file Arc.hpp
 * \author Raphael Marczak, based on Antoine Allombert LISP code
 * \date 2008-2009
 */

#include "PetriNetItem.hpp"
#include "ExtendedInt.hpp"


#include "IllegalArgumentException.hpp"

#define NO_NUMBER -1

#define NB_OF_TOKEN_TO_ACTIVE_ARC 1
#define NB_OF_TOKEN_TO_PRODUCE 1
#define NB_OF_TOKEN_TO_CONSUME 1

class PetriNetNode;
class Transition;
class Place;

/*!
 * \class Arc
 *
 * An arc is a link between two nodes in the Petri Network.
 * It could exist between from a place to a transition, or
 * from a transition to a place.
 *
 * An arc must have a color (1 if not provided).
 *
 * An arc from a place to a transition could consume a token
 * if both token and arc have the same color.
 *
 * An arc from a transition to a place could produce a token
 * in its color.
 */
class Arc : public PetriNetItem
{
public:
	friend class PetriNet;

	/*!
	 * Gets the node where this arc come from.
	 *
	 * \return the node where the arc come from.
	 */
	PetriNetNode* getFrom();

	/*!
	 * Gets the node where this arc go into.
	 *
	 * \return the node where the arc go into.
	 */
	PetriNetNode* getTo();

	/*!
	 * Sets the min time value (relative) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given min value is greater than the arc max value.
	 *
	 * \param minTime : the new min time value (relative).
	 */
	void changeRelativeMinTime(ExtendedInt minTime);

	/*!
	 * Sets the max time value (relative) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given max value is lower than the arc min value.
	 *
	 * \param maxTime : the new max time value (relative).
	 */
	void changeRelativeMaxTime(ExtendedInt maxTime);

	/*!
	 * Sets the min and max time values (relative) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given min value is greater than the given max value.
	 *
	 * \param minTime : the new min time value (relative).
	 * \param maxTime : the new max time value (relative).
	 */
	void changeRelativeTime(ExtendedInt minTime, ExtendedInt maxTime);

	/*!
	 * Sets the min time value (absolute) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given min value is greater than the arc max value.
	 *
	 * \param minTime : the new min time value (absolute).
	 */
	void changeAbsoluteMinTime(ExtendedInt minTime);

	/*!
	 * Sets the max time value (absolute) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given max value is lower than the arc min value.
	 *
	 * \param maxTime : the new max time value (absolute).
	 */
	void changeAbsoluteMaxTime(ExtendedInt maxTime);

	/*!
	 * Sets the min and max time values (absolute) to make the arc active (crossable).
	 *
	 * Throws IllegalArgumentException if the given min value is greater than the given max value.
	 *
	 * \param minTime : the new min time value (absolute).
	 * \param maxTime : the new max time value (absolute).
	 */
	void changeAbsoluteTime(ExtendedInt minTime, ExtendedInt maxTime);

//	/*!
//	 * Adds a timestep to the arc intern timer.
//	 *
//	 * \param timeStep : (optional) timestep to add to the time (1 if not provided).
//	 */
//	void passingTime(unsigned int timeStep = 1);
//
//	/*!
//	 * Tests if the intern timer is greater than the min value to make this arc active (crossable).
//	 *
//	 * return : true if the timer is greater than the min value.
//	 */
//	bool haveReachedMinLimit();
//
//	/*!
//	 * Tests if the intern timer is greater than the max value to make this arc active (crossable).
//	 *
//	 * return : true if the timer is greater than the max value.
//	 */
//	bool haveReachedMaxLimit();

	/*!
	 * Gets the number of token in the place where the arc is coming (same color as the arc).
	 *
	 * Throws a IllegalArgumentException if this arc is not from a place, but
	 * from a transition.
	 *
	 * \return the number of token in the "from place".
	 */
	int nbOfArcColorLabelTokensInFrom();

	/*!
	 * Tests if this arc is active.
	 *
	 * Throws a IllegalArgumentException if this arc is not from a place, but
	 * from a transition.
	 *
	 * \return true this arc is active.
	 */
	bool isActive();

	/*!
	 * Tests if the place from where the arc is coming
	 * have enough token to make this arc crossable.
	 *
	 * Throws a IllegalArgumentException if this arc is not from a place, but
	 * from a transition.
	 *
	 * \return true if there is enough token to the "from place" to cross this arc.
	 */
	bool haveEnoughTokensInFrom();

	/*!
	 * Consumes token in the place where the arc is coming.
	 *
	 * Throws a IllegalArgumentException if this arc is not from a place, but
	 * from a transition.
	 *
	 * Throws a IllegalArgumentException if the place does not have enough token to consume.
	 */
	unsigned int consumeTokenInFrom();

	/*!
	 * Produces token in the place where the arc is going into.
	 *
	 * Throws a IllegalArgumentException if this arc is not going into a place, but
	 * into a transition.
	 */
	void produceTokenInTo(unsigned int);

	/*!
	 * Gets the arc color.
	 *
	 * \return arc color.
	 */
	int getColor();

	/*!
	 * Gets the arc min value (relative).
	 *
	 * \return arc min value (relative).
	 */
	ExtendedInt getRelativeMinValue();

	/*!
	 * Gets the arc max value (relative).
	 *
	 * \return arc max value (relative).
	 */
	ExtendedInt getRelativeMaxValue();

	/*!
	 * Gets the arc min value (absolute).
	 *
	 * \return arc min value (absolute).
	 */
	ExtendedInt getAbsoluteMinValue();

	/*!
	 * Gets the arc max value (absolute).
	 *
	 * \return arc max value (absolute).
	 */
	ExtendedInt getAbsoluteMaxValue();

//	/*!
//	 * Gets the arc timer.
//	 *
//	 * \return arc timer.
//	 */
//	ExtendedInt getTimer();

	/*!
	 * Sets the arc number (matching index in Transition bit array).
	 *
	 * \param arcNumber : arc number to be set.
	 */
	void setNumber(unsigned int arcNumber);

	/*!
	 * Gets the arc number (matching index in Transition bit array).
	 *
	 * \return arc number.
	 */
	int getNumber();


	// Destructor.
	~Arc();

private:
	// Arc color
	int m_color;

	// Node from.
	PetriNetNode* m_nodeFrom;

	// Node to.
	PetriNetNode* m_nodeTo;

	// Min time value (relative) to make this arc active (crossable).
	ExtendedInt m_relativeMinValue;

	// Max time value (relative) to make this arc active (crossable).
	ExtendedInt m_relativeMaxValue;

	// Min time value (absolute) to make this arc active (crossable).
	ExtendedInt m_absoluteMinValue;

	// Max time value (absolute) to make this arc active (crossable).
	ExtendedInt m_absoluteMaxValue;

//	// Time value.
//	ExtendedInt m_internTimer;

	// Number of this Arc if it goes into a transition.
	int m_number;

	/*!
	 * Constructor using the two nodes and a color.
	 *
	 * Must only be called by a PetriNet object.
	 *
	 * \param petriNet : the petriNet containing this arc.
	 * \param from : the node from.
	 * \param to : the node to.
	 * \param color : (optionnal) arc color (1 if not provided).
	 */
	Arc(PetriNet* petriNet, PetriNetNode* from, PetriNetNode* to, int color = 1);

	/*!
	 * Default constructor.
	 *
	 * Must not be called (an Arc is a PetriNetItem, so it must belong to a Petri Network).
	 */
	Arc();
};

#endif /*ARC_H_*/
