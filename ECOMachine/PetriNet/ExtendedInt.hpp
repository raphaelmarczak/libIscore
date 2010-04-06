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

#ifndef EXTENDEDINT_H_
#define EXTENDEDINT_H_

/*!
 * \file ExtendedInt.hpp
 * \author Raphael Marczak, based on Antoine Allombert LISP code
 * \date 2008-2009
 */


#include <iostream>
#include "ArithmeticException.hpp"

#define PLUS_INFINITY 1
#define INTEGER 0
#define MINUS_INFINITY -1

/*!
 * \class ExtendedInt
 *
 * ExtendedInt allows arithmetics operations using +infinity and -infinity.
 *
 * To create a ExtendedInt, type (PLUS_INFINITY, INTEGER, MINUS_INFINITY) must be provided
 * and a value if INTEGER (useless if PLUS_INFINITY or MINUS_INFINITY).
 *
 * Default constructor will create a 0 INTEGER.
 */
class ExtendedInt
{
public:
	// Constructors.

	/*!
	 * Default constructor.
	 *
	 * Creates an ExtendedInt as an INTEGER and with 0 value.
	 */
	ExtendedInt();

	/*!
	 * Constructor
	 *
	 * Creates an ExtendedInt using the given type and value.
	 *
	 * \param type : ExtendedInt type [PLUS_INFINITY/INTEGER/MINUS_INFINITY].
	 * \param value : ExtendedInt value if INTEGER (useless otherwise).
	 */
	ExtendedInt(short type, int value = 0);

	/*!
	 * Copy constructor
	 *
	 * Creates an ExtendedInt copying the given ExtendedInt.
	 *
	 * \param other : ExtendedInt to copy.
	 */
	ExtendedInt(const ExtendedInt& other);

	// Destructor.

	/*!
	 * Destructor
	 *
	 * Does nothing in this class.
	 */
	~ExtendedInt();

	/*!
	 * Assignment operator.
	 *
	 * Creates an ExtendedInt copying the given ExtendedInt.
	 * Allows the use of ExtendedInt = AnotherExtendedInt.
	 *
	 * \param other : ExtendedInt to assign.
	 * \return
	 */
	ExtendedInt& operator=(const ExtendedInt& other);


	// Getters and setters.

	/*!
	 * Sets the ExtendedInt to be an INTEGER, and sets the value at the given one.
	 *
	 * \param value : value to give at this ExtendedInt.
	 */
	void setAsInteger(int value);

	/*!
	 * Sets the ExtendedInt to be a PLUS_INFINITY.
	 * Does not modify the value.
	 */
	void setAsPlusInfinity();

	/*!
	 * Sets the ExtendedInt to be a MINUS_INFINITY.
	 * Does not modify the value.
	 */
	void setAsMinusInfinity();

	/*!
	 * \return the ExtendedInt type [PLUS_INFINITY/INTEGER/MINUS_INFINITY].
	 */
	short getType() const;

	/*!
	 * \return the ExtendedInt value, even if it is not an INTEGER (so the type must be checked before).
	 */
	int getValue() const;


	// Questions about type and value.

	/*!
	 * \return true if the ExtendedInt is a PLUS_INFINITY.
	 */
	bool isPlusInfinity() const;

	/*!
	 * \return returns true if the ExtendedInt is a MINUS_INFINITY.
	 */
	bool isMinusInfinity() const;

	/*!
	 * \return true if the ExtendedInt is a MINUS_INFINITY or PLUS_INFINITY.
	 */
	bool isInfinity() const;

	/*!
	 * \return true if the ExtendedInt is an INTEGER.
	 */
	bool isInteger() const;

	/*!
	 * \return true if the ExtendedInt is an INTEGER and its value is 0.
	 */
	bool isZero() const;


	// Arithmetics operators.

	/*!
	 * Addition operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt + ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * +infinity + -infinity
	 * -infinity + +infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator+(const ExtendedInt &a, const ExtendedInt &b);

	/*!
	 * Addition operator.
	 *
	 * Allows the use of ExtendedInt = int + ExtendedInt
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator+(const int &a, const ExtendedInt &b);

	/*!
	 * Addition operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt + int
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator+(const ExtendedInt &a, const int &b);

	/*!
	 * Substraction operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt - ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * +infinity - +infinity
	 * -infinity - -infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator-(const ExtendedInt &a, const ExtendedInt &b);

	/*!
	 * Substraction operator.
	 *
	 * Allows the use of ExtendedInt = int - ExtendedInt
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator-(const int &a, const ExtendedInt &b);

	/*!
	 * Substraction operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt - int
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator-(const ExtendedInt &a, const int &b);

	/*!
	 * Multiplication operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt * ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * 0 * +infinity
	 * 0 * -infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator*(const ExtendedInt &a, const ExtendedInt &b);

	/*!
	 * Multiplication operator.
	 *
	 * Allows the use of ExtendedInt = int * ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * 0 * +infinity
	 * 0 * -infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator*(const int &a, const ExtendedInt &b);

	/*!
	 * Multiplication operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt * int
	 *
	 * Throws a ArithmeticException in these cases :
	 * 0 * +infinity
	 * 0 * -infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator*(const ExtendedInt &a, const int &b);

	/*!
	 * Division operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt / ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * x / 0
	 * +infinity / 0
	 * -infinity / 0
	 * +infinity / -infinity
	 * +infinity / +infinity
	 * -infinity / -infinity
	 * -infinity / +infinity
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator/(const ExtendedInt &a, const ExtendedInt &b);

	/*!
	 * Division operator.
	 *
	 * Allows the use of ExtendedInt = int / ExtendedInt
	 *
	 * Throws a ArithmeticException in these cases :
	 * x / 0
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator/(const int &a, const ExtendedInt &b);

	/*!
	 * Division operator.
	 *
	 * Allows the use of ExtendedInt = ExtendedInt / int
	 *
	 * Throws a ArithmeticException in these cases :
	 * x / 0
	 * +infinity / 0
	 * -infinity / 0
	 *
	 * \param a
	 * \param b
	 */
	friend ExtendedInt operator/(const ExtendedInt &a, const int &b);

	/*!
	 * Addition operator.
	 *
	 * Allows the use of ExtendedInt += ExtendedInt
	 * Cf operator+(const ExtendedInt &a, const ExtendedInt &b)
	 *
	 * \param a
	 */
	void operator+=(const ExtendedInt &a);

	/*!
	 * Addition operator.
	 *
	 * Allows the use of ExtendedInt += int
	 * Cf operator+(const ExtendedInt &a, const int &b)
	 *
	 * \param a
	 */
	void operator+=(const int &a);

	/*!
	 * Substraction operator.
	 *
	 * Allows the use of ExtendedInt -= ExtendedInt
	 * Cf operator-(const ExtendedInt &a, const ExtendedInt &b)
	 *
	 * \param a
	 */
	void operator-=(const ExtendedInt &a);

	/*!
	 * Substraction operator.
	 *
	 * Allows the use of ExtendedInt -= int
	 * Cf operator-(const ExtendedInt &a, const int &b)
	 *
	 * \param a
	 */
	void operator-=(const int &a);

	/*!
	 * Multiplication operator.
	 *
	 * Allows the use of ExtendedInt *= ExtendedInt
	 * Cf operator*(const ExtendedInt &a, const ExtendedInt &b)
	 *
	 * \param a
	 */
	void operator*=(const ExtendedInt &a);

	/*!
	 * Multiplication operator.
	 *
	 * Allows the use of ExtendedInt *= int
	 * Cf operator*(const ExtendedInt &a, const int &b)
	 *
	 * \param a
	 */
	void operator*=(const int &a);

	/*!
	 * Division operator.
	 *
	 * Allows the use of ExtendedInt /= ExtendedInt
	 * Cf operator/(const ExtendedInt &a, const ExtendedInt &b)
	 *
	 * \param a
	 */
	void operator/=(const ExtendedInt &a);

	/*!
	 * Division operator.
	 *
	 * Allows the use of ExtendedInt /= int
	 * Cf operator/(const ExtendedInt &a, const int &b)
	 *
	 * \param a
	 */
	void operator/=(const int &a);

	/*!
	 * Increment operator (prefix)
	 *
	 * Adds 1 to the ExtendedInt value.
	 * Allows the use of ++ExtendedInt.
	 *
	 */
	ExtendedInt& operator++();

	/*!
	 * Increment operator (postfix)
	 *
	 * Adds 1 to the ExtendedInt value.
	 * Allows the use of ExtendedInt++.
	 *
	 * \param a : useless parameter (it stands for differentiate postfix and prefix).
	 */
	ExtendedInt operator++(int a);

	/*!
	 * Decrement operator (prefix)
	 *
	 * Substracts 1 to the ExtendedInt value.
	 * Allows the use of --ExtendedInt.
	 *
	 */
	ExtendedInt& operator--();    // prefix--.

	/*!
	 * Decrement operator (prefix)
	 *
	 * Substracts 1 to the ExtendedInt value.
	 * Allows the use of ExtendedInt--.
	 *
	 * \param a : useless parameter (it stands for differentiate postfix and prefix).
	 */
	ExtendedInt operator--(int a); // postfix--.


	// Comparison operators.

	/*!
	 * Equality operator.
	 *
	 * Allows the use of ExtendedInt == ExtendedInt
	 *
	 * \param a
	 * \return true if both types are the same, and both values are the same in the INTEGER case.
	 *
	 */
	bool operator==(ExtendedInt a) const;

	/*!
	 * Inequality operator.
	 *
	 * Allows the use of ExtendedInt != ExtendedInt
	 *
	 * \param a
	 * \return true if both types are the different, or both values are different in the INTEGER case.
	 */
	bool operator!=(ExtendedInt a) const;

	/*!
	 * Lower operator.
	 *
	 * Allows the use of ExtendedInt < ExtendedInt
	 *
	 * \param a
	 * \return true in these cases :
	 * x < +infinity
	 * -infinity < x
	 * -infinity < +infinity
	 * x < y if x.getValue() < y.getValue()
	 */
	bool operator<(ExtendedInt a) const;

	/*!
	 * Lower or equal operator.
	 *
	 * Allows the use of ExtendedInt <= ExtendedInt
	 *
	 * \param a
	 * \return true in these case :
	 * x <= +infinity
	 * -infinity <= x
	 * -infinity <= -infinity
	 * -infinity <= +infinity
	 * +infinity <= +infinity
	 * x <= y if x.getValue() <= y.getValue()
	 */
	bool operator<=(ExtendedInt a) const;

	/*!
	 * Greater operator.
	 *
	 * Allows the use of ExtendedInt > ExtendedInt
	 *
	 * \param a
	 * \return true in these case :
	 * x > -infinity
	 * +infinity > x
	 * +infinity > -infinity
	 * x > y if x.getValue() > y.getValue()
	 */
	bool operator>(ExtendedInt a) const;

	/*!
	 * Greater or equal operator.
	 *
	 * Allows the use of ExtendedInt >= ExtendedInt
	 *
	 * \param a
	 * \return true in these case :
	 * x >= -infinity
	 * +infinity >= x
	 * -infinity >= -infinity
	 * +infinity >= -infinity
	 * +infinity >= +infinity
	 * x >= y if x.getValue() >= y.getValue()
	 */
	bool operator>=(ExtendedInt a) const;

	// Ostream operator.

	friend std::ostream & operator<<(std::ostream &os, const ExtendedInt &a);

private:
	short m_type; // ExtendedInt type : MINUS_INFINITY, PLUS_INFINITY or INTEGER.
	int m_value; // ExtendedInt value if type is INTEGER (useless otherwise).
};

#endif /*EXTENDEDINT_H_*/
