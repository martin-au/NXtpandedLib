/*
 * Uncopyable.hpp
 *
 *  Created on: 03.11.2013
 *      Author: Martin
 */

#ifndef UNCOPYABLE_HPP_
#define UNCOPYABLE_HPP_

class Uncopyable {
protected:
	Uncopyable() {}
	~Uncopyable() {}
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};

#endif /* UNCOPYABLE_HPP_ */
