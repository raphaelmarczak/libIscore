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

/*!
 * \file Engines.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "Includes/Engines.hpp"

#include <stdio.h>
#include <math.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "EnginesPrivate/EnginesPrivate.hpp"
#include "StoryLine/StoryLine.hpp"

using namespace std;

void receiveNetworkMessageCallBack(void* arg, std::string message, std::string attribut, std::string& value)
{
	std::string play = "/Transport/Play";
	std::string stop = "/Transport/Stop";
	std::string pause = "/Transport/Pause";
	std::string rewind = "/Transport/Rewind";
	std::string startPoint = "/Transport/StartPoint";
	std::string speed = "/Transport/Speed";
		
	Engines* engines = (Engines*) arg;
	
	std::string argument = value;
	
	unsigned int action = UNKNOWN_MESSAGE;

	void (*updateAction)(unsigned int, std::string, std::string) = engines->m_implementation->m_enginesUpdateWithNetworkMessageAction;
	
	if (engines->receiveNetworkMessage(message)) {
		action = TRIGGER_MESSAGE;
	} else {
		std::string currentAddress = message;
			 		
		if (currentAddress == play) {
			engines->play();
			action = ENGINES_PLAY;
		} else if (currentAddress == stop) {
			engines->stop();
			action = ENGINES_STOP;
		} else if (currentAddress == pause) {
			engines->pause(!engines->isPaused());
			action = ENGINES_PAUSE_MODIFICATION;
		} else if (currentAddress == rewind) {
			engines->stop();
			engines->setGotoValue(0);
			action = ENGINES_REWIND;
		} else if (currentAddress == startPoint) {
			if (isFloat(argument)) {
				engines->setGotoValue(toInt(argument));
				action = ENGINES_GOTO_MODIFICATION;
			}
		} else if (currentAddress == speed) {
			if (isFloat(argument)) {
				float speedFactor = toFloat(argument);
				engines->setExecutionSpeedFactor(speedFactor);
				action = ENGINES_SPEED_MODIFICATION;
			}
		}
		
		if (updateAction != NULL) {
			updateAction(action, message, argument);
		} 
	}
} 

void triggerPointStateCallBack(void* arg, bool isWaited, unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex, std::string waitedString)
{
	Engines* engines = (Engines*) arg;

	if(isWaited) {
		engines->m_implementation->m_networkController->setNamespaceValue(waitedString, 1 /* TRIGGER_WAITED */, engines->getBoxOptionalArguments(boxId));
	} else {
		engines->m_implementation->m_networkController->setNamespaceValue(waitedString, 2 /* TRIGGER_PUSHED */, engines->getBoxOptionalArguments(boxId));
	}

	if (engines->m_implementation->m_waitedTriggerPointMessageAction != NULL) {
		engines->m_implementation->m_waitedTriggerPointMessageAction(isWaited, triggerId, boxId, controlPointIndex, waitedString);
	}
}

void executionFinishedCallBack(void* arg)
{ 
	Engines* engines = (Engines*) arg;

	engines->m_implementation->m_networkController->resetTriggerPointStates();

	if (engines->m_implementation->m_isECOMachineFinish != NULL) {
		engines->m_implementation->m_isECOMachineFinish();
	}

}

void Engines::initializeObjects(unsigned int maxSceneWidth, std::string plugginsLocation)
{
	m_implementation = new EnginesPrivate();

	m_implementation->m_editor = new Editor(maxSceneWidth);
	m_implementation->m_executionMachine = new ECOMachine();

	m_implementation->m_networkController = new Controller("Virage");
	m_implementation->m_networkController->pluginLoad(plugginsLocation);
	m_implementation->m_networkController->deviceSetCurrent();
	m_implementation->m_networkController->namespaceSetAddCallback(this, &receiveNetworkMessageCallBack);

	m_implementation->m_executionMachine->addWaitedTriggerPointMessageAction(this, &triggerPointStateCallBack);
	m_implementation->m_executionMachine->addIsECOFinishedAction(this, &executionFinishedCallBack);
	
	m_implementation->m_enginesUpdateWithNetworkMessageAction = NULL;
}

Engines::Engines(unsigned int maxSceneWitdth, std::string plugginsLocation)
{
	initializeObjects(maxSceneWitdth, plugginsLocation);
}

Engines::Engines(void(*crossAction)(unsigned int, unsigned int),
				 void(*triggerAction)(bool, unsigned int, unsigned int, unsigned int, std::string),
				 void(*isExecutionFinished)(),
				 unsigned int maxSceneWitdth,
				 std::string plugginsLocation)
{
	initializeObjects(maxSceneWitdth, plugginsLocation);

	if (crossAction != NULL) {
		addCrossingCtrlPointCallback(crossAction);
	}

	if (triggerAction != NULL) {
		addCrossingTrgPointCallback(triggerAction);
	}

	if (isExecutionFinished != NULL) {
		addExecutionFinishedCallback(isExecutionFinished);
	}
}

Engines::~Engines()
{
	delete m_implementation->m_executionMachine;
	delete m_implementation->m_editor;
	delete m_implementation->m_networkController;
}

ECOMachine* Engines::getExecutionMachine()
{
	return m_implementation->m_executionMachine;
}

Editor* Engines::getEditor()
{
	return m_implementation->m_editor;
}

void Engines::reset(unsigned int maxSceneWidth)
{
	delete m_implementation->m_executionMachine;
	delete m_implementation->m_editor;

	m_implementation->m_editor = new Editor(maxSceneWidth);
	m_implementation->m_executionMachine = new ECOMachine();

	m_implementation->m_executionMachine->addWaitedTriggerPointMessageAction(this, &triggerPointStateCallBack);
	m_implementation->m_executionMachine->addIsECOFinishedAction(this, &executionFinishedCallBack);
}

void Engines::reset(void(*crossAction)(unsigned int, unsigned int),
					void(*triggerAction)(bool, unsigned int, unsigned int, unsigned int, std::string),
					unsigned int maxSceneWidth)
{
	reset(maxSceneWidth);

	if (crossAction != NULL) {
		addCrossingCtrlPointCallback(crossAction);
	}

	if (triggerAction != NULL) {
		addCrossingTrgPointCallback(triggerAction);
	}
}

void deprecated(std::string a, std::string b)
{
	std::cout << std::endl;
	std::cout << "**** " << a << " DEPECRATED ! Please use " << b << " ****" ;
	std::cout << std::endl << std::endl;
}

void Engines::addCrossingCtrlPointCallback(void(*pt2Func)(unsigned int, unsigned int))
{
	getExecutionMachine()->addCrossAControlPointAction(pt2Func);
}

void Engines::removeCrossingCtrlPointCallback()
{
	getExecutionMachine()->removeCrossAControlPointAction();
}

void Engines::addCrossingTrgPointCallback(void(*pt2Func)(bool, unsigned int, unsigned int, unsigned int, std::string))
{
	m_implementation->m_waitedTriggerPointMessageAction = pt2Func;
}

void Engines::removeCrossingTrgPointCallback()
{
	getExecutionMachine()->removeWaitedTriggerPointMessageAction();
}

void Engines::addExecutionFinishedCallback(void(*pt2Func)())
{
	m_implementation->m_isECOMachineFinish = pt2Func;
}

void Engines::removeExecutionFinishedCallback()
{
	m_implementation->m_executionMachine->removeIsECOFinishedAction();
}

void Engines::addEnginesNetworkUpdateCallback(void(*pt2Func)(unsigned int, std::string, std::string))
{
	m_implementation->m_enginesUpdateWithNetworkMessageAction = pt2Func;
}

void Engines::removeEnginesNetworkUpdateCallback()
{
	m_implementation->m_enginesUpdateWithNetworkMessageAction = NULL;
}




// Edition ////////////////////////////////////////////////////////////////////////

unsigned int Engines::addBox(int boxBeginPos, int boxLength, unsigned int motherId)
{
	unsigned int newBoxId = getEditor()->addBox(boxBeginPos, boxLength, motherId);

	if (newBoxId != NO_ID) {
		getExecutionMachine()->newProcess(PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND, newBoxId, m_implementation->m_networkController);
	}

	return newBoxId;
}

unsigned int Engines::addBox(int boxBeginPos, int boxLength, unsigned int motherId, int minBound, int maxBound)
{
	unsigned int newBoxId = addBox(boxBeginPos, boxLength, motherId);

	if (newBoxId != NO_ID) {
		changeBoxBounds(newBoxId, minBound, maxBound);
	}

	return newBoxId;
}

void Engines::changeBoxBounds(unsigned int boxId, int minBound, int maxBound)
{
	unsigned int flexibilityRelationID = getEditor()->addAntPostRelation(boxId, BEGIN_CONTROL_POINT_INDEX, boxId, END_CONTROL_POINT_INDEX, ANTPOST_ANTERIORITY, minBound, maxBound, NULL);

	if (flexibilityRelationID != NO_ID) {
		ConstrainedBox* currentConstrainedBox = getEditor()->getBoxById(boxId);

		if (currentConstrainedBox->getFlexibilityRelationId() != NO_ID) {
			removeTemporalRelation(currentConstrainedBox->getFlexibilityRelationId());
		}

		currentConstrainedBox->setFlexibilityInformations(flexibilityRelationID, minBound, maxBound);
	}
}

void Engines::removeBox(unsigned int boxId)
{
	getEditor()->removeBox(boxId);

	getExecutionMachine()->removeProcess(boxId);
}

void Engines::setBoxOptionalArgument(unsigned int boxId, std::string key, std::string value)
{
	getEditor()->setOptionalArgument(boxId, key, value);
}

void Engines::removeBoxOptionalArgument(unsigned int boxId, std::string key)
{
	getEditor()->removeOptionalArgument(boxId, key);
}

std::map<std::string, std::string> Engines::getBoxOptionalArguments(unsigned int boxId)
{
	return getEditor()->getOptionalArguments(boxId);
}

unsigned int Engines::addTemporalRelation(unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, int minBound, int maxBound, vector<unsigned int>* movedBoxes)
{
	if (boxId1 == boxId2) {
		return NO_ID;
	}

	return getEditor()->addAntPostRelation(boxId1, controlPoint1, boxId2, controlPoint2, type, minBound, maxBound, movedBoxes);
}

unsigned int Engines::addTemporalRelation(unsigned int boxId1, unsigned int controlPoint1, unsigned int boxId2, unsigned int controlPoint2, TemporalRelationType type, vector<unsigned int>* movedBoxes)
{
	return addTemporalRelation(boxId1, controlPoint1, boxId2, controlPoint2, type, NO_BOUND, NO_BOUND, movedBoxes);
}

void Engines::changeTemporalRelationBounds(unsigned int relationId, int minBound, int maxBound, vector<unsigned int>* movedBoxes)
{
	getEditor()->changeAntPostRelationBounds(relationId, minBound, maxBound, movedBoxes);
}

bool Engines::isTemporalRelationExisting(unsigned int boxId1, unsigned int controlPoint1Index, unsigned int boxId2, unsigned int controlPoint2Index)
{
	return getEditor()->isAntPostRelationAlreadyExist(boxId1, controlPoint1Index, boxId2, controlPoint2Index);
}



void Engines::removeTemporalRelation(unsigned int relationId)
{
	getEditor()->removeTemporalRelation(relationId);
}

unsigned int Engines::getRelationFirstBoxId(unsigned int relationId)
{
	return getEditor()->getRelationFirstBoxId(relationId);
}

unsigned int Engines::getRelationFirstCtrlPointIndex(unsigned int relationId)
{
	return getEditor()->getRelationFirstControlPointIndex(relationId);
}

unsigned int Engines::getRelationSecondBoxId(unsigned int relationId)
{
	return getEditor()->getRelationSecondBoxId(relationId);
}

unsigned int Engines::getRelationSecondCtrlPointIndex(unsigned int relationId)
{
	return getEditor()->getRelationSecondControlPointIndex(relationId);
}

bool Engines::performBoxEditing(unsigned int boxId, int x, int y, vector<unsigned int>* movedBoxes, unsigned int maxModification)
{
	return getEditor()->performMoving(boxId, x, y, movedBoxes, maxModification);
}

int Engines::getBoxBeginTime(unsigned int boxId)
{
	return getEditor()->getBeginValue(boxId);
}

int Engines::getBoxEndTime(unsigned int boxId)
{
	return getEditor()->getEndValue(boxId);
}

int Engines::getBoxDuration(unsigned int boxId)
{
	return getEditor()->getEndValue(boxId) - getEditor()->getBeginValue(boxId);
}

int Engines::getBoxNbCtrlPoints(unsigned int boxId)
{
	return getEditor()->nbControlPoint(boxId);
}

int Engines::getBoxFirstCtrlPointIndex(unsigned int boxId)
{
	return getEditor()->getFirstControlPointIndex(boxId);
}

int Engines::getBoxLastCtrlPointIndex(unsigned int boxId)
{
	return getEditor()->getLastControlPointIndex(boxId);
}

void Engines::setCtrlPointMessagesToSend(unsigned int boxId, unsigned int controlPointIndex, std::vector<std::string> messageToSend, bool muteState)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		ControlPoint* currentControlPoint = getEditor()->getBoxById(boxId)->getControlPoint(controlPointIndex);
		currentSendOSCProcess->addMessages(messageToSend, currentControlPoint->getProcessStepId(), muteState);
	}
}

void Engines::getCtrlPointMessagesToSend(unsigned int boxId, unsigned int controlPointIndex, std::vector<std::string>* messages)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		ControlPoint* currentControlPoint = getEditor()->getBoxById(boxId)->getControlPoint(controlPointIndex);

		currentSendOSCProcess->getMessages(messages, currentControlPoint->getProcessStepId());
	}
}

void Engines::removeCtrlPointMessagesToSend(unsigned int boxId, unsigned int controlPointIndex)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		ControlPoint* currentControlPoint = getEditor()->getBoxById(boxId)->getControlPoint(controlPointIndex);
		currentSendOSCProcess->removeMessage(currentControlPoint->getProcessStepId());
	}
}

void Engines::setCtrlPointMutingState(unsigned int boxId, unsigned int controlPointIndex, bool mute)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		ControlPoint* currentControlPoint = getEditor()->getBoxById(boxId)->getControlPoint(controlPointIndex);
		currentSendOSCProcess->setMessageMuteState(currentControlPoint->getProcessStepId(), mute);
	}
}

bool Engines::getCtrlPointMutingState(unsigned int boxId, unsigned int controlPointIndex)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		ControlPoint* currentControlPoint = getEditor()->getBoxById(boxId)->getControlPoint(controlPointIndex);
		return currentSendOSCProcess->getMessageMuteState(currentControlPoint->getProcessStepId());
	}

	return false;
}

void Engines::setProcessMutingState(unsigned int boxId, bool mute)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->setCurvesMuteState(mute);
	}
}

bool Engines::getProcessMutingState(unsigned int boxId)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		return currentSendOSCProcess->getCurvesMuteState();
	}

	return false;
}

void Engines::setBoxMutingState(unsigned int boxId, bool mute)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->setAllMessagesMuteState(mute);
		currentSendOSCProcess->setCurvesMuteState(mute);
	}
}


//CURVES ////////////////////////////////////////////////////////////////////////////////////

void Engines::addCurve(unsigned int boxId, std::string address)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->addCurves(address);

	}
}

void Engines::removeCurve(unsigned int boxId, std::string address)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->removeCurves(address);

	}
}

void Engines::clearCurves(unsigned int boxId)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->removeAllCurves();
	}
}

std::vector<std::string> Engines::getCurvesAddress(unsigned int boxId)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);
	
	std::vector<std::string> addressToReturn;

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		addressToReturn = currentSendOSCProcess->getCurvesAdress();
	} else {
		//TODO : exception
	}
	
	return addressToReturn;
}

void Engines::setCurveSampleRate(unsigned int boxId, std::string address, unsigned int nbSamplesBySec)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		currentSendOSCProcess->setNbSamplesBySec(address, nbSamplesBySec);
	}
}

unsigned int Engines::getCurveSampleRate(unsigned int boxId, std::string address)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);
	
	unsigned int sampleRate = 0;

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		sampleRate = currentSendOSCProcess->getNbSamplesBySec(address);
	} else {
		//TODO : exception
	}
	
	return sampleRate;
}

void Engines::setCurveRedundancy(unsigned int boxId, std::string address, bool avoidRedondance)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		currentSendOSCProcess->setAvoidRedondanceInformation(address, avoidRedondance);
	}
}

bool Engines::getCurveRedundancy(unsigned int boxId, std::string address)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);
	
	bool curveRedundancyInformation = false;

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;
		curveRedundancyInformation = currentSendOSCProcess->getAvoidRedondanceInformation(address);
	} else {
		//TODO : exception
	}
	
	return curveRedundancyInformation;
}

void Engines::getCurveArgTypes(std::string stringToParse, std::vector<std::string>* result)
{
	result->clear();

	StringParser addressAndArgs(stringToParse);

	result->push_back(addressAndArgs.getAddress());

	for (unsigned int i = 0; i < addressAndArgs.getNbArg(); ++i) {
		StringParser::argType currentType = addressAndArgs.getType(i);

		if (currentType == StringParser::TYPE_FLOAT) {
			result->push_back("FLOAT");
		} else if (currentType == StringParser::TYPE_INT) {
			result->push_back("INT");
		} else if (currentType == StringParser::TYPE_STRING) {
			result->push_back("STRING");
		} else {
			result->push_back("SYMBOL");
		}
	}
}

bool Engines::setCurveSections(unsigned int boxId, std::string address, unsigned int argNb,
						   std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff)
{
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		return currentSendOSCProcess->setCurvesSections(address, argNb, percent, y, sectionType, coeff);
	} else {
		//TODO : exception
	}
	
	return false;
}

bool Engines::getCurveValues(unsigned int boxId, std::string address, unsigned int argNb, std::vector<float>* result)
{
	result->clear();
	ECOProcess* currentProcess = getExecutionMachine()->getProcess(boxId);

	if ((currentProcess->getType() == PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND)) {
		SendNetworkMessageProcess* currentSendOSCProcess = (SendNetworkMessageProcess*) currentProcess;

		return currentSendOSCProcess->getCurves(address, argNb, getBoxEndTime(boxId) - getBoxBeginTime(boxId), BEGIN_CONTROL_POINT_INDEX, END_CONTROL_POINT_INDEX, result);
	} else {
		return false;
	}
}





unsigned int Engines::addTriggerPoint()
{
	return getEditor()->addTriggerPoint();
}

void Engines::removeTriggerPoint(unsigned int triggerId)
{
	m_implementation->m_networkController->removeTriggerPointLeave(triggerId);
	getEditor()->removeTriggerPoint(triggerId);
}

bool Engines::assignCtrlPointToTriggerPoint(unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex)
{
	return getEditor()->setTriggerPointRelatedControlPoint(triggerId, boxId, controlPointIndex);
}

void Engines::freeTriggerPointFromCtrlPoint(unsigned int triggerId)
{
	getEditor()->removeTriggerPointRelatedControlPoint(triggerId);
}

void Engines::setTriggerPointMessage(unsigned int triggerId, std::string triggerMessage)
{
	m_implementation->m_networkController->addTriggerPointLeave(triggerId, triggerMessage);
	getEditor()->setTriggerPointMessage(triggerId, triggerMessage);
}

std::string Engines::getTriggerPointMessage(unsigned int triggerId)
{
	return getEditor()->getTriggerPointMessage(triggerId);
}

unsigned int Engines::getTriggerPointRelatedBoxId(unsigned int triggerId)
{
	return getEditor()->getTriggerPointRelatedBoxId(triggerId);
}

unsigned int Engines::getTriggerPointRelatedCtrlPointIndex(unsigned int triggerId)
{
	return getEditor()->getTriggerPointRelatedControlPointIndex(triggerId);
}

void Engines::setTriggerPointMutingState(unsigned int triggerId, bool muteState)
{
	getEditor()->setTriggerPointMuteState(triggerId, muteState);
}

bool Engines::getTriggerPointMutingState(unsigned int triggerId)
{
	return getEditor()->getTriggerPointMuteState(triggerId);
}

void Engines::getBoxesId(vector<unsigned int>* boxesID)
{
	getEditor()->getAllBoxesId(boxesID);
}

void Engines::getRelationsId(vector<unsigned int>* relationsID)
{
	getEditor()->getAllAntPostRelationsId(relationsID);
}

void Engines::getTriggersPointId(vector<unsigned int>* triggersID)
{
	getEditor()->getAllTriggersId(triggersID);
}

//Execution ///////////////////////////////////////////////////////////
void Engines::setGotoValue(unsigned int gotoValue)
{
	getExecutionMachine()->setGotoInformation(gotoValue);
}

unsigned int Engines::getGotoValue()
{
	return getExecutionMachine()->getGotoInformation();
}

bool Engines::play()
{
	if (!isRunning()) {
		compile();
		return run();

	} else {
		return false;
	}
}

void Engines::pause(bool pauseValue)
{
	getExecutionMachine()->pause(pauseValue);
}

bool Engines::isPaused()
{
	return getExecutionMachine()->isPaused();
}

bool Engines::stop()
{
	return getExecutionMachine()->stop();
}

bool Engines::isRunning()
{
	return getExecutionMachine()->isRunning();
}

void Engines::compile()
{
	getExecutionMachine()->reset();

	StoryLine* storyLine = new StoryLine(getEditor()->getCSP());

	if (storyLine->m_constrainedBoxes.size() >= 1) {
		getExecutionMachine()->compileECO(storyLine, getExecutionMachine()->getGotoInformation());
	}

	delete storyLine;
}

bool Engines::run()
{
	bool runIsOk = getExecutionMachine()->run();

	if (runIsOk) {
		usleep(50);
		getExecutionMachine()->getPetriNet()->addTime(getExecutionMachine()->getGotoInformation() * 1000);
		setExecutionSpeedFactor(getExecutionSpeedFactor());
	}

	return runIsOk;
}

unsigned int Engines::getCurrentExecutionTime()
{
	return getExecutionMachine()->getTimeInMs();
}

void Engines::setExecutionSpeedFactor(float factor)
{
	getExecutionMachine()->setSpeedFactor(factor);
}

float Engines::getExecutionSpeedFactor()
{
	return getExecutionMachine()->getSpeedFactor();
}

float Engines::getProcessProgression(unsigned int processId)
{
	if (getExecutionMachine()->isRunning()) {
		return getExecutionMachine()->getProcessProgressionPercent(processId);
	} else {
		return 0;
	}
}

void Engines::ignoreTriggerPointOnce()
{
	if (getExecutionMachine()->isRunning()) {
		getExecutionMachine()->ignoreTriggerPointForOneStep();
	}
}

bool Engines::receiveNetworkMessage(std::string netMessage)
{
	if (getExecutionMachine()->isRunning()) {
		return getExecutionMachine()->receiveNetworkMessage(netMessage);
	} else {
		return false;
	}
}

void Engines::simulateNetworkMessageReception(std::string netMessage)
{
	std::string value("");
	receiveNetworkMessageCallBack(this, netMessage, "", value);
//	if (getExecutionMachine()->isRunning()) {
//		getExecutionMachine()->receiveNetworkMessage(netMessage);
//	}
}

void Engines::getLoadedNetworkPlugins(std::vector<std::string>* pluginsName, std::vector<unsigned int>* listeningPort)
{
	*pluginsName = m_implementation->m_networkController->pluginGetLoadedByName();

	for (unsigned int i = 0; i < pluginsName->size(); ++i) {
		listeningPort->push_back(toInt(m_implementation->m_networkController->pluginGetCommParameter(pluginsName->at(i), "pluginReceptionPort")));
	}
}

void Engines::addNetworkDevice(std::string deviceName, std::string pluginToUse, std::string DeviceIp, std::string DevicePort)
{
	//m_implementation->m_networkController->addDevice(deviceName, pluginToUse, DeviceIp, DevicePort);
	std::map<std::string, std::string>* commParameters = new std::map<std::string, std::string>();

	(*commParameters)["ip"] = DeviceIp;
	(*commParameters)["port"] = DevicePort;

	m_implementation->m_networkController->deviceAdd(deviceName, pluginToUse, commParameters);

	delete commParameters;

}

void Engines::removeNetworkDevice(std::string deviceName)
{
	m_implementation->m_networkController->deviceRemove(deviceName);
}

void Engines::sendNetworkMessage(std::string stringToSend)
{
	m_implementation->m_networkController->deviceSendSetRequest(stringToSend); 
}

void Engines::getNetworkDevicesName(std::vector<std::string>* devicesName, std::vector<bool>* couldSendNamespaceRequest)
{
	std::map<std::string, Device*> mapDevices = *(m_implementation->m_networkController->deviceGetCurrent());

	map<string, Device*>::iterator it = mapDevices.begin();

	while (it != mapDevices.end()) {
		if (m_implementation->m_networkController->deviceIsVisible(it->first)) {
			devicesName->push_back(it->first);
			if (couldSendNamespaceRequest != NULL) {
				couldSendNamespaceRequest->push_back(m_implementation->m_networkController->deviceUnderstandDiscoverRequest(it->first));
			}
		}

		++it;
	}
}

std::vector<std::string> Engines::requestNetworkSnapShot(std::string address)
{
	return m_implementation->m_networkController->deviceSnapshot(address);
}

int Engines::requestNetworkNamespace(std::string address, vector<string>* nodes, vector<string>* leaves, vector<string>* attributs, vector<string>* attributsValue)
{
//	int state = TIMEOUT_EXCEEDED;
//	bool namespaceSent = true;
	
	return m_implementation->m_networkController->deviceSendDiscoverRequest(address, nodes, leaves, attributs, attributsValue);

//	namespaceSent = m_implementation->m_networkController->sendNamespaceRequest(address);
//
//	if (namespaceSent){
//		do  {
//			usleep(50);
//			state = m_implementation->m_networkController->getNamespaceRequestAnswer(address, nodes, leaves, attributs, attributsValue);
//		} while (state == NO_ANSWER);
//	}
//
//	return state;
}



// LOAD AND STORE

void Engines::store(std::string fileName)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "ENGINES");
	xmlSetProp(root_node, BAD_CAST "version", BAD_CAST ENGINE_VERSION);

	std::ostringstream oss;
	oss << getEditor()->scenarioSize();
	xmlSetProp(root_node, BAD_CAST "scenarioSize", BAD_CAST oss.str().data());

	xmlDocSetRootElement(doc, root_node);

	getEditor()->store(root_node);
	getExecutionMachine()->store(root_node);

	xmlSaveFormatFileEnc(fileName.data(), doc, "UTF-8", 1);

	xmlFreeDoc(doc);
}

void Engines::load(std::string fileName)
{
	xmlNodePtr racine = NULL;
	xmlNodePtr n = NULL;
	xmlDocPtr doc = xmlReadFile(fileName.data(), "UTF-8", 0);

	racine = xmlDocGetRootElement(doc);

	xmlChar* version = xmlGetProp(racine, BAD_CAST "version");
	xmlChar* xmlScenarioSize = xmlGetProp(racine, BAD_CAST "scenarioSize");

	(void) version;
	
	int scenarioSize = XMLConversion::xmlCharToInt(xmlScenarioSize);

	reset(scenarioSize);

	for (n = racine->children; n != NULL; n = n->next) {
		if (n->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(n->name, BAD_CAST "EDITOR")) {
				std::cout << "LOAD: EDITOR" << std::endl;
				m_implementation->m_editor->load(n);
			} else if (xmlStrEqual(n->name, BAD_CAST "ECOMACHINE")) {
				std::cout << "LOAD: ECO-MACHINE" << std::endl;
				m_implementation->m_executionMachine->load(n, m_implementation->m_networkController);
			}
		}
	}


	xmlFreeDoc(doc);
}

void Engines::load(std::string fileName,
				   void(*crossAction)(unsigned int, unsigned int),
				   void(*triggerAction)(bool, unsigned int, unsigned int, unsigned int, std::string))
{
	load(fileName);

	if (crossAction != NULL) {
		addCrossingCtrlPointCallback(crossAction);
	}

	if (triggerAction != NULL) {
		addCrossingTrgPointCallback(triggerAction);
	}
}

// NETWORK



void Engines::print() {
	getExecutionMachine()->getPetriNet()->print();
}



void Engines::printExecutionInLinuxConsole()
{
	std::vector<unsigned int>* boxesId = new std::vector<unsigned int>();

	getBoxesId(boxesId);

	bool mustDisplay = true;

	while(isRunning()){
		if ((getCurrentExecutionTime()/60)%2 == 0) {
			if (mustDisplay) {
				std::system("clear");
				for (unsigned int i = 0; i < boxesId->size(); ++i) {
					unsigned int processPercent;

					processPercent = getProcessProgression(boxesId->at(i));

					if ((processPercent > 0) && (processPercent < 99)) {
						std::cout << "[*";

						for (unsigned j = 10; j <= processPercent; j += 10) {
							std::cout << "*";
						}

						for (unsigned j = processPercent + 1; j <= 90; j += 10) {
							std::cout << ".";
						}

						std::cout << "] -> (" << i << ") (" << processPercent << ")" << std::endl;
					}
				}
				mustDisplay = false;
			}
		} else {
			mustDisplay = true;
		}
		usleep(50);
	}
}

