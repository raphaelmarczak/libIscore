/*
Copyright: 
LaBRI (http://www.labri.fr) - BlueYeti (http://www.blueyeti.fr) - GMEA (http://www.gmea.net)

Author(s): Laurent Garnier, Theo Delahogue
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
 *  Controller.cpp
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include "Controller.h"
#include "Device.h"
#include "PluginFactories.h"
#include "Plugin.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <sstream>

Controller::Controller(std::string aName)
{
	m_application_name = aName;
	
	deviceId = 0;
	factories = new PluginFactories();
	netDevices = new std::map<std::string, Device*>;
	netPlugins = new std::map<std::string, Plugin*>;
	
	m_namespace = new std::map<unsigned int, std::string>;
	m_values = new std::map<unsigned int, unsigned int>;
}

Controller::~Controller()
{
	netDevices->clear();
	netPlugins->clear();

	delete netDevices;
	delete netPlugins;
	delete factories;
	delete m_namespace;
	delete m_values;
}

/************************************************
 PLUGINS METHODS :
 a set of methods used to manage plugins.
 note : each method of this set is prepend by 'plugin'
 ************************************************/

void
Controller::pluginLoad(std::string path)
{	
	factories->loadPlugins(path);
	
	//create an instance of each plugin available
	IteratorPluginNames it = factories->getPluginNames();
	while (it.hasNext()) {
		std::string pname = it.next();
		std::cout << pname << std::endl;
		Plugin *p = factories->createPlugin(pname, this);
		if (p != 0) {
			netPlugins->insert((std::pair<std::string, Plugin*>(pname, p)));
		}
	}
	
	//define parameters and run a reception thread for each plugin available
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	while (itr != netPlugins->end()) {		
		if (itr->first.compare("CopperLAN") != 0) {
			itr->second->commDefineParameters();		
			itr->second->commRunReceivingThread();
		}
		
		itr++;
	}	
}

std::vector<std::string>
Controller::pluginGetLoadedByName() 
{	
	std::map<std::string, Plugin*>::iterator it  = netPlugins->begin();
	
	std::vector<std::string> result;
	
	while (it != netPlugins->end()) {
		//std::cout << it->first << std::endl;
		result.push_back(it->first);
		++it;
	}
	
	return result;
}

std::string
Controller::pluginGetCommParameter(std::string pluginName, std::string parameterName)
{	
	std::map<std::string, Plugin*>::iterator it  = netPlugins->find(pluginName);
	
	if (it != netPlugins->end()) {
		return it->second->commGetParameter(parameterName);
	} else {
		std::cout << "Controller::pluginGetCommParameter No Plugin named : " << pluginName << std::endl;
		return "ERROR";
	}
}

bool
Controller::pluginIsLoaded(std::string pluginName)
{
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	
	while(itr != netPlugins->end())
	{		
		if(itr->first.compare(pluginName))
		{
			return true;
		}
		itr++;
	}	
	
	return false;
}


/************************************************
 DEVICES METHODS :
 a set of methods used to manage devices
 note : each method of this set is prepend by 'device'
 ************************************************/

void 
Controller::deviceSetCurrent()
{
	
	//scan the network using each plugin available (by broadcast for osc, by scan for CopperLan?) to find the devices
	std::map<std::string, Plugin*>::iterator itr = netPlugins->begin();
	while( itr != netPlugins->end()){
		if (itr->second != NULL) {
			itr->second->deviceSetCurrent(netDevices);
		}
	
		itr++;
	}
}

std::map<std::string, Device*>*
Controller::deviceGetCurrent()
{
	return netDevices;
}

Device* 
Controller::deviceGet(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = this->netDevices->find(deviceName);
	
	if (it == netDevices->end())
		return NULL;
	
	return it->second;
}

int
Controller::deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters)
{	
	std::map<std::string, Device*>::iterator it1 = netDevices->find(deviceName);
	if (it1 != netDevices->end()) {
		std::cout << "Controller::deviceAdd the device name already exists : " << deviceName << std::endl;
		return -1;
	}
	
	std::map<std::string, Plugin*>::iterator it2  = netPlugins->find(pluginToUse);
	
	if (it2 == netPlugins->end()) {
		std::cout << "Controller::deviceAdd the plugin doesn't exist : " << pluginToUse << std::endl;
		return -1;
	} else if (it2->second != NULL) {
		it2->second->deviceAdd(deviceName, commParameters, netDevices);	
		return 0;
	} else {
		return -1;	
	}
}

void 
Controller::deviceRemove(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		netDevices->erase(deviceName);
	}
}

bool 
Controller::deviceUnderstandDiscoverRequest(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		Device* currentDevice = it->second;
		
		std::map<std::string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());
		
		if (it2 != netPlugins->end()) {
			return it2->second->understandDiscoverRequest();
		} else {
			return false;
		}
	}
	
	return false;
}


int 
Controller::deviceSendDiscoverRequest(std::string deviceName, Address address, std::vector<std::string>* returnedNodes, std::vector<std::string>* returnedLeaves, std::vector<std::string>* returnedAttributes)
{
	Device* currentDevice;
	std::map<std::string, Device*>::iterator it;
	std::map<std::string, Plugin*>::iterator it2;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		
		currentDevice = it->second;
		
		// find the plugin
		it2 = netPlugins->find(currentDevice->getPluginToUse());
		
		if (it2 != netPlugins->end() && it2->second != NULL)
			return it2->second->deviceSendDiscoverRequest(currentDevice, address, returnedNodes, returnedLeaves, returnedAttributes);
		else
			std::cout << "Controller::deviceSendDiscoverRequest : no plugin for the device named " << deviceName << std::endl;
	}else
		std::cout << "Controller::deviceSendDiscoverRequest : no device named " << deviceName << std::endl;

	return NO_ANSWER;
}

int 
Controller::deviceSendDiscoverRequest(std::string deviceAndAddress, std::vector<std::string>* returnedNodes, std::vector<std::string>* returnedLeaves, std::vector<std::string>* returnedAttributes, std::vector<Value>* returnedValues)
{
	int state;
	unsigned int i;
	Value returnedValue;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSendDiscoverRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	state = deviceSendDiscoverRequest(deviceName, address, returnedNodes, returnedLeaves, returnedAttributes);
	
	// DEBUG
	//std::cout << "Controller::deviceSendDiscoverRequest NODES" << std::endl;
	//for (unsigned int i = 0; i < (*returnedNodes).size(); ++i)
	//std::cout << "                                       " << (*returnedNodes)[i] << std::endl;

	//std::cout << "Controller::deviceSendDiscoverRequest LEAVES" << std::endl;
	//for (unsigned int i = 0; i < (*returnedLeaves).size(); ++i)
	//std::cout << "                                       " << (*returnedLeaves)[i] << std::endl;

	//std::cout << "Controller::deviceSendDiscoverRequest ATTRIBUTES" << std::endl;
	//for (unsigned int i = 0; i < (*returnedAttributes).size(); ++i)
	//std::cout << "                                       " << (*returnedAttributes)[i] << std::endl;

	
	if(state == ANSWER_RECEIVED){
		
		// for each attribute of the node : get the value
		//returnedValues = new vector<Value>();
		for(i = 0; i < returnedAttributes->size(); i++){
			
			state = deviceSendGetRequest(deviceName, address, returnedAttributes->at(i), &returnedValue);
			
			if(state == ANSWER_RECEIVED)
				returnedValues->push_back(returnedValue);
			else
				returnedValues->push_back(NAMESPACE_NO_VALUE);
		}
	}
	
	//std::cout << "Controller::deviceSendDiscoverRequest VALUE" << std::endl;
	//for (unsigned int i = 0; i < (*returnedValues).size(); ++i)
	//std::cout << "                                       " << (*returnedValues)[i] << std::endl;
	
	return state;
}

int
Controller::deviceSendGetRequest(std::string deviceName, Address address, std::string attribute, Value* returnedValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		return netPlugins->find(it->second->getPluginToUse())->second->deviceSendGetRequest(currentDevice, address, attribute, returnedValue);
	}
	else
	  std::cout << "Controller::deviceSendGetRequest no device named " << deviceName << std::endl;
	
	return NO_ANSWER;
}

int 
Controller::deviceSendGetRequest(std::string deviceAndAddress, Value* returnedValue)
{
	int state;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSendGetRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	state = deviceSendGetRequest(deviceName, address, NAMESPACE_ATTR_VALUE, returnedValue);
	
	// DEBUG
	//std::cout << "Controller::deviceSendGetRequest VALUE = " << returnedValue << std::endl;
	
	return state;
}

void 
Controller::deviceSendSetRequest(std::string deviceName, Address address, Value& newValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendSetRequest(currentDevice, address, newValue);
	}
	else
		std::cout << "Controller::deviceSendSetRequest no device named " << deviceName << std::endl;
}

void 
Controller::deviceSendSetRequest(std::string deviceAndAddress, Value& newValue)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSendSetRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendSetRequest(deviceName, address, newValue);
}

void 
Controller::deviceSendSetRequest(std::string deviceAndAddressAndValue)
{
	std::string deviceAndAddress, deviceName;
	Address address;
	Value newValue;
	
	// Parse the value
	unsigned int nbOfSpace = std::count(deviceAndAddressAndValue.begin(), deviceAndAddressAndValue.end(), ' ');
	unsigned int spacePos = deviceAndAddressAndValue.find_first_of(' ');
	
	if((nbOfSpace) && (spacePos != deviceAndAddressAndValue.npos)){
		deviceAndAddress = deviceAndAddressAndValue.substr(0, spacePos);
		newValue = deviceAndAddressAndValue.substr(spacePos, deviceAndAddressAndValue.size());
	}
	else{
		deviceAndAddress = deviceAndAddressAndValue;
		newValue = "";
	}
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);
	//std::cout << "Controller::deviceSendSetRequest deviceName = " << deviceName << " address = " << address << " value = " << newValue << std::endl;
	
	// send the request
	deviceSendSetRequest(deviceName, address, newValue);
}

void 
Controller::deviceSendListenRequest(std::string deviceName, Address address, std::string attribute, bool enable)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendListenRequest(currentDevice, address, attribute, enable);
	}
	else
		std::cout << "Controller::deviceSendListenRequest no device named " << deviceName << std::endl;
}

void 
Controller::deviceSendListenRequest(std::string deviceAndAddress, std::string attribute, bool enable)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSendListenRequest deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendListenRequest(deviceName, address, enable);
}

void 
Controller::deviceSendListenAnswer(std::string deviceName, Address address, std::string attribute, Value& changedValue)
{
	std::map<std::string, Device*>::iterator it;
	Device* currentDevice;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = netDevices->find(deviceName)->second;
		
		// send the request to the device
		netPlugins->find(it->second->getPluginToUse())->second->deviceSendListenAnswer(currentDevice, address, attribute, changedValue);
	}
	else
		std::cout << "Controller::deviceSendListenAnswer no device named " << deviceName << std::endl;
}

void 
Controller::deviceSendListenAnswer(std::string deviceAndAddress, std::string attribute, Value& changedValue)
{
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSendListenAnswer deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	deviceSendListenAnswer(deviceName, address, attribute, changedValue);
}

std::vector<std::string> 
Controller::deviceSnapshot(std::string deviceName, Address address)
{
	Device* currentDevice;
	std::vector<std::string> result;
	std::map<std::string, Device*>::iterator it;
	
	// find the device
	it = netDevices->find(deviceName);
	
	if(it != netDevices->end())
	{
		currentDevice = it->second;
		
		// find the plugin and wait answer from the device
		// Here we used a general recursive process built-in the controller
		result = snapshotProcess(netPlugins->find(currentDevice->getPluginToUse())->second, currentDevice, address);
		
		// edit snapshot by adding the device name before each address of the snapshot
		for(unsigned int i = 0; i < result.size(); ++i)
			result[i] = deviceName + result[i];
	} 
	else
		std::cout << "Controller::deviceSnapshot no device named " << deviceName << std::endl;
	
	return result;
}

std::vector<std::string> 
Controller::deviceSnapshot(std::string deviceAndAddress)
{
	std::vector<std::string> result;
	std::string deviceName;
	Address address;
	
	parseDeviceAndAddress(deviceAndAddress, deviceName, address);	
	//std::cout << "Controller::deviceSnapshot deviceName = " << deviceName << " address = " << address << std::endl;
	
	// send the request
	result = deviceSnapshot(deviceName, address);
	
	// DEBUG
	//std::cout << "Controller::deviceSnaphot" << std::endl;
	//for (unsigned int i = 0; i < result.size(); ++i)
	//std::cout << "			" << result[i] << std::endl;
	
	return result;
}

bool 
Controller::deviceIsVisible(std::string deviceName)
{
	std::map<std::string, Device*>::iterator it = netDevices->find(deviceName);
	
	if (it != netDevices->end()) {
		return it->second->isVisible();
	} else {
		std::cout << "Controller::deviceIsVisible no device named : " << deviceName << std::endl;
		return false;	
	}
}


/*********************************************************
 NAMESPACE METHODS :
 a set of methods used to manage the local namespace 
 note : each method of this set is prepend by 'namespace'
 *********************************************************/

std::string
Controller::namespaceApplicationName()
{
	return m_application_name;
}

void 
Controller::namespaceDiscoverAddCallback(void* arg, void(*pt2Func)(void*, Address, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&))
{
	m_discover_callback = pt2Func;
	m_discover_arguments = arg;
}

void  
Controller::namespaceDiscover(Address whereToDiscover, std::vector<std::string>& returnedNodes, std::vector<std::string>& returnedLeaves, std::vector<std::string>& returnedAttributes)
{
	if(m_discover_callback)
		m_discover_callback(m_discover_arguments , whereToDiscover, returnedNodes, returnedLeaves, returnedAttributes);
	else
		; // TODO : notify the Controller there is no discoverer for this Namespace
}

void 
Controller::namespaceGetAddCallback(void* arg, void(*pt2Func)(void*, Address, std::string, Value&))
{
	m_get_callback = pt2Func;
	m_get_arguments = arg;
}

void  
Controller::namespaceGet(Address whereToGet, std::string attribute, Value& returnedValue)
{
	if(m_get_callback)
		m_get_callback(m_get_arguments , whereToGet, attribute, returnedValue);
	else
		; // TODO : notify the Controller there is no getter for this Namespace
}

void 
Controller::namespaceSetAddCallback(void* arg, void(*pt2Func)(void*, Address, std::string, Value&))
{
	m_set_callback = pt2Func;
	m_set_arguments = arg;
}

void  
Controller::namespaceSet(Address whereToSet, std::string attribute, Value& newValue)
{
	if(m_set_callback)
		m_set_callback(m_set_arguments , whereToSet, attribute, newValue);
	else
		; // TODO : notify the Controller there is no setter for this Namespace
}

void 
Controller::namespaceListenAddCallback(void* arg, void(*pt2Func)(void*, std::string, Address, std::string, bool))
{
	m_listen_callback = pt2Func;
	m_listen_arguments = arg;
}

void  
Controller::namespaceListen(Device* whereToSend, Address whereToListen, std::string attributeToListen, bool enableListening)
{
	if(m_listen_callback)
		m_listen_callback(m_listen_arguments, whereToSend->getName(), whereToListen, attributeToListen, enableListening);
	else
		; // TODO : notify the Controller there is no linker for this Namespace
}


/************************************************
 
 PRIVATE METHODS
 used for internal and specific purposes
 
 ************************************************/
void 
Controller::parseDeviceAndAddress(std::string deviceAndAddress, std::string& device, std::string& address)
{
	// Deal with device name starting with a slash
	// and add a slash in case there is only the device name
	unsigned int nbOfSlash = std::count(deviceAndAddress.begin(), deviceAndAddress.end(), '/');
	unsigned int slashPos = deviceAndAddress.find_first_of('/');
	
	// if deviceAndAddress is something like : "deviceName" or "/deviceName"
	//		add a slash at the end
	if(nbOfSlash == 0 || (nbOfSlash == 1 && (slashPos == 0)))
		deviceAndAddress += "/";
	
	// parse the devicename and the address
	slashPos = deviceAndAddress.find_first_of('/', 1);
	device = deviceAndAddress.substr(0, slashPos);
	address = deviceAndAddress.substr(slashPos, deviceAndAddress.size());
	address += "";
}

std::vector<std::string> 
Controller::snapshotProcess(Plugin *plugin, Device *device, Address address)
{
	unsigned int maxTryAgain = 2;
	unsigned int currentTry = 0;
	unsigned int i;
	int state;
	std::vector<std::string> nodes;
	std::vector<std::string> leaves;
	std::vector<std::string> attributes;
	std::vector<std::string> snapshot;
	std::vector<std::string> snapshotBelow;
	Value getValueAnswer = "";
	std::string currentAttribute, currentLeaf, nodeAddress;
	
	// Ask the namespace at the address
	while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
		
		state = plugin->deviceSendDiscoverRequest(device, address, &nodes, &leaves, &attributes);
		
		if(state == ANSWER_RECEIVED)
			currentTry = maxTryAgain;
		else
			++currentTry;
	}
	
	// look for a 'value' attribute and ask the value
	for(unsigned int i = 0; i < attributes.size(); ++i){
		
		currentAttribute = attributes.at(i);
		
		if(currentAttribute == NAMESPACE_ATTR_VALUE) {
			currentTry = 0;
			
			while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
				
				state = plugin->deviceSendGetRequest(device, address, currentAttribute, &getValueAnswer);
				
				if(state == ANSWER_RECEIVED){
					
					// add the "address value" string to the snaphot
					if(getValueAnswer != "")
						snapshot.push_back(address + " " + getValueAnswer);
					
					// to the while loop
					currentTry = maxTryAgain;
				}
				else
					++currentTry;
			}
		}
	}
	
	// ask the value of each leaf
	for(unsigned int i = 0; i < leaves.size(); ++i){
		
		currentLeaf = leaves.at(i);
		
		currentTry = 0;
		while(currentTry < maxTryAgain){	// is it necessary to make it several times ? --to
			
			state = plugin->deviceSendGetRequest(device, address + "/" + currentLeaf, NAMESPACE_ATTR_VALUE, &getValueAnswer);
			
			if(state == ANSWER_RECEIVED){
				
				// add the "address/leaf value" string to the snaphot
				if(getValueAnswer != "")
					snapshot.push_back(address + "/" + currentLeaf + " " + getValueAnswer);
				
				// to the while loop
				currentTry = maxTryAgain;
			}
			else
				++currentTry;
		}
	}
	
	// Then do a snapshot for each nodes below the address
	for(i = 0; i < nodes.size(); ++i){
		
		// make the absolute address of the node : /address/node
		if (address != "/")
			nodeAddress = address + "/" + nodes.at(i);
		else
			nodeAddress = address + nodes.at(i);
		
		// do a snapshot on it
		snapshotBelow = snapshotProcess(plugin, device, nodeAddress);
		
		// add the snapshot below to the snapshot
		snapshot.insert(snapshot.end(), snapshotBelow.begin(), snapshotBelow.end());
	}
	
	return snapshot;
}





/*********************************************************************
 TEMPORARY METHOD TO ALLOW THE TPITOUCH TO SPEAK WITH THE CONTROLLER
 *********************************************************************/

void
Controller::addTriggerPointLeave(unsigned int triggerId, std::string triggerMessage)
{
	std::map<unsigned int, std::string>::iterator it = m_namespace->find(triggerId);
	
	if (it != m_namespace->end()) {
		it->second = triggerMessage;
	} else {
		m_namespace->insert((std::pair<unsigned int, std::string>(triggerId, triggerMessage)));
		m_values->insert((std::pair<unsigned int, unsigned int>(triggerId, TRIGGER_READY)));
	}
	//cout << "trigger added in controller : " << triggerId << " " << triggerMessage << endl;
}

void
Controller::removeTriggerPointLeave(unsigned int triggerId)
{
	std::map<unsigned int, std::string>::iterator it = m_namespace->find(triggerId);
	
	if (it != m_namespace->end()) {
		m_namespace->erase(triggerId);
		m_values->erase(triggerId);
	}
	//cout << "trigger removed in controller" << endl;
	
}

void
Controller::setNamespaceValue(std::string address, int value, std::map<std::string, std::string> optionalArguments)
{
	std::map<unsigned int, std::string>::iterator it = m_namespace->begin();
	
	int key = -1;
	while(it != m_namespace->end()){
		if(it->second == address){
			key = it->first;
		}
		it++;
	}
	
	if(key != -1){
		m_values->find(key)->second = value;
		m_boxColorArg[key] = optionalArguments["color"];
		m_boxCommentArg[key] = optionalArguments["comment"];
	} else {
		std::cout << "Controller::setNamespaceValue failed : Address doesn't exist"	<< std::endl;
	}
	
	//send a message to Stantum device to active the update button flashing
	if((unsigned int) value == TRIGGER_WAITED) {
		deviceSendSetRequest((std::string)("StantumUpdating/update 1"));
	}
}

void
Controller::resetTriggerPointStates(){
	std::map<unsigned int, unsigned int>::iterator it = m_values->begin();
	
	while (it != m_values->end()) {
		it->second = TRIGGER_READY;
		it++;
	}
	
	//send a message to Stantum device to reinit the display
	deviceSendSetRequest((std::string)("StantumDisplay/update 1"));
}

void
Controller::askControllerNamespaceFor(std::string address, std::vector<std::string>* namespaceVectorToFill)
{
	namespaceVectorToFill->clear();
	std::map<unsigned int, std::string>::iterator it = m_namespace->begin();
	
	while (it != m_namespace->end()) {
		//cout << it->second << endl;
		namespaceVectorToFill->push_back(it->second);
		it++;
	}
}

std::string
Controller::askControllerValueFor(std::string address, std::string attribute)
{
	std::map<unsigned int, std::string>::iterator it = m_namespace->begin();
	//cout << "asking value for " << address << endl;
	
	int key = -1;
	while (it != m_namespace->end()) {
		if (it->second == address) {
			key = it->first;
		}
		it++;
	}
	
	if (key != -1) {
		if (attribute == "value") {
			std::ostringstream stringValue;
			stringValue <<  m_values->find(key)->second;
			return stringValue.str();
		} 
		else if (attribute == "comment") {
			//			map<unsigned int, string>::iterator it = m_boxCommentArg.begin();
			//			while (it != m_boxCommentArg.end()) {
			//				cout << it->second << endl;
			//				it++;
			//			}
			
			if (m_boxCommentArg.find(key) != m_boxCommentArg.end())
				return m_boxCommentArg.find(key)->second;
			else
				return "";
		}
	} else {
		std::cout << "Controller::askControllerValueFor failed : Address doesn't exist : " << address << std::endl;
		return "ERROR";
	}
	
	return "";
}



