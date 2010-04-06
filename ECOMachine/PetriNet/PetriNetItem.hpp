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

#ifndef PETRINETITEM_H_
#define PETRINETITEM_H_

/*!
 * \file PetriNetItem.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

class PetriNet;

/*!
 * \class PetriNetItem
 *
 * This class represents every item in a Petri Network (Place, Transition, Arc).
 *
 * A petriNetItem must belong to a specific Petri Network.
 */
class PetriNetItem
{
public:
	/*!
	 * Destructor.
	 */
	virtual ~PetriNetItem();

	/*!
	 * Gets the Petri Network.
	 *
	 * \return the PetriNet in which this PetriNetItem is.
	 */
	PetriNet* getPetriNet();

	/*!
	 * Gets the number of colors in the Petri Network
	 *
	 * \return the number of colors
	 */
	unsigned int nbOfPossibleColors();

	/*!
	 * Tests the validity of a color (>1 and <= number of petri network colors)
	 *
	 * \param color : color to test validity
	 * \return true if the given color number is valid
	 */
	bool isColorValid(int color);

protected :
	/*!
	 * Default constructor.
	 *
	 * Must not be called (a PetriNetItem must belong to a Petri Network).
	 */
	PetriNetItem();

	/*!
	 * Constructor specifying the Petri Network.
	 *
	 * Must only be called by childs.
	 *
	 * \param petriNet : the PetriNet in which the PetriNetItem is.
	 */
	PetriNetItem(PetriNet* petriNet);

private :
	PetriNet* m_petriNet; // PetriNet containing this PetriNetItem.
};

#endif /*PETRINETITEM_H_*/
