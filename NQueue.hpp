/*
 * NQueue.h
 *
 *  Created on: 23.01.2014
 *      Author: Martin
 */

#ifndef __NQUEUE_H_
#define __NQUEUE_H_

#include "stdlib"

/*
* This interface defines a general queue abstraction that uses
* templates so that it can work with any element type.
*/
namespace nxpl {

template <typename T>
class NQueue {
private:
	static const U32 defaultCapacity = 10;

	T *data;
	S32 capacity;
	S32 head; // idx
	S32 tail; // idx

	void expandCapacity(S32 newCapacity);

public:
	NQueue(U32 capacity = defaultCapacity)
	 : capacity(defaultCapacity), data(new T[capacity]), head(0), tail(0) {
	}

	~NQueue() {
		delete[] data;
	}

	S32 size() const {
		return (tail + capacity - head) % capacity;
	}

	S32 getCapacity() const {
		return capacity;
	}

	bool isEmpty() const {
		return head == tail;
	}

	void clear() {
		head = tail = 0;
	}

	void enqueue(T value) {
		if (size() == capacity - 1) {
			expandCapacity(capacity*2);
		}
		data[tail] = value;
		tail = (tail + 1) % capacity;
	}

	T dequeue() {
		if (isEmpty()) { // TODO: What to do? error!
			std::exit(-1);
		}
		head = (head + 1) % capacity;
		return data[head];
	}

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
