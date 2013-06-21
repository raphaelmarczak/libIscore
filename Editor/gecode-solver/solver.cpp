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

#include "solver.hpp"

#include "customSpace.hpp"
#include "integerVariable.hpp"
#include "linearConstraint.hpp"
#include "relations_type.hpp"
#include "searchEngine.hpp"

Solver::Solver()
{
	_space = new CustomSpace();
	_lastSpace = new CustomSpace();
	_integerVariablesMap = new map<int, IntegerVariable*>;
	_constraintsMap = new map<int, LinearConstraint*>;
	_engine = new SearchEngine(_space);
	_suggest = false;

	_maxModification = NO_MAX_MODIFICATION;
}

Solver::~Solver()
{
	map<int, IntegerVariable*>::iterator p;
	for (p = _integerVariablesMap->begin(); p != _integerVariablesMap->end(); p++)
	{
		IntegerVariable *oldVar = p->second;
		delete(oldVar);
	}
	delete(_integerVariablesMap);
	_integerVariablesMap = NULL;

	if (_space)
	{
		delete(_space);
		_space = NULL;
	}

	if (_lastSpace)
	{
		delete(_lastSpace);
		_lastSpace = NULL;
	}
}

// find the Gecode relation type corresponding to a binary relation
IntRelType
Solver::getGecodeRelType(int relType)
{
	IntRelType rt = IRT_EQ;

	switch (relType)
	{
	case REL_EQ :
		rt = IRT_EQ;
		break;
	case REL_NQ :
		rt = IRT_NQ;
		break;
	case REL_LQ :
		rt = IRT_LQ;
		break;
	case REL_LE :
		rt = IRT_LE;
		break;
	case REL_GQ :
		rt = IRT_GQ;
		break;
	case REL_GR :
		rt = IRT_GR;
		break;
	default : break;
	}

	return rt;
}

// find the lowest free integer ID for a constraint
int
Solver::findNewRelationID() const
{
	int newID = 0;
	while (_constraintsMap->find(newID) != _constraintsMap->end())
		newID++;
	return newID;
}

// find the lowest free integer ID for a variable
int
Solver::findNewVariableID() const
{
	int newID = 0;
	while (_integerVariablesMap->find(newID) != _integerVariablesMap->end())
		newID++;
	return newID;
}

CustomSpace*
Solver::getSpace() const
{
	return _space;
}

// find an IntegerVariable from its ID
IntegerVariable *
Solver::varFromID(int varID) const
{
	return (_integerVariablesMap->find(varID))->second ;
}

// find a linearConstraint from its ID
LinearConstraint *
Solver::constraintFromID(int constID) const
{
	return (_constraintsMap->find(constID))->second ;
}

int
Solver::addIntVar(int min, int max, int val, int weight)
{
	// to each abstract variable we associate 4 Gecode variable
	// usefull to minimize the variations
	int index = _space->addVariable(min, max);
	int pdelta = _space->addVariable(0, max);
	int ndelta = _space->addVariable(0, max);
	int total = _space->addVariable(min, max);

	// tha abstract variable, i.e. the variable for the solver user
	IntegerVariable *newVar = new IntegerVariable(min, max, val, index, weight, pdelta, ndelta, total);
	int newID = findNewVariableID();
	_integerVariablesMap->insert(pair<int, IntegerVariable*>(newID, newVar));

	return newID;
}

int
Solver::setIntVar(int id, int min, int max, int val, int weight)
{
	//removeIntVar(id);

	// to each abstract variable we associate 4 Gecode variable
	// usefull to minimize the variations
	int index = _space->addVariable(min, max);
	int pdelta = _space->addVariable(0, max);
	int ndelta = _space->addVariable(0, max);
	int total = _space->addVariable(min, max);

	// tha abstract variable, i.e. the variable for the solver user
	IntegerVariable *newVar = new IntegerVariable(min, max, val, index, weight, pdelta, ndelta, total);

	(*_integerVariablesMap)[id] = newVar;

	return id;
}

bool
Solver::removeIntVar(int varID)
{
	map<int, IntegerVariable*>::iterator p = _integerVariablesMap->find(varID);
	if (p == _integerVariablesMap->end())
		return false;

	IntegerVariable *oldVar = p->second;
	delete(oldVar);
	_integerVariablesMap->erase(p);

	vector<int> constraintsToRemove;

	map<int, LinearConstraint*>::iterator i;

	// find the relations implicating the deleted variable
	for (i = _constraintsMap->begin() ; i != _constraintsMap->end() ; i++)
		if ( (i->second)->dependsOn(varID) )
			constraintsToRemove.push_back(i->first);

	// delete the obsolete relations
	while (!constraintsToRemove.empty())
	{
		i = _constraintsMap->find(constraintsToRemove.back());
		delete(i->second);
		_constraintsMap->erase(i);
		constraintsToRemove.pop_back();
	}

	return true;
}

int
Solver::addConstraint(int *varsIDs, int *varsCoeffs, int nbVars, int relType, int val, bool mustCallSolver)
{
	vector<int> *ids = new vector<int>;
	vector<int> *coeffs = new vector<int>;

	IntRelType rt = getGecodeRelType(relType);

	for (int i=0; i<nbVars; i++)
	{
		ids->push_back( varsIDs[i] );
		coeffs->push_back( varsCoeffs[i] );
	}

	// new linear constraint
	LinearConstraint *newCst = new LinearConstraint(this, ids, coeffs, rt, val);

	int newID = findNewRelationID();

	// insert the constraint in the map
	_constraintsMap->insert(pair<int, LinearConstraint*>(newID, newCst));

	if (!mustCallSolver) {
		return newID;
	}

	if (updateVariablesValues())
		return newID;

	removeConstraint(newID);
	return -1;
}

bool
Solver::removeConstraint(int constID)
{
	map<int, LinearConstraint*>::iterator p = _constraintsMap->find(constID);
	if (p == _constraintsMap->end())
		return false;

	delete(p->second);
	_constraintsMap->erase(p);

	return true;
}

// rebuild a Gecode space with the current variable and constraints
void
Solver::updateState()
{
  if (_space)
    {
      _lastSpace = (CustomSpace*)_space;
      delete _space;
      _space = NULL;
    }

  _space = new CustomSpace();

  // create gecode variables
  for (map<int, IntegerVariable*>::iterator q = _integerVariablesMap->begin(); q != _integerVariablesMap->end(); q++)
    {
      IntegerVariable *v = q->second;

      if ((_suggest) && (_maxModification != NO_MAX_MODIFICATION)) {
          v->adjustMinMax(_suggest, _maxModification);
        } else {
          v->adjustMinMax(_suggest);
        }


      v->setIndex(_space->addVariable(v->getVal(), v->getVal()));
      v->setPosDeltaIndex(_space->addVariable(0, v->getMax() - v->getVal() + 1));
      v->setNegDeltaIndex(_space->addVariable(0, v->getVal() - v->getMin() + 1));//crash
      v->setTotalIndex(_space->addVariable(v->getMin(), v->getMax()));

    }
  // add constraints to space
  for (map<int, LinearConstraint*>::iterator p = _constraintsMap->begin(); p != _constraintsMap->end(); p++)
    {
      (p->second)->addToSpace();
    }

  //TODO: avant "LinExpr expr;"
  LinExpr expr;
  bool init=false;

  // construct the linear combination of delta variables balanced by the weight associated with their type
  for (map<int, IntegerVariable*>::iterator q = _integerVariablesMap->begin(); q != _integerVariablesMap->end(); q++)
    {
      IntegerVariable *currVar = q->second;

      // add a delta variable for each beginning or length
      int multiplier = 1;

      bool isStrong = false;

      // give more weight to the edited variables
      if (_suggest)
        {
          for (unsigned int i=0; i<_strongVars->size(); i++)
            if (_strongVars->at(i) == q->first)
              {
                isStrong = true;
                //multiplier = 10;
                break;
              }
        }

      IntVarArgs vars(4);
      IntArgs coeffs(4);

      if (isStrong) {
          vars[0] = _space->getIntVar(currVar->getTotalIndex());
          vars[1] = _space->getIntVar(currVar->getIndex());
          vars[2] = _space->getIntVar(currVar->getTotalIndex());
          vars[3] = _space->getIntVar(currVar->getIndex());

          coeffs[0] = -1;
          coeffs[1] = 1;
          coeffs[2] = -1;
          coeffs[3] = 1;
        } else {
          // constraint : <initial or wanted value> + <positive delta> - <negative delta> = <optimal value>
          vars[0] = _space->getIntVar(currVar->getNegDeltaIndex());
          vars[1] = _space->getIntVar(currVar->getPosDeltaIndex());
          vars[2] = _space->getIntVar(currVar->getTotalIndex());
          vars[3] = _space->getIntVar(currVar->getIndex());
          coeffs[0] = -1;
          coeffs[1] = 1;
          coeffs[2] = 1;
          coeffs[3] = -1;
        }

      //TODO: avant "linear(_space, coeffs, vars, IRT_EQ, 0);"
      linear(*_space, coeffs, vars, IRT_EQ, 0);

      // construction of the objective function
      if (!init)
        {
          //TODO: avant "expr = LinExpr(vars[0], currVar->getWeight()*multiplier);"
          expr = LinExpr(vars[0], currVar->getWeight()*multiplier);

          init = true;

          //TODO: avant "LinExpr tmp(vars[1], currVar->getWeight()*multiplier);"
          LinExpr tmp(vars[1], currVar->getWeight()*multiplier);

          //TODO : je ne sais pas : "expr = LinExpr(expr, tmp, 1);"
          expr = LinExpr(expr, Gecode::LinExpr::NT_ADD, tmp);
        }
      else
        {
          //TODO: avant "LinExpr tmp(vars[0], currVar->getWeight()*multiplier);"
          LinExpr tmp(vars[0], currVar->getWeight()*multiplier);

          //TODO: je ne sais pas : "expr = LinExpr(expr, tmp, 1);"
          expr = LinExpr(expr, Gecode::LinExpr::NT_ADD, tmp);

          //TODO: avant "tmp = LinExpr(vars[1], currVar->getWeight()*multiplier);"
          tmp = LinExpr(vars[1], currVar->getWeight()*multiplier);

          //TODO: je ne sas pas : "expr = LinExpr(expr, tmp, 1);"
          expr = LinExpr(expr, Gecode::LinExpr::NT_ADD, tmp);
        }
    }

  // the objective function is a linear combination of the delta variables (lengths are more important than beginnings)
  _space->setObjFunc(Gecode::expr(*_space, expr));
}

// edit some variables and try to reach the new values
bool
Solver::suggestValues(int *varsIDs, int* values, int nbVars, int maxModification)
{
	_suggest = true;

	_maxModification = maxModification;

	_strongVars = new vector<int>;

	// distinction between edited variables and the other
	for (int i=0; i<nbVars; i++)
	{
		map<int, IntegerVariable*>::iterator p = _integerVariablesMap->find(varsIDs[i]);
		if (p == _integerVariablesMap->end())
		{
			return false;
		}
		else
		{
			(p->second)->updateValue(values[i]);
			_strongVars->push_back(varsIDs[i]);
		}
	}
	bool res = updateVariablesValues();//crash

	_strongVars->clear();
	delete _strongVars;
	_strongVars = NULL;

	_suggest = false;
	_maxModification = NO_MAX_MODIFICATION;
	return res;
}

bool
Solver::updateVariablesValues()
{
	CustomSpace *result = solve();//crash
	if (result == NULL) {
		return false;
	}


	if (_space)
	{
		delete _space;
		_space = NULL;
	}
	_space = (CustomSpace*)result->clone(false);
	delete result; // des fois \8Da merde !!!

	for (map<int, IntegerVariable*>::iterator q = _integerVariablesMap->begin(); q != _integerVariablesMap->end(); q++)
		(q->second)->updateValue(_space);
	return true;
}

CustomSpace *
Solver::solve()
{
	updateState();//crash

	// branch variables
	_space->doBranching();

	return run();
}


// Returns the best solution, NULL if the system can't be solved
CustomSpace *
Solver::run()
{
	if (_engine)
	{
		delete (_engine);
		_engine = NULL;
	}

	Search::TimeStop* ts = new Search::TimeStop(100);
	Search::Options o;

	o.stop = ts;

	_engine = new SearchEngine(_space, o);

	CustomSpace *last = NULL;

	while (true)
	{
		CustomSpace *ex = _engine->next();

		// When there's no better solution, stop the search
		if (ex == NULL)
			break;

		if (last)
		{
			delete last; // des fois \8Da merde !!!
			last = NULL;
		}

		last = (CustomSpace*)ex->clone(false);
		delete(ex);
	}

	delete ts;	
	return last;
}

int
Solver::getMemoryPeak()
{
	if (_engine) return _engine->getMemoryPeak();
	return 0;
}

int
Solver::getVariableValue(int varID ) const
{
	IntegerVariable *var = varFromID(varID);
	if (var)
		return var->getVal();
	return -1;
}

int
Solver::getVariableMin(int varID ) const
{
	IntegerVariable *var = varFromID(varID);
	if (var)
		return var->getMin();
	return -1;
}

int
Solver::getVariableMax(int varID ) const
{
	IntegerVariable *var = varFromID(varID);
	if (var)
		return var->getMax();
	return -1;
}

void
Solver::resetSpace(){
  if(_lastSpace!=NULL){
//    _space = (CustomSpace*)_lastSpace->clone(true);
      _space = (CustomSpace*)_lastSpace;
    updateState();
    }
}
