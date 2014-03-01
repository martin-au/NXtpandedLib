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

/** \brief Label for NXT Display.
 *
 * A Label is a text field with a position. If you constructed the label with a starting position you do not have
 * to worry about positioning again. You may call now NLabel::setText or NLabel::setNumber to give the Label some data which
 * you want to monitor. NLabel::show(), NLabel::hide() will make the Label visible at the right time.
 *
 * Note that multiline labels are not supported, the end line char is disabled.
 * const NLabel means constant position, data but not constant visibility!
 */
class NLabel: public NWidget, private Uncopyable {

private:
	NString *label;
	bool somenew;
public:
	/** \brief Construct label object with positioning only.
	 *
	 * If charWidth is 0 the fieldWidth will be set to the size of the first passed string/number.
	 * Its recommenced to set charWidth explicit to get no unexpected results.
	 *
	 * @param indent Indent in chars from the left side of the display. (0 - 15)
	 * @param row    Row of the lcd (0 - 7)
	 * @param charWidth The width of the label in chars.
	 */
	explicit NLabel(S8 indent = 0, S8 row = 0, S8 charWidth = 5);

	/** \brief Construct label object with position and string.
	 *
	 * If charWidth is 0 the fieldWidth will be set to the size of the text.
	 * Its recommenced to set charWidth explicit to get no unexpected results.
	 *
	 * @param text Starting text string displayed in the label.
	 * @param indent Indent in chars from the left side of the display. (0 - 15)
	 * @param row    Row of the lcd (0 - 7)
	 * @param charWidth The width of the label in chars.
	 */
	explicit NLabel(const NString &text, S8 indent = 0, S8 row = 0, S8 charWidth = 0);

	/** Construct label object with position and C-String.
	 *
	 * If charWidth is 0 the fieldWidth will be set to the size of the text.
	 * Its recommenced to set charWidth explicit to get no unexpected results.
	 *
	 * @param text Starting text C-String displayed in the label.
	 * @param indent Indent in chars from the left side of the display. (0 - 15)
	 * @param row Row of the lcd (0 - 7)
	 * @param charWidth The width of the label in chars.
	 */
	explicit NLabel(const char *text, S8 indent = 0, S8 row = 0, S8 charWidth = 0);

	/** \brief Construct label object with position and number.
	 *
	 * If charWidth is 0 the fieldWidth will be set to the size of the number.
	 * Its recommenced to set charWidth explicit to get no unexpected results.
	 *
	 * @param num Starting number displayed in the label.
	 * @param indent Indent in chars from the left side of the display. (0 - 15)
	 * @param row Row of the lcd (0 - 7)
	 * @param numWidth The width of the label in chars.
	 */
	template<typename T>
	NLabel(const T num, const S8 indent, const S8 row, S8 numWidth = 5);

	/** \brief Destructor clears the label from display.
	 *
	 * It will not update the display.
	 */
	~NLabel();

	/** \brief Set precision for floating-point-numbers.
	 *
	 * @param places Number of places after floating-point to display.
	 */
	void setPrecision(S8 places) {
		label->setPrecision(static_cast<U16>(places));
	}

	/**
	 * \brief Get the actual text.
	 * @return Text of label.
	 */
	NString getText() const {
		return *label;
	}

	/** \brief Set a label text.
	 *
	 * @param text Text to set.
	 */
	void setText(const NString &text) {
		if(this->fieldWidth() == 0) this->fieldWidth(label->size());
		if (text.size() > 0) {
			*label = text;
			somenew = true;
		}
	}

	/** \brief Set a label text.
	 *
	 * @param text Text to set.
	 */
	void setText(const char *text) {
		if(this->fieldWidth() == 0) this->fieldWidth(label->size());
		if (text) {
			*label = text;
			somenew = true;
		}
	}

	/** \brief Set a label number.
	 *
	 * @param number Number to set.
	 */
	template<typename T>
	void setNumber(T number);

	/** \brief Set a label text.
	 *
	 * @param text Text to set.
	 */
	NLabel& operator=(const NString &text) {
		setText(text);
		return *this;
	}

	/** \brief Set a label text.
	 *
	 * @param text Text to set.
	 */
	NLabel& operator=(const char *text) {
		setText(text);
		return *this;
	}

	/** \brief Set a label number.
	 *
	 * @param number Number to set.
	 */
	template<typename T>
	NLabel& operator=(T number) {
		setNumber(number);
		return *this;
	}

	/** \brief Clear the label data.
	 */
	void clear() {
		label->clear();    // !const
		somenew = true;
		hide(false);
	}

	/** Set position of the label on lcd.
	 *
	 * @param indent Indent in chars from the left side of the display. (0 - 15)
	 * @param row Row of the lcd (0 - 7)
	 * @return True if the position is on the lcd. (is visible)
	 */
	bool setPosition(const S8 indent, const S8 row) {
		hide();

		setField(indent, row, 1, this->fieldWidth());
		//if (buddy)
		//	buddy->alignBuddy(buddyAlignment);
		somenew = true;
		return inLcd();
	}

	/** \brief Make the label visible.
	 *
	 * @param update If true update the display.
	 */
	virtual void show(bool update = false) const;

	/** \brief Make the label invisible.
	 *
	 * @param update If true update the display.
	 */
	virtual void hide(bool update = false) const;
};

// Must be declared here because of extern "C" problem

template<typename T>
NLabel::NLabel(const T num, const S8 indent, const S8 row, S8 numWidth)
 : label(new NString(numWidth)), somenew(true) {
	setField(indent, row, 1, numWidth);
	setNumber(num);
}

template<typename T>
void NLabel::setNumber(T number) {
	label->clear();
	label->append(number);
	// i think this is better you see the overflow and then optimize your program
	if (static_cast<S8>(label->size()) > fieldWidth()) {
		label->clear();
		for (S8 i = 0; i < this->fieldWidth(); ++i)
			label->append('#');
	}
	if(this->fieldWidth() == 0) this->fieldWidth(label->size());
	somenew = true;
}


}

#endif /* NLABEL_HPP_ */
