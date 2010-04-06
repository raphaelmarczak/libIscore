/*
 *  MinuitCommunicationMethods.cpp
 *
 *
 *  Created by Laurent Garnier on 04/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MinuitCommunicationMethods.h"

#include <iostream>

using namespace std;

MinuitCommunicationMethods::MinuitCommunicationMethods()
{
	m_receiveOSCThread = NULL;
}

void MinuitCommunicationMethods::minuitRunOSCListening(unsigned int port)
{
	if (m_receiveOSCThread == NULL) {
		m_receiveOSCThread = new ReceiveOSCThread(this);
		m_receiveOSCThread->setPort(port);
	}
	
	if (!m_receiveOSCThread->isRunning()) {
		m_receiveOSCThread->run();
	}
}

void MinuitCommunicationMethods::minuitSendRequest(const char* type, string address, string ip, unsigned int port, int timeOutInMs)
{
	
	map<int, int> testInt;
	
	unsigned int outputBufferSize = 1024;
	UdpTransmitSocket transmitSocket( IpEndpointName( ip.data(), port ) );
	
	char buffer[outputBufferSize];
	osc::OutboundPacketStream p( buffer, outputBufferSize );
	
	p << osc::BeginBundleImmediate
	<< osc::BeginMessage(type)
	<< address.data() << osc::EndMessage
	<< osc::EndBundle;
	
	transmitSocket.Send( p.Data(), p.Size() );
	
	if (type == MINUIT_REQUEST_NAMESPACE) {
		//MinuitNamespaceAnswer* namespaceAnswer = new MinuitNamespaceAnswer();
		m_namespaceAnswer[address] = new MinuitNamespaceAnswer();
		m_namespaceAnswer[address]->setTimeOut(timeOutInMs);
		m_namespaceAnswer[address]->setIp(ip);
		m_namespaceAnswer[address]->setPort(port);
		
	} else if (type == MINUIT_REQUEST_GET) {
		MinuitGetAnswer* getAnswer = new MinuitGetAnswer();
		m_getAnswer[address] = getAnswer;
		m_getAnswer[address]->setTimeOut(timeOutInMs);
	}
	
}

void MinuitCommunicationMethods::minuitParseNamespaceRequest(string address, const osc::ReceivedMessage&m)
{
	
	
	map<string, MinuitNamespaceAnswer*>::iterator it  = m_namespaceAnswer.find(address);
	
	if (it != m_namespaceAnswer.end()) {
		if (m_namespaceAnswer[address]->getState() != TIMEOUT_EXCEEDED) {
			m_namespaceAnswer[address]->parseMinuitNamespaceAnswer(m);
		}
	}
	//	MinuitNamespaceAnswer namespaceAnswer;
	//	namespaceAnswer.parseMinuitNamespaceAnswer(m);
	//	m_namespaceAnswer[address] = namespaceAnswer;
	
}

void MinuitCommunicationMethods::minuitParseGetRequest(string address, const osc::ReceivedMessage&m)
{
	
	map<string, MinuitGetAnswer*>::iterator it  = m_getAnswer.find(address);
	
	if (it != m_getAnswer.end()) {
		if (m_getAnswer[address]->getState() != TIMEOUT_EXCEEDED) {
			m_getAnswer[address]->parseMinuitGetAnswer(m);
		}
	}
	//	MinuitGetAnswer getAnswer;
	//	getAnswer.parseMinuitGetAnswer(m);
	//	m_getAnswer[address] = getAnswer;
	
}

int MinuitCommunicationMethods::minuitGetNamespaceRequestAnswer(string address,
																vector<string>* nodes,
																vector<string>* leaves,
																vector<string>* attributs)
{
	
	map<string, MinuitNamespaceAnswer*>::iterator it  = m_namespaceAnswer.find(address);
	
	if (it != m_namespaceAnswer.end()) {
		int state = it->second->getState();
		
		if (state == ANSWER_RECEIVED) {
			it->second->getNamespaceRequestAnswer(nodes, leaves, attributs);
			delete it->second;
			m_namespaceAnswer.erase(it);
		}
		
		return state;
	}
	
	return REQUEST_NOT_SENT;
}

int MinuitCommunicationMethods::minuitGetNamespaceRequestAnswer(string address,
																vector<string>* nodes,
																vector<string>* leaves,
																vector<string>* attributs,
																vector<string>* attributsValue)
{
	attributsValue->clear();
	
	map<string, MinuitNamespaceAnswer*>::iterator it  = m_namespaceAnswer.find(address);
	
	string ip;
	unsigned int port;
	
	if (it != m_namespaceAnswer.end()) {
		ip = m_namespaceAnswer[address]->getIp();
		port = m_namespaceAnswer[address]->getPort();
	} else {
		return REQUEST_NOT_SENT;
	}
	
	int state = minuitGetNamespaceRequestAnswer(address, nodes, leaves, attributs);
	
	if (state != ANSWER_RECEIVED) {
		return state;
	} else {
		for (unsigned int i = 0; i < attributs->size(); ++i) {
			unsigned int maxTryAgain = 2;
			unsigned int currentTry = 0;
			
			int stateGet;
			string currentGetRequestAddress;
			
			if (attributs->at(i) != "value") {
				currentGetRequestAddress = address + ":" + attributs->at(i);
			} else {
				currentGetRequestAddress = address;
			}
			
			string* getValueAnswer = new string("NO VALUE RECEIVED");
			
			while (currentTry < maxTryAgain) {
				minuitSendRequest(MINUIT_REQUEST_GET, currentGetRequestAddress, ip, port);
				
				do  {
					usleep(50);
					stateGet = minuitGetGetRequestAnswer(currentGetRequestAddress, getValueAnswer, false);
				} while (stateGet == NO_ANSWER);
				
				if (state == ANSWER_RECEIVED) {
					currentTry = maxTryAgain;
				} else {
					++currentTry;
				}
			}
			
			attributsValue->push_back(*getValueAnswer);
			
			delete getValueAnswer;
		}
		
		return state;
	}
}

int MinuitCommunicationMethods::minuitGetGetRequestAnswer(string address,
														  string* getSetMessage,
														  bool repeatAddress)
{
	
	map<string, MinuitGetAnswer*>::iterator it  = m_getAnswer.find(address);
	
	if (it != m_getAnswer.end()) {
		int state = it->second->getState();
		
		if (state == ANSWER_RECEIVED) {
			*getSetMessage = it->second->getGetRequestAnswer(repeatAddress);
			delete it->second;
			m_getAnswer.erase(it);
		}
		
		return state;
	}
	
	return REQUEST_NOT_SENT;
}

vector<string> MinuitCommunicationMethods::minuitSnapShot(string address, string ip, unsigned int port, int timeOutInMs)
{
	
	unsigned int maxTryAgain = 2;
	unsigned int currentTry = 0;
	
	vector<string> resultString;
	
	vector<string>* nodes = new vector<string>;
	vector<string>* leaves = new vector<string>;
	vector<string>* attributs = new vector<string>;
	
	while (currentTry < maxTryAgain) {
		int state;
		
		minuitSendRequest(MINUIT_REQUEST_NAMESPACE, address, ip, port, timeOutInMs);
		
		do  {
			usleep(50);
			state = minuitGetNamespaceRequestAnswer(address, nodes, leaves, attributs);
		} while (state == NO_ANSWER);
		
		if (state == ANSWER_RECEIVED) {
			currentTry = maxTryAgain;
		} else {
			++currentTry;
		}
	}
	
	string* getValueAnswer = new string(" ");
	string* getAccessAnswer = new string(ACCESS_GETSETTER);
	
	for (unsigned int i = 0 ; i < attributs->size() ; ++i) {
		string currentAttributsString = attributs->at(i);
		
		int state;
		
		if (currentAttributsString == "value") {
			currentTry = 0;
			
			while (currentTry < maxTryAgain) {
				string getAddress = address;
				minuitSendRequest(MINUIT_REQUEST_GET, getAddress, ip, port, timeOutInMs);
				
				do  {
					usleep(50);
					state = minuitGetGetRequestAnswer(getAddress, getValueAnswer);
				} while (state == NO_ANSWER);
				
				if (state == ANSWER_RECEIVED) {
					currentTry = maxTryAgain;
				} else {
					++currentTry;
				}
			}
			
		}
		//		else 	if (currentAttributsString == "acces") {
		//			string getAddress = address + ":" + currentAttributsString;
		//			minuitSendRequest(MINUIT_REQUEST_GET, getAddress, ip, port, timeOutInMs);
		//
		//			do  {
		//				usleep(50);
		//				state = minuitGetGetRequestAnswer(getAddress, getAccessAnswer);
		//			} while (state == NO_ANSWER);
		//		}
		
		
	}
	
	
	if (getAccessAnswer->find(ACCESS_GETSETTER) != getAccessAnswer->npos) {
		if (*getValueAnswer != " ") {
			resultString.push_back(*getValueAnswer);
		}
	}
	
	delete getAccessAnswer;
	delete getValueAnswer;
	
	for (unsigned int i = 0 ; i < nodes->size() ; ++i) {
		vector<string> nodeResult;
		
		string nodeAddress;
		
		if (address != "/") {
			nodeAddress = address + "/" + nodes->at(i);
		} else {
			nodeAddress = address + nodes->at(i);
		}
		
		nodeResult = minuitSnapShot(nodeAddress, ip, port);
		
		resultString.insert(resultString.end(), nodeResult.begin(), nodeResult.end());
	}
	
	for (unsigned int i = 0 ; i < leaves->size() ; ++i) {
		vector<string> leaveResult;
		
		string leaveAddress;
		
		if (address != "/") {
			leaveAddress = address + "/" + leaves->at(i);
		} else {
			leaveAddress = address + leaves->at(i);
		}
		
		leaveResult = minuitSnapShot(leaveAddress, ip, port);
		
		resultString.insert(resultString.end(), leaveResult.begin(), leaveResult.end());
	}
	
	delete nodes;
	delete leaves;
	delete attributs;
	
	return resultString;
}

void MinuitCommunicationMethods::receiveNetworkMessage(std::string netMessage)
{
	m_callBack(m_callBackArgument, netMessage);
	cout << "receive message " << netMessage << endl;
	
}

void MinuitCommunicationMethods::receiveNetworkRequest(std::string address, std::string requestType)
{
	if(requestType.compare("?namespace") == 0){
		m_namespaceRequestCallBack(m_namespaceRequestCallBackArgument, address);
		//cout << "receive namespace request at : " << address << endl;
	} else if(requestType.compare("?get") == 0){
		m_getRequestCallBack(m_getRequestCallBackArgument, address);	
		//cout << "receive get request at : " << address << endl;
	}
	
}

void MinuitCommunicationMethods::minuitSendNamespaceAnswer(std::string address, std::vector<std::string> *seqNamespace)
{
	//Pour l'instant le namespace du sequenceur n'est compose que de la map contenant les trigger points consideres comme des leaves
	
	//cout << "namespaceRequest!!! at " << address << endl;
	
	string ip = "127.0.0.1";
	unsigned int port = 9999;
	string type = ":namespace";
	string startArg = "leaves={";
	string endArg = "}";
	
	unsigned int outputBufferSize = 1024;
	UdpTransmitSocket transmitSocket( IpEndpointName( ip.data(), port ) );
	
	char buffer[outputBufferSize];
	osc::OutboundPacketStream p( buffer, outputBufferSize );
	
	p << osc::BeginBundleImmediate
	<< osc::BeginMessage(type.data())
	<< address.data()
	<< startArg.data();
	
	for(int i = 0; i < seqNamespace->size(); i++){
		p << seqNamespace->at(i).data();
	}
	
	p << endArg.data() << osc::EndMessage
	<< osc::EndBundle;
	transmitSocket.Send( p.Data(), p.Size() );
}

void MinuitCommunicationMethods::minuitSendGetAnswer(std::string address, int value)
{
	//Pour l'instant le namespace /TriggerPoint n'est envoye que pour les TP en attente. Le get n'est pas encore utilise.
	
	//cout << "getRequest!!!!! at " << address << endl;
	
	string ip = "127.0.0.1";
	unsigned int port = 9999;
	string type = ":get";
	//int value = 1;
	
	unsigned int outputBufferSize = 1024;
	UdpTransmitSocket transmitSocket( IpEndpointName( ip.data(), port ) );
	
	char buffer[outputBufferSize];
	osc::OutboundPacketStream p( buffer, outputBufferSize );
	
	p << osc::BeginBundleImmediate
	<< osc::BeginMessage(type.data())
	<< address.data()
	<< value << osc::EndMessage
	<< osc::EndBundle;
	
	transmitSocket.Send( p.Data(), p.Size() );	
}