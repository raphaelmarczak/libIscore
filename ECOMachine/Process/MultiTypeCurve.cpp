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

#include "MultiTypeCurve.hpp"

/*!
 * \file MultiTypeCurve.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include <iostream>
#include <cmath>

MultiTypeCurve::MultiTypeCurve()
:m_valueType(CURVE_NOTYPE),m_curveType(CURVE_NOTYPE),m_creationDuration(0),m_currentDuration(0),m_nbSampleBySecond(10),m_storeFactor(5), m_avoidRedondance(true)
{
	m_sectionPercent.clear();

	m_sectionY.clear();

	m_sectionType.clear();
	m_sectionType.push_back(CURVE_POW);

	m_sectionCoeff.clear();
	m_sectionCoeff.push_back(1);
}

MultiTypeCurve::~MultiTypeCurve()
{
}

void MultiTypeCurve::resetPreviouslyAskedInformation()
{
	for (unsigned int i = 0; i < m_wasNotPreviouslyAsked.size(); ++i) {
		m_wasNotPreviouslyAsked[i] = true;
	}
}

void MultiTypeCurve::computeFunction(int beginVal, int endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance)
{
	m_creationDuration = duration;
	m_nbSampleBySecond = sampleBySecond;
	m_storeFactor = storeFactor;

	m_avoidRedondance = avoidRedondance;

	m_valueType = CURVE_INT;

	float durationInSecond = m_creationDuration/1000;

	unsigned int nbSamples = durationInSecond * m_nbSampleBySecond * m_storeFactor;

	m_intCurve.clear();
	m_wasNotPreviouslyAsked.clear();

	if (m_sectionCoeff.size() == 0) {
		for (unsigned int i = 0; i < nbSamples; ++i) {
			m_intCurve.push_back((int)(beginVal));

			m_wasNotPreviouslyAsked.push_back(true);
		}
	} else {
		std::vector<unsigned int> x;
		x.push_back(0);

		std::vector<int> y;
		y.push_back(beginVal);

		for (unsigned int i = 0; i < m_sectionPercent.size(); ++i) {
			x.push_back((int) (m_sectionPercent[i]/100. * nbSamples));
			y.push_back((int) m_sectionY[i]);
		}

		x.push_back(nbSamples);
		y.push_back(endVal);

		unsigned int currentSample = 0;
		unsigned int currentSection = -1;

		float xDiff = 0;
		float yDiff = 0;
		float currentCoeff = 0;
		short currentType = CURVE_POW;

		while (currentSample < nbSamples) {
			if (currentSample == x[currentSection + 1]) {
				++currentSection;

				xDiff = x[currentSection + 1] - x[currentSection];
				yDiff = y[currentSection + 1] - y[currentSection];

				currentCoeff = m_sectionCoeff[currentSection];
				currentType = m_sectionType[currentSection];
			}

			//std::cout << std::pow((float)((float)(currentSample - x[currentSection])/(float)xDiff), (float)currentCoeff) << "<-----" << std::endl;

			m_intCurve.push_back((int) y[currentSection] + std::pow((float)((float)(currentSample - x[currentSection])/(float)xDiff), (float)currentCoeff) * yDiff);
			m_wasNotPreviouslyAsked.push_back(true);

			++currentSample;
		}
	}

	if (m_intCurve.size() > 0) {
		int value = m_intCurve[0];
		m_wasNotPreviouslyAsked[0] = false;

		if (m_avoidRedondance) {
			bool identicalValue = true;

			unsigned int i = 1;

			while ((i < m_intCurve.size()) && (identicalValue)) {
				if (m_intCurve[i] == value) {
					m_wasNotPreviouslyAsked[i] = false;
					++i;
				} else {
					identicalValue = false;
				}
			}
		}
	}
}

void MultiTypeCurve::computeFunction(float beginVal, float endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance)
{
	m_creationDuration = duration;
	m_nbSampleBySecond = sampleBySecond;
	m_storeFactor = storeFactor;

	m_avoidRedondance = avoidRedondance;

	m_valueType = CURVE_FLOAT;

	float durationInSecond = m_creationDuration/1000;

	unsigned int nbSamples = durationInSecond * m_nbSampleBySecond * m_storeFactor;

	m_floatCurve.clear();
	m_wasNotPreviouslyAsked.clear();

	if (m_sectionCoeff.size() == 0) {
		for (unsigned int i = 0; i < nbSamples; ++i) {
			m_floatCurve.push_back(beginVal);

			m_wasNotPreviouslyAsked.push_back(true);
		}
	} else {
		std::vector<unsigned int> x;
		x.push_back(0);

		std::vector<float> y;
		y.push_back(beginVal);

		for (unsigned int i = 0; i < m_sectionPercent.size(); ++i) {
			x.push_back((int) (m_sectionPercent[i]/100. * nbSamples));
			y.push_back((float) m_sectionY[i]);
		}

		x.push_back(nbSamples);
		y.push_back(endVal);
		
		unsigned int currentSample = 0;
		unsigned int currentSection = -1;

		float xDiff = 0;
		float yDiff = 0;
		float currentCoeff = 0;
		short currentType = CURVE_POW;

		while (currentSample < nbSamples) {
			if (currentSample == x[currentSection + 1]) {
				++currentSection;

				xDiff = x[currentSection + 1] - x[currentSection];
				yDiff = y[currentSection + 1] - y[currentSection];

				currentCoeff = m_sectionCoeff[currentSection];
				currentType = m_sectionType[currentSection];
			}

//			std::cout << std::pow((float)((float)(currentSample - x[currentSection])/(float)xDiff), (float)currentCoeff) << "<-----" << std::endl;
//			std::cout << yDiff << std::endl;;
			
			m_floatCurve.push_back((float) y[currentSection] + std::pow((float)((float)(currentSample - x[currentSection])/(float)xDiff), (float)currentCoeff) * yDiff);
			
			m_wasNotPreviouslyAsked.push_back(true);

			++currentSample;
		}
	}

	if (m_floatCurve.size() > 0) {
		float value = m_floatCurve[0];
		m_wasNotPreviouslyAsked[0] = false;

		if (m_avoidRedondance) {
			bool identicalValue = true;

			unsigned int i = 1;

			while ((i < m_floatCurve.size()) && (identicalValue)) {
				if (m_floatCurve[i] == value) {
					m_wasNotPreviouslyAsked[i] = false;
					++i;
				} else {
					identicalValue = false;
				}
			}
		}
	}
}

void MultiTypeCurve::computeFunction(std::string beginVal, std::string endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance)
{
	m_creationDuration = duration;
	m_nbSampleBySecond = sampleBySecond;
	m_storeFactor = storeFactor;

	m_avoidRedondance = avoidRedondance;

	m_valueType = CURVE_STRING;

	float durationInSecond = m_creationDuration/1000;

	unsigned int nbSamples = durationInSecond * m_nbSampleBySecond * m_storeFactor;

	m_stringCurve.clear();
	m_wasNotPreviouslyAsked.clear();

	for (unsigned int i = 0; i < nbSamples; ++i) {
		m_stringCurve.push_back(beginVal);
		m_wasNotPreviouslyAsked.push_back(true);
	}

	if (m_stringCurve.size() > 0) {
		std::string value = m_stringCurve[0];
		m_wasNotPreviouslyAsked[0] = false;

		if (m_avoidRedondance) {
			bool identicalValue = true;

			unsigned int i = 1;

			while ((i < m_stringCurve.size()) && (identicalValue)) {
				if (m_stringCurve[i] == value) {
					m_wasNotPreviouslyAsked[i] = false;
					++i;
				} else {
					identicalValue = false;
				}
			}
		}
	}
}


bool MultiTypeCurve::getValue(int time, int& value)
{
	float timeInSec = (float)time/1000.0;

	unsigned int index = ((unsigned int)(timeInSec * m_nbSampleBySecond * (m_creationDuration/m_currentDuration))) * m_storeFactor;

	if (index >= m_intCurve.size()) {
		index = m_intCurve.size() - 1;
	}

	bool wasNotPreviouslySend = m_wasNotPreviouslyAsked[index];

	value = m_intCurve[index];

	m_wasNotPreviouslyAsked[index] = false;

	if (m_avoidRedondance) {
		bool identicalValue = true;

		unsigned int i = index + 1;

		while ((i < m_intCurve.size()) && (identicalValue)) {
			if (m_intCurve[i] == value) {
				m_wasNotPreviouslyAsked[i] = false;
				++i;
			} else {
				identicalValue = false;
			}
		}
	}

	return wasNotPreviouslySend;
}

bool MultiTypeCurve::getValue(int time, float& value)
{
	float timeInSec = (float)time/1000.0;

	unsigned int index = ((unsigned int)(timeInSec * m_nbSampleBySecond * (m_creationDuration/m_currentDuration))) * m_storeFactor;

	if (index >= m_floatCurve.size()) {
		index = m_floatCurve.size() - 1;
	}

	bool wasPreviouslySend = m_wasNotPreviouslyAsked[index];

	value = m_floatCurve[index];

	m_wasNotPreviouslyAsked[index] = false;

	if (m_avoidRedondance) {
		bool identicalValue = true;

		unsigned int i = index + 1;

		while ((i < m_floatCurve.size()) && (identicalValue)) {
			if (m_floatCurve[i] == value) {
				m_wasNotPreviouslyAsked[i] = false;
				++i;
			} else {
				identicalValue = false;
			}
		}
	}

	return wasPreviouslySend;
}

bool MultiTypeCurve::getValue(int time, std::string& value)
{
	float timeInSec = (float)time/1000.0;

	unsigned int index = ((unsigned int)(timeInSec * m_nbSampleBySecond * (m_creationDuration/m_currentDuration))) * m_storeFactor;

	if (index >= m_stringCurve.size()) {
		index = m_stringCurve.size() - 1;
	}

	bool wasPreviouslySend = m_wasNotPreviouslyAsked[index];

	value = m_stringCurve[index];

	m_wasNotPreviouslyAsked[index] = false;

	if (m_avoidRedondance) {
		bool identicalValue = true;

		unsigned int i = index + 1;

		while ((i < m_stringCurve.size()) && (identicalValue)) {
			if (m_stringCurve[i] == value) {
				m_wasNotPreviouslyAsked[i] = false;
				++i;
			} else {
				identicalValue = false;
			}
		}
	}

	return wasPreviouslySend;
}

std::vector<int> MultiTypeCurve::getIntCurve()
{
	return m_intCurve;
}

std::vector<float> MultiTypeCurve::getFloatCurve()
{
	return m_floatCurve;
}

std::vector<std::string> MultiTypeCurve::getStringCurve()
{
	return m_stringCurve;
}

bool MultiTypeCurve::setSections(std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff)
{
	if (percent.size() != y.size()) {
		return false;
	}

	if (sectionType.size() != (percent.size() + 1)) {
		return false;
	}

	if (coeff.size() != sectionType.size()) {
		return false;
	}

	m_sectionPercent = percent;
	m_sectionY = y;
	m_sectionType = sectionType;
	m_sectionCoeff = coeff;

	return true;
}

void MultiTypeCurve::getSections(std::vector<float>* percent, std::vector<float>* y, std::vector<short>* sectionType, std::vector<float>* coeff)
{
	*percent = m_sectionPercent;
	*y = m_sectionY;
	*sectionType = m_sectionType;
	*coeff = m_sectionCoeff;
}

short MultiTypeCurve::getType()
{
	return m_valueType;
}

void MultiTypeCurve::changeDuration(int duration)
{
	m_currentDuration = duration;
}
