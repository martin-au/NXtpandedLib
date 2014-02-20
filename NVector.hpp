/*
 * NVector.hpp
 *
 *  Created on: 12.10.2013
 *      Author: Martin
 */

// NVector has problems with linking correctly in extern "C" block
// C-Arrays are the way to go


#ifndef __NVECTOR_HPP_
#define __NVECTOR_HPP_


extern "C" {
	#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

//for codecorrection
#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"


// NULL
#include <stddef.h>
// memcpy
#include "C:/cygwin/GNUARM/arm-elf/include/string.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c++/util/New.cpp"

/**
* @author Berenger
* @version 0.5
* @date 24 novembre 2009
* @file NVector.hpp
* @package Utils
* @brief Basic vector based on a memory area
*
* This Vector used a memory area, so you have to use it as an array.
* So it is easy and fast to put data at start/end position.
* But it takes a long time to insert or remove value in the center arrea
*
* @must You must give the buffer size if you know it!
*
* @example NVector<S32> vec(100);
* @example for(S32 i = 0 ; i < 10 ;++i) vec.pushBack(i);
* @example
* @example vec[0] = 99;
* @example vec[2] = 100;
* @example
* @example vec.insert(0u,25);
* @example vec.insert(5u,25);
* @example
* @example for(S32 i = 0 ; i < 5 ;++i) vec.popBack();
*
* @example for(NVector<S32>::iterator iter( vec.begin() ); iter != vec.end() ; ++iter){
* @example  (*iter)+=5;
* @example }
*/

namespace nxpl {

// antibug
const float _DefaultStart = 0.30; /**< Default start position in %*/

template<typename T>
class NVector {
protected:
	T* buffer; /**< memory area*/

	S32 capacity; /**< memory capacity*/
	static const S32 SizeOfT = sizeof(T); /**< size of the object*/

	S32 startIndex; /**< start index*/
	S32 endIndex; /**< end inde*/

	static const S32 DefaultSize = 15; /**< Default size */

public:
	/**
	 *@brief constructor
	 */
	NVector() {
		this->buffer = reinterpret_cast<T*>(new char[sizeof(T) * DefaultSize]);
		this->capacity = DefaultSize;
		this->endIndex = this->startIndex = this->capacity * _DefaultStart;
	}

	/**
	 *@brief constructor
	 *@param inSize the buffer size
	 *@param inPointOfStart the point of start [0;1]
	 */
	NVector(const S32 inSize, const float inPointOfStart = _DefaultStart) {
		this->buffer = reinterpret_cast<T*>(new char[sizeof(T) * inSize]);
		this->capacity = inSize;
		if (inPointOfStart <= 1 && inPointOfStart >= 0)
			this->endIndex = this->startIndex = static_cast<S32>(this->capacity
					* inPointOfStart);
		else
			this->endIndex = this->startIndex = static_cast<S32>(this->capacity
					* _DefaultStart);
	}

	/**
	 *@brief destructor
	 */
	 ~NVector() {
		while (this->startIndex != this->endIndex) {
			(&this->buffer[this->startIndex++])->~T();
		}

		delete[] reinterpret_cast<char*>(this->buffer);
	}

	/**
	 *@brief get the buffer capacity
	 *@return the buffer capacity
	 */
	S32 getCapacity() const {
		return this->capacity;
	}

	/**
	 *@brief set the buffer capacity
	 *@param in_capacity to change the capacity
	 */
	void doSetCapacity(S32 in_capacity);

	// inline optimization
	// we return before we call the not inline function
	// note that we do not allow change capacity to less then current cap
	// use doSetCapacity for this
	void setCapacity(S32 in_capacity) {
		if (in_capacity <= this->capacity)
			return;
		doSetCapacity(in_capacity);
	}

	/**
	 *@brief set a value if possible
	 *@param inPosition the query position
	 *@param inValue the new value
	 */
	void set(const S32 inPosition, const T& inValue) {
		if (this->startIndex + inPosition < this->endIndex)
			this->buffer[this->startIndex + inPosition] = inValue;
	}

	/**
	 *@brief get a const reference of a given value
	 *@param inPosition the query position
	 *@return the value
	 */
	T& at(const S32 inPosition) {
		return this->buffer[this->startIndex + inPosition];
	}

	/**
	 *@brief get a const reference of a given value
	 *@param inPosition the query position
	 *@return the value
	 */
	const T& at(const S32 inPosition) const {
		return this->buffer[this->startIndex + inPosition];
	}

	/**
	 *@brief get a const reference of a given value
	 *@param inPosition the query position
	 *@return the value
	 */
	T& operator[](const S32 inPosition) {
		return this->buffer[this->startIndex + inPosition];
	}

	/**
	 *@brief get a const reference of a given value
	 *@param inPosition the query position
	 *@return the value
	 */
	const T& operator[](const S32 inPosition) const {
		return this->buffer[this->startIndex + inPosition];
	}

	/**
	 *@brief Append a value
	 *@param inValue the value to append
	 */
	void append(const T & inValue) {
		new ((void*) &this->buffer[this->endIndex++]) T(inValue);

		if (this->endIndex == this->capacity) {
			setCapacity(static_cast<S32>(this->capacity * 1.5));
		}
	}

	/**
	 *@brief Append a value
	 *@param other the vector to append
	 */
	void append(const NVector<T> & other) {
		if (this->capacity - this->endIndex < other.size())
			setCapacity(this->capacity + other.size());

		for (S32 index = other.startIndex; index < other.endIndex; ++index) {
			new ((void*) &this->buffer[++this->endIndex]) T(
					other.buffer[index]);
		}
	}

	/**
	 *@brief Get a value
	 *@param inPosition query position
	 *@return the data at requiered position, or the last data if inPosition is too big
	 * You must be sure that there is data in the vector
	 */
	const T& value(const S32 inPosition) const {
		return this->buffer[this->startIndex + inPosition];
	}

	/**
	 *@brief Get a value
	 *@param inPosition query position
	 *@param defaultValue defaulf value in case of bad inPosition
	 *@return the data at requiered position, or the defaultValue if inPosition is too big or vector is empty
	 */
	const T& value(const S32 inPosition, const T& defaultValue) const {
		inPosition += this->startIndex;
		if (inPosition >= this->endIndex)
			return defaultValue;
		return this->buffer[inPosition];
	}

	/**
	 *@brief last data
	 *@return end->data
	 */
	const T & last() const {
		return this->buffer[this->endIndex - 1];
	}

	/**
	 *@brief last data
	 *@return end->data
	 */
	T & last() {
		return this->buffer[this->endIndex - 1];
	}

	/**
	 *@brief last data
	 *@return begin->data
	 */
	const T & first() const {
		return this->buffer[this->startIndex];
	}

	/**
	 *@brief delete all, then size = 0
	 */
	void clear() {
		while (this->startIndex != this->endIndex) {
			(&this->buffer[this->startIndex++])->~T();
		}
		this->endIndex = this->startIndex = this->capacity
				* (_DefaultStart / 100.0);
	}

	/**
	 *@brief count the value
	 *@param inValue the value to test
	 *@return the value occured number
	 */
	S32 size() const {
		return this->endIndex - this->startIndex;
	}

	/**
	 *@brief count the value
	 *@param inValue the value to test
	 *@return the value occured number
	 */
	S32 count(const T & inValue) const {
		S32 counter = 0;
		for (S32 index = this->startIndex; index < this->endIndex; ++index) {
			if (this->buffer[index] == inValue)
				++counter;
		}
		return counter;
	}

	/**
	 *@brief test if the vector contains the value
	 *@param inValue the value to test
	 *@return true if inValue is found
	 */
	bool contains(const T & inValue) const {
		for (S32 index = this->startIndex; index < this->endIndex; ++index) {
			if (this->buffer[index] == inValue)
				return true;
		}
		return false;
	}

	/**
	 *@brief test if the vector is empty
	 *@return true if vector is empty
	 */
	bool empty() const {
		return this->startIndex == this->endIndex;
	}

	/**
	 *@brief test the last value
	 *@param inValue the value to test
	 *@return true if vector end with the value
	 */
	bool endsWith(const T & inValue) const {
		return !empty() && this->buffer[this->endIndex - 1] == inValue;
	}

	/**
	 *@brief test the first value
	 *@param inValue the value to test
	 *@return true if vector start with the value
	 */
	bool startsWith(const T & inValue) const {
		return !empty() && this->buffer[this->startIndex] == inValue;
	}

	/**
	 *@brief test the last value
	 *@param inValue the value to test
	 *@param inFrom the offset
	 *@return the position of the first found inValue or size if not found
	 */
	S32 indexOf(const T & inValue, S32 inFrom = 0) const {
		inFrom += this->startIndex;

		for (S32 index = this->startIndex; index < this->endIndex; ++index) {
			if (this->buffer[index] == inValue)
				return index - this->startIndex;
		}

		return size();
	}
	/**
	 *@brief insert a value at a certain position
	 *@param inPosition where to insert
	 *@param inValue the value to put
	 */
	void insert(S32 inPosition, T inValue);

	/**
	 *@brief test the last value
	 *@param inValue the value to test
	 *@param inFrom the offset
	 *@return the position of the last found inValue or size if not found
	 */
	S32 lastIndexOf(const T & inValue, S32 inFrom = 0) const;

	/**
	 *@brief create a subvector
	 *@param inPosition
	 *@param in_length
	 *@return the new vector
	 */
	NVector<T> subvector(S32 inPosition, S32 in_length = 0) const;

	/**
	 *@brief pop the last node
	 */
	void popBack() {
		if (this->endIndex != this->startIndex) {
			--this->endIndex;
			(&this->buffer[this->endIndex])->~T();
		}
	}
	/**
	 *@brief pop the first node
	 */
	void popFront() {
		if (this->endIndex != this->startIndex) {
			(&this->buffer[this->endIndex])->~T();
			++this->startIndex;
		}
	}

	/**
	 *@brief push a new node in back
	 *@param inValue the new value
	 */
	void pushBack(const T & inValue) {
		append(inValue);
	}

	/**
	 *@brief push a new node in front
	 *@param inValue the new value
	 */
	void pushFront(const T & inValue) {
		if (!this->startIndex) {
			setCapacity(this->capacity * 1.5);
		}
		--this->startIndex;

		new ((void*) &this->buffer[this->startIndex]) T(inValue);
	}

	/**
	 *@brief removeAll value occurence
	 *@param inValue the value to test
	 *@return the number of value delete
	 */
	S32 removeAll(const T & inValue);

	/**
	 *@brief remove a value at a position
	 *@param inPosition the position to delete
	 */
	void removeAt(S32 inPosition);

	/**
	 *@brief remove the first value
	 *@param inValue the value to delete
	 *@return true if something has been deleted
	 */
	bool removeOne(const T & inValue);

	/**
	 *@brief replace all value
	 *@param inValue the value to delete
	 *@return true if something has been deleted
	 */
	void replaceAll(const T & in_oldValue, const T & in_newValue) {
		for (S32 index = this->startIndex; index != this->endIndex; ++index) {
			if (this->buffer[index] == in_oldValue)
				this->buffer[index] == in_newValue;
		}
	}

	/**
	 *@brief replace a value to
	 *@param inPosition the position to replace
	 *@param in_newValue the new value
	 */
	void replace(S32 inPosition, const T & in_newValue) {
		inPosition += this->startIndex;
		if (inPosition < this->endIndex)
			this->buffer[inPosition] == in_newValue;
	}

	/**
	 *@brief swap to value
	 *@param i the first position
	 *@param j the second position
	 */
	void swap(S32 i, S32 j);

	/**
	 *@brief test if two vector are different
	 *@param other the vector to test
	 *@return true if different
	 */
	bool operator!=(const NVector<T> & other) const {
		return !(*this) == other;
	}

	/**
	 *@brief test if two vector are equal
	 *@param other the vector to test
	 *@return true if equal
	 */
	bool operator==(const NVector<T> & other) const;

	/**
	 *@brief concat two vector
	 *@param other the vector to add
	 *@return a new vector
	 */
	NVector<T> operator+(const NVector<T> & other) const {
		NVector<T> vec(*this);
		vec.append(other);
		return vec;
	}
	/**
	 *@brief add a vector
	 *@param other the vector to add
	 *@return a current vector
	 */
	NVector<T> & operator+=(const NVector<T> & other) {
		append(other);
		return *this;
	}
	/**
	 *@brief add a value
	 *@param inValue the value to add
	 *@return a current vector
	 */
	NVector<T> & operator+=(const T & inValue) {
		append(inValue);
		return *this;
	}
	/**
	 *@brief add a vector
	 *@param other the vector to add
	 *@return a current vector
	 */
	NVector<T> & operator<<(const NVector<T> & other) {
		append(other);
		return *this;
	}
	/**
	 *@brief add a value
	 *@param inValue the value to add
	 *@return a current vector
	 */
	NVector<T> & operator<<(const T & inValue) {
		append(inValue);
		return *this;
	}
	/**
	 *@brief set the vector from another
	 *@param other the vector to copy
	 *@return the current vector
	 */
	NVector<T> & operator=(const NVector<T> & other) {
		clear();
		append(other);
		return *this;
	}

	/**
	 *@class iterator
	 *@brief vector iterator
	 */
	class iterator {
	protected:
		NVector* const vec; /**< the vector to work on*/
		S32 index; /**< the current node*/

		/**
		 *@brief constructor
		 */
		iterator() :
				vec(NULL), index(0) {
		}
	public:
		/**
		 *@brief destructor
		 */
		~iterator() {
		}
		/**
		 *@brief constructor with node
		 *@param in_node the node
		 */													//changed from start
		iterator(NVector* inVec) :
				vec(inVec), index(inVec->startIndex) {
		}
		/**
		 *@brief constructor with node
		 *@param in_node the node
		 */
		iterator(NVector* inVec, const S32 inIndex) :
				vec(inVec), index(inIndex) {
		}
		/**
		 *@brief copy constructor
		 *@param inIter the iterator
		 */
		iterator(const iterator &inIter) :
				vec(inIter.vec), index(inIter.index) {
		}

		/**
		 *@brief copy opperator
		 *@param inIter the iterator
		 *@return this
		 */
		iterator &operator=(const iterator &inIter) {
			if (inIter.vec == this->vec) {
				this->index = inIter.index;
			}
			return *this;
		}

		/**
		 *@brief get data pointed by the operator
		 *@return data
		 */
		T& operator*() const {
			return this->vec->buffer[this->index];
		}
		/**
		 *@brief get adresse data pointed by the operator
		 *@return &data
		 */
		T* operator->() const {
			return &this->vec->buffer[this->index];
		}

		/**
		 *@brief test equality (if data are equal)
		 *@return true if equal
		 */
		bool operator==(const iterator &o) const {
			return this->vec == o.vec && this->index == o.index;
		}
		/**
		 *@brief test different (if data are different)
		 *@return true if different
		 */
		bool operator!=(const iterator &o) const {
			return this->vec != o.vec || this->index != o.index;
		}

		/**
		 *@brief progress in the vector
		 *@return the current iterator
		 */
		iterator& operator++() {
			++this->index;
			return *this;
		}
		/**
		 *@brief reverse progress in the vector
		 *@return the current iterator
		 */
		iterator& operator--() {
			--this->index;
			return *this;
		}

		/**
		 *@brief progress in the vector
		 *@return an iterator before progress
		 */
		iterator operator++(S32) {
			iterator iter(*this);
			++this->index;
			return iter;
		}
		/**
		 *@brief reverse progress in the vector
		 *@return an iterator before progress
		 */
		iterator operator--(S32) {
			iterator iter(*this);
			--this->index;
			return iter;
		}

		/**
		 *@brief test if it is at the begining of vector
		 *@return true if at the begining
		 */
		bool begin() {
			return this->vec && this->vec->start == this->index;
		}
		/**
		 *@brief test if it is at the end of vector
		 *@return true if at the end
		 */
		bool end() {
			return this->vec && this->vec->end == this->index;
		}

		/**
		 *@brief test if no data are pointed
		 *@return true not data are pointed
		 */
		//bool null() { return !(this->vec); }
		friend class NVector;
	};
	friend class iterator;

	/**
	 *@class const iterator
	 */
	class const_iterator: public iterator {
	protected:
		/**
		 *@brief constructor
		 */
		const_iterator() :
				iterator() {
		}
	public:
		/**
		 *@brief constructor with node
		 *@param in_node the node
		 */
		const_iterator(NVector<T> *inVec) :
				iterator(inVec) {
		}
		/**
		 *@brief constructor with node
		 *@param in_node the node
		 */
		const_iterator(NVector<T>* inVec, const S32 inIndex) :
				iterator(inVec, inIndex) {
		}
		/**
		 *@brief constructor with iterator
		 *@param inIter the iterator
		 */
		const_iterator(const iterator &inIter) :
				iterator(inIter) {
		}

		/**
		 *@brief destructor
		 */
		~const_iterator() {
		}

		/**
		 *@brief get data pointed by the operator
		 *@return data
		 */
		const T& operator*() const {
			return iterator::operator*();
		}
		/**
		 *@brief get adress data pointed by the operator
		 *@return &data
		 */
		const T* operator->() const {
			return iterator::operator*();
		}
	};
	friend class const_iterator;

	/**
	 *@brief get begin iterator
	 *@return iterator iter(_begin)
	 */
	iterator begin() {
		iterator iter(this, this->startIndex);
		return iter;
	}

	/**
	 *@brief get end iterator
	 *@return const_iterator iter(_begin)
	 */
	const_iterator constBegin() const {
		const_iterator iter(this, this->startIndex);
		return iter;
	}

	/**
	 *@brief get end iterator
	 *@return const_iterator iter(this->endIndex)
	 */
	const_iterator constEnd() const {
		const_iterator iter(this, this->endIndex);
		return iter;
	}

	/**
	 *@brief get end iterator
	 *@return iterator iter(this->endIndex)
	 */
	iterator end() {
		iterator iter(this, this->endIndex);
		return iter;
	}

	/**
	 *@brief erase the node pointed by an iterator
	 *@param inIter iterator
	 *@return new iterator
	 */
	iterator erase(iterator inIter);

	/**
	 *@brief erase the node pointed by an iterator
	 *@param inBegin begining of the deleted area
	 *@param inEnd ending  of the deleted area
	 *@return new iterator
	 */
	iterator erase(iterator inBegin, iterator inEnd);

	/**
	 *@brief insert a value after an iterator
	 *@param in_before where to insert
	 *@param inValue value to add
	 *@return new iterator
	 */
	iterator insert(iterator in_before, const T & inValue);

};

}

#include "NVector.cpp"

#endif /* NVECTOR_HPP_ */
