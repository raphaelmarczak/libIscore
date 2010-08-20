#ifndef MINUITGETANSWER_HPP_
#define MINUITGETANSWER_HPP_

#include "MinuitInclude.hpp"
#include "ReceiveOSCThread.hpp"

#include <string>

class MinuitGetAnswer
{
public:
	MinuitGetAnswer();
	virtual ~MinuitGetAnswer();

	void setTimeOut(int timeout);

	int getState();

	void parseMinuitGetAnswer(const osc::ReceivedMessage&m);

	std::string getGetRequestAnswer(bool withAddress = true);

private:
	int m_state;
	long long m_launchTimeInMs;

	int m_timeOutInMs;

	std::string m_getStringWithAddress;
	std::string m_getStringWithoutAddress;
};

#endif /*MINUITGETANSWER_HPP_*/
