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

#ifndef MINUITDISCOVERANSWER_HPP_
#define MINUITDISCOVERANSWER_HPP_

#include "MinuitInclude.hpp"
#include "ReceiveOSCThread.hpp"

#include <vector>
#include <string>

class MinuitDiscoverAnswer
{
public:
	MinuitDiscoverAnswer();
	virtual ~MinuitDiscoverAnswer();

	int getState();
	void setTimeOut(int timeout);

	std::string getIp();
	void setIp(std::string ip);

	unsigned int getPort();
	void setPort(unsigned int port);

	void parseMinuitDiscoverAnswer(const osc::ReceivedMessage&m);

	void waitDiscoverAnswer(std::vector<std::string> *returnedNodes, std::vector<std::string> *returnedLeaves, std::vector<std::string> *returnedAttributes);

private:
	int m_state;

	long long m_launchTimeInMs;

	int m_timeOutInMs;

	std::string m_ip;
	unsigned int m_port;

	std::vector<std::string> m_nodes;
	std::vector<std::string> m_leaves;
	std::vector<std::string> m_attributes;
};

#endif /*MINUITDISCOVERANSWER_HPP_*/
