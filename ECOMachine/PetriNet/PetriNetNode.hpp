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

#ifndef PETRINETNODE_H_
#define PETRINETNODE_H_

/*!
 * \file PetriNetNode.hpp
 * \author Raphael Marczak (LaBRI), based on Antoine Allombert (LaBRI) LISP code
 * \date 2008-2009
 */

#include "PetriNetItem.hpp"
#include "Arc.hpp"
#include "OutOfBoundException.hpp"
#include "IllegalArgumentException.hpp"

#include <vector>

typedef std::vector<Arc*> arcList;
typedef std::vector<arcList> arcListByColor;
typedef std::vector<PetriNetNode*> petriNetNodeList;

/*!
 * \class PetriNetNode
 *
 * A Petri Net Node is an element which could be linked with another node
 * by an arc. Places and Transitions are nodes.
 *
 */
class PetriNetNode : public PetriNetItem
{
public:
	friend class PetriNet;

	/*!
	 * Destructor.
	 */
	virtual ~PetriNetNode();

	/*!
	 * Gets all the in-going arcs of this node (all colors taken together).
	 *
	 * \return all the in-going arcs of this node (all colors taken together).
	 */
	arcList inGoingArcsOf();

	/*!
	 * Gets the in-going arcs of the given color.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param colorLabel : color of the in-going arcs.
	 * \return the in-going arcs of the given color.
	 */
	arcList inGoingArcsOf(int colorLabel);

	/*!
	 * Gets all the out-going arcs of this node (all colors taken together).
	 *
	 * \return all the out-going arcs of this node (all colors taken together).
	 */
	arcList outGoingArcsOf();

	/*!
	 * Gets the out-going arcs of the given color.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param colorLabel : color of the out-going arcs.
	 * \return the out-going arcs of the given color.
	 */
	arcList outGoingArcsOf(int colorLabel);

	/*!
	 * Sets the in-going arcs list of the given color.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param list : new list of in-going arcs.
	 * \param colorLabel : (optional) color of the in-going arcs list (1 if not provided).
	 */
	void setInGoingArcs(arcList list, int colorLabel = 1);

	/*!
	 * Sets the out-going arcs list of the given color.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param list : new list of out-going arcs.
	 * \param colorLabel : (optional) color of the out-going arcs list (1 if not provided).
	 */
	void setOutGoingArcs(arcList list, int colorLabel = 1);

	/*!
	 * Adds a colored arc to the in-going arcs list.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param arcToAdd : arc to add to the in-going arcs list.
	 * \param colorLabel : (optional) color of the arc to add (1 if not provided).
	 */
	void addInGoingArcs(Arc* arcToAdd, int colorLabel = 1);

	/*!
	 * Adds a colored arc to the out-going arcs list.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param arcToAdd : arc to add to the out-going arcs list.
	 * \param colorLabel : (optional) color of the arc to add (1 if not provided).
	 */
	void addOutGoingArcs(Arc* arcToAdd, int colorLabel = 1);

	/*!
	 * Removes from the in-going arcs list the given arc.
	 *
	 * \param arcToRemove : arc to remove from the in-going arcs list.
	 */
	void removeInGoingArcs(Arc* arcToRemove);

	/*!
	 * Removes from the out-going arcs list the given arc.
	 *
	 * \param arcToRemove : arc to remove from the out-going arcs list.
	 */
	void removeOutGoingArcs(Arc* arcToRemove);

	/*!
	 * Removes from the in-going arcs list the given arc.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param arcToRemove : arc to remove from the in-going arcs list.
	 * \param colorLabel : color of the arc to delete.
	 */
	void removeInGoingArcs(Arc* arcToRemove, int colorLabel);

	/*!
	 * Removes from the out-going arcs list the given arc.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param arcToRemove : arc to remove from the out-going arcs list.
	 * \param colorLabel : color of the arc to delete.
	 */
	void removeOutGoingArcs(Arc* arcToRemove, int colorLabel);

	/*!
	 * Tests if an arc exist between the two given nodes (all colors taken together).
	 *
	 * \param from : node from.
	 * \param to : node to.
	 * \return the matching arc if it exists (NULL if not).
	 */
	friend Arc* haveArc(PetriNetNode* from, PetriNetNode* to);

	/*!
	 * Tests if an arc exist between the two given nodes.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param from : node from.
	 * \param to : node to.
	 * \param colorLabel : arc color.
	 * \return the matching arc if it exists (NULL if not).
	 */
	friend Arc* haveArc(PetriNetNode* from, PetriNetNode* to, int colorLabel);

	/*!
	 * Tests if an arc exist between the current node, and from another given (all colors taken together).
	 *
	 * \param from : node from.
	 * \return the matching arc if it exists (NULL if not).
	 */
	Arc* haveArcFrom(PetriNetNode* from);

	/*!
	 * Tests if an arc exist between the current node, and from another given (all colors taken together).
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param from : node from.
	 * \param colorLabel : arc color.
	 * \return the matching arc if it exists (NULL if not).
	 */
	Arc* haveArcFrom(PetriNetNode* from, int colorLabel);

	/*!
	 * Tests if an arc exist between the current node, and to another given.
	 *
	 * \param to : node to.
	 * \return the matching arc if it exists (NULL if not).
	 */
	Arc* haveArcTo(PetriNetNode* to);

	/*!
	 * Tests if an arc exist between the current node, and to another given.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param to : node to.
	 * \param colorLabel : arc color.
	 * \return the matching arc if it exists (NULL if not).
	 */
	Arc* haveArcTo(PetriNetNode* to, int colorLabel);

	/*!
	 * Gets all successors nodes (all colors taken together).
	 *
	 * \return list of successors nodes.
	 */
	petriNetNodeList returnSuccessors();

	/*!
	 * Gets all successors nodes.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param colorLabel : arc color.
	 * \return list of successors nodes.
	 */
	petriNetNodeList returnSuccessors(int colorLabel);

	/*!
	 * Gets all predecessors nodes (all colors taken together).
	 *
	 * \return list of predecessors nodes.
	 */
	petriNetNodeList returnPredecessors();

	/*!
	 * Gets all predecessors nodes.
	 *
	 * Throws a OutOfBoundException if colorLabel is not valid.
	 *
	 * \param colorLabel : arc color.
	 * \return list of predecessors nodes.
	 */
	petriNetNodeList returnPredecessors(int colorLabel);


private :
	/*!
	 * Default constructor.
	 *
	 * Must not be called (a Node is a PetriNetItem, so it must belong to a Petri Network).
	 */
	PetriNetNode(); // default constructor.

	arcListByColor m_inGoingArcs; // ingoing arcs, by color.

	arcListByColor m_outGoingArcs; // outgoing arcs, by color.

protected :
	/*!
	 * Constructor specifying the Petri Network.
	 *
	 * Must only be called by childs.
	 *
	 * \param petriNet : the PetriNet in which the PetriNetItem is.
	 */
	PetriNetNode(PetriNet* petriNet);

	/*!
	 * Changes the number of colors.
	 * The new number must be greater.
	 *
	 * This function must only be called by a child when it changes its number of colors.
	 *
	 * \param newNbColors : new number of colors
	 */
	void changeNbOfColors(int newNbColors);

	// TODO : m_internAction;
	// TODO : m_externAction;
};

#endif /*PETRINETNODE_H_*/
