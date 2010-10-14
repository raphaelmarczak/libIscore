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

#include "ECOMachine.hpp"

/*!
 * \file ECOMachine.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include <stdio.h>
#include <stdlib.h>

#include <sstream>
#include <algorithm>

#include "ChangeTempo.hpp"
#include "PetriNet/PetriNet.hpp"

using namespace std;


void* ECOMachineMainFunction(void* threadArg)
{
	ECOMachine* ECO = (ECOMachine*) threadArg;

	ECO->m_petriNet->launch();

	while (ECO->m_petriNet->isRunning()) {
		usleep(50);
	}

	if (ECO->m_isEcoMachineFinished != NULL) {
		ECO->m_isEcoMachineFinished(ECO->m_isEcoMachineFinishedArg);
	}

	return NULL;
}


void crossAControlPointCallBack(void* arg)
{
	ControlPointInformations* controlPointInformations = (ControlPointInformations*) arg;

	if (controlPointInformations->m_crossAControlPointAction != NULL) {
		//std::cout << "CP : " << controlPointInformations->m_boxId << " " << controlPointInformations->m_controlPointIndex << std::endl;
		controlPointInformations->m_crossAControlPointAction(controlPointInformations->m_boxId, controlPointInformations->m_controlPointIndex, controlPointInformations->m_processIdToStop);
	}

	for (unsigned int i = 0; i < controlPointInformations->m_processIdToStop.size(); ++i) {
	//	std::cout << "Process " << controlPointInformations->m_processIdToStop[i] << " stopped" << std::endl;
		controlPointInformations->m_ECOMachine->m_process[controlPointInformations->m_processIdToStop[i]]->changeState(ECOProcess::STOP);
	}

}

void waitedTriggerPointMessageCallBack(void* arg, bool isWaited, Transition* transition)
{
	ECOMachine* currentECOMachine = (ECOMachine*) arg;

	if (currentECOMachine->hasTriggerPointInformations(transition)) {
		TriggerPointInformations currentTriggerPointInformations = currentECOMachine->getTriggerPointInformations(transition);

		if (currentTriggerPointInformations.m_waitedTriggerPointMessageAction != NULL) {
			currentTriggerPointInformations.m_waitedTriggerPointMessageAction(currentECOMachine->m_waitedTriggerPointMessageArg,
																			  isWaited,
																			  currentTriggerPointInformations.m_triggerId,
																			  currentTriggerPointInformations.m_boxId,
																			  currentTriggerPointInformations.m_controlPointIndex,
																			  currentTriggerPointInformations.m_waitedString);

		}
	}
}


ECOMachine::ECOMachine()
:m_lastEventNumber(1)
{
	m_petriNet = new PetriNet();

	m_gotoValue = 0;
	m_currentSpeedFactor = 1;

	//m_receiveOSCThread = new ReceiveOSCThread(this);
	m_crossAControlPointAction = NULL;
	m_waitedTriggerPointMessageAction = NULL;
	m_isEcoMachineFinished = NULL;

	m_waitedTriggerPointMessageArg = NULL;

	m_petriNet->addWaitedTriggerPointMessageAction(this, &waitedTriggerPointMessageCallBack);
}

ECOMachine::~ECOMachine()
{
	delete m_petriNet;
}

std::string ECOMachine::addNetworkMessage(std::string netMessage)
{
	map<string, string>::iterator iter = m_fromNetworkMessagesToPetriMessages.find(netMessage);

	if(iter == m_fromNetworkMessagesToPetriMessages.end()) {
		ostringstream oss;
	    oss << EVENT_NAME;
	    oss << m_lastEventNumber;

		m_fromNetworkMessagesToPetriMessages[netMessage] = oss.str();
		++m_lastEventNumber;

	}
	return m_fromNetworkMessagesToPetriMessages[netMessage];
}

bool ECOMachine::receiveNetworkMessage(std::string netMessage)
{
	map<string, string>::iterator iter = m_fromNetworkMessagesToPetriMessages.find(netMessage);

	if(iter == m_fromNetworkMessagesToPetriMessages.end()) {
		return false;
	} else {
		if (getPetriNet()->getUpdateFactor() != 0) {
			m_petriNet->putAnEvent(m_fromNetworkMessagesToPetriMessages[netMessage]);
		}
		return true;
	}

}

void ECOMachine::newProcess(std::string type, unsigned int processId, DeviceManager* controller)
{
	if (type == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND) {
		m_process[processId] = new SendNetworkMessageProcess(processId, controller);
		m_process[processId]->setType(type);
	}
}

ECOProcess* ECOMachine::getProcess(unsigned int processId)
{
	map<unsigned int, ECOProcess*>::iterator iter = m_process.find(processId);

	if(iter == m_process.end())  {
		throw OutOfBoundException();
	}

	return m_process[processId];
}

void ECOMachine::removeProcess(unsigned int processId)
{
	m_process.erase(processId);
}

unsigned int ECOMachine::getTimeInMs()
{
	return m_petriNet->getCurrentTimeInMs();
}

void ECOMachine::ignoreTriggerPointForOneStep()
{
	if (m_petriNet->getUpdateFactor() != 0) {
		m_petriNet->ignoreEventsForOneStep();
	}
}


PetriNet* ECOMachine::getPetriNet()
{
	return m_petriNet;
}

void ECOMachine::reset() {
	if (m_petriNet != NULL) {
		if (isRunning()) {
			stop();

			while (isRunning()) {
				usleep(50);
			}
		}

		delete m_petriNet;

		m_petriNet = new PetriNet();

		if (waitedTriggerPointMessageCallBack != NULL) {
			m_petriNet->addWaitedTriggerPointMessageAction(this, &waitedTriggerPointMessageCallBack);
		}
	}
}

void ECOMachine::compileECO(std::map<unsigned int, StoryLine> hierarchyStoryLine,unsigned int timeToStartTriggerPoint)
{
	if (m_petriNet != NULL) {
		delete m_petriNet;
		m_petriNet = NULL;
	}

	std::vector<unsigned int> processIdToStop;

	m_transitionToTriggerPointInformations.clear();
	m_petriNet = compilePetriNet(hierarchyStoryLine[ROOT_BOX_ID], hierarchyStoryLine, processIdToStop, timeToStartTriggerPoint, &waitedTriggerPointMessageCallBack);

//	if (waitedTriggerPointMessageCallBack != NULL) {
//		m_petriNet->addWaitedTriggerPointMessageAction(this, &waitedTriggerPointMessageCallBack);
//	}
}

PetriNet* ECOMachine::compilePetriNet(StoryLine storyLineToCompile,
									  std::map<unsigned int, StoryLine> hierarchyStoryLine,
									  std::vector<unsigned int>& processIdToStop,
									  unsigned int timeToStartTriggerPoint,
									  void(*triggerAction)(void*, bool, Transition*))
{
	PetriNet* petriNet = new PetriNet();

	if (triggerAction != NULL) {
		petriNet->addWaitedTriggerPointMessageAction(this, triggerAction);
	}

	ExtendedInt plusInfinity(PLUS_INFINITY, 0);
	ExtendedInt minusInfinity(MINUS_INFINITY, 0);
	ExtendedInt integer0(INTEGER, 0);

	map<ControlPoint*, Transition*> controlPointToTransitionConversion;
	map<Arc*, AntPostRelation*> arcToRelationConversion;

	/********** FIRST COMPILATION PHASE **********
	 * Relations are not taken into account.
	 * All boxes are statically compiled.
	 *********************************************/
	Place* startPlace = petriNet->createPlace();
	Transition* startTransition = petriNet->createTransition();
	Arc* startArc = petriNet->createArc(startPlace, startTransition);

	Place* endPlace = petriNet->createPlace();
	Transition* endTransition = petriNet->createTransition();
	Arc* endArc = petriNet->createArc(endTransition, endPlace);

	petriNet->setStartPlace(startPlace);
	petriNet->setEndPlace(endPlace);

	startArc->changeRelativeTime(integer0, plusInfinity);
	endArc->changeRelativeTime(integer0, plusInfinity);

	for (unsigned int i = 0 ; i < storyLineToCompile.m_constrainedBoxes.size() ; ++i) {
		Transition* previousTransition = startTransition;
		ConstrainedBox* currentBox = storyLineToCompile.m_constrainedBoxes[i];

		unsigned int currentBoxId = currentBox->getId();

		Transition* currentTransition;
		Place* currentPlace;

		ECOProcess* currentProcess = m_process[currentBoxId];
		currentProcess->init();

		unsigned int boxEndTime = currentBox->beginValue() + currentBox->lengthValue();

		if (((unsigned int) currentBox->beginValue() < timeToStartTriggerPoint) && (boxEndTime > timeToStartTriggerPoint)) {
			currentProcess->setTimeOffsetInMs(timeToStartTriggerPoint - (currentBox->beginValue()));
		}

		vector<unsigned int>* controlPointID = new vector <unsigned int>;
		currentBox->getAllControlPointsId(controlPointID);

		Transition* firstTransition = NULL;
		Transition* lastTransition = NULL;

		for (unsigned int j = 0 ; j < controlPointID->size() ; ++j) {
			currentTransition = petriNet->createTransition();
			currentPlace = petriNet->createPlace();

			Arc* arcFromCurrentPlaceToCurrentTransition = petriNet->createArc(currentPlace, currentTransition);
			Arc* arcFromPreviousTransitionToCurrentPlace = petriNet->createArc(previousTransition, currentPlace);

			(void) arcFromPreviousTransitionToCurrentPlace;

			ExtendedInt controlPointValue;

			if (controlPointID->at(j) == BEGIN_CONTROL_POINT_INDEX) {
				controlPointValue.setAsInteger(currentBox->getControlPoint(controlPointID->at(j))->beginValue());
			} else {
				controlPointValue.setAsInteger(currentBox->getControlPoint(controlPointID->at(j))->beginValue()
											   - currentBox->getControlPoint(controlPointID->at(j-1))->beginValue());
			}
			arcFromCurrentPlaceToCurrentTransition->changeRelativeTime(controlPointValue, plusInfinity);

			ProcessStep* currentProcessStep = new ProcessStep();

			currentProcessStep->m_step = currentBox->getControlPoint(controlPointID->at(j))->getProcessStepId();
			currentProcessStep->m_process = currentProcess;

			currentTransition->addExternAction(&processCallBack, currentProcessStep);

			ControlPointInformations* currentControlPointInformations = new ControlPointInformations();
			currentControlPointInformations->m_ECOMachine = this;
			currentControlPointInformations->m_controlPointIndex = controlPointID->at(j);
			currentControlPointInformations->m_boxId = currentBox->getControlPoint(controlPointID->at(j))->getContainingBoxId();
			currentControlPointInformations->m_crossAControlPointAction = m_crossAControlPointAction;

			if (j == 0) {
				firstTransition = currentTransition;
			} else if (j == (controlPointID->size() - 1)) {
				lastTransition = currentTransition;

				if (hierarchyStoryLine.find(currentBoxId) != hierarchyStoryLine.end()) {
					std::vector<unsigned int> currentProcessIdToStop;

					unsigned int currentTime = (timeToStartTriggerPoint > (unsigned int) currentBox->beginValue())?(timeToStartTriggerPoint - currentBox->beginValue()):0;
					PetriNet* currentPetriNet = compilePetriNet(hierarchyStoryLine[currentBoxId], hierarchyStoryLine, currentProcessIdToStop, currentTime, triggerAction);

					petriNet->addInternPetriNet(firstTransition, lastTransition, currentPetriNet);

					lastTransition->setPetriNetToEnd(currentPetriNet);

					if ((currentBox->beginValue() + currentBox->lengthValue()) >= timeToStartTriggerPoint) {
						lastTransition->setMustWaitThePetriNetToEnd(true);

						arcList incomingArcs = currentTransition->inGoingArcsOf();
						for (unsigned int k = 0; k < incomingArcs.size(); ++k) {
							incomingArcs[k]->changeRelativeTime(incomingArcs[k]->getRelativeMinValue(), plusInfinity);
						}
					}

					currentControlPointInformations->m_processIdToStop = currentProcessIdToStop;

					for (unsigned int k = 0; k < currentProcessIdToStop.size(); ++k) {
						processIdToStop.push_back(currentProcessIdToStop[k]);
					}
				}


			}

			currentTransition->addExternAction(&crossAControlPointCallBack, currentControlPointInformations);

			controlPointToTransitionConversion[currentBox->getControlPoint(controlPointID->at(j))] = currentTransition;

			previousTransition = currentTransition;
		}

		currentPlace = petriNet->createPlace();
		Arc* arcFromPreviousTransitionToCurrentPlace = petriNet->createArc(previousTransition, currentPlace);
		Arc* arcFromCurrentPlaceToTheEnd = petriNet->createArc(currentPlace, endTransition);

		(void) arcFromPreviousTransitionToCurrentPlace;

		arcFromCurrentPlaceToTheEnd->changeRelativeTime(integer0, plusInfinity);

		currentProcess->setWrittenTime(currentBox->lengthValue());

		processIdToStop.push_back(currentBoxId);

		if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
			SendNetworkMessageProcess* currentOSCProcess = (SendNetworkMessageProcess*) currentProcess;

			ControlPoint* firstControlPoint = currentBox->getControlPoint(BEGIN_CONTROL_POINT_INDEX);
			ControlPoint* secondControlPoint = currentBox->getControlPoint(END_CONTROL_POINT_INDEX);

			currentOSCProcess->computeCurves(BEGIN_CONTROL_POINT_INDEX,END_CONTROL_POINT_INDEX,
					secondControlPoint->beginValue() - firstControlPoint->beginValue());
		}


		/********/
	}

	std::map<Transition*, Transition*> mergeTransitions;
	std::map<Transition*, Transition*>::iterator mergeIterator;

	/********** SECOND COMPILATION PHASE **********
	 * Relations are added.
	 **********************************************/
	for (unsigned int i = 0 ; i < storyLineToCompile.m_antPostRelations.size() ; ++i) {
		ControlPoint* firstControlPoint = NULL;
		ControlPoint* secondControlPoint = NULL;

		AntPostRelation* currentRelation = storyLineToCompile.m_antPostRelations[i];

		if (currentRelation->antPostType() == ANTPOST_ANTERIORITY) {
			firstControlPoint = (ControlPoint*) currentRelation->entity1();
			secondControlPoint = (ControlPoint*) currentRelation->entity2();
		} else if (currentRelation->antPostType() == ANTPOST_POSTERIORITY) {
			firstControlPoint = (ControlPoint*) currentRelation->entity2();
			secondControlPoint = (ControlPoint*) currentRelation->entity1();
		}

		if (firstControlPoint->getContainingBoxId() != secondControlPoint->getContainingBoxId()) {
			Transition* firstTransition = controlPointToTransitionConversion[firstControlPoint];
			Transition* secondTransition = controlPointToTransitionConversion[secondControlPoint];

			mergeIterator = mergeTransitions.find(firstTransition);

			if (mergeIterator != mergeTransitions.end()) {
				firstTransition = mergeIterator->second;
			}

			mergeIterator = mergeTransitions.find(secondTransition);

			if (mergeIterator != mergeTransitions.end()) {
				secondTransition = mergeIterator->second;
			}

			if ((currentRelation->minBound() == 0) && (currentRelation->maxBound() == 0)) {
				firstTransition->merge(secondTransition);
				mergeTransitions[secondTransition] = firstTransition;
			} else {

				Place* currentPlace = petriNet->createPlace();

				Arc* arcFromFirstTransitionToCurrentPlace = petriNet->createArc(firstTransition, currentPlace);
				Arc* arcFromCurrentPlaceToSecondTransition = petriNet->createArc(currentPlace, secondTransition);

				(void) arcFromFirstTransitionToCurrentPlace;

				ExtendedInt intervalValue;

				intervalValue.setAsInteger(secondControlPoint->beginValue() - firstControlPoint->beginValue());
				arcFromCurrentPlaceToSecondTransition->changeRelativeTime(intervalValue, plusInfinity);

				arcToRelationConversion[arcFromCurrentPlaceToSecondTransition] = currentRelation;

				// First cleaning.
				petriNetNodeList placesAfterFirstTransition = firstTransition->returnSuccessors();

				for (unsigned j = 0; j < placesAfterFirstTransition.size() ; ++j) {
					Place* placeToCheckIfLinkedWithEndTransition = (Place*) placesAfterFirstTransition[j];
					Transition* transitionToCheckIfEqualToEndTransition = (Transition*) placeToCheckIfLinkedWithEndTransition->returnSuccessors()[0];

					if (transitionToCheckIfEqualToEndTransition == endTransition) {
						petriNet->deleteItem(placeToCheckIfLinkedWithEndTransition);
					}
				}

				petriNetNodeList placesBeforeSecondTransition = secondTransition->returnPredecessors();

				for (unsigned j = 0; j < placesBeforeSecondTransition.size() ; ++j) {
					Place* placeToCheckIfLinkedWithStartTransition = (Place*) placesBeforeSecondTransition[j];
					Transition* transitionToCheckIfEqualToStartTransition = (Transition*) placeToCheckIfLinkedWithStartTransition->returnPredecessors()[0];

					if (transitionToCheckIfEqualToStartTransition == startTransition) {
						petriNet->deleteItem(placeToCheckIfLinkedWithStartTransition);
					}
				}
			}


		}
	}

	/********** THIRD COMPILATION PHASE ***********
	 * PetriNet is cleaned
	 **********************************************/
	cleanPetriNet(petriNet, endTransition);

	/********** FOURTH COMPILATION PHASE **********
	 * Interaction is added.
	 **********************************************/

	map<unsigned int, TriggerPoint*>::iterator it  = storyLineToCompile.m_triggerPoints.begin();
	while (it != storyLineToCompile.m_triggerPoints.end())
	{
		TriggerPoint* currentTriggerPoint = it->second;

		if (!currentTriggerPoint->isMute()) {
			ControlPoint* relatedControlPoint = currentTriggerPoint->getRelatedControlPoint();

			if (relatedControlPoint != NULL) {
				if ((unsigned int) relatedControlPoint->beginValue() >= timeToStartTriggerPoint) {
					Transition* currentTransition = controlPointToTransitionConversion[relatedControlPoint];
					mergeIterator = mergeTransitions.find(currentTransition);

					if (mergeIterator != mergeTransitions.end()) {
						currentTransition = mergeIterator->second;
					}

					currentTransition->setEvent(addNetworkMessage(currentTriggerPoint->getTriggerMessage()));

					currentTransition->setMustWaitThePetriNetToEnd(false);

					TriggerPointInformations currentTriggerPointInformations;

					currentTriggerPointInformations.m_triggerId = currentTriggerPoint->getTriggerId();
					currentTriggerPointInformations.m_waitedString = currentTriggerPoint->getTriggerMessage();
					currentTriggerPointInformations.m_boxId = currentTriggerPoint->getRelatedControlPoint()->getContainingBoxId();
					currentTriggerPointInformations.m_controlPointIndex = currentTriggerPoint->getRelatedControlPoint()->getId();
					currentTriggerPointInformations.m_waitedTriggerPointMessageAction = m_waitedTriggerPointMessageAction;

					m_transitionToTriggerPointInformations[currentTransition] = currentTriggerPointInformations;

					if (currentTriggerPoint->getType() == TRIGGER_END_TEMPO_CHANGE) {
						unsigned int tempoChangeLastValue = relatedControlPoint->beginValue();

						unsigned int previousTriggerId = currentTriggerPoint->getPreviousTriggerId();
						TriggerPoint* previousTriggerPoint = storyLineToCompile.m_triggerPoints[previousTriggerId];

						ControlPoint* previousRelatedTrigerPoint = previousTriggerPoint->getRelatedControlPoint();
						Transition* previousTransition = controlPointToTransitionConversion[previousRelatedTrigerPoint];

						unsigned int tempoChangeFirstValue = previousRelatedTrigerPoint->beginValue();

						ChangeTempo* tempo = new ChangeTempo(this);
						tempo->m_writtenTime = tempoChangeLastValue - tempoChangeFirstValue;

						currentTransition->addExternAction(&lastTriggerReception, tempo);
						previousTransition->addExternAction(&firstTriggerReception, tempo);

					}

					arcList incomingArcs = currentTransition->inGoingArcsOf();
					for (unsigned int i = 0; i < incomingArcs.size(); ++i) {
						Arc* currentArc = incomingArcs[i];
						ExtendedInt minBound;
						ExtendedInt maxBound;

						minBound.setAsInteger(0);
						maxBound.setAsPlusInfinity();

						if (arcToRelationConversion.find(currentArc) != arcToRelationConversion.end()) {
							AntPostRelation* currentRelation = arcToRelationConversion[currentArc];

							if (currentRelation->minBound() != NO_BOUND) {
								minBound.setAsInteger(currentRelation->minBound());
							}

							if (currentRelation->maxBound() != NO_BOUND) {
								maxBound.setAsInteger(currentRelation->maxBound());
							}
						}

						currentArc->changeRelativeTime(minBound, maxBound);
					}
				}
			}
		}

		++it;
	}

	petriNet->setTimeOffset(timeToStartTriggerPoint);

	return petriNet;

}

bool ECOMachine::isRunning()
{
	return (m_petriNet->isRunning());
}

bool ECOMachine::isPaused()
{
	return (m_isPaused);
}

bool ECOMachine::run()
{
//	if (isRunning() && isPaused()) {
//		m_petriNet->setUpdateFactor(m_currentSpeedFactor);
//		pause(false);
//		return true;
//	}

	if (m_petriNet != NULL) {
		if (m_petriNet->getPlaces().size() >= 1) {
			m_isPaused = false;

			pthread_create(&m_thread, NULL, ECOMachineMainFunction, this);
			//pthread_create(&m_receiveThread, NULL, ECOMachineReceiveFunction, this);
			//m_receiveOSCThread->run();

			while(!isRunning())
			{
				usleep(10);
			}
			//getPetriNet()->addTime(getGotoInformation() * 1000);
			setSpeedFactor(getSpeedFactor());

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void ECOMachine::pause(bool pauseValue)
{
	m_isPaused = pauseValue;

	if (isRunning()) {
		if (!isPaused()) {
			changeSpeedFactor(m_currentSpeedFactor);
		} else {
			changeSpeedFactor(0);
		}
	}
}

bool ECOMachine::stop() {
	if (m_petriNet->isRunning()) {
		m_petriNet->mustStop();
		//m_mustStop = true;

		map<unsigned int, ECOProcess*>::iterator iter = m_process.begin();

		while (iter != m_process.end()) {
			iter->second->changeState(ECOProcess::STOP);
			++iter;
		}

		return true;
	} else {
		return false;
	}
}

void ECOMachine::setGotoInformation(unsigned int gotoInformation)
{
	m_gotoValue = gotoInformation;
}

unsigned int ECOMachine::getGotoInformation()
{
	return m_gotoValue;
}

bool ECOMachine::setSpeedFactor(float factor)
{
	m_currentSpeedFactor = factor;

	if (isRunning() && !isPaused()) {
		return changeSpeedFactor(m_currentSpeedFactor);
	} else {
		return false;
	}
}

float ECOMachine::getSpeedFactor()
{
	return m_currentSpeedFactor;
}

bool ECOMachine::changeSpeedFactor(float factor)
{
	if (isRunning()) {
		getPetriNet()->setUpdateFactor(factor);

		map<unsigned int, ECOProcess*>::iterator iter = m_process.begin();

		while (iter != m_process.end()) {
			iter->second->setSpeedFactor(factor);
			++iter;
		}

		return true;
	} else {
		return false;
	}
}

set<Transition*>
ECOMachine::getOneDepthPredecessorsTransitions(Transition* transition)
{
	set<Transition*> oneDepthPredecessors;
	petriNetNodeList oneDepthPlaces = transition->returnPredecessors();

	for (unsigned int i = 0; i < oneDepthPlaces.size(); ++i) {
		Place* currentPlace = (Place*) oneDepthPlaces[i];
		petriNetNodeList oneDepthTransitions = currentPlace->returnPredecessors();

		for(unsigned int j = 0; j < oneDepthTransitions.size(); ++j) {
			oneDepthPredecessors.insert((Transition*)oneDepthTransitions[j]);
		}
	}

	return oneDepthPredecessors;
}

set<Transition*>
ECOMachine::computeTransitionsSet(Transition* endTransition, map<Transition*, set<Transition*> >* transitionsSets)
{
	set <Transition*>::iterator setIterator;

	set<Transition*> oneDepthPredecessors = getOneDepthPredecessorsTransitions(endTransition);

	set<Transition*> twoDepthPredecessors;
	set<Transition*> nDepthPredecessors;

	for (setIterator = oneDepthPredecessors.begin(); setIterator != oneDepthPredecessors.end(); setIterator++) {
		set<Transition*> twoDepthPredecessorsTemp = getOneDepthPredecessorsTransitions(*setIterator);
		twoDepthPredecessors.insert(twoDepthPredecessorsTemp.begin(), twoDepthPredecessorsTemp.end());
	}

	if (twoDepthPredecessors.size() > 0) {
		for (setIterator = twoDepthPredecessors.begin(); setIterator != twoDepthPredecessors.end(); setIterator++) {
			set<Transition*> nDepthPredecessorsTemp = computeTransitionsSet(*setIterator, transitionsSets);
			nDepthPredecessors.insert(nDepthPredecessorsTemp.begin(), nDepthPredecessorsTemp.end());
		}

		nDepthPredecessors.insert(twoDepthPredecessors.begin(), twoDepthPredecessors.end());

		(*transitionsSets)[endTransition] = nDepthPredecessors;
	}



	return nDepthPredecessors;
}

void
ECOMachine::cleanPetriNet(PetriNet* petriNet, Transition* endTransition) {
	if (petriNet == NULL) {
		return;
	}

	map<Transition*, set<Transition*> >* transitionsSets = new map<Transition*, set<Transition*> >;

	if (endTransition == NULL) {
		computeTransitionsSet(endTransition, transitionsSets);
	}

	map<Transition*, set<Transition*> >::iterator it  = transitionsSets->begin();
	while (it != transitionsSets->end())
	{
		Transition* currentTransition = it->first;
		set<Transition*> currentSet = it->second;

		set<Place*> successorsOfNDepth;
		set<Place*> currentTransitionPredecessors;
		set<Place*> placesToDelete;

		set <Transition*>::iterator transitionSetIterator;
		set <Place*>::iterator placeSetIterator;

		for (transitionSetIterator = currentSet.begin(); transitionSetIterator != currentSet.end(); transitionSetIterator++) {
			petriNetNodeList successors = (*transitionSetIterator)->returnSuccessors();

			set<Place*> successorsOfNDepthTemp;

			for (unsigned int i = 0; i < successors.size() ; ++i) {
				successorsOfNDepthTemp.insert((Place*) successors[i]);
			}

			successorsOfNDepth.insert(successorsOfNDepthTemp.begin(), successorsOfNDepthTemp.end());
		}

		petriNetNodeList predecessors = currentTransition->returnPredecessors();


		for (unsigned int i = 0; i < predecessors.size() ; ++i) {
			currentTransitionPredecessors.insert((Place*) predecessors[i]);
		}

		set_intersection(successorsOfNDepth.begin(),successorsOfNDepth.end(),
						 currentTransitionPredecessors.begin(),currentTransitionPredecessors.end(),
						 std::inserter( placesToDelete, placesToDelete.end() ) );

		for (placeSetIterator = placesToDelete.begin(); placeSetIterator != placesToDelete.end(); placeSetIterator++) {
			Place* currentPlaceToDelete = *placeSetIterator;
			petriNet->deleteItem(currentPlaceToDelete);
		}

		++it;
	}
}

void
ECOMachine::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;
	xmlNodePtr processNode = NULL;
	std::ostringstream oss;

	node = xmlNewNode(NULL, BAD_CAST "ECOMACHINE");
	//oss << getTriggerPort();
	//xmlSetProp(node, BAD_CAST "listeningPort", BAD_CAST oss.str().data());
	xmlAddChild(father, node);

	processNode = xmlNewNode(NULL, BAD_CAST "ECOPROCESS");
	xmlAddChild(node, processNode);
	map<unsigned int, ECOProcess*>::iterator it  = m_process.begin();
	while (it != m_process.end())
	{
		it->second->store(processNode);
		++it;
	}
}

void ECOMachine::load(xmlNodePtr root, DeviceManager* controller)
{
	std::cout << "-> PROCESS ";
	std::cout.flush();

	xmlNodePtr n = NULL;

	//TODO: c'est stupide, regarder mieux !
	//xmlChar* xmlListeningPort = xmlGetProp(root, BAD_CAST "listeningPort");

	//if (xmlListeningPort != NULL) {
		//setTriggerPort(XMLConversion::xmlCharToUnsignedInt(xmlListeningPort));
	//}

	for (n = root->children; n != NULL; n = n->next) {
		if (n->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(n->name, BAD_CAST "ECOPROCESS")) {
				xmlNodePtr xmlProcess;
				for (xmlProcess = n->children; xmlProcess != NULL; xmlProcess = xmlProcess->next) {
					if (xmlProcess->type == XML_ELEMENT_NODE) {
						if (xmlStrEqual(xmlProcess->name, BAD_CAST "PROCESS")) {
							unsigned int processId;

							xmlChar* xmlProcessId = xmlGetProp(xmlProcess, BAD_CAST "id");
							xmlChar* xmlProcessType = xmlGetProp(xmlProcess, BAD_CAST "type");

							if ((xmlProcessId != NULL) && (xmlProcessType != NULL)) {
								processId = XMLConversion::xmlCharToUnsignedInt(xmlProcessId);
								string processType = XMLConversion::xmlCharToString(xmlProcessType);

								newProcess(processType, processId, controller);

								getProcess(processId)->load(xmlProcess);

								std::cout << ".";
								std::cout.flush();
							} else {
								throw InvalidFileFormatException();
							}

							if (false) {
								//TODO: verifier s'il y a un lien vers une application
							}
						}
					}
				}
			}
		}
	}

	std::cout << std::endl;
}

void ECOMachine::addCrossAControlPointAction(void(*pt2Func)(unsigned int, unsigned int, std::vector<unsigned int>))
{
	m_crossAControlPointAction = pt2Func;
}

void ECOMachine::removeCrossAControlPointAction()
{
	m_crossAControlPointAction = NULL;
}

void ECOMachine::addWaitedTriggerPointMessageAction(void* arg, void(*pt2Func)(void*, bool, unsigned int, unsigned int, unsigned int, std::string))
{
	m_waitedTriggerPointMessageArg = arg;
	m_waitedTriggerPointMessageAction = pt2Func;
}

void ECOMachine::addIsECOFinishedAction(void* arg, void(*pt2Func)(void *))
{
	m_isEcoMachineFinishedArg = arg;
	m_isEcoMachineFinished = pt2Func;
}

void ECOMachine::removeIsECOFinishedAction()
{
	m_isEcoMachineFinished = NULL;
}

bool ECOMachine::hasTriggerPointInformations(Transition* transition)
{
	map<Transition*, TriggerPointInformations>::iterator iter = m_transitionToTriggerPointInformations.find(transition);

	if(iter == m_transitionToTriggerPointInformations.end()) {
		return false;
	} else {
		return true;
	}
}

TriggerPointInformations ECOMachine::getTriggerPointInformations(Transition* transition)
{
	return m_transitionToTriggerPointInformations[transition];
}

void ECOMachine::removeWaitedTriggerPointMessageAction()
{
	m_waitedTriggerPointMessageAction = NULL;
}

float ECOMachine::getProcessProgressionPercent(unsigned int processId)
{
	map<unsigned int, ECOProcess*>::iterator iter = m_process.find(processId);

	if(iter == m_process.end())  {
		return 0;
	} else {
		return iter->second->getProgressionPercent();
	}

}



