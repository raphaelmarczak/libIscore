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

/*!
 * \file Process.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "Process.hpp"
#include "ProcessStep.hpp"

#include <iostream>
#include <sstream>


void* processMainFunction(void* threadArg)
{
	((ECOProcess*)threadArg)->mainFunction();
	
	return NULL;
}

ECOProcess::ECOProcess(unsigned int id)
{
	m_id = id;
	
	init();
}

ECOProcess::~ECOProcess()
{
}

void ECOProcess::init()
{
	m_state = STOP;
	
	m_currentStepIndex = -1;
	m_lastStepIdReceived = -1;
	
	m_timeOffsetInMs = 0;
	m_speedFactor = 1;
	
	m_writtenTime = 0;
}

std::string ECOProcess::getType() const
{
	return m_type;
}

void ECOProcess::setType(std::string m_type)
{
	this->m_type = m_type;
}

unsigned int ECOProcess::getId() const
 {
     return m_id;
 }

 void ECOProcess::setId(unsigned int m_id)
 {
     this->m_id = m_id;
 }

void ECOProcess::nextStep(unsigned int step)
{
	m_lastStepIdReceived = step;

	if (m_state == STOP) {
		run();
	}
}

void ECOProcess::run()
{
	pthread_create(&m_thread, NULL, processMainFunction, this);
}

void ECOProcess::setTimeOffsetInMs(unsigned int time)
{
	m_timeOffsetInMs = time;
}

void ECOProcess::setSpeedFactor(float factor)
{
	m_speedFactor = factor;
}

void ECOProcess::mainFunction()
{
	m_state = STOP;
}

void ECOProcess::changeState(ECOProcess::state state)
{
	m_state = state;
}

void ECOProcess::setWrittenTime(unsigned int time)
{
	m_writtenTime = time;
}

float ECOProcess::getProgressionPercent()
{
	return 0;
}

void processCallBack(void* arg)
{
	ProcessStep* processStep = (ProcessStep*) arg;
	processStep->m_process->nextStep(processStep->m_step - 1);
}


xmlNodePtr ECOProcess::storeGlobalProcess(xmlNodePtr father)
{
	xmlNodePtr node = NULL;
	node = xmlNewNode(NULL, BAD_CAST "PROCESS");

	std::ostringstream oss;

	oss << getId();

	xmlSetProp(node, BAD_CAST "id", BAD_CAST oss.str().data());

	xmlSetProp(node, BAD_CAST "type", BAD_CAST getType().data());

	xmlAddChild(father, node);

	return node;
}

void ECOProcess::store(xmlNodePtr father)
{

}

void ECOProcess::load(xmlNodePtr root)
{

}
