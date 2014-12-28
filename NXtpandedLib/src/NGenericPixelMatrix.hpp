/*
 * GenericPixelMatrix.hpp
 *
 *  Created on: 17.11.2013
 *      Author: Martin
 */

#ifndef __GPIXELMATRIX_HPP_
#define __GPIXELMATRIX_HPP_

/** \file
 *	\ingroup NxtLcd
*/

extern "C" {
#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h"
}

#include "../../../GNUARM/arm-elf/include/string.h"

#include "Uncopyable.hpp"
#include "NPoint.hpp"

//#define NXPL_GENERIC_PIXEL_MATRIX_ERRCHECK

namespace nxpl {

struct PixelOn;
struct PixelOff;
struct PixelInvert;

/** \brief Provides low level pixel functions.
 *
 * GenericPixelMatrix is a low level class
 * It is designed to direct access pixels of the lcd
 * Its high efficient so there is no range checking!!
 * If you write for example on pixel x 101 y 65 anything could happen!
 */
class NGenericPixelMatrix : private Uncopyable {
private:
	U8 *pixelMatrix;

	static const S8 DEPTH = 8; // 8 bit
	U8 mWidth, mRows;

protected:

	explicit NGenericPixelMatrix(U8 *matrix, U8 width, U8 rows) :
		pixelMatrix(matrix), mWidth(width), mRows(rows) {}

	~NGenericPixelMatrix() {}

	void setRows(U8 rows) {
		mRows = rows;
	}

	void setWidth(U8 width) {
		mWidth = width;
	}

	void deleteMatrix() {
		delete[] pixelMatrix;
	}

public:

	/** \brief Checks if there is an error.
	 *
	 *	If NXT crashes when using this class, first check noError before drawing.
	 *
	 * @return true if no error.
	 */
	bool noError() {
		return (pixelMatrix != 0);
	}

	U8 getRows() const {
		return mRows;
	}

	U8 getWidth() const {
		return mWidth;
	}

	/** \brief Set a pixel mask at any position in matrix
	 *
	 * This sounds complicated but let me explain: <br>
	 * The display array looks like this.  <br>
	 * |||| < line 0 <br>
	 * |||| < line 1 <br>
	 * 0123 < x <br>
	 * The mask is represented by '|' <br>
	 * If we look closer to the mask we see that each mask consists of 8 pixels (LCD::DEPTH)<br>
	 * The 8 pixels are represented by a unsigned 8 bit number<br>
	 * So if the number is 1 the pixel mask looks like this: 0000 0001 -> first pixel is set<br>
	 * If the number is 20 the pixel mask looks like this: 0001 0100<br>
	 *
	 * \note This is the fastest way to update a set of pixels.
	 *
	 * @param X   The X-Coordinate from the left.
	 * @param line The line from the top.
	 * @param mask Pixelmask.
	 */
	inline void pixelMask(U8 X, U8 line, U8 mask) {
		*(pixelMatrix + (line * this->getWidth() + X)) = mask; // overlay! change to xor for no overlay?
	}

	/** Set a pixel mask at any position in matrix
	 *
	 * Same as pixelMask but this functions keeps the pixels which are set at specified position.
	 *
	 * @param X
	 * @param line
	 * @param mask
	 */
	inline void pixelMaskKeep(U8 X, U8 line, U8 mask) {
		*(pixelMatrix + (line * this->getWidth() + X)) |= mask; // overlay! change to xor for no overlay?
	}

	inline void clear() {
		memset(pixelMatrix, 0, mWidth * mRows);
	}

	/** \brief Copy bitmap array into bitmap handled by GenericPixelMatrix or directly to nxt lcd.
	 *
	 * @param data Bitmap array
	 * @param width Width of bitmap
	 * @param depth Depth of bitmap
	 * @param x Offset-X-Coordinate in destination
	 * @param y Offset-Y-Coordinate in destination
	 */
	bool overlayOtherPixelMatrix(NGenericPixelMatrix matrix, bool transparent = false, U32 xPixel = 0, U32 yRow = 0);

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
		 if(!pixelIGenericPixelMatrix(X, Y)) {
		 cout << "fail" << endl;
		 timer.wait(2000);
		 return;
		 }
		 */
		//*(disp + (LCD::pixelToLine(Y) * LCD::WIDTH + X)) |= (1 << (Y % LCD::DEPTH));
		*(pixelMatrix + ((Y / DEPTH) * this->getWidth() + X)) |=
				(1 << (Y % DEPTH));
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
		*(pixelMatrix + ((Y / DEPTH) * this->getWidth() + X)) &=
				~(1 << (Y % DEPTH));
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
		*(pixelMatrix + ((Y / DEPTH) * this->getWidth() + X)) ^=
				(1 << (Y % DEPTH));
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
		U8 k = Y % DEPTH;
		return ((*(pixelMatrix + ((Y / DEPTH) * this->getWidth() + X))) & (1 << k)) >> k;
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
	 * @param op Functor: GenericPixelMatrix::PixelOn, GenericPixelMatrix::PixelOff, GenericPixelMatrix::PixelInvert
	 * @param X  X-Coordinate.
	 * @param Y  Y-Coordinate.
	 */
	template<typename PixelOp>
	inline void setPixel(PixelOp op, U8 X, U8 Y) {
		op(*this, X, Y);
	}

	/**
	 * \brief Set pixel with functor
	 * @param op Functor: GenericPixelMatrix::PixelOn, GenericPixelMatrix::PixelOff, GenericPixelMatrix::PixelInvert
	 * @param X  X-Coordinate.
	 * @param Y  Y-Coordinate.
	 */
	template<typename PixelOp>
	inline void setPixel(PixelOp op, NPoint p) {
		op(*this, p);
	}

	/**
	 * \brief Functor for GenericPixelMatrix::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelOn {
		void operator()(NGenericPixelMatrix& matrix, U8 x, U8 y) const {
			matrix.pixelOn(x, y);
		}

		void operator()(NGenericPixelMatrix& matrix, NPoint p) const {
			matrix.pixelOn(p);
		}
	};

	/**
	 * \brief Functor for GenericPixelMatrix::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelOff {
		void operator()(NGenericPixelMatrix& matrix, U8 x, U8 y) const {
			matrix.pixelOff(x, y);
		}

		void operator()(NGenericPixelMatrix& matrix, NPoint p) const {
			matrix.pixelOff(p);
		}
	};

	/**
	 * \brief Functor for GenericPixelMatrix::setPixel(PixelOp op, const U8 X, const U8 Y)
	*/
	struct PixelInvert {
		void operator()(NGenericPixelMatrix& matrix, U8 x, U8 y) const {
			matrix.invertPixel(x, y);
		}

		void operator()(NGenericPixelMatrix& matrix, NPoint p) const {
			matrix.invertPixel(p);
		}
	};
};

} // namespace nxpl

#endif /* __GenericPixelMatrix_HPP_ */
