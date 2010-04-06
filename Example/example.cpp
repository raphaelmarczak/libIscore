/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak
Last modification: 04/04/2010

Adviser(s): Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)

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

#include <IScore/Engines.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

void controlPointCallBack(unsigned int boxId, unsigned int controlPointIndex)
{
  std::string controlPointPosition;

  if (controlPointIndex == BEGIN_CONTROL_POINT_INDEX) {
    controlPointPosition = "BOX BEGIN";
  } else {
    controlPointPosition = "BOX END";
  }

  std::cout << "* A Control Point was just crossed *" << std::endl;
  std::cout << "<BoxID, ControlPoint position>: <" 
	    << boxId << ", " << controlPointPosition
	    << ">" << std::endl;
}

void waitedTriggerPointCallBack(bool isWaited, unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex, std::string triggerMessage)
{
  std::string controlPointPosition;

  if (controlPointIndex == BEGIN_CONTROL_POINT_INDEX) {
    controlPointPosition = "BOX BEGIN";
  } else {
    controlPointPosition = "BOX END";
  }

  std::cout << "* A Trigger Point is ready to be activated *" << std::endl;
  std::cout << "Please send the Trigger Point message ("<< triggerMessage << ") to this computer using the port 7002" << std::endl;
  std::cout << "(or quit by pressing ctrl-C)" << std::endl;
  std::cout << "<BoxID, ControlPoint Position, Trigger message>: <" 
	    << boxId << ", " << controlPointPosition << ", " << triggerMessage 
	    << ">" << std::endl;
}

void isExecutionFinishedCallBack()
{
  std::cout << "* The execution is now finished *" << std::endl;
}

int main()
{
  // Maximum scenario size (in ms).
  unsigned int scenarioSize = 60000;

  // Engines initialization for Editing and Executing
  Engines* mainEngines = new Engines(scenarioSize);

  // Add a device to communicate with, using the OSC plugin.
  // Communication will be on port 9998 and localhost.
  mainEngines->addNetworkDevice("/myDevice", "OSC", "127.0.0.1", "9998");

  // Add a box which will start after 1 seconde, and last 2 secondes.
  // The third parameter is here to precise the box mother id for hierarchy.
  // If the box belong to the upper level in hierarchy, this parameter must be ROOT_BOX_ID.
  //
  // Remark: when a box is created, it automaticaly got two control point, one for the
  // beginning (BEGIN_CONTROL_POINT_INDEX), and the other for the ending (END_CONTROL_POINT_INDEX).
  int firstBoxBeginInMs = 1000;
  int firstBoxDurationInMs = 2000;
  int box1ID = mainEngines->addBox(firstBoxBeginInMs, firstBoxDurationInMs, ROOT_BOX_ID);

  // Add a second box which will start after 4,5 seconde, and last 1 secondes.
  int secondBoxBeginInMs = 4500;
  int secondBoxDurationInMs = 1000;
  int box2ID = mainEngines->addBox(secondBoxBeginInMs, secondBoxDurationInMs, ROOT_BOX_ID);

  // Force the begin of the second box to be after the end of the first one.
  // Two relations: ANTPOST_ANTERIORITY and ANTPOST_POSTERIORITY.
  mainEngines->addTemporalRelation(box1ID, END_CONTROL_POINT_INDEX, box2ID, BEGIN_CONTROL_POINT_INDEX, ANTPOST_ANTERIORITY);

  // Add three callbacks to be informed during execution:
  // - When a controlPoint (start or end of a box) is crossed.
  // - When a triggerPoint is waited.
  // - When the scenario is over.
  mainEngines->addCrossingCtrlPointCallback(&controlPointCallBack);
  mainEngines->addCrossingTrgPointCallback(&waitedTriggerPointCallBack);
  mainEngines->addExecutionFinishedCallback(&isExecutionFinishedCallBack);


  // **** FIRST EXAMPLE ****
  // Only discrete messages

  // Send "/box1/start" and "/box1/parameter 0." to the device "/myDevice"
  // when the first box will start.
  std::vector<std::string> box1MessagesStart;
  box1MessagesStart.push_back("/myDevice/box1/start");
  box1MessagesStart.push_back("/myDevice/box1/parameter 0.");
  mainEngines->setCtrlPointMessagesToSend(box1ID, BEGIN_CONTROL_POINT_INDEX, box1MessagesStart);

  // Send "/box1/end" and "/box1/parameter 1." to the same device when the first box will end.
  std::vector<std::string> box1MessagesEnd;
  box1MessagesEnd.push_back("/myDevice/box1/end");
  box1MessagesEnd.push_back("/myDevice/box1/parameter 1.");
  mainEngines->setCtrlPointMessagesToSend(box1ID, END_CONTROL_POINT_INDEX, box1MessagesEnd);

  // Send "/box2/start" and "/box2/parameter 0" to the device "/myDevice"
  // when the second box will start.
  std::vector<std::string> box2MessagesStart;
  box2MessagesStart.push_back("/myDevice/box2/start");
  box2MessagesStart.push_back("/myDevice/box2/parameter 0");
  mainEngines->setCtrlPointMessagesToSend(box2ID, BEGIN_CONTROL_POINT_INDEX, box2MessagesStart);

  // Send "/box2/end" and "/box2/parameter 100" to the same device when the second box will end.
  std::vector<std::string> box2MessagesEnd;
  box2MessagesEnd.push_back("/myDevice/box2/end");
  box2MessagesEnd.push_back("/myDevice/box2/parameter 100");
  mainEngines->setCtrlPointMessagesToSend(box2ID, END_CONTROL_POINT_INDEX, box2MessagesEnd);

  

  std::cout << std::endl;
  std::cout << "********************************" << std::endl;
  std::cout << "FIRST EXAMPLE (DISCRETE) PLAYING" << std::endl;
  std::cout << "You could launch the PureData patch provided to see the received messages" << std::endl;
  std::cout << "Press A Key to continue" << std::endl;
  std::cout << std::endl;

  std::getc(stdin);
 
  mainEngines->play();

  while (mainEngines->isRunning()) {
    usleep(50);
  } 

  std::cout << "END OF FIRST EXAMPLE" << std::endl << std::endl;

  
  // **** SECOND EXAMPLE ****
  // Add interpolation

  // Add curve information (address to create curve).
  // By default, a linear interpolation is create, respecting
  // the parameter type (float, int, ...).
  mainEngines->addCurve(box1ID,"/myDevice/box1/parameter");
  mainEngines->addCurve(box2ID,"/myDevice/box2/parameter");

  std::cout << std::endl;
  std::cout << "********************************" << std::endl;
  std::cout << "SECOND EXAMPLE (INTERPOLATION) PLAYING" << std::endl;
  std::cout << "You could launch the PureData patch provided to see the received messages" << std::endl;
  std::cout << "Press A Key to continue" << std::endl;
  std::cout << std::endl;

 std::getc(stdin);

  mainEngines->play();

  while (mainEngines->isRunning()) {
    usleep(50);
  } 


  // **** THIRD EXAMPLE ****
  // Add interraction to the second box start.

  // Creation of a triggerPoint.
  // First, it is not linked to any controlPoint.
  unsigned int triggerID = mainEngines->addTriggerPoint();

  // Link the triggerPoint to a controlPoint.
  mainEngines->assignCtrlPointToTriggerPoint(triggerID, box2ID, BEGIN_CONTROL_POINT_INDEX);

  // Set the message to trigger this triggerPoint.
  mainEngines->setTriggerPointMessage(triggerID, "/box2/start");

  std::cout << std::endl;
  std::cout << "********************************" << std::endl;
  std::cout << "THIRD EXAMPLE (INTERRACTION) PLAYING" << std::endl;
  std::cout << "You could launch the PureData patch provided to see the received messages, and to trigger the triggerPoint" << std::endl;
  std::cout << "Press A Key to continue" << std::endl;
  std::cout << std::endl;

 std::getc(stdin);

  mainEngines->play();

  while (mainEngines->isRunning()) {
    usleep(50);
  } 
}
