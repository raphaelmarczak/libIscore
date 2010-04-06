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

#include "MinuitGetAnswer.hpp"

#include <iostream>
#include <sstream>

using namespace std;

MinuitGetAnswer::MinuitGetAnswer()
{
	m_state = NO_ANSWER;

	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, NULL);

	m_launchTimeInMs = (tv.tv_sec * 1000000L + tv.tv_usec)/1000;
	m_timeOutInMs = NO_TIMEOUT;
}

MinuitGetAnswer::~MinuitGetAnswer()
{
}

int MinuitGetAnswer::getState()
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
void MinuitGetAnswer::setTimeOut(int timeout) {
	m_timeOutInMs = timeout;
}

void MinuitGetAnswer::parseMinuitGetAnswer(const osc::ReceivedMessage&m)
{
	osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
	std::ostringstream ossWithAddress;
	std::ostringstream ossWithoutAddress;
	std::ostringstream floatCorrection;

	// get the address
	ossWithAddress << arg->AsString();
	++arg;
	
	while(arg != m.ArgumentsEnd()) {

		if (arg->IsChar()) {
			ossWithAddress  << arg->AsChar();
			ossWithoutAddress  << arg->AsChar();
		} else if (arg->IsInt32()) {
			ossWithAddress  << arg->AsInt32();
			ossWithoutAddress  << arg->AsInt32();
		} else if (arg->IsString()) {
			ossWithAddress  << arg->AsString();
			ossWithoutAddress  << arg->AsString();
		}else if (arg->IsFloat()) {
			floatCorrection.str("");

			floatCorrection << arg->AsFloat();

			std::string floatWithPoint = floatCorrection.str();

			if(floatWithPoint.find(".") == floatWithPoint.npos) {
				floatWithPoint = floatWithPoint + ".";
			}

			ossWithAddress  << floatWithPoint;
			ossWithoutAddress  << floatWithPoint;
		}

		++arg;
		if(arg != m.ArgumentsEnd()){
			ossWithAddress << " ";
			ossWithoutAddress << " ";
		}
	}

	m_getStringWithAddress = ossWithAddress.str();
	m_getStringWithoutAddress = ossWithoutAddress.str();
	
	m_state = ANSWER_RECEIVED;
}

std::string MinuitGetAnswer::waitGetAnswer(bool withAddress)
{
	if (withAddress) {
		return m_getStringWithAddress;
	} else {
		return m_getStringWithoutAddress;
	}
}
