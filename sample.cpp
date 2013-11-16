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
DeclareTask(FlushConsole);
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
ostream cout(ostreamRes);

#include "NWidget.hpp"
#include "NNumIndicator.hpp"


#include "Uncopyable.hpp"



struct PixelOn;
struct PixelOff;
struct PixelInvert;


/**
 * NLcd is a very low level class
 * It is designed to direct access pixels of the lcd
 * Its high efficient so there is no range checking!!
 * If you write for example on pixel x 101 y 65 anything could happen!
 */
class NLcd : private Uncopyable{
private:
	U8 *disp;
public:
	static const S8 LCD_WIDTH = 100;
	static const S8 LCD_HEIGHT = 64;

	static const S8 LCD_ROWS = 8;
	static const S8 LCD_DEPTH = 8;

	NLcd() {
		disp = display_get_buffer();
	}
	~NLcd() {
		disp = 0;
	}

	bool noError() {
		return (disp != 0);
	}

	/// fastest way to update pixel fields!
	// for example a horizontal line   *******  <- mask is same for every one
	inline void pixelMask(const U8 X, const U8 line, U8 const mask) {
		*(disp + (line * LCD_WIDTH + X)) |= mask;
	}

	// disp = 0100 0000
	//        OR
	// set  = 0000 1000
	// res  = 0100 1000

	inline void pixelOn(const U8 X, const U8 Y) {
		*(disp + ((Y / LCD_DEPTH) * LCD_WIDTH + X)) |= (1 << (Y % LCD_DEPTH));
	}

	// disp = 0100 0000
	//        AND NOT
	// set  = 0000 1000
	// res  = 0100 0000


	inline void pixelOff(const U8 X, const U8 Y) {
		*(disp + ((Y / LCD_DEPTH) * LCD_WIDTH + X)) &= ~(1 << (Y % LCD_DEPTH));
	}

	// disp = 0100 1000
	//        XOR
	// set  = 0000 1001
	// res  = 0100 0001

	inline void invertPixel(const U8 X, const U8 Y) {
		*(disp + ((Y / LCD_DEPTH) * LCD_WIDTH + X)) ^= (1 << (Y % LCD_DEPTH));
	}



	template <typename PixelOp>
	inline void setPixel(PixelOp op, const U8 X, const U8 Y) {
		op(*this, X, Y);
	}

	inline bool getPixel(const U8 X, const U8 Y) const {
		U8 k = Y % LCD_DEPTH;
		return ((*(disp + ((Y / LCD_DEPTH) * LCD_WIDTH + X))) & (1 << k)) >> k;
	}
	static bool pixelInLcd(const U8 X, const U8 Y) {
		return  (X < LCD_WIDTH) && (Y < LCD_HEIGHT);
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



// better way then passing reference to NLcd?


#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib"

class NLine : public NWidget {
private:
	S8 endX, endY;
	mutable NLcd *lcd;
	// 0 off, 1 on, -1 invert
	void bresenham(void (NLcd::*fpPixelState)(const U8, const U8)) const;

	/*
	template <typename PixelOperation>
	void straight(PixelOperation pixelOperation) const;
	*/
	void straight(S8 op) const;
public:
	static const S8 keep = -1;

	explicit NLine(NLcd &nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1);
	NLine();
	~NLine() {}
	bool setLcd(NLcd &nlcd) const;
	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 x1 = keep, const S8 y1 = keep);
	void show(bool update = false) const;
	void erase(bool update = false) const;
	void invert(bool update = false) const;
};


NLine::NLine(NLcd &nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1)
 : lcd(&nlcd)
{
	setPosition(x0, y0, x1, y1);
	visible = false;
}

NLine::NLine()
	: endX(0), endY(0), lcd(0)
{
	visible = false;
}

bool NLine::setLcd(NLcd &nlcd) const {
	lcd = &nlcd;
	if(lcd->noError()) {
		return true;
	} else {
		lcd = 0;
		return false;
	}
}

//TODO: setPosition erease last position?
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



void NLine::bresenham(void (NLcd::*fpPixelState)(const U8, const U8)) const {
	// predetermine function to avoid ifs during calculation!
	// low level pixel functions use U8!
	S8 ix = x;
	S8 iy = y;
	S8 sx = ix < endX ? 1 : -1;
	S8 sy = iy < endY ? 1 : -1;
	S16 err = width + (-height), e2;

	for (;;) {
		// how to get the compiler to inline this (template?)!
		(lcd->*fpPixelState)(static_cast<U8>(ix), static_cast<U8>(iy));
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
	// predetermine function to avoid ifs during calculation!


	// test
	S8 sx = x < endX ? 1 : -1;
	S8 sy = y < endY ? 1 : -1;


	if(y == endY) {
		for(S8 ix=x; ; ix+=sx) {
			//lcd->pixelOn(pixelOperation, static_cast<U8>(ix), static_cast<U8>(y));
			switch(op) {
			case 1:
				lcd->pixelOn(static_cast<U8>(ix), static_cast<U8>(y));
				break;
			case 0:
				lcd->pixelOff(static_cast<U8>(ix), static_cast<U8>(y));
				break;
			case 2:
				lcd->invertPixel(static_cast<U8>(ix), static_cast<U8>(y));
				break;
			}
			if(ix == endX) break;
		}
	} else if(x == endX) {
		for(S8 iy=y; ; iy+=sy) {
			//lcd->pixelOn(pixelOperation, static_cast<U8>(x), static_cast<U8>(iy));
			switch(op) {
			case 1:
				lcd->pixelOn(static_cast<U8>(x), static_cast<U8>(iy));
				break;
			case 0:
				lcd->pixelOff(static_cast<U8>(x), static_cast<U8>(iy));
				break;
			case 2:
				lcd->invertPixel(static_cast<U8>(x), static_cast<U8>(iy));
				break;
			}
			if(iy == endY) break;
		}
	}
}



void NLine::show(bool update) const {
	if(lcd == 0) return;

	// i think user should be able to enter x,y in lcd
	// otherwise nothing will happen
	// intime pixel checking is not efficient!
	if (!NLcd::pixelInLcd(x, y) && !NLcd::pixelInLcd(endX, endY))
		return;
	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::pixelOn);
		straight(1);
	} else {
		bresenham(&NLcd::pixelOn);
	}
	visible = true;
	if (update) {
		display_update();
	}
}


void NLine::erase(bool update) const {
	if(lcd == 0) return;
	// i think user should be able to enter x,y in lcd
	// otherwise nothing will happen
	// intime pixel checking is not efficient!
	if( !NLcd::pixelInLcd(x, y) && !NLcd::pixelInLcd(endX, endY) )
		return;

	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::pixelOff);
		straight(0);
	} else {
		bresenham(&NLcd::pixelOn);
	}
	visible = false;
	if (update) {
		display_update();
	}
}


void NLine::invert(bool update) const {
	if(lcd == 0) return;
	// i think user should be able to enter x,y in lcd
	// otherwise nothing will happen
	// intime pixel checking is not efficient!
	if( !NLcd::pixelInLcd(x, y) && !NLcd::pixelInLcd(endX, endY) )
		return;

	if ((x == endX) || (y == endY)) {
		//straight(&NLcd::invertPixel);
		straight(2);
	} else {
		bresenham(&NLcd::pixelOn);
	}
	visible = !visible;
	if (update) {
		display_update();
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
	void show(bool update = false)   const;
	void erase(bool update = false)  const;
	void invert(bool update = false) const;
	void fill() 	 const;
	void fillErase() const;
	void fillInvert() const;

	//void erase(bool update = false);
	//void invert(bool update = false);
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



class NCircle : public NWidget {
private:
	mutable NLcd *lcd;
	S8 r;

	void rasterCircle(void (NLcd::*fpPixelState)(const U8, const U8)) const;
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

	// biggest circle
	NCircle circle(lcd, 50, 32, 31);
	t0 = timer.now();
	circle.show();
	t1 = timer.now();
	// all inline
	// mem: 26160
	// RT: 0

	// func pointer
	// mem: 26048
	// RT: 0

	dtIndic.setNumber(t1-t0);

	dtIndic.show();
	dtIndic.getBuddy()->show(true);

	TerminateTask();
}









TASK(FlushConsole)
{
	TerminateTask();
}

}
