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

using namespace std;

Controller::Controller()
{
	deviceId = 0;
	factories = new PluginFactories();
	netDevices = new map<string, Device*>;
	netPlugins = new map<string, Plugin*>;
	m_namespace = new map<unsigned int, string>;
	m_values = new map<unsigned int, unsigned int>;

	//add the first device with the id 0 which is the sequencer
	//addDevice("/Sequencer", "OSC2", "127.0.0.1", "7002");

}

Controller::~Controller()
{
	netDevices->clear();
	netPlugins->clear();
	m_namespace->clear();
	m_values->clear();
	delete netDevices;
	delete netPlugins;
	delete factories;
	delete m_namespace;
	delete m_values;
}

void waitedMessageCallback(void *object, string address)
{
	Controller *controller = (Controller*) object;

	if (controller->m_waitedMessageAction != NULL) {
		controller->m_waitedMessageAction(controller->m_waitedMessageActionArgument, address);
	}
}

void
Controller::addWaitedMessageAction(void* arg, void(*pt2Func)(void*, string))
{
	m_waitedMessageAction = pt2Func;
	m_waitedMessageActionArgument = arg;

	map<string, Plugin*>::iterator itr = netPlugins->begin();
	while (itr != netPlugins->end()) {
		itr->second->addWaitedMessageAction(&waitedMessageCallback, this);
		itr++;
	}
}



/************************************************
 PLUGINS METHODS :
 a set of methods used to manage plugins.
 note : each method of this set is prepend by 'plugin'
 ************************************************/

/***********NEW METHODS*******************/

void
Controller::pluginLoad(string path)
{
	factories->loadPlugins(path);

	//create an instance of each plugin available
	IteratorPluginNames it = factories->getPluginNames();
	while (it.hasNext()) {
		string pname = it.next();
		cout << pname << endl;
		Plugin *p = factories->createPlugin(pname, this);
		if (p != 0) {
			netPlugins->insert((pair<string, Plugin*>(pname, p)));
		}
	}

	//define parameters and run a reception thread for each plugin available
	map<string, Plugin*>::iterator itr = netPlugins->begin();
	while (itr != netPlugins->end()) {
		if (itr->first.compare("CopperLAN") != 0) {
			itr->second->commDefineParameters();
			itr->second->commRunReceivingThread();
		}

		itr++;
	}
}

vector<string>
Controller::pluginGetLoadedByName()
{
	map<string, Plugin*>::iterator it  = netPlugins->begin();

	vector<string> result;

	while (it != netPlugins->end()) {
		//std::cout << it->first << std::endl;
		result.push_back(it->first);
		++it;
	}

	return result;
}

string
Controller::pluginGetCommParameter(string pluginName, string parameterName)
{
	map<string, Plugin*>::iterator it  = netPlugins->find(pluginName);

	if (it != netPlugins->end()) {
		return it->second->commGetParameter(parameterName);
	} else {
		std::cout << "No Plugin named : " << pluginName << std::endl;
		return "ERROR";
	}
}

//Plugin *
//Controller::pluginGetInstance(std::string pluginName)
//{
//	factories->loadPlugins("Plugins");
//
//	Plugin *p = factories->createPlugin(pluginName, this);
//	if(p == NULL) {
//		cout << "Controller::couldn't create instance of plugin " << pluginName << endl;
//		return NULL;
//	}
//
//	return p;
//}


/***********DEPRECATED METHODS*******************/

void
Controller::initPlugins(string path)
{
	cout << "Method Controller::initPlugins(std::string path) is DEPRECATED. Use pluginLoad(std::string path)" << endl;

	factories->loadPlugins(path);

	//create an instance of each plugin available
	IteratorPluginNames it = factories->getPluginNames();
	while (it.hasNext()) {
		string pname = it.next();
		Plugin *p = factories->createPlugin(pname, this);
		if (p != 0) {
			netPlugins->insert((pair<string, Plugin*>(pname, p)));
		}
	}

	//define parameters and run a reception thread for each plugin available
	map<string, Plugin*>::iterator itr = netPlugins->begin();
	while( itr != netPlugins->end()){
		if (itr->first.compare("CopperLAN") != 0) {
			itr->second->commDefineParameters();
			itr->second->commRunReceivingThread();
		}
		itr++;
	}
}

vector<string>
Controller::getLoadedPluginsName()
{
	cout << "Method Controller::getLoadedPluginsName() is DEPRECATED. Use pluginGetLoadedByName()" << endl;

	map<string, Plugin*>::iterator it  = netPlugins->begin();

	vector<string> result;

	while (it != netPlugins->end()) {
		//std::cout << it->first << std::endl;
		result.push_back(it->first);
		++it;
	}

	return result;
}

unsigned int Controller::getPluginListeningPort(string pluginName)
{
	cout << "Method unsigned int Controller::getPluginListeningPort(string pluginName) is DEPRECATED. Use std::string pluginGetCommParameter(std::string pluginName, std::string parameterName)" << endl;

//	map<string, Plugin*>::iterator it  = netPlugins->find(pluginName);
//
//	if (it != netPlugins->end()) {
//		return it->second->getPort();
//	}

	string port = getPluginCommParameter(pluginName, "port");
	if (port.compare("ERROR") != 0) {
		return atoi(port.c_str());
	}

	return 0;
}

string
Controller::getPluginCommParameter(string pluginName, string parameterName)
{
	cout << "Method string Controller::getPluginCommParameter(string pluginName, string parameterName) is DEPRECATED. Use std::string pluginGetCommParameter(std::string pluginName, std::string parameterName)" << endl;

	map<string, Plugin*>::iterator it  = netPlugins->find(pluginName);

	if (it != netPlugins->end()) {
		return it->second->commGetParameter(parameterName);
	} else {
		std::cout << "No Plugin named : " << pluginName << std::endl;
		return "ERROR";
	}
}





/************************************************
 DEVICES METHODS :
 a set of methods used to manage devices
 note : each method of this set is prepend by 'device'
 ************************************************/

/*************NEW METHODS************************/

void
Controller::deviceSetCurrent()
{
	//scan the network using each plugin available (by broadcast for osc, by scan for CopperLan?) to find the devices
	map<string, Plugin*>::iterator itr = netPlugins->begin();
	while( itr != netPlugins->end()){
		if (itr->second != NULL) {
			itr->second->deviceSetCurrent(netDevices);
		}
		//cout << itr->first << endl;
		itr++;
	}
}

map<string, Device*>*
Controller::deviceGetCurrent()
{
	return netDevices;
}

int
Controller::deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters)
{
//	if (deviceName.find_first_of("/") != 0) {
//		deviceName = "/" + deviceName;
//	}

	map<string, Device*>::iterator it1 = netDevices->find(deviceName);
	if (it1 != netDevices->end()) {
		cout << "Device name already exists : " << deviceName << endl;
		return -1;
	}

	map<string, Plugin*>::iterator it2  = netPlugins->find(pluginToUse);
	if (it2 == netPlugins->end()) {
		cout << "Plugin doesn't exist : " << pluginToUse << endl;
		return -1;
	} else if (it2->second != NULL) {
		it2->second->deviceAdd(deviceName, commParameters, netDevices);
		//cout << deviceName << endl;
		return 0;
	} else {
		return -1;
	}
}

void
Controller::deviceRemove(std::string deviceName)
{
	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		netDevices->erase(deviceName);
	}
}

bool
Controller::deviceUnderstandNamespaceRequest(string address)
{
	map<string, Device*>::iterator it = netDevices->find(address);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;
		//cout << currentDevice->getPluginToUse() << endl;

		map<string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());

		if (it2 != netPlugins->end()) {
			return it2->second->understandNamespaceRequest();
		} else {
			return false;
		}
	}

	return false;
}

bool
Controller::deviceSendNamespaceRequest(string address)
{
	//add a slash before the address if not present
//	if (address.find_first_of("/") != 0) {
//		address = "/" + address;
//	}
	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2 && address.find_first_of("/") == 0) {
		address += "/";
	}

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	string addressRequest = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {

		Device* currentDevice = it->second;

		map<string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());

		if (it2 != netPlugins->end() && it2->second != NULL) {
			//netPlugins->find(currentDevice->getPluginToUse())->second->deviceSendNamespaceRequest(addressRequest, currentDevice->getAllParameters());
			it2->second->deviceSendNamespaceRequest(deviceName, addressRequest, currentDevice->getAllParameters());

		} else {
			std::cout << "*** Plugin unexistant " << address << std::endl;
			return false;
		}

	} else {
		std::cout << "*** Namespace request failed on " << address << std::endl;
		return false;
	}

	return true;
}

int
Controller::deviceGetNamespaceAnswer(string address, vector<string>* nodes, vector<string>* leaves, vector<string>* attributs, vector<string>* attributsValue)
{
	//add a slash before the address if not present
//	if (address.find_first_of("/") != 0) {
//		address = "/" + address;
//	}
	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2 && address.find_first_of("/") == 0) {
		address += "/";
	}

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	address = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;
		string pluginNameToUse = currentDevice->getPluginToUse();
		Plugin* matchinPlugin = netPlugins->find(pluginNameToUse)->second;

		return matchinPlugin->deviceGetNamespaceAnswer(deviceName, address, nodes, leaves, attributs, attributsValue);
	} else {
		cout << "controller::deviceGetNamespaceAnswer : devicenotfound" << endl;
		return -1;
	}
}

vector<string>
Controller::deviceSnapshot(string address)
{
	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2) {
		address += "/";
	}

	std::vector<std::string> result;

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	string addressRequest = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;

		result = netPlugins->find(currentDevice->getPluginToUse())->second->deviceSnapshot(deviceName, addressRequest, currentDevice->getAllParameters());

		for (unsigned int i = 0; i < result.size(); ++i) {
			result[i] = deviceName + result[i];
		}

	} else {
		std::cout << "*** Snapshot request failed on : " << address << std::endl;
	}

	return result;
}

void
Controller::deviceSendMessage(string stringToSend)
{
	//parse message url until a "/" or " " character to find the device name

	size_t slashIndex;
	slashIndex = stringToSend.find_first_of("/ ", 1);

	if (slashIndex != string::npos) {
		string deviceName = stringToSend.substr(0, slashIndex);
		stringToSend = stringToSend.substr(slashIndex, stringToSend.size());

		map<string, Device*>::iterator it = netDevices->find(deviceName);

//		cout << deviceName << endl;
//		cout << stringToSend << endl;

		if (it != netDevices->end()) {
			Device* currentDevice = netDevices->find(deviceName)->second;

			netPlugins->find(it->second->getPluginToUse())->second->deviceSendMessage(deviceName, stringToSend, currentDevice->getAllParameters());
		} else {
			std::cout << "No device named : " << deviceName << std::endl;
		}
	}
}

bool
Controller::deviceIsVisible(std::string deviceName)
{
	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		return it->second->deviceIsVisible();
	} else {
		std::cout << "No device named : " << deviceName << std::endl;
		return false;
	}
}


/***********DEPRECATED METHODS*******************/

void
Controller::setCurrentDevices()
{
	cout << "Method Controller::setCurrentDevices() is DEPRECATED. Use deviceSetCurrent()" << endl;

	//scan the network using each plugin available (by broadcast for osc, by scan for CopperLan?) to find the devices
	map<string, Plugin*>::iterator itr = netPlugins->begin();
	while( itr != netPlugins->end()){
		itr->second->deviceSetCurrent(netDevices);
		itr++;
	}
}

int
Controller::addDevice(std::string deviceName, std::string pluginToUse, std::string DeviceIp, std::string DevicePort)
{
	cout << "Method Controller::addDevice(std::string deviceName, std::string pluginToUse, std::string DeviceIp, std::string DevicePort) is DEPRECATED.";
	cout << "Use int deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters);" << endl;

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		return -1;
	}

	Device *newDevice = new Device(deviceId++, pluginToUse, DeviceIp, DevicePort);
	netDevices->insert((pair<string, Device*>(deviceName, newDevice)));

	return 0;
}

int
Controller::addDevice(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters)
{
	cout << "Method Controller::addDevice(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters) is DEPRECATED.";
	cout << "Use int deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters);" << endl;

	map<string, Device*>::iterator it1 = netDevices->find(deviceName);
	if (it1 != netDevices->end()) {
		cout << "Device name already exist : " << deviceName << endl;
		return -1;
	}

	map<string, Plugin*>::iterator it2  = netPlugins->find(pluginToUse);
	if (it2 == netPlugins->end()) {
		cout << "Plugin doesn't exist : " << pluginToUse << endl;
		return -1;
	} else {
		it2->second->deviceAdd(deviceName, commParameters, netDevices);
		return 0;
	}
}

void
Controller::removeDevice(std::string deviceName)
{
	cout << "Method Controller::removeDevice(std::string deviceName) is DEPRECATED. Use deviceRemove(std::string deviceName)" << endl;

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		netDevices->erase(deviceName);
	}
}

map<string, Device*>*
Controller::getCurrentDevices()
{
	cout << "Method Controller::getCurrentDevices() is DEPRECATED. Use deviceGetCurrent()" << endl;

	return netDevices;
}

bool Controller::understandNamespaceRequest(string address)
{
	map<string, Device*>::iterator it = netDevices->find(address);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;

		map<string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());

		if (it2 != netPlugins->end()) {
			return it2->second->understandNamespaceRequest();
		} else {
			return false;
		}
	}

	return false;
}

vector<string>
Controller::snapshot(string address)
{
	cout << "Method Controller::snapshot(string address) is DEPRECATED. Use deviceSnapshot(std::string address)" << endl;

	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2) {
		address += "/";
	}

	std::vector<std::string> result;

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	string addressRequest = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;

		result = netPlugins->find(currentDevice->getPluginToUse())->second->deviceSnapshot(deviceName, addressRequest, currentDevice->getAllParameters());

		for (unsigned int i = 0; i < result.size(); ++i) {
			result[i] = deviceName + result[i];
		}

	} else {
		std::cout << "*** Snapshot request failed on : " << address << std::endl;
	}

	return result;
}

bool
Controller::sendNamespaceRequest(string address)
{
	cout << "Method Controller::sendNamespaceRequest(string address) is DEPRECATED. Use deviceSendNamespaceRequest(std::string address)" << endl;

	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2) {
		address += "/";
	}

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	string addressRequest = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {

		Device* currentDevice = it->second;

		map<string, Plugin*>::iterator it2 = netPlugins->find(currentDevice->getPluginToUse());

		if (it2 != netPlugins->end()) {
			netPlugins->find(currentDevice->getPluginToUse())->second->deviceSendNamespaceRequest(deviceName, addressRequest, currentDevice->getAllParameters());

		} else {
			std::cout << "*** Plugin unexistant " << address << std::endl;
			return false;
		}

	} else {
		std::cout << "*** Namespace request failed on " << address << std::endl;
		return false;
	}

	return true;
}

int
Controller::getNamespaceRequestAnswer(string address, vector<string>* nodes, vector<string>* leaves, vector<string>* attributs, vector<string>* attributsValue)
{
	cout << "Method Controller::getNamespaceRequestAnswer(string address, vector<string>* nodes, vector<string>* leaves, vector<string>* attributs, vector<string>* attributsValue) is DEPRECATED.";
	cout << "Use deviceGetNamespaceAnswer(std::string address, std::vector<std::string>* nodes, std::vector<std::string>* leaves, std::vector<std::string>* attributs, std::vector<std::string>* attributsValue)" << endl;

	//parse message url until a "/" or " " character to find the device name
	unsigned int nbOfSlash = std::count(address.begin(), address.end(), '/');

	if (nbOfSlash < 2) {
		address += "/";
	}

	string deviceName = address.substr(0, address.find_first_of("/ ", 1));
	address = address.substr(address.find_first_of("/ ", 1), address.size());

	map<string, Device*>::iterator it = netDevices->find(deviceName);

	if (it != netDevices->end()) {
		Device* currentDevice = it->second;
		string pluginNameToUse = currentDevice->getPluginToUse();
		Plugin* matchinPlugin = netPlugins->find(pluginNameToUse)->second;

		return matchinPlugin->deviceGetNamespaceAnswer(deviceName, address, nodes, leaves, attributs, attributsValue);
	} else {
		return -1;
	}
}

void
Controller::sendMessage(string stringToSend)
{
	cout << "Method Controller::sendMessage(string stringToSend) is DEPRECATED. Use deviceSendMessage(std::string stringToSend)" << endl;

	//parse message url until a "/" or " " character to find the device name
	size_t slashIndex;
	slashIndex = stringToSend.find_first_of("/ ", 1);

	if (slashIndex != string::npos) {
		string deviceName = stringToSend.substr(0, slashIndex);
		stringToSend = stringToSend.substr(slashIndex, stringToSend.size());

		map<string, Device*>::iterator it = netDevices->find(deviceName);

		if (it != netDevices->end()) {
			Device* currentDevice = netDevices->find(deviceName)->second;

			netPlugins->find(it->second->getPluginToUse())->second->deviceSendMessage(deviceName, stringToSend, currentDevice->getAllParameters());
		} else {
			std::cout << "No device named : " << deviceName << std::endl;
		}
	}
}




/************************************************
 NAMESPACE METHODS :
 a set of methods used to manage the local namespace
 note : each method of this set is prepend by 'namespace'
 ************************************************/

void
Controller::addTriggerPointLeave(unsigned int triggerId, std::string triggerMessage)
{
	map<unsigned int, string>::iterator it = m_namespace->find(triggerId);

	if (it != m_namespace->end()) {
		it->second = triggerMessage;
	} else {
		m_namespace->insert((pair<unsigned int, string>(triggerId, triggerMessage)));
		m_values->insert((pair<unsigned int, unsigned int>(triggerId, TRIGGER_READY)));
	}
	//cout << "trigger added in controller : " << triggerId << " " << triggerMessage << endl;
}

void
Controller::removeTriggerPointLeave(unsigned int triggerId)
{
	map<unsigned int, string>::iterator it = m_namespace->find(triggerId);

	if (it != m_namespace->end()) {
		m_namespace->erase(triggerId);
		m_values->erase(triggerId);
	}
	//cout << "trigger removed in controller" << endl;

}

void
Controller::setNamespaceValue(string address, int value, std::map<std::string, std::string> optionalArguments)
{
	map<unsigned int, string>::iterator it = m_namespace->begin();

	int key = -1;
	while(it != m_namespace->end()){
		if(it->second == address){
			key = it->first;
		}
		it++;
	}

	if(key != -1){
		m_values->find(key)->second = value;
		//cout << "Controller::Trigger state changed : " << address << " " << value << endl;
	} else {
		cout << "Controller::setNamespaceValue failed : Address doesn't exist"	<< endl;
	}

	//send a message to Stantum device to active the update button flashing
	if(value == TRIGGER_WAITED)
		sendMessage("/StantumUpdating/update");
}

void
Controller::resetTriggerPointStates(){
	map<unsigned int, unsigned int>::iterator it = m_values->begin();

	while (it != m_values->end()) {
		it->second = TRIGGER_READY;
		it++;
	}

	//send a message to Stantum device to reinit the display
	sendMessage("/StantumDisplay/update");
}

map<unsigned int, string>*
Controller::getControllerNamespace()
{
	return m_namespace;
}

void
Controller::getWaitedTriggerPoints(vector<string>* waitedTP)
{
	map<unsigned int, unsigned int>::iterator it = m_values->begin();

	while (it != m_values->end()) {
		if(it->second == TRIGGER_WAITED){
			waitedTP->push_back(m_namespace->find(it->first)->second);
		}
		it++;
	}
}

void
Controller::askControllerNamespaceFor(string address, vector<string>* namespaceVectorToFill)
{
	namespaceVectorToFill->clear();
	map<unsigned int, string>::iterator it = m_namespace->begin();

	//cout << "map :" << endl;

	while (it != m_namespace->end()) {
		//cout << it->second << endl;
		namespaceVectorToFill->push_back(it->second);
		it++;
	}
}

int
Controller::askControllerValueFor(string address)
{
	map<unsigned int, string>::iterator it = m_namespace->begin();
	//cout << "asking value for " << address << endl;

	int key = -1;
	while(it != m_namespace->end()){
		if(it->second == address)
			key = it->first;
		it++;
	}

	if(key != -1){
		return m_values->find(key)->second;
	} else {
		cout << "Controller::askControllerValueFor failed : Address doesn't exist : " << address << endl;
		return -1;
	}
}



