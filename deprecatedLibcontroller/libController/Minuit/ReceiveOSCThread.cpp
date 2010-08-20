#include "ReceiveOSCThread.hpp"
#include "MinuitCommunicationMethods.h"

#include <sstream>

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
	std::ostringstream oss;

	if ((m.AddressPattern())[0] == '/') {
		m_minuitMethods->receiveNetworkMessage(m.AddressPattern());
	} else {
		std::string currentString(m.AddressPattern());

		if (currentString.find(":namespace") != currentString.npos) {
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

			if (arg->IsChar()) {
				oss << arg->AsChar();
			} else if (arg->IsInt32()) {
				oss << arg->AsInt32();
			} else if (arg->IsString()) {
				oss << arg->AsString();
			} 
			
			m_minuitMethods->minuitParseNamespaceRequest(oss.str(), m);
		} else if (currentString.find(":get") != currentString.npos) {
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

			if (arg->IsChar()) {
				oss << arg->AsChar();
			} else if (arg->IsInt32()) {
				oss << arg->AsInt32();
			} else if (arg->IsString()) {
				oss << arg->AsString();
			}

			m_minuitMethods->minuitParseGetRequest(oss.str(), m);
		} else if (currentString.find("?namespace") != currentString.npos ||
				   currentString.find("?get") != currentString.npos) {
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
	
			if (arg->IsChar()) {
				oss << arg->AsChar();
			} else if (arg->IsInt32()) {
				oss << arg->AsInt32();
			} else if (arg->IsString()) {
				oss << arg->AsString();
			}
			
			m_minuitMethods->receiveNetworkRequest(oss.str(), currentString);
		} 
	}
};


