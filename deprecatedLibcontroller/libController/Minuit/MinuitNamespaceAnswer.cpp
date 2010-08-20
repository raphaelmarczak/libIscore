#include "MinuitNamespaceAnswer.hpp"
#include <sstream>
#include <iostream>

MinuitNamespaceAnswer::MinuitNamespaceAnswer()
{
	m_state = NO_ANSWER;

	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	m_launchTimeInMs = (tv.tv_sec * 1000000L + tv.tv_usec)/1000;
	m_timeOutInMs = NO_TIMEOUT;
}

MinuitNamespaceAnswer::~MinuitNamespaceAnswer()
{
}

int MinuitNamespaceAnswer::getState()
{
	if ((m_state == NO_ANSWER) && (m_timeOutInMs != NO_TIMEOUT)) {

		struct timeval tv;
		struct timezone tz;

		long long dt = 0;

		gettimeofday(&tv, &tz);

		dt = (tv.tv_sec * 1000000L + tv.tv_usec)/1000 - m_launchTimeInMs;

		if (dt > m_timeOutInMs) {
			m_state = TIMEOUT_EXCEEDED;
		}
	}
	return m_state;
}

void MinuitNamespaceAnswer::setTimeOut(int timeout) {
	m_timeOutInMs = timeout;
}

std::string MinuitNamespaceAnswer::getIp()
{
	return m_ip;
}

void MinuitNamespaceAnswer::setIp(std::string ip)
{
	m_ip = ip;
}

unsigned int MinuitNamespaceAnswer::getPort()
{
	return m_port;
}

void MinuitNamespaceAnswer::setPort(unsigned int port)
{
	m_port = port;
}

void MinuitNamespaceAnswer::parseMinuitNamespaceAnswer(const osc::ReceivedMessage&m)
{
	std::ostringstream oss;
	osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

	while (arg != m.ArgumentsEnd()) {
		std::string currentString = arg->AsString();

		if (currentString.find("nodes") != currentString.npos) {

			++arg;
			
			if (arg->IsString()) {
				currentString = arg->AsString();
			} else if (arg->IsInt32()) {
				oss.str("");
				oss << arg->AsInt32();
				currentString = oss.str();
			} else if (arg->IsFloat()) {
				oss.str("");
				oss << arg->AsFloat();
				currentString = oss.str();
			}

			while (currentString.find("}") == currentString.npos) {
				m_nodes.push_back(currentString);
				++arg;
				
				
				if (arg->IsString()) {
					currentString = arg->AsString();
				} else if (arg->IsInt32()) {
					oss.str("");
					oss << arg->AsInt32();
					currentString = oss.str();
				} else if (arg->IsFloat()) {
					oss.str("");
					oss << arg->AsFloat();
					currentString = oss.str();
				}

				//std::cout << "nodes : " << currentString << std::endl;
			}
		} else if  (currentString.find("leaves") != currentString.npos){

			++arg;
		
			if (arg->IsString()) {
				currentString = arg->AsString();
			} else if (arg->IsInt32()) {
				oss.str("");
				oss << arg->AsInt32();
				currentString = oss.str();
			} else if (arg->IsFloat()) {
				oss.str("");
				oss << arg->AsFloat();
				currentString = oss.str();
			}

			while (currentString.find("}") == currentString.npos) {
				m_leaves.push_back(currentString);
				++arg;
				
				if (arg->IsString()) {
					currentString = arg->AsString();
				} else if (arg->IsInt32()) {
					oss.str("");
					oss << arg->AsInt32();
					currentString = oss.str();
				} else if (arg->IsFloat()) {
					oss.str("");
					oss << arg->AsFloat();
					currentString = oss.str();
				}
			}

		} else if  ((currentString.find("attributes") != currentString.npos) || (currentString.find("attributs") != currentString.npos)){

			++arg;
			
			if (arg->IsString()) {
				currentString = arg->AsString();
			} else if (arg->IsInt32()) {
				oss.str("");
				oss << arg->AsInt32();
				currentString = oss.str();
			} else if (arg->IsFloat()) {
				oss.str("");
				oss << arg->AsFloat();
				currentString = oss.str();
			}

			while (currentString.find("}") == currentString.npos) {
				m_attributs.push_back(currentString);
				++arg;
				
				if (arg->IsString()) {
					currentString = arg->AsString();
				} else if (arg->IsInt32()) {
					oss.str("");
					oss << arg->AsInt32();
					currentString = oss.str();
				} else if (arg->IsFloat()) {
					oss.str("");
					oss << arg->AsFloat();
					currentString = oss.str();
				}
			}
		}

		++arg;
	}

//	std::cout << "NODES" << std::endl;
//
//	for (unsigned int i = 0; i < m_nodes.size(); ++i) {
//		std::cout << m_nodes[i] << std::endl;
//	}
//
//	std::cout << "LEAVES" << std::endl;
//
//	for (unsigned int i = 0; i < m_leaves.size(); ++i) {
//		std::cout << m_leaves[i] << std::endl;
//	}
//
//	std::cout << "ATTRIBUTES" << std::endl;
//
//	for (unsigned int i = 0; i < m_attributes.size(); ++i) {
//		std::cout << m_attributes[i] << std::endl;
//	}
	//
	m_state = ANSWER_RECEIVED;
}

void MinuitNamespaceAnswer::getNamespaceRequestAnswer(std::vector<std::string> *nodes,
													  std::vector<std::string> *leaves,
													  std::vector<std::string> *attributs)
{
	*nodes = m_nodes;
	*leaves = m_leaves;
	*attributs = m_attributs;
}
