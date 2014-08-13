
#include "LcdDrawer.hpp"
#include "..\..\..\GNUARM\include\c++\4.0.2\cstdlib" //abs

namespace nxpl {

bool drawLine(NLcd &lcd, NPoint start, NPoint end, DrawOpt op) {
	if(!lcd.noError()) return false;
	if(!pointInLcd(start) || !pointInLcd(end)) return false;

	const S8 height = std::abs(end.y() - start.y());
	const S8 width = std::abs(end.x() - start.x());

	S8 ix = start.x();
	S8 iy = start.y();
	const S8 sx = ix < end.x() ? 1 : -1;
	const S8 sy = iy < end.y() ? 1 : -1;
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
		if (ix == end.x() && iy == end.y())
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
	return true;
}


bool drawRectangle(NLcd &lcd, NPixelBox geometry, DrawOpt op) {
	if(!pixelBoxInLcd(geometry)) return false;

	NPoint rightTop(geometry.base().x() + geometry.width(), geometry.base().y());
	NPoint rightBottom(geometry.base().x() + geometry.width(), geometry.base().y() + geometry.height());
	NPoint leftBottom(geometry.base().x(), geometry.base().y() + geometry.height());

	drawLine(lcd, geometry.base(), rightTop, op);
	drawLine(lcd, rightTop.setY(rightTop.y() + 1), rightBottom, op);
	drawLine(lcd, rightBottom.setX(rightBottom.x() - 1), leftBottom, op);
	drawLine(lcd, leftBottom.setY(leftBottom.y()-1), geometry.base().setY(geometry.base().y()+1), op);
	return true;
}


bool drawRectangleFilled(NLcd &lcd, const NPixelBox &geometry, DrawOpt op) {
	if(!drawRectangle(lcd, geometry, op)) return false;

	S8 xlast = geometry.base().x() + geometry.width();
	S8 ylast = geometry.base().y() + geometry.height();
	for (S8 iy = geometry.base().y() + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = geometry.base().x() + 1; ix < xlast; ++ix) {
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
	return true;
}


bool drawCircle(NLcd &lcd, NPoint center, S8 radius, DrawOpt op) {
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


bool drawCircleFilled(NLcd &lcd, NPoint center, S8 radius, DrawOpt op) {
	if(!drawCircle(lcd, center, radius, op)) return false;

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


bool drawEllipse(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op) {
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



bool drawEllipseFilled(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op) {
	if(!drawEllipse(lcd, center, a, b, op)) return false;

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