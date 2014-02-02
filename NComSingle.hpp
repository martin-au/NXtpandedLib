/*
 * NComSingle.hpp
 *
 *  Created on: 30.01.2014
 *      Author: Martin
 */


#ifndef __NCOMSINGLE_HPP_
#define __NCOMSINGLE_HPP_

#include "NCom.hpp"

#include "NString.hpp"

namespace Com {

class NComSingle {
private:
	NCom &com;

	unsigned char data[ecrobot::Usb::MAX_USB_DATA_LENGTH];

	union floatUnion_t {
	    float f;
	    unsigned char bytes[4];
	};

public:
	NComSingle(NCom &ncom) : com(ncom) {
		clear();
	}
	~NComSingle() {}


	U32 send(bool b, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeBool;
		NCom::comNModes mode = NCom::modeSingle;

		data[2] = static_cast<unsigned char>(b);
		return com.send(data, idx, type, mode);
	}


	U32 send(U32 num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modeSingle;

		data[2] =  (num >> 24) & 0xFF;
		data[3] =  (num >> 16) & 0xFF;
		data[4] =  (num >> 8) & 0xFF;
		data[5] =  num & 0xFF;
		return com.send(data, idx, type, mode);
	}

	U32 send(S32 num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeS32;
		NCom::comNModes mode = NCom::modeSingle;

		data[2] =  (num >> 24) & 0xFF;
		data[3] =  (num >> 16) & 0xFF;
		data[4] =  (num >> 8) & 0xFF;
		data[5] =  num & 0xFF;
		return com.send(data, idx, type, mode);
	}

	U32 send(float num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeFloat;
		NCom::comNModes mode = NCom::modeSingle;

		floatUnion_t fu;
		fu.f = num;

		memcpy(data+2, fu.bytes, 4);
		return com.send(data, idx, type, mode);
	}

	U32 send(char ch, U8 idx = 0) {
			NCom::comDatatype type = NCom::typeChar;
			NCom::comNModes mode = NCom::modeSingle;

			data[2] = static_cast<unsigned char>(ch);
			return com.send(data, idx, type, mode);
	}

	U32 send(const NString &string, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeString;
		NCom::comNModes mode = NCom::modeSingle;

		const char *str = string.data();

		if(string.size()+1 <= NCom::MAX_DATA_LENGTH) {
			memcpy((data+2), str, string.size()+1);
		} else {
			data[ecrobot::Usb::MAX_USB_DATA_LENGTH - 1] = static_cast<unsigned char>('\0');
			memcpy((data+2), str, NCom::MAX_DATA_LENGTH-1);
		}

		return com.send(data, idx, type, mode);
	}


	// this puts next message into the buffer and gives information about message
	// user decides how to process data
	U32 receive(U8 &idx, NCom::comDatatype &datatype, NCom::comNModes &nmode) {
		return com.receive(data, idx, datatype, nmode);
	}

	U32 getDataU32() {
		 U32 ret = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5]);
		 memset(data, 0, 6);
		 return ret;
	}

	S32 getDataS32() {
		S32 ret = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5]);
		memset(data, 0, 6);
		return ret;
	}

	bool getDataBool() {
		bool ret = (data[2] > 0) ? true : false;
		memset(data, 0, 3);
		return ret;
	}

	float getDataFloat() {
		floatUnion_t fu;
		memcpy(fu.bytes, data+2, 4);
		float ret = fu.f;
		memset(data, 0, 6);
		return ret;
	}

	char getDataChar() {
		char ret = static_cast<char>(data[2]);
		memset(data, 0, 3);
		return ret;
	}

	NString getDataString() {
		// TODO Dynamic buffer not 20!
		NString ret(20);
		for(S16 i=2; ;++i) {
            char ch = static_cast<char>(data[i]);
            if(ch == '\0') break;
            ret += ch;
        }
        memset(data, 0, 2+1+ret.size());
        return ret;
	}

	// only for debugging, i know bad coding style ;)
	unsigned char * getDataRaw() {
		return &data[0];
	}

	void clear() {
		memset(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
	}
};

} /* namespace Com */
#endif /* NCOMSINGLE_HPP_ */
