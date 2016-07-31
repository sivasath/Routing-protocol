#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


/*
** packi16() -- store a 16-bit int into a char buffer (like htons())
*/ 
void packi16(uint8_t *buf, int16_t i)
{
	uint16_t i2 = i;

	*buf++ = i2>>8; *buf++ = i2;
}

/*
** packi32() -- store a 32-bit int into a char buffer (like htonl())
*/ 
void packi32(uint8_t *buf, int32_t i)
{
	uint32_t i2 = i;

	*buf++ = i2>>24; *buf++ = i2>>16;
	*buf++ = i2>>8;  *buf++ = i2;
}


/*
** unpacki16() -- unpack a 16-bit int from a char buffer (like ntohs())
*/ 
int16_t unpacki16(uint8_t *buf)
{
	uint16_t i2 = ((uint16_t)buf[0]<<8) | buf[1];
	int16_t i;

	// change unsigned numbers to signed
	if (i2 <= 0x7fffu) { i = i2; }
	//else { i = -(int16_t)((uint16_t)0xffff - i2 + (uint16_t)1u); }
	else { i = -1 - (uint16_t)(0xffffu - i2); }

	return i;
}

/*
** unpacki32() -- unpack a 32-bit int from a char buffer (like ntohl())
*/ 
int32_t unpacki32(uint8_t *buf)
{
	uint32_t i2 = ((uint32_t)buf[0]<<24) | ((uint32_t)buf[1]<<16) |
	              ((uint32_t)buf[2]<<8)  | buf[3];
	int32_t i;

	// change unsigned numbers to signed
	if (i2 <= 0x7fffffffu) { i = i2; }
	else { i = -1 - (int32_t)(0xffffffffu - i2); }

	return i;
}


/*
** pack() -- store data dictated by the format string in the buffer

    h - 16-bit signed int
    l - 32-bit signed int    
  
*/ 
int32_t pack(uint8_t *buf, char *format, ...)
{
	va_list ap;
	int16_t h;
	int32_t l;
	int32_t size = 0, len;

	va_start(ap, format);

	for(; *format != '\0'; format++) {
		switch(*format) {
		case 'h': // 16-bit
			size += 2;
			h = (int16_t)va_arg(ap, int); // promoted
			packi16(buf, h);
			buf += 2;
			break;

		case 'l': // 32-bit
			size += 4;
			l = va_arg(ap, int32_t);
			packi32(buf, l);
			buf += 4;
			break;

		}
	}

	va_end(ap);

	return size;
}

/*
** unpack() -- unpack data dictated by the format string into the buffer
*/
void unpack(uint8_t *buf, char *format, ...)
{
	va_list ap;
	int16_t *h;
	int32_t *l;
	int64_t *L;
	int32_t pf;
	int64_t pF;
	int32_t len, count, maxstrlen=0;

	va_start(ap, format);

	for(; *format != '\0'; format++) {
		switch(*format) {
		case 'h': // 16-bit
			h = va_arg(ap, int16_t*);
			*h = unpacki16(buf);
			buf += 2;
			break;

		case 'l': // 32-bit
			l = va_arg(ap, int32_t*);
			*l = unpacki32(buf);
			buf += 4;
			break;

		}

		if (!isdigit(*format)) maxstrlen = 0;
	}

	va_end(ap);
}



