/*
 * NGuiObject.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NGUIOBJECT_HPP_
#define __NGUIOBJECT_HPP_

extern "C" {
#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h" // display_update
}

extern "C" int __cxa_pure_virtual() {return 0;}

namespace nxpl {

class NGuiObject {
private:
	mutable bool visible;
	virtual void showImpl() const = 0;
	virtual void hideImpl() const = 0;

protected:
	NGuiObject() : visible(false) {}
	virtual ~NGuiObject() {}

public:

	/** \brief Make the object visible.
	 *
	 * @param update If true update the display.
	 */
	void show(bool update = false) const {
		visible = true;
		if (update) {
			display_update();
		}
	}

	/** \brief Make the object invisible.
	 *
	 * @param update If true update the display.
	 */
	void hide(bool update = false) const {
		visible = false;
		if (update) {
			display_update();
		}
	}

	/** \brief Checks if object is visible.
	 *
	 * @return true if visible.
	 */
	bool isVisible() const {
		return visible;
	}
};

}


#endif /* __NGUIOBJECT_HPP_ */
