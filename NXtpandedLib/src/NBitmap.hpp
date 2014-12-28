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

/** \brief Draw on bitmap.
**/
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
