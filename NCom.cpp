/*
 * NCom.cpp
 *
 *  Created on: 02.03.2014
 *      Author: Martin
 */


#include "NCom.hpp"

namespace nxpl {

U32 NCom::send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode, U32 length) {
	header mHeader;
	mHeader.disconReq = noDisconnect;
	mHeader.datatype = datatype;
	mHeader.mode = nmode;

	data[0] = mHeader.asU8;
	data[1] = idx;
	return com.send(data, 0, headerOverhead+length);
}


U32 NCom::receive(U8 *data, U8 &idx, comDatatype &datatype, comNModes &nmode) const {
	U32 len = 0;
	len = com.receive(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
	if(len > 0) {
		header mHeader;
		mHeader.asU8 = data[0];
		idx = data[1];

		if (mHeader.disconReq == disconnect) {
			com.close();
			return 0;
		}
		datatype = static_cast<comDatatype>(mHeader.datatype);
		nmode = static_cast<comNModes>(mHeader.mode);
	}
	return len;
}

}
