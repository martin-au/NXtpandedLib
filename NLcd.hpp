/*
 * NLcd.hpp
 *
 *  Created on: 17.11.2013
 *      Author: Martin
 */

#ifndef __NLCD_HPP_
#define __NLCD_HPP_

//#include "Uncopyable.hpp"

#include "LcdConstants.hpp"
//#include <algorithm>

namespace nxpl {

// functors for setPixel function
struct PixelOn;
struct PixelOff;
struct PixelInvert;

/**
 * NLcd is a very low level class
 * It is designed to direct access pixels of the lcd
 * Its high efficient so there is no range checking!!
 * If you write for example on pixel x 101 y 65 anything could happen!
 */
class NLcd {
private:
	U8 *disp;
public:

	// use standard lcd of nxt
	NLcd() : disp(display_get_buffer()) {
	}


	~NLcd() {
	}

	bool noError() {
		return (disp != 0);
	}


	/// fastest way to update pixel fields!
	// for example a horizontal line   *******  <- mask is same for every one
	inline void pixelMask(const U8 X, const U8 line, U8 const mask) {
		*(disp + (line * LCD::WIDTH + X)) = mask; // overlay! change to xor for no overlay?
	}

	void copyBitmap(const U8 *data, U32 width, U32 depth, U32 x, U32 y) {
		if (!LCD::objectInLcd(x, y, LCD::lineToPixel(depth), width)) {
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


	// disp = 0100 0000
	//        OR
	// set  = 0000 1000
	// res  = 0100 1000

	inline void pixelOn(const U8 X, const U8 Y) {
		/* Test for pixel write fails!
		if(!pixelInLcd(X, Y)) {
			cout << "fail" << endl;
			timer.wait(2000);
			return;
		}
		*/
		//*(disp + (LCD::pixelToLine(Y) * LCD::WIDTH + X)) |= (1 << (Y % LCD::DEPTH));
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) |= (1 << (Y % LCD::DEPTH));
	}


	// disp = 0100 0000
	//        AND NOT
	// set  = 0000 1000
	// res  = 0100 0000


	inline void pixelOff(const U8 X, const U8 Y) {
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) &= ~(1 << (Y % LCD::DEPTH));
	}

	// disp = 0100 1000
	//        XOR
	// set  = 0000 1001
	// res  = 0100 0001

	inline void invertPixel(const U8 X, const U8 Y) {
		*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X)) ^= (1 << (Y % LCD::DEPTH));
	}

	inline bool getPixel(const U8 X, const U8 Y) const {
		U8 k = Y % LCD::DEPTH;
		return ((*(disp + ((Y / LCD::DEPTH) * LCD::WIDTH + X))) & (1 << k)) >> k;
	}

	// is this useful? put outside of lcd class?
	template <typename PixelOp>
	inline void setPixel(PixelOp op, const U8 X, const U8 Y) {
		op(*this, X, Y);
	}
};


// functors
struct PixelOn {
	void operator()(NLcd& lcd, U8 x, U8 y) const {
		lcd.pixelOn(x, y);
	}
};

struct PixelOff {
	void operator()(NLcd& lcd, U8 x, U8 y) const {
		lcd.pixelOff(x, y);
	}
};

struct PixelInvert {
	void operator()(NLcd& lcd, U8 x, U8 y) const {
		lcd.invertPixel(x, y);
	}
};

} // namespace nxpl

#endif /* __NLCD_HPP_ */
