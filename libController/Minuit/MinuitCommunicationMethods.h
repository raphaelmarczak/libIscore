/*
Copyright: 
LaBRI (http://www.labri.fr) - BlueYeti (http://www.blueyeti.fr) - GMEA (http://www.gmea.net)

Author(s): Raphael Marczak, Laurent Garnier, Theo Delahogue
Last modification: 08/03/2010

Adviser(s): 
Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)

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

#include "MinuitDiscoverAnswer.hpp"
#include "MinuitGetAnswer.hpp"
#include "ReceiveOSCThread.hpp"
#include "OSCParser.hpp"
#include "OSCFunctions.hpp"
#include "MinuitInclude.hpp"

#include <string>
#include <map>
#include <vector>

class MinuitCommunicationMethods{
private:
	ReceiveOSCThread* m_receiveOSCThread;
	std::map<std::string, MinuitDiscoverAnswer*> m_discoverAnswer;
	std::map<std::string, MinuitGetAnswer*> m_getAnswer;
	
public:
	MinuitCommunicationMethods();
	
	/** */
	void minuitRunOSCListening(unsigned int port);

	void(*m_discoverRequestCallBack)(void*, std::string, std::string);
	void(*m_getRequestCallBack)(void*, std::string, std::string, std::string);
	void(*m_setRequestCallBack)(void*, std::string, std::string, std::string, std::string);
	void(*m_listenRequestCallBack)(void*, std::string, std::string, std::string, bool);
	
	void* m_discoverRequestCallBackArgument;
	void* m_getRequestCallBackArgument;
	void* m_setRequestCallBackArgument;
	void* m_listenRequestCallBackArgument;
	
	void minuitReceiveNetworkDiscoverRequest(std::string from, std::string address);
	void minuitReceiveNetworkGetRequest(std::string from, std::string address, std::string attribute);
	void minuitReceiveNetworkSetRequest(std::string from, std::string address, std::string attribute, std::string value);
	void minuitReceiveNetworkListenRequest(std::string from, std::string address, std::string attribute, bool);

	void minuitSendMessage(std::string stringToSend, std::string ip, unsigned int port);
	
	void minuitParseDiscoverAnswer(std::string from, std::string address, const osc::ReceivedMessage&m);
	void minuitAddDiscoverAnswer(std::string from, std::string address, std::string ip, unsigned int port, int timeOutInMs = DEFAULT_TIMEOUT);
	int minuitWaitDiscoverAnswer(std::string from, std::string address, std::vector<std::string>* returnednodes, std::vector<std::string>* returnedleaves, std::vector<std::string>* returnedAttributes);
	
	void minuitParseGetAnswer(std::string from, std::string address, const osc::ReceivedMessage&m);
	void minuitAddGetAnswer(std::string from, std::string address, int timeOutInMs = DEFAULT_TIMEOUT);
	int minuitWaitGetAnswer(std::string from, std::string address, std::string* returnedValues, bool repeatAddress = true);
};

unsigned int computeOSCMessageSize(OSCParser OSCParsed);

#endif /*MINUITCOMMUNICATIONMETHODS_HPP_*/
