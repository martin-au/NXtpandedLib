#ifndef __NSTRING_H_
#define __NSTRING_H_

/** \file
 *	\ingroup Utilities
*/

extern "C" {
	#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}


#include "StringConvert.hpp"
//#include "NOstream.hpp"

namespace nxpl {


// to do: replace, erase
/** \brief A basic, light and fast implementation of a C++ String.
 * \sa http://en.wikipedia.org/wiki/String_%28C%2B%2B%29
 */
class NString {
protected:
	char* string; /**< String Buffer*/
	S32 length; /**< String length*/
	S32 bufferlength; /**< Buffer length - as less string length*/

	U16 decplaces;
	// static const char HexChars[];

/*
	static const S32 BCharMaxlength;
	static const S32 BShortMaxlength;
	static const S32 BIntMaxlength;

	static const S32 BUCharMaxlength;
	static const S32 BUShortMaxlength;
	static const S32 BUIntMaxlength;
*/
	static const S32 BFloatMaxlength;

public:
	static const S32 npos = -1;

	/**
	 * @brief Default Constructor
	 */
	NString();

	/**
	 * @brief Constructor with buffer size
	 * @param inBufferlength The buffer size
	 * When you know approximately the buffer you will use
	 * this constructor is the more appropriate.
	 */
	explicit NString(S32 inBufferlength);

	/**
	 * @brief Copy Constructor
	 * @param inString The original string
	 */
	NString(const NString& inString);
	/**
	 * @brief Copy Constructor with char*
	 * @param inString The original string
	 */
	explicit NString(const char* const inString);

	/**
	 * @brief Destructor
	 */
	~NString() {
		if (string)
			delete[] string;
	}

	/**
	 * @brief String length
	 * @return length
	 */
	S32 size() const {
		return (this->length);
	}

	/**
	 * \brief Capacity of buffer.
	 * @return Capacity.
	 */
	S32 capacity() const {
		return (this->bufferlength);
	}

	/**
	 * @brief Clear string but keep buffer in memory
	 */
	void clear() {
		if (string && this->length) {
			this->string[0] = '\0';
		}
		this->length = 0;
	}

	/**
	 * @brief Returns whether the string is empty (i.e. whether its length is 0).
	 * @return true if the string length is 0, false otherwise.
	 */
	bool empty() const {
		return (this->length) ? (false) : (true);
	}

	/**
	 * @brief Get a character from the string
	 * @param in_index the char position
	 * @return the char or '?' if index too large or to negative.
	 */
	const char at(const S32 in_index) const {
		if (0 <= in_index && in_index < length)
			return string[in_index];;
		return '?';
	}

	// saver than a operator implementation!
	// we do not give away handle to internal data with at!
	/** \brief Copy character to position.
	 *
	 * @param in_index the character position
	 * @param c character to set.
	 * @return false if index too large or to negative.
	 */
	bool assign(const S32 in_index, char c) {
		if(in_index >= 0 && in_index < this->length) {
			this->string[in_index] = c;
			return true;
		}
		return false;
	}

	/** \brief Returns a newly constructed string object with its value initialized to a copy of a substring of this object.
     * <br>
     * The substring is the portion of the object that starts at character position pos and spans len characters (or until the end of the string, whichever comes first).
	 *
	 * @param pos Position of the first character to be copied as a substring.
	 * @param len Position of the first character to be copied as a substring. A value of NString::npos indicates all characters until the end of the string.
	 * @return A string object with a substring of this object or empty string if pos or len is out of range.
	 */
	NString substr(S32 pos = 0, S32 len = npos) const;

	/**\brief Inserts string into the string right before the character indicated by pos (or p):
	 *
	 * @param pos Insertion point: The new contents are inserted before the character at position pos.
	 * @param str Another string object.
	 * @return This string. There is no error checking -> if pos is out of range it will return a not changed string.
	 */
	NString& insert(S32 pos, const NString& str);

	// NString& erase(S32 pos = 0, S32 len = npos);

	// NString& replace(S32 pos, S32 len, const NString& str);

	/** \brief Searches the string for the first occurrence of str.
     * When pos is specified, the search only includes characters at or after position pos,
     * ignoring any possible occurrences that include characters before pos.
	 *
	 * @param str Another string with the subject to search for.
	 * @param pos Position of the first character in the string to be considered in the search.
	 * @return The position of the first character of the first match. If no matches were found, the function returns string::npos.
	 */
	S32 find(const NString& str, S32 pos = 0) const;

	/**
	 * @brief Check if there is enough free space, if not
	 * @brief allocate and copy string
	 * @param in_requeriedSpace the space that is needed
	 * @return true if space was allocate
	 */
	bool addBuffer(const S32 in_requeriedSpace);

	/** \brief Requests the string to reduce its capacity to fit its size.
	 */
	void shrinkToFit();

	/**
	 * @brief Append with C-String.
	 * @param inString The original string
	 */
	NString& append(const char * const inString);
	/**
	 * @brief Append with NString
	 * @param inString The original string
	 */
	NString& append(const NString& inString);
	/**
	 * @brief Append with char
	 * @param in_char The char
	 */
	NString& append(const char in_char) {
		addBuffer(1);                                // check size

		string[length + 1] = '\0';  	//this->string[length];           //
		string[length] = in_char;       // copy append char
		++length;                       // calcul new size

		return *this;
	}
	/**
	 * @brief Append with NString
	 * @param inString The original string
	 */
	const NString operator+(const NString& inString) const;
	/**
	 * @brief Append with C-String
	 * @param inString The original string
	 */
	const NString operator+(const char* const inString) const;
	/**
	 * @brief Get a char from the string.
	 *
	 * This functions do not checks for errors. Use NString::at().
	 * It should only be used for tested loops.
	 *
	 * @param in_index the char position
	 * @return The character at position in_index.
	 */
	const char NString::operator[](const S32 in_index) const {
		//if (0 <= in_index && in_index < length)
		return string[in_index];
	}

	/**
	 * @brief Set the string
	 * @param inString the new string
	 * @return the current string after modifying
	 */
	NString& operator=(const NString & inString);
	/**
	 * @brief Set the C-String
	 * @param inString the new string
	 * @return the current string after modifying
	 */
	NString& operator=(const char* const inString);
	/**
	 * @brief Test if 2 string are equals
	 * @param inString the string to test
	 * @return true if string are equal
	 */
	bool operator==(const NString& inString) const;
	/**
	 * @brief Test if 2 string are equals
	 * @param inString the string to test
	 * @return true if string are equal
	 */
	bool operator==(const char* const inString) const;

	/**
	 * @brief Test if 2 string are different
	 * @param inString the string to test
	 * @return true if string are different
	 */
	bool operator!=(const NString& inString) const {
		return !((*this) == inString);
	}

	/**
	 * @brief Get the buffer. (C-String)
	 *
	 * This should be used only if a function needs a c-string.
	 *
	 * @return The string buffer.
	 */
	const char *data() const {
		return this->string;
	}

	/**
	 * @brief Count the length of a string
	 * The method count until '\0'
	 * @param inString the string to test
	 * @return the length
	 */
	static S32 strlen(const char* const inString);

	/**
	 * @brief Copy source string to destination string.
	 * The method copy until '\0'
	 * @param in_src the source
	 * @param in_dest the destination
	 * @return the length of the copy
	 */
	static S32 copy(const char* const in_src, char* const in_dest);

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return The resulting string.
	 */
	NString& operator+=(const NString& inString) {
		append(inString);
		return *this;
	}

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return the resulting string.
	 */
	NString& operator+=(const char* inString) {
		append(inString);
		return *this;
	}

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return the resulting string.
	 */
	NString& operator+=(const char inString) {
		append(inString);
		return *this;
	}

	/** \brief Put NString into ostream.
	 *
	 * @param os
	 * @param str
	 * @return
	 */
	/*
	friend NOstream& operator<<(NOstream &os, const NString& str) {
		os << str.data();
		return os;
	}
	*/

	//******************************************************************************
	// Converter for numbers -> should make things easier

	/** @brief Append a value to the current string */
	NString& append(const S16 in_value) {
		if (in_value == 0)
			append('0');
		else {
			addBuffer(numDigits(static_cast<S32>(in_value)));
			length += numToStr(static_cast<S32>(in_value), this->string + length);
		}
		return *this;
	}
	/** @brief Append a value to the current string */
	NString& append(const U16 in_value) {
		if (in_value == 0u)
			append('0');
		else {
			addBuffer(numDigits(static_cast<U32>(in_value)));
			length += numToStr(static_cast<U32>(in_value), this->string + length);
		}
		return *this;
	}
	/** @brief Append a value to the current string */
	NString& append(const S32 in_value) {
		if (in_value == 0)
			append('0');
		else {
			addBuffer(numDigits(in_value));
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}

	/** @brief Append a value to the current string */
	NString& append(const int in_value) {
		append(static_cast<S32>(in_value));
		return *this;
	}

	/** @brief Append a value to the current string */
	NString& append(const U32 in_value) {
		if (in_value == 0u)
			append('0');
		else {
			addBuffer(numDigits(in_value));
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}


    /** @brief Append a value to the current string and show as hex*/
	NString& appendHex(const U32 in_value) {
		if (in_value == 0u)
			append("0x00");
		else {
			addBuffer(6); // 4 byte + 2 ?
			length += numToHex(in_value, this->string + length);
		}
		return *this;
	}


    /** @brief Append a value to the current string
     *
     *  Converting a float into a string is not really space efficient.
     *  It will allocate the maximum possible length of a float string: NString::BFloatMaxlength
     *  You may use NString::shrinkToFit() after this function to save memory.
     */
	NString& append(const float in_value) {
		if (in_value < 0.0000001 && in_value > -0.0000001)
			append("0.0");
		else {
			addBuffer(BFloatMaxlength); // float is really inefficient ...
			length += numToStr(in_value, this->string + length, decplaces);
		}
		return *this;
	}

	/** \brief Set floating-point-precision
	 *
	 * @param places
	 */
	void setPrecision(U16 places) {
		decplaces = places;
	}
};

}

#endif
