/*
 * NComSingle.cpp
 *
 *  Created on: 30.01.2014
 *      Author: Martin
 */

#include "NComSingle.hpp"

namespace nxpl {

U32 NComSingle::send(const NString &string, U8 idx) {
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

U32 NComSingle::send(U32 *package, U8 idx, U32 len) {
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

U32 NComSingle::send(S32 *package, U8 idx, U32 len) {
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


S32 NComSingle::getDataPackageU32(U32 *dataPack) {
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
S32 NComSingle::getDataPackageS32(S32 *dataPack) {
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

}

