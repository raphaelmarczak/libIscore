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

#ifndef SENDNETWORKMESSAGEPROCESS_HPP_
#define SENDNETWORKMESSAGEPROCESS_HPP_

/*!
 * \file SendNetworkMessageProcess.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include <pthread.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Process.hpp"
#include "NetworkMessageCurves.hpp"

#include <DeviceManager/DeviceManager.h>


#define DEFAUT_NB_SAMPLES_BY_SEC 20

#define MY_MIN_STEP_TIME_IN_MS 100

/*!
 * \class SendNetworkMessageProcess
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class SendNetworkMessageProcess : public ECOProcess
{
public:
	SendNetworkMessageProcess(unsigned int id, DeviceManager* controller);

	virtual ~SendNetworkMessageProcess();

	void pushNoMessage();

	void init();

	void addMessages(std::vector<std::string> messages, unsigned int stepId, bool muteState = false);

	void getMessages(std::vector<std::string>& messages, unsigned int stepId);

	void removeMessage(unsigned int stepId);

	bool addCurvesPoints(std::string address, unsigned int argNb, std::vector<float> percent,
						 std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff);

	bool getCurveSections(std::string address, unsigned int argNb, std::vector<float>& percent,
													std::vector<float>& y, std::vector<short>& sectionType, std::vector<float>& coeff);

	void setMessageMuteState(unsigned int stepId, bool muteState);

	bool getMessageMuteState(unsigned int stepId);

	void setAllMessagesMuteState(bool muteState);

	void addAddressToInterpolate(std::vector<std::string> addressToInterpolate);

	void setAvoidRedondanceInformation(std::string address, bool avoidRedondance);
	bool getAvoidRedondanceInformation(std::string address);

	void setCurveMuteStateInformation(std::string address, bool muteState);
	bool getCurveMuteStateInformation(std::string address);

	std::vector<std::string> getCurvesAdress();

	void removeAllCurves();

	void setNbSamplesBySec(std::string address, unsigned int nbSample);
	unsigned int getNbSamplesBySec(std::string address);

	void setCurvesMuteState(bool muteState);
	bool getCurvesMuteState();

	bool setCurvesSections(std::string address, unsigned int argNb,
						   std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff);

	void computeCurves(unsigned int firstStep, unsigned int lastStep, unsigned int duration);

	long long computeDt();
	void sendCurves(unsigned int time);

	void addCurves(std::string address);
	void removeCurves(std::string address);

	bool getCurves(std::string address, unsigned int argNb, unsigned int duration, unsigned int firstControlPointIndex, unsigned int lastControlPointIndex, std::vector<float>& result);

	virtual float getProgressionPercent();

protected :
	virtual void mainFunction();
	virtual void store(xmlNodePtr father);
	virtual void load(xmlNodePtr root);

private:
	DeviceManager* m_networkController;

	std::map< unsigned int, std::map<std::string, std::string> > m_messageToSendInMap;
	std::map< unsigned int, std::vector<std::string> > m_messageToSendInVector;
	std::map< unsigned int, std::vector<bool> > m_messageMuteState;
	std::map< unsigned int, bool > m_stepMuteState;

	std::vector<NetworkMessageCurves> m_curvesToSend;

	std::map<std::string, NetworkMessageCurves> m_curves;
	bool m_curvesMuteState;

	unsigned int m_nbSampleBySec;

	long long m_previousCallOfMakeOneStepInMs;
	unsigned int m_currentTime;

	void sendMessage(std::string);
};



#endif /*SENDNETWORKMESSAGEPROCESS_HPP_*/
