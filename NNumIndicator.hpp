/*
 * NNumIndicator.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NNUMINDICATOR_HPP_
#define __NNUMINDICATOR_HPP_

#include "NLabel.hpp"

template<typename T = S32>
class NNumIndicator: public NLabel {
private:
	T num;
	S8 precision;
public:
	// jump to next line with \n is possible but multiline labels are not supported, use the console.
	NNumIndicator(const S8 indent = 0, const S8 row = 0, S8 numWidth = 5);
	NNumIndicator(const T num = 0, const S8 indent = 0, const S8 row = 0, S8 numWidth = 5);

	explicit NNumIndicator(NLabel * const buddyOf, S8 numWidth = 5);
	explicit NNumIndicator(const T num = 0, NLabel * const buddyOf, S8 numWidth = 5);

	~NNumIndicator();

	T getNumber() const;
	void setNumber(const T number);
	NNumIndicator& operator=(const T number) {
		setNumber(number);
		return *this;
	}

	void setPrecision(S8 places);
};


#include "NNumIndicator.cpp"


#endif /* NNUMINDICATOR_HPP_ */
