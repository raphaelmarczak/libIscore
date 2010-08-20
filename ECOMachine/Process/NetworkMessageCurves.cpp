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

/*!
 * \file NetworkMessageCurve.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "NetworkMessageCurves.hpp"
#include "StringTypeFunctions.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>

#define DEFAUT_STORE_FACTOR 2
#define DEFAUT_SAMPLE_BY_SEC 10

NetworkMessageCurves::NetworkMessageCurves()
{
	m_avoidRedondance = true;
	m_sampleBySec = DEFAUT_SAMPLE_BY_SEC;
	m_storeFactor = DEFAUT_STORE_FACTOR;
}

NetworkMessageCurves::NetworkMessageCurves(std::string address)
{
	m_avoidRedondance = true;
	m_sampleBySec = DEFAUT_SAMPLE_BY_SEC;
	m_storeFactor = DEFAUT_STORE_FACTOR;

	m_address = address;
}

void NetworkMessageCurves::setAvoidRedondance(bool avoidRedondance)
{
	m_avoidRedondance = avoidRedondance;
}

bool NetworkMessageCurves::getAvoidRedondance()
{
	return m_avoidRedondance;
}

void NetworkMessageCurves::setSampleBySec(int sampleBySec)
{
	m_sampleBySec = sampleBySec;
}

unsigned int NetworkMessageCurves::getSampleBySec()
{
	return m_sampleBySec;
}

void NetworkMessageCurves::setStoreFactor(int storeFactor)
{
	m_storeFactor = storeFactor;
}

int NetworkMessageCurves::getStoreFactor()
{
	return m_storeFactor;
}

bool NetworkMessageCurves::addCurvesPoints(unsigned int argNb, std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff)
{
	if (m_argCurves.size() <= argNb) {
		m_argCurves.resize(argNb + 1);
	}

	for (unsigned int i = 0; i < percent.size(); ++i) {
		percent[i] = percent[i] * 100;
	}

	return m_argCurves[argNb].setSections(percent, y, sectionType, coeff);
}

bool NetworkMessageCurves::getCurveSections(unsigned int argNb, std::vector<float>& xPercents,  std::vector<float>& yValues,  std::vector<short>& sectionType,  std::vector<float>& coeff)
{
	xPercents.clear();
	yValues.clear();
	sectionType.clear();
	coeff.clear();

	if (m_argCurves.size() <= argNb) {
		return false;
	}

	m_argCurves[argNb].getSections(xPercents, yValues, sectionType, coeff);

	for (unsigned int i = 0; i < xPercents.size(); ++i) {
		xPercents[i] = xPercents[i] / 100;
	}

	return true;
}

bool NetworkMessageCurves::getCurves(StringParser firstMessage, StringParser lastMessage, unsigned int argNb, unsigned int duration, std::vector<float>& result)
{
	result.clear();

	if ((firstMessage.getNbArg() <= argNb) || (lastMessage.getNbArg() <= argNb)) {
		return false;
	}

	if (m_argCurves.size() <= argNb) {
		m_argCurves.resize(argNb + 1);
	}

	StringParser::argType type;

	if (firstMessage.getType(argNb) == lastMessage.getType(argNb)) {
		type = firstMessage.getType(argNb);
	} else if (firstMessage.getType(argNb) == StringParser::TYPE_STRING || lastMessage.getType(argNb) == StringParser::TYPE_STRING) {
		type = StringParser::TYPE_STRING;
	} else if (firstMessage.getType(argNb) == StringParser::TYPE_SYMBOL || lastMessage.getType(argNb) == StringParser::TYPE_SYMBOL) {
		type = StringParser::TYPE_SYMBOL;
	} else if (firstMessage.getType(argNb) == StringParser::TYPE_FLOAT || lastMessage.getType(argNb) == StringParser::TYPE_FLOAT) {
		type = StringParser::TYPE_FLOAT;
	} else {
		std::string errorString("OSCCurves : for linear function, all matching arguments must be in the same type. Problem with : ");
		errorString += firstMessage.getAddress();

		throw std::invalid_argument(errorString);
	}

	std::string startValue = firstMessage.getArg(argNb);
	std::string endValue = lastMessage.getArg(argNb);

	if (type == StringParser::TYPE_INT) {
		m_argCurves[argNb].computeFunction(toInt(startValue), toInt(endValue), duration, m_sampleBySec, 1, m_avoidRedondance);
	} else if (type == StringParser::TYPE_FLOAT) {
		m_argCurves[argNb].computeFunction(toFloat(startValue), toFloat(endValue), duration, m_sampleBySec, 1, m_avoidRedondance);
	} else {
		m_argCurves[argNb].computeFunction(startValue, endValue, duration, m_sampleBySec, 1, m_avoidRedondance);
	}

	if (m_argCurves[argNb].getType() == CURVE_STRING) {
		return false;
	}

	if (m_argCurves[argNb].getType() == CURVE_FLOAT) {
		std::vector<float> curveFloat = m_argCurves[argNb].getFloatCurve();

		for (unsigned int i = 0; i < curveFloat.size() ; ++i) {
			result.push_back(curveFloat[i]);
		}

		result.push_back(toFloat(endValue));

		return true;
	}

	if (m_argCurves[argNb].getType() == CURVE_INT) {
		std::vector<int> curveInt = m_argCurves[argNb].getIntCurve();

		for (unsigned int i = 0; i < curveInt.size() ; ++i) {
			result.push_back(curveInt[i]);
		}

		result.push_back(toInt(endValue));

		return true;
	}

	return false;
}

void NetworkMessageCurves::computeAllCurves(StringParser first, StringParser second, int duration)
{

	if (first.getAddress() == second.getAddress()) {
		m_address = first.getAddress();
	} else {
		throw std::invalid_argument("OSCCurves : for linear function, args must be with the same address");
	}

	if (first.getNbArg() == second.getNbArg()) {
		unsigned int nbArg = first.getNbArg();

		if (m_argCurves.size() < nbArg) {
			m_argCurves.resize(nbArg);
		}

		for (unsigned int i = 0; i < nbArg; ++i) {
			StringParser::argType type;
			if (first.getType(i) == second.getType(i)) {
				type = first.getType(i);
			} else if (first.getType(i) == StringParser::TYPE_STRING || second.getType(i) == StringParser::TYPE_STRING) {
				type = StringParser::TYPE_STRING;
			} else if (first.getType(i) == StringParser::TYPE_SYMBOL || second.getType(i) == StringParser::TYPE_SYMBOL) {
				type = StringParser::TYPE_SYMBOL;
			} else if (first.getType(i) == StringParser::TYPE_FLOAT || second.getType(i) == StringParser::TYPE_FLOAT) {
				type = StringParser::TYPE_FLOAT;
			} else {
				std::string errorString("OSCCurves : for linear function, all matching arguments must be in the same type. Problem with : ");
				errorString += m_address;

				throw std::invalid_argument(errorString);
			}

			std::string startValue = first.getArg(i);
			std::string endValue = second.getArg(i);

			//MultiTypeCurve currentCurve;

			if (type == StringParser::TYPE_INT) {
				m_argCurves[i].computeFunction(toInt(startValue), toInt(endValue), duration, m_sampleBySec, m_storeFactor, m_avoidRedondance);
			} else if (type == StringParser::TYPE_FLOAT) {
				m_argCurves[i].computeFunction(toFloat(startValue), toFloat(endValue), duration, m_sampleBySec, m_storeFactor, m_avoidRedondance);
			} else {
				m_argCurves[i].computeFunction(startValue, endValue, duration, m_sampleBySec, m_storeFactor, m_avoidRedondance);
			}

			m_argCurves[i].changeDuration(duration);

		//	m_argCurves[i] = currentCurve;
		}
	} else {
		throw std::invalid_argument("OSCCurves : for linear funcion, args must have the same number of arguments");
	}

}

NetworkMessageCurves::~NetworkMessageCurves()
{
}

bool NetworkMessageCurves::getMessageToSend(int time, std::string& message)
{
	bool hasNotAlreadyBeenSend = false;

	std::stringstream ss("");
	ss << m_address;

	for (unsigned int i = 0; i < m_argCurves.size(); ++i) {
		short currentType = m_argCurves[i].getType();

		if (currentType == CURVE_INT) {
			int currentInt;

			if(m_argCurves[i].getValue(time, currentInt)) {
				hasNotAlreadyBeenSend = true;
			}

			ss << " ";
			ss << currentInt;
		} else if (currentType == CURVE_FLOAT) {
			float currentFloat;

			if(m_argCurves[i].getValue(time, currentFloat)) {
				hasNotAlreadyBeenSend = true;
			}

			ss << " ";
			ss << currentFloat;
		} else {
			std::string currentString;

			if(m_argCurves[i].getValue(time, currentString)) {
				hasNotAlreadyBeenSend = true;
			}

			ss << " ";
			ss << currentString.data();
		}

	}

	message = ss.str();

	return hasNotAlreadyBeenSend;
}

void NetworkMessageCurves::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;
	xmlNodePtr argNode = NULL;

	node = xmlNewNode(NULL, BAD_CAST "CURVE_INFORMATION");

	std::ostringstream oss;

	xmlSetProp(node, BAD_CAST "address", BAD_CAST m_address.data());

	oss.str("");
	oss << (m_sampleBySec);

	xmlSetProp(node, BAD_CAST "samplerate", BAD_CAST oss.str().data());

	if (m_avoidRedondance) {
		xmlSetProp(node, BAD_CAST "avoidRedundancy", BAD_CAST "true");
	} else {
		xmlSetProp(node, BAD_CAST "avoidRedundancy", BAD_CAST "false");
	}

	xmlAddChild(father, node);

	for (unsigned int i = 0; i < m_argCurves.size() ; ++i) {
		argNode = xmlNewNode(NULL, BAD_CAST "ARGUMENT");

		oss.str("");
		oss << i;

		xmlSetProp(argNode, BAD_CAST "nb", BAD_CAST oss.str().data());

		std::vector<float> percent;
		std::vector<float> y;
		std::vector<short> sectionType;
		std::vector<float> coeff;

		m_argCurves[i].getSections(percent, y, sectionType, coeff);

		for (unsigned int j = 0; j < percent.size() ; ++j) {
			xmlNodePtr coordinatesNode = xmlNewNode(NULL, BAD_CAST "COORDINATE");

			oss.str("");
			oss << percent.at(j);

			xmlSetProp(coordinatesNode, BAD_CAST "x_percent", BAD_CAST oss.str().data());

			oss.str("");
			oss << y.at(j);

			xmlSetProp(coordinatesNode, BAD_CAST "y_value", BAD_CAST oss.str().data());

			xmlAddChild(argNode, coordinatesNode);
		}

		for (unsigned int j = 0; j < sectionType.size() ; ++j) {
			xmlNodePtr sectionNode = xmlNewNode(NULL, BAD_CAST "SECTION");

			if (sectionType.at(i) == CURVE_POW) {
				xmlSetProp(sectionNode, BAD_CAST "section_type", BAD_CAST "pow");
			}

			oss.str("");
			oss << coeff.at(j);

			xmlSetProp(sectionNode, BAD_CAST "coeff", BAD_CAST oss.str().data());

			xmlAddChild(argNode, sectionNode);
		}


		xmlAddChild(node, argNode);
	}
}
