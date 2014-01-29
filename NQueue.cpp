/*
 * NQueue.cpp
 *
 *  Created on: 23.01.2014
 *      Author: Martin
 */

#include "NQueue.hpp"


/*
* Implementation notes: Queue constructor
* ---------------------------------------
* The constructor must allocate the array storage for the queue
* elements and initialize the fields of the object.
*/
template <typename T>
NQueue<T>::NQueue()
 : capacity(INITIAL_CAPACITY), data(new T[capacity]), head(0), tail(0) {
}
/*
* Implementation notes: ~Queue destructor
* ---------------------------------------
* The destructor frees any memory that is allocated by the implementation.
*/
template <typename T>
NQueue<T>::~NQueue() {
	delete[] data;
}


/*
* Implementation notes: size
* --------------------------
* The size of the queue can be calculated from the head and tail
* indices by using modular arithmetic.
*/
template <typename T>
S32 NQueue<T>::size() {
	return (tail + capacity - head) % capacity;
}
/*
* Implementation notes: isEmpty
* -----------------------------
* The queue is empty whenever the head and tail indices are
* equal. Note that this interpretation means that the queue
* cannot be allowed to fill the capacity entirely and must
* always leave one unused space.
*/
template <typename T>
bool NQueue<T>::isEmpty() {
	return head == tail;
}


/*
* Implementation notes: clear
* ---------------------------
* The clear method need not take account of where in the
* ring buffer any existing data is stored and can simply
* set the head and tail index back to the beginning.
*/
template <typename T>
void NQueue<T>::clear() {
	head = tail = 0;
}
/*
* Implementation notes: enqueue
* -----------------------------
* This method must first check to see whether there is
* enough room for the element and expand the array storage
* if necessary.
*/
template <typename T>
void NQueue<T>::enqueue(T value) {
	if (size() == capacity - 1) {
		expandCapacity();
	}
	data[tail] = value;
	tail = (tail + 1) % capacity;
}


/*
* Implementation notes: dequeue, peek
* -----------------------------------
* These methods must check for an empty queue and report an
* error if there is no first element.
*/
template <typename T>
T NQueue<T>::dequeue() {
	if (isEmpty()) { // TODO: What to do? error!
		return dynamic_cast<T>(0);
	}
	T result = data[head];
	head = (head + 1) % capacity;
	return result;
}


template <typename T>
T NQueue<T>::peek() {
	if (isEmpty()) {
		// Error? User fault!
	}
	return data[head];
}


/*
* Implementation notes: expandCapacity
* ------------------------------------
* This private method doubles the capacity of the elements array
* whenever it runs out of space. To do so, it must allocate a new
* array, copy all the elements from the old array to the new one,
* and free the old storage. Note that this implementation also
* shifts all the elements back to the beginning of the array.
*/
template <typename T>
void NQueue<T>::expandCapacity() {
	S32 count = size();
	S32 oldCapacity = capacity;
	capacity *= 2;
	T *oldData = data;
	data = new T[capacity];
	for (S32 i = 0; i < count; i++) {
		data[i] = oldData[(head + i) % oldCapacity];
	}
	head = 0;
	tail = count;
	delete[] oldData;
}








