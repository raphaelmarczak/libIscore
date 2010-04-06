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
 *  Device.cpp
 *  
 *
 *  Created by Laurent Garnier on 27/05/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include "Device.h"

Device::Device(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters, bool isVisible)
{
	m_name = deviceName;
	m_pluginToUse = pluginToUse;
	
	// copy communication parameters
	m_commParameters = new std::map<std::string, std::string>;
	std::map<std::string, std::string>::iterator it = commParameters->begin();
	while (it != commParameters->end()) {
		m_commParameters->insert(std::pair<std::string, std::string>(it->first, it->second));
		it++;
	}
	
	// set visible or not
	m_isVisible = isVisible;
}

Device::~Device()
{
	m_commParameters->erase(m_commParameters->begin(), m_commParameters->end());
	delete m_commParameters;
}

std::string
Device::getName()
{
	return m_name;
}

std::string 
Device::getPluginToUse()
{
	return m_pluginToUse;
}

std::string 
Device::getCommParameter(std::string parameterName)
{
	std::map<std::string, std::string>::iterator it = m_commParameters->find(parameterName);
	
	if(it != m_commParameters->end())
	{
		return it->second;	
	}
	else
	{
		std::cout << "No device Parameter named : " << parameterName << std::endl;
		return "ERROR";
	}
}

std::map<std::string, std::string>*
Device::getAllCommParameter()
{
	if (m_commParameters->size() == 0) {
		return NULL;
	}
	return m_commParameters;
}

bool
Device::isVisible() 
{
	return m_isVisible;
}

void
Device::visible(bool v) 
{
	m_isVisible = v;
}
