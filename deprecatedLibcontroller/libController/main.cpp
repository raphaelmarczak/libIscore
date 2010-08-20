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
	
	Controller *controller = new Controller();	
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
		vector<std::string>* attributs = new vector<std::string>;
		vector<std::string>* values = new vector<std::string>;
		vector<string>::iterator iter;
		
		controller->deviceSendNamespaceRequest(/*it->first +*/ "/CopperLAN/CopperMax");
		
		std::cout << "trying to connect" << endl << endl;
		
		int state = 1;
		do {
			usleep(1000);
			state = controller->deviceGetNamespaceAnswer(/*it->first +*/ "/CopperLAN/CopperMax", nodes, leaves, attributs, values);
			
			if (state == -1) {
				cout << "TIMEOUT !!!" << endl;
			}
			
		} while(state == 0);
		
		if(state == 1){
			for( iter = nodes->begin(); iter != nodes->end(); iter++ ) {
				cout << *iter << endl;
			}
			for( iter = leaves->begin(); iter != leaves->end(); iter++ ) {
				cout << *iter << endl;
			}
			for( iter = attributs->begin(); iter != attributs->end(); iter++ ) {
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
	delete leaves;
	delete attributs;
	delete values;
	delete controller;
	return EXIT_SUCCESS;
}

