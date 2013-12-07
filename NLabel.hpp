/*
 * NLabel.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NLABEL_HPP_
#define __NLABEL_HPP_

/*
 TODO Test copy construcotrs!
 TODO Align buddy and const?
 TODO Take the buddy system out in own class?
 */

#include "NString.hpp"

#include "NWidget.hpp"


class NLabel: public NWidget {
protected:
	NString *label;

	mutable NLabel *buddy;
	mutable NAlignment buddyAlignment;
public:
	/// if width = 0 then the width will be set to the len of the text or if there is no text to default 5!
	// jump to next line with \n is possible but multiline labels are not supported, use the console
	NLabel(const S8 indent = 0, const S8 row = 0, const S8 charWidth = 5);
	NLabel(const NString &text, const S8 indent = 0, const S8 row = 0, const S8 charWidth = 0);
	NLabel(const char *text, const S8 indent = 0, const S8 row = 0, const S8 charWidth = 0);

	// buddy constructors
	explicit NLabel(NLabel * const buddyOf, S8 charWidth = 5);
	NLabel(const NString &text, NLabel * const buddyOf, S8 charWidth = 0);
	NLabel(const char *text, NLabel * const buddyOf, S8 charWidth = 0);


	~NLabel();

	NString getText() const;
	void setText(const NString &text);
	void setText(const char *text);
	NLabel& operator=(const NString &text) {
		setText(text);
		return *this;
	}
	NLabel& operator=(const char *text) {
		setText(text);
		return *this;
	}

	void clear();

	bool setPosition(const S8 indent, const S8 row);
	// problem : what if we have a const label?
	void show(bool update = false) const;
	void erase(bool update = false) const;

	// const label may have non const buddys
	void setBuddy(NLabel * const myBuddy, const NAlignment &align = NAlignment::top()) const;
	void alignBuddy(const NAlignment &align) const;
	NLabel *getBuddy() const {
		return buddy;
	}
};




#include "NLabel.cpp"

#endif /* NLABEL_HPP_ */
