
#ifndef __LCDDRAWER_HPP_
#define __LCDDRAWER_HPP_

#include "NLcd.hpp"
#include "GuiTypes.hpp"
#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib" // abs?

namespace Drawer {

void drawLine(NLcd &lcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1, const DrawOpt &op) {
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


void drawRectangle(NLcd &lcd, const S8 x0, const S8 y0, const S8 w, const S8 h, const DrawOpt &op) {
	drawLine(lcd, x0, 		  y0, 		  x0 + w, 	y0,     op);
	drawLine(lcd, x0 + w, 	  y0 + 1, 	  x0 + w, 	y0 + h, op);
	drawLine(lcd, x0 + w - 1, y0 + h, 	  x0, 		y0 + h, op);
	drawLine(lcd, x0, 		  y0 + h - 1, x0, 		y0,     op);
}


void drawRectangleFilled(NLcd &lcd, const S8 x0, const S8 y0, const S8 w, const S8 h, const DrawOpt &op) {
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
		}
	}
}


void drawCircle(NLcd &lcd, const S8 centerX, const S8 centerY, const S8 radius, const DrawOpt &op) {
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

// drawCircleFilled only available in NCircle class at the moment!

}


#endif
