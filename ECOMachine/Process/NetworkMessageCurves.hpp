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

#ifndef NETWORKMESSAGECURVES_HPP_
#define NETWORKMESSAGECURVES_HPP_

/*!
 * \file NetworkMessageCurve.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "MultiTypeCurve.hpp"
#include "StringParser.hpp"

#include "../../Editor/CSP/XMLInclude.hpp"

#include <vector>
#include <string>

/*!
 * \class NetworkMessageCurves
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class NetworkMessageCurves
{
public:
	NetworkMessageCurves();
	NetworkMessageCurves(std::string address);

	virtual ~NetworkMessageCurves();

	bool getMessageToSend(int time, std::string& message);

	void setAvoidRedondance(bool avoidRedondance);
	bool getAvoidRedondance();

	void setSampleBySec(int sampleBySec);
	unsigned int getSampleBySec();

	void setStoreFactor(int storeFactor);
	int getStoreFactor();

	bool addCurvesPoints(unsigned int argNb, std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff);
	bool getCurveSections(unsigned int argNb, std::vector<float>& xPercents,  std::vector<float>& yValues,  std::vector<short>& sectionType,  std::vector<float>& coeff);
	bool getCurves(StringParser firstMessage, StringParser lastMessage, unsigned int argNb, unsigned int duration, std::vector<float>& result);

	void computeAllCurves(StringParser first, StringParser second, int duration);

	void setMuteState(bool muteState);
	bool getMuteState();

	void store(xmlNodePtr father);

private:
	bool m_avoidRedondance;
	bool m_muteState;
	unsigned int m_sampleBySec;
	int m_storeFactor;

	std::string m_address;
	std::vector<MultiTypeCurve> m_argCurves;
};

#endif /*NETWORKMESSAGECURVES_HPP_*/
