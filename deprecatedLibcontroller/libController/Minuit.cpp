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

#include "Plugin.h"
#include "Device.h"
#include "Minuit/MinuitCommunicationMethods.h"
#include "Controller.h"

#include <iostream>
#include <sstream>
#include <vector>

static const unsigned int MINUIT_RECEPTION_PORT = 7002;

using namespace std;

void receiveCallBack(void* arg, std::string message);
void receiveNamespaceCallback(void* arg, std::string message);
void receiveGetCallback(void* arg, std::string message);

class Minuit : public Plugin {
private:
	MinuitCommunicationMethods *m_minuitMethods;//this class contains the osc communication methods
	
	friend void receiveCallBack(void* arg, std::string message);
	friend void receiveNamespaceCallback(void* arg, std::string message);
	friend void receiveGetCallback(void* arg, std::string message);
	
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
	void commRunReceivingThread(){
		m_minuitMethods = new MinuitCommunicationMethods();
		m_minuitMethods->minuitRunOSCListening(MINUIT_RECEPTION_PORT);
		
		m_minuitMethods->m_callBackArgument = this;
		m_minuitMethods->m_callBack = &receiveCallBack;
		
		m_minuitMethods->m_namespaceRequestCallBackArgument = this;
		m_minuitMethods->m_namespaceRequestCallBack = &receiveNamespaceCallback;
		
		m_minuitMethods->m_getRequestCallBackArgument = this;
		m_minuitMethods->m_getRequestCallBack = &receiveGetCallback;
		
		//cout << "run thread" << endl;
	}
	
	/*!
	 * Define the plugin parameters to communicate if it needs
	 *
	 */
	void commDefineParameters(){
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
	string commGetParameter(string paramaterName){
		if(paramaterName.compare("pluginReceptionPort")==0){
			ostringstream oss;
			oss << MINUIT_RECEPTION_PORT;
			return oss.str();;
		} else {
			cout << "No Parameter named : " << paramaterName << "for minuit plugin" << endl;
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
	void deviceSetCurrent(map<string, Device*> *netDevices){
		//add 3 minuit devices to communicate with the Stantum device
		map<string, string> *deviceParameters = new map<string, string>;
		deviceParameters->insert(pair<string, string>("ip", "127.0.0.1"));
		deviceParameters->insert(pair<string, string>("port", "10000"));
		netDevices->insert(pair<string, Device*>("/StantumUpdating", new Device("Minuit", deviceParameters, false)));
		
		deviceParameters->find("port")->second = "10001";
		netDevices->insert(pair<string, Device*>("/StantumReseting", new Device("Minuit", deviceParameters, false)));
		
		
		deviceParameters->find("port")->second = "10002";
		netDevices->insert(pair<string, Device*>("/StantumDisplay", new Device("Minuit", deviceParameters, false)));
		
		
		//add 1 default minuit device to communicate with Jamoma environment
		deviceParameters->find("port")->second = "9998";
		deviceAdd("/MinuitDevice1", deviceParameters, netDevices);
		
		cout << "Minuit : found 1 device" << endl;
	}
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	void deviceAdd(string deviceName, map<string, string> *commParameters, map<string, Device*> *netDevices){
		Device *device = new Device("Minuit", commParameters);
		netDevices->insert(pair<string, Device*>(deviceName, device));
	}
	
	/*!
	 * Get a current snapshot (all the leave) of a device using the address  
	 *
	 * \param address : the device address to get the snapshot.
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 * \return a vector containing the snapshot
	 */
	vector<string> deviceSnapshot(string deviceName, string address, std::map<std::string, std::string> *commParameters){
		string ip = commParameters->find("ip")->second;
		string port = commParameters->find("port")->second;
		vector<string> snapshot = m_minuitMethods->minuitSnapShot(address, ip, toInt(port));
		
		return snapshot;
	}
	
	/*!
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandNamespaceRequest() {
		return true;
	}
	
	/*!
	 * Send a request to a device using the address to get a part of the namespace
	 *
	 * \param address : the device address to get the snapshot.
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 */
	void deviceSendNamespaceRequest(string deviceName, string address, std::map<std::string, std::string> *commParameters){
		string ip = commParameters->find("ip")->second;
		string port = commParameters->find("port")->second;
		m_minuitMethods->minuitSendRequest(MINUIT_REQUEST_NAMESPACE, address, ip, toInt(port));
	}
	
	/*!
	 * Get the answer request of the send request using the address
	 *
	 * \param address : the device address to get the snapshot.
	 * \param nodes : the vector which is going to be full with the node names of the namespace 
	 * \param leaves : the vector which is going to be full with the leaves names of the namespace
	 * \param attributs : the vector which is going to be full with the attributs names of the namespace
	 * \param attributsValue : the vector which is going to be full with the attributs values of the namespace
	 * \return the reception state : TIMEOUT_EXCEEDED ; NO_ANSWER ; ANSWER_RECEIVED
	 */
	int deviceGetNamespaceAnswer(string deviceName, string address, vector<string>* nodes, vector<string>* leaves, vector<string>* attributs, vector<string>* attributsValue){
		return m_minuitMethods->minuitGetNamespaceRequestAnswer(address, nodes, leaves, attributs, attributsValue);
	}
	
	unsigned int computeOSCMessageSize(OSCParser OSCParsed) 
	{
		unsigned int result = 0;
		
		result += 8; //#bundle
		result += 8; //timetag
		result += 4; //datasize
		
		unsigned int addressSize = OSCParsed.getAddress().size();
		addressSize += 1; // /0 for end of string
		
		result += ((addressSize/4) + 1) * 4; //Address Size
		
		unsigned int argumentsSize = OSCParsed.getNbArg();
		argumentsSize += 1; // , for indicating this is an argument string information
		
		result += ((argumentsSize/4) + 1) * 4; //ArgumentTag Size
		
		for (unsigned int i = 0; i < OSCParsed.getNbArg(); ++i) {
			string currentString = OSCParsed.getArg(i);
			if (OSCParsed.getType(i) == osc::INT32_TYPE_TAG) {
				result += 4; //Int size
			} else if (OSCParsed.getType(i) == osc::FLOAT_TYPE_TAG) {
				result += 4; //Float size
			} else {
				unsigned int stringSize = currentString.size();
				stringSize += 1; // /0 for end of string
				result += ((stringSize/4) + 1) * 4; //String Size
			}
		}
		
		//	std::cout << "OSC size calcul result : " << result << std::endl;
		return result;
	}
	
	/*!
	 * Send a message to the specific device
	 *
 	 * \param deviceName : the device name like this "/<deviceName>"
	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>" 
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 */
	void deviceSendMessage(string deviceName, string stringToSend, map<string, string> *commParameters) {
		//	cout << "Jenvoie en OSC2 au Device " << deviceName << " d'ID" << deviceId << " à l'adresse " << stringToSend  << endl;
		
		string ip = commParameters->find("ip")->second;
		string port = commParameters->find("port")->second;
		
		OSCParser OSCParsed(stringToSend);
		
		unsigned int bufferSize = computeOSCMessageSize(OSCParsed);
		
		UdpTransmitSocket transmitSocket( IpEndpointName(ip.data(), toInt(port)) );
		
		char buffer[bufferSize];
		osc::OutboundPacketStream oscStream(buffer, bufferSize);
		
		oscStream << osc::BeginBundleImmediate;
		oscStream << osc::BeginMessage(OSCParsed.getAddress().data());
		
		for (unsigned int i = 0; i < OSCParsed.getNbArg(); ++i) {
			string currentString = OSCParsed.getArg(i);
			if (OSCParsed.getType(i) == osc::INT32_TYPE_TAG) {
				//	cout << "INT" << endl;
				oscStream << toInt(currentString);
			} else if (OSCParsed.getType(i) == osc::FLOAT_TYPE_TAG) {
				//	cout << "FLOAT" << endl;
				oscStream << toFloat(currentString);
			} else {
				//	cout << "OTHER" << endl;
				oscStream << currentString.data();
			}
		}
		
		oscStream << osc::EndMessage << osc::EndBundle;
		
		//	std::cout << "real osc size : " << oscStream.Size() << std::endl;
		
		transmitSocket.Send( oscStream.Data(), oscStream.Size());
		oscStream.Clear();
	}
	
};

/*!
 *
 * The receive callback methods
 *
 */
void receiveCallBack(void* arg, std::string message) {
	Minuit* oscArg = (Minuit*) arg;
	
	if (oscArg->m_callBackArgument != NULL) {
		oscArg->m_waitedMessageAction(oscArg->m_callBackArgument, message);
	} else {
		std::cout << "NULL" << std::endl;
	}
}

void receiveNamespaceCallback(void* arg, std::string address){
	Minuit* oscArg = (Minuit*) arg;	
	//cout << "namespace callback" << endl;
	
	std::vector<std::string> *triggerNamespace = new std::vector<std::string>;
	oscArg->m_controller->askControllerNamespaceFor(address, triggerNamespace);
	
	//for(int i = 0; i < triggerNamespace->size(); i++)
	//std::cout << triggerNamespace->at(i) << endl; 
	
	oscArg->m_minuitMethods->minuitSendNamespaceAnswer(address, triggerNamespace);
	
	//send a message to Stantum device to stop the uptodate button flashing
	if(address.find("/TriggerNamespace"))
		oscArg->m_controller->sendMessage("/StantumReseting/reset");
	
}

void receiveGetCallback(void* arg, std::string address){
	Minuit* oscArg = (Minuit*) arg;
	//cout << "get callback" << endl;
	
	int value = oscArg->m_controller->askControllerValueFor(address);
	
	oscArg->m_minuitMethods->minuitSendGetAnswer(address, value);
}




/*!
 * \class MinuitFactory
 *
 * It's the Minuit plugin informations
 * 
 */
class MinuitFactory : public PluginFactory {
	string getPluginName(){return "Minuit";}
	string getPluginVersion(){return "0.1";}
	string getPluginAuthor(){return "Raphael Marczak/Laurent Garnier";}
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


