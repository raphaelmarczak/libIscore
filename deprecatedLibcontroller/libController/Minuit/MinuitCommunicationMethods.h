/*
 *  MinuitCommunicationMethods.h
 *
 *
 *  Created by Laurent Garnier on 04/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MINUITCOMMUNICATIONMETHODS_HPP_
#define MINUITCOMMUNICATIONMETHODS_HPP_

#include "MinuitNamespaceAnswer.hpp"
#include "MinuitGetAnswer.hpp"
#include "ReceiveOSCThread.hpp"
#include "OSCParser.hpp"
#include "OSCFunctions.hpp"
#include "MinuitInclude.hpp"

#include <string>
#include <map>
#include <vector>

#define MINUIT_REQUEST_NAMESPACE "?namespace"
#define MINUIT_REQUEST_GET "?get"

class MinuitCommunicationMethods{
private:
	ReceiveOSCThread* m_receiveOSCThread;
	std::map<std::string, MinuitNamespaceAnswer*> m_namespaceAnswer;
	std::map<std::string, MinuitGetAnswer*> m_getAnswer;
	
public:
	MinuitCommunicationMethods();
	
	void* m_callBackArgument;
	void* m_namespaceRequestCallBackArgument; 
	void* m_getRequestCallBackArgument;
	
	void(*m_callBack)(void*, std::string);
	void(*m_namespaceRequestCallBack)(void*, std::string);
	void(*m_getRequestCallBack)(void*, std::string);
	
	void minuitRunOSCListening(unsigned int port);
	
	void minuitSendRequest(const char* type, std::string address, std::string ip, unsigned int port, int timeOutInMs = DEFAULT_TIMEOUT);
	
	void minuitParseNamespaceRequest(std::string address, const osc::ReceivedMessage&m);
	
	void minuitParseGetRequest(std::string address, const osc::ReceivedMessage&m);
	
	int minuitGetNamespaceRequestAnswer(std::string address,
										std::vector<std::string>* nodes,
										std::vector<std::string>* leaves,
										std::vector<std::string>* attributs);
	
	int minuitGetNamespaceRequestAnswer(std::string address,
										std::vector<std::string>* nodes,
										std::vector<std::string>* leaves,
										std::vector<std::string>* attributs,
										std::vector<std::string>* attributsValue);
	
	int minuitGetGetRequestAnswer(std::string address,
								  std::string* getSetMessage,
								  bool repeatAddress = true);
	
	std::vector<std::string> minuitSnapShot(std::string address, std::string ip, unsigned int port, int timeOutInMs = DEFAULT_TIMEOUT);
	
	void receiveNetworkMessage(std::string netMessage);
	
	void receiveNetworkRequest(std::string address, std::string requestType);
	
	void minuitSendNamespaceAnswer(std::string address, std::vector<std::string>* seqNamespace);
	
	void minuitSendGetAnswer(std::string address, int value);
	
};

#endif /*MINUITCOMMUNICATIONMETHODS_HPP_*/
