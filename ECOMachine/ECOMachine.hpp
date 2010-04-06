/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak
Last modification: 08/03/2010

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

#ifndef ECOMACHINE_HPP_
#define ECOMACHINE_HPP_

/*!
 * \file ECOMachine.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 *
 * \brief This file contains the ECOMachine (all functions for managing the execution).
 */

#include "PetriNet/PetriNet.hpp"
#include "Process/ProcessInclude.hpp"
#include "ControlPointInformations.hpp"
#include "TriggerPointInformations.hpp"

#include "../StoryLine/StoryLine.hpp"

#include <map>
#include <set> 
#include <string>

#include "../Editor/CSP/XMLInclude.hpp"

#define EVENT_NAME "event"

/*!
 * \class ECOMachine
 *
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 *
 * \brief Class which contains all functions for execution.
 *
 * This class contains all functions to communicate with the ECOMachine.
 */
class ECOMachine
{
public:
	/*!
	 * ECOMachine constructor.
	 */
	ECOMachine();

	/*!
	 * ECOMachine destructor.
	 */
	virtual ~ECOMachine();

	/*!
	 * Adds a network message in order to make it understandable by
	 * the ECO Machine.
	 *
	 * \param netMessage : message to be understandable by the ECO Machine
	 * \return the matching event in the PetriNet
	 */
	std::string addNetworkMessage(std::string netMessage);

	/*!
	 * Informs the ECO Machine that a network message arrived.
	 * The ECO Machine use this message if it was previously added. If not, it ignores
	 * the message.
	 *
	 * \param netMessage : message
	 * \return true if the message is known by the ECO Machine. False if this message was not added previously.
	 */
	bool receiveNetworkMessage(std::string netMessage);

	/*!
	 * Adds a new ECOProcess in the ECOMachine.
	 *
	 * \param type : process type.
	 * \param processId : the processId to give to this process.
	 * \param controller : the network controller used by this process to communicate using network.
	 */
	void newProcess(std::string type, unsigned int processId, Controller* controller);

	/*!
	 * Gets a ECOProcess.
	 *
	 * \param processId : the process Id.
	 *
	 * \return the ECOProcess matching the given Id.
	 */
	ECOProcess* getProcess(unsigned int processId);

	/*!
	 * Removes a ECOProcess.
	 *
	 * \param processId : the process Id to remove.
	 */
	void removeProcess(unsigned int processId);

	/*!
	 * Gets the ECOMachine current execution time in milliseconds.
	 *
	 * \return the execution time in milliseconds.
	 */
	unsigned int getTimeInMs();

	/*!
	 * Ignores the waited trigger points for one step in the ECOMachine.
	 * The ECOMachine must be in running state.
	 */
	void ignoreTriggerPointForOneStep();

	/*!
	 * Resets the PetriNet.
	 */
	void reset();

	/*!
	 * Compiles the ECOMachine.
	 *
	 * \param storyLineToCompile : the storyLine containing all informations for compilation.
	 * \timeToStartTriggerPoint : time in ms after which the trigger point are handled.
	 */
	void compileECO(StoryLine* storyLineToCompile, unsigned int timeToStartTriggerPoint = 0);

	/*!
	 * Runs the ECOMachine.
	 *
	 * \return true if the ECOMachine is now running.
	 */
	bool run();
	
	void pause(bool pauseValue);

	/*!
	 * Tells the ECOMachine that it must stop.
	 *
	 * \return true if the ECOMachine is now in a "must stop" state and was previously running.
	 */
	bool stop();
	
	void setGotoInformation(unsigned int gotoInformation);
	unsigned int getGotoInformation();
	
	bool setSpeedFactor(float factor);
	bool changeSpeedFactor(float factor);
	
	float getSpeedFactor();

	/*!
	 * Tests if the ECOMachine is actually running.
	 *
	 * \return true if the ECOMachine is running. False if not.
	 */
	bool isRunning();

	/*!
	 * Tests if the ECOMachine is in a "must stop" state (it will stops when it will be possible).
	 *
	 * \return true if the ECOMachine is in a "must stop" state.
	 */
	bool mustStop();
	
	bool isPaused();

	/*!
	 * Adds the callback called during the execution when a control point is crossed.
	 * This callback gives information about the box id, and the control point index.
	 *
	 * \param pt2Func : the callback.
	 */
	void addCrossAControlPointAction(void(*pt2Func)(unsigned int, unsigned int));

	/*!
	 * Removes the callback called during the execution when a control point is crossed.
	 */
	void removeCrossAControlPointAction();

	/*!
	 * Adds the callback called during the execution when a control point is crossed.
	 * This callback gives information about the box id, and the control point index.
	 *
	 * \param pt2Func : the callback.
	 */
	void addWaitedTriggerPointMessageAction(void* arg, void(*pt2Func)(void*, bool, unsigned int, unsigned int, unsigned int, std::string));

	/*!
	 * Removes the callback called during the execution when a control point is crossed.
	 */
	void removeWaitedTriggerPointMessageAction();

	/*!
	 * Adds the callback called when the execution is finished
	 * (with a stop action, or because the scenario ended normally).
	 *
	 * \param pt2Func : the callback.
	 */
	void addIsECOFinishedAction(void* arg, void(*pt2Func)(void *));

	/*!
	 * Removes the callback called when the execution is finished
	 */
	void removeIsECOFinishedAction();
	
	/*!
	 * Gets the progression of a process (in percent).
	 *
	 * \param processId: the id of the process.
	 * \return the progression inpercent.
	 */
	float getProcessProgressionPercent(unsigned int processId);

	/*!
	 * Stores the ECOMachine.
	 *
	 * \param father : xmlNode.
	 */
	void store(xmlNodePtr father);

	/*!
	 * Loads the ECOMachine.
	 *
	 * \param root : xmlNode.
	 * \param controller : the controller used for sending network message.
	 */
	void load(xmlNodePtr root, Controller* controller);

	/*!
	 * Gets the PetriNet of this ECO Machine.
	 * It is only usefull for tests.
	 *
	 * This function SHOULD NOT BE CALLED outside tests classes.
	 *
	 * \return the PetriNet of this ECO Machine.
	 */
	PetriNet* getPetriNet();


	friend void* ECOMachineMainFunction(void* threadArg);

	friend void crossAControlPointCallBack(void* arg);
	friend void waitedTriggerPointMessageCallBack(void* arg, bool isWaited, Transition* transition);

private:
	PetriNet* m_petriNet;

	Place* m_startPlace;
	Place* m_endPlace;

	Transition* m_startTransition;
	Transition* m_endTransition;

	unsigned int m_lastEventNumber;

	std::map<std::string, std::string> m_fromNetworkMessagesToPetriMessages;
	std::map<unsigned int, ECOProcess*> m_process;
	std::map<Transition*, TriggerPoint*> m_transitionToTriggerPoint;

	std::map<Transition*, TriggerPointInformations> m_transitionToTriggerPointInformations;

	pthread_t m_thread;
	
	bool m_isRunning;
	bool m_isPaused;
	bool m_mustStop;
	
	float m_currentSpeedFactor;
	unsigned int m_gotoValue;
	
	std::set<Transition*> getOneDepthPredecessorsTransitions(Transition* transition);
	std::set<Transition*>  computeTransitionsSet(Transition* endTransition, std::map<Transition*, std::set<Transition*> >* transitionsSets);

	void (*m_crossAControlPointAction)(unsigned int, unsigned int);

	void (*m_waitedTriggerPointMessageAction)(void*, bool, unsigned int, unsigned int, unsigned int, std::string);
	void* m_waitedTriggerPointMessageArg;

	void (*m_isEcoMachineFinished)(void*);
	void* m_isEcoMachineFinishedArg;

	void cleanECO(Transition* endTransition = NULL);

	bool hasTriggerPointInformations(Transition* transition);
	TriggerPointInformations getTriggerPointInformations(Transition* transition);
};

#endif /*ECOMACHINE_HPP_*/
