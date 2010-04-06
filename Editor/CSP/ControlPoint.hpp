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

#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H

/*!
 * \file ControlPoint.h
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "ConstrainedTemporalEntity.hpp"

#include <string>

#include <libxml/tree.h>
#include <libxml/parser.h>

class TriggerPoint;

/*!
 * \class ControlPoint
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */
class ControlPoint : public ConstrainedTemporalEntity
{
public :
	ControlPoint(CSPConstrainedVariable *beg, CSPConstrainedVariable *length, unsigned int containingBoxID);
	~ControlPoint();

	unsigned int getContainingBoxId();

	void setProcessStepId(unsigned int stepId);
	unsigned int getProcessStepId();

	void addTriggerPoint(TriggerPoint* relatedTriggerPoint);
	TriggerPoint* getTriggerPoint();
	void removeTriggerPoint();

	void store(xmlNodePtr father);


private:
	unsigned int _containingBoxID;

	unsigned int _processStepID;

	TriggerPoint* _relatedTriggerPoint;
};

#endif
