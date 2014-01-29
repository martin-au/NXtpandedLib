/*
 * NQueue.h
 *
 *  Created on: 23.01.2014
 *      Author: Martin
 */

#ifndef NQUEUE_H_
#define NQUEUE_H_


/*
* This interface defines a general queue abstraction that uses
* templates so that it can work with any element type.
*/


template <typename T>
class NQueue {
public:
	NQueue();
	~NQueue();

	S32 size();

	bool isEmpty();

	void clear();

	void enqueue(T value);

	T dequeue();

	T peek();

private:
	static const int INITIAL_CAPACITY = 10;
	/* Instance variables */
	T *data;
	/* A dynamic array of the elements */
	S32 capacity;
	/* The allocated size of the array */
	S32 head;
	/* The index of the head element */
	S32 tail;
	/* The index of the tail element */

	void expandCapacity();
};


#include "NQueue.cpp"

#endif /* NQUEUE_H_ */
