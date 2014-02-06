/*
 * NNumIndicator.cpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */


#include "NNumIndicator.hpp"

// consturcotren von NLabel verwenden!!!


NNumIndicator::NNumIndicator(S8 indent, S8 row, S8 numWidth) :
	NLabel(indent, row, numWidth), num(0), precision(2) {

}


NNumIndicator::NNumIndicator(NLabel * const buddyOf, S8 numWidth) :
	NLabel(buddyOf, numWidth), num(0),	precision(2) {
}


NNumIndicator::~NNumIndicator() {
}

float NNumIndicator::getNumber() const {
	return num;
}


template<typename T>
void NNumIndicator::setNumber(T number) {
	num = static_cast<float>(number);
}

void NNumIndicator::show(bool update) const {
	label->clear();
	label->append(num);
	// not optimized but good indicator if somethings wrong!
	if (static_cast<S8>(label->size()) > fieldWidth()) {
		label->clear();
		for (S8 i = 0; i < this->fieldWidth(); ++i)
			label->append('#');
	}
	NLabel::show(update);
}


void NNumIndicator::setPrecision(S8 places) {
	precision = places;
	label->setPrecision(static_cast<U16>(places));
}
