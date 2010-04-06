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
 * \file StringParser.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "StringParser.hpp"
#include "StringTypeFunctions.hpp"

#include <sstream>
#include <iterator>

#include <stdexcept>

StringParser::StringParser()
{

}

StringParser::StringParser(std::string OSCMessageToParse)
{
	using namespace std;

	vector<string> strList;

	istringstream iss( OSCMessageToParse );
	copy(
			istream_iterator<string>( iss ),
			istream_iterator<string>(),
			back_inserter( strList ) );

	if (strList.size() > 0) {
		m_address = strList.at(0);

		for (unsigned int i = 1; i < strList.size(); ++i) {
			string currentString = strList.at(i);
			if (isInt(currentString)) {
				m_arg.push_back(currentString);
				m_type.push_back(StringParser::TYPE_INT);
			} else if (isFloat(currentString)) {
				m_arg.push_back(currentString);
				m_type.push_back(StringParser::TYPE_FLOAT);
			} else {
				if (currentString.data()[0] == '"') {
					string symbolToSend = currentString.substr(1, currentString.size());

					while (currentString.data()[currentString.size()-1] != '"' && (i != (strList.size() - 1))) {
						i++;
						currentString = strList.at(i);

						symbolToSend += " ";
						symbolToSend += currentString;
					}

					if (i == (strList.size() - 1)) {
						m_arg.push_back(symbolToSend);
						m_type.push_back(StringParser::TYPE_SYMBOL);
					} else {
						m_arg.push_back(symbolToSend.substr(0, symbolToSend.size() - 1));
						m_type.push_back(StringParser::TYPE_SYMBOL);
					}


				} else {
					m_arg.push_back(currentString);
					m_type.push_back(StringParser::TYPE_STRING);
				}
			}
		}
	}
}

StringParser::~StringParser()
{
}

std::string StringParser::getAddress()
{
	return m_address;
}

unsigned int StringParser::getNbArg()
{
	return m_arg.size();
}

std::string StringParser::getArg(unsigned int i)
{
	if (i > m_arg.size()) {
		throw std::out_of_range("ask for an argument in OSCParser which do not exist");
	}
	return m_arg[i];
}

StringParser::argType StringParser::getType(unsigned int i)
{
	if (i > m_arg.size()) {
		throw std::out_of_range("ask for the type of an argument in OSCParser which do not exist");
	}
	return m_type[i];
}
