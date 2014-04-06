/*
 * NShape_new.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NSHAPE_NEW_HPP_
#define __NSHAPE_NEW_HPP_

#include "NGuiObject.hpp"
#include "NLcd.hpp"

/**
 * \file
 * \ingroup NxtLcd
*/

namespace nxpl {

/**
 * \brief Baseclass for all shapes.
 *
 * The class bundles together the NLcd class which gives access to actual draw pixels on the lcd.
 * For example it looks before every call to showImpl if the NLcd object is ok and if the object is in lcd.
 * This makes all derived classes saver because we check before the actual call to the function if everything is ok.
 */
class NShape : public NGuiObject {
protected:
	// at the moment protected for my comfort
	mutable NLcd *lcd;

private:
	virtual void showShapeImpl() const = 0;
	virtual void hideShapeImpl() const = 0;
	virtual void invertShapeImpl() const = 0;

	virtual void showImpl() const {
		if(lcd == 0) return;
		showShapeImpl();
	}

	virtual void hideImpl() const {
		if(lcd == 0) return;
		hideShapeImpl();
	}

	virtual void invertImpl() const {
		if(lcd == 0) return;
		invertShapeImpl();
		setVisibility(true);
	}
public:

	NShape(NLcd &nlcd) : lcd(&nlcd) {}
	NShape() : lcd(0) {}
	virtual ~NShape() {}

	bool setLcd(NLcd &nlcd) const {
		lcd = &nlcd;
		if(lcd->noError()) {
			return true;
		} else {
			lcd = 0;
			return false;
		}
	}
};

}

#endif /* __NSHAPE_NEW_HPP_ */
