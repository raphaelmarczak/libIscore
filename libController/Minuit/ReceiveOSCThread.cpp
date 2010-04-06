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

#include "ReceiveOSCThread.hpp"
#include "MinuitCommunicationMethods.h"

#include <iostream>
#include <sstream>

using namespace std;

void* OSCReceiveFunction(void* threadArg)
{
	ReceiveOSCThread* ReceiveOSC = (ReceiveOSCThread*) threadArg;

	UdpListeningReceiveSocket* s = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, ReceiveOSC->getPort()), ReceiveOSC);
	ReceiveOSC->m_listeningSocket = s;

	ReceiveOSC->m_listeningSocket->Run();
}

ReceiveOSCThread::ReceiveOSCThread(MinuitCommunicationMethods *minuitMethods)
{
	m_port = DEFAULT_RECEIVE_OSC_THREAD_PORT;
	m_minuitMethods = minuitMethods;
	m_isRunning = false;
	m_listeningSocket = NULL;
}

ReceiveOSCThread::~ReceiveOSCThread()
{
}

void
ReceiveOSCThread::setPort(unsigned int port)
{
	m_port = port;
}

unsigned int
ReceiveOSCThread::getPort()
{
	return m_port;
}

void
ReceiveOSCThread::run()
{
	m_isRunning = true;
	pthread_create(&m_receiveThread, NULL, OSCReceiveFunction, this);
}

bool
ReceiveOSCThread::isRunning()
{
	return m_isRunning;
}

void
ReceiveOSCThread::asynchronousBreak()
{
	unsigned int usecToStopTheSelect = 20000;
	if (m_listeningSocket != NULL) {
		m_listeningSocket->AsynchronousBreak();

		usleep(usecToStopTheSelect);

		delete m_listeningSocket;
		m_listeningSocket = NULL;
	}

	m_isRunning = false;
}

void
ReceiveOSCThread::ProcessMessage(const osc::ReceivedMessage&m, const IpEndpointName& remoteEndPoint)
{
	std::string currentString(m.AddressPattern());
	std::string sender = "";
	std::string operation = "";
	int operationStart;
	std:string address;
	std::string whereTo = "";
	std::string attribute = "";
	int attributeStart;
	std::ostringstream arguments;
	
	/*
	 if start with '/'
	
	 else if start with '?'
		parse sender?operation /whereTo:attribute
	
		if ?namespace
	
		if ?get
	 
		if ?listen
	
	 else if start with ':'
		parse sender:operation /whereTo:attribute
		get arguments
	 
		if :namespace
	
		if :get
	 
		if :listen
	
	 */
	
	// if start with '/'
	if (currentString[0] == '/')
	{
		attributeStart = currentString.find(':');								// parse /whereTo:attribute
		if(attributeStart != currentString.npos)
		{
			whereTo = currentString.substr(0, attributeStart);					// get whereTo
			attribute = currentString.substr(attributeStart+1);					// get attribute
		}
		else
		{
			whereTo = currentString;											// get where to
			attribute = "value";												// default attribute is 'value'
		}
		
		osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();			// get arguments
		
		while(arg != m.ArgumentsEnd()){
			
			if (arg->IsChar()) {
				arguments << arg->AsChar();
			} else if (arg->IsInt32()) {
				arguments << arg->AsInt32();
			} else if (arg->IsFloat()) {
				arguments << arg->AsFloat();
			} else if (arg->IsString()) {
				arguments << arg->AsString();
			}
			
			arg++;
			
			if(arg != m.ArgumentsEnd())
				arguments << " ";
		}
		
		// DEBUG
		cout << "Receive set request (OSC style) at " << whereTo << endl;
		cout << "Arguments : " << arguments.str() << endl;
		
		m_minuitMethods->minuitReceiveNetworkSetRequest(sender, whereTo, attribute, arguments.str());
		return;
	} 
	else {
		
		// Is it a request ?
		operationStart = currentString.find('?');
		if(operationStart != currentString.npos)
		{
			sender = currentString.substr(0, operationStart);					// get sender
			operation = currentString.substr(operationStart);					// get request
			
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();		// parse /whereTo:attribute
			if(arg->IsString()){
				
				address = arg->AsString();
				
				attributeStart = address.find(':');
				if(attributeStart != address.npos)
				{
					whereTo = address.substr(0, attributeStart);				// get whereTo
					attribute = address.substr(attributeStart+1);				// get attribute
				}
				else
				{
					whereTo = address;											// get where to
					attribute = "value";										// default attribute is 'value'
				}
			}
			
			// DEBUG
			cout << "Receive " << operation << " request from "<< sender << " at " << whereTo << " for " << attribute << endl;
			
			// switch on request
			if(operation.compare(MINUIT_REQUEST_DISCOVER) == 0){
				m_minuitMethods->minuitReceiveNetworkDiscoverRequest(sender, whereTo);
				return;
			}
			
			else if(operation.compare(MINUIT_REQUEST_GET) == 0){
				m_minuitMethods->minuitReceiveNetworkGetRequest(sender, whereTo, attribute);
				return;
			}
			
			else if(operation.compare(MINUIT_REQUEST_LISTEN) == 0){
				
				arg++;																// get 1 string argument
				if (arg->IsString())
					arguments << arg->AsString();
				
				// DEBUG
				cout << "Arguments : " << arguments.str() << endl;

				if(arguments.str().compare(MINUIT_REQUEST_LISTEN_ENABLE) == 0)
					m_minuitMethods->minuitReceiveNetworkListenRequest(sender, whereTo, attribute, true);
				else if(arguments.str().compare(MINUIT_REQUEST_LISTEN_DISABLE) == 0)
					m_minuitMethods->minuitReceiveNetworkListenRequest(sender, whereTo, attribute, false);
				
				return;
			}
			
			return;
		}// end if starts by '?'
		
		
		// Is it a return ?
		operationStart = currentString.find(':');
		if(operationStart != currentString.npos)
		{
			sender = currentString.substr(0, operationStart);					// get sender
			operation = currentString.substr(operationStart);					// get answer
			
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();		// get where to
			if(arg->IsString())
				whereTo = arg->AsString();
			
			// DEBUG
			cout << "Receive " << operation << " answer from "<< sender << " at " << whereTo << endl;
			
			arg++;																// get arguments
			while(arg != m.ArgumentsEnd()){
				
				if (arg->IsChar()) {
					arguments << arg->AsChar();
				} else if (arg->IsInt32()) {
					arguments << arg->AsInt32();
				} else if (arg->IsFloat()) {
					arguments << arg->AsFloat();
				}  else if (arg->IsString()) {
					arguments << arg->AsString();
				}
				
				arg++;
				
				if(arg != m.ArgumentsEnd())
					arguments << " ";
			}
			
			// DEBUG
			cout << "Arguments : " << arguments.str() << endl;
			
			// switch on answer
			if(operation.compare(MINUIT_ANSWER_DISCOVER) == 0){
				m_minuitMethods->minuitParseDiscoverAnswer(sender, whereTo, m);
				return;
			}
			
			else if(operation.compare(MINUIT_ANSWER_GET) == 0){
				m_minuitMethods->minuitParseGetAnswer(sender, whereTo, m);
				return;
			}
			
			else if(operation.compare(MINUIT_ANSWER_LISTEN) == 0){
				// TODO : what ?
				return;
			}
			
			return;
		} // end if starts by ':'
		
	} // end else
};


