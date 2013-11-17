/* sample.cpp for TOPPERS/ATK(OSEK) */

/**
 * nxtOsek C++ NXtpandedLib
 * Author: Martin Aumair
 * License: GNU GPL on all Files
 */




// ECRobot++ API
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"

//
#include "ostream.hpp"

using namespace ecrobot;
Clock timer;

extern "C" {

#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"


DeclareResource(ostreamRes);
DeclareTask(TaskMain);
DeclareEvent(EventSleep);
DeclareEvent(EventSleepI2C);
DeclareCounter(SysTimerCnt);


// hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);


// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	SleeperMonitor();
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}


} // extern C
// ostream cout(ostreamRes);

//#include "NWidget.hpp"
#include "NNumIndicator.hpp"

// pixel set functions
#include "NLcd.hpp"


#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib"

// make pure virtual function work with nxtOSEK
extern "C" int __cxa_pure_virtual(){return 0;}

class NShape : public NWidget {
protected:
	// at the moment protected for my comfort
	mutable NLcd *lcd;
public:
	NShape(NLcd &nlcd) : lcd(&nlcd) {}
	NShape() : lcd(0) {}
	virtual ~NShape() {}

	bool setLcd(NLcd &nlcd) const {
		lcd = &nlcd;
		if(lcd->noError()) {
			return true;
		} else {
			lcd = 0;
			return false;
		}
	}



	void show(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		showImpl(update);
		visible = true;
		if (update) {
			display_update();
		}
	}
	void erase(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		showImpl(update);
		visible = false;
		if (update) {
			display_update();
		}
	}
	void invert(bool update = false) const {
		if(lcd == 0) return;
		if (!inLcd()) return;
		showImpl(update);
		visible = !visible;
		if (update) {
			display_update();
		}
	}

private:
	virtual void showImpl(bool update) const = 0;
	virtual void eraseImpl(bool update) const = 0;
	virtual void invertImpl(bool update) const = 0;
};


#include "LcdConstants.hpp"

class NLine : public NShape {
private:
	S8 endX, endY;
	// 0 off, 1 on, -1 invert
	inline void pixelOperation(const S8 op, const S8 xx, const S8 yy) const;

	void bresenham(S8 op) const;  //
	void straight(S8 op) const;
public:
	static const S8 keep = -1;

	explicit NLine(NLcd &nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1);
	NLine();
	~NLine() {}
	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 x1 = keep, const S8 y1 = keep);

private:
	void showImpl(bool update) const;
	void eraseImpl(bool update) const;
	void invertImpl(bool update) const;
};


// set pixels in for loop ..
// I use switch -> see NLine::pixelOperation
/*
NLine line(lcd, 0, 0, 99, 0);
t0 = timer.now();
for(S8 i=0; i<NLcd::LCD_HEIGHT; ++i) {
	// x0, y0, xend, yend
	line.setPosition(NLine::keep, i, NLine::keep, i);
	line.show();
	line.erase();
	line.invert();
}
t1 = timer.now();
// call only straight not the bigger bresenham function
// function pointer
// 26384 flash memory
// at 51 ms with function pointer

// function objects
// 26592 flash memory
// 27 ms

// switch in for loop
// 26416
// 36 ms
*/




NLine::NLine(NLcd &nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1)
 : NShape(nlcd)
{
	setPosition(x0, y0, x1, y1);
	visible = false;
}

NLine::NLine()
	: endX(0), endY(0)
{
	visible = false;
}



void NLine::setPosition(const S8 x0, const S8 y0, const S8 x1, const S8 y1) {
	if (x0 != keep)
		this->x = x0;
	if (y0 != keep)
		this->y = y0;
	if (x1 != keep)
		this->endX = x1;
	if (y1 != keep)
		this->endY = y1;
	height = abs(endY - y);
	width = abs(endX - x);
}


/**
 *
 */
void NLine::pixelOperation(const S8 op, const S8 xx, const S8 yy) const {
	switch (op) {
	case 1:
		lcd->pixelOn(static_cast<U8>(xx), static_cast<U8>(yy));
		break;
	case 0:
		lcd->pixelOff(static_cast<U8>(xx), static_cast<U8>(yy));
		break;
	case 2:
		lcd->invertPixel(static_cast<U8>(xx), static_cast<U8>(yy));
		break;
	}
}


void NLine::bresenham(S8 op) const {   // void (NLcd::*fpPixelState)(const U8, const U8)
	// low level pixel functions use U8!
	S8 ix = x;
	S8 iy = y;
	S8 sx = ix < endX ? 1 : -1;
	S8 sy = iy < endY ? 1 : -1;
	S16 err = width + (-height), e2;

	for (;;) {
		//(lcd->*fpPixelState)(static_cast<U8>(ix), static_cast<U8>(iy));
		pixelOperation(op, ix, iy);
		if (ix == endX && iy == endY)
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

//template <typename PixelOperation>
//void NLine::straight(PixelOperation pixelOperation) const {
void NLine::straight(S8 op) const {
	S8 sx = x < endX ? 1 : -1;
	S8 sy = y < endY ? 1 : -1;

	if(y == endY) {
		for(S8 ix=x; ; ix+=sx) {
			//lcd->pixelOn(pixelOperation, static_cast<U8>(ix), static_cast<U8>(y));
			pixelOperation(op, ix, y);
			if(ix == endX) break;
		}
	} else if(x == endX) {
		for(S8 iy=y; ; iy+=sy) {
			//lcd->pixelOn(pixelOperation, static_cast<U8>(x), static_cast<U8>(iy));
			pixelOperation(op, x, iy);
			if(iy == endY) break;
		}
	}
}



void NLine::showImpl(bool update) const {
	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::pixelOn);
		straight(1);
	} else {
		bresenham(1);
	}
}


void NLine::eraseImpl(bool update) const {

	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::pixelOff);
		straight(0);
	} else {
		bresenham(0);
	}
}


void NLine::invertImpl(bool update) const {
	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::invertPixel);
		straight(2);
	} else {
		bresenham(2);
	}
}



#include "NVector.hpp"

class NRectangle : public NWidget {
private:
	mutable NLcd *lcd;
	// static saves memory with nxtosek
	enum lineOrientation {top, right, bottom, left, sides};

	NLine side[sides];
public:
	static const S8 keep = -1;

	NRectangle(NLcd &nlcd, const S8 x0, const S8 y0, const S8 w, const S8 h);
	~NRectangle() {}
	bool setLcd(NLcd &nlcd) const;


	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 w = keep, const S8 h = keep);
	void fill() 	 const;
	void fillErase() const;
	void fillInvert() const;

	void show(bool update = false)   const;
	void erase(bool update = false)  const;
	void invert(bool update = false) const;
};

NRectangle::NRectangle(NLcd &nlcd, const S8 x0, const S8 y0, const S8 w, const S8 h)
	: lcd(&nlcd) {

	for(U8 i=top; i < sides; ++i) {
		side[i].setLcd(nlcd);
	}

	setPosition(x0, y0, w, h);
}


void NRectangle::setPosition(const S8 x0, const S8 y0, const S8 w, const S8 h) {
	if (x0 != keep)
		this->x = x0;
	if (y0 != keep)
		this->y = y0;
	if (getWidth() != keep)
		this->width = w;
	if (getHeight() != keep)
		this->height = h;
	side[top].setPosition(		x0, 		y0, 		x0 + w, 	y0);
	side[right].setPosition(	x0 + w, 	y0 + 1, 	x0 + w, 	y0 + h);
	side[bottom].setPosition(	x0 + w - 1, y0 + h, 	x0, 		y0 + h);
	side[left].setPosition(		x0, 		y0 + h - 1, x0, 		y0);
}


bool NRectangle::setLcd(NLcd &nlcd) const {
	lcd = &nlcd;
	if(lcd->noError()) {
		return true;
	} else {
		lcd = 0;
		return false;
	}
}


void NRectangle::show(bool update) const {
	if(lcd == 0) {
		return;
	}

	for(U8 i=top; i < sides; ++i) {
		side[i].show();
	}
	visible = true;
	if (update) {
		display_update();
	}
}

void NRectangle::erase(bool update) const {
	if(lcd == 0) return;

	for(U8 i=top; i < sides; ++i) {
		side[i].erase();
	}
	visible = false;
	if (update) {
		display_update();
	}
}


void NRectangle::invert(bool update) const {
	if(lcd == 0) return;

	for(U8 i=top; i < sides; ++i) {
		side[i].invert();
	}
	visible = !visible;
	if (update) {
		display_update();
	}
}

void NRectangle::fill() const {
	if(lcd == 0) {
		return;
	}

	// V1 26416 - draw every line (pixel) - 2 ms
	// V2 26624 - fast drawing with pixel mask - 1 ms
	// conclusion:
	// V1 is better for small rectangles and constant / not time-critical
	// V2 is better for big rectangles - fast updating.
	// TODO What should i implement?
	// Argumentation:
	// Time: I do not know any applications where rectangles should be changed every display refresh (games on nxt?^^)
	// Size: Big filled rectangles on the small nxt screen would be space-inefficient?

	// A1 ------------------------------------------

	S8 ylast = y + height;
	S8 xlast = x + width;
	for (S8 iy = y + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = x + 1; ix < xlast ; ++ix) {
			lcd->pixelOn(ix, iy);
		}
	}

	// A2 -----------------------------------------------
	/*
	S8 xlast = x + width;
	S8 ylast = (y+height);
	S8 firstFull = (NWidget::pixelToLine(y)+1)*8;
	S8 lastFull =  (NWidget::pixelToLine(y+height)*8);
	S8 iy = 0;

	for (iy = y+1; iy < firstFull ; ++iy) {
		// straight line
		for (S8 ix = x+1; ix < xlast; ++ix) {
			lcd->pixelOn(ix, iy);
		}
	}
	// if (full lines)
	S8 precalc = (lastFull)/8;
	for (iy = (firstFull)/8; iy < precalc; ++iy) {
		// line width height 8
		for (S8 ix = x + 1; ix < xlast; ++ix) {
			lcd->pixelMask(ix, iy, 0xFF);
		}
	}

	for (iy = lastFull; iy < ylast; ++iy) {
		for (S8 ix = x + 1; ix < xlast; ++ix) {
			// straight line
			lcd->pixelOn(ix, iy);
		}
	}
	*/
}


void NRectangle::fillErase() const {
	if (lcd == 0) {
		return;
	}

	S8 ylast = y + height;
	S8 xlast = x + width;
	for (S8 iy = y + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = x + 1; ix < xlast; ++ix) {
			lcd->pixelOff(ix, iy);
		}
	}

}


void NRectangle::fillInvert() const {
	if(lcd == 0) {
		return;
	}

	S8 ylast = y + height;
	S8 xlast = x + width;
	for (S8 iy = y + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = x + 1; ix < xlast ; ++ix) {
			lcd->invertPixel(ix, iy);
		}
	}
}


/***************************************//////////////////////////



class NCircle : public NShape {
private:
	mutable NLcd *lcd;
	S8 r;

	void rasterCircle(void (NLcd::*fpPixelState)(const U8, const U8)) const;
	void rasterCircleFill(void (NLcd::*fpPixelState)(const U8, const U8)) const;
public:
	static const S8 keep = -1;

	// low memory approach (no new variables)
	const S8 centerX() const {
		return x + r;
	}
	const S8 centerY() const {
		return y + r;
	}

	NCircle(NLcd &nlcd, const S8 x0, const S8 y0, const S8 radius);
	~NCircle() {}
	bool setLcd(NLcd &nlcd) const;

	void setRadius(S8 radius) {
		if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
			r = radius;
	}

	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 radius = keep);
	void show(bool update = false)   const;
	void erase(bool update = false)  const;
	void invert(bool update = false) const;
	void fill() 	 const;
	void fillErase() const;
	void fillInvert() const;
};


NCircle::NCircle(NLcd &nlcd, const S8 x0, const S8 y0, const S8 radius)
	: lcd(&nlcd), r(radius) {

	setPosition(x0, y0, r);
}


void NCircle::setPosition(const S8 x0, const S8 y0, const S8 radius) {
	//erase();
	if (x0 != keep)
		this->x = x0 - radius;
	if (y0 != keep)
		this->y = y0 - radius;
	if (getWidth() != keep)
		this->width = 2*radius;
	if (getHeight() != keep)
		this->height = this->width;
}


bool NCircle::setLcd(NLcd &nlcd) const {
	lcd = &nlcd;
	if(lcd->noError()) {
		return true;
	} else {
		lcd = 0;
		return false;
	}
}


void NCircle::rasterCircle(void (NLcd::*fpPixelState)(const U8, const U8)) const {

	S8 f = 1 - r;
	S8 ddF_x = 0;
	S8 ddF_y = -2 * r;
	S8 x = 0;
	S8 y = r;

	// no casts at moment
	S8 cX = centerX();
	S8 cY = centerY();

	(lcd->*fpPixelState)(cX, cY + r);
	(lcd->*fpPixelState)(cX, cY - r);
	(lcd->*fpPixelState)(cX + r, cY);
	(lcd->*fpPixelState)(cX - r, cY);
	/*
	lcd->pixelOn(cX, cY + r);
	lcd->pixelOn(cX, cY - r);
	lcd->pixelOn(cX + r, cY);
	lcd->pixelOn(cX - r, cY);
	*/

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		(lcd->*fpPixelState)(cX + x, cY + y);
		(lcd->*fpPixelState)(cX - x, cY + y);
		(lcd->*fpPixelState)(cX + x, cY - y);
		(lcd->*fpPixelState)(cX - x, cY - y);
		(lcd->*fpPixelState)(cX + y, cY + x);
		(lcd->*fpPixelState)(cX - y, cY + x);
		(lcd->*fpPixelState)(cX + y, cY - x);
		(lcd->*fpPixelState)(cX - y, cY - x);

		/*
		lcd->pixelOn(cX + x, cY + y);
		lcd->pixelOn(cX - x, cY + y);
		lcd->pixelOn(cX + x, cY - y);
		lcd->pixelOn(cX - x, cY - y);
		lcd->pixelOn(cX + y, cY + x);
		lcd->pixelOn(cX - y, cY + x);
		lcd->pixelOn(cX + y, cY - x);
		lcd->pixelOn(cX - y, cY - x);
		*/
	}
}


void NCircle::rasterCircleFill(void (NLcd::*fpPixelState)(const U8, const U8)) const {
	S8 cX = centerX();
	S8 cY = centerY();

	for(int y=-r; y<=r; y++) {
	    for(int x=-r; x<=r; x++) {
	        if(x*x+y*y <= r*r)
	            (lcd->*fpPixelState)(cX+x, cY+y);
	    }
	}
	// brute force algo may overwrite outer circle
	if(visible) {
		rasterCircle(&NLcd::pixelOn);
	}
}

void NCircle::show(bool update) const {
	if(lcd == 0) {
		return;
	}
	rasterCircle(&NLcd::pixelOn);
	visible = true;
	if(update) {
		display_update();
	}
}

void NCircle::erase(bool update) const {
	if(lcd == 0) {
		return;
	}
	rasterCircle(&NLcd::pixelOff);
	visible = true;
	if(update) {
		display_update();
	}
}


void NCircle::invert(bool update) const {
	if(lcd == 0) {
		return;
	}
	rasterCircle(&NLcd::invertPixel);
	visible = !visible;
	if(update) {
		display_update();
	}
}


void NCircle::fill() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::pixelOn);
}


void NCircle::fillErase() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::pixelOff);
}


void NCircle::fillInvert() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::invertPixel);
}


#include "NVector.hpp"

extern "C" {

TASK(TaskMain)
{
	NNumIndicator<U32> dtIndic(static_cast<S8>(9), 6);
	NLabel dtLabel("time: ", &dtIndic);
	dtIndic.alignBuddy(NAlignment::top());
	U32 t0, t1; t0 = t1 = 0;


	NLcd lcd;

	// show raster lines

	/*
	for(S8 i=0; i<8; i++) {
		for(S8 k=51; k<100; k++) {
			lcd.pixelMask(k, i, static_cast<U8>(1));
		}
	}
	*/

	display_update();

	/*
	NLine line(lcd, 0, 0, 99, 0);
	t0 = timer.now();
	for(S8 i=0; i<NLcd::LCD_HEIGHT; ++i) {
		// x0, y0, xend, yend
		line.setPosition(NLine::keep, i, NLine::keep, i);
		line.show();
		line.erase();
		line.invert();
	}
	t1 = timer.now();
	// call only straight not the bigger bresenham function
	// function pointer
	// 26384 flash memory
    // at 51 ms with function pointer

	// function objects
	// 26592 flash memory
	// 27 ms

	// switch in for loop
	// 26416
	// 36 ms
	*/

	/*
	// biggest circle
	NCircle circle(lcd, 40, 32, 25);
	NCircle circleI(lcd, 40, 32, 10);
	t0 = timer.now();
	circle.show();
	circleI.show();
	circle.fill();
    circleI.fillInvert();
	t1 = timer.now();
	*/



	// dirty virtual test
	/*
	NCircle circle(lcd, 50, 32, 25);
	NLine line(lcd, circle.getX(), circle.getY(), circle.getX() + circle.getWidth(), circle.getY() + circle.getHeight());
	NRectangle rect(lcd, circle.getX(), circle.getY(), circle.getWidth(), circle.getHeight());

	NShape *obj[] = {&circle, &line, &rect};
	t0 = timer.now();
	for (S32 i=0; i<3; ++i) {
		obj[i]->show();
	}
	t1 = timer.now();
	*/

	NLine line(lcd, 0 , 0, 50, 50);

	t0 = timer.now();
	line.show();
	t1 = timer.now();


	dtIndic.setNumber(t1-t0);

	dtIndic.show();
	dtIndic.getBuddy()->show(true);


	display_update();
	TerminateTask();
}

}
