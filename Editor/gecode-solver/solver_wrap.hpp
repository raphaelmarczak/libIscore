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

#ifndef SOLVER_WRAP_H
#define SOLVER_WRAP_H

#ifdef __cplusplus
extern "C" {
#endif

#define DISALLOWED_CONSTRAINT -1

#include "relations_type.hpp"

	// initialization of the solver
	void *instanciate_solver();

	// add an integer variable between min and max and return its ID
	int add_int_var( void *solver, int min, int max, int val, int weight );

	// remove a variable
	int remove_int_var( void *solver, int varID );

	// add a constraint "c[0].x[0] + c[1].x[1] + ... + c[nbVar-1].x[nbVar-1] <relType> val"
	// and return its ID or DISALLOWED_CONSTRAINT if the constraint can't be added
	int add_linear_constraint( void *solver, int *x, int *c, int nbVars, int relType, int val);

	// remove a constraint or DISALLOWED_CONSTRAINT if the constraint can't be added
	int remove_constraint( void *solver, int constID );

	// TODO
	int suggest_values( void *solver, int *x, int *vals, int nbVars );

	// deallocate solver
	void delete_solver( void *solver );

	// calculate new values for the current set of variables
	void update_variables_values(void *solver);

	// get value and bounds for the variable <varID>
	int get_val( void *solver, int varID );
	int get_min( void *solver, int varID );
	int get_max( void *solver, int varID );

	// for debugging : returns memory peak
	int memory_peak( void *solver);

#ifdef __cplusplus
};

#endif

#endif
