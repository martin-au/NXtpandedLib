
#include "convert.hpp"


//dest must have 12 chars
U32 numToStr(S32 value, char* dest) {
	static char const digit[] = "0123456789";
	char *p = dest;
	U32 length = 0;

	if (value < 0) {
		*p++ = '-';
		length++;
		value *= -1;
	}
	S32 shifter = value;
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


U32 numToStr(U32 value, char* dest) {
	static char const digit[] = "0123456789";
	char *p = dest;
	U32 length = 0;

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


U32 numToStr(float value, char* dest, U16 places) {
	int minwidth = 0;
	bool rightjustify = false;
	// this is used to write a float value to string, outstr.  oustr is also the return value.
	int digit;
	static char const chardigit[] = "0123456789";
	float tens = 0.1;
	int tenscount = 0;
	int i;
	float tempfloat = value;
	int c = 0;
	int charcount = 1;
	int extra = 0;
	// make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
	// if this rounding step isn't here, the value  54.321 prints as 54.3209

	// calculate rounding term d:   0.5/pow(10,places)
	float d = 0.5;
	if (value < 0)
		d *= -1.0;
	// divide by ten for each decimal place
	for (i = 0; i < places; i++)
		d /= 10.0;
	// this small addition, combined with truncation will round our values properly
	tempfloat += d;

	// first get value tens to be the large power of ten less than value
	if (value < 0)
		tempfloat *= -1.0;
	while ((tens * 10.0) <= tempfloat) {
		tens *= 10.0;
		tenscount += 1;
	}

	if (tenscount > 0)
		charcount += tenscount;
	else
		charcount += 1;

	if (value < 0)
		charcount += 1;
	charcount += 1 + places;

	minwidth += 1; // both count the null final character
	if (minwidth > charcount) {
		extra = minwidth - charcount;
	}

	if (extra > 0 && rightjustify) {
		for (int i = 0; i < extra; i++) {
			dest[c++] = ' ';
		}
	}

	// write out the negative if needed
	if (value < 0)
		dest[c++] = '-';

	if (tenscount == 0)
		dest[c++] = '0';

	for (i = 0; i < tenscount; i++) {
		digit = (int) (tempfloat / tens);
		dest[c++] = chardigit[digit];
		tempfloat = tempfloat - ((float) digit * tens);
		tens /= 10.0;
	}

	// otherwise, write the point and continue on
	if (places > 0)
		dest[c++] = '.';

	// now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
	for (i = 0; i < places; i++) {
		tempfloat *= 10.0;
		digit = (int) tempfloat;
		dest[c++] = chardigit[digit];
		// once written, subtract off that digit
		tempfloat = tempfloat - (float) digit;
	}
	if (extra > 0 && !rightjustify) {
		for (int i = 0; i < extra; i++) {
			dest[c++] = ' ';
		}
	}

	dest[c++] = '\0';
	return static_cast<U32>(charcount);
}

// dest must have 8 chars
U32 numToHex(U32 value, char* dest) {
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
