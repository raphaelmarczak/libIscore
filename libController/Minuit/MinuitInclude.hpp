/*
Copyright: 
LaBRI (http://www.labri.fr) - BlueYeti (http://www.blueyeti.fr) - GMEA (http://www.gmea.net)

Author(s): Raphael Marczak, Laurent Garnier, Theo Delahogue
Last modification: 08/03/2010

Adviser(s): 
Myriam Desainte-Catherine (myriam.desainte-catherine@labri.fr)

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

#ifndef MINUITINCLUDE_HPP_
#define MINUITINCLUDE_HPP_

#include <sys/time.h>

#define MINUIT_REQUEST_DISCOVER "?namespace"
#define MINUIT_REQUEST_GET "?get"
#define MINUIT_REQUEST_LISTEN "?listen"

#define MINUIT_ANSWER_DISCOVER ":namespace"
#define MINUIT_ANSWER_GET ":get"
#define MINUIT_ANSWER_LISTEN ":listen"

#define MINUIT_REQUEST_LISTEN_ENABLE "enable"
#define MINUIT_REQUEST_LISTEN_DISABLE "disable"

#define MINUIT_START_NODES "nodes={"
#define MINUIT_END_NODES "}"
#define MINUIT_START_LEAVES "leaves={"
#define MINUIT_END_LEAVES "}"
#define MINUIT_START_ATTRIBUTES "attributes={"
#define MINUIT_END_ATTRIBUTES "}"

#define ACCESS_GETTER "getter"
#define ACCESS_SETTER "setter"
#define ACCESS_GETSETTER "getsetter"

#define DEFAULT_TIMEOUT 1000
#define NO_TIMEOUT -1

// TODO use the define of Controller.h
#define REQUEST_NOT_SENT -2
#define TIMEOUT_EXCEEDED -1
#define NO_ANSWER 0
#define ANSWER_RECEIVED 1

#endif /*MINUITINCLUDE_HPP_*/
