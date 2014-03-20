/*
 * NShape.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */

#ifndef __NSHAPE_HPP_
#define __NSHAPE_HPP_

/** \file
 *	\ingroup NxtLcd
*/

namespace nxpl {

/**
 * \brief Baseclass for all shapes.
 *
 * The class bundles together the NWidget class which is for positioning show/hide and
 * the NLcd class which gives access to actual draw pixels on the lcd.
 * For example it looks before every call to showImpl if the NLcd object is ok and if the object is in lcd.
 * This makes all derived classes saver because we check before the actual call to the function if everything is ok.
 */
class NShape : public NWidget {
protected:
	// at the moment protected for my comfort
	mutable NLcd *lcd;
public:

	NShape(NLcd *nlcd) : lcd(nlcd) {}
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

	virtual void show(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		showImpl(update);
		setVisibility(true);
		if (update) {
			display_update();
		}
	}

	void hide(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		eraseImpl(update);
		setVisibility(false);
		if (update) {
			display_update();
		}
	}

	void invert(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		invertImpl(update);
		setVisibility(!isVisible());
		if (update) {
			display_update();
		}
	}

private:
	virtual void showImpl(bool update) const = 0;
	virtual void eraseImpl(bool update) const = 0;
	virtual void invertImpl(bool update) const = 0;
};

}

#endif /* __NSHAPE_HPP_ */
