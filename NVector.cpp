/*
 * NVector.cpp
 *
 *  Created on: 12.10.2013
 *      Author: Martin
 */


#include "NVector.hpp"

template<typename T>
void NVector<T>::setCapacity(S32 in_capacity) {
	const S32 currentSize = this->endIndex - this->startIndex;
	S32 pointOfStart = (in_capacity * this->startIndex / this->capacity);

	if (in_capacity < currentSize) {
		in_capacity = currentSize;
		pointOfStart = 0;
	}

	if (in_capacity == this->capacity)
		return;

	T* buffer = reinterpret_cast<T*>(new char[sizeof(T) * in_capacity]);

	memcpy(&buffer[pointOfStart], &this->buffer[this->startIndex],
			sizeof(T) * currentSize);

	delete[] reinterpret_cast<char*>(this->buffer);
	this->buffer = buffer;

	this->startIndex = pointOfStart;
	this->endIndex -= this->startIndex + currentSize;
	this->capacity = in_capacity;
}

// ------------------------------------------------------------------- //
// ------------------------  vector  methods     --------------------- //
// ------------------------------------------------------------------- //

template<typename T>
void NVector<T>::insert(S32 inPosition, T inValue) {
	if ((this->endIndex - this->startIndex) == this->capacity) {
		setCapacity(static_cast<S32>(this->capacity * 1.5));
	}

	inPosition += this->startIndex;
	if (inPosition <= this->endIndex) {
		if (inPosition - this->startIndex > this->endIndex - inPosition
				|| this->startIndex == 0) {

			for (S32 index = this->endIndex; index != inPosition; --index) {
				this->buffer[index] = this->buffer[index - 1];
			}
			new ((void*) &this->buffer[inPosition]) T(inValue);
			++this->endIndex;
		} else {
			memcpy(&this->buffer[this->startIndex - 1],
					&this->buffer[this->startIndex], sizeof(T));
			new ((void*) &this->buffer[inPosition]) T(inValue);
		}

	}
}


template<typename T>
S32 NVector<T>::lastIndexOf(const T & inValue, S32 inFrom) const {
	inFrom += this->startIndex;
	for (S32 index = this->endIndex - 1; index >= inFrom; --index) {
		if (this->buffer[index] == inValue)
			return index;
	}

	return size();
}

template<typename T>
NVector<T> NVector<T>::subvector(S32 inPosition, S32 in_length) const {
	NVector vec(in_length * 2, in_length * 0.2);

	inPosition += this->startIndex;

	while (in_length--) {
		new ((void*) &vec.buffer[vec.end++]) T(this->buffer[inPosition++]);
	}

	return vec;
}


template<typename T>
S32 NVector<T>::removeAll(const T &inValue) {
	S32 index = this->startIndex;
	S32 counter = 0;
	// Do we find a value
	while (index != this->endIndex && this->buffer[index] != inValue)
		++index;
	// There is at least a value
	if (index != this->endIndex) {
		// Copying here
		S32 copyIndex = index;
		++index;
		++counter;
		// >>
		while (index != this->endIndex) {
			memcpy(&this->buffer[copyIndex], &this->buffer[index], sizeof(T));
			++copyIndex;
			++index;

			if (this->buffer[index] != inValue) {
				(&this->buffer[index])->~T();
				++index;
				++counter;
			}
		}
		// Delete other
		this->endIndex = copyIndex;
	}

	return counter;
}

template<typename T>
void NVector<T>::removeAt(S32 inPosition) {
	inPosition += this->startIndex;
	if (inPosition < this->endIndex) {
		(&this->buffer[inPosition])->~T();
		if (inPosition - this->startIndex > this->endIndex - inPosition) {
			++inPosition;
			memcpy(&this->buffer[inPosition - 1], &this->buffer[inPosition],
					sizeof(T) * (this->endIndex - inPosition));
			--this->endIndex;
		} else {
			--inPosition;
			while (inPosition != this->startIndex) {
				memcpy(&this->buffer[inPosition + 1], &this->buffer[inPosition],
						sizeof(T));
				--inPosition;
			}
			memcpy(&this->buffer[inPosition + 1], &this->buffer[inPosition],
					sizeof(T));
			++this->startIndex;
		}
	}
}

template<typename T>
bool NVector<T>::removeOne(const T & inValue) {
	S32 index = this->startIndex;

	while (index != this->endIndex && this->buffer[index] != inValue)
		++index;

	if (index != this->endIndex) {
		(&this->buffer[index])->~T();
		if (index - this->startIndex > this->endIndex - index) {
			++index;
			memcpy(&this->buffer[index - 1], &this->buffer[index],
					sizeof(T) * (this->endIndex - index));
			--this->endIndex;
		} else {
			--index;
			while (index != this->startIndex) {
				memcpy(&this->buffer[index + 1], &this->buffer[index],
						sizeof(T));
				--index;
			}
			memcpy(&this->buffer[index + 1], &this->buffer[index], sizeof(T));
			++this->startIndex;
		}
		return true;
	}
	return false;
}



template<typename T>
void NVector<T>::swap(S32 i, S32 j) {
	i += this->startIndex;
	j += this->startIndex;
	if (i < this->endIndex && j < this->endIndex) {
		T tmp = this->buffer[i];
		this->buffer[i] = this->buffer[j];
		this->buffer[j] = tmp;
	}
}

template<typename T>
bool NVector<T>::operator==(const NVector<T> & other) const {
	if (this->endIndex - this->startIndex != other.end - other.startIndex)
		return false;

	for (S32 index = this->startIndex, otherIndex = other.startIndex;
			index != this->endIndex; ++index, ++otherIndex) {
		if (this->buffer[index] != this->buffer[otherIndex])
			return false;
	}

	return true;
}



// ------------------------------------------------------------------- //
// ------------------------  iterator methods    --------------------- //
// ------------------------------------------------------------------- //

template<typename T>
typename NVector<T>::iterator NVector<T>::erase(
		typename NVector<T>::iterator inIter) {
	if (inIter.vec == this && inIter.index < this->endIndex
			&& inIter.index >= this->startIndex) {
		removeAt(inIter.index - this->startIndex);
		if (inIter.index >= this->endIndex)
			--inIter.index;
	}
	return inIter;
}

template<typename T>
typename NVector<T>::iterator NVector<T>::erase(
		typename NVector<T>::iterator inBegin,
		typename NVector<T>::iterator inEnd) {
	// check iterators
	if (inBegin.vec != this || inEnd.vec != this
			|| inBegin.index < this->startIndex
			|| inEnd.index < this->startIndex || inBegin.index >= this->endIndex
			|| inEnd.index >= this->endIndex)
		return begin();

	// if end befor begin, swap
	if (inBegin.index > inEnd.index) {
		const S32 tmp = inEnd.index;
		inEnd.index = inBegin.index;
		inBegin.index = tmp;
	}

	// if beginthis->start longer than endthis->end, we work from the end
	if (inBegin.index - this->startIndex > this->endIndex - inEnd.index) {
		// offset is the position difference
		S32 offset = inEnd.index - inBegin.index;
		// destructor for the next deleted elements
		while (offset--) {
			(&this->buffer[inBegin.index + offset])->~T();
		}
		// use memcpy
		offset = inEnd.index - inBegin.index;
		memcpy(&this->buffer[inBegin.index], &this->buffer[inEnd.index],
				sizeof(T) * (offset));
		// move end indicator
		this->endIndex -= offset;
		inEnd.index -= offset;
	} else {
		S32 offset = inEnd.index - inBegin.index;
		// destructor for the next deleted elements
		while (offset--) {
			(&this->buffer[inBegin.index + offset])->~T();
		}
		// copy one by one
		while (inEnd.index != inBegin.index) {
			memcpy(&this->buffer[inBegin.index],
					&this->buffer[inBegin.index - 1], sizeof(T));
			++inBegin.index;
		}
		this->startIndex += inEnd.index - inBegin.index;
	}

	return inBegin;
}

template<typename T>
typename NVector<T>::iterator NVector<T>::insert(
		typename NVector<T>::iterator in_before, const T & inValue) {
	// check iterator index
	if (in_before.vec == this && in_before.index < this->endIndex
			&& in_before.index >= this->startIndex) {
		in_before.index -= this->startIndex;
		// insert with normal form
		insert(in_before.index, inValue);
		// set index to old index +1
		in_before.index += this->startIndex + 1;
	} else
		in_before.index = this->startIndex;

	return in_before;
}

