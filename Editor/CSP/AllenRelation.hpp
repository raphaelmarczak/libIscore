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

#ifndef ALLEN_RELATION_H
#define ALLEN_RELATION_H

/*!
 * \file AllenRelation.h
 * \author Bruno Valeze (LaBRI)
 */

#include "../../Includes/CSPTypes.hpp"
#include "BinaryTemporalRelation.hpp"

class CSPLinearConstraint;

/*!
 * \class AllenRelation
 *
 * \brief Class for Allen relations.
 *
 * This class is derived from BinaryTemporalRelation. Allen relations are :
 * before, after, meets, during, equals, starts, overlaps, finishes.
 * An Allen relation is a binary temporal relation.
 *
 * \author Bruno Valeze (LaBRI)
 */
class AllenRelation : public BinaryTemporalRelation
{
public :

	AllenRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2, AllenType type);
	~AllenRelation();

	RelationType relationType() const;
	AllenType allenType() const;
	string toString() const;

private :

	AllenType _type;
};


#endif

