/*
 * NLcd.hpp
 *
 *  Created on: 17.11.2013
 *      Author: Martin
 */

#ifndef __NLCD_HPP_
#define __NLCD_HPP_

/** \file
 *	\ingroup NxtLcd
*/

extern "C" {
#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h"
}

#include "Uncopyable.hpp"
#include "NGenericPixelMatrix.hpp"


namespace nxpl {

/** \brief Provides low level pixel functions.
 *
 * NLcd is a very low level class
 * It is designed to direct access pixels of the lcd
 * Its high efficient so there is no range checking!!
 * If you write for example on pixel x 101 y 65 anything could happen!
 */
class NLcd : public NGenericPixelMatrix {

public:

	/** \brief Use standard lcd.
	 * Constructs a lcd object which directly draws on the nxt lcd.
	 */
	NLcd() : NGenericPixelMatrix(display_get_buffer(), LCD::WIDTH, LCD::ROWS) {
	}

	static void update() {
		display_update();
	}
};

}

#endif
