/*
 * Motorcontroller.hpp
 *
 *  Created on: 28.01.2014
 *      Author: Martin
 */

#ifndef __MOTORCONTROLLER_HPP_
#define __MOTORCONTROLLER_HPP_

/** \file
 *	\ingroup NxtIO
*/

#include "Mutex.hpp"
#include "Uncopyable.hpp"

//#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"
//#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Port.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Motor.h"

// Sleep
extern "C" {
	#include "C:/cygwin/nxtOSEK/ecrobot/c/rtoscalls.h"
}


namespace nxpl {

/** \brief Motorcontroller for NXT motors position controlling.
 *
 * This is a aggressive motorcontroller. It tries to move the motor as fast as possible to the setpoint and
 * then hold the position consequently.
 * Its important to call the Motorcontroller::process() method in a rate monotonic task.
 * After that you can use all functions.
 */
class Motorcontroller {
private:
	//static const S32 nxtMotorPorts = NUM_PORT_M;
	ecrobot::Motor* mot;

	//long M_DELAY;
	S16  	 M_SCALE;

	S16      amax;
	S16      vmax;

	S16      phigh;
	//S8       mmot;
	bool     mon;
	bool     mgo;
	bool     mup;
	S32      mtx;
	S32      mx;
	S32      mv;
	S32      ma;
	S32      mp;
	S32      me;

public:

	/**
	 * \brief Construct and initialize the motorcontroller control parameters.
	 *
	 * Set control parameters for given motor.
	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 * Vmax is the maximum Velocity that will be achieved.
	 * High vmax -> smoother breaking, no overshooting.
	 *
	 * @param motor The motor which should be controlled.
	 * @param paraAmax Control-parameter maximum Acceleration.
	 * @param paraVmax Control-parameter maximum Velocity.
	*/
	Motorcontroller(ecrobot::Motor *motor, S16 paraAmax, S16 paraVmax)
		: mot(motor),
		  M_SCALE(12),
		  amax(paraAmax),
		  vmax(paraVmax),
		  phigh(87),
		  //mmot(0),
		  mon(0),
		  mgo(0),
		  mup(0),
		  mtx(0),
		  mx(0),
		  mv(0),
		  ma(0),
		  mp(0),
		  me(0) {
		mot->setBrake(true);
	}

	/**
	 * \brief Destructor calls Motorcontroller::controllerOff();
	 */
	~Motorcontroller() {
		// TODO Problem because we use Sleep
		this->controllerOff();
	}

	/**
	  \brief Set control parameter for one motor.

	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 *
	 * \param paraAmax Control-parameter maximum Acceleration.
	*/
	void setAmax(S16 paraAmax) {
		// GetResource
		amax = paraAmax;
		// ReleaseResource
	}

	/**
	 \brief Set control parameter for one motor.

	 * Vmax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 *
	 * \param paraVmax Control-parameter maximum Velocity.
	 */
	void setVmax(S16 paraVmax) {
		vmax = paraVmax;
	}

	/**
	 * \brief Turns the controller on.
	 *
	 * Note that Motorcontroller will be started automatically when calling a move funtion.
     * Its mainly for saving process time with controllerOff/On.
	 */
	void controllerOn() {
		mon = true;
	}

	/**
	 * \brief Turns the controller off.
	 *
	 * Waits until motor is on setpoint then brake the motor.
	 * Controller intern regulation parameters are reseted.
	 */
	void controllerOff();


	/** \brief Wait until motor is on setpoint.
	*
	* This function is a blocking function. This means it will return when the motor is on setpoint.
	*/
	void waitMove() const {
	   bool go;
	   do {
	      Sleep(1);
	      //Shedule();
	      //Acquire(motor_mtx);
	      go = mgo;
	      //Release(motor_mtx);
	   } while (go);
	}

	/** \brief Checks if motor is on setpoint.
	 *
	 * @return true if motor is on setpoint.
	 */
	bool moveDone() const {
	   bool go;
	   //Acquire(motor_mtx);
	   go = mgo;
	   //Release(motor_mtx);
	   return go;
	}

	/** \brief Move motor relative.
	 *
	 * Move relative means move motor to position: actual position + angle.
	 * If you pass pwr 0 it will take the last used motorpower.
	 *
	 * @param angle Angle in degrees.
	 * @param pwr   The maximum allowed motor PWM power. (0 - 100)
	 * @param waitMove If true: Wait until move complete. (blocking)
	 * @param off
	 */
	void moveRel(S32 angle, S16 pwr = 0, bool waitMove = true, S32 off = 0);

	/** \brief Move motor absolute.
	 *
	 * Move absolute means move motor to give position on a circle 0° - 360°
	 * If you pass pwr 0 it will take the last used motorpower.
	 *
	 * @param pos  Angle in degrees.
	 * @param pwr   The maximum allowed motor PWM power. (0 - 100)
	 * @param waitMove If true: Wait until move complete. (blocking)
	 */
	void moveAbs(S32 pos, S16 pwr = 0, bool waitMove = true);

	/** \brief Move motor.
	 *
	 * Move motor with given power. This will turn the controller off.
	 * The direction is regulated with the sign.
	 *
	 * @param pwr  The maximum allowed motor PWM power. (0 - 100)
	 */
	void move(S16 pwr) {
		this->controllerOff();
		mot->setPWM(pwr);
	}

	/** \brief Stop motor.
	 *
	 * Stop motor and set in breaking or floating mode. This will turn the controller off.
	 *
	 * @param brake If true set brake else set floating mode.
	 */
	void stop(bool brake) {
		this->controllerOff();
		mot->setBrake(brake);
		mot->setPWM(0);
	}

	/**
	 * \brief Reset the motor position.
	 *
	 * Reset the motor by turning the shaft until stalling.
	 * Be aware of using this function. The position is used as absolute 0 position.
	 *
	 * @param pwr Output power, from -50 to +50, use the sign for direction.
	 *
	*/
	void resetMotorPos(S16 pwr);

	/** \brief Control-Algorithm process.
	 *
	 * Call this function in a rate monotonic scheduled task.
	 * Cycle times around 10 ms are ok. Priority should be very high!
	 * All the control work is done in this function.
	 */
	void process();
};


void Motorcontroller::controllerOff() {
   bool on;
   do {
      on = false;
      if (mon) {
         if(mgo) {
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
      Sleep(1);
   } while (on);
}


// will set the controller to on!
void Motorcontroller::moveRel(S32 pos, S16 pwr, bool waitMove, S32 off) {
   pos = -pos;
   //Acquire(motor_mtx);
   if(pwr > 0)
      phigh = pwr;
   mtx += -M_SCALE*(pos+off);
   mup = (mtx > mx);
   mon = true;
   mgo = true;
   //Release(motor_mtx);
   if (off != 0) {
     this->waitMove();
     //Acquire(motor_mtx);
     mtx -= -M_SCALE*off;
     mup = (mtx > mx);
     mgo = true;
     //Release(motor_mtx);
   }
   if(waitMove)
      this->waitMove();
}



void Motorcontroller::moveAbs(S32 pos, S16 pwr, bool waitMove) {
   pos = -pos;
   if(pwr > 0) {
	  // Acquire(motor_mtx);
      phigh = pwr;
   	  mtx = -M_SCALE*pos;
   	  mup = (mtx > mx);
   	  mon = true;
   	  mgo = true;
   	  //Release(motor_mtx);
   	  if(waitMove) {
   		  this->waitMove();
   	  }
   }
}



// please let the motor time to settle!
void Motorcontroller::resetMotorPos(S16 pwr) {
   S32 tachoNow, tachoPrev;

   // power limiting (anything above this level would be dangerous)
   if(pwr > 50)
      pwr = 50;
   if(pwr < -50)
      pwr = -50;

   //Acquire(motor_mtx);
   mon = false;
   //Release(motor_mtx);
   mot->setPWM(pwr);
   mot->setBrake(true);
   tachoNow = mot->getCount();
   do {
      Sleep(25);
      tachoPrev = tachoNow;
      tachoNow = mot->getCount();
   } while(tachoNow != tachoPrev);
   mot->setPWM(0);
   mot->reset();
}



void Motorcontroller::process() {
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



} /* namespace nxpl */


#endif /* __MOTORCONTROLLER_HPP_ */
