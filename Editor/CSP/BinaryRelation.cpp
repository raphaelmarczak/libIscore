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
 * \file BinaryRelation.cpp
 * \author Bruno Valeze (LaBRI)
 */

#include "BinaryRelation.hpp"

#include "ConstrainedTemporalEntity.hpp"
#include "CSPLinearConstraint.hpp"

BinaryRelation::BinaryRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2)
{
	_entity1 = ent1;
	_entity2 = ent2;
	_constraints = new vector<CSPLinearConstraint*>;
}

BinaryRelation::~BinaryRelation()
{
	_entity1->removeRelatedEntity(_entity2);
	_entity2->removeRelatedEntity(_entity1);
	_constraints->clear();
	delete _constraints;
}

void
BinaryRelation::addLinearConstraint(CSPLinearConstraint *cst)
{
	_constraints->push_back(cst);
}

vector<CSPLinearConstraint*>*
BinaryRelation::constraints() const
{
	return _constraints;
}

ConstrainedTemporalEntity*
BinaryRelation::entity1() const
{
	return _entity1;
}

ConstrainedTemporalEntity*
BinaryRelation::entity2() const
{
	return _entity2;
}

bool
BinaryRelation::implicates(ConstrainedTemporalEntity* ent) const
{
	return ( (ent==entity1()) || (ent==entity2()) );
}

bool
BinaryRelation::validate() const
{
	for (vector<CSPLinearConstraint*>::iterator it = constraints()->begin() ; it != constraints()->end() ; it++) {
		if (! (*it))
			return false;
	}
	return true;
}
