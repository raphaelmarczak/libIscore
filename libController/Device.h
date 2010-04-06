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
 *
 */

#ifndef _Device_H_
#define _Device_H_

#include "Controller.h"

class Device{
	
private:
	
	std::string							m_name;				//< The name of the device
	std::string							m_pluginToUse;		//< the plugin name must be the same string that was given by the plugin developper
	std::map<std::string, std::string>*	m_commParameters;	//< a map with the communication parameter needs by the plugin and their values
	bool								m_isVisible;		//< a device could be used by the user or only for an internal communication.
	
public:

	/*!
	 * Constructor
	 */
	Device(std::string deviceName, std::string pluginToUse, std::map<std::string, std::string> *commParameters, bool isVisible = true);
	
	/*!
	 * Destructor
	 */
	~Device();
	
	/*!
	 * Get the name of the device
	 */
	std::string getName();
	
	/*!
	 * Get the communication plugin used by the device
	 */
	std::string getPluginToUse();
	
	/*!
	 * Get a communicationa parameter value (depending on plugin)
	 */
	std::string getCommParameter(std::string parameterName);
	
	/*!
	 * Get all communication parameters value(depending on plugin)
	 */
	std::map<std::string, std::string>* getAllCommParameter();

	/*!
	 * is the device visible ?
	 */
	bool isVisible();
	
	/*!
	 * Set the device visible or not
	 */
	void visible(bool v);
	
};

#endif //_Device_H_
