#ifndef __NSTRING_H_
#define __NSTRING_H_

extern "C" {
	#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

//for codecorrection
#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"

#include "convert.hpp"
#include "ostream.hpp"


///******************************************************************************
// string!
// to do: replace, erase
class NString {
protected:
	char* string; /**< String Buffer*/
	S32 length; /**< String length*/
	S32 bufferlength; /**< Buffer length - as less string length*/

	U16 decplaces;
	// static const char HexChars[];

	/** MAx length S32 a print version */
	static const S32 BCharMaxlength; /**< BChar Max length in string format		*/
	static const S32 BShortMaxlength; /**< BShort Max length in string format		*/
	static const S32 BIntMaxlength; /**< BInt Max length in string format		*/

	static const S32 BUCharMaxlength; /**< BUChar Max length in string format		*/
	static const S32 BUShortMaxlength; /**< BUShort Max length in string format	*/
	static const S32 BUIntMaxlength; /**< BUInt Max length in string format		*/

	static const S32 BFloatMaxlength; /**< BFloat Max length in string format  	*/

public:
	static const S32 npos = -1;
	/**
	 * @brief Default Constructor
	 */
	NString();
	/**
	 * @brief Constructor with buffer size
	 * @param inBufferlength The buffer size
	 * When you know approximatly the buffer you will use
	 * this constructor is the more appropriate
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
	~NString() { // !deleted virtual
		if (string)
			delete (string);
	}

	/**
	 * @brief String length
	 * @return length
	 */
	S32 size() const {
		return (this->length);
	}

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
	 * @brief Get a char from the string
	 * @param in_index the char position
	 * @return the char or BCharMax if index too large
	 */
	char at(const S32 in_index) const {
		return operator[](in_index);
	}

	// saver than a operator implementation!
	void assign(const S32 in_index, char c) {
		if(in_index >= 0 && in_index < this->length) {
			this->string[in_index] = c;
		}
	}

	NString substr(S32 pos = 0, S32 len = npos) const;

	NString& insert(S32 pos, const NString& str);

	// NString& erase(S32 pos = 0, S32 len = npos);

	// NString& replace(S32 pos, S32 len, const NString& str);

	S32 find(const NString& str, S32 pos = 0) const;

	/**
	 * @brief Check if there is enough free space, if not
	 * @brief allocate and copy string
	 * @param in_requeriedSpace the space that is needed
	 * @return true if space was allocate
	 */
	bool addBuffer(const S32 in_requeriedSpace);
	/**
	 * @brief Append with char*
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
	NString& append(const char in_char);
	/**
	 * @brief Append with NString
	 * @param inString The original string
	 */
	const NString operator+(const NString& inString) const;
	/**
	 * @brief Append with NString
	 * @param inString The original string
	 */
	const NString operator+(const char* const inString) const;
	/**
	 * @brief Get a char from the string
	 * @param in_index the char position
	 * @return the char or BCharMax if index too large
	 */
	char operator[](const S32 in_index) const;

	/**
	 * @brief Set the string
	 * @param inString the new string
	 * @return the current string after modifying
	 */
	NString& operator=(const NString & inString);
	/**
	 * @brief Set the string
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
	 * @brief To get the buffer
	 * @return the string buffer of char
	 */
	const char *data() const {
		return this->string;
	}

	/**
	 * @brief Count the length of a string
	 * @param inString the string to test
	 * @return the length
	 * The method count until '\0'
	 */
	static S32 strlen(const char* const inString);

	/**
	 * @brief Count the length of a string
	 * @param in_src the source
	 * @param in_dest the destination
	 * @return the length of the copy
	 * The method copy until '\0'
	 */
	static S32 copy(const char* const in_src, char* const in_dest);

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return the string concat in in_string1
	 */
	NString& operator+=(const NString& inString) {
		append(inString);
		return *this;
	}

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return the string concat in in_string1
	 */
	NString& operator+=(const char* inString) {
		append(inString);
		return *this;
	}

	/**
	 * @brief Add a string to another
	 * @param inString the string to add
	 * @return the string concat in in_string1
	 */
	NString& operator+=(const char inString) {
		append(inString);
		return *this;
	}

	friend ostream & operator<<(ostream &os, const NString& str) {
		os << str.data();
		return os;
	}

	//******************************************************************************
	// Converter for numbers -> should make things easier

	/** @brief Append a value to the current string */
	NString& append(const S16 in_value) {
		if (in_value == 0)
			append('0');
		else {
			addBuffer(BShortMaxlength);
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}
	/** @brief Append a value to the current string */
	NString& append(const U16 in_value) {
		if (in_value == 0u)
			append('0');
		else {
			addBuffer(BUShortMaxlength);
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}
	/** @brief Append a value to the current string */
	NString& append(const S32 in_value) {
		if (in_value == 0)
			append('0');
		else {
			addBuffer(BIntMaxlength);
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}
	/** @brief Append a value to the current string */
	NString& append(const U32 in_value) {
		if (in_value == 0u)
			append('0');
		else {
			addBuffer(BUIntMaxlength);
			length += numToStr(in_value, this->string + length);
		}
		return *this;
	}


    /** @brief Append a value to the current string */
	NString& appendHex(const U32 in_value) {
		if (in_value == 0u)
			append("0x00");
		else {
			addBuffer(6); // 4 byte + 2 ?
			length += numToHex(in_value, this->string + length);
		}
		return *this;
	}


    /** @brief Append a value to the current string */
	NString& append(const float in_value) {
		if (in_value < 0.0000001 && in_value > -0.0000001)
			append("0.0");
		else {
			addBuffer(BFloatMaxlength); // Inefficient with NNumIndicator!
			length += numToStr(in_value, this->string + length, decplaces);
		}
		return *this;
	}

	void setPrecision(U16 places) {
		decplaces = places;
	}
};


#include "NString.cpp"

#endif
