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

#include "customSpace.hpp"

CustomSpace::CustomSpace() 
//TODO: avant ": Space(), _dat(this, 0), _objFuncInitialized(false)"
: Space(), _dat(*this, 0), _objFuncInitialized(false)
{
	_lastVal = -1;
	_cpt = 0;
}

CustomSpace::CustomSpace(bool share, CustomSpace& s) : Space(share,s) 
{
	//TODO: avant "_dat.update(this, share, s._dat);"
	_dat.update(*this, share, s._dat);
	_objFuncInitialized = s._objFuncInitialized;
	_lastVal = s._lastVal;
	_cpt = s._cpt;

	if (_objFuncInitialized)
	//TODO: avant "_objFunc.update(this,share,s._objFunc);	"
		_objFunc.update(*this,share,s._objFunc);	
}

CustomSpace::~CustomSpace()
{
}

int
CustomSpace::getNbVars() const
{
	return _dat.size();
}

void 
CustomSpace::setObjFunc(IntVar v)
{
	_objFunc = v;
	_objFuncInitialized = true;
}

void
CustomSpace::constrain(const Space& t)
{
//TODO: avant "rel(this, _objFunc, IRT_LE, (s)->_objFunc);"
		
	Space* s = &const_cast<Space&>(t);
	
	CustomSpace* s_copy = (CustomSpace*) s;
	
//	std::cout << "BRAAAAAA" << std::endl;		
	
	rel(*this, _objFunc, IRT_LE, (s_copy)->_objFunc);
}

CustomSpace* 
CustomSpace::copy(bool share) 
{
	return new CustomSpace(share,*this);
}

int
CustomSpace::addVariable(int min, int max)
{
//TODO: avant "	IntVarArray newArray(this, _dat.size()+1);"
//TODO:	avant "IntVar v(this, min, max);"

	IntVarArray newArray(*this, _dat.size()+1);
	IntVar v(*this, min, max); //crash

	// Copy from the old array to the new
	for (int i=0; i<newArray.size()-1; i++)
		newArray[i] = _dat[i];
	newArray[newArray.size()-1] = v;

	// Replace the array
	_dat = newArray;

	return _dat.size()-1;
}

void
CustomSpace::doBranching()
{
//TODO: avant "	branch(this, _dat, BVAR_MAX_MIN, BVAL_MIN);"
	branch(*this, _dat, INT_VAR_MAX_MIN, INT_VAL_MIN);
}

IntVar 
CustomSpace::getIntVar(int i) const
{
	return _dat[i];
}
