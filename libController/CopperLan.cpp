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
 *  CopperLan.cpp
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include "Plugin.h"
#include "Device.h"
#include <iostream>
#include <vector>

using namespace std;

#include <vector>

using namespace std;

void receiveCallBack(void* arg, std::string message);

class CopperLan : public Plugin {
private:	
	friend void receiveCallBack(void* arg, std::string message);
	
public:
	/*!
	 * Run the message reception thread 
	 * Prepare the receive callback method to be passed to the Controller to intercept the message
	 *
	 */
	void runReceivingThread(){
		
	}
	
	/*!
	 * Let to know if the specific plugin need namespace request to get a namespace
	 *
	 * \return true if the plugin need or false if not
	 */
	bool understandNamespaceRequest() {
		return false;
	}
	
//	/*!
//	 * Define the plugin parameters to communicate if it needs
//	 *
//	 */
//	void defineParametersToUseWithThisPlugin(){}
	
	/*!
	 * Scan the network to find the device which want to communicate.
	 *
	 * \param netDevices : the Device map which is going to be full with the network CopperLan devices. 
	 */	
	void setCurrentDevices(map<string, Device*> *netDevices){
		
		//add 1 CPL devices
		cout << "CPL device scan..." << endl;
		
		string devName = "/CopperlanDevice1";
		map<string, string> *deviceParameters = new map<string, string>;
		addDevice(devName, deviceParameters, netDevices);
		
		cout << "CPL device scan ok" << endl;
	}
	
	/*!
	 * Add one device in the netDevices map
	 *
	 */
	void addDevice(string deviceName, map<string, string> *commParameters, map<string, Device*> *netDevices){
		Device *device = new Device("CopperLan", commParameters);
		netDevices->insert(pair<string, Device*>(deviceName, device));
	}
	
	/*!
	 * Send a message to the specific device
	 *
 	 * \param deviceName : the device name like this "/<deviceName>"
	 * \param stringToSend : the message to send like this : "/<deviceName>/<subDeviceName>/.../<input> <value>" 
	 * \param commParameters : a map containing couples <parameterName, parameterValue> to communicate according to the plugin 
	 */
	void sendMessage(string deviceName, string stringToSend, map<string, string> *commParameters) {
		//	cout << "Jenvoie en Copperlan au Device " << deviceName  << " à l'adresse " << stringToSend  << endl;

	}
};

/*!
 *
 * The receive callback method
 *
 */
void receiveCallBack(void* arg, std::string message) {

}

class CopperLanFactory : public PluginFactory {
	string getPluginName(){return "CopperLan";}
	string getPluginVersion(){return "0.1beta";}
	string getPluginAuthor(){return "";}
	Plugin *getInstance(Controller *controller){
		CopperLan *copperLan = new CopperLan();
		copperLan->setController(controller);
		return copperLan;
	}
};

extern "C" {
	PluginFactory* createFactory() {
		return new CopperLanFactory();
	}
}

