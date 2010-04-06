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

#include "Transition.hpp"

/*!
 * \file Transition.cpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "PetriNet.hpp"

#include <set>

using namespace std;


Transition::Transition(PetriNet* petriNet)
:PetriNetNode(petriNet), m_activeArcsBitArray(NULL), m_startDate(MINUS_INFINITY),
m_endDate(PLUS_INFINITY), m_startAction(NULL), m_endAction(NULL)
{
	m_events.push_back(STATIC_EVENT);
}

bool Transition::isStatic()
{
	return ((m_events.size() > 0) && (m_events.back() == STATIC_EVENT));
}

void Transition::setEvent(std::string s)
{
	m_events.clear();
	m_events.push_back(s);
}

void Transition::createBitArray()
{
	if (m_activeArcsBitArray != NULL) {
		delete m_activeArcsBitArray;
		m_activeArcsBitArray = NULL;
	}

	arcList inGoingArcs = inGoingArcsOf();

	if (inGoingArcs.size() != 0) {
		m_activeArcsBitArray = new TransitionBitArray(inGoingArcs.size());

		for (unsigned int i = 0; i < inGoingArcs.size(); ++i) {
			inGoingArcs[i]->setNumber(i);
		}
	}
}

void Transition::resetBitArray()
{
	for (unsigned int i = 0 ; i < m_activeArcsBitArray->getSize() ; ++i) {
		m_activeArcsBitArray->setToZero(i);
	}

	if (m_startAction != NULL) {
		m_startAction->disable();
		m_startAction = NULL;
	}

	m_startDate = MINUS_INFINITY;

	if (m_endAction != NULL) {
		m_endAction->disable();
		m_endAction = NULL;
	}

	m_endDate = PLUS_INFINITY;
}

void Transition::resetIncomingArcsState()
{
	arcList inGoingArcs = inGoingArcsOf();

	for (unsigned int i = 0; i < inGoingArcs.size(); ++i) {
		if(inGoingArcs[i]->isActive()) {
			setArcAsActive(inGoingArcs[i], 0, false);
		}
	}
}

void Transition::merge(Transition* transitionToMerge)
{
	for (unsigned int i = 0 ; i < transitionToMerge->m_externActions.size() ; ++i) {
		m_externActions.push_back(transitionToMerge->m_externActions[i]);
	}
	
	arcList mergeInGoingArcs = transitionToMerge->inGoingArcsOf();
	arcList mergeOutGoingArcs = transitionToMerge->outGoingArcsOf();
	
	for (unsigned int i = 0; i < mergeInGoingArcs.size() ; ++i) {
		Arc* currentArc = mergeInGoingArcs[i];
		
		ExtendedInt relativeMinValue = currentArc->getRelativeMinValue();
		ExtendedInt relativeMaxValue = currentArc->getRelativeMaxValue();
		ExtendedInt absoluteMinValue = currentArc->getAbsoluteMinValue();
		ExtendedInt absoluteMaxValue = currentArc->getAbsoluteMaxValue();
		
		Place* inGoingPlace = (Place*) currentArc->getFrom();
		
		Arc* newArc = getPetriNet()->createArc(inGoingPlace, this);
		newArc->changeAbsoluteTime(absoluteMinValue, absoluteMaxValue);
		newArc->changeRelativeTime(relativeMinValue, relativeMaxValue);
		
		getPetriNet()->deleteArc(inGoingPlace, transitionToMerge);
	}
	
	createBitArray();
	
	for (unsigned int i = 0; i < mergeOutGoingArcs.size() ; ++i) {
		Arc* currentArc = mergeOutGoingArcs[i];
		
		Place* outGoingPlace = (Place*) currentArc->getTo();
		getPetriNet()->createArc(this, outGoingPlace);
		
		getPetriNet()->deleteArc(transitionToMerge, outGoingPlace);
	}
}

void Transition::addExternAction(void(*pt2Func)(void*), void* argPt2Func)
{
	m_externActions.push_back(new TransitionAction(pt2Func, argPt2Func));
//	m_externAction = pt2Func;
//	m_externActionArgument = argPt2Func;
}

void Transition::removeExternActions()
{
	m_externActions.clear();
}

void Transition::setArcAsActiveByNumber(int arcNumber)
{
	m_activeArcsBitArray->setToOne(arcNumber);
}

void Transition::setArcAsInactiveByNumber(int arcNumber)
{
	m_activeArcsBitArray->setToZero(arcNumber);
}

void Transition::setArcAsActive(Arc* arc, unsigned int timeOffset, bool recalculateArcTime)
{
	setArcAsActiveByNumber(arc->getNumber());

	unsigned int currentTime = getPetriNet()->getCurrentTimeInMs();
	ExtendedInt startDate;
	ExtendedInt endDate;

	if (recalculateArcTime) {
		startDate = arc->getRelativeMinValue() - timeOffset + currentTime;
		endDate = arc->getRelativeMaxValue() - timeOffset + currentTime;

		arc->changeAbsoluteTime(startDate, endDate);
	} else {
		startDate = arc->getAbsoluteMinValue() - timeOffset;
		endDate = arc->getAbsoluteMaxValue() - timeOffset;
	}

	if (startDate > m_startDate) {
		m_startDate = startDate;
	}

	if (endDate < m_endDate) {
		m_endDate = endDate;

		if (m_endAction != NULL) {
			m_endAction->disable();
		}

		m_endAction = new PriorityTransitionAction(this, END, m_endDate);

		getPetriNet()->addActionToPriorityQueue(m_endAction);
	}

	if (areAllInGoingArcsActive()) {
		m_startAction = new PriorityTransitionAction(this, START, m_startDate);
		getPetriNet()->addActionToPriorityQueue(m_startAction);
	}
}

void Transition::setArcAsInactive(Arc* arc)
{
	if (areAllInGoingArcsActive()) {
		m_startAction->disable();
	}
	setArcAsInactiveByNumber(arc->getNumber());


}

bool Transition::areAllInGoingArcsActive()
{
	if (m_activeArcsBitArray == NULL) {
		throw IncoherentStateException();
	}

	return m_activeArcsBitArray->isAllOne();
}

string Transition::getEvent()
{
	return m_events.back();
}

void Transition::turnIntoStatic()
{
	if (m_events.size() <= 1 && !isStatic()) {
		m_events.push_back(STATIC_EVENT);
	}
}

void Transition::turnIntoOriginalStatus()
{
	if (m_events.size() > 1) {
		m_events.pop_back();
	}
}

//bool Transition::isPureAndOnMinLimit()
//{
//	arcList list = inGoingArcsOf();
//
//	for (unsigned int i = 0; i < list.size(); i++) {
//		if (list[i]->haveReachedMinLimit()) {
//			return true;
//		}
//	}
//
//	return false;
//}
//
//Arc* Transition::isPureAndOnMaxLimit()
//{
//	for (unsigned int i = 0; i < m_activeArcs.size(); i++) {
//		if (m_activeArcs[i]->haveReachedMaxLimit()) {
//			return m_activeArcs[i];
//		}
//	}
//
//	return NULL;
//}

void Transition::crossTransition(bool mustChangeTokenValue, unsigned int newTokenValue)
{
	if (!areAllInGoingArcsActive()) {
		throw IncoherentStateException();
	}

	arcList inGoingArc = inGoingArcsOf();
	arcList outGoingArc = outGoingArcsOf();

	resetBitArray();

	set<Transition*>  transitionsToReset;

	unsigned int tokenValue = 0;

	for (unsigned int i = 0 ; i < inGoingArc.size() ; ++i) {
		tokenValue = inGoingArc[i]->consumeTokenInFrom();

		if (isStatic()) {
			tokenValue -= inGoingArc[i]->getRelativeMinValue().getValue();
		} else {
			tokenValue -= inGoingArc[i]->getRelativeMaxValue().getValue();
		}

		if (inGoingArc[i]->haveEnoughTokensInFrom()) {
			setArcAsActive(inGoingArc[i], 0, false);
		} else {
			Place* place = (Place*) inGoingArc[i]->getFrom();

			petriNetNodeList nodeList = place->returnSuccessors();

			if (nodeList.size() > 1) {
				for (unsigned int i = 0 ; i < nodeList.size() ; ++i) {
					if (nodeList[i] != this) {
						transitionsToReset.insert((Transition*) nodeList[i]);
					}
				}
			}

		}
	}

	if (mustChangeTokenValue) {
		tokenValue = newTokenValue;
	}

	transitionList transitionsToResetList(transitionsToReset.begin(), transitionsToReset.end());

	for (unsigned int i = 0 ; i < transitionsToResetList.size() ; ++i) {
		transitionsToResetList[i]->resetBitArray();
		transitionsToResetList[i]->resetIncomingArcsState();
	}

	if (m_externActions.size() > 0) {
		for (unsigned int i = 0 ; i < m_externActions.size() ; ++i) {
			m_externActions[i]->m_transitionAction(m_externActions[i]->m_transitionActionArgument);
		}
	}

	for (unsigned int i = 0 ; i < outGoingArc.size() ; ++i) {
		outGoingArc[i]->produceTokenInTo(tokenValue);
	}
}

Arc* Transition::hasATokenInPredecessors()
{
	arcList list = inGoingArcsOf();

		for (unsigned int i = 0; i < list.size(); i++) {
			if (list[i]->nbOfArcColorLabelTokensInFrom() > 0) {
				return list[i];
			}
		}

	return NULL;
}

void Transition::print() {
	std::cout << "Transition : " << this << std::endl;

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

ExtendedInt Transition::getStartDate()
{
	return m_startDate;
}

ExtendedInt Transition::getEndDate()
{
	return m_endDate;
}

Transition::~Transition()
{
}
