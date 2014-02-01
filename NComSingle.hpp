/*
 * NComSingle.hpp
 *
 *  Created on: 30.01.2014
 *      Author: Martin
 */


#ifndef __NCOMSINGLE_HPP_
#define __NCOMSINGLE_HPP_

#include "NCom.hpp"

namespace Com {

class NComSingle {
private:
	NCom &com;

	unsigned char data[ecrobot::Usb::MAX_USB_DATA_LENGTH];
public:
	NComSingle(NCom &ncom) : com(ncom) {
		clear();
	}
	~NComSingle() {}


	U32 send(U32 n, U8 idx = 0) {
		NCom::comDatatype type = NCom::typeU32;
		NCom::comNModes mode = NCom::modeSingle;

		data[2] =  (n >> 24) & 0xFF;
		data[3] =  (n >> 16) & 0xFF;
		data[4] =  (n >> 8) & 0xFF;
		data[5] =  n & 0xFF;
		return com.send(data, idx, type, mode);
	}

	// this puts next message into the buffer and gives information about message
	// user decides how to process data
	U32 receive(U8 &idx, NCom::comDatatype &datatype, NCom::comNModes &nmode) {
		return com.receive(data, idx, datatype, nmode);
	}

	U32 getData() {
		 U32 ret = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5]);
		 memset(data, 0, 6);
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
