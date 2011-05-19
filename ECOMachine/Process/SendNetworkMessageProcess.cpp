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
 * \file SendNetworkMessageProcess.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "SendNetworkMessageProcess.hpp"
#include "StringTypeFunctions.hpp"

#include "StringParser.hpp"

#include <vector>
#include <sstream>
#include <iostream>

#include <iterator>
#include <sys/time.h>

//#define DEBUG_OSC

SendNetworkMessageProcess::SendNetworkMessageProcess(unsigned int id, DeviceManager* controller)
:ECOProcess(id)
{
	m_type = PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND;

	m_currentTime = 0;

	m_nbSampleBySec = DEFAUT_NB_SAMPLES_BY_SEC;
	m_previousCallOfMakeOneStepInMs = 0;

	m_networkController = controller;

	m_curvesMuteState = false;
}

void SendNetworkMessageProcess::init()
{
	ECOProcess::init();

	unsigned int firstStep = 0;

	std::map<unsigned int, std::map<std::string, std::string> >::iterator itMessageToSendInMap = m_messageToSendInMap.find(firstStep);

	if (itMessageToSendInMap != m_messageToSendInMap.end()) {
		std::map<std::string, std::string> firstMap = itMessageToSendInMap->second;

		std::map<unsigned int, std::vector<std::string> >::iterator itMessageToSendInVector = m_messageToSendInVector.begin();

		while (itMessageToSendInVector != m_messageToSendInVector.end()) {
			unsigned int currentStep = itMessageToSendInVector->first;
			if (currentStep != firstStep) {
				std::vector<std::string> currentVector = itMessageToSendInVector->second;

				for (unsigned int i = 0; i < currentVector.size(); ++i) {
					std::string currentMessage = currentVector[i];
					StringParser currentStringParser(currentMessage);

					std::map<std::string, NetworkMessageCurves>::iterator itCurves = m_curves.find(currentStringParser.getAddress());

					if (itCurves == m_curves.end()) {
						std::map<std::string, std::string>::iterator itMessage = firstMap.find(currentStringParser.getAddress());

						if (itMessage == firstMap.end()) {
							m_messageMuteState[currentStep].at(i) = false;
						} else {
							if (currentMessage == itMessage->second) {
								m_messageMuteState[currentStep].at(i) = true;
							} else {
								m_messageMuteState[currentStep].at(i) = false;
							}
						}

					} else {
						m_messageMuteState[currentStep].at(i) = false;
					}
				}
			}

			itMessageToSendInVector++;
		}

	}

}

SendNetworkMessageProcess::~SendNetworkMessageProcess()
{

}

void SendNetworkMessageProcess::addMessages(std::vector<std::string> messages, unsigned int stepId, bool muteState)
{
	--stepId;

	std::map<std::string, std::string> newMap;

	m_messageToSendInMap[stepId] = newMap;

	m_messageToSendInVector[stepId].clear();

	for (unsigned int i = 0; i < messages.size(); ++i) {
		std::string stringToCut = messages[i];

		std::istringstream iss(stringToCut);

		std::string currentStringToAdd;
		while ( std::getline( iss, currentStringToAdd, ',' ) )
		{
			StringParser currentMessage(currentStringToAdd);

			m_messageToSendInMap[stepId][currentMessage.getAddress()] = currentStringToAdd;
			m_messageToSendInVector[stepId].push_back(currentStringToAdd);
			m_messageMuteState[stepId].push_back(false);
			m_stepMuteState[stepId] = muteState;
		}
	}
}

void SendNetworkMessageProcess::getMessages(std::vector<std::string>& messages, unsigned int stepId)
{
	--stepId;

	messages.clear();

//	std::map<unsigned int, std::map<std::string, std::string > >::iterator it  = m_messageToSendInMap.find(stepId);
//
//	if (it != m_messageToSendInMap.end()) {
//		std::map<std::string, std::string >::iterator it2 = it->second.begin();
//
//		while (it2 != it->second.end()) {
//			messages->push_back(it2->second);
//
//			it2++;
//		}
//	}

	for (unsigned int i = 0; i < m_messageToSendInVector[stepId].size(); ++i) {
		messages.push_back(m_messageToSendInVector[stepId][i]);
	}
}

void SendNetworkMessageProcess::removeMessage(unsigned int stepId)
{
	--stepId;

	std::map<unsigned int, std::map<std::string, std::string > >::iterator it = m_messageToSendInMap.find(stepId);
	std::map<unsigned int, std::vector<std::string > >::iterator it2 = m_messageToSendInVector.find(stepId);

	if (it != m_messageToSendInMap.end()) {
		m_messageToSendInMap.erase(it);
	}

	if (it2 != m_messageToSendInVector.end()) {
		m_messageToSendInVector.erase(it2);
	}
}

bool SendNetworkMessageProcess::addCurvesPoints(std::string address, unsigned int argNb, std::vector<float> percent,
												std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it == m_curves.end()) {
		return false;
	}

	return it->second.addCurvesPoints(argNb, percent, y, sectionType, coeff);
}

bool SendNetworkMessageProcess::getCurveSections(std::string address, unsigned int argNb, std::vector<float>& percent,
												std::vector<float>& y, std::vector<short>& sectionType, std::vector<float>& coeff)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it == m_curves.end()) {
		return false;
	}

	return it->second.getCurveSections(argNb, percent, y, sectionType, coeff);
}

void SendNetworkMessageProcess::setMessageMuteState(unsigned int stepId, bool muteState)
{
	--stepId;

	m_stepMuteState[stepId] = muteState;
}

bool SendNetworkMessageProcess::getMessageMuteState(unsigned int stepId)
{
	--stepId;

	return m_stepMuteState[stepId];
}

void SendNetworkMessageProcess::setAllMessagesMuteState(bool muteState)
{
	std::map<unsigned int, std::map<std::string, std::string > >::iterator it = m_messageToSendInMap.begin();

	while (it != m_messageToSendInMap.end()) {
		m_stepMuteState[it->first] = muteState;
		++it;
	}

}

bool SendNetworkMessageProcess::getCurves(std::string address, unsigned int argNb, unsigned int duration, unsigned int firstControlPointIndex, unsigned int lastControlPointIndex, std::vector<float>& result)
{
	std::vector<float> returnVector;

	std::map< unsigned int, std::map<std::string, std::string> >::iterator itFirstCP = m_messageToSendInMap.find(firstControlPointIndex - 1);
	std::map< unsigned int, std::map<std::string, std::string> >::iterator itLastCP = m_messageToSendInMap.find(lastControlPointIndex - 1);

	if ((itFirstCP != m_messageToSendInMap.end()) && (itLastCP != m_messageToSendInMap.end())) {
		std::map<std::string, std::string>::iterator itFirstMessage = itFirstCP->second.find(address);
		std::map<std::string, std::string>::iterator itLastMessage = itLastCP->second.find(address);

		if ((itFirstMessage != itFirstCP->second.end()) && (itLastMessage != itLastCP->second.end())) {
			std::string firstMessage = itFirstMessage->second;
			std::string lastMessage = itLastMessage->second;

			return m_curves[address].getCurves(StringParser(firstMessage), StringParser(lastMessage), argNb, duration, result);
		} else {
			return false;
		}
	} else {
		return false;
	}


}

//void SendNetworkMessageProcess::addAddressToInterpolate(std::vector<std::string> addressToInterpolate)
//{
//	m_avoidRedondanceForInterpolation.clear();
//	m_addressToInterpolate = addressToInterpolate;
//}

//void SendNetworkMessageProcess::addAvoidRedondanceInformation(std::vector<bool> avoidRedondance)
//{
//	m_avoidRedondanceForInterpolation.clear();
//
//	if (avoidRedondance.size() == m_addressToInterpolate.size()) {
//		m_avoidRedondanceForInterpolation = avoidRedondance;
//	}
//}

void SendNetworkMessageProcess::setAvoidRedondanceInformation(std::string address, bool avoidRedondance)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		it->second.setAvoidRedondance(avoidRedondance);
	}
}

bool SendNetworkMessageProcess::getAvoidRedondanceInformation(std::string address)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		return it->second.getAvoidRedondance();
	} else {
		std::cout << "Impossible to get the avoidRedondance Information ! " << address << " is not defined as Curves" << std::endl;
		return false;
	}
}

void SendNetworkMessageProcess::setCurveMuteStateInformation(std::string address, bool muteState)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		it->second.setMuteState(muteState);
	}
}

bool SendNetworkMessageProcess::getCurveMuteStateInformation(std::string address)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		return it->second.getMuteState();
	} else {
		std::cout << "Impossible to get the mute state Information ! " << address << " is not defined as Curves" << std::endl;
		return false;
	}
}

std::vector<std::string> SendNetworkMessageProcess::getCurvesAdress()
{
	std::vector<std::string> returnVector;

	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.begin();

	while (it != m_curves.end()) {
		returnVector.push_back(it->first);
		it++;
	}

	return returnVector;
}

void SendNetworkMessageProcess::removeAllCurves()
{
	m_curves.clear();
}

void SendNetworkMessageProcess::setNbSamplesBySec(std::string address, unsigned int nbSample)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		it->second.setSampleBySec(nbSample);
	}
}

unsigned int SendNetworkMessageProcess::getNbSamplesBySec(std::string address)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		return it->second.getSampleBySec();
	} else {
		std::cout << "Impossible to get the nbOfSamples Information ! " << address << " is not defined as Curves" << std::endl;
		return 0;
	}
}

void SendNetworkMessageProcess::setCurvesMuteState(bool muteState)
{
	m_curvesMuteState = muteState;
}

bool SendNetworkMessageProcess::getCurvesMuteState()
{
	return m_curvesMuteState;
}

bool SendNetworkMessageProcess::setCurvesSections(std::string address, unsigned int argNb,
					   std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff)
{
	std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(address);

	if (it != m_curves.end()) {
		it->second.addCurvesPoints(argNb, percent, y, sectionType, coeff);
	}

	return true;
}

void SendNetworkMessageProcess::computeCurves(unsigned int firstStep, unsigned int lastStep, unsigned int duration)
{
	m_curvesToSend.clear();
	std::map<std::string, std::string> firstMapForInterpolation = m_messageToSendInMap[firstStep - 1];
	std::map<std::string, std::string> secondMapForInterpolation = m_messageToSendInMap[lastStep - 1];

	std::vector<std::string> startMessages = m_messageToSendInVector[firstStep - 1];

	for (unsigned int i = 0; i < startMessages.size(); ++i) {
		std::string currentAddress = StringParser(startMessages[i]).getAddress();

		std::map<std::string, NetworkMessageCurves>::iterator it = m_curves.find(currentAddress);

		if (it != m_curves.end()) {
			if (!it->second.getMuteState()) {

				std::map<std::string, std::string>::iterator it1 = firstMapForInterpolation.find(currentAddress);
				std::map<std::string, std::string>::iterator it2 = secondMapForInterpolation.find(currentAddress);

				if ((it1 != firstMapForInterpolation.end()) && (it2 != secondMapForInterpolation.end())) {
					NetworkMessageCurves currentCurves = it->second;

					currentCurves.computeAllCurves(StringParser(it1->second), StringParser(it2->second), duration);

					m_curvesToSend.push_back(currentCurves);
				}
			}

			++it;
		}
	}




}

void SendNetworkMessageProcess::sendMessage(std::string stringToSend)
{
	m_networkController->deviceSendSetRequest(stringToSend);
}

long long SendNetworkMessageProcess::computeDt()
{
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

void SendNetworkMessageProcess::addCurves(std::string address)
{
	NetworkMessageCurves n(address);

	m_curves[address] = n;
}

void SendNetworkMessageProcess::removeCurves(std::string address)
{
	if (m_curves.find(address) != m_curves.end()) {
		m_curves.erase(address);
	}
}

void SendNetworkMessageProcess::sendCurves(unsigned int time)
{
	if (m_curvesToSend.size() > 0) {
		bool mustSend = false;

		std::string messageToSend;

//		char buffer[OUTPUT_BUFFER_SIZE];
//		osc::OutboundPacketStream oscStream(buffer, OUTPUT_BUFFER_SIZE );
//		UdpTransmitSocket transmitSocket( IpEndpointName(ip.data(), port) );
//
//		oscStream << osc::BeginBundleImmediate;

		for (unsigned int i = 0; i < m_curvesToSend.size(); ++i) {
			if(m_curvesToSend[i].getMessageToSend(time, messageToSend)) {
				mustSend = true;
			}
			//usleep(100);
			if (mustSend) {
				sendMessage(messageToSend);
			}
		}

		//oscStream  << osc::EndBundle;


	}
}

float SendNetworkMessageProcess::getProgressionPercent()
{
	float percent =  ((float)m_currentTime/(float)(m_writtenTime * 1000));

	if (percent > 1.) {
		percent = 1.;
	}

	return percent;
}


void SendNetworkMessageProcess::mainFunction()
{
	m_state = PLAY;

	SendNetworkMessageProcess* NetworkMessageThreadArg = this;
	NetworkMessageThreadArg->m_previousCallOfMakeOneStepInMs = 0;

	NetworkMessageThreadArg->m_currentTime = 0 + (m_timeOffsetInMs * 1000);

	while (((NetworkMessageThreadArg->m_currentStepIndex) < (int)(NetworkMessageThreadArg->m_messageToSendInMap.size() - 1)) && (m_state == PLAY)) {
		NetworkMessageThreadArg->m_currentTime += (NetworkMessageThreadArg->computeDt() * m_speedFactor);

		if ((NetworkMessageThreadArg->m_currentStepIndex) < (NetworkMessageThreadArg->m_lastStepIdReceived))
		{
			++NetworkMessageThreadArg->m_currentStepIndex;

			//if (((m_curvesToSend.size() > 0) && (OSCThreadArg->m_currentStepIndex != 0)) || (m_curvesToSend.size() == 0)) {


			//std::map<unsigned int, std::map<std::string, std::string > >::iterator it  = NetworkMessageThreadArg->m_messageToSendInMap.find(NetworkMessageThreadArg->m_currentStepIndex);
			std::map<unsigned int, std::vector<std::string> >::iterator it  = NetworkMessageThreadArg->m_messageToSendInVector.find(NetworkMessageThreadArg->m_currentStepIndex);

		//	if ((it != m_messageToSendInMap.end()) && !m_messageMuteState[it->first]) {
			if ((it != m_messageToSendInVector.end()) && !m_stepMuteState[it->first]) {
				//std::map<std::string, std::string > currentMap = NetworkMessageThreadArg->m_messageToSendInMap[NetworkMessageThreadArg->m_currentStepIndex];
				std::vector<std::string> currentVector = NetworkMessageThreadArg->m_messageToSendInVector[NetworkMessageThreadArg->m_currentStepIndex];
				std::vector<bool> currentMuteState = m_messageMuteState[NetworkMessageThreadArg->m_currentStepIndex];
//				std::map<std::string, std::string >::iterator it2 = it->second.begin();
//
//				while (it2 != it->second.end()) {
//					std::string stringToSend = it2->second;
//					sendMessage(stringToSend);
//					it2++;
//				}

				for (unsigned int i = 0; i < currentVector.size(); ++i) {
					if (!currentMuteState[i]) {
						sendMessage(currentVector[i]);
					}
				}
			}


			//}
		} else {

			if ((m_curvesToSend.size() > 0) && !m_curvesMuteState) {
				NetworkMessageThreadArg->sendCurves((NetworkMessageThreadArg->m_currentTime)/1000);
			}
		}

		usleep(100);
	}

	m_state = STOP;
}

void SendNetworkMessageProcess::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;
	xmlNodePtr messageNode = NULL;
	xmlNodePtr processNode = storeGlobalProcess(father);

	if (m_curvesMuteState) {
		xmlSetProp(processNode, BAD_CAST "mute", BAD_CAST "true");
	} else {
		xmlSetProp(processNode, BAD_CAST "mute", BAD_CAST "false");
	}

	std::ostringstream oss;

	for (unsigned int i = 0; i < m_messageToSendInVector.size(); ++i) {
		std::map<unsigned int, std::vector<std::string > >::iterator it = m_messageToSendInVector.find(i);

		bool isMute = m_stepMuteState[it->first];

		node = xmlNewNode(NULL, BAD_CAST "STEP");

		oss.str("");
		oss << (i + 1);

		xmlSetProp(node, BAD_CAST "id", BAD_CAST oss.str().data());

		if (isMute) {
			xmlSetProp(node, BAD_CAST "mute", BAD_CAST "true");
		} else {
			xmlSetProp(node, BAD_CAST "mute", BAD_CAST "false");
		}

		xmlAddChild(processNode, node);

		if (it != m_messageToSendInVector.end()) {
			std::vector<std::string> currentVector = m_messageToSendInVector[i];

			for (unsigned int i = 0; i < currentVector.size(); ++i) {
				messageNode = xmlNewNode(NULL, BAD_CAST "MESSAGE");
				xmlSetProp(messageNode, BAD_CAST "value", BAD_CAST currentVector[i].data());
				xmlAddChild(node, messageNode);
			}

//			std::map<std::string, std::string >::iterator it2 = currentMap.begin();
//
//			while (it2 != currentMap.end()) {
//				messageNode = xmlNewNode(NULL, BAD_CAST "MESSAGE");
//				xmlSetProp(messageNode, BAD_CAST "value", BAD_CAST it2->second.data());
//				xmlAddChild(node, messageNode);
//				it2++;
//			}
		}
	}

//	std::vector<std::string> curvesAdressToSend = getCurvesAdress();
//
//	for (unsigned int i = 0; i < curvesAdressToSend.size(); ++i) {
//		node = xmlNewNode(NULL, BAD_CAST "CURVE_ADDRESS_TO_SEND");
//
//		xmlSetProp(node, BAD_CAST "address", BAD_CAST curvesAdressToSend[i].data());
//
//		if (m_curvesMuteState) {
//			xmlSetProp(node, BAD_CAST "mute", BAD_CAST "true");
//		} else {
//			xmlSetProp(node, BAD_CAST "mute", BAD_CAST "false");
//		}
//
//		xmlAddChild(processNode, node);
//	}


	std::map<std::string, NetworkMessageCurves >::iterator it3 = m_curves.begin();

	while (it3 != m_curves.end()) {
		it3->second.store(processNode);

		++it3;
	}

}

void SendNetworkMessageProcess::load(xmlNodePtr root)
{
	xmlNodePtr xmlStep = NULL;
	xmlNodePtr xmlCurvesArg = NULL;
	xmlNodePtr xmlCurvesInformation = NULL;

	unsigned int stepId;

	xmlChar* xmlAddressMute = xmlGetProp(root, BAD_CAST "mute");

	if (xmlAddressMute != NULL) {
		std::string muteState = XMLConversion::xmlCharToString(xmlAddressMute);

		if (muteState=="true") {
			m_curvesMuteState = true;
		}
	}

	for (xmlStep = root->children; xmlStep != NULL; xmlStep = xmlStep->next) {
		if (xmlStep->type == XML_ELEMENT_NODE) {
			if (xmlStrEqual(xmlStep->name, BAD_CAST "STEP")) {
				xmlChar* xmlStepId = xmlGetProp(xmlStep, BAD_CAST "id");
				xmlChar* xmlStepMute = xmlGetProp(xmlStep, BAD_CAST "mute");

				bool stepMute = false;

				if (xmlStepId != NULL) {
					stepId = XMLConversion::xmlCharToUnsignedInt(xmlStepId);
				} else {
					throw InvalidFileFormatException();
				}

				std::vector<std::string> messages;

				xmlNodePtr xmlStepMessage = NULL;
				for (xmlStepMessage = xmlStep->children; xmlStepMessage != NULL; xmlStepMessage = xmlStepMessage->next) {
					if (xmlStepMessage->type == XML_ELEMENT_NODE) {
						if (xmlStrEqual(xmlStepMessage->name, BAD_CAST "MESSAGE")) {
							xmlChar* xmlMessage = xmlGetProp(xmlStepMessage, BAD_CAST "value");

							if (xmlMessage != NULL) {
								messages.push_back(XMLConversion::xmlCharToString(xmlMessage));
							}
						}
					}
				}

				if (xmlStepMute != NULL) {
					std::string muteState = XMLConversion::xmlCharToString(xmlStepMute);

					if (muteState=="true") {
						stepMute = true;
					}
				}

				addMessages(messages, stepId, stepMute);

			}

			if (xmlStrEqual(xmlStep->name, BAD_CAST "CURVE_INFORMATION")) {
				xmlChar* xmlAddress = xmlGetProp(xmlStep, BAD_CAST "address");
				xmlChar* xmlSampleRate = xmlGetProp(xmlStep, BAD_CAST "samplerate");
				xmlChar* xmlRedundancy = xmlGetProp(xmlStep, BAD_CAST "avoidRedundancy");
				xmlChar* xmlMuteState = xmlGetProp(xmlStep, BAD_CAST "muteState");

				if (xmlAddress != NULL) {
					std::string address = XMLConversion::xmlCharToString(xmlAddress);

					addCurves(address);

					if (xmlSampleRate != NULL) {
						setNbSamplesBySec(address, XMLConversion::xmlCharToInt(xmlSampleRate));
					}

					if (xmlRedundancy != NULL) {
						std::string avoidRedundancy = XMLConversion::xmlCharToString(xmlRedundancy);

						if (avoidRedundancy=="true") {
							setAvoidRedondanceInformation(address, true);
						} else {
							setAvoidRedondanceInformation(address, false);
						}

					}

					if (xmlMuteState != NULL) {
						std::string muteState = XMLConversion::xmlCharToString(xmlMuteState);

						if (muteState=="true") {
							setCurveMuteStateInformation(address, true);
						} else {
							setCurveMuteStateInformation(address, false);
						}

					}

					for (xmlCurvesArg = xmlStep->children; xmlCurvesArg != NULL; xmlCurvesArg = xmlCurvesArg->next) {
						if (xmlStrEqual(xmlCurvesArg->name, BAD_CAST "ARGUMENT")) {
							unsigned int argNb;
							std::vector<float> percent;
							std::vector<float> y;
							std::vector<short> sectionType;
							std::vector<float> coeff;

							xmlChar* xmlArgNb = xmlGetProp(xmlCurvesArg, BAD_CAST "nb");

							argNb = XMLConversion::xmlCharToUnsignedInt(xmlArgNb);

							for (xmlCurvesInformation = xmlCurvesArg->children; xmlCurvesInformation != NULL; xmlCurvesInformation = xmlCurvesInformation->next) {
								if (xmlStrEqual(xmlCurvesInformation->name, BAD_CAST "COORDINATE")) {
									xmlChar* XMLx = xmlGetProp(xmlCurvesInformation, BAD_CAST "x_percent");
									xmlChar* XMLy = xmlGetProp(xmlCurvesInformation, BAD_CAST "y_value");

									percent.push_back(XMLConversion::xmlCharToInt(XMLx));
									y.push_back(XMLConversion::xmlCharToInt(XMLy));
								} else if (xmlStrEqual(xmlCurvesInformation->name, BAD_CAST "SECTION")) {
									xmlChar* XMLSectionType = xmlGetProp(xmlCurvesInformation, BAD_CAST "section_type");
									xmlChar* XMLCoeff = xmlGetProp(xmlCurvesInformation, BAD_CAST "coeff");

									std::string sectionTypeString = XMLConversion::xmlCharToString(XMLSectionType);

									if (sectionTypeString == "pow") {
										sectionType.push_back(CURVE_POW);
									}

									coeff.push_back(XMLConversion::xmlCharToFloat(XMLCoeff));
								}
							}

							addCurvesPoints(address, argNb, percent, y, sectionType, coeff);
						}
					}
				}
			}
		}
	}
}


