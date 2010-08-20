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

#include "Place.hpp"

/*!
 * \file Place.cpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "PetriNet.hpp"


Place::Place(PetriNet* petriNet)
: PetriNetNode(petriNet) {
	m_childPetriNet = NULL;

	int nbColors = getPetriNet()->nbOfPossibleColors();

	m_tokenByColor.resize(nbColors);
}

int Place::getNbOfTokens(unsigned int color) {
	if (!isColorValid(color)) {
		throw OutOfBoundException();
	}

	return m_tokenByColor[color - 1].size();
}

int Place::nbOfTokens() {
	int nbTokens = 0;

	for (unsigned int i = 0 ; i < m_tokenByColor.size() ; i++) {
		nbTokens += m_tokenByColor[i].size();
	}

	return nbTokens;
}

unsigned int Place::consumeTokens(unsigned int nbOfTokens, unsigned int colorLabel) {
	colorLabel--;

	unsigned int time = 0;

	if (m_tokenByColor[colorLabel].size() < nbOfTokens) {
		throw IllegalArgumentException();
	} else {
		for (unsigned int i = 0; i < nbOfTokens; ++i) {
			time = m_tokenByColor[colorLabel].back().getRemainingTime();
			m_tokenByColor[colorLabel].pop_back();
		}
	}

	return time;
}

void Place::produceTokens(unsigned int nbOfTokens, unsigned int colorLabel, unsigned int tokensTime) {
	unsigned int oldNumberOfTokens = getNbOfTokens(colorLabel);

	for (unsigned int i = 0; i < nbOfTokens; ++i) {
		Token token;
		token.setRemainingTime(tokensTime);
		m_tokenByColor[colorLabel - 1].push_back(token);
	}

	if ((oldNumberOfTokens < NB_OF_TOKEN_TO_ACTIVE_ARC) && (getNbOfTokens(colorLabel) >= NB_OF_TOKEN_TO_ACTIVE_ARC)) {
		arcList outGoingArcs = outGoingArcsOf(colorLabel);
		for (unsigned int i = 0 ; i < outGoingArcs.size() ; ++i) {
			Arc* arc = outGoingArcs[i];

			if (!(dynamic_cast<Transition*>(arc->getTo()))) {
				throw IncoherentStateException();
			}

			Transition* transitionTo = ((Transition*) arc->getTo());

			transitionTo->setArcAsActive(arc, tokensTime, true);

			if (transitionTo->isStatic()) {
				if(arc->getRelativeMinValue().getValue() < (int) tokensTime) {
					getPetriNet()->pushTransitionToCrossWhenAcceleration(transitionTo);
				}
			} else {
				if(arc->getRelativeMaxValue().getValue() < (int) tokensTime) {
					getPetriNet()->pushTransitionToCrossWhenAcceleration(transitionTo);
				}
			}

		}
	}



}

void Place::changeNbOfColors(int newNbColors) {
	PetriNetNode::changeNbOfColors(newNbColors);

	m_tokenByColor.resize(newNbColors);
}

void Place::print() {
	std::cout << "Place : " << this << std::endl;

	std::cout << "Outgoings Arcs : " << std::endl;

	arcList outGoingArcs = outGoingArcsOf();
	for (unsigned int i = 0; i < outGoingArcs.size(); ++i) {
		std::cout << "To : " << outGoingArcs[i]->getTo() << std::endl;
		std::cout << "RelMin : " << outGoingArcs[i]->getRelativeMinValue().getValue() << std::endl;
		std::cout << "RelMax : " << outGoingArcs[i]->getRelativeMaxValue().getValue() << std::endl;
		std::cout << "AbsMin : " << outGoingArcs[i]->getAbsoluteMinValue().getValue() << std::endl;
		std::cout << "AbsMax : " << outGoingArcs[i]->getAbsoluteMaxValue().getValue() << std::endl;

	}

	std::cout << "Ingoings Arcs : " << std::endl;
	arcList inGoingArcs = inGoingArcsOf();
	for (unsigned int i = 0; i < inGoingArcs.size(); ++i) {
		std::cout << "From : " << inGoingArcs[i]->getFrom() << std::endl;
		std::cout << "RelMin : " << inGoingArcs[i]->getRelativeMinValue().getValue() << std::endl;
		std::cout << "RelMax : " << inGoingArcs[i]->getRelativeMaxValue().getValue() << std::endl;
		std::cout << "AbsMin : " << inGoingArcs[i]->getAbsoluteMinValue().getValue() << std::endl;
		std::cout << "AbsMax : " << inGoingArcs[i]->getAbsoluteMaxValue().getValue() << std::endl;
	}
}

Place::~Place() {

}
