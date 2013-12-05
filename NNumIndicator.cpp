/*
 * NNumIndicator.cpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */


#include "NNumIndicator.hpp"

// consturcotren von NLabel verwenden!!!

template<typename T>
NNumIndicator<T>::NNumIndicator(const S8 indent, const S8 row, S8 numWidth) :
	NLabel(indent, row, numWidth),  precision(2) {
	setField(indent, row, 1, numWidth);
	label = new NString(numWidth);
}

template<typename T>
NNumIndicator<T>::NNumIndicator(const T num, const S8 indent, const S8 row, S8 numWidth) :
	NLabel(indent, row, numWidth),	precision(2) {
	setNumber(num);
}

template<typename T>
NNumIndicator<T>::NNumIndicator(NLabel * const buddyOf, S8 numWidth) :
	NLabel(buddyOf, numWidth),	precision(2) {
}

template<typename T>
NNumIndicator<T>::NNumIndicator(const T num, NLabel * const buddyOf, S8 numWidth) :
	NLabel(buddyOf, numWidth),	precision(2) {
	setNumber(num);
}


template<typename T>
NNumIndicator<T>::~NNumIndicator() {
}

template<typename T>
T NNumIndicator<T>::getNumber() const {
	return num;
}

template<typename T>
void NNumIndicator<T>::setNumber(const T number) {
	num = number;
	label->clear();
	label->append(num);
	// not optimized but good indicator if somethings wrong!
	if (static_cast<S8>(label->size()) > this->fieldWidth()) {
		label->clear();
		for (S8 i = 0; i < this->fieldWidth(); ++i)
			label->append('#');
	}
}

template<typename T>
void NNumIndicator<T>::setPrecision(S8 places) {
	precision = places;
	label->setPrecision(static_cast<U16>(places));
}
