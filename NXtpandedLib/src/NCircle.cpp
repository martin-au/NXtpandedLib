/*
 * NCircle.cpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#include "NCircle.hpp"

namespace nxpl {

bool NCircle::draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op) {
	void (NLcd::*fpPixelState)(const U8, const U8) = &NLcd::pixelOn;

	switch (op()) {
	case DrawOpt::drawID:
		fpPixelState = &NLcd::pixelOn;
		break;
	case DrawOpt::clearID:
		fpPixelState = &NLcd::pixelOff;
		break;
	case DrawOpt::invertID:
		fpPixelState = &NLcd::invertPixel;
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
	(lcd.*fpPixelState)(right.x(), right.y());
	(lcd.*fpPixelState)(top.x(), top.y());
	(lcd.*fpPixelState)(left.x(), left.y());
	(lcd.*fpPixelState)(bottom.x(), bottom.y());

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		(lcd.*fpPixelState)(center.x() + x, center.y() + y);
		(lcd.*fpPixelState)(center.x() - x, center.y() + y);
		(lcd.*fpPixelState)(center.x() + x, center.y() - y);
		(lcd.*fpPixelState)(center.x() - x, center.y() - y);
		(lcd.*fpPixelState)(center.x() + y, center.y() + x);
		(lcd.*fpPixelState)(center.x() - y, center.y() + x);
		(lcd.*fpPixelState)(center.x() + y, center.y() - x);
		(lcd.*fpPixelState)(center.x() - y, center.y() - x);
	}
	return true;
}

bool NCircleFilled::draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op) {
	if(!NCircle::draw(lcd, center, radius, op)) return false;

	for (int y = -radius; y <= radius; y++) {
		for (int x = -radius; x <= radius; x++) {
			if (x * x + y * y <= radius * radius) {
				switch (op()) {
				case DrawOpt::drawID:
					lcd.pixelOn(center.x() + x, center.y() + y);
					break;
				case DrawOpt::clearID:
					lcd.pixelOff(center.x() + x, center.y() + y);
					break;
				case DrawOpt::invertID:
					lcd.invertPixel(center.x() + x, center.y() + y);
					break;
				default:
					lcd.pixelOn(center.x() + x, center.y() + y);
				}
				//lcd.pixelOn(centerX + x, centerY + y);
			}
		}
	}
	return true;
}

}


