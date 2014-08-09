/*
 * NQueue.cpp
 *
 *  Created on: 23.01.2014
 *      Author: Martin
 */

#include "NQueue.hpp"

namespace nxpl {


/*
* This private method doubles the capacity of the elements array
* whenever it runs out of space. To do so, it must allocate a new
* array, copy all the elements from the old array to the new one,
* and free the old storage. Note that this implementation also
* shifts all the elements back to the beginning of the array.
*/
template <typename T>
void NQueue<T>::expandCapacity(S32 newCapacity) {
	if(newCapacity <= capacity) return;
	S32 count = size();
	T *oldData = data;
	data = new T[newCapacity];
	for (S32 i = 0; i < count; i++) {
		data[i] = oldData[(head + i) % capacity];
	}
	capacity = newCapacity;
	head = 0;
	tail = count;
	delete[] oldData;
}

} // namespace nxpl






