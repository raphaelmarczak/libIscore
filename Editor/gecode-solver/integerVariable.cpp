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

#include "integerVariable.hpp"

#include "customSpace.hpp"

IntegerVariable::IntegerVariable(int min, int max, int val, int i, int weight, int pDelta, int nDelta, int total)
{
	_infBound = min;
	_supBound = max;
	_min = min;
	_max = max;
	_val = val;
	_indexInIntVarArray = i;
	_weight = weight;
	_indexOfPosDelta = pDelta;
	_indexOfNegDelta = nDelta;
	_indexOfTotal = total;
}

int
IntegerVariable::getMin() const
{
	return _min;
}

int
IntegerVariable::getMax() const
{
	return _max;
}

int
IntegerVariable::getVal() const
{
	return _val;
}

void
IntegerVariable::setIndex(int i)
{
	_indexInIntVarArray = i;
}

int
IntegerVariable::getIndex() const
{
	return _indexInIntVarArray;
}

int
IntegerVariable::getPosDeltaIndex() const
{
	return _indexOfPosDelta;
}

void
IntegerVariable::setPosDeltaIndex(int i)
{
	_indexOfPosDelta = i;
}

int
IntegerVariable::getNegDeltaIndex() const
{
	return _indexOfNegDelta;
}

void
IntegerVariable::setNegDeltaIndex(int i)
{
	_indexOfNegDelta = i;
}

int
IntegerVariable::getTotalIndex() const
{
	return _indexOfTotal;
}

void
IntegerVariable::setTotalIndex(int i)
{
	_indexOfTotal = i;
}


int
IntegerVariable::getWeight() const
{
	return _weight;
}

void
IntegerVariable::updateDomain(CustomSpace *space)
{
	_min = space->getIntVar(_indexInIntVarArray).min();
	_max = space->getIntVar(_indexInIntVarArray).max();
}

void
IntegerVariable::updateValue(CustomSpace *space)
{
	_val = space->getIntVar(_indexOfTotal).val();
}

void
IntegerVariable::updateValue(int val)
{
	_val = val;
}

void
IntegerVariable::adjustMinMax(bool reduceDomain, int reduction)
{

	if (reduceDomain)
	{
		if (_val - reduction < _infBound){
			_min = _infBound;
		  }
		else{
			_min = _val - reduction;
		  }

		if (_val + reduction > _supBound){
			_max = _supBound;
		  }
		else{
			_max = _val + reduction;
		  }
	}
	else
	{
		_min = _infBound;
		_max = _supBound;
	}
}
