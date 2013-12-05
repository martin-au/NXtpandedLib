/*
 * NShape.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */

#ifndef NSHAPE_HPP_
#define NSHAPE_HPP_


// make pure virtual function work with nxtOSEK
extern "C" int __cxa_pure_virtual(){return 0;}


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

	void show(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		showImpl(update);
		setVisibility(true);
		if (update) {
			display_update();
		}
	}

	void erase(bool update = false) const {
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

#endif /* NSHAPE_HPP_ */
