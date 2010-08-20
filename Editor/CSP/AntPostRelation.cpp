/*
Copyright: LaBRI (http://www.labri.fr)

Author(s): Raphael Marczak
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
 * \file AntPostRelation.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "AntPostRelation.hpp"

#include "ConstrainedTemporalEntity.hpp"
#include "CSP.hpp"
#include "CSPLinearConstraint.hpp"

#include <sstream>

using namespace std;

AntPostRelation::AntPostRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2, TemporalRelationType type)
: BinaryTemporalRelation(ent1, ent2)
{
	_type = type;

	_minBound = NO_BOUND;
	_maxBound = NO_BOUND;
}

AntPostRelation::AntPostRelation(ConstrainedTemporalEntity *ent1, ConstrainedTemporalEntity *ent2, TemporalRelationType type, int minBound, int maxBound)
: BinaryTemporalRelation(ent1, ent2)
{
	_type = type;

	_minBound = minBound;
	_maxBound = maxBound;
}

AntPostRelation::~AntPostRelation()
{
}

RelationType
AntPostRelation::relationType() const
{
	return ANTPOST;
}

string
AntPostRelation::toString() const
{
	string s = "AntPost relation : ";

	s += _entity1->name() + " " + CSP::getNameFromRelation(_type) + " " + _entity2->name();

	return s;
}

TemporalRelationType
AntPostRelation::antPostType() const
{
	return _type;
}

int
AntPostRelation::minBound() const
{
	return _minBound;
}

int
AntPostRelation::maxBound() const
{
	return _maxBound;
}

void AntPostRelation::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;

	node = xmlNewNode(NULL, BAD_CAST "RELATION");

	std::ostringstream oss;
	oss << getId();

	xmlSetProp(node, BAD_CAST "id", BAD_CAST oss.str().data());

	xmlSetProp(node, BAD_CAST "type", BAD_CAST CSP::getNameFromRelation(_type).data());

	ControlPoint* firstControlPoint = (ControlPoint*) entity1();
	ControlPoint* secondControlPoint = (ControlPoint*) entity2();

	oss.str("");
	oss << firstControlPoint->getContainingBoxId();

	xmlSetProp(node, BAD_CAST "firstBoxId", BAD_CAST oss.str().data());

	oss.str("");
	oss << firstControlPoint->getId();

	xmlSetProp(node, BAD_CAST "firstPointId", BAD_CAST oss.str().data());

	oss.str("");
	oss << secondControlPoint->getContainingBoxId();

	xmlSetProp(node, BAD_CAST "secondBoxId", BAD_CAST oss.str().data());

	oss.str("");
	oss << secondControlPoint->getId();

	xmlSetProp(node, BAD_CAST "secondPointId", BAD_CAST oss.str().data());

	if (minBound() != NO_BOUND) {
		oss.str("");
		oss << minBound();

		xmlSetProp(node, BAD_CAST "minBound", BAD_CAST oss.str().data());
	}

	if (maxBound() != NO_BOUND) {
		oss.str("");
		oss << maxBound();

		xmlSetProp(node, BAD_CAST "maxBound", BAD_CAST oss.str().data());
	}


	xmlAddChild(father, node);
}
