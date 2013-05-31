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

#ifndef CONSTRAINED_BOX_H
#define CONSTRAINED_BOX_H

/*!
 * \file ConstrainedBox.h
 * \author Bruno Valeze (LaBRI) and Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "ConstrainedTemporalEntity.hpp"
#include "ControlPoint.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <libxml/tree.h>
#include <libxml/parser.h>

class CSPConstrainedVariable;
//class BoundingRelation;
class CSP;

/*!
 * \class ConstrainedBox
 *
 * \brief A ConstrainedBox is the logical representation of a GUIBasicBox.
 *
 * This class aims to separate the graphical layer of the application whith
 * the classes derived from GUIBasicBox, and the logical layer which consists
 * in temporal constraints, numerical data... A ConstrainedBox is an object
 * representing a graphical box for the constraints solver.
 *
 * \author Bruno Valeze and Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class ConstrainedBox : public ConstrainedTemporalEntity
{
public :
	ConstrainedBox(CSPConstrainedVariable *beg, CSPConstrainedVariable *length);

	ConstrainedBox* mother() const;
//	BoundingRelation* boundingRelation() const;

	std::vector<ConstrainedBox*> *children() const;
	std::vector<ConstrainedBox*> *descendants() const;

	void setMother(ConstrainedBox *box);

	bool isDescendantOf(ConstrainedBox *box);
	bool isAncestorOf(ConstrainedBox *box);

	void addChild(ConstrainedBox *box);
	void removeChild(ConstrainedBox *box);

	void addControlPoint(ControlPoint* controlPoint, unsigned int index);
	int nbControlPoint();
	ControlPoint* getControlPoint(unsigned int i);
	ControlPoint* getFirstControlPoint();
	ControlPoint* getLastControlPoint();

	void getAllControlPointsId(std::vector<unsigned int>* controlPointID);

	unsigned int getFlexibilityRelationId();
	unsigned int getHierarchyRelationId();

	int minBound() const;
	int maxBound() const;

	CSP* getCSP() const;

	void setFlexibilityInformations(unsigned int flexibilityRelationId, int minBound, int maxBound);
	void setHierarchyInformations(unsigned int hierarchyRelationId);

	friend std::ostream &operator<<(std::ostream &stream, ConstrainedBox& ob);

	void setOptionalArgument(std::string key, std::string value);
	void removeOptionalArgument(std::string key);
	std::map<std::string, std::string> getOptionalArguments();

	void store(xmlNodePtr father);

	void lockDuration();
	void lockDuration(int duration);
	void unlockDuration();

private :
	//std::string _ipAddress;
	//unsigned int _port;

	CSP* m_csp;

	//GUIBasicBox *_gBox ;
	ConstrainedBox *m_motherBox;  // the possible bounding box
	std::vector<ConstrainedBox*> *m_childrenBoxes; // the boxes bounded by this

	std::map<unsigned int, ControlPoint*> *m_controlPoints;

	std::map<std::string, std::string> m_optionalArguments;

	int m_minBound;
	int m_maxBound;

	unsigned int m_flexiblityRelationId;
	unsigned int m_hierarchyRelationId;
};




#endif
