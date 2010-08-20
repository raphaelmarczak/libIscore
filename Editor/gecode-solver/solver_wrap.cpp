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

#include "solver_wrap.hpp"

#include "integerVariable.hpp"
#include "relations_type.hpp"
#include "solver.hpp"

int fonction_test(int i)
{
	return i;
}

void *instanciate_solver()
{
	return new Solver();
}

int add_int_var( void *solver, int min, int max, int val, int weight )
{
	return static_cast<Solver*>(solver)->addIntVar(min, max, val, weight);
}

int remove_int_var( void *solver, int varID )
{
	if (static_cast<Solver*>(solver)->removeIntVar(varID))
		return 1;
	return 0;
}

int add_linear_constraint( void *solver, int *x, int *c, int nbVars, int relType, int val)
{
	return static_cast<Solver*>(solver)->addConstraint(x, c, nbVars, relType, val);
}

int remove_constraint( void *solver, int constID )
{
	if (static_cast<Solver*>(solver)->removeConstraint(constID))
		return 1;
	return 0;
}

int suggest_values( void *solver, int *x, int *vals, int nbVars )
{
	if (static_cast<Solver*>(solver)->suggestValues(x, vals, nbVars))
		return 1;
	return 0;
}

void delete_solver( void *solver )
{
	delete static_cast<Solver*>(solver);
}

void update_variables_values(void *solver)
{
	static_cast<Solver*>(solver)->updateVariablesValues();
}

int get_val( void *solver, int varID )
{
	return static_cast<Solver*>(solver)->getVariableValue(varID);
}

int get_min( void *solver, int varID )
{
	return static_cast<Solver*>(solver)->getVariableMin(varID);
}

int get_max( void *solver, int varID )
{
	return static_cast<Solver*>(solver)->getVariableMax(varID);
}

int memory_peak( void *solver)
{
	return static_cast<Solver*>(solver)->getMemoryPeak();
}

