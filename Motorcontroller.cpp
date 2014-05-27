/*
 * Motorcontroller.cpp
 *
 *  Created on: 28.01.2014
 *      Author: Martin
 */

#include "Motorcontroller.hpp"

#include "..\..\..\GNUARM\include\c++\4.0.2\cstdlib" // abs

namespace nxpl {


void Motorcontroller::controllerOff() {
   bool on;
   do {
      on = false;
      controlMtx.acquire();
      if (mon) {   // controller is on
         if(mgo) { // motor is not at target position
            on = true;
         } else {
            mot->setBrake(true);
            mot->setPWM(0);
            mon = false;
            mv  = 0;
            ma  = 0;
            mp  = 0;
         }
      }
      controlMtx.release();
      Sleep(1);
   } while (on);
}


// will set the controller to on!
void Motorcontroller::moveRel(S32 pos, S16 pwr, bool waitMove, S32 off) {
   pos = -pos;
   controlMtx.acquire();
   if(pwr > 0)
      phigh = pwr;
   mtx += -M_SCALE*(pos+off);
   mup = (mtx > mx);
   mon = true;
   mgo = true;
   controlMtx.release();
   if (off != 0) {
     this->waitMove();
     controlMtx.acquire();
     mtx -= -M_SCALE*off;
     mup = (mtx > mx);
     mgo = true;
     controlMtx.release();
   }
   if(waitMove)
      this->waitMove();
}



void Motorcontroller::moveAbs(S32 pos, S16 pwr, bool waitMove) {
   pos = -pos;
   if(pwr > 0) {
	  controlMtx.acquire();
      phigh = pwr;
   	  mtx = -M_SCALE*pos;
   	  mup = (mtx > mx);
   	  mon = true;
   	  mgo = true;
   	  controlMtx.release();
   	  if(waitMove) {
   		  this->waitMove();
   	  }
   }
}


S32 Motorcontroller::getAbsPos() const {
	S32 relCount = mot->getCount();
	S32 fullTurns = abs(relCount / 360);
	S32 absCount = relCount;
	if(relCount > 359) absCount = relCount - 360 * fullTurns;
	else if(relCount < 0) absCount = 360 * (fullTurns + 1) + relCount;
	return (absCount != 360) ? absCount : 0;
}


// please let the motor time to settle!
void Motorcontroller::resetMotorPos(S16 pwr) {
   S32 tachoNow, tachoPrev;

   // power limiting (anything above this level would be dangerous)
   if(pwr > 50)
      pwr = 50;
   if(pwr < -50)
      pwr = -50;

   //controlMtx.acquire();
   //mon = false;
   controllerOff();
   //Release(motor_mtx);
   mot->setPWM(pwr);
   mot->setBrake(true);
   tachoNow = mot->getCount();
   do {
      Sleep(50-pwr/2);
      tachoPrev = tachoNow;
      tachoNow = mot->getCount();
   } while(tachoNow != tachoPrev);
   mot->reset();
}


void Motorcontroller::process() {
	LockGuard lock(controlMtx);
	// if motor controller is on
	if (mon) {
		bool rev = false;
		//S8 mot = mc.mmot[m];
		S32 x = mx;
		S32 v = mv;
		S32 a = ma;
		S32 p = mp;
		S32 amax = this->amax;
		S32 vmax = this->vmax;
		S32 phigh = this->phigh;

		S32  e = 0;
		S32 ax = M_SCALE * mot->getCount();
		S32 ex = ax - x;
		if (-M_SCALE < ex && ex < M_SCALE)
			ax = x;
		else if (mgo)
			e = me - ex;
		S32 d = mtx - ax;
		if (mup ? (d < M_SCALE) : (d > -M_SCALE)) {
			mgo = false;
			e = 0;
		}
		if (d < 0) {
			d = -d;
			v = -v;
			a = -a;
			p = -p;
			e = -e;
			rev = true;
		}
		if (d >= M_SCALE) {
			if (v >= 0) {
				long dd = (v + amax / 2) + (v * v) / (2 * amax);
				if (d >= dd) {
					p = phigh;
					a = (amax * (vmax - v)) / vmax;
					e = 0;
				} else {
					a = -(v * v) / (2 * d);
					if (a < -v)
						a = -v;
					if (a < -amax)
						a = -amax;
					p = (phigh * a * (vmax - v)) / (amax * vmax);
				}
			} else {
				a = -v;
				if (a > amax)
					a = amax;
				p = (phigh * a * (vmax + v)) / (amax * vmax);
			}
		} else {
			a = -v;
			if (a < -amax)
				a = -amax;
			else if (a > amax)
				a = amax;
			p = (phigh * a) / amax;
		}
		d = v + a / 2;
		v += a;
		p += e;
		if (p > phigh) {
			p = phigh;
			e = 0;
		} else if (p < -phigh) {
			p = -phigh;
			e = 0;
		}
		if (rev) {
			d = -d;
			v = -v;
			a = -a;
			p = -p;
			e = -e;
		}

		if (p != mp) {
			if (p != 0)
				mot->setPWM(p);
			else {
				mot->setBrake(true);
				mot->setPWM(0);
			}
			mp = p;
		}
		mx = ax + d;
		mv = v;
		ma = a;
		me = e;
	}
}

} /* namespace Nxt */
