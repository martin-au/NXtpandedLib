/*
 * NCom.hpp
 *
 *  Created on: 30.01.2014
 *      Author: Martin
 */

#ifndef __NCOM_HPP_
#define __NCOM_HPP_

#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Usb.h"


namespace Com {

class NCom {
private:
	ecrobot::Usb &com; // only ref because global

	typedef union header {
		struct {
			U8 disconReq :1;
			U8 datatype :4;
			U8 mode :3;
		};
		U8 asU8;
	};

public:
	enum disconReq {
		noDisconnect, disconnect
	};
	enum comDatatype {
		typeUnspec = 0,
		typeU32 = 	 1,
		typeS32 = 	 2,
		typeBool = 	 3,
		typeFloat =	 4,
		typeChar = 	 5,
		typeString = 6
	};
	enum comNModes {
		modeBasic = 	0,
		modeSingle = 	1,
		modePackage = 	2,
		modePart = 		3,
		modeStream = 	4
	};

	static const U8 MAX_DATA_LENGTH = ecrobot::Usb::MAX_USB_DATA_LENGTH - 2;

	NCom(ecrobot::Usb &comObject) : com(comObject) {}
	~NCom() {}

	U32 send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode = modeBasic);
	U32 receive(U8 *data, U8 &idx, comDatatype &datatype, comNModes &nmode) const;
};


U32 NCom::send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode) {
	header mHeader;
	mHeader.disconReq = noDisconnect;
	mHeader.datatype = datatype;
	mHeader.mode = nmode;

	data[0] = mHeader.asU8;
	data[1] = idx;
	return com.send(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
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

} /* namespace Com */
#endif /* NCOM_HPP_ */
