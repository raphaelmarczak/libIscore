/*
Copyright: 
LaBRI (http://www.labri.fr) - BlueYeti (http://www.blueyeti.fr) - GMEA (http://www.gmea.net)

Author(s): Raphael Marczak, Laurent Garnier, Theo Delahogue
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

#include "OSCParser.hpp"
#include "OSCFunctions.hpp"

#include <sstream>
#include <iterator>

#include <stdexcept>

OSCParser::OSCParser()
{
	
}

OSCParser::OSCParser(std::string OSCMessageToParse)
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
				m_type.push_back(osc::INT32_TYPE_TAG);
			} else if (isFloat(currentString)) {
				m_arg.push_back(currentString);
				m_type.push_back(osc::FLOAT_TYPE_TAG);
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
						m_type.push_back(osc::SYMBOL_TYPE_TAG);
					} else {
						m_arg.push_back(symbolToSend.substr(0, symbolToSend.size() - 1));
						m_type.push_back(osc::SYMBOL_TYPE_TAG);
					}


				} else {
					m_arg.push_back(currentString);
					m_type.push_back(osc::STRING_TYPE_TAG);
				}
			}
		}
	}
}

OSCParser::~OSCParser()
{
}

std::string OSCParser::getAddress() 
{
	return m_address;
}

unsigned int OSCParser::getNbArg()
{
	return m_arg.size();
}

std::string OSCParser::getArg(unsigned int i)
{
	if (i > m_arg.size()) {
		throw std::out_of_range("ask for an argument in OSCParser which do not exist");
	}
	return m_arg[i];
}

char OSCParser::getType(unsigned int i)
{
	if (i > m_arg.size()) {
		throw std::out_of_range("ask for the type of an argument in OSCParser which do not exist");
	}
	return m_type[i];
}
