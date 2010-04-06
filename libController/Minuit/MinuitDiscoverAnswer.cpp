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

#include "MinuitDiscoverAnswer.hpp"
#include "MinuitInclude.hpp"
#include <sstream>
#include <iostream>

using namespace std;

MinuitDiscoverAnswer::MinuitDiscoverAnswer()
{
	m_state = NO_ANSWER;

	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	m_launchTimeInMs = (tv.tv_sec * 1000000L + tv.tv_usec)/1000;
	m_timeOutInMs = NO_TIMEOUT;
}

MinuitDiscoverAnswer::~MinuitDiscoverAnswer()
{
	;
}

int MinuitDiscoverAnswer::getState()
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

void MinuitDiscoverAnswer::setTimeOut(int timeout) {
	m_timeOutInMs = timeout;
}

std::string MinuitDiscoverAnswer::getIp()
{
	return m_ip;
}

void MinuitDiscoverAnswer::setIp(std::string ip)
{
	m_ip = ip;
}

unsigned int MinuitDiscoverAnswer::getPort()
{
	return m_port;
}

void MinuitDiscoverAnswer::setPort(unsigned int port)
{
	m_port = port;
}

void MinuitDiscoverAnswer::parseMinuitDiscoverAnswer(const osc::ReceivedMessage&m)
{
	bool find_end;
	osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
	
	// don't parse sender?operation /whereTo
	arg++;
	
	// parse something like : nodes={ some nodes } leaves={ some leaves } attributes={ somes attributes }
	while(arg != m.ArgumentsEnd()){
		
		std::string currentString = arg->AsString();
		
		// parse nodes
		if(currentString.compare(MINUIT_START_NODES) == 0){

			do{
				arg++;
				std::ostringstream parsed;
				if (arg->IsChar()) {
					parsed << arg->AsChar();
				} else if (arg->IsInt32()) {
					parsed << arg->AsInt32();
				} else if (arg->IsFloat()) {
					parsed << arg->AsFloat();
				}  else if (arg->IsString()) {
					parsed << arg->AsString();
				}
				cout << "Parsed nodes " << parsed.str() << endl;
				currentString = parsed.str();
				// don't store the end flag
				find_end = currentString.compare(MINUIT_END_NODES) == 0;
				if(!find_end)
					m_nodes.push_back(currentString);
			}
			while(!find_end);
			
		}
		else if(currentString.compare(MINUIT_START_LEAVES) == 0){
			
			do{
				arg++;
				std::ostringstream parsed;
				if (arg->IsChar()) {
					parsed << arg->AsChar();
				} else if (arg->IsInt32()) {
					parsed << arg->AsInt32();
				} else if (arg->IsFloat()) {
					parsed << arg->AsFloat();
				}  else if (arg->IsString()) {
					parsed << arg->AsString();
				}
				cout << "Parsed leaves " << parsed.str() << endl;
				currentString = parsed.str();
				// don't store the end flag
				find_end = currentString.compare(MINUIT_END_LEAVES) == 0;
				if(!find_end)
					m_leaves.push_back(currentString);
			}
			while(!find_end);
			
		}
		else if(currentString.compare(MINUIT_START_ATTRIBUTES) == 0){

			do{
				arg++;
				std::ostringstream parsed;
				if (arg->IsChar()) {
					parsed << arg->AsChar();
				} else if (arg->IsInt32()) {
					parsed << arg->AsInt32();
				} else if (arg->IsFloat()) {
					parsed << arg->AsFloat();
				}  else if (arg->IsString()) {
					parsed << arg->AsString();
				}
				cout << "Parsed attributes " << parsed.str() << endl;
				currentString = parsed.str();
				// don't store the end flag
				find_end = currentString.compare(MINUIT_END_ATTRIBUTES) == 0;
				if(!find_end)
					m_attributes.push_back(currentString);
			}
			while(!find_end);
			
		}
		else
			arg++;
	}

	m_state = ANSWER_RECEIVED;
}

void MinuitDiscoverAnswer::waitDiscoverAnswer(std::vector<std::string> *returnedNodes,
											   std::vector<std::string> *returnedLeaves,
												std::vector<std::string> *returnedAttributes)
{
	*returnedNodes = m_nodes;
	*returnedLeaves = m_leaves;
	*returnedAttributes = m_attributes;
}
