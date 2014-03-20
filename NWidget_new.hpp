/*
 * NWidget_new.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NWIDGET_NEW_HPP_
#define __NWIDGET_NEW_HPP_

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

	NWidget(NTextBox textBox) : box(textBox) {}
	virtual ~NWidget() {}

public:
	NTextBox textBox() const {
		return NTextBox;
	}

	NTextBox & textBox() {
		return &NTextBox;
	}

	void setTextBox(NTextBox textBox) {
		box = textBox;
	}
};

}

#endif /* NWIDGET_NEW_HPP_ */
