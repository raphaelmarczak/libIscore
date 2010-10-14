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
 * \file TriggerPoint.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#include "TriggerPoint.hpp"
#include "ControlPoint.hpp"

#include <sstream>

TriggerPoint::TriggerPoint(unsigned int triggerId)
{
	_relatedControlPoint = NULL;
	_triggerMessage = " ";

	_type = TRIGGER_DEFAULT;
	_previousTriggerId = NO_ID;
	
	_isMute = false;

	setTriggerId(triggerId);
}

void TriggerPoint::setType(int type) 
{
	_type = type;
}

int TriggerPoint::getType() 
{
	return _type;
}

void TriggerPoint::setPreviousTriggerId(unsigned int id) 
{
	_previousTriggerId = id;
}

unsigned int TriggerPoint::getPreviousTriggerId() 
{
	return _previousTriggerId;
}

bool TriggerPoint::setRelatedControlPoint(ControlPoint* relatedControlPoint)
{
	// a controlPoint could not have two triggerPoints !
	if (relatedControlPoint->getTriggerPoint() != NULL) {
		return false;
	}

	if (_relatedControlPoint != NULL) {
		removeRelatedControlPoint();
	}

	_relatedControlPoint = relatedControlPoint;
	_relatedControlPoint->addTriggerPoint(this);

	return true;
}

ControlPoint* TriggerPoint::getRelatedControlPoint()
{
	return _relatedControlPoint;
}

void TriggerPoint::setTriggerMessage(std::string triggerString)
{
	_triggerMessage = triggerString;
}

std::string TriggerPoint::getTriggerMessage()
{
	return _triggerMessage;
}

void TriggerPoint::setTriggerId(unsigned int triggerId)
{
	_id = triggerId;
}

unsigned int TriggerPoint::getTriggerId()
{
	return _id;
}


void TriggerPoint::removeRelatedControlPoint()
{
	if(_relatedControlPoint != NULL) {
		_relatedControlPoint->removeTriggerPoint();
		_relatedControlPoint = NULL;
	}
}

void TriggerPoint::mute(bool muteState)
{
	_isMute = muteState;
}

bool TriggerPoint::isMute()
{
	return _isMute;
}


void
TriggerPoint::store(xmlNodePtr father)
{
	xmlNodePtr node = NULL;

	std::ostringstream oss;
	oss << getTriggerId();

	node = xmlNewNode(NULL, BAD_CAST "TRIGGERPOINT");
	xmlSetProp(node, BAD_CAST "id", BAD_CAST oss.str().data());
	xmlSetProp(node, BAD_CAST "message", BAD_CAST getTriggerMessage().data());


	if (getRelatedControlPoint() != NULL) {
		oss.str("");
		oss << getRelatedControlPoint()->getContainingBoxId();
		xmlSetProp(node, BAD_CAST "relatedBoxId", BAD_CAST oss.str().data());

		oss.str("");
		oss << getRelatedControlPoint()->getId();
		xmlSetProp(node, BAD_CAST "relatedControlPointId", BAD_CAST oss.str().data());
	}

	if(isMute()) {
		xmlSetProp(node, BAD_CAST "mute", BAD_CAST "true");
	} else {
		xmlSetProp(node, BAD_CAST "mute", BAD_CAST "false");
	}

	xmlAddChild(father, node);
}
