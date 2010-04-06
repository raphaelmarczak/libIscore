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

#ifndef MULTITYPECURVE_H_
#define MULTITYPECURVE_H_

/*!
 * \file MultiTypeCurve.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#define CURVE_NOTYPE 0

#define CURVE_INT 1
#define CURVE_FLOAT 2
#define CURVE_STRING 3

#define CURVE_LINEAR 2
#define CURVE_POW 1

#include <vector>
#include <string>

/*!
 * \class MultiTypeCurve
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class MultiTypeCurve
{
public:
	MultiTypeCurve();
	virtual ~MultiTypeCurve();

	void resetPreviouslyAskedInformation();

	void computeFunction(int beginVal, int endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance);
	void computeFunction(float beginVal, float endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance);
	void computeFunction(std::string beginVal, std::string endVal, int duration, int sampleBySecond, int storeFactor, bool avoidRedondance);

	bool getValue(int time, int& value);
	bool getValue(int time, float& value);
	bool getValue(int time, std::string& value);

	std::vector<int> getIntCurve();
	std::vector<float> getFloatCurve();
	std::vector<std::string> getStringCurve();

	bool setSections(std::vector<float> percent, std::vector<float> y, std::vector<short> sectionType, std::vector<float> coeff);
	void getSections(std::vector<float>* percent, std::vector<float>* y, std::vector<short>* sectionType, std::vector<float>* coeff);

	short getType();

	void changeDuration(int duration);

private:
	short m_valueType;
	short m_curveType;

	std::vector<float> m_sectionPercent;
	std::vector<float> m_sectionY;
	std::vector<short> m_sectionType;
	std::vector<float> m_sectionCoeff;

	std::vector<int> m_intCurve;
	std::vector<float> m_floatCurve;
	std::vector<std::string> m_stringCurve;

	std::vector<bool> m_wasNotPreviouslyAsked;

	int m_creationDuration;
	int m_currentDuration;

	int m_nbSampleBySecond;

	int m_storeFactor;

	bool m_avoidRedondance;
};

#endif /*MULTITYPECURVE_H_*/
