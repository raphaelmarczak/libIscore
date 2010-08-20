/*
 *  Device.cpp
 *  
 *
 *  Created by Laurent Garnier on 27/05/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include "Device.h"
#include <string> 
#include <iostream>
#include <map>

using namespace std;


Device::Device(unsigned int DeviceId, std::string pluginToUse, std::string DeviceIp, std::string DevicePort, bool isVisible)
{
	std::cout << "Constructor Device::Device(unsigned int DeviceId, std::string pluginToUse, std::string DeviceIp, std::string DevicePort, bool isVisible) is DEPRECATED " << std::endl;
	std::cout << "Use Device::Device(std::string pluginToUse, std::map<std::string, std::string> *commParameters, bool isVisible)" << std::endl; 

	m_DeviceId = DeviceId;
	m_pluginToUse = pluginToUse;
	m_DeviceIp = DeviceIp;
	m_DevicePort = DevicePort;
	
	m_commParameters = new map<string, string>;

	m_commParameters->insert(std::pair<std::string, std::string>("ip", DeviceIp));
	m_commParameters->insert(std::pair<std::string, std::string>("port", DevicePort));
	
	m_isVisible = isVisible;
}

Device::Device(std::string pluginToUse, std::map<std::string, std::string> *commParameters, bool isVisible)
{
	m_pluginToUse = pluginToUse;
	m_commParameters = new map<string, string>;
	map<string, string>::iterator it = commParameters->begin();
	while (it != commParameters->end()) {
		m_commParameters->insert(std::pair<std::string, std::string>(it->first, it->second));
		it++;
	}
	
	m_isVisible = isVisible;
}


Device::~Device()
{
	m_commParameters->erase(m_commParameters->begin(), m_commParameters->end());
	delete m_commParameters;
}

unsigned int 
Device::getDeviceId()
{
	return m_DeviceId;
}

std::string 
Device::getPluginToUse()
{
	return m_pluginToUse;
}

std::string
Device::getDeviceIp()
{
	std::cout << "Method std::string Device::getDeviceIp() is DEPRECATED." << std::endl;
	std::cout << "Use std::string Device::getParameter(std::string parameterName)" << std::endl;
	return m_DeviceIp;
}

std::string 
Device::getDevicePort()
{
	std::cout << "Method std::string Device::getDevicePort() is DEPRECATED." << std::endl;
	std::cout << "Use std::string Device::getParameter(std::string parameterName)" << std::endl;
	return m_DevicePort;
}

std::string 
Device::getParameter(std::string parameterName)
{
	std::map<std::string, std::string>::iterator it = m_commParameters->find(parameterName);
	
	if (it != m_commParameters->end()) {
		return it->second;	
	} else {
		std::cout << "No device Parameter named : " << parameterName << std::endl;
		return "ERROR";
	}
}

std::map<std::string, std::string>*
Device::getAllParameters()
{
	if (m_commParameters->size() == 0) {
		return NULL;
	}
	return m_commParameters;
}

bool
Device::deviceIsVisible() 
{
	return m_isVisible;
}
