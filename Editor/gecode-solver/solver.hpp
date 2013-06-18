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

#ifndef SOLVER_H
#define SOLVER_H

#include "gecode_headers.hpp"
#include "relations_type.hpp"

class CustomSpace;
class IntegerVariable;
class LinearConstraint;
class SearchEngine;
class linearConstraint;

#define NO_MAX_MODIFICATION 0

///////////////////////////////////////////////////////////////////////
//
// Top-level interface for the Gecode Solver.
// This class provides all the functions needed to solve Constraints
// Solving Problems having several balanced integer variables, and 
// several linear constraints on these variables.
//
///////////////////////////////////////////////////////////////////////

class Solver {

private :

	// Gecode space
	CustomSpace *_space;
	CustomSpace *_lastSpace;

	// Hash table containing the currently used variables
	map<int, IntegerVariable*> *_integerVariablesMap;

	// Hash table containing linear constraints
	map<int, LinearConstraint*> *_constraintsMap;

	// The search engine to find a solution
	SearchEngine *_engine;

	// Reloads all variables and constraints
	void updateState();

	// Get the relation type as defined by Gecode
	static IntRelType getGecodeRelType(int relType);

	// Find the lowest ID to insert a new element in a map
	int findNewRelationID() const;
	int findNewVariableID() const;

	// Launch the search engine
	CustomSpace *run();

	// To put strong variables when edition
	vector<int> *_strongVars ;

	// true if edition
	bool _suggest;
	
	int _maxModification;

public :

	Solver();
	~Solver();

	// Access to the variables and relations
	IntegerVariable *varFromID(int varID) const;
	LinearConstraint *constraintFromID(int constID) const;

	// Accessor to the CustomSpace Pointer
	CustomSpace *getSpace() const;

	//Reset the previous space
	void resetSpace();

	// inserts a new entry in the variables map and return the variable ID
	int addIntVar(int min, int max, int val, int weight);
	
	int setIntVar(int id, int min, int max, int val, int weight);

	// removes a variable from the variables map
	bool removeIntVar(int varID);

	// inserts a new entry in the relations map and return the constraint ID
	int addConstraint(int *varsIDs, int *varsCoeffs, int nbVars, int relType, int val, bool mustCallSolver = true);

	// remove a constraint from the solver
	bool removeConstraint(int constID); 

	// check if the new value for the variable 'varID' is in the variable's domain
	bool suggestValues(int *varsIDs, int* values, int nbVars, int maxModification = NO_MAX_MODIFICATION);

	// find a solution to the system
	// returns NULL if there's no solution
	CustomSpace *solve();

	// update the value of each variable
	bool updateVariablesValues();

	// get the values for each variable
	int getVariableValue(int varID) const;
	int getVariableMin(int varID) const;
	int getVariableMax(int varID) const;

	int getMemoryPeak();
};


#endif // SOLVER_H
