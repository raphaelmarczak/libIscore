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
 *  main.cpp
 *  Controller
 *
 *  Created by Laurent Garnier on 03/06/09.
 *  Copyright 2009 __BlueYeti/LaBRI__. All rights reserved.
 *
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Controller.h"
#include "Device.h"
#include "Minuit/MinuitInclude.hpp"

using namespace std;

//void usage(std::string commande) 
//{
//	std::cout << "Usage : " << commande << " " << "<String to Send>" << endl; 
//	exit(EXIT_FAILURE); 
//}
//
//void mainCallBack(void* unused, std::string message) 
//{
//	std::cout << "MAIN CALLBACK" << std::endl;
//}

int 
main(int argc, char **argv) 
{
	//if(argc != 2)
	//	usage(argv[0]);
	
	Controller *controller = new Controller("Main");	
	controller->pluginLoad("Plugins");
	cout << endl;
	
	vector<string> plugins = controller->pluginGetLoadedByName();
	cout << "Current loaded plugins : " << endl;
	for (int i = 0; i < plugins.size(); i++) {
		cout << plugins.at(i) << endl;
	}
	cout << endl;
	
	cout << "Device scan : " << endl;
	controller->deviceSetCurrent();
	cout << endl;
	
	map<string, Device*> *devices = controller->deviceGetCurrent();
	
	cout << "Current devices : " << endl;
	map<string, Device*>::iterator it = devices->begin();
	while(it != devices->end()){
		cout << it->first << endl;
		it++;
	}
	cout << endl;
	
	/*test sendMessage*/
//	controller->deviceSendMessage("/CopperLAN/CopperMax/DeviceA/SubDeviceA/SubSubDeviceA/in1 0.3"); 
//	controller->deviceSendMessage("/CopperLAN/CopperMax/DeviceA/in2 0.2");
	/***************/
	
	/*test getNamespace*/
//	it = devices->begin();
//	while (it != devices->end()) {
		vector<std::string>* nodes = new vector<std::string>;
		vector<std::string>* leaves = new vector<std::string>;
		vector<std::string>* attributes = new vector<std::string>;
		vector<std::string>* values = new vector<std::string>;
		vector<string>::iterator iter;
		bool isLeave;
		
		controller->deviceSendDiscoverRequest("CopperLANdevice1", "/CopperMax", nodes, leaves, attributes);
		
		std::cout << "trying to connect" << endl << endl;
		
		int state = 1;
		do {
			usleep(1000);
			//state = controller->deviceWaitNamespaceAnswer(/*it->first +*/ "/CopperLAN/CopperMax", nodes, leaves, attributs, values);
			
			if (state == -1) {
				cout << "TIMEOUT !!!" << endl;
			}
			
		} while(state == 0);
		
		if(state == 1){
			for( iter = nodes->begin(); iter != nodes->end(); iter++ ) {
				cout << *iter << endl;
			}
			for( iter = attributes->begin(); iter != attributes->end(); iter++ ) {
				cout << *iter << endl;
			}
			for( iter = values->begin(); iter != values->end(); iter++ ) {
				cout << *iter << endl;
			}
		}
		
//		it++;
//	}
	/***************/
	
	//	/*test snapshot*/
	//	std::vector<std::string> arbre = controller->deviceSnapshot("/control");
	//	for( iter = arbre.begin(); iter != arbre.end(); iter++ ) {
	//		cout << *iter << endl;
	//	}
	/***************/
	
	//	/*test receiveMessage*/
	//	controller->addWaitedMessageAction(NULL, &mainCallBack);
	//	while (true) {
	//		usleep(1000);
	//	}
	//	/***************/
	
	delete nodes;
	delete attributes;
	delete values;
	delete controller;
	return EXIT_SUCCESS;
}

