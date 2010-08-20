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

#include "linearConstraint.hpp"

#include "customSpace.hpp"
#include "integerVariable.hpp"
#include "solver.hpp"

LinearConstraint::LinearConstraint()
{
}

LinearConstraint::LinearConstraint(Solver *s, vector<int> *varsIDs, vector<int> *varsCoeffs, IntRelType relType, int val)
{
	_solver = s;
	_varsIDs = varsIDs;
	_varsCoeffs = varsCoeffs;
	_relType = relType;
	_val = val;
}

LinearConstraint::~LinearConstraint()
{
	_varsCoeffs->clear();
	delete _varsCoeffs;
	_varsIDs->clear();
	delete _varsIDs;
}

void 
LinearConstraint::addToSpace() const
{
	int size = _varsIDs->size();

	// vars is an array of the IntVar instances implicated in the constraint
	IntVarArgs vars(size);
	// coeffs is an array of integer coefficients for the variables
	IntArgs coeffs(size);

	for (int i=0; i<size; i++)
	{
		IntegerVariable *iv = _solver->varFromID(_varsIDs->at(i));
		vars[i] = _solver->getSpace()->getIntVar(iv->getTotalIndex());
		coeffs[i] = _varsCoeffs->at(i);
	}

//TODO: avant "Gecode::linear((Space*)_solver->getSpace(), coeffs, vars, _relType, _val);"
	Gecode::linear(*(Space*)_solver->getSpace(), coeffs, vars, _relType, _val); 
}

bool 
LinearConstraint::dependsOn(int id) const
{
	for (unsigned int i=0; i<_varsIDs->size(); i++)
		if (_varsIDs->at(i) == id)
			return true;

	return false;
}


