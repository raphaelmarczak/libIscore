/*
 *  Device.h
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

/*!
 * \class Device
 * \author Laurent Garnier (BlueYeti/LaBRI)
 * \date 03/06/09
 *
 * A Device represents an application with which the sequencer is going to communicate. 
 * It is characterised by an id, the plugin name to use with it, an ip address and a listening port if it needs. 
 * When the Controller scans the network it instanciates one Device for each module which give a answer.
 */

#ifndef _Device_H_
#define _Device_H_

#include <string> 
#include <map>

class Device{
private:
	unsigned int m_DeviceId;
	std::string m_pluginToUse; //the plugin name must be the same string that was given by the plugin developper
	std::string m_DeviceIp;
	std::string m_DevicePort;
	std::map<std::string, std::string> *m_commParameters;
	
	bool m_isVisible;
	
public:

	/*!
	 * DEPRECATED
	 * Constructor
	 */
	Device(unsigned int DeviceId, std::string pluginToUse, std::string DeviceIp, std::string DevicePort, bool isVisible = true);
	
	/*!
	 * More generic constructor
	 */
	Device(std::string pluginToUse, std::map<std::string, std::string> *commParameters, bool isVisible = true);
	
	/*!
	 * Destructor
	 */
	~Device();
	
	/*!
	 * DEPRECATED
	 * Access methods
	 */
	unsigned int getDeviceId();
	std::string getPluginToUse();
	std::string getDeviceIp();
	std::string getDevicePort();
	
	std::string getParameter(std::string parameterName);
	std::map<std::string, std::string> *getAllParameters();
	
	bool deviceIsVisible();
};

#endif //_Device_H_
