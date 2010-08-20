/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Bruno Valeze, Raphael Marczak
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

#ifndef INTEGER_VARIABLE_H
#define INTEGER_VARIABLE_H

#include "gecode_headers.hpp"
#include "relations_type.hpp"

#define MAX_AUTHORIZED_MODIFICATION 5000

class CustomSpace;

///////////////////////////////////////////////////////////////////////
//
// An integer variable has a weight and a value, bounded by a min and a max.
// Each integer value is associated with 4 Gecode-variables, see
// IntegerVariable.cpp for further information.
//
///////////////////////////////////////////////////////////////////////

class IntegerVariable {

private :

	int _infBound;
	int _supBound;
	int _min;
	int _max;
	int _val;
	int _indexInIntVarArray;
	int _weight;
	int _indexOfPosDelta;
	int _indexOfNegDelta;
	int _indexOfTotal;

public :

	IntegerVariable(int min, int max, int val, int i, int weight, int pDelta, int nDelta, int total);
	int getMin() const;
	int getMax() const;
	int getVal() const;
	void setIndex(int i);
	int getIndex() const;
	int getPosDeltaIndex() const;
	void setPosDeltaIndex(int i);
	int getNegDeltaIndex() const;
	void setNegDeltaIndex(int i);
	int getTotalIndex() const;
	void setTotalIndex(int i);
	int getWeight() const;
	void updateDomain(CustomSpace *space);
	void updateValue(CustomSpace *space);
	void updateValue(int val);
	void adjustMinMax(bool reduceDomain, int reduction = MAX_AUTHORIZED_MODIFICATION);

};

#endif