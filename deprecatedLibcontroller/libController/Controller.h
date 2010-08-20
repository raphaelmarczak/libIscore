/*
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

/*!
 * \class Controller
 * \author Laurent Garnier (BlueYeti/LaBRI)
 * \date 03/06/09
 *
 * The Controller is a network plugin manager.
 * It is totally independant. It can be used in other communication applications.
 * It is compiled in a library called libController.a that can be added in a project.
 *
 * In the sequencer it establishes the communication between the engine and devices detected on the network.
 * It create the good link and use the good plugin according to the device communication protocol.
 * It select the good plugin to use with the device name
 *
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>

class Device;
class Plugin;
class PluginFactories;

static const unsigned int TRIGGER_READY = 0;
static const unsigned int TRIGGER_WAITED = 1;
static const unsigned int TRIGGER_PUSHED = 2;

class Controller{
private:
	PluginFactories *factories;						  //the plugin factories
	std::map<std::string, Plugin*> *netPlugins;		  //a map between a plugin name and an instance of this Plugin
	std::map<std::string, Device*> *netDevices;		  //a map between a device name and an instance of this Device
	int deviceId;								      //the device id witch is incremented automatically

	std::map<unsigned int, std::string> *m_namespace; //TriggerId , TriggerAddress
	std::map<unsigned int, unsigned int> *m_values;   //TriggerId , TriggerValue

	void (*m_waitedMessageAction)(void*, std::string);//a callback used when a message is received
	void* m_waitedMessageActionArgument;			  //argument for the callback method

public:

	/************************************************
	 CONTROLLER METHODS :
	 a set of methods used to handle Controller.
	 ************************************************/

	/*!
	 * Default constructor.
	 */
	Controller();

	/*!
	 * Destructor.
	 */
	~Controller();

	/*!
	 * Add a message reception callback method
	 *
	 */
	void addWaitedMessageAction(void* arg, void(*pt2Func)(void*, std::string));

	/*!
	 * The callback method
	 *
	 */
	friend void waitedMessageCallback(void *, std::string);




	/************************************************
	 PLUGINS METHODS :
	 a set of methods used to manage plugins.
	 note : each method of this set is prepend by 'plugin'
	 ************************************************/

	/*!
	 * Load the plugins available in the plugin directory and instanciates them.
	 *
	 * \param path : the Plugin directory path.
	 */
	void pluginLoad(std::string path);

	/*!
	 *
	 *\return the list of loaded plugin names
	 */
	std::vector<std::string> pluginGetLoadedByName();

	/*!
	 * Get a plugin parameter value
	 *
	 *\return the value for parameter "parameterName" according to the pluginName
	 */
	std::string pluginGetCommParameter(std::string pluginName, std::string parameterName);

	/*!
	 * Get an instance of a plugin
	 *
	 */
	Plugin *pluginGetInstance(std::string pluginName);

	/*!
	 * DEPRECATED
	 * Init the plugins available in the plugin directory and instanciates them.
	 *
	 * \param path : the Plugin directory path.
	 */
	void initPlugins(std::string path);

	/*!
 	 * DEPRECATED
	 *
	 *\return the list of loaded plugin names
	 */
	std::vector<std::string> getLoadedPluginsName();

	/*!
 	 * DEPRECATED
	 * Get a plugin parameter value
	 *
	 *\return the value for parameter "parameterName" according to the pluginName
	 */
	std::string getPluginCommParameter(std::string pluginName, std::string parameterName);

	/*!
 	 * DEPRECATED
	 * Get a plugin port value
	 *
	 *\return the listening port
	 */
	unsigned int getPluginListeningPort(std::string pluginName);





	/************************************************
	 DEVICES METHODS :
	 a set of methods used to manage devices
	 note : each method of this set is prepend by 'device'
	 ************************************************/

	/*!
	 * Scan the network automatically to detect devices using the appropriate plugin.
	 * This method full the netDevices map.
	 */
	void deviceSetCurrent();

	/*!
	 * Possible access to the netDevices map
	 *
	 * \return the netDevices map.
	 */
	std::map<std::string, Device*> *deviceGetCurrent();

	/*!
	 * Add a device manually
	 *
	 * \param deviceName : the device name to add like this : /<deviceName> .
	 * \param pluginToUse : the plugin name to use which has to be the same string of the plugin name given by his developper. ("OSC", "Minuit", ...)
	 * \param deviceIp : the ip address of the device if the plugin needs it else you can pass NO_IP
	 * \param devicePort : the listening port of the device if the plugin needs it else you can pass NO_PORT
	 */
	int deviceAdd(std::string deviceName, std::string pluginToUse, std::string deviceIp, std::string devicePort);

	/*!
	 * Add one device manually according to the genericity of the different plugins parameters
	 *
	 * \param deviceName : the device name to add like this : /<deviceName> .
	 * \param pluginToUse : the plugin name to use which has to be the same string of the plugin name given by his developper. ("OSC", "Minuit", ...)
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin
	 *
	 * \return 0 if the device is added, -1 if deviceName already exist or if pluginToUse doesn't exist
	 */
	int deviceAdd(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters);

	/*!
	 * Remove a device in the netDevices map using the device name
	 */
	void deviceRemove(std::string deviceName);

	/*!
	 * Let to know if the specific plugin used by the device "address" is able to answer to a namespace request
	 *
	 * \return true if the plugin need or false if not
	 */
	bool deviceUnderstandNamespaceRequest(std::string address);

	/*!
	 * Send a message to the specific device
	 *
	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>"
	 */
	void deviceSendMessage(std::string stringToSend);

	/*!
	 * Send a request to a device using the address to get a part of the namespace
	 *
	 * \param address : the device address to get the snapshot.
	 */
	bool deviceSendNamespaceRequest(std::string address);

	/*!
	 * Get the answer of the send request using the address
	 *
	 * \param address : the device address to get the snapshot.
	 * \param nodes : the vector which is going to be full with the node names of the namespace
	 * \param leaves : the vector which is going to be full with the leaves names of the namespace
	 * \param attributs : the vector which is going to be full with the attributs names of the namespace
	 * \param attributsValue : the vector which is going to be full with the attributs values of the namespace
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int deviceGetNamespaceAnswer(std::string address, std::vector<std::string>* nodes, std::vector<std::string>* leaves, std::vector<std::string>* attributs, std::vector<std::string>* attributsValue);

	/*!
	 * Get a snapshot (total namespace) of a device using the address
	 *
	 * \param address : the device address to get the snapshot.
	 * \return a vector containing the snapshot
	 */
	std::vector<std::string> deviceSnapshot(std::string address);

	/*!
	 * Let to know if a device has to be visible in the UI.
	 * In other words if it could de used by the user or only has an internal use.
	 *
	 * \param deviceName : the device name.
	 * \return true if the device is visible in the UI.
	 */
	bool deviceIsVisible(std::string deviceName);

	/*!
	 * DEPRECATED
	 * Scan the network automatically to detect devices using the appropriate plugin.
	 * This method full the netDevices map.
	 */
	void setCurrentDevices();

	/*!
	 * DEPRECATED
	 * Add a device manually
	 *
	 * \param deviceName : the device name to add like this : /<deviceName> .
	 * \param pluginToUse : the plugin name to use which has to be the same string of the plugin name given by his developper. ("OSC", "Minuit", ...)
	 * \param deviceIp : the ip address of the device if the plugin needs it else you can pass NO_IP
	 * \param devicePort : the listening port of the device if the plugin needs it else you can pass NO_PORT
	 */
	int addDevice(std::string deviceName, std::string pluginToUse, std::string deviceIp, std::string devicePort);

	/*!
	 * DEPRECATED
	 * Add one device manually according to the genericity of the different plugins parameters
	 *
	 * \param deviceName : the device name to add like this : /<deviceName> .
	 * \param pluginToUse : the plugin name to use which has to be the same string of the plugin name given by his developper. ("OSC", "Minuit", ...)
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin
	 *
	 * \return 0 if the device is added, -1 if deviceName already exist or if pluginToUse doesn't exist
	 */
	int addDevice(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters);

	/*!
	 * DEPRECATED
	 * Remove a device in the netDevices map using the device name
	 */
	void removeDevice(std::string deviceName);

	/*!
	 * DEPRECATED
	 * Possible access to the netDevices map
	 *
	 * \return the netDevices map.
	 */
	std::map<std::string, Device*> *getCurrentDevices();

	/*!
	 * DEPRECATED
	 * Let to know if the specific plugin used by the device "address" need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandNamespaceRequest(std::string address);

	/*!
	 * DEPRECATED
	 * Get a snapshot (total namespace) of a device using the address
	 *
	 * \param address : the device address to get the snapshot.
	 * \return a vector containing the snapshot
	 */
	std::vector<std::string> snapshot(std::string address);

	/*!
	 * DEPRECATED
	 * Send a request to a device using the address to get a part of the namespace
	 *
	 * \param address : the device address to get the snapshot.
	 */
	bool sendNamespaceRequest(std::string address);

	/*!
	 * DEPRECATED
	 * Get the answer request of the send request using the address
	 *
	 * \param address : the device address to get the snapshot.
	 * \param nodes : the vector which is going to be full with the node names of the namespace
	 * \param leaves : the vector which is going to be full with the leaves names of the namespace
	 * \param attributs : the vector which is going to be full with the attributs names of the namespace
	 * \param attributsValue : the vector which is going to be full with the attributs values of the namespace
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int getNamespaceRequestAnswer(std::string address, std::vector<std::string>* nodes, std::vector<std::string>* leaves, std::vector<std::string>* attributs, std::vector<std::string>* attributsValue);

	/*!
	 * DEPRECATED
	 * Send a message to the specific device
	 *
	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>"
	 */
	void sendMessage(std::string stringToSend);



	/************************************************
	 NAMESPACE METHODS :
	 a set of methods used to manage the local namespace
	 note : each method of this set is prepend by 'namespace'
	 ************************************************/

	void addTriggerPointLeave(unsigned int triggerId, std::string triggerMessage);
	void removeTriggerPointLeave(unsigned int triggerId);
	void setNamespaceValue(std::string address, int value, std::map<std::string, std::string> optionalArguments);
	void resetTriggerPointStates();
	std::map<unsigned int, std::string> *getControllerNamespace();
	void getWaitedTriggerPoints(std::vector<std::string> *waitedTP);

	void askControllerNamespaceFor(std::string address, std::vector<std::string>* namespaceVectorToFill);
	int askControllerValueFor(std::string address);
};

#endif //_CONTROLLER_H_
