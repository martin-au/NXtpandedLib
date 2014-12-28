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

/** \brief Draw directly on nxt lcd.
 */
class NLcd : public NGenericPixelMatrix {

public:

	/** \brief Use standard lcd.
	 * Constructs a lcd object which directly draws on the nxt lcd.
	 */
	NLcd() : NGenericPixelMatrix(display_get_buffer(), LCD::WIDTH, LCD::ROWS) {
	}

	/** \brief Update display (hardware)*/
	static void update() {
		display_update();
	}
};

}

#endif
