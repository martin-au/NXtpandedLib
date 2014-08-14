/*
 * NEllipse.cpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#include "NEllipse.hpp"

namespace nxpl {


bool NEllipse::draw(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op) {
	NPoint right(center.x() + a, center.y());
	NPoint top(center.x(), center.y() - b);
	NPoint left(center.x() - a, center.y());
	NPoint bottom(center.x(), center.y() + b);

	if (!pointInLcd(right) || !pointInLcd(top) || !pointInLcd(left)
			|| !pointInLcd(bottom))
		return false;

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

	S8 dx = 0, dy = b; /* im I. Quadranten von links oben nach rechts unten */
	S32 a2 = a * a, b2 = b * b;
	S32 err = b2 - (2 * b - 1) * a2, e2; /* Fehler im 1. Schritt */

	do {
		(lcd.*fpPixelState)(center.x() + dx, center.y() + dy); /* I. Quadrant */
		(lcd.*fpPixelState)(center.x() - dx, center.y() + dy); /* II. Quadrant */
		(lcd.*fpPixelState)(center.x() - dx, center.y() - dy); /* III. Quadrant */
		(lcd.*fpPixelState)(center.x() + dx, center.y() - dy); /* IV. Quadrant */

		e2 = 2 * err;
		if (e2 < (2 * dx + 1) * b2) {
			dx++;
			err += (2 * dx + 1) * b2;
		}
		if (e2 > -(2 * dy - 1) * a2) {
			dy--;
			err -= (2 * dy - 1) * a2;
		}
	} while (dy >= 0);

	while (dx++ < a) { /* fehlerhafter Abbruch bei flachen Ellipsen (b=1) */
		(lcd.*fpPixelState)(center.x() + dx, center.y()); /* -> Spitze der Ellipse vollenden */
		(lcd.*fpPixelState)(center.x() - dx, center.y());
	}
	return true;
}



bool NEllipseFilled::draw(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op) {
	if(!draw(lcd, center, a, b, op)) return false;

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
	S32 hh = b * b;
	S32 ww = a * a;
	S32 hhww = hh * ww;
	S8 x0 = a;
	S8 dx = 0;

// do the horizontal diameter
	for (int x = -a; x <= a; x++)
		//lcd.pixelOn(centerX + x, centerY);
		(lcd.*fpPixelState)(center.x() + x, center.y());

// now do both halves at the same time, away from the diameter
	for (int y = 1; y <= b; y++) {
		int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
		for (; x1 > 0; x1--)
			if (x1 * x1 * hh + y * y * ww <= hhww)
				break;
		dx = x0 - x1;  // current approximation of the slope
		x0 = x1;

		for (int x = -x0; x <= x0; x++) {
			//lcd.pixelOn(centerX + x, centerY - y);
			//lcd.pixelOn(centerX + x, centerY + y);
			(lcd.*fpPixelState)(center.x() + x, center.y() - y);
			(lcd.*fpPixelState)(center.x() + x, center.y() + y);
		}
	}
	return true;
}

}



