/*
 * NCircle.cpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#include "NCircle.hpp"

namespace nxpl {

bool NCircle::draw(NGenericPixelMatrix *matrix, NPoint center, S8 radius, DrawOpt op) {
	void (NGenericPixelMatrix::*fpPixelState)(const U8, const U8) = &NLcd::pixelOn;

	switch (op()) {
	case DrawOpt::drawID:
		fpPixelState = &NGenericPixelMatrix::pixelOn;
		break;
	case DrawOpt::clearID:
		fpPixelState = &NGenericPixelMatrix::pixelOff;
		break;
	case DrawOpt::invertID:
		fpPixelState = &NGenericPixelMatrix::invertPixel;
		break;
	}

	S8 f = 1 - radius;
	S8 ddF_x = 0;
	S8 ddF_y = -2 * radius;
	S8 x = 0;
	S8 y = radius;

	NPoint right(center.x() + radius, center.y());
	NPoint top(center.x(), center.y() - radius);
	NPoint left(center.x() - radius, center.y());
	NPoint bottom(center.x(), center.y() + radius);

	if (!pointInLcd(right) || !pointInLcd(top) || !pointInLcd(left)
			|| !pointInLcd(bottom))
		return false;

	// no casts at moment
	(matrix->*fpPixelState)(right.x(), right.y());
	(matrix->*fpPixelState)(top.x(), top.y());
	(matrix->*fpPixelState)(left.x(), left.y());
	(matrix->*fpPixelState)(bottom.x(), bottom.y());

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		(matrix->*fpPixelState)(center.x() + x, center.y() + y);
		(matrix->*fpPixelState)(center.x() - x, center.y() + y);
		(matrix->*fpPixelState)(center.x() + x, center.y() - y);
		(matrix->*fpPixelState)(center.x() - x, center.y() - y);
		(matrix->*fpPixelState)(center.x() + y, center.y() + x);
		(matrix->*fpPixelState)(center.x() - y, center.y() + x);
		(matrix->*fpPixelState)(center.x() + y, center.y() - x);
		(matrix->*fpPixelState)(center.x() - y, center.y() - x);
	}
	return true;
}

bool NCircleFilled::draw(NGenericPixelMatrix *matrix, NPoint center, S8 radius, DrawOpt op) {
	if(!NCircle::draw(matrix, center, radius, op)) return false;

	for (int y = -radius; y <= radius; y++) {
		for (int x = -radius; x <= radius; x++) {
			if (x * x + y * y <= radius * radius) {
				switch (op()) {
				case DrawOpt::drawID:
					matrix->pixelOn(center.x() + x, center.y() + y);
					break;
				case DrawOpt::clearID:
					matrix->pixelOff(center.x() + x, center.y() + y);
					break;
				case DrawOpt::invertID:
					matrix->invertPixel(center.x() + x, center.y() + y);
					break;
				default:
					matrix->pixelOn(center.x() + x, center.y() + y);
				}
				//lcd.pixelOn(centerX + x, centerY + y);
			}
		}
	}
	return true;
}

}


