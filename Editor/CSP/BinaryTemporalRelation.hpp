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

#ifndef BINARY_TEMPORAL_RELATION_H
#define BINARY_TEMPORAL_RELATION_H

/*!
 * \file BinaryTemporalRelation.h
 * \author Bruno Valeze (LaBRI)
 */

#include "../../Includes/CSPTypes.hpp"
#include "BinaryRelation.hpp"

class ConstrainedTemporalEntity;

/*!
 * \class BinaryTemporalRelation
 *
 * \brief Class for binary temporal relations, derived from BinaryRelation.
 *
 * A binary temporal relation is always defined between 2 objects.
 * It's composed of several linear constraints that implicate the
 * temporal variables of the 2 objects.
 *
 * \author Bruno Valeze (LaBRI)
 * \date 2008-2009
 */
class BinaryTemporalRelation : public BinaryRelation
{
public :
	BinaryTemporalRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2);
	~BinaryTemporalRelation();

	unsigned int getId();
	void setId(unsigned int newId);

protected :
private :
	int m_id;

};


#endif
