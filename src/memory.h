/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_MEMORY_H_
#define _LEGOTOOL_MEMORY_H_	1

#include "legotoolContext.h"
#include <stdint.h>


// raw read/write
uintmax_t read16(void *ptr, int endianness);
uintmax_t read32(void *ptr, int endianness);
uintmax_t read64(void *ptr, int endianness);
void *readCounted32Bytes(void *ptr, uintmax_t availableBytes, uintmax_t *resultBytes, int endianness); // result is malloc()ed
void write16(void *ptr, int endianness, uintmax_t value);
void write32(void *ptr, int endianness, uintmax_t value);
void write64(void *ptr, int endianness, uintmax_t value);
void writeCounted32Bytes(void *ptr, uintmax_t length, void *data, int endianness);

// set/get
uintmax_t get16(context_t *context, uintmax_t offset);
uintmax_t get32(context_t *context, uintmax_t offset);
uintmax_t get64(context_t *context, uintmax_t offset);
void set16(context_t *context, uintmax_t offset, uintmax_t value);
void set32(context_t *context, uintmax_t offset, uintmax_t value);
void set64(context_t *context, uintmax_t offset, uintmax_t value);

#endif // _LEGOTOOL_MEMORY_H_
