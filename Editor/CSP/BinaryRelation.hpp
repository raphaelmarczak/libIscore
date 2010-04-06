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

#ifndef BINARY_RELATION_H
#define BINARY_RELATION_H

/*!
 * \file BinaryRelation.h
 * \author Bruno Valeze (LaBRI)
 */

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "../../Includes/CSPTypes.hpp"

class CSPLinearConstraint;
class ConstrainedTemporalEntity;

/*!
 * \class BinaryRelation
 *
 * \brief Base class for binary relations.
 *
 * Representing a relation between 2 entities and composed of a set
 * of constraints.
 *
 * \author Bruno Valeze (LaBRI)
 * \date 2008-2009
 */
class BinaryRelation
{
public :

	BinaryRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2);
	virtual ~BinaryRelation();

	// accessor for the linear constraints
	vector<CSPLinearConstraint*> *constraints() const;

	// accessors for the 2 entities
	ConstrainedTemporalEntity *entity1() const;
	ConstrainedTemporalEntity *entity2() const;

	// must be implemented for each derived class
	virtual RelationType relationType() const = 0;
	virtual string toString() const = 0;

	// add a linear constraint to the binary relation
	void addLinearConstraint(CSPLinearConstraint *cst);

	// TRUE if ent if one of the 2 entities
	bool implicates(ConstrainedTemporalEntity* ent) const;

	// TRUE if the relation is compatible with the system
	bool validate() const;

protected :

	ConstrainedTemporalEntity *_entity1;
	ConstrainedTemporalEntity *_entity2;
	vector<CSPLinearConstraint*> *_constraints;
};


#endif
