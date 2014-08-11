/*
 * NWidget_new.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NWIDGET_HPP_
#define __NWIDGET_HPP_

#include "NGuiObject.hpp"
#include "NTextBox.hpp"
#include "LcdConstants.hpp"

/** \file
 *	\ingroup NxtLcd
*/

namespace nxpl {

// NWidget is designed as a base class which provides space managing functions
// Be aware: All coordinates are converted into pixel coordinates (100x64)
// but never in other direction! If you set for example x, height then the functions indent, lines will return 0!
// You can convert Text-Coordinates into pixel coordinates with data loss: see LcdConstants.hpp
/** \brief Base class for lcd-widgets.
 *
 * The class holds the position of the widget and allows to manage the space of the lcd.
 * Every derived class must set the field with NWidget::setField.
 * Be aware: All coordinates are converted into pixel coordinates (100x64) but never in other direction!
 * If you set for example height in pixels then the function lines will return 0
 * You can convert Text-Coordinates into pixel coordinates with data loss: see LcdConstants.hpp
 * <br>
 * const NWidget means constant position but not constant visibility.
 * <br>
 * It also specifies the two pure virtual functions NWidget::show() and NWidget::hide() which every derived class must implement.
 */
class NWidget : public NGuiObject {
private:
	NTextBox box;

	virtual void showImpl() const {
		showWidgetImpl();
	}
	virtual void hideImpl() const {
		hideWidgetImpl();
	}
	virtual void showWidgetImpl() const = 0;
	virtual void hideWidgetImpl() const = 0;

protected:
	NWidget(NTextBox textBox) : box(textBox) {}

	virtual ~NWidget() {}

	virtual void textBoxChangedHandler() {}
public:
	NTextBox textBox() const {
		return box;
	}

	void setTextBox(NTextBox textBox) {
		bool visible = this->isVisible();
		if(visible) {
			hide(false); // Really important, otherwise we do not know where to clean!
		}
		box = textBox;
		textBoxChangedHandler();
		if(visible) {
			show(false);
		}
	}

	// top-level functions for easy use

	void setPosition(NCursor position) {
		bool visible = this->isVisible();
		if(visible) {
			this->hide(false); // Really important, otherwise we do not know where to clean!
		}
		box.setBase(position);
		textBoxChangedHandler();
		if(visible) {
			this->show(false);
		}
	}

	void setPosition(S8 indent, S8 line) {
		setPosition(NCursor(indent, line));
	}

	void setIndent(S8 indent) {
		setPosition(NCursor(indent, box.base().line()));
	}

	void setLine(S8 line) {
		setPosition(NCursor(box.base().indent(), line));
	}
};

}

#endif /* NWIDGET_HPP_ */
