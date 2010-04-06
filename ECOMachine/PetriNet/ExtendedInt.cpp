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

#include "ExtendedInt.hpp"

/*!
 * \file ExtendedInt.cpp
 * \author Raphael Marczak, based on Antoine Allombert LISP code
 * \date 2008-2009
 */

using namespace std;

ExtendedInt::ExtendedInt()
:m_type(INTEGER), m_value(0)
{

}

ExtendedInt::ExtendedInt(short type, int value)
:m_type(type), m_value(value)
{

}

ExtendedInt::ExtendedInt(const ExtendedInt& other)
:m_type(other.m_type), m_value(other.m_value)
{

}

ExtendedInt::~ExtendedInt()
{

}

ExtendedInt& ExtendedInt::operator=(const ExtendedInt& other)
{
	if (this != &other) {
		m_type = other.m_type;
		m_value = other.m_value;
	}

	return *this;
}

void ExtendedInt::setAsInteger(int value)
{
	m_type = INTEGER;
	m_value = value;
}

void ExtendedInt::setAsPlusInfinity()
{
	m_type = PLUS_INFINITY;
}

void ExtendedInt::setAsMinusInfinity()
{
	m_type = MINUS_INFINITY;
}

short ExtendedInt::getType() const
{
	return m_type;
}

int ExtendedInt::getValue() const
{
	return m_value;
}

bool ExtendedInt::isPlusInfinity() const
{
	return (getType() == PLUS_INFINITY);
}

bool ExtendedInt::isMinusInfinity() const
{
	return (getType() == MINUS_INFINITY);
}

bool ExtendedInt::isInfinity() const
{
	return (isPlusInfinity() || isMinusInfinity());
}


bool ExtendedInt::isInteger() const
{
	return (getType() == INTEGER);
}

bool ExtendedInt::isZero() const
{
	return (isInteger() && (getValue() == 0));
}

ExtendedInt operator+(const ExtendedInt &a, const ExtendedInt &b)
{
//	assert(!(a.isPlusInfinity() && (b.isMinusInfinity())));
//	assert(!(a.isMinusInfinity() && (b.isPlusInfinity())));

	if (a.isPlusInfinity() && b.isMinusInfinity()) {
		throw ArithmeticException();
	}

	if (a.isMinusInfinity() && (b.isPlusInfinity())) {
		throw ArithmeticException();
	}

	ExtendedInt result;

	if (a.isPlusInfinity() || b.isPlusInfinity()) {
		result.setAsPlusInfinity();
	} else if (a.isMinusInfinity() || b.isMinusInfinity()) {
		result.setAsMinusInfinity();
	} else {
		result.setAsInteger(a.getValue() + b.getValue());
	}

	return result;
}

ExtendedInt operator+(const ExtendedInt &a, const int &b)
{
	ExtendedInt result(INTEGER, b);

	return operator+(a, result);
}

ExtendedInt operator+(const int &a, const ExtendedInt &b)
{
	ExtendedInt result(INTEGER, a);

	return operator+(result, b);
}

ExtendedInt operator-(const ExtendedInt &a, const ExtendedInt &b)
{
//	assert(!((a.isPlusInfinity()) && (b.isPlusInfinity())));
//	assert(!((a.isMinusInfinity()) && (b.isMinusInfinity())));

	if ((a.isPlusInfinity()) && (b.isPlusInfinity())) {
		throw ArithmeticException();
	}

	if ((a.isMinusInfinity()) && (b.isMinusInfinity())) {
		throw ArithmeticException();
	}

	ExtendedInt result;

	if (a.isPlusInfinity() || b.isMinusInfinity()) {
		result.setAsPlusInfinity();
	} else if (a.isMinusInfinity() || b.isPlusInfinity()) {
		result.setAsMinusInfinity();
	} else {
		result.setAsInteger(a.getValue() - b.getValue());
	}

	return result;

}

ExtendedInt operator-(const int &a, const ExtendedInt &b)
{
	ExtendedInt result(INTEGER, a);

	return operator-(result, b);
}

ExtendedInt operator-(const ExtendedInt &a, const int &b)
{
	ExtendedInt result(INTEGER, b);

	return operator-(a, result);
}

ExtendedInt operator*(const ExtendedInt &a, const ExtendedInt &b)
{
//	assert(!((a.isZero())
//		  && (b.isInfinity())));
//	assert(!((b.isZero())
//			  && (a.isInfinity())));

	if ((a.isZero()) && (b.isInfinity())) {
		throw ArithmeticException();
	}

	if ((b.isZero()) && (a.isInfinity())) {
		throw ArithmeticException();
	}

	ExtendedInt result;

	if (a.isPlusInfinity() && b.isMinusInfinity()) {
		result.setAsMinusInfinity();
	} else if (a.isPlusInfinity() && b.isPlusInfinity()) {
		result.setAsPlusInfinity();
	} else if (a.isMinusInfinity() && b.isPlusInfinity()) {
		result.setAsMinusInfinity();
	} else if (a.isMinusInfinity() && b.isMinusInfinity()) {
		result.setAsPlusInfinity();
	} else if (a.isInteger() && b.isPlusInfinity()) {
		if (a.m_value > 0) {
			result.setAsPlusInfinity();
		} else {
			result.setAsMinusInfinity();
		}
	} else if (a.isInteger() && b.isMinusInfinity()) {
		if (a.m_value < 0) {
			result.setAsPlusInfinity();
		} else {
			result.setAsMinusInfinity();
		}
	} else if (a.isPlusInfinity() && b.isInteger()) {
		if (b.m_value > 0) {
			result.setAsPlusInfinity();
		} else {
			result.setAsMinusInfinity();
		}
	} else if (a.isMinusInfinity() && b.isInteger()) {
		if (b.m_value < 0) {
			result.setAsPlusInfinity();
		} else {
			result.setAsMinusInfinity();
		}
	} else {
		result.setAsInteger(a.getValue() * b.getValue());
	}

	return result;
}

ExtendedInt operator*(const int &a, const ExtendedInt &b)
{
	ExtendedInt result(INTEGER, a);

	return operator*(result, b);
}

ExtendedInt operator*(const ExtendedInt &a, const int &b)
{
	ExtendedInt result(INTEGER, b);

	return operator*(a, result);
}


ExtendedInt operator/(const ExtendedInt &a, const ExtendedInt &b)
{
//	assert(!(a.isInfinity() && b.isInfinity()));
//	assert(!(b.isZero()));

	if (a.isInfinity() && b.isInfinity()) {
		throw ArithmeticException();
	}

	if (b.isZero()) {
		throw ArithmeticException();
	}

	ExtendedInt result;

	if (a.m_type == MINUS_INFINITY) {
		if (b.m_value > 0) {
			result.setAsMinusInfinity();
		} else {
			result.setAsPlusInfinity();
		}
	} else if (a.m_type == PLUS_INFINITY) {
		if (b.m_value < 0) {
			result.setAsMinusInfinity();
		} else {
			result.setAsPlusInfinity();
		}
	} else if (b.m_type == MINUS_INFINITY) {
		result.setAsInteger(0);
	} else if (b.m_type == PLUS_INFINITY) {
		result.setAsInteger(0);
	} else {
		result.setAsInteger(a.getValue()/b.getValue());
	}

	return result;
}

ExtendedInt operator/(const int &a, const ExtendedInt &b)
{
	ExtendedInt result(INTEGER, a);

	return operator/(result, b);
}

ExtendedInt operator/(const ExtendedInt &a, const int &b)
{
	ExtendedInt result(INTEGER, b);

	return operator/(a, result);
}

void ExtendedInt::operator+=(const ExtendedInt &a)
{
	*this = *this + a;
}

void ExtendedInt::operator+=(const int &a)
{
	*this = *this + a;
}

void ExtendedInt::operator-=(const ExtendedInt &a)
{
	*this = *this - a;
}

void ExtendedInt::operator-=(const int &a)
{
	*this = *this - a;
}

void ExtendedInt::operator*=(const ExtendedInt &a)
{
	*this = *this * a;
}

void ExtendedInt::operator*=(const int &a)
{
	*this = *this * a;
}

void ExtendedInt::operator/=(const ExtendedInt &a)
{
	*this = *this / a;
}

void ExtendedInt::operator/=(const int &a)
{
	*this = *this / a;
}


ExtendedInt& ExtendedInt::operator++()
{
	this->m_value++;
	return *this;
}

ExtendedInt ExtendedInt::operator++(int a)
{
	ExtendedInt tmpExtendedInt = *this;
	++(*this);
	return tmpExtendedInt;
}

ExtendedInt& ExtendedInt::operator--()
{
	this->m_value--;
	return *this;
}

ExtendedInt ExtendedInt::operator--(int a)
{
	ExtendedInt tmpExtendedInt = *this;
	--(*this);
	return tmpExtendedInt;
}

bool ExtendedInt::operator==(ExtendedInt a) const
{
	bool plusInfinityEquality = (this->isPlusInfinity() && a.isPlusInfinity());
	bool minusInfinityEquality = (this->isMinusInfinity() && a.isMinusInfinity());
	bool integerEquality = (this->isInteger() && a.isInteger())
	 					&& (this->getValue() == a.getValue());

	return (plusInfinityEquality || minusInfinityEquality || integerEquality);
}

bool ExtendedInt::operator!=(ExtendedInt a) const
{
	return !operator==(a);
}

bool ExtendedInt::operator<(ExtendedInt a) const
{
	bool isLowerThan;

	if (operator==(a)) {
		isLowerThan = false;
	} else if (this->isPlusInfinity()) {
		isLowerThan = false;
	} else if (a.isMinusInfinity()) {
		isLowerThan = false;
	} else if (this->isMinusInfinity()) {
		isLowerThan = true;
	} else if (a.isPlusInfinity()) {
		isLowerThan = true;
	} else if (this->getValue() < a.getValue()) {
		isLowerThan = true;
	} else {
		isLowerThan = false;
	}

	return (isLowerThan);
}

bool ExtendedInt::operator<=(ExtendedInt a) const
{
	return (!operator>(a));
}

bool ExtendedInt::operator>(ExtendedInt a) const
{
	return (!operator<(a) && !operator==(a));
}

bool ExtendedInt::operator>=(ExtendedInt a) const
{
	return (!operator<(a));
}

ostream& operator<<(std::ostream &os, const ExtendedInt &a)
{
	if (a.isPlusInfinity()) {
		os << "+infinity";
	} else if (a.isMinusInfinity()) {
		os << "-infinity";
	} else {
		os << a.getValue();
	}

	return os;
}




