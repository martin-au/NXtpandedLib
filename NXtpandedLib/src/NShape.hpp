/*
 * NShape_new.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NSHAPE_NEW_HPP_
#define __NSHAPE_NEW_HPP_

#include "NGuiObject.hpp"
#include "NGenericPixelMatrix.hpp"

/**
 * \file
 * \ingroup NxtLcd
*/

namespace nxpl {

/**
 * \brief Baseclass for all shapes.
 *
 * The class bundles together the NGenericPixelMatrix class which gives access to actual draw pixels on the any pixel matrix (bitmap, lcd, virtual lcd ..).
 */
class NShape : public NGuiObject {
protected:
	// at the moment protected for my comfort
	mutable NGenericPixelMatrix *pixelMatrix;

private:
	virtual void showShapeImpl() const = 0;
	virtual void hideShapeImpl() const = 0;
	virtual void invertShapeImpl() const = 0;

	virtual void showImpl() const {
		if(pixelMatrix == 0) return;
		showShapeImpl();
	}

	virtual void hideImpl() const {
		if(pixelMatrix == 0) return;
		hideShapeImpl();
	}

	virtual void invertImpl() const {
		if(pixelMatrix == 0) return;
		invertShapeImpl();
		setVisibility(true);
	}
public:
	NShape(NGenericPixelMatrix *matrix) : pixelMatrix(matrix) {}
	NShape() : pixelMatrix(0) {}
	virtual ~NShape() {}

	bool setPixelMatrix(NGenericPixelMatrix *matrix) const {
		pixelMatrix = matrix;
		if(pixelMatrix->noError()) {
			return true;
		} else {
			pixelMatrix = 0;
			return false;
		}
	}
};

}

#endif /* __NSHAPE_NEW_HPP_ */
