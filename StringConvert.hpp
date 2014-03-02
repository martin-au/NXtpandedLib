
#ifndef __CONVERT_H_
#define __CONVERT_H_

/** \file
 *	\ingroup Utilities
*/

extern "C" {
#include "../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../ecrobot/c/ecrobot_interface.h"
}

namespace nxpl {

// the functions should be used to calculate the resulting string size before
// actual converting. Remember to let space for the '\0' char!!
/**
 * \brief Get number of digits in a number.
 * The sign is treated as digit.
 * @param num The number to test.
 * @return Number of digits + sign.
 */
inline S8 numDigits(S32 num) {
	S8 cnt= (num < 0) ? 1 : 0;
	for(; num; num /= 10, cnt++);
	return cnt;
}

/**
 * \brief Get number of digits in a number.
 * @param num The number to test.
 * @return Number of digits.
 */
inline S8 numDigits(U32 num) {
	S8 cnt= 0;
	for(; num; num /= 10, cnt++);
	return cnt;
}

/**\brief Convert number to string.
 * The size of destination string must be big enough.
 * 12 chars should be enough for all 32 bit types.
 * You can pre calculate the resulting string size with nxpl::numDigits()+1 for nulltermination.
 *
 * @param value Number to convert.
 * @param dest Destination string.
 * @return Size of resulting string.
 */
S8 numToStr(S32 value, char* dest);

/**\brief Convert number to string.
 * The size of destination string must be big enough.
 * 12 chars should be enough for all 32 bit types.
 * You can pre calculate the resulting string size with nxpl::numDigits()+1 for nulltermination.
 *
 * @param value Number to convert.
 * @param dest Destination string.
 * @return Size of resulting string.
 */
S8 numToStr(U32 value, char* dest);

/**\brief Convert floating point number to string.
 * The size of destination string must be big enough.
 * Max length of biggest float is 16 chars.
 * @param f Number to convert.
 * @param dest Destination string.
 * @param places The floating-point-precision.
 * @return Size of resulting string.
 */
S8 numToStr(float f, char* dest, U8 places = 2);

/**\brief Convert number to hex string.
 * The size of destination string must be big enough.
 * 8 chars should be enough for all 32 bit types.
 *
 * @param value Number to convert.
 * @param dest Destination string.
 * @return Size of resulting string.
 */
S8 numToHex(U32 value, char* dest);




}

#endif // __CONVERT_H_
