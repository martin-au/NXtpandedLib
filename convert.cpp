
#include "convert.hpp"

// TODO digit global in namespace!
// TODO test lenght calculation with

//dest must have 12 chars
S8 numToStr(S32 value, char* dest) {
	static char const digit[] = "0123456789";
	char *p = dest;
	U8 length = 0;

	if (value < 0) {
		*p++ = '-';
		//length++;
		value *= -1;
	}
	S32 shifter = value;
	do { //Move to where representation ends
		++p;
		//length++;
		shifter = shifter / 10;
	} while (shifter);
	length = p - dest;
	*p = '\0';
	do { //Move back, inserting digits as u go
		*--p = digit[value % 10];
		value = value / 10;
	} while (value);
	return length;
}


S8 numToStr(U32 value, char* dest) {
	static char const digit[] = "0123456789";
	char *p = dest;
	U8 length = 0;

	U32 shifter = value;
	do { //Move to where representation ends
		++p;
		length++;
		shifter = shifter / 10;
	} while (shifter);
	*p = '\0';
	do { //Move back, inserting digits as u go
		*--p = digit[value % 10];
		value = value / 10;
	} while (value);
	return length;
}


// for float to char*
typedef union {
	long L;
	float F;
} LF_t;

S8 numToStr(float f, char* dest, U8 places) {
	S32 mantissa, int_part, frac_part;
	S16 exp2;
	LF_t x;
	char *p;
	//static char outbuf[15];

	if (f == 0.0) {
		dest[0] = '0';
		dest[1] = '.';
		dest[2] = '0';
		dest[3] = 0;
		return 4;
	}
	x.F = f;

	exp2 = (unsigned char) (x.L >> 23) - 127;
	mantissa = (x.L & 0xFFFFFF) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 23)
		int_part = mantissa << (exp2 - 23);
	else if (exp2 >= 0) {
		int_part = mantissa >> (23 - exp2);
		frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
	} else
		/* if (exp2 < 0) */
		frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);

	p = &(dest[0]);

	if (x.L < 0) {
		*p++ = '-';
	}

	if (int_part == 0) {
		*p++ = '0';
	}
	else {
		numToStr(int_part, p);
		while (*p)
			p++;
	}
	*p++ = '.';

	if (frac_part == 0) {
		*p++ = '0';
	}
	else {
		char m;

		places = 2;//sizeof(outbuf) - (p - outbuf) - 1;
		//if (max > 7)
		//	max = 7;
		/* print BCD */
		for (m = 0; m < places; m++) {
			/* frac_part *= 10;	*/
			frac_part = (frac_part << 3) + (frac_part << 1);

			*p++ = (frac_part >> 24) + '0';
			frac_part &= 0xFFFFFF;
		}
		/* delete ending zeroes */
		for (--p; p[0] == '0' && p[-1] != '.'; --p)
			;
		++p;
	}
	*p = 0;

	return (p - dest);
}


// dest must have 8 chars
S8 numToHex(U32 value, char* dest) {
	// static for performance boost?
	static char const digit[] = "0123456789ABCDEF";
	char *p = dest;
	U32 length = 0;

	S32 shifter = value;
	do { //Move to where representation ends
		++p;
		length++;
		shifter = shifter / 16;
	} while (shifter);
	*p = '\0';
	do { //Move back, inserting digits as u go
		*--p = digit[value % 16];
		value = value / 16;
	} while (value);
	return length;
}
