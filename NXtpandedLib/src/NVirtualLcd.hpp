/*
 * NVirtualLcd.hpp
 *
 *  Created on: 16.08.2014
 *      Author: Martin
 */

#ifndef NVIRTUALLCD_HPP_
#define NVIRTUALLCD_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "Uncopyable.hpp"
#include "NGenericPixelMatrix.hpp"

namespace nxpl {

/** \brief Draw on virtual not visible lcd.
**/
class NVirtualLcd : public NGenericPixelMatrix {

public:

	/** \brief Construct a virtual lcd
	 *
	 * You may copy it at any time to the real lcd with NLcd::overlayOtherPixelMatrix
	 */
	NVirtualLcd() :
		NGenericPixelMatrix(new U8[LCD::WIDTH * LCD::ROWS], LCD::WIDTH, LCD::ROWS) {
	}

	~NVirtualLcd() {
		deleteMatrix();
	}

	// TODO Implement resize,.. methods ...
};



#endif /* NVIRTUALLCD_HPP_ */
