
#ifndef __LCDDRAWER_HPP_
#define __LCDDRAWER_HPP_

#include "NLcd.hpp"
#include "GuiTypes.hpp"
#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib" // abs?

namespace Drawer {

void drawLine(NLcd lcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1, const DrawOpt &op) {
	// TODO: low level pixel functions use U8!
	S8 height = abs(y1 - y0);
	S8 width = abs(x1 - x0);

	S8 ix = x0;
	S8 iy = y0;
	S8 sx = ix < x1 ? 1 : -1;
	S8 sy = iy < y1 ? 1 : -1;
	S16 err = width + (-height), e2;

	for (;;) {

		switch (op()) {
		case DrawOpt::drawID:
			lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		case DrawOpt::clearID:
			lcd.pixelOff(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		case DrawOpt::invertID:
			lcd.invertPixel(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		default:
			lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
		}

		//lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));

		if (ix == x1 && iy == y1)
			break;
		e2 = 2 * err;
		if (e2 > (-height)) {
			err += (-height);
			ix += sx;
		}
		if (e2 < width) {
			err += width;
			iy += sy;
		}
	}
}


void drawRectangle(NLcd lcd, const S8 x0, const S8 y0, const S8 w, const S8 h, const DrawOpt &op) {
	drawLine(lcd, x0, 		  y0, 		  x0 + w, 	y0,     op);
	drawLine(lcd, x0 + w, 	  y0 + 1, 	  x0 + w, 	y0 + h, op);
	drawLine(lcd, x0 + w - 1, y0 + h, 	  x0, 		y0 + h, op);
	drawLine(lcd, x0, 		  y0 + h - 1, x0, 		y0,     op);
}


void drawRectangleFilled(NLcd lcd, const S8 x0, const S8 y0, const S8 w, const S8 h, const DrawOpt &op) {
	drawRectangle(lcd, x0, y0, w, h, op);
	S8 xlast = x0 + w;
	S8 ylast = y0 + h;
	for (S8 iy = y0 + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = x0 + 1; ix < xlast; ++ix) {

			switch (op()) {
			case DrawOpt::drawID:
				lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
				break;
			case DrawOpt::clearID:
				lcd.pixelOff(static_cast<U8>(ix), static_cast<U8>(iy));
				break;
			case DrawOpt::invertID:
				lcd.invertPixel(static_cast<U8>(ix), static_cast<U8>(iy));
				break;
			default:
				lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
			}

			//lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
		}
	}
}


void drawCircle(NLcd lcd, const S8 centerX, const S8 centerY, const S8 radius, const DrawOpt &op) {
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

	// no casts at moment

	(lcd.*fpPixelState)(centerX, centerY + radius);
	(lcd.*fpPixelState)(centerX, centerY - radius);
	(lcd.*fpPixelState)(centerX + radius, centerY);
	(lcd.*fpPixelState)(centerX - radius, centerY);


	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		(lcd.*fpPixelState)(centerX + x, centerY + y);
		(lcd.*fpPixelState)(centerX - x, centerY + y);
		(lcd.*fpPixelState)(centerX + x, centerY - y);
		(lcd.*fpPixelState)(centerX - x, centerY - y);
		(lcd.*fpPixelState)(centerX + y, centerY + x);
		(lcd.*fpPixelState)(centerX - y, centerY + x);
		(lcd.*fpPixelState)(centerX + y, centerY - x);
		(lcd.*fpPixelState)(centerX - y, centerY - x);
	}
}

// TODO work here with functors?
void drawCircleFilled(NLcd lcd, const S8 centerX, const S8 centerY, const S8 radius, const DrawOpt &op) {
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


	for (int y = -radius; y <= radius; y++) {
		for (int x = -radius; x <= radius; x++) {
			if (x * x + y * y <= radius * radius)
				(lcd.*fpPixelState)(centerX + x, centerY + y);
				//lcd.pixelOn(centerX + x, centerY + y);
		}
	}

	drawCircle(lcd, centerX, centerY, radius, op);
}


void drawEllipse(NLcd lcd, const S8 centerX, const S8 centerY, const S8 a,
		const S8 b, const DrawOpt &op) {
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
		(lcd.*fpPixelState)(centerX + dx, centerY + dy); /* I. Quadrant */
		(lcd.*fpPixelState)(centerX - dx, centerY + dy); /* II. Quadrant */
		(lcd.*fpPixelState)(centerX - dx, centerY - dy); /* III. Quadrant */
		(lcd.*fpPixelState)(centerX + dx, centerY - dy); /* IV. Quadrant */

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
		(lcd.*fpPixelState)(centerX + dx, centerY); /* -> Spitze der Ellipse vollenden */
		(lcd.*fpPixelState)(centerX - dx, centerY);
	}
}


// TODO work with functors
void drawEllipseFilled(NLcd lcd, const S8 centerX, const S8 centerY, const S8 a, const S8 b, const DrawOpt &op) {
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
		(lcd.*fpPixelState)(centerX + x, centerY);

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
			(lcd.*fpPixelState)(centerX + x, centerY - y);
			(lcd.*fpPixelState)(centerX + x, centerY + y);
		}
	}
	Drawer::drawEllipse(lcd, centerX, centerY, a, b, op);
}


} // namespace


#endif
