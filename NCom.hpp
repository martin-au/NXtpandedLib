/*
 * NCom.hpp
 *
 *  Created on: 30.01.2014
 *      Author: Martin
 */

#ifndef __NCOM_HPP_
#define __NCOM_HPP_

/** \file
 *	\ingroup Communication
*/

#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Usb.h"


namespace nxpl {

/** \brief Handler-Class implementing communication with message information.
 *
 * The class specifies how the message header is implemented.
 * It's used that the receiver always knows which message belongs to some code.
 * This is really useful in a system with multiple tasks where message fly around at different times/positions!
 *
 * You do not have to use this class if high level class NComSingle fits your needs!
 */
class NCom {
private:
	ecrobot::Usb &com; // ref because usb objects must be global

	typedef union header {
		struct {
			U8 disconReq :1;
			U8 datatype :4;
			U8 mode :3;
		};
		U8 asU8;
	};

public:
	static const S32 headerByteIdx = 0; /**<The index of the info-header byte in the message-data array. Never write on this byte!*/
	static const S32 idxByteIdx = 1;    /**<The index of the userIdx-header byte in the message-data array.*/
	static const S32 data0ByteIdx = 2;  /**<The index of the first data (payload) byte in the message-data array.*/
	static const U8 headerOverhead = 2; /**<The number of bytes used for header-data*/
	static const U8 MAX_DATA_LENGTH = ecrobot::Usb::MAX_USB_DATA_LENGTH - headerOverhead; /**<Max length of payload data in bytes*/

	enum disconReq {
		noDisconnect, /**<Not disconnect connection*/
		disconnect    /**<Disconnect request for the receiver*/
	};
	/**
	 * \brief Datatypes to send available with nxtOSEK.
	 *
	 * This specifies how the data looks like and in which type it should be converted.
	 * The numbers from 7 to 15 are free for future.
	 */
	enum comDatatype {
		typeUnspec = 0, /**<Unspecified type, may be a error*/
		typeU32 = 	 1, /**<Unsigned 32bit arithmetical type*/
		typeS32 = 	 2, /**<Signed 32bit arithmetical type*/
		typeBool = 	 3, /**<Boolean type*/
		typeFloat =	 4, /**<32bit floating-point number*/
		typeChar = 	 5, /**<8bit signed char*/
		typeString = 6  /**<C-String type*/
		// free 7 - 15
	};
	/**
	 * \brief The communication modes available in NXtpandedLib.
	 *
	 * This specifies how the message should be handled by the the receiver.
	 * 5-6 are free for future implementations.
	 */
	enum comNModes {
		modeBasic = 	0, /**<raw data, may be error*/
		modeSingle = 	1, /**<Send/receive single variable @sa NComSingle*/
		modePackage = 	2, /**<Send/receive arrays, vectors, packed data/classes @sa NComSingle*/
		modePart = 		3, /**<Send/receive big package/string sending in parts*/
		modeStream = 	4  /**<Direct stream to pc ostrem fstream ...*/
		//free = 		5  //
		//free = 		6  //
		//free = 		7  //
	};

	/**
	 * \brief Construct a communication handler.
	 * @param comObject A global constructed ecrobot::usb object.
	 */
	NCom(ecrobot::Usb &comObject) : com(comObject) {}
	~NCom() {}

	/** \brief Send data with info-header.
	 *
	 * Data should be array with capacity > NCom::headerOverhead.
	 * Remind that you are not allowed to use the first two bytes. They will be overwritten by the handler!
	 * The user may use the idx parameter to make the message unique so that the receiver knows what to do with this message.
	 *
	 * @param data The data to send including the two bytes for header.
	 * @param idx  Special user message identifier between 0 and 255.
	 * @param datatype The data type of the encoded data.
	 * @param nmode  The communication modes available in NXtpandedLib.
	 * @param length Length of the payload data in bytes.
	 * @return Length of sent data.
	 */
	U32 send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode = modeBasic, U32 length = ecrobot::Usb::MAX_USB_DATA_LENGTH);

	/** \brief Receive data with info-header.
	 *
	 * This function takes the data out of the in-queue. If there is no message in the in-queue it will return a length <= 0.
	 *	The user may use the idx parameter to make the message unique so that the receiver knows what to do with this message.
	 *
	 * @param data The received data including the two bytes for header. Make sure there is enough space for the data!
	 * @param idx  Special user message identifier between 0 and 255.
	 * @param datatype The data type of the encoded data.
	 * @param nmode The communication modes available in NXtpandedLib.
	 * @return Length of received data.
	 */
	U32 receive(U8 *data, U8 &idx, comDatatype &datatype, comNModes &nmode) const;
};


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

} /* namespace nxpl */
#endif /* __NCOM_HPP_ */
