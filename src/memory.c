/* this file is part of legotool, by xvi */
#include "memory.h"



uintmax_t read16(void *ptr, int endianness)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
default:
case 0:
	return * (uint16_t *) bytePtr;
case 1:
	return (uintmax_t) bytePtr[0] | ((uintmax_t) bytePtr[1] << 8);
case 2:
	return (uintmax_t) bytePtr[3] | ((uintmax_t) bytePtr[2] << 8);
	}
}



uintmax_t read32(void *ptr, int endianness)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
default:
case 0:
	return * (uint32_t *) bytePtr;
case 1:
	return (uintmax_t) bytePtr[0] | ((uintmax_t) bytePtr[1] << 8) | ((uintmax_t) bytePtr[2] << 16) | ((uintmax_t) bytePtr[3] << 24);
case 2:
	return (uintmax_t) bytePtr[3] | ((uintmax_t) bytePtr[2] << 8) | ((uintmax_t) bytePtr[1] << 16) | ((uintmax_t) bytePtr[0] << 24);
	}
}



uintmax_t read64(void *ptr, int endianness)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
default:
case 0:
	return * (uint64_t *) bytePtr;
case 1:
	return (uintmax_t) bytePtr[0] | ((uintmax_t) bytePtr[1] << 8) | ((uintmax_t) bytePtr[2] << 16) | ((uintmax_t) bytePtr[3] << 24) | ((uintmax_t) bytePtr[4] << 32) | ((uintmax_t) bytePtr[5] << 40) | ((uintmax_t) bytePtr[6] << 48) | ((uintmax_t) bytePtr[7] << 56);
case 2:
	return (uintmax_t) bytePtr[7] | ((uintmax_t) bytePtr[6] << 8) | ((uintmax_t) bytePtr[5] << 16) | ((uintmax_t) bytePtr[4] << 24) | ((uintmax_t) bytePtr[3] << 32) | ((uintmax_t) bytePtr[2] << 40) | ((uintmax_t) bytePtr[1] << 48) | ((uintmax_t) bytePtr[0] << 56);
	}
}



void write16(void *ptr, int endianness, uintmax_t value)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
case 0:
	* (uint16_t *) bytePtr = value;
	break;
case 1:
	bytePtr[0] = value; value >>= 8;
	bytePtr[1] = value;
	break;
case 2:
	bytePtr[1] = value; value >>= 8;
	bytePtr[0] = value;
	break;
	}
}



void write32(void *ptr, int endianness, uintmax_t value)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
case 0:
	* (uint32_t *) bytePtr = value;
	break;
case 1:
	bytePtr[0] = value; value >>= 8;
	bytePtr[1] = value; value >>= 8;
	bytePtr[2] = value; value >>= 8;
	bytePtr[3] = value;
	break;
case 2:
	bytePtr[3] = value; value >>= 8;
	bytePtr[2] = value; value >>= 8;
	bytePtr[1] = value; value >>= 8;
	bytePtr[0] = value;
	break;
	}
}



void write64(void *ptr, int endianness, uintmax_t value)
{
uint8_t *bytePtr;


bytePtr = (uint8_t *) ptr;
switch (endianness)
	{
case 0:
	* (uint64_t *) bytePtr = value;
	break;
case 1:
	bytePtr[0] = value; value >>= 8;
	bytePtr[1] = value; value >>= 8;
	bytePtr[2] = value; value >>= 8;
	bytePtr[3] = value; value >>= 8;
	bytePtr[4] = value; value >>= 8;
	bytePtr[5] = value; value >>= 8;
	bytePtr[6] = value; value >>= 8;
	bytePtr[7] = value;
	break;
case 2:
	bytePtr[7] = value; value >>= 8;
	bytePtr[6] = value; value >>= 8;
	bytePtr[5] = value; value >>= 8;
	bytePtr[4] = value; value >>= 8;
	bytePtr[3] = value; value >>= 8;
	bytePtr[2] = value; value >>= 8;
	bytePtr[1] = value; value >>= 8;
	bytePtr[0] = value;
	break;
	}
}



uintmax_t get16(context_t *context, uintmax_t offset)
{
return read16(context->fileData + offset, context->endianness);
}



uintmax_t get32(context_t *context, uintmax_t offset)
{
return read32(context->fileData + offset, context->endianness);
}



uintmax_t get64(context_t *context, uintmax_t offset)
{
return read64(context->fileData + offset, context->endianness);
}



void set16(context_t *context, uintmax_t offset, uintmax_t value)
{
context->fileModified++;
write16(context->fileData + offset, context->endianness, value);
}



void set32(context_t *context, uintmax_t offset, uintmax_t value)
{
context->fileModified++;
write32(context->fileData + offset, context->endianness, value);
}



void set64(context_t *context, uintmax_t offset, uintmax_t value)
{
context->fileModified++;
write64(context->fileData + offset, context->endianness, value);
}
