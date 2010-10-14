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

#include "PetriNet.hpp"

/*!
 * \file PetriNet.cpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <iostream>
#include <algorithm>

using namespace std;

PetriNet::PetriNet(unsigned int nbColors)
: m_currentTime(0), m_nbColors((nbColors > 0)?nbColors:1), m_previousCallOfMakeOneStepInMs(0)
,m_mustCrossAllTransitionWithoutWaitingEvent(false)
{
	m_parentPetriNet = NULL;

	m_updateFactor = 1;
	m_waitedTriggerPointMessageAction = NULL;
	m_waitedTriggerPointMessageArgument = NULL;

	m_mustStop = false;
	m_isRunning = false;

	m_timeOffset = 0;

	resetEvents();
}

void* mainThreadFunction(void* threadArg)
{
	PetriNet* petriNet = (PetriNet*) threadArg;

	petriNet->m_startPlace->produceTokens(1);
	petriNet->m_endPlace->consumeTokens(petriNet->m_endPlace->nbOfTokens());

	petriNet->addTime(petriNet->getTimeOffset() * 1000);
	//petriNet->makeOneStep();

	while (petriNet->m_endPlace->nbOfTokens() == 0 && !petriNet->m_mustStop) {
		petriNet->update();
	}

	petriNet->m_isRunning = false;

	return NULL;
}

long long PetriNet::computeDt() {
	struct timeval tv;
	struct timezone tz;

	long long dt = 0;

	gettimeofday(&tv, &tz);

	long long currentTimeInMs = tv.tv_sec * 1000000L + tv.tv_usec;

	if (m_previousCallOfMakeOneStepInMs != 0) {
		dt = currentTimeInMs - m_previousCallOfMakeOneStepInMs;
		if (dt < MY_MIN_STEP_TIME_IN_MS) {
			usleep(MY_MIN_STEP_TIME_IN_MS - dt);

			dt = MY_MIN_STEP_TIME_IN_MS;
		}

		m_previousCallOfMakeOneStepInMs += dt;
	} else {
		m_previousCallOfMakeOneStepInMs = currentTimeInMs;
	}

	return dt;
}

void PetriNet::update()
{
	long long dt = computeDt();

	dt *= m_updateFactor;

	addTime(dt);

	makeOneStep();
}

void PetriNet::makeOneStep()
{
	bool stop = false;

	while (!stop) {
		if (m_priorityTransitionsActionQueue.size() == 0) {
			stop = true;
		} else {
			PriorityTransitionAction* topAction = getTopActionOnPriorityQueue();

			if (!topAction->isEnable()) {
				removeTopActionOnPriorityQueue();
			} else if ((unsigned int) topAction->getDate().getValue() > getCurrentTimeInMs()) {
				stop = true;
			} else {
				Transition* topTransition = topAction->getTransition();

				if (topAction->getType() == START) {
					if (topTransition->couldBeSensitize()) {
						turnIntoSensitized(topTransition);

						if (m_waitedTriggerPointMessageAction != NULL) {
							m_waitedTriggerPointMessageAction(m_waitedTriggerPointMessageArgument, true, topTransition);
						}

						removeTopActionOnPriorityQueue();
					} else {
						topAction->setDate(getCurrentTimeInMs() + 1);
						removeTopActionOnPriorityQueue();
						m_priorityTransitionsActionQueue.push(topAction);
					}

					//stop = true;
				} else {
					if (topTransition->areAllInGoingArcsActive()) {
						topTransition->crossTransition(true, getCurrentTimeInMs() - topAction->getDate().getValue());
						removeTopActionOnPriorityQueue();
					} else {
						removeTopActionOnPriorityQueue();
						throw IncoherentStateException();
					}
				}
			}
		}
	}

	for (unsigned int i = 0 ; i < m_sensitizedTransitions.size() ; ++i) {
		Transition* sensitizedTransitionToTestTheEvent;
		sensitizedTransitionToTestTheEvent = m_sensitizedTransitions[i];

		if (!sensitizedTransitionToTestTheEvent->areAllInGoingArcsActive()) {

			m_sensitizedTransitions.erase(m_sensitizedTransitions.begin() + i);
			--i;

			if (m_waitedTriggerPointMessageAction != NULL) {
				m_waitedTriggerPointMessageAction(m_waitedTriggerPointMessageArgument, false, sensitizedTransitionToTestTheEvent);
			}

		} else if (isAnEvent(sensitizedTransitionToTestTheEvent->getEvent()) || m_mustCrossAllTransitionWithoutWaitingEvent){

			if (sensitizedTransitionToTestTheEvent->isStatic()) {
				sensitizedTransitionToTestTheEvent->crossTransition(true, getCurrentTimeInMs() - sensitizedTransitionToTestTheEvent->getStartDate().getValue());
			} else {
				sensitizedTransitionToTestTheEvent->crossTransition(true,0);
			}

			m_sensitizedTransitions.erase(m_sensitizedTransitions.begin() + i);
			--i;

			if (m_waitedTriggerPointMessageAction != NULL) {
				m_waitedTriggerPointMessageAction(m_waitedTriggerPointMessageArgument, false, sensitizedTransitionToTestTheEvent);
			}
		}
	}

	resetEvents();

	while(!m_transitionsToCrossWhenAcceleration.empty()) {
		Transition* currentTransition = m_transitionsToCrossWhenAcceleration.back();

		if (currentTransition->areAllInGoingArcsActive()) {
			currentTransition->crossTransition(false, 0);
		} else {
			throw IncoherentStateException();
		}

		m_transitionsToCrossWhenAcceleration.pop_back();
	}

	m_transitionsToCrossWhenAcceleration.clear();
}

void PetriNet::addTime(unsigned int dt)
{
	m_currentTime += dt;
}

void PetriNet::launch()
{
	m_isRunning = true;
	pthread_create(&m_thread, NULL, mainThreadFunction, this);
}

unsigned int PetriNet::getCurrentTimeInMs()
{
	return m_currentTime / 1000;
}

bool PetriNet::isColorValid(unsigned int color)
{
	return ((color > 0) && (color <= nbOfPossibleColors()));
}

// gets the number of colors in this PetriNet.
unsigned int PetriNet::nbOfPossibleColors()
{
	return m_nbColors;
}

// sets the number of colors in this PetriNet.
void PetriNet::changeNbOfColors(unsigned int newNbColors)
{
	if (newNbColors <= m_nbColors) {
		throw IllegalArgumentException();
	}

	for (unsigned int i = 0; i < m_places.size(); i++) {
		m_places[i]->changeNbOfColors(newNbColors);
	}

	for (unsigned int i = 0; i < m_transitions.size(); i++) {
		m_transitions[i]->changeNbOfColors(newNbColors);
	}

	m_nbColors = newNbColors;
}

void PetriNet::resetEvents()
{
	m_mustCrossAllTransitionWithoutWaitingEvent = false;
	m_incomingEvents.clear();
	m_incomingEvents.push_back(std::string(STATIC_EVENT));
}

void PetriNet::putAnEvent(std::string event)
{
	m_incomingEvents.push_back(event);

	for (std::map<PetriNet*, PetriNet*>::iterator it = m_activeChildPetriNet.begin(); it != m_activeChildPetriNet.end(); ++it)
	{
		it->second->putAnEvent(event);
	}

}

bool PetriNet::isAnEvent(std::string event)
{
	std::list<std::string> tempList = m_incomingEvents.getList();
	std::list<std::string>::iterator it  = tempList.begin();

	while (it != tempList.end()) {
		if (*it == event) {
			return true;
		}
		++it;
	}
	return false;
}

Place* PetriNet::createPlace()
{
	Place* newPlace;

	newPlace = new Place(this);
	m_places.push_back(newPlace);

	return newPlace;
}

Transition* PetriNet::createTransition()
{
	Transition* newTransition;

	newTransition = new Transition(this);
	m_transitions.push_back(newTransition);

	return newTransition;
}

Arc* PetriNet::newArc(PetriNetNode* from, PetriNetNode* to, int color)
{
	Arc* existArc = to->haveArcFrom(from,color);

	if (existArc != NULL) {
		return existArc;
	}

	return new Arc(this, from, to, color);
}

Arc* PetriNet::createArc(Place* from, Transition* to, int color)
{
	return newArc(from, to, color);
}

Arc* PetriNet::createArc(Transition* from, Place* to, int color)
{
	return newArc(from, to, color);
}

void PetriNet::deleteArc(PetriNetNode* from, PetriNetNode* to)
{
	for (unsigned int i = 1; i <= nbOfPossibleColors(); i++) {
		Arc* arcToDelete = to->haveArcFrom(from, i);

		if (arcToDelete != NULL) {
			to->removeInGoingArcs(arcToDelete, i);
			from->removeOutGoingArcs(arcToDelete, i);

			delete arcToDelete;
		}
	}
}

void PetriNet::deleteArc(PetriNetNode* from, PetriNetNode* to, int color)
{
	Arc* arcToDelete = to->haveArcFrom(from, color);

	if (arcToDelete == NULL) {
		return;
	}

	to->removeInGoingArcs(arcToDelete, color);
	from->removeOutGoingArcs(arcToDelete, color);

	delete arcToDelete;
}

void PetriNet::deleteItem(PetriNetNode* nodeToDelete)
{
	petriNetNodeList successorsList = nodeToDelete->returnSuccessors();
	for (unsigned int i = 0; i < successorsList.size(); ++i) {
		deleteArc(nodeToDelete, successorsList[i]);
	}

	petriNetNodeList predecessorsList = nodeToDelete->returnPredecessors();
	for (unsigned int i = 0; i < predecessorsList.size(); ++i) {
		deleteArc(predecessorsList[i], nodeToDelete);
	}
}

void PetriNet::deleteItem(Place* & placeToDelete)
{
	if (placeToDelete == NULL) {
		return;
	}

	int indexToRemove = -1;
	unsigned int i = 0;

	while (indexToRemove == -1 && i < m_places.size()) {
	//for (unsigned int i = 0; i < m_places.size(); i++) {
		if (m_places[i] == placeToDelete) {
			indexToRemove = i;
		}
		++i;
	}

	if (indexToRemove != -1) {
		m_places.erase(m_places.begin() + indexToRemove);
	}

	deleteItem((PetriNetNode*) placeToDelete);

	delete placeToDelete;
	placeToDelete = NULL;

}

void PetriNet::deleteItem(Transition* & transitionToDelete)
{
	if (transitionToDelete == NULL) {
		return;
	}

	int indexToRemove = -1;
	unsigned int i = 0;

	while (indexToRemove == -1 && i < m_transitions.size()) {
	//for (unsigned int i = 0; i < m_transitions.size(); i++) {
		if (m_transitions[i] == transitionToDelete) {
			indexToRemove = i;
		}
		++i;
	}

	if (indexToRemove != -1) {
		m_transitions.erase(m_transitions.begin() + indexToRemove);
	}

	deleteItem((PetriNetNode*) transitionToDelete);

	delete transitionToDelete;
	transitionToDelete = NULL;
}

void PetriNet::turnIntoSensitized(Transition* t)
{
	m_sensitizedTransitions.push_back(t);
}
//
//void PetriNet::turnIntoUnsensitized(Transition* t)
//{
//	if (t->hasATokenInPredecessors()) {
//		m_activeTransitions.push_back(t);
//	}
//}

placeList PetriNet::getPlaces()
{
	return m_places;
}

void PetriNet::setStartPlace(Place* place)
{
	m_startPlace = place;
}

Place* PetriNet::getStartPlace()
{
	return m_startPlace;
}

void PetriNet::setEndPlace(Place* place)
{
	m_endPlace = place;
}

Place* PetriNet::getEndPlace()
{
	return m_endPlace;
}

transitionList PetriNet::getTransitions()
{
	return m_transitions;
}

//transitionList PetriNet::getActiveTransitions() {
//	return m_activeTransitions;
//}

transitionList PetriNet::getSensitizedTransitions()
{
	return m_sensitizedTransitions;
}

stringList PetriNet::getEvents()
{
	return m_incomingEvents.getList();
}

bool PetriNet::isRunning()
{
	return m_isRunning;
}

void PetriNet::mustStop()
{
	m_mustStop = true;
}

void PetriNet::addActionToPriorityQueue(PriorityTransitionAction* action)
{
	m_priorityTransitionsActionQueue.push(action);
}

priorityTransitionActionQueue PetriNet::getPriorityQueue()
{
	return m_priorityTransitionsActionQueue;
}

PriorityTransitionAction* PetriNet::getTopActionOnPriorityQueue()
{
	if (isEmptyPriorityQueue()) {
		throw IncoherentStateException();
	}

	return m_priorityTransitionsActionQueue.top();
}

void PetriNet::removeTopActionOnPriorityQueue()
{
	if (isEmptyPriorityQueue()) {
		throw IncoherentStateException();
	}

	m_priorityTransitionsActionQueue.pop();
}

bool PetriNet::isEmptyPriorityQueue()
{
	return m_priorityTransitionsActionQueue.empty();
}

void PetriNet::setUpdateFactor(float updateFactor)
{
	m_updateFactor = updateFactor;

	std::map<PetriNet*, PetriNet*>::iterator it;

	for (it = m_activeChildPetriNet.begin(); it != m_activeChildPetriNet.end(); ++it) {
		it->second->setUpdateFactor(updateFactor);
	}
}

float PetriNet::getUpdateFactor()
{
	return m_updateFactor;
}

void PetriNet::setTimeOffset(unsigned int timeOffset)
{
	m_timeOffset = timeOffset;
}

unsigned int PetriNet::getTimeOffset()
{
	return m_timeOffset;
}

void PetriNet::ignoreEventsForOneStep()
{
	m_mustCrossAllTransitionWithoutWaitingEvent = true;
}

void PetriNet::addWaitedTriggerPointMessageAction(void* arg, void(*pt2Func)(void*, bool, Transition*))
{
	m_waitedTriggerPointMessageAction = pt2Func;
	m_waitedTriggerPointMessageArgument = arg;
}

void PetriNet::removeWaitedTriggerPointMessageAction()
{
	m_waitedTriggerPointMessageAction = NULL;
	m_waitedTriggerPointMessageArgument = NULL;
}

void PetriNet::pushTransitionToCrossWhenAcceleration(Transition* t)
{
	transitionList::iterator it;

	it = std::find(m_transitionsToCrossWhenAcceleration.begin(), m_transitionsToCrossWhenAcceleration.end(), t);

	if (it != m_transitionsToCrossWhenAcceleration.end()) {
		m_transitionsToCrossWhenAcceleration.push_back(t);
	}
}

void PetriNet::addInternPetriNet(Transition* startTransition, Transition* endTransition, PetriNet* petriNet)
{
	petriNet->m_parentPetriNet = this;
	m_childrenPetriNet[petriNet] = petriNet;

	startTransition->addExternAction(&externLaunch, petriNet);
	endTransition->addExternAction(&externMustStop, petriNet);

	if (m_waitedTriggerPointMessageAction != NULL) {
		petriNet->addWaitedTriggerPointMessageAction(m_waitedTriggerPointMessageArgument, m_waitedTriggerPointMessageAction);
	}
}

void PetriNet::print()
{
	std::cout << "PETRI NET" << std::endl;

	std::cout << "TransitionList elements " << m_sensitizedTransitions.size() << std::endl;
	std::cout << "PriorityQueu elements " << m_priorityTransitionsActionQueue.size() << std::endl;

	std::cout << std::endl;
	for (unsigned int i = 0; i < m_places.size(); ++i) {
		m_places[i]->print();
		std::cout << std::endl;
	}

	for (unsigned int i = 0; i < m_transitions.size(); ++i) {
		m_transitions[i]->print();
		std::cout << std::endl;
	}

	std::cout << "********" << std::endl;
}


PetriNet::~PetriNet()
{
	for (unsigned int i = 0; i < m_places.size(); ++i) {
		if (m_places[i] != NULL) {
			delete m_places[i];
		}
	}

	for (unsigned int i = 0; i < m_transitions.size(); ++i) {
		if (m_transitions[i] != NULL) {
			delete m_transitions[i];
		}
	}

//	while (!m_transitions.empty()) {
//		deleteItem(m_transitions[0]);
//	}
}

void externLaunch(void* arg)
{
	PetriNet* petriNet = (PetriNet*) arg;

	if (petriNet->m_parentPetriNet != NULL) {
		petriNet->m_parentPetriNet->m_activeChildPetriNet[petriNet] = petriNet;
		petriNet->setUpdateFactor(petriNet->m_parentPetriNet->getUpdateFactor());
	}

	petriNet->launch();
}

void externMustStop(void* arg)
{
	PetriNet* petriNet = (PetriNet*) arg;

	if (petriNet->m_parentPetriNet != NULL) {
		petriNet->m_parentPetriNet->m_activeChildPetriNet.erase(petriNet);
	}

	petriNet->mustStop();
}
