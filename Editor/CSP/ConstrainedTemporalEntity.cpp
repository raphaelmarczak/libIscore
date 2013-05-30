/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak, Bruno Valeze
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

/*!
 * \file ConstrainedTemporalEntity.cpp
 * \author Bruno Valeze (LaBRI)
 */

#include "ConstrainedTemporalEntity.hpp"

#include "CSP.hpp"
#include "CSPConstrainedVariable.hpp"

ConstrainedTemporalEntity::ConstrainedTemporalEntity(CSPConstrainedVariable *beg, CSPConstrainedVariable *length)
{
	_beginVar = beg;
	_lengthVar = length;

	_relatedEntities = new vector<ConstrainedTemporalEntity*>;
}

ConstrainedTemporalEntity::~ConstrainedTemporalEntity()
{
	delete _relatedEntities;
}

int
ConstrainedTemporalEntity::beginValue() const
{
	return _beginVar->getValue();
}

int
ConstrainedTemporalEntity::beginID() const
{
	return _beginVar->getID();
}

int
ConstrainedTemporalEntity::lengthValue() const
{
	return _lengthVar->getValue();
}

int
ConstrainedTemporalEntity::lengthID() const
{
	return _lengthVar->getID();
}

int ConstrainedTemporalEntity::getBeginMax() const
{
	return _beginVar->getMax();
}

string
ConstrainedTemporalEntity::name()
{
	return _name;
}

void
ConstrainedTemporalEntity::setName(string n)
{
	_name = n;
}

vector<CSPConstrainedVariable*> *
ConstrainedTemporalEntity::variables() const {
	vector<CSPConstrainedVariable*> *result = new vector<CSPConstrainedVariable*>;
	result->push_back(_beginVar);
	result->push_back(_lengthVar);

	return result;
}

void
ConstrainedTemporalEntity::addRelatedEntity(ConstrainedTemporalEntity *ent) {
	if (!isRelatedWith(ent))
		_relatedEntities->push_back(ent);
}

void
ConstrainedTemporalEntity::removeRelatedEntity(ConstrainedTemporalEntity *ent) {
	vector<ConstrainedTemporalEntity*>::iterator it;
	for (it = _relatedEntities->begin() ; it != _relatedEntities->end() ; it++) {
		if (*it == ent) {
			_relatedEntities->erase(it) ;
			return;
		}
	}
}

bool
ConstrainedTemporalEntity::isRelatedWith(ConstrainedTemporalEntity *ent) const
{
	vector<ConstrainedTemporalEntity*>::iterator it;
	for (it = _relatedEntities->begin() ; it != _relatedEntities->end() ; it++) {
		if (*it == ent)
			return true;
	}
	return false;
}

vector<ConstrainedTemporalEntity*>*
ConstrainedTemporalEntity::relatedEntities() {
	return _relatedEntities;
}


void
ConstrainedTemporalEntity::setId(unsigned int id) {
	_id = id;
}

unsigned int
ConstrainedTemporalEntity::getId() {
	return _id;
}

void ConstrainedTemporalEntity::changeMax(Solver * solver, int newValue)
{
	solver->setIntVar(_beginVar->getID(), _beginVar->getMin(), newValue, _beginVar->getValue(), (int)BEGIN_VAR_TYPE);
	_beginVar->setMax(newValue);
	
	solver->setIntVar(_lengthVar->getID(), _lengthVar->getMin(), newValue, _lengthVar->getValue(), (int)LENGTH_VAR_TYPE);
	_lengthVar->setMax(newValue);

}

void
ConstrainedTemporalEntity::setBeginVar(CSPConstrainedVariable *begin){
  _beginVar = begin;
}

void
ConstrainedTemporalEntity::setLengthVar(CSPConstrainedVariable *length){
  _lengthVar = length;
}

CSPConstrainedVariable *
ConstrainedTemporalEntity::getBeginVar(){
  return _beginVar;
}

CSPConstrainedVariable *
ConstrainedTemporalEntity::getLengthVar(){
  return _lengthVar;
}
