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

/*
 *  OSC.cpp
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

/*!
 * \class OSC
 *
 * It's the OSC plugin which use the Open Sound Control protocol 
 * 
 */

#include "Controller.h"
#include "Plugin.h"
#include "Device.h"

#include "Minuit/MinuitInclude.hpp"
#include "Minuit/MinuitCommunicationMethods.h"

#include <sstream>

static const unsigned int OSC_RECEPTION_PORT = 7003;

using namespace std;

void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value);

// unused methods (just here to catch wrong request)
void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address);
void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute);

void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable);

class OSC : public Plugin {
	
private:
	MinuitCommunicationMethods *m_minuitMethods;						//this class contains the osc communication methods
		
	friend void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value);
		
	friend void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address);
	friend void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute);
	friend void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable);
	
public:
	
	~OSC()
	{
		delete m_minuitMethods;	
	}
	
	/************************************************
	 COMMUNICATION METHODS :
	 a set of methods used to init the communication.
	 note : each method of this set is prepend by 'comm'
	 ************************************************/
	
	/*!
	 * Run the message reception thread 
	 * Prepare the receive callback method to be passed to the Controller to intercept the message
	 *
	 */
	void commRunReceivingThread()
	{
		m_minuitMethods = new MinuitCommunicationMethods();
		m_minuitMethods->minuitRunOSCListening(OSC_RECEPTION_PORT);
		
		m_minuitMethods->m_setRequestCallBackArgument = this;
		m_minuitMethods->m_setRequestCallBack = &receiveSetRequestCallBack;
		
		m_minuitMethods->m_discoverRequestCallBackArgument = this;
		m_minuitMethods->m_discoverRequestCallBack = &receiveDiscoverRequestCallback;
		
		m_minuitMethods->m_getRequestCallBackArgument = this;
		m_minuitMethods->m_getRequestCallBack = &receiveGetRequestCallback;
		
		m_minuitMethods->m_listenRequestCallBackArgument = this;
		m_minuitMethods->m_listenRequestCallBack = &receiveListenRequestCallBack;
	}
	
	/*!
	 * Define the plugin parameters to communicate if it needs
	 *
	 */
	void commDefineParameters()
	{
		m_commParameterNames = new vector<string>;
		m_commParameterNames->push_back("ip");
		m_commParameterNames->push_back("port");
		m_commParameterNames->push_back("pluginReceptionPort");
	}
	
	/*!
	 * Get a plugin parameter like a reception port for example
	 * All parameters names and values are defined by the plugin itself
	 *
	 * \param paramaterName : the name of the parameter you want the value
	 * \return the string value for this parameter name or "ERROR" if the name corresponds to any parameter
	 */
	string commGetParameter(string paramaterName)
	{
		if(paramaterName.compare("pluginReceptionPort")==0)
		{
			ostringstream oss;
			oss << OSC_RECEPTION_PORT;
			return oss.str();;
		} else {
			cout << "No Parameter named : " << paramaterName << "for osc plugin" << endl;
			return "ERROR";
		}
	}
	
	
	
	
	/************************************************
	 DEVICES METHODS :
	 a set of methods used to manage devices
	 note : each method of this set is prepend by 'device'
	 ************************************************/
	
	/*!
	 * Scan the network to find the device which want to communicate.
	 * The deamon is not implemented here but it's possible to develop an udp broadcast
	 *
	 * \param netDevices : the Device map which is going to be full with the network OSC2 devices. 
	 */	
	void deviceSetCurrent(map<string, Device*> *netDevices)
	{
		// TEST : add 1 osc devices
		map<string, string> *deviceParameters = new map<string, string>;
		
		deviceParameters->insert(pair<string, string>(m_commParameterNames->at(0),"127.0.0.1"));
		deviceParameters->insert(pair<string, string>(m_commParameterNames->at(1),"9999"));
		
		deviceAdd("OSCDevice1", deviceParameters, netDevices);
		
		cout << "OSC : found 1 device" << endl;
	}
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	void deviceAdd(string deviceName, map<string, string> *commParameters, map<string, Device*> *netDevices)
	{
		Device *device = new Device(deviceName, "OSC", commParameters);
		netDevices->insert(pair<string, Device*>(deviceName, device));
	}
	
	/*!
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandDiscoverRequest() {
		return false;
	}
	
	
	
	
	/**************************************************************************************************************************
	 *
	 *	SEND REQUEST METHODS
	 *
	 **************************************************************************************************************************/
	
	/*!
	 * Send a set request to set a value of a specific device
	 *
	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param value : anything to send
	 */
	int deviceSendSetRequest(Device* device, Address address, Value& newValue)
	{
		std::string stringToSend = address + " " + newValue;
		//	std::cout << "Minuit : deviceSendSetRequest : " << stringToSend << std::endl;
		m_minuitMethods->minuitSendMessage(stringToSend, device->getCommParameter("ip"), toInt(device->getCommParameter("port")));
		
		return 1;
	}
	
	/*!
	 * Send a discover request to a device to get a part of the namespace at the given address
	 *
 	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param returnedNodes : the vector which is going to be full with the node names at the given address
	 * \param returnedNodes : the vector which is going to be full with the leaves names at the given address
	 * \param returnedAttributes : the vector which is going to be full with the attributes names at the given address
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int deviceSendDiscoverRequest(Device* device, Address address, vector<string>* returnedNodes, std::vector<std::string>* returnedLeaves, vector<string>* returnedAttributes)
	{
		return 0;
	}
	
	/*!
	 * Send a get request to a device to get a value at the given address
	 *
 	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param attribute : the asked attribute
	 * \param returnedValue : the Value which is going to be full
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int deviceSendGetRequest(Device* device, Address address, string attribute, Value* returnedValue)
	{		
		return 0;
	}
	
	/*!
	 * Send a listen request to a specific device
	 *
	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param attribute : the attribute to listen
	 * \param enable : enable/disable the listening
	 */
	int deviceSendListenRequest(Device* device, Address address, string attribute, bool enable)
	{
		return 0;
	}
	
	
	/**************************************************************************************************************************
	 *
	 *	SEND ANSWER METHODS
	 *
	 **************************************************************************************************************************/
	
	/*!
	 * Send a namespace answer to a device which ask for.
	 *
	 * \param to : the device where to send answer
	 * \param address : the address where comes from the description
	 * \param returnedNodes : the description of nodes below the address
	 * \param returnedLeaves : the description of leaves below the address
	 * \param returnedAttributes : the description of attributes at the address
	 */
	void deviceSendDiscoverAnswer(Device* to, std::string address, std::vector<std::string>& returnedNodes, std::vector<std::string>& returnedLeaves, std::vector<std::string>& returnedAttributes)
	{
		;
	}
	
	/*!
	 * Send a get answer to a device which ask for.
	 *
	 * \param to : the device where to send answer
	 * \param address : the address where comes from the value
	 * \param attribute : the attribute where comes from the value
	 * \param returnedValue : the value of the attribute at the address
	 */
	void deviceSendGetAnswer(Device* to, Address address, std::string attribute, Value& returnedValue)
	{
		;
	}
	
	/*!
	 * Send a listen answer to a device which ask for.
	 *
	 * \param to : the device where to send answer
	 * \param address : the address where comes from the value
	 * \param attribute : the attribute where comes from the value
	 * \param returnedValue : the value of the attribute at the address
	 */
	void deviceSendListenAnswer(Device* to, Address address, std::string attribute, Value& returnedValue)
	{
		;
	}
	
	
	/**************************************************************************************************************************
	 *
	 *	RECEIVE ANSWER METHODS : No methods because each plugin deals with answers when it send a request (maybe we could add an answer manager)
	 *
	 **************************************************************************************************************************/
};

/*!
 *
 * The receive set callback method
 *
 */
void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value)
{
	Device* fromDevice;												// this is optionnal (used to notify in case of error)
	Address whereToSet = address;
	Value newValue = value;
	OSC* osc = (OSC*) arg;
	
	// get device
	fromDevice = osc->m_controller->deviceGet(from);
	
	// Use built-in plugin method (fromDevice could be NULL)
	osc->deviceReceiveSetRequest(fromDevice, whereToSet, attribute, newValue);
}

void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address)
{
	return;
}

void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute)
{
	return;
}

void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable)
{
	return;
}


/*!
 * \class OSC2Factory
 *
 * It's the OSC2 plugin informations
 * 
 */
class OSCFactory : public PluginFactory {
	string getPluginName(){return "OSC";}
	string getPluginVersion(){return "0.2";}
	string getPluginAuthor(){return "Raphael Marczak/Laurent Garnier";}
	Plugin *getInstance(Controller *controller){
		OSC *osc = new OSC();
		osc->setController(controller);
		return osc;
	}
};

extern "C" {
	PluginFactory* createFactory() {
		return new OSCFactory();
	}
}


