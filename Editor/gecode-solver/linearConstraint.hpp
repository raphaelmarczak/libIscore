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

#ifndef LINEAR_CONSTRAINT_H
#define LINEAR_CONSTRAINT_H

#include "gecode_headers.hpp"

class Solver;

///////////////////////////////////////////////////////////////////////
//
// A linear constraint is defined by :
//	- a linear combination of variables
//  - a comparison operator
//  - an integer value
// example : a_1*var_1 + a_2*var_2 + ... + a_n*var_n <= c
//
///////////////////////////////////////////////////////////////////////

class LinearConstraint  
{

private :

	Solver *_solver;
	IntRelType _relType;
	vector<int> *_varsCoeffs;
	vector<int> *_varsIDs;
	int _val;

public :

	LinearConstraint();
	LinearConstraint(Solver *s, vector<int> *varsIDs, vector<int> *varsCoeffs, IntRelType relType, int val);
	~LinearConstraint();

	// add the constraint to Gecode's space
	void addToSpace() const;	

	// TRUE if the constraint implicated the variable whith the id 'i'
	bool dependsOn(int i) const;	

};

#endif