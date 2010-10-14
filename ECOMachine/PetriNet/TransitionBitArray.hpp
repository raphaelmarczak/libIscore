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

#ifndef TRANSITIONBITARRAY_H_
#define TRANSITIONBITARRAY_H_

#include <stdint.h>
#include "OutOfBoundException.hpp"

/*!
 * \file BitArray.hpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

#define INT_SIZE 16

/*!
 * \class BitArray
 *
 * This array permits to quickly know if every bits is 1.
 */
class TransitionBitArray
{
public:
	/*!
	 * Constructor
	 *
	 * Throws OutOfBoundException if size is 0.
	 *
	 * Creates a BitArray with a given size.
	 *
	 * \param size : size of the BitArray.
	 */
	TransitionBitArray(unsigned int size);

	/*!
	 * Destructor
	 */
	~TransitionBitArray();

	/*!
	 * Sets the bit at given index as 1.
	 *
	 * Throws OutOfBoundException if index is greater or equal the bitArray size.
	 *
	 * \param index : index where the bit will be change to 1.
	 */
	void setToOne(unsigned int index);

	/*!
	 * Sets the bit at given index as 0.
	 *
	 * Throws OutOfBoundException if index is greater or equal the bitArray size.
	 *
	 * \param index : index where the bit will be change to 0.
	 */
	void setToZero(unsigned int index);

	/*!
	 * Sets all bits to 0.
	 */
	void eraseArray();

	/*!
	 * Tests if all bits are 1.
	 *
	 * \return true if all bits are 1.
	 */
	bool isAllOne();

	/*!
	 * Gets the BitArray size.
	 *
	 * \return the BitArray size.
	 */
	unsigned int getSize();

private :
	unsigned int m_size; // BitArray size.
	unsigned int m_maxIndexInBitArray; // Max index of the BitArray.
	uint16_t* m_bitArray; // BitArray.
};

#endif /*TRANSITIONBITARRAY_H_*/
