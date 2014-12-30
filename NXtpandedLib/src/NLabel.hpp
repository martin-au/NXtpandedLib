/*
 * NLabel_new.hpp
 *
 *  Created on: 21.03.2014
 *      Author: Martin
 */

#ifndef __NLABEL_HPP_
#define __NLABEL_HPP_

#include "NWidget.hpp"
#include "NString.hpp"
#include "Uncopyable.hpp"

#include "../../../GNUARM/arm-elf/include/string.h" // strlen

/** \file
 *	\ingroup NxtLcd
*/

/** \brief Label for NXT Display.
 *
 * A Label is a text field with a position. If you constructed the label with a starting position you do not have
 * to worry about positioning again. You may call now NLabel::setText or NLabel::setNumber to give the Label some data which
 * you want to monitor. NLabel::show(), NLabel::hide() will make the Label visible at the right time.
 *
 * Note that multiline labels are not supported, the end line char is disabled.
 * const NLabel means constant position and constant data but not constant visibility!
 */
class NLabel: public NWidget, private Uncopyable
{
private:
	NString *label;
	bool somenew;

	virtual void showWidgetImpl() const;
	virtual void hideWidgetImpl() const;

public:

	/** \brief Construct label object at zero point.
	 *
	 * Default width (max chars in line) is set to 1.
	 */
	NLabel::NLabel() :
			NWidget(NTextBox()), label(new NString(textBox().charsInLine())), somenew(true) {
		textBox().setLines(1);
	}

	/** \brief Construct label object with given textbox.
	 * TextBox chars in line should be set to a useful value. Multiline textbox is set back to 1.
	 *
	 * @param box TextBox with indent, line and width.
	 */
	NLabel::NLabel(NTextBox box) :
			NWidget(box), label(new NString(box.charsInLine())), somenew(true) {
		textBox().setLines(1);
	}

	/** \brief Construct label object with given textbox and initial string.
	 *
	 * TextBox chars in line should be set to a useful value. Multiline textbox is set back to 1.
	 *
	 * @param text Starting text string displayed in the label.
	 * @param box TextBox with indent, line and width.
	 */
	explicit NLabel(const NString &text, NTextBox box) :
			NWidget(box), label(new NString(box.charsInLine())), somenew(true) {
		textBox().setLines(1);
		setText(text);
	}

	/** \brief Construct label object with given textbox and initial c-string.
	 *
	 * TextBox chars in line should be set to a useful value. Multiline textbox is set back to 1.
	 *
	 * @param text Starting text string displayed in the label.
	 * @param box TextBox with indent, line and width.
	 */
	explicit NLabel(char *text, NTextBox box) :
			NWidget(box), label(new NString(box.charsInLine())), somenew(true) {
		textBox().setLines(1);
		setText(text);
	}

	/** \brief Construct label object with given textbox and initial number.
	 *
	 * TextBox chars in line should be set to a useful value. Multiline textbox is set back to 1.
	 *
	 * @param text Starting number displayed in the label.
	 * @param box TextBox with indent, line and width.
	 */
	template<typename T>
	NLabel(const T num, NTextBox box) :
			NWidget(box), label(new NString(box.charsInLine())), somenew(true) {
		textBox().setLines(1);
		setNumber(num);
	}

	/** \brief Destructor clears the label from display.
	 *
	 * It will not update the display.
	 */
	virtual ~NLabel() {
		this->clear(); // TODO do not call virtual functions in destructors?
		delete label;
	}

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
	 * Text will be cut if its to long for the label
	 * @param text Text to set.
	 */
	void setText(const NString &text) {
		if (text.size() > textBox().charsInLine()) {
			*label = text.substr(0, textBox().charsInLine());
		} else {
			*label = text;
		}
		somenew = true;
	}

	/** \brief Set a label text.
	 *
	 * @param text Text to set.
	 */
	void setText(char *text) {
		if (text) {
			somenew = true;
			if (static_cast<U8>(strlen(text)) > textBox().charsInLine()) {
				text[textBox().charsInLine()] = '\0';
			}
			*label = text;
		}
	}

	/** \brief Set a label number.
	 *
	 * In case of overflow the label will display '#'.
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
	NLabel& operator=(char *text) {
		setText(text);
		return *this;
	}

	/** \brief Set a label number.
	 *
	 * In case of overflow the label will display '#'.
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
};

// Must be declared here because of extern "C" problem

template<typename T>
void NLabel::setNumber(T number) {
	label->clear();
	label->append(number);
	// i think this is better, you see the overflow and then optimize your program
	if (static_cast<S8>(label->size()) > textBox().charsInLine()) {
		label->clear();
		for (S8 i = 0; i < textBox().charsInLine(); ++i)
			label->append('#');
	}
	somenew = true;
}

#endif /* __NLABEL_HPP_ */
