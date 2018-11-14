/* by xvi */
#ifndef _FNV_H_
#define _FNV_H_	1

#include <stdint.h>



#define FNV1_OFFSET_BASIS_SEED 0
#define FNV1_OFFSET_BASIS_STRING "chongo <Landon Curt Noll> /\\../\\"	// beware: the two '\\' are escaped '\'


uint32_t FNV1DataHash32(uint32_t seed, const void *data, uintmax_t length);
uint64_t FNV1DataHash64(uint64_t seed, const void *data, uintmax_t length);
uint32_t FNV1aDataHash32(uint32_t seed, const void *data, uintmax_t length);
uint64_t FNV1aDataHash64(uint64_t seed, const void *data, uintmax_t length);

uint32_t FNV1StringHash32(uint32_t seed, const char *data);
uint64_t FNV1StringHash64(uint64_t seed, const char *data);
uint32_t FNV1aStringHash32(uint32_t seed, const char *data);
uint64_t FNV1aStringHash64(uint64_t seed, const char *data);

uint32_t FNV1UppercaseStringHash32(uint32_t seed, const char *data);
uint64_t FNV1UppercaseStringHash64(uint64_t seed, const char *data);
uint32_t FNV1aUppercaseStringHash32(uint32_t seed, const char *data);
uint64_t FNV1aUppercaseStringHash64(uint64_t seed, const char *data);

uint32_t ReverseFNV1DataHash32(uint32_t seed, const void *data, uintmax_t length);
uint64_t ReverseFNV1DataHash64(uint64_t seed, const void *data, uintmax_t length);
uint32_t ReverseFNV1aDataHash32(uint32_t seed, const void *data, uintmax_t length);
uint64_t ReverseFNV1aDataHash64(uint64_t seed, const void *data, uintmax_t length);

uint32_t ReverseFNV1StringHash32(uint32_t seed, const char *data);
uint64_t ReverseFNV1StringHash64(uint64_t seed, const char *data);
uint32_t ReverseFNV1aStringHash32(uint32_t seed, const char *data);
uint64_t ReverseFNV1aStringHash64(uint64_t seed, const char *data);

uint32_t ReverseFNV1UppercaseStringHash32(uint32_t seed, const char *data);
uint64_t ReverseFNV1UppercaseStringHash64(uint64_t seed, const char *data);
uint32_t ReverseFNV1aUppercaseStringHash32(uint32_t seed, const char *data);
uint64_t ReverseFNV1aUppercaseStringHash64(uint64_t seed, const char *data);



#define FNV1_INITIAL_SEED_32	0x811c9dc5         	// = FNV1StringHash32(0, FNV_OFFSET_BASIS_STRING);
#define FNV1_INITIAL_SEED_64	0xcbf29ce484222325 	// = FNV1StringHash64(0, FNV_OFFSET_BASIS_STRING);

#define FNV_PRIME_32			0x1000193	
#define FNV_REVERSE_PRIME_32	0x359c449b	        // (FNV_REVERSE_PRIME_32 * FNV_PRIME_32) == 1 (modulo 2^32)
#define FNV_PRIME_64			0x100000001b3	
#define FNV_REVERSE_PRIME_64	0xce965057aff6957b	// (FNV_REVERSE_PRIME_64 * FNV_PRIME_64) == 1 (modulo 2^64)


#endif // _FNV_H_

