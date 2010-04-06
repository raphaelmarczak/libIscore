/*
 *  Plugin.h
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

/*!
 * \class Plugin
 * \author Laurent Garnier (BlueYeti/LaBRI)
 * \date 03/06/09
 *
 * Plugin and PluginFactory are abstract classes, interfaces to develop plugins.
 * Every communication plugins developped in the futur will have to be implemented according to this model.
 * Each methods of both interfaces will have to be redefined in the plugin code.   
 *
 */

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "Device.h"
#include <string>
#include <vector>
#include <map>

class Controller;

class Plugin {
public:
	unsigned int m_port;
	
	std::vector<std::string> *m_commParameterNames;
	Controller *m_controller;
	
	void (*m_waitedMessageAction)(void*, std::string);
	void *m_callBackArgument;
	
	
	/************************************************
	 PLUGIN METHODS :
	 a set of methods used to handle Plugin.
	 ************************************************/
	
	virtual ~Plugin()=0;
	
	/*!
	 * A plugin need to know the Controller to access it methods
	 */
	void setController(Controller *controller){
		m_controller = controller;
	}
	
	Controller *getController(){
		return m_controller;
	}
	
	/*!
	 * Add a reception message callback in the parent class
	 */
	void addWaitedMessageAction(void(*pt2Func)(void *, std::string), void *arg){
		m_waitedMessageAction = pt2Func;
		m_callBackArgument = arg;
	}
	
	
	
	
	/************************************************
	 COMMUNICATION METHODS :
	 a set of methods used to init the communication.
	 note : each method of this set is prepend by 'comm'
	 ************************************************/
	
	/******************NEW METHODS*******************/
	
	/*!
	 * Define the plugin parameters to communicate if it needs
	 *
	 */
	virtual void commDefineParameters()=0;
	
	/*!
	 * Get a plugin parameter like a reception port for example
	 * All parameters names and values are defined by the plugin itself
	 * This method doesn't have to be implemented by the plugin child if it doesn't need
	 *
	 * \param paramaterName : the name of the parameter you want the value
	 * \return the string value for this parameter name or "ERROR" if the name corresponds to any parameter
	 */
	virtual std::string commGetParameter(std::string paramaterName){};
	
	/*!
	 * Run the message reception thread 
	 *
	 */
	virtual void commRunReceivingThread()=0;
	
	
	/***************DEPRECATED METHODS**************/
	
//	/*!
//	 * Run the message reception thread 
//	 *
//	 */
//	virtual void runReceivingThread()=0;
//	
//	unsigned int getPort()
//	{
//		//cout << "DEPRECATED" << endl;
//		return m_port;
//	}
//	
//	/*!
//	 * Define the plugin parameters to communicate if it needs
//	 * Else this method doesn't have to be implemented in the plugin child
//	 *
//	 */
//	virtual void defineParametersToUseWithThisPlugin(){};
//	
//	/*!
//	 * Get a plugin parameter like a reception port for example
//	 * All parameters names and values are defined by the plugin itself
//	 * This method doesn't have to be implemented by the plugin child if it doesn't need
//	 *
//	 * \param paramaterName : the name of the parameter you want the value
//	 * \return the string value for this parameter name or "ERROR" if the name corresponds to any parameter
//	 */
//	virtual std::string getPluginCommParameter(std::string paramaterName){};
	
	
	
	
	
	/************************************************
	 DEVICES METHODS :
	 a set of methods used to manage devices
	 note : each method of this set is prepend by 'device'
	 ************************************************/
	
	/******************NEW METHODS*******************/
	
	/*!
	 * Scan the network automatically to detect devices.
	 * This method full the netDevices map. It is not yet implemented : it needs a daemon like a udp broadcast (osc) or a scan (copperlan)
	 */
	virtual void deviceSetCurrent(std::map<std::string, Device*> *netDevices)=0;
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	virtual void deviceAdd(std::string deviceName, std::map<std::string, std::string> *commParameters, std::map<std::string, Device*> *netDevices)=0;
	
	/*!
	 * Send a message to the specific device
	 *
 	 * \param deviceName : the device name like this "/<deviceName>"
	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>" 
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 */
	virtual void deviceSendMessage(std::string deviceName, std::string stringToSend, std::map<std::string, std::string> *commParameters)=0;
	
	/*!
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	virtual bool understandNamespaceRequest()=0;
	
	/*!
	 * Send a request to a device using the address to get a part of the namespace
	 * This method doesn't have to be implemented by the plugin child if it doesn't need
	 *
	 * \param address : the device address to get the snapshot.
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 */
	virtual void deviceSendNamespaceRequest(std::string deviceName, std::string address, std::map<std::string, std::string> *commParameters){};
	
	/*!
	 * Get the answer request of the send request using the address
	 * This method doesn't have to be implemented by the plugin child if it doesn't need
	 *
	 * \param address : the device address to get the snapshot.
	 * \param nodes : the vector which is going to be full with the node names of the namespace 
	 * \param leaves : the vector which is going to be full with the leaves names of the namespace
	 * \param attributs : the vector which is going to be full with the attributs names of the namespace
	 * \param attributsValue : the vector which is going to be full with the attributs values of the namespace
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	virtual int deviceGetNamespaceAnswer(std::string deviceName, std::string address, std::vector<std::string>* nodes, std::vector<std::string>* leaves, std::vector<std::string>* attributs, std::vector<std::string>* attributsValue){};
	
	/*!
	 * Get a current snapshot (all the leave) of a device using the address  
	 * This method doesn't have to be implemented by the plugin child if it doesn't need
	 *
	 * \param address : the device address to get the snapshot.
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 * \return a vector containing the snapshot
	 */
	virtual std::vector<std::string> deviceSnapshot(std::string deviceName, std::string address, std::map<std::string, std::string> *commParameters){};
	
	
	/**************DEPRECATED METHODS****************/
	
//	/*!
//	 * Scan the network automatically to detect devices.
//	 * This method full the netDevices map. It is not yet implemented : it needs a daemon like a udp broadcast (osc) or a scan (copperlan)
//	 */
//	virtual void setCurrentDevices(std::map<std::string, Device*> *netDevices)=0;
//	
//	/*!
//	 * Add one device in the netDevices map
//	 *
//	 */
//	virtual void addDevice(std::string deviceName, std::map<std::string, std::string> *commParameters, std::map<std::string, Device*> *netDevices)=0;
//	
//	/*!
//	 * Get a current snapshot (all the leave) of a device using the address  
//	 * This method doesn't have to be implemented by the plugin child if it doesn't need
//	 *
//	 * \param address : the device address to get the snapshot.
//	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
//	 * \return a vector containing the snapshot
//	 */
//	virtual std::vector<std::string> snapshot(std::string address, std::map<std::string, std::string> *commParameters){};
//	
//	/*!
//	 * Send a request to a device using the address to get a part of the namespace
//	 * This method doesn't have to be implemented by the plugin child if it doesn't need
//	 *
//	 * \param address : the device address to get the snapshot.
//	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
//	 */
//	virtual void sendNamespaceRequest(std::string address, std::map<std::string, std::string> *commParameters){};
//	
//	/*!
//	 * Get the answer request of the send request using the address
//	 * This method doesn't have to be implemented by the plugin child if it doesn't need
//	 *
//	 * \param address : the device address to get the snapshot.
//	 * \param nodes : the vector which is going to be full with the node names of the namespace 
//	 * \param leaves : the vector which is going to be full with the leaves names of the namespace
//	 * \param attributs : the vector which is going to be full with the attributs names of the namespace
//	 * \param attributsValue : the vector which is going to be full with the attributs values of the namespace
//	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
//	 */
//	virtual int getNamespaceRequestAnswer(std::string address, std::vector<std::string>* nodes, std::vector<std::string>* leaves, std::vector<std::string>* attributs, std::vector<std::string>* attributsValue){};
//
//	/*!
//	 * Send a message to the specific device
//	 *
// 	 * \param deviceName : the device name like this "/<deviceName>"
//	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>" 
//	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
//	 */
//	virtual void sendMessage(std::string deviceName, std::string stringToSend, std::map<std::string, std::string> *commParameters)=0;
//	
};

/*!
 * \Class PluginFactory
 *
 * PluginFactory just contains informations concerning a plugin
 * It's separated from the Plugin class to make the disctinction between the informations of the plugin and the communication methods
 * 
 */

class PluginFactory {
public:	
	virtual std::string getPluginName()=0;
	virtual std::string getPluginVersion()=0;
	virtual std::string getPluginAuthor()=0;
	virtual Plugin *getInstance(Controller *controller)=0;
};

extern "C" {
	PluginFactory* createFactory();
}

#endif //_PLUGIN_H_



