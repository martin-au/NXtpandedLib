/*
 * NNumIndicator.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NNUMINDICATOR_HPP_
#define __NNUMINDICATOR_HPP_

/*
#include "NLabel.hpp"

// deleted template class implementation because code bloat is extreme!

// TODO Test efficiency for floats to string, second impl with S32/U32??


class NNumIndicator: public NLabel {
private:
	// float datatype is a good compromise for template template impl
	float num;
	S8 precision;
public:
	// jump to next line with \n is possible but multiline labels are not supported, use the console.
	NNumIndicator(S8 indent, S8 row, S8 numWidth = 5);
	//NNumIndicator(const T num, const S8 indent, const S8 row, S8 numWidth = 5);

	explicit NNumIndicator(NLabel * const buddyOf, S8 numWidth = 5);
	//explicit NNumIndicator(const T num = 0, NLabel * const buddyOf, S8 numWidth = 5);

	~NNumIndicator();

	float getNumber() const;

	template<typename T>
	void setNumber(T number);

	NNumIndicator& operator=(const float number) {
		setNumber(number);
		return *this;
	}
	void show(bool update = false) const;
	void setPrecision(S8 places);
};


#include "NNumIndicator.cpp"
*/

#endif /* NNUMINDICATOR_HPP_ */
