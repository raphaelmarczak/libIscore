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

#ifndef PRIORITYTRANSITIONACTION_HPP_
#define PRIORITYTRANSITIONACTION_HPP_

/*!
 * \file PriorityTransitionAction.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "IllegalArgumentException.hpp"
#include "ExtendedInt.hpp"

#define START 1
#define END 0

class Transition;

/*!
 * \class PriorityTransitionAction
 *
 * PriorityTransitionAction.
 */
class PriorityTransitionAction
{
public:
	/*!
	 * Constructor specifying the transition, the type (START, END) and the starting date.
	 *
	 * Throws IllegalArgumentException if type is not START or END.
	 * Throws IllegalArgumentException if transition is NULL.
	 *
	 * \param transition : the transition linked to this action.
	 * \param type : action type (STARD, END).
	 * \param date : action starting date.
	 */
	PriorityTransitionAction(Transition* transition, short type, ExtendedInt date);

	/*!
	 * Gets the action type (START, END).
	 *
	 * \return the action type (START, END).
	 */
	short getType() const;

	/*!
	 * Gets the action starting date.
	 *
	 * \return the action starting date.
	 */
	ExtendedInt getDate() const;

	ExtendedInt setDate(unsigned int date);

	/*!
	 * Gets the linked transition.
	 *
	 * \return the linked transition.
	 */
	Transition* getTransition() const;

	/*!
	 * Tests if the action is still enable.
	 *
	 * \return true if this action is still enable.
	 */
	bool isEnable() const;

	/*!
	 * Disables this PriorityTranstionAction.
	 */
	void disable();

//	/*!
//	 * Lower operator.
//	 *
//	 * Allows the use of PriorityTranstionAction < PriorityTranstionAction
//	 *
//	 * \param a
//	 * \return true if this.date < a.date
//	 */
//	bool operator<(PriorityTransitionAction a) const;

	virtual ~PriorityTransitionAction();

private:
	PriorityTransitionAction();

	Transition* m_linkedTransition;
	short m_type;
	ExtendedInt m_date;
	bool m_isEnable;
};

#endif /*PRIORITYTRANSITIONACTION_HPP_*/
