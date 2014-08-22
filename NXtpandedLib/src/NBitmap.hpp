/*
 * NLcd.hpp
 *
 *  Created on: 17.11.2013
 *      Author: Martin
 */

#ifndef __NBITMAP_HPP_
#define __NBITMAP_HPP_

/** \file
 *	\ingroup NxtLcd
*/

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
class NBitmap : public NGenericPixelMatrix {

public:

	/** \brief Construct a bitmap
	 *
	 * You can then copy the bitmap to the lcd or to another bitmap,
	 */
	NBitmap(U8 width, U8 rows) :
		NGenericPixelMatrix(new U8[width * rows], width, rows) {
	}

	~NBitmap() {
		deleteMatrix();
	}

	// TODO Implement resize,.. methods ...
};
