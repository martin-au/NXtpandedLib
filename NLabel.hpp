/*
 * NLabel.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NLABEL_HPP_
#define __NLABEL_HPP_

#include "NString.hpp"
#include "NWidget.hpp"
#include "Uncopyable.hpp"

namespace nxpl {

class NLabel: public NWidget, private Uncopyable {

private:
	NString *label;
	bool somenew;
	// mutable NLabel *buddy;
	// mutable NAlignment buddyAlignment;
public:
	/// if width = 0 then the width will be set to the len of the text or if there is no text to default 5!
	// multiline labels are not supported, use the console
	explicit NLabel(S8 indent = 0, S8 row = 0, S8 charWidth = 5);

	explicit NLabel(const NString &text, S8 indent = 0, S8 row = 0, S8 charWidth = 0);
	explicit NLabel(const char *text, S8 indent = 0, S8 row = 0, S8 charWidth = 0);

	template<typename T>
	NLabel(const T num, const S8 indent, const S8 row, S8 numWidth = 5);


	// buddy constructors
	/*
	explicit NLabel(NLabel * const buddyOf, S8 charWidth = 5);
	NLabel(const NString &text, NLabel * const buddyOf, S8 charWidth = 0);
	NLabel(const char *text, NLabel * const buddyOf, S8 charWidth = 0);
	*/

	~NLabel();

	void setPrecision(S8 places) {
		label->setPrecision(static_cast<U16>(places));
	}

	NString getText() const;

	void setText(const NString &text);
	void setText(const char *text);

	template<typename T>
	void setNumber(T number);

	NLabel& operator=(const NString &text) {
		setText(text);
		return *this;
	}
	NLabel& operator=(const char *text) {
		setText(text);
		return *this;
	}

	template<typename T>
	NLabel& operator=(T number) {
		setNumber(number);
		return *this;
	}

	void clear();

	bool setPosition(const S8 indent, const S8 row);
	// problem : what if we have a const label?
	void show(bool update = false) const;
	void hide(bool update = false) const;

	// const label may have non const buddys
	/*
	void setBuddy(NLabel * const myBuddy, const NAlignment &align = NAlignment::top()) const;
	void alignBuddy(const NAlignment &align) const;
	NLabel *getBuddy() const {
		return buddy;
	}
	*/
};

}

#include "NLabel.cpp"

#endif /* NLABEL_HPP_ */
