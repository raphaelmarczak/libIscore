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
 *  Minuit.cpp
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

/*!
 * \class Minuit
 *
 * It's the Minuit plugin which use the Open Sound Control protocol 
 * 
 */

#include "Controller.h"
#include "Plugin.h"
#include "Device.h"

#include "Minuit/MinuitInclude.hpp"
#include "Minuit/MinuitCommunicationMethods.h"

#include <sstream>

static const unsigned int MINUIT_RECEPTION_PORT = 7002;

void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address);
void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute);
void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value);
void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable);

class Minuit : public Plugin {
	
private:
	
	MinuitCommunicationMethods *m_minuitMethods;		//this class contains the osc communication methods
	
	friend void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address);
	friend void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute);
	friend void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value);
	friend void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable);
	
public:
	
	~Minuit() 
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
		m_minuitMethods->minuitRunOSCListening(MINUIT_RECEPTION_PORT);
		
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
	 * Define the device parameters needs by each device to communicate
	 *
	 */
	void commDefineParameters()
	{
		m_commParameterNames = new std::vector<std::string>;
		m_commParameterNames->push_back("ip");
		m_commParameterNames->push_back("port");
		m_commParameterNames->push_back("pluginReceptionPort");
	}
	
	/*!
	 * Get a device parameter like a reception port for example
	 * All parameters names and values are defined by the plugin itself
	 * This method doesn't have to be implemented by the plugin child if it doesn't need
	 *
	 * \param paramaterName : the name of the parameter you want the value
	 * \return the string value for this parameter name or "ERROR" if the name corresponds to any parameter
	 */
	std::string commGetParameter(std::string paramaterName)
	{
		if(paramaterName.compare("pluginReceptionPort")==0)
		{
			std::ostringstream oss;
			oss << MINUIT_RECEPTION_PORT;
			return oss.str();;
		} else {
			std::cout << "No Parameter named : " << paramaterName << "for minuit plugin" << std::endl;
			return "ERROR";
		}
	}
	
	
	
	/************************************************
	 DEVICES METHODS :
	 a set of methods used to manage devices
	 note : each method of this set is prepended by 'device'
	 ************************************************/
	
	/*!
	 * Scan the network to find the device which want to communicate.
	 * The deamon is not implemented here but it's possible to develop a udp broadcast
	 *
	 * \param netDevices : the Device map which is going to be full with the network OSC2 devices. 
	 */	
	void deviceSetCurrent(std::map<std::string, Device*> *netDevices)
	{
		// TEST : add 3 minuit devices to communicate with the Stantum device
		std::map<std::string, std::string> *deviceParameters = new std::map<std::string, std::string>;
		
		deviceParameters->insert(std::pair<std::string, std::string>("ip", "127.0.0.1"));
		deviceParameters->insert(std::pair<std::string, std::string>("port", "10000"));
		
		netDevices->insert(std::pair<std::string, Device*>("StantumUpdating", new Device("StantumUpdating", "Minuit", deviceParameters, false)));
		
		deviceParameters->find("port")->second = "10001";
		netDevices->insert(std::pair<std::string, Device*>("StantumReseting", new Device("StantumReseting", "Minuit", deviceParameters, false)));
		
		deviceParameters->find("port")->second = "10002";
		netDevices->insert(std::pair<std::string, Device*>("StantumDisplay", new Device("StantumDisplay", "Minuit", deviceParameters, false)));
		
		deviceParameters->find("port")->second = "10003";
		netDevices->insert(std::pair<std::string, Device*>("TpTouch", new Device("TpTouch", "Minuit", deviceParameters, false)));
		
		// TEST : add 1 default minuit device to communicate with any environment
		deviceParameters->find("port")->second = "9998";
		deviceAdd("MinuitDevice1", deviceParameters, netDevices);
	}
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	void deviceAdd(std::string deviceName, std::map<std::string, std::string> *commParameters, std::map<std::string, Device*> *netDevices)
	{
		Device *device = new Device(deviceName, "Minuit", commParameters);
		netDevices->insert(std::pair<std::string, Device*>(deviceName, device));
		
	}
	
	/*!
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandDiscoverRequest()
	{
		return true;
	}
	
	
	
	
	/**************************************************************************************************************************
	 *
	 *	SEND REQUEST METHODS
	 *
	 **************************************************************************************************************************/
	
	/*!
	 * Send a discover request to a device to get a part of the namespace at the given address
	 *
 	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param returnedNodes : the vector which is going to be full with the node names at the given address
	 * \param returnedLeaves : the vector which is going to be full with the leaf names at the given address
	 * \param returnedAttributes : the vector which is going to be full with the attributes names at the given address
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int deviceSendDiscoverRequest(Device* device, Address address, std::vector<std::string>* returnedNodes, std::vector<std::string>* returnedLeaves, std::vector<std::string>* returnedAttributes)
	{
		int state;
		std::string stringToSend;
		std::string name = device->getName();
		std::string ip = device->getCommParameter("ip");
		std::string port = device->getCommParameter("port");
		
		// edit request
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_REQUEST_DISCOVER;
		stringToSend += " ";
		stringToSend += address;
		
		// send request
		//	std::cout << "Minuit : deviceSendDiscoverRequest : " << stringToSend << std::endl;
		m_minuitMethods->minuitSendMessage(stringToSend, ip, toInt(port));
		
		// Wait for an answer from an IP on a specific port
		m_minuitMethods->minuitAddDiscoverAnswer(name, address, ip, toInt(port), DEFAULT_TIMEOUT);
		
		state = ANSWER_RECEIVED;
		do
		{
			usleep(1000);
			state = m_minuitMethods->minuitWaitDiscoverAnswer(name, address, returnedNodes, returnedLeaves, returnedAttributes);
		}
		while(state == NO_ANSWER);

		return state;
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
	int deviceSendGetRequest(Device* device, Address address, std::string attribute, Value* returnedValue)
	{	
		int state;
		std::string name = device->getName();
		std::string addressAndAttribute;
		std::string stringToSend;
		
		// edit request
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_REQUEST_GET;
		stringToSend += " ";
		stringToSend += address;
		stringToSend += ":";
		stringToSend += attribute;
		
		// send request
		//	std::cout << "Minuit : deviceSendGetRequest : " << stringToSend << std::endl;
		m_minuitMethods->minuitSendMessage(stringToSend, device->getCommParameter("ip"), toInt(device->getCommParameter("port")));
		
		// Wait for an answer from an IP on a specific port
		addressAndAttribute = address + ":" + attribute;
		m_minuitMethods->minuitAddGetAnswer(name, addressAndAttribute, DEFAULT_TIMEOUT);
		
		state = ANSWER_RECEIVED;
		do
		{
			usleep(1000);
			state = m_minuitMethods->minuitWaitGetAnswer(name, addressAndAttribute, returnedValue, false);
		}
		while(state == NO_ANSWER);
		
		return state;
	}
	
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
	 * Send a listen request to a specific device
	 *
	 * \param device : a pointer to a Device instance
	 * \param address : something like "/<subDeviceName>/.../<input>"
	 * \param attribute : the attribute to listen
	 * \param enable : enable/disable the listening
	 */
	int deviceSendListenRequest(Device* device, Address address, std::string attribute, bool enable)
	{
		std::string addressAndAttribute;
		std::string stringToSend;
		
		// edit request
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_REQUEST_LISTEN;
		stringToSend += " ";
		stringToSend += address;
		stringToSend += ":";
		stringToSend += attribute;
		
		stringToSend += "";
		if(enable)
			stringToSend += MINUIT_REQUEST_LISTEN_ENABLE;
		else
			stringToSend += MINUIT_REQUEST_LISTEN_DISABLE;
		
		// send request
		std::cout << "Minuit : deviceSendListenRequest : " << stringToSend << std::endl;
		m_minuitMethods->minuitSendMessage(stringToSend, device->getCommParameter("ip"), toInt(device->getCommParameter("port")));
		
		return 1;
	}
	
	
	/**************************************************************************************************************************
	 *
	 *	SEND ANSWER METHODS
	 *
	 **************************************************************************************************************************/
	
	/*!
	 * Send a disover answer to a device which ask for.
	 *
	 * \param to : the device where to send answer
	 * \param address : the address where comes from the description
	 * \param returnedNodes : the description of nodes below the address
	 * \param returnedLeaves : the description of leaves below the address
	 * \param returnedAttributes : the description of attributes at the address
	 */
	void deviceSendDiscoverAnswer(Device* to, std::string address, std::vector<std::string>& returnedNodes, std::vector<std::string>& returnedLeaves, std::vector<std::string>& returnedAttributes)
	{
		std::vector<std::string>::const_iterator iter;
		std::string stringToSend;
		
		// edit answer
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_ANSWER_DISCOVER;
		stringToSend += " ";
		stringToSend += address;
		
		// add each nodes
		if(returnedNodes.size()){
			stringToSend += " ";
			stringToSend += MINUIT_START_NODES;
			stringToSend += " ";
			for(iter = returnedNodes.begin(); iter != returnedNodes.end(); iter++)
			{
				stringToSend += *iter;
				stringToSend += " ";
			}
			stringToSend += MINUIT_END_NODES;
		}
		
		// add each leaves
		if(returnedLeaves.size()){
			stringToSend += " ";
			stringToSend += MINUIT_START_LEAVES;
			stringToSend += " ";
			for(iter = returnedLeaves.begin(); iter != returnedLeaves.end(); iter++)
			{
				stringToSend += *iter;
				stringToSend += " ";
			}
			stringToSend += MINUIT_END_LEAVES;
		}
		
		// add each attributes
		if(returnedAttributes.size()){
			stringToSend += " ";
			stringToSend += MINUIT_START_ATTRIBUTES;
			stringToSend += " ";
			for(iter = returnedAttributes.begin(); iter != returnedAttributes.end(); iter++)
			{
				stringToSend += *iter;
				stringToSend += " ";
			}
			stringToSend += MINUIT_END_ATTRIBUTES;
		}
		
		// DEBUG
		std::cout << "Minuit : deviceSendNamespaceAnswer : " << stringToSend << std::endl;
		
		// send answer
		m_minuitMethods->minuitSendMessage(stringToSend, to->getCommParameter("ip"), toInt(to->getCommParameter("port")));
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
		std::string stringToSend;
		
		// edit answer
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_ANSWER_GET;
		stringToSend += " ";
		stringToSend += address;
		stringToSend += ":";
		stringToSend += attribute;
		
		// add value
		stringToSend += " ";
		stringToSend += returnedValue;

		// DEBUG
		std::cout << "Minuit : deviceSendGetAnswer : " << stringToSend << std::endl;
		
		//send answer
		m_minuitMethods->minuitSendMessage(stringToSend, to->getCommParameter("ip"), toInt(to->getCommParameter("port")));
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
		std::string stringToSend;
		
		// edit answer
		stringToSend = m_controller->namespaceApplicationName();	// add name of application
		stringToSend += MINUIT_ANSWER_LISTEN;
		stringToSend += " ";
		stringToSend += address;
		stringToSend += ":";
		stringToSend += attribute;
		
		// add value
		stringToSend += " ";
		stringToSend += returnedValue;
		
		// DEBUG
		std::cout << "Minuit : deviceSendListenAnswer : " << stringToSend << std::endl;
		
		//send answer
		m_minuitMethods->minuitSendMessage(stringToSend, to->getCommParameter("ip"), toInt(to->getCommParameter("port")));
	}
	
	
	
	
	/**************************************************************************************************************************
	 *
	 *	RECEIVE ANSWER METHODS : No methods because each plugin deals with answers when it send a request (maybe we could add an answer manager)
	 *
	 **************************************************************************************************************************/
	
};

/*!
 *
 * The receive callback methods
 *
 */

void receiveDiscoverRequestCallback(void* arg, std::string from, std::string address)
{
	Device* fromDevice;
	Address whereToDiscover = address;
	Minuit* minuit = (Minuit*) arg;
	
	/******************************************************************************************/
	/**This part is temporarily used to communicate with TPTouch waiting for a tree structure**/
	
	std::vector<std::string> nodes;
	std::vector<std::string> triggerNamespace;
	std::vector<std::string> attributes;
	minuit->m_controller->askControllerNamespaceFor(address, &triggerNamespace);

	//send a message to Stantum device to stop the uptodate button flashing
	if(address == "/TriggerPoints") {
		minuit->m_controller->deviceSendSetRequest((std::string)("StantumReseting/reset 1"));
	}
	
	/******************************************************************************************/
	
	// get device
	if(fromDevice = minuit->m_controller->deviceGet(from)) {
		// Use built-in plugin method
		//minuit->deviceReceiveDiscoverRequest(fromDevice, whereToDiscover);
		minuit->deviceSendDiscoverAnswer(fromDevice, whereToDiscover, nodes, triggerNamespace, attributes);
	}
}

void receiveGetRequestCallback(void* arg, std::string from, std::string address, std::string attribute)
{
	Device* fromDevice;
	Address whereToGet = address;
	Minuit* minuit = (Minuit*) arg;
	
	/******************************************************************************************/
	/**This part is temporarily used to communicate with TPTouch waiting for a tree structure**/
	
	Value value = minuit->m_controller->askControllerValueFor(address, attribute);

	/******************************************************************************************/
	
	// get device
	if(fromDevice = minuit->m_controller->deviceGet(from)) {
		// Use built-in plugin method
		//minuit->deviceReceiveGetRequest(fromDevice, whereToGet, attribute);

		minuit->deviceSendGetAnswer(fromDevice, whereToGet, attribute, value);
	}
}

void receiveSetRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, std::string value)
{
	Device* fromDevice;												// this is optionnal (used to notify in case of error)
	Address whereToSet = address;
	Value newValue = value;
	Minuit* minuit = (Minuit*) arg;
	
	// get device
	fromDevice = minuit->m_controller->deviceGet(from);
	
	// Use built-in plugin method (fromDevice could be NULL)
	minuit->deviceReceiveSetRequest(fromDevice, whereToSet, attribute, newValue);
}

void receiveListenRequestCallBack(void* arg, std::string from, std::string address, std::string attribute, bool enable)
{
	Device* fromDevice;
	Address whereToListen = address;
	Minuit* minuit = (Minuit*) arg;
	
	// get device
	if(fromDevice = minuit->m_controller->deviceGet(from))
		// Use built-in plugin method
		minuit->deviceReceiveListenRequest(fromDevice, whereToListen, attribute, enable);
}


/*!
 * \class MinuitFactory
 *
 * It's the Minuit plugin informations
 * 
 */
class MinuitFactory : public PluginFactory {
	std::string getPluginName(){return "Minuit";}
	std::string getPluginVersion(){return "0.2";}
	std::string getPluginAuthor(){return "Raphael Marczak/Laurent Garnier";}
	Plugin *getInstance(Controller *controller){
		Minuit *minuit = new Minuit(); 
		minuit->setController(controller);
		return minuit;
	}
};

extern "C" {
	PluginFactory* createFactory() {
		return new MinuitFactory();
	}
}


