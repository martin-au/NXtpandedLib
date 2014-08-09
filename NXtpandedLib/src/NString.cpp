
#include "NString.hpp"

namespace nxpl {

NString::NString()
: string(0), length(0), bufferlength(0), decplaces(2)
{
}

NString::NString(S32 inBufferlength)
: length(0), decplaces(2)
{
	if (inBufferlength > 0) {                  // buffer size > 0
		this->string = new char[inBufferlength];              // allocate buffer
		this->bufferlength = inBufferlength;
		this->string[0] = '\0';                   // set first char \0
	} else {
		this->string = 0;                     // else NULL
		this->bufferlength = 0;
	}
	// this->bufferlength = inBufferlength;         // Copy buffer length
	//this->length = 0;                     		// str length = 0
}


  NString::NString(const NString& inString)
  : decplaces(2)
  {
	if (inString.length) {                    // if original string is not empty
		this->length = inString.length;
		this->bufferlength = inString.length + 1;
		this->string = new char[bufferlength];                 //allocate buffer
		NString::copy(inString.string, string);          // copy and save size
	} else {
		this->string = 0;                     // set to zeros
		this->length = 0;                     //
		this->bufferlength = 0;                   //
	}
}


  NString::NString(const char* const inString)
  : decplaces(2)
  {
	S32 inlength;

	if ((inlength = NString::strlen(inString))) { // if original string is not empty
		this->length = inlength;
		this->bufferlength = inlength + 1;
		this->string = new char[bufferlength];        //allocate buffer
		NString::copy(inString, string);                        // copy and save size
	} else {
		this->string = 0;                                 // set to zeros
		this->length = 0;                                 //
		this->bufferlength = 0;                           //
	}
}


// Max length as string but without the 0-termination

/*
const S32 NString::BCharMaxlength = 3;
const S32 NString::BShortMaxlength = 6;
const S32 NString::BIntMaxlength = 11;

const S32 NString::BUCharMaxlength = 3;
const S32 NString::BUShortMaxlength = 5;
const S32 NString::BUIntMaxlength = 10;
*/
const S32 NString::BFloatMaxlength = 15;


bool NString::addBuffer(const S32 inRequeriedSpace) {
	if (bufferlength <= inRequeriedSpace + length) { // If the buffer is too small
		bufferlength = 2 * (length + inRequeriedSpace); // calcul requierd buffer
		char * str = new char[bufferlength];                // allocate buffer

		if(!str) return false;

		if (string) {
			NString::copy(string, str);                  // copy original string
			delete (string);                             // remove buffer
		} else {
			str[0] = '\0';
		}
		string = str;
		return true;
	}
	return false;
}

void NString::shrinkToFit() {
	char * str = new char[size() + 1];
	if (str) {
		bufferlength = size() + 1;
		if (string) {
			NString::copy(string, str);                  // copy original string
			delete[] string;
		} else {
			str[0] = '\0';
		}
		string = str;
	}
}


NString NString::substr(S32 pos, S32 len) const {
	if (pos < 0 || pos >= this->length) {
		// not use throw on nxt
		NString err("");
		return err;
	}
	S32 end;
	if (len == npos) {
		end = (this->length);
	} else {
		end = pos + len;
		if (end >= this->length)
			end = (this->length);
	}

	NString sub(end - pos + 1);

	S32 j = 0;
	for (S32 i = pos; i < end; i++) {
		sub.string[j] = this->string[i];
		j++;
	}
	sub.string[j] = '\0';
	sub.length = end - pos;
	return sub;
}




NString &NString::insert(S32 pos, const NString &str) {
	// not use throw
	if (pos < 0 || pos >= this->length)
		return *this;
	addBuffer(str.length);

	this->length += str.length;
	S32 i = this->length;
	this->string[i] = '\0';
	--i;

	// push back
	S32 endstr = pos + str.length - 1;
	for (; i > endstr; --i) {
		this->string[i] = this->string[i - str.length];
	}

	i = 0;
	// insert
	for (; i < str.length; ++i) {
		this->string[pos + i] = str.string[i];
	}

	return *this;
}

// pos = 0
S32 NString::find(const NString& str, S32 pos) const {
	if (str.length > (this->length - pos) || str.length <= 0) {
		return npos;
	}
	S32 i = pos; 			  // Textiterator
	S32 j = str.length - 1;   // Wortiterator
	while (i <= (this->length - str.length)) {
		while (j >= 0 && str.string[j] == this->string[i + j]) {
			if (j == 0)
				return i;
			j--;
		}
		j = str.length - 1;
		i++;
	}
	return npos;
}


NString& NString::append(const char * const inString) {
	if (!inString)
		return *this;                     // Test if null

	const S32 sl = NString::strlen(inString);       // Check str length

	addBuffer(sl);

	NString::copy(inString, &string[length]);    // copy append string
	length += sl;                               // calcul new size

	return *this;
}


NString& NString::append(const NString& inString) {
	addBuffer(inString.length);
	NString::copy(inString.string, &string[length]); // copy append string
	length += inString.length;                      // calcul new size

	return *this;
}





const NString NString::operator+(const NString& inString) const {
	NString concat(length + inString.length + 1); // create a string with big buffer

	NString::copy(string, concat.string);
	NString::copy(inString.string, &concat.string[length]);
	concat.length = length + inString.length;

	return concat;
}

const NString NString::operator+(const char* const inString) const {
	const S32 stringlength = NString::strlen(inString);
	NString concat(length + stringlength + 1); // create a string with big buffer

	NString::copy(string, concat.string);                // add currrent string
	NString::copy(inString, &concat.string[length]); // add 2nd string
	concat.length = length + stringlength;

	return concat;
}

NString& NString::operator=(const NString & inString) {
	// self assigment save -> lenght == bufferlengt?
	if (inString.length >= bufferlength) {     // check if buffer is enougth big
		if (string)
			delete (string);                  // if not delete
		bufferlength = inString.length + 1;         // allocate buffer
		string = new char[bufferlength];
	}
	NString::copy(inString.string, string);          // copy string
	length = inString.length;                       // save length
	return *this;
}

NString& NString::operator=(const char* const inString) {
	if (!inString) {
		if (string)
			this->string[0] = '\0';
		length = 0;
		return *this;
	}
	const S32 sl = NString::strlen(inString);
	if (sl >= bufferlength) {
		if (string)
			delete (string);
		bufferlength = sl + 1;
		string = new char[bufferlength];
	}

	NString::copy(inString, string);
	length = sl;

	return *this;
}


bool NString::operator==(const NString& inString) const {
	if (length != inString.length
			|| (length && this->string[0] != inString.string[0]))
		return false;

	for (S32 indexString = 0; indexString < length; ++indexString) {
		if (string[indexString] != inString.string[indexString])
			return false;
	}

	return true;
}

bool NString::operator==(const char* inString) const {
	if (length != NString::strlen(inString)
			|| (length && this->string[0] != inString[0]))
		return false;

	for (S32 indexString = 0; indexString < length; ++indexString) {
		if (string[indexString] != inString[indexString])
			return false;
	}

	return true;
}

S32 NString::strlen(const char* const inString) {
	S32 indexString = 0;
	if (inString) {
		while (inString[indexString] != '\0')
			++indexString;
	}
	return indexString;
}


S32 NString::copy(const char* const in_src, char* const in_dest) {
	if (!in_dest)
		return 0;
	if (!in_src) {
		in_dest[0] = '\0';
		return 0;
	}

	S32 indexString = 0;

	while (in_src[indexString] != '\0') {
		in_dest[indexString] = in_src[indexString];
		++indexString;
	}
	in_dest[indexString] = in_src[indexString];

	return indexString;
}

}
