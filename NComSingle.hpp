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
#include "NVector.hpp"

namespace nxpl {


inline unsigned char* num2Bytes (S32 num, unsigned char *pstart) {
	*pstart 	=  (num >> 24) & 0xFF;
	*(pstart+1) =  (num >> 16) & 0xFF;
	*(pstart+2) =  (num >> 8) & 0xFF;
	*(pstart+3) =   num & 0xFF;
	return (pstart+4);
}

inline unsigned char* num2Bytes (U32 num, unsigned char *pstart) {
	*pstart 	=  (num >> 24) & 0xFF;
	*(pstart+1) =  (num >> 16) & 0xFF;
	*(pstart+2) =  (num >> 8) & 0xFF;
	*(pstart+3) =   num & 0xFF;
	return (pstart+4);
}

inline unsigned char* bytes2Num (S32 &num, unsigned char *pstart) {
	num = (*pstart << 24) | (*(pstart+1) << 16) | (*(pstart+2) << 8) | (*(pstart+3));
	return (pstart+4);
}

inline unsigned char* bytes2Num (U32 &num, unsigned char *pstart) {
	num = (*pstart << 24) | (*(pstart+1) << 16) | (*(pstart+2) << 8) | (*(pstart+3));
	return (pstart+4);
}


class NComSingle {
private:
	NCom &com;

	unsigned char data[ecrobot::Usb::MAX_USB_DATA_LENGTH];
	S32 lastLen;

	union floatUnion_t {
	    float f;
	    unsigned char bytes[4];
	};

	void clearData() {
		memset(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
	}

public:
	NComSingle(NCom &ncom) : com(ncom), lastLen(0) {
		clearData();
	}
	~NComSingle() {}


	U32 send(bool b, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeBool;
		NCom::comNModes mode = NCom::modeSingle;

		data[NCom::data0ByteIdx] = static_cast<unsigned char>(b);
		U32 len = com.send(data, idx, type, mode, 1);
		memset(data, 0, NCom::headerOverhead+1);
		return len;
	}


	U32 send(U32 num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modeSingle;

		num2Bytes(num, &data[NCom::data0ByteIdx]);
		U32 len = com.send(data, idx, type, mode, 4);
		memset(data, 0, NCom::headerOverhead+4);
		return len;
	}

	U32 send(S32 num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeS32;
		NCom::comNModes mode = NCom::modeSingle;

		num2Bytes(num, &data[NCom::data0ByteIdx]);
		U32 len = com.send(data, idx, type, mode, 4);
		memset(data, 0, NCom::headerOverhead+4);
		return len;
	}

	U32 send(float num, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeFloat;
		NCom::comNModes mode = NCom::modeSingle;

		floatUnion_t fu;
		fu.f = num;

		memcpy(data+2, fu.bytes, 4);
		U32 len = com.send(data, idx, type, mode, 4);
		memset(data, 0, NCom::headerOverhead+4);
		return len;
	}

	U32 send(char ch, U8 idx = 0) {
			NCom::comDatatype type = NCom::typeChar;
			NCom::comNModes mode = NCom::modeSingle;

			data[NCom::data0ByteIdx] = static_cast<unsigned char>(ch);
			U32 len = com.send(data, idx, type, mode, 1);
			memset(data, 0, NCom::headerOverhead+1);
			return len;
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

		U32 len = com.send(data, idx, type, mode, string.size()+1); // +1 -> NUL \0 !
		memset(data, 0, NCom::headerOverhead+string.size()+1);
		return len;
	}

	/*
	U32 send(const NVector<U32> &vec, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modePackage;

		const S16 maxlen = (static_cast<S16>(NCom::MAX_DATA_LENGTH/sizeof(U32)) > vec.size()) ?
				vec.size() : NCom::MAX_DATA_LENGTH/sizeof(U32);

		unsigned char *pstart = &data[NCom::data0ByteIdx];

		for(S16 i=0; i < maxlen; ++i) {
			pstart = num2Bytes(vec.at(i), pstart);
		}
		U32 len = com.send(data, idx, type, mode, maxlen);
		memset(data, 0, NCom::headerOverhead+maxlen);
		return len;
	}
	*/

	U32 send(U32 *package, U8 idx = 0, U32 len = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modePackage;

		const S16 maxlen = (static_cast<S16>(NCom::MAX_DATA_LENGTH/sizeof(U32)) > len) ?
		len : NCom::MAX_DATA_LENGTH/sizeof(U32);

		unsigned char *pstart = &data[NCom::data0ByteIdx];

		for(S16 i=0; i < maxlen; ++i) {
			pstart = num2Bytes(*(package+i), pstart);
		}
		U32 retlen = com.send(data, idx, type, mode, (maxlen*4));
		memset(data, 0, NCom::headerOverhead+maxlen*4);
		return retlen;
	}


	U32 send(S32 *package, U8 idx = 0, U32 len = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modePackage;

		const S16 maxlen = (static_cast<S16>(NCom::MAX_DATA_LENGTH/sizeof(S32)) > len) ?
		len : NCom::MAX_DATA_LENGTH/sizeof(S32);

		unsigned char *pstart = &data[NCom::data0ByteIdx];

		for(S16 i=0; i < maxlen; ++i) {
			pstart = num2Bytes(*(package+i), pstart);
		}
		U32 retlen = com.send(data, idx, type, mode, (maxlen*4));
		memset(data, 0, NCom::headerOverhead+maxlen*4);
		return retlen;
	}


	// this puts next message into the buffer and gives information about message
	// user decides how to process data
	U32 receive(U8 &idx, NCom::comDatatype &datatype, NCom::comNModes &nmode) {
		lastLen = com.receive(data, idx, datatype, nmode);
		return lastLen;
	}

	// call this if you receive a message and you do nothing with it!
	void discard() {
		memset(data, 0, lastLen);
	}

	U32 getDataU32() {
		 U32 ret = 0;
		 bytes2Num(ret, &data[NCom::data0ByteIdx]);
		 memset(data, 0, 6);
		 return ret;
	}

	S32 getDataS32() {
		S32 ret = 0;
		bytes2Num(ret, &data[NCom::data0ByteIdx]);
		memset(data, 0, 6);
		return ret;
	}

	bool getDataBool() {
		bool ret = (data[NCom::data0ByteIdx] > 0) ? true : false;
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
		char ret = static_cast<char>(data[NCom::data0ByteIdx]);
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

	S32 getDataPackageU32(U32 *dataPack) {
		if(dataPack != NULL) return -1;
        S16 numPackage = (lastLen-NCom::headerOverhead)/4;
        if (numPackage <= 0) return numPackage;

        dataPack = new U32[numPackage];

        unsigned char *pstart = &data[NCom::data0ByteIdx];
        for(S16 i=0; i<numPackage; ++i) {
            U32 num = 0;
            pstart = bytes2Num(num, pstart);
            dataPack[i] = num;
        }
        return numPackage;
	}

	// TODO use smart pointer here!!!
	S32 getDataPackageS32(S32 *dataPack) {
		if(dataPack != NULL) return -1;
		S16 numPackage = (lastLen-NCom::headerOverhead)/4;
        if (numPackage <= 0) return numPackage;

        dataPack = new S32[numPackage]; // problem with new returning always NULL

        unsigned char *pstart = &data[NCom::data0ByteIdx];
        for(S16 i=0; i<numPackage; ++i) {
            S32 num = 0;
            pstart = bytes2Num(num, pstart);
            dataPack[i] = num;
        }
        return numPackage;
	}


	/*
	NVector<U32> getDataVectorU32() {
		//NVector<U32> ret(10);
	}
	*/

	// only for debugging, i know bad coding style ;)
	unsigned char * getDataRaw() {
		return &data[0];
	}
};

} /* namespace nxpl */

#endif /* NCOMSINGLE_HPP_ */
