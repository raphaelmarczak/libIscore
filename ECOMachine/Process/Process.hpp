/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak
Last modification: 08/03/2010

Adviser(s): Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)

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

#ifndef PROCESS_HPP_
#define PROCESS_HPP_

/*!
 * \file Process.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#define PROCESS_TYPE_NETWORK_MESSAGE_TO_SEND "networkMessageToSend"

#include <string>
#include <pthread.h>

#include "../../Editor/CSP/XMLInclude.hpp"

/*!
 * \class ECOProcess
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class ECOProcess
{
public:
	enum state{PLAY, STOP};
	ECOProcess(unsigned int id);
	virtual ~ECOProcess();

	virtual void init();

	void nextStep(unsigned int step);
	friend void processCallBack(void* arg);
	friend void* processMainFunction(void* threadArg);
	void run();
	
	void setTimeOffsetInMs(unsigned int time);
	void setSpeedFactor(float factor);

    std::string getType() const;
    void setType(std::string m_type);
    unsigned int getId() const;
    void setId(unsigned int m_id);

	void changeState(ECOProcess::state state);
	
	void setWrittenTime(unsigned int time);
	virtual float getProgressionPercent();

	virtual void store(xmlNodePtr father);
	virtual void load(xmlNodePtr root);
protected :
	virtual void mainFunction();
	xmlNodePtr storeGlobalProcess(xmlNodePtr father);

	unsigned int m_id;
	
	unsigned int m_timeOffsetInMs;
	float m_speedFactor;

	std::string m_type;

	int m_currentStepIndex;
	int m_lastStepIdReceived;
	
	unsigned int m_writtenTime;

	ECOProcess::state m_state;

private:
	pthread_t m_thread;

};

void processCallBack(void*);

#endif /*PROCESS_HPP_*/
