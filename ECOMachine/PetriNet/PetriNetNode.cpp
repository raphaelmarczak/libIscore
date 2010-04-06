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

#include "PetriNetNode.hpp"

/*!
 * \file PetriNetNode.cpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "PetriNet.hpp"

#include <set>

using namespace std;

PetriNetNode::PetriNetNode() {
}

PetriNetNode::PetriNetNode(PetriNet* petriNet)
:PetriNetItem(petriNet) {
	int nbColors = getPetriNet()->nbOfPossibleColors();

	//m_inGoingArcs.assign(nbColors, std::vector<Arc*>(NULL));
	m_inGoingArcs.assign(nbColors, arcList(NULL));
	m_outGoingArcs.assign(nbColors, arcList(NULL));
}

//TODO : regarder le comportement de back_inserter;
arcList PetriNetNode::inGoingArcsOf() {
	arcList resultArcList;

	for (unsigned int i = 0; i < nbOfPossibleColors(); i++) {
		resultArcList.insert(resultArcList.end(), m_inGoingArcs[i].begin(), m_inGoingArcs[i].end());
	}

	return resultArcList;
}

arcList PetriNetNode::inGoingArcsOf(int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	return m_inGoingArcs[colorLabel - 1];
}

//TODO : regarder le comportement de back_inserter;
arcList PetriNetNode::outGoingArcsOf() {
	arcList resultArcList;

	for (unsigned int i = 0; i < nbOfPossibleColors(); i++) {
		resultArcList.insert(resultArcList.end(), m_outGoingArcs[i].begin(), m_outGoingArcs[i].end());
	}

	return resultArcList;
}

arcList PetriNetNode::outGoingArcsOf(int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	return m_outGoingArcs[colorLabel - 1];
}

void PetriNetNode::setInGoingArcs(arcList list, int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	m_inGoingArcs[colorLabel - 1] = list;
}

void PetriNetNode::setOutGoingArcs(arcList list, int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	m_outGoingArcs[colorLabel - 1] = list;
}

void PetriNetNode::addInGoingArcs(Arc* arcToAdd, int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	colorLabel--;

	m_inGoingArcs[colorLabel].push_back(arcToAdd);
}

void PetriNetNode::addOutGoingArcs(Arc* arcToAdd, int colorLabel) {
	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	colorLabel--;

	m_outGoingArcs[colorLabel].push_back(arcToAdd);
}

void PetriNetNode::removeInGoingArcs(Arc* arcToRemove) {
	for (unsigned int i = 1; i <= nbOfPossibleColors(); i++) {
		removeInGoingArcs(arcToRemove, i);
	}
}

void PetriNetNode::removeOutGoingArcs(Arc* arcToRemove) {
	for (unsigned int i = 1; i <= nbOfPossibleColors(); i++) {
		removeOutGoingArcs(arcToRemove, i);
	}
}

void PetriNetNode::removeInGoingArcs(Arc* arcToRemove, int colorLabel) {
	int indexToRemove = -1;

	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	colorLabel --;

	for (unsigned int i = 0; i < m_inGoingArcs[colorLabel].size(); i++) {
		if (m_inGoingArcs[colorLabel][i] == arcToRemove) {
			indexToRemove = i;
		}
	}

	if (indexToRemove != -1) {
		m_inGoingArcs[colorLabel].erase(m_inGoingArcs[colorLabel].begin() + indexToRemove);
	}
}

void PetriNetNode::removeOutGoingArcs(Arc* arcToRemove, int colorLabel) {
	int indexToRemove = -1;

	if (!isColorValid(colorLabel)) {
		throw OutOfBoundException();
	}

	colorLabel --;

	for (unsigned int i = 0; i < m_outGoingArcs[colorLabel].size(); i++) {
		if (m_outGoingArcs[colorLabel][i] == arcToRemove) {
			indexToRemove = i;
		}
	}

	if (indexToRemove != -1) {
		m_outGoingArcs[colorLabel].erase(m_outGoingArcs[colorLabel].begin() + indexToRemove);
	}
}

Arc* haveArc(PetriNetNode* from, PetriNetNode* to, int colorLabel) {
	arcList outGoingsArcs = from->outGoingArcsOf(colorLabel);

	for (unsigned int i = 0; i < outGoingsArcs.size(); i++) {
		if (outGoingsArcs[i]->getTo() == to) {
			return outGoingsArcs[i];
		}
	}

	return NULL;
}

Arc* haveArc(PetriNetNode* from, PetriNetNode* to) {
	arcList outGoingsArcs = from->outGoingArcsOf();

	for (unsigned int i = 0; i < outGoingsArcs.size(); i++) {
		if (outGoingsArcs[i]->getTo() == to) {
			return outGoingsArcs[i];
		}
	}

	return NULL;
}

Arc* PetriNetNode::haveArcFrom(PetriNetNode* from) {
	return haveArc(from, this);
}

Arc* PetriNetNode::haveArcFrom(PetriNetNode* from, int colorLabel) {
	return haveArc(from, this, colorLabel);
}

Arc* PetriNetNode::haveArcTo(PetriNetNode* to) {
	return haveArc(this, to);
}

Arc* PetriNetNode::haveArcTo(PetriNetNode* to, int colorLabel) {
	return haveArc(this, to, colorLabel);
}

// TODO : regarder l'utilisation de unique !
petriNetNodeList PetriNetNode::returnSuccessors() {
	arcList outGoingArcs = outGoingArcsOf();

	set<PetriNetNode*> petriNetSet;

	for (unsigned int i = 0; i < outGoingArcs.size(); i++) {
		petriNetSet.insert(outGoingArcs[i]->getTo());
	}

	petriNetNodeList result(petriNetSet.begin(), petriNetSet.end());

	return result;
}

// TODO : regarder l'utilisation de unique !
petriNetNodeList PetriNetNode::returnSuccessors(int colorLabel) {
	arcList outGoingArcs = outGoingArcsOf(colorLabel);

	set<PetriNetNode*> petriNetSet;

	for (unsigned int i = 0; i < outGoingArcs.size() ; i++) {
		petriNetSet.insert(outGoingArcs[i]->getTo());
	}

	petriNetNodeList result(petriNetSet.begin(), petriNetSet.end());

	return result;
}

// TODO : regarder l'utilisation de unique !
petriNetNodeList PetriNetNode::returnPredecessors() {
	arcList inGoingArcs = inGoingArcsOf();

	set<PetriNetNode*> petriNetSet;

	for (unsigned int i = 0; i < inGoingArcs.size() ; i++) {
		petriNetSet.insert(inGoingArcs[i]->getFrom());
	}

	petriNetNodeList result(petriNetSet.begin(), petriNetSet.end());

	return result;
}

// TODO : regarder l'utilisation de unique !
petriNetNodeList PetriNetNode::returnPredecessors(int colorLabel) {
	arcList inGoingArcs = inGoingArcsOf(colorLabel);

	set<PetriNetNode*> petriNetSet;

	for (unsigned int i = 0; i < inGoingArcs.size() ; i++) {
		petriNetSet.insert(inGoingArcs[i]->getFrom());
	}

	petriNetNodeList result(petriNetSet.begin(), petriNetSet.end());

	return result;
}

void PetriNetNode::changeNbOfColors(int newNbColors) {
	m_inGoingArcs.resize(newNbColors, arcList(NULL));
	m_outGoingArcs.resize(newNbColors, arcList(NULL));
}

PetriNetNode::~PetriNetNode() {
	//delete[] m_inGoingArcs;
	//delete[] m_outGoingArcs;
}
