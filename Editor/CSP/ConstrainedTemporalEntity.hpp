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

#ifndef CONSTRAINED_TEMPORAL_ENTITY
#define CONSTRAINED_TEMPORAL_ENTITY

/*!
 * \file ConstrainedTemporalEntity.h
 * \author Bruno Valeze (LaBRI)
 */

#include "../gecode-solver/solver.hpp"

#include <string>
#include <vector>

class CSPConstrainedVariable;

/*!
 * \class ConstrainedTemporalEntity
 *
 * \brief Base class for the logical representation of a graphical object in the solver.
 *
 * A ConstrainedTemporalEntity is the logical representation of a
 * graphical object which can be subject to temporal relations.
 * It has 4 interger variables : begin date and y coordinates of the upper left
 * corner, length and vertical size.
 *
 * \author Bruno Valeze (LaBRI)
 * \date 2008-2009
 */
class ConstrainedTemporalEntity
{
public :

	ConstrainedTemporalEntity(CSPConstrainedVariable *beg, CSPConstrainedVariable *length);
	virtual ~ConstrainedTemporalEntity();

	// Accessors for the 4 variables
	int beginValue() const;
	int beginID() const;
	int lengthValue() const;
	int lengthID() const;

	int getBeginMax() const;

	std::string name();
	void setName(std::string n);

	// Return a vector of the variables' pointers
	virtual std::vector<CSPConstrainedVariable*> *variables() const;

	// Add a related entity, i.e. which is linked by a binary relation
	void addRelatedEntity(ConstrainedTemporalEntity *ent);

	// Remove a related entity
	void removeRelatedEntity(ConstrainedTemporalEntity *ent);

	// TRUE if the entity 'ent' is in relation whith this
	bool isRelatedWith(ConstrainedTemporalEntity *ent) const;

	// Returns related entities
	std::vector<ConstrainedTemporalEntity*>* relatedEntities();

	void setId(unsigned int id);
	unsigned int getId();
	
	void changeMax(Solver* solver, int newValue);
	void setBeginVar(CSPConstrainedVariable *begin);
	void setLengthVar(CSPConstrainedVariable *length);
	CSPConstrainedVariable *getBeginVar();
	CSPConstrainedVariable *getLengthVar();

protected :
	unsigned int _id;

	std::string _name;

	CSPConstrainedVariable *_beginVar;
	CSPConstrainedVariable *_lengthVar;

	std::vector<ConstrainedTemporalEntity*> *_relatedEntities;

};

#endif
