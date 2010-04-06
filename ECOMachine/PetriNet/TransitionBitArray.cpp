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

#include "TransitionBitArray.hpp"
#include <math.h>

/*!
 * \file BitArray.cpp
 * \author Raphael Marczak (LaBRI)
 * \date 2008-2009
 */

TransitionBitArray::TransitionBitArray(unsigned int size)
:m_size(size), m_maxIndexInBitArray((size - 1)/INT_SIZE)
{
	if (size == 0) {
		throw OutOfBoundException();
	}

	m_bitArray = new uint16_t[m_maxIndexInBitArray + 1];

	uint16_t allBitToOne = pow(2, INT_SIZE) - 1;

	for (unsigned int i = 0; i <= m_maxIndexInBitArray; ++i) {
		m_bitArray[i] = allBitToOne;
	}

	eraseArray();
}

void TransitionBitArray::setToOne(unsigned int index)
{
	if (index >= getSize()) {
		throw OutOfBoundException();
	}

	unsigned int indexInBitArray = index / INT_SIZE;
	unsigned int indexInInt = index % INT_SIZE;

	uint16_t bitToSetAsOne = 1 << indexInInt;

	m_bitArray[indexInBitArray] = m_bitArray[indexInBitArray] | bitToSetAsOne;
}

void TransitionBitArray::setToZero(unsigned int index)
{
	if (index >= getSize()) {
		throw OutOfBoundException();
	}

	unsigned int indexInBitArray = index / INT_SIZE;
	unsigned int indexInInt = index % INT_SIZE;

	uint16_t bitToSetAsZero = 1 << indexInInt;
	bitToSetAsZero = ~bitToSetAsZero;

	m_bitArray[indexInBitArray] = m_bitArray[indexInBitArray] & bitToSetAsZero;
}

void TransitionBitArray::eraseArray()
{
	for (unsigned int i = 0; i < getSize() ; ++i) {
		setToZero(i);
	}
}

bool TransitionBitArray::isAllOne()
{
	uint16_t allBitToOne = pow(2, INT_SIZE) - 1;

	for (unsigned int i = 0; i <= m_maxIndexInBitArray; ++i) {
		if (m_bitArray[i] != allBitToOne) {
			return false;
		}
	}

	return true;
}

unsigned int TransitionBitArray::getSize()
{
	return m_size;
}

TransitionBitArray::~TransitionBitArray()
{
	delete[] m_bitArray;
}
