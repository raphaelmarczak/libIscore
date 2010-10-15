#ifndef MINUITNAMESPACEANSWER_HPP_
#define MINUITNAMESPACEANSWER_HPP_

#include "MinuitInclude.hpp"
#include "ReceiveOSCThread.hpp"

#include <vector>
#include <string>

class MinuitNamespaceAnswer
{
public:
	MinuitNamespaceAnswer();
	virtual ~MinuitNamespaceAnswer();

	int getState();
	void setTimeOut(int timeout);

	std::string getIp();
	void setIp(std::string ip);

	unsigned int getPort();
	void setPort(unsigned int port);

	void parseMinuitNamespaceAnswer(const osc::ReceivedMessage&m);

	void getNamespaceRequestAnswer(std::vector<std::string> *nodes,
								   std::vector<std::string> *leaves,
								   std::vector<std::string> *attributs);

private:
	int m_state;

	long long m_launchTimeInMs;

	int m_timeOutInMs;

	std::string m_ip;
	unsigned int m_port;

	std::vector<std::string> m_nodes;
	std::vector<std::string> m_leaves;
	std::vector<std::string> m_attributs;
};

#endif /*MINUITNAMESPACEANSWER_HPP_*/
