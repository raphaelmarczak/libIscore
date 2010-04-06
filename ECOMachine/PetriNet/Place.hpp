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

#ifndef PLACE_H_
#define PLACE_H_

/*!
 * \file Place.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "Token.hpp"
#include "PetriNetNode.hpp"
#include "OutOfBoundException.hpp"
#include <vector>

/*!
 * \class Place
 *
 * In a Petri Network, a place is a node which could contain colored tokens.
 */
class Place : public PetriNetNode
{
public:
	friend class PetriNet;

	/*!
	 * Gets the number of tokens in the place (all colors taken together).
	 *
	 * \return the number of token (all colors taken together).
	 */
	int nbOfTokens();

	/*!
	 * Gets the number of tokens in the place matching the given color.
	 *
	 * Throws a OutOfBoundException if the color is not valid.
	 *
	 * \param color : color of the tokens.
	 * \return the number of token matching the color.
	 */
	int getNbOfTokens(unsigned int color);

	/*!
	 * Consumes token in the place.
	 *
	 * Throws a IllegalArgumentException if the number of token to consume
	 * is greater than the number of token effectively present.
	 *
	 * \param nbOfTokens : number of tokens to consume.
	 * \param colorLabel : (optional) color of the tokens to consume (1 if not provided).
	 */
	unsigned int consumeTokens(unsigned int nbOfTokens, unsigned int colorLabel = 1);

	/*!
	 * Produces token in the place.
	 *
	 * \param nbOfTokens : number of tokens to produce.
	 * \param colorLabel : (optional) color of the tokens to produce (1 if not provided).
	 */
	void produceTokens(unsigned int nbOfTokens, unsigned int colorLabel = 1, unsigned int tokensTime = 0);

	void print();

	/*!
	 * Destructor.
	 */
	virtual ~Place();

private :
	std::vector<std::vector<Token> > m_tokenByColor; // tokens by color.

	/*!
	 * Default constructor.
	 *
	 * Must not be called (a Place is a PetriNetItem, so it must belong to a Petri Network).
	 */
	Place();

	/*!
	 * Constructor specifying the Petri Network.
	 *
	 * Must be called only by a PetriNet object.
	 *
	 * \param petriNet : the PetriNet in which the Place is.
	 */
	Place(PetriNet* petriNet);

	/*!
	 * Changes the number of colors.
	 * The new number must be greater.
	 *
	 * This function must only be called by a PetriNet object.
	 *
	 * \param newNbColors : new number of colors
	 */
	void changeNbOfColors(int newNbColors);
};

#endif /*PLACE_H_*/
