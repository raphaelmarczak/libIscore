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

#include "Plugin.h"
#include "Device.h"
#include "Minuit/MinuitCommunicationMethods.h"

#include <iostream>
#include <sstream>
#include <vector>

static const unsigned int OSC_RECEPTION_PORT = 7003;

using namespace std;

void receiveCallBack(void* arg, std::string message);

class OSC : public Plugin {
private:
	MinuitCommunicationMethods *m_minuitMethods;//this class contains the osc communication methods
	
	friend void receiveCallBack(void* arg, std::string message);
	
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
	void commRunReceivingThread(){
		m_minuitMethods = new MinuitCommunicationMethods();
		
		//m_port = OSC_RECEPTION_PORT;
		m_minuitMethods->minuitRunOSCListening(OSC_RECEPTION_PORT);
		
		m_minuitMethods->m_callBackArgument = this;
		m_minuitMethods->m_callBack = &receiveCallBack;
		
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
	void deviceSetCurrent(map<string, Device*> *netDevices){
		//add 1 osc devices
		//cout << "osc device scan..." << endl;
				
		map<string, string> *deviceParameters = new map<string, string>;
		deviceParameters->insert(pair<string, string>(m_commParameterNames->at(0),"127.0.0.1"));
		deviceParameters->insert(pair<string, string>(m_commParameterNames->at(1),"9998"));
		
		string devName = "/OSCDevice1";
		deviceAdd(devName, deviceParameters, netDevices);
		
//		Device *control = new Device("OSC", deviceParameters);
//		netDevices->insert(pair<string, Device*>(devName, control));
		
		cout << "OSC : found 1 device" << endl;
	}
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	void deviceAdd(string deviceName, map<string, string> *commParameters, map<string, Device*> *netDevices){
		Device *device = new Device("OSC", commParameters);
		netDevices->insert(pair<string, Device*>(deviceName, device));
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
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandNamespaceRequest() {
		return false;
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
 * The receive callback method
 *
 */
void receiveCallBack(void* arg, std::string message) {
	OSC* oscArg = (OSC*) arg;
	
	if (oscArg->m_callBackArgument != NULL) {
		oscArg->m_waitedMessageAction(oscArg->m_callBackArgument, message);
	} else {
		std::cout << "NULL" << std::endl;
	}
}




/*!
 * \class OSC2Factory
 *
 * It's the OSC2 plugin informations
 * 
 */
class OSCFactory : public PluginFactory {
	string getPluginName(){return "OSC";}
	string getPluginVersion(){return "0.1";}
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


