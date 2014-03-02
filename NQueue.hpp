/*
 * NQueue.h
 *
 *  Created on: 23.01.2014
 *      Author: Martin
 */

#ifndef __NQUEUE_H_
#define __NQUEUE_H_

/** \file
 *	\ingroup Utilities
*/

#include "stdlib"
#include "Uncopyable.hpp"

namespace nxpl {

/** \brief Defines a general queue abstraction.
*
* This interface defines a general queue abstraction that uses
* templates so that it can work with any element type.
*
* \sa http://en.wikipedia.org/wiki/Queue_%28abstract_data_type%29
*/
template <typename T>
class NQueue : private Uncopyable { // no copy functions at the moment.
private:
	static const U32 defaultCapacity = 10;

	T *data;
	S32 capacity;
	S32 head; // idx
	S32 tail; // idx

	void expandCapacity(S32 newCapacity);

public:
	/** \brief Construct queue with starting capacity.
	 *
	 * If you know how much elements will go into the queue, its good to pass the capacity so no memory is wasted.
	 *
	 * @param capacity Element capacity of queue.
	 */
	NQueue(U32 capacity = defaultCapacity)
	 : capacity(defaultCapacity), data(new T[capacity]), head(0), tail(0) {
	}

	~NQueue() {
		delete[] data;
	}

	/** \brief Get number of elements in the queue.
	 *
	 * @return number of elements in the queue.
	 */
	S32 size() const {
		return (tail + capacity - head) % capacity;
	}

	/** \brief Get capacity of queue.
	 *
	 * @return Capacity.
	 */
	S32 getCapacity() const {
		return capacity;
	}

	/** \brief Check if there are no elements in the queue.
	 *
	 * @return True if there are no elements.
	 */
	bool isEmpty() const {
		return head == tail;
	}

	/** \brief Clear all elements in queue (reset)
	 * The elements are not really deleted. (fast)
	 */
	void clear() {
		head = tail = 0;
	}

	/** \brief Append element to end of queue.
	 *
	 * If there is not enough capacity it will double the capacity.
	 *
	 * @param value Element.
	 */
	void enqueue(T value) {
		if (size() == capacity - 1) {
			expandCapacity(capacity*2);
		}
		data[tail] = value;
		tail = (tail + 1) % capacity;
	}

	/** \brief Put first element out of the queue.
	 *
	 * If there are no elements in the queue the program will crash!
	 * Please check before with NQueue::isEmpty() if the queue is empty.
	 *
	 * @return The dequeued element.
	 */
	T dequeue() {
		if (isEmpty()) { // TODO: What to do? error!
			std::exit(-1);
		}
		head = (head + 1) % capacity;
		return data[head];
	}

	/** \brief Get the first element of queue.
	 *
	 * @return First element.
	 */
	T peek() const {
		if (isEmpty()) {
			std::exit(-1);
		}
		return data[head];
	}
};

// extern "C" and Templates problem
#include "NQueue.cpp"

}

#endif /* __NQUEUE_H_ */
