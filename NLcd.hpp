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
#include "LcdConstants.hpp"
#include "NPoint.hpp"


namespace nxpl {

struct PixelOn;
struct PixelOff;
struct PixelInvert;

/** \brief Provides low level pixel functions.
 *
 * NLcd is a very low level class
 * It is designed to direct access pixels of the lcd
 * Its high efficient so there is no range checking!!
 * If you write for example on pixel x 101 y 65 anything could happen!
 */
class NLcd : private Uncopyable {
private:
	U8 *disp;
public:

	/** \brief Use standard lcd.
	 * Constructs a lcd object which directly draws on the nxt lcd.
	 */
	NLcd() :
			disp(display_get_buffer()) {
	}

	/** \brief Bitmap constructor
	 * Constructs an lcd object which draws on a bitmap with size width * depth.
	 * @param width Width of the bitmap in pixels.
	 * @param depth Depth of the bitmap in rows.
	 */
	NLcd(U32 width, U32 depth) :
			disp(new U8[width * depth]) {
	}

	/**
	 * \brief Deletes bitmap if you used bitmap constructor
	 */
	~NLcd() {
		if (disp != display_get_buffer()) {
			delete[] disp;
		}
	}

	/** \brief Checks if there is an error.
	 *
	 * @return true if no error.
	 */
	bool noError() {
		return (disp != 0);
	}

	/** \brief Set a pixel mask of size LCD::DEPTH.
	 *
	 * This sounds complicated but let me explain: <br>
	 * The display array looks like this.  <br>
	 * |||| < line 0 <br>
	 * |||| < line 1 <br>
	 * 0123 < x <br>
	 * The mask is represented by | <br>
	 * If we look closer to the mask we see that each mask consists of 8 pixels (LCD::DEPTH)<br>
	 * The 8 pixels are represented by a unsigned 8 bit number<br>
	 * So if the number is 1 the pixel mask looks like this: 0000 0001 first pixel ist set<br>
	 * If the number is 20 the pixel mask looks like this: 0001 0100<br>
	 *
	 * \note This is the fastest way to update a set of pixels.
	 *
	 * @param X   The X-Coordinate from the left.
	 * @param line The line from the top.
	 * @param mask Pixelmask.
	 */
	inline void pixelMask(U8 X, U8 line, U8 mask) {
		*(disp + (line * LCD::WIDTH + X)) = mask; // overlay! change to xor for no overlay?
	}

	/** \brief Copy bitmap array into bitmap handled by NLcd or directly to nxt lcd.
	 *
	 * @param data Bitmap array
	 * @param width Width of bitmap
	 * @param depth Depth of bitmap
	 * @param x Offset-X-Coordinate in destination
	 * @param y Offset-Y-Coordinate in destination
	 */
	void copyBitmap(const U8 *data, U32 width, U32 depth, U32 x, U32 y);

	/** \brief Copy data handled by other NLcd into this NLcd.
	 *
	 * @param lcdBitmap Source to copy.
	 * @param width Width of source bitmap.
	 * @param depth Depth of source bitmap.
	 * @param x Offset-X-Coordinate in destination
	 * @param y Offset-Y-Coordinate in destination
	 */
	void copyNLcdBitmap(const NLcd &lcdBitmap, U32 width, U32 depth, U32 x, U32 y) {
		copyBitmap(lcdBitmap.disp, width, depth, x, y);
	}

	// disp = 0100 0000
	//        OR
	// set  = 0000 1000
	// res  = 0100 1000

	/**\brief Set pixel on.
	 * Make pixel visible.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void pixelOn(U8 X, U8 Y) {
		/* Test for pixel write fails!
		 if(!pixelInLcd(X, Y)) {
		 cout << "fail" << endl;
		 timer.wait(2000);
		 return;
		 }
		 */
		//*(disp + (LCD::pixelToLine(Y) * LCD::WIDTH + X)) |= (1 << (Y % LCD::DEPTH));
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) |=
				(1 << (Y % LCD::DEPTH));
	}

	/**\brief Set pixel on.
	 * Make pixel visible.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void pixelOn(NPoint p) {
		pixelOn(p.x(), p.y());
	}

	// disp = 0100 0000
	//        AND NOT
	// set  = 0000 1000
	// res  = 0100 0000

	/**\brief Set pixel off.
	 * Make pixel invisible.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void pixelOff(U8 X, U8 Y) {
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) &=
				~(1 << (Y % LCD::DEPTH));
	}

	/**\brief Set pixel off.
	 * Make pixel invisible.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void pixelOff(NPoint p) {
		pixelOff(p.x(), p.y());
	}

	// disp = 0100 1000
	//        XOR
	// set  = 0000 1001
	// res  = 0100 0001

	/**\brief Invert pixel.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void invertPixel(U8 X, U8 Y) {
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) ^=
				(1 << (Y % LCD::DEPTH));
	}

	/**\brief Invert pixel.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline void invertPixel(NPoint p) {
		invertPixel(p.x(), p.y());
	}

	/**\brief Get pixel state.
	 * \note This function takes more time than all other set functions.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline bool getPixel(U8 X, U8 Y) const {
		U8 k = Y % LCD::DEPTH;
		return ((*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X))) & (1 << k)) >> k;
	}

	/**\brief Get pixel state.
	 * \note This function takes more time than all other set functions.
	 * @param X X-Coordinate.
	 * @param Y Y-Coordinate.
	 */
	inline bool getPixel(NPoint p) const {
		return getPixel(p.x(), p.y());
	}

	/**
	 * \brief Set pixel with functor
	 * @param op Functor: NLcd::PixelOn, NLcd::PixelOff, NLcd::PixelInvert
	 * @param X  X-Coordinate.
	 * @param Y  Y-Coordinate.
	 */
	template<typename PixelOp>
	inline void setPixel(PixelOp op, U8 X, U8 Y) {
		op(*this, X, Y);
	}

	/**
	 * \brief Set pixel with functor
	 * @param op Functor: NLcd::PixelOn, NLcd::PixelOff, NLcd::PixelInvert
	 * @param X  X-Coordinate.
	 * @param Y  Y-Coordinate.
	 */
	template<typename PixelOp>
	inline void setPixel(PixelOp op, NPoint p) {
		op(*this, p);
	}

	/**
	 * \brief Functor for NLcd::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelOn {
		void operator()(NLcd& lcd, U8 x, U8 y) const {
			lcd.pixelOn(x, y);
		}

		void operator()(NLcd& lcd, NPoint p) const {
			lcd.pixelOn(p);
		}
	};

	/**
	 * \brief Functor for NLcd::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelOff {
		void operator()(NLcd& lcd, U8 x, U8 y) const {
			lcd.pixelOff(x, y);
		}

		void operator()(NLcd& lcd, NPoint p) const {
			lcd.pixelOff(p);
		}
	};

	/**
	 * \brief Functor for NLcd::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelInvert {
		void operator()(NLcd& lcd, U8 x, U8 y) const {
			lcd.invertPixel(x, y);
		}

		void operator()(NLcd& lcd, NPoint p) const {
			lcd.invertPixel(p);
		}
	};
};


void NLcd::copyBitmap(const U8 *data, U32 width, U32 depth, U32 x, U32 y) {
	if (!LCD::objectInLcd(x, y, LCD::lineToPixelY(depth), width)) {
		return;
	}
	// copy full bitmap to display
	U32 iy; // in y
	U32 jx; // in x

	for (iy = y; iy < depth; iy++) {
		for (jx = x; jx < width; jx++) {
			pixelMask(jx, iy, data[iy * LCD::WIDTH + jx]);
		}
	}
}


} // namespace nxpl

#endif /* __NLCD_HPP_ */
