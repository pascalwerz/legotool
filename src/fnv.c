/* by xvi */
#include "fnv.h"

#include <string.h>
#include <ctype.h>



uint32_t FNV1DataHash32(uint32_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = (uint8_t *) data;


	while (length--)
		seed = (seed * FNV_PRIME_32) ^ (*ptr++);

	return seed;
}



uint64_t FNV1DataHash64(uint64_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = (uint8_t *) data;


	while (length--)
		seed = (seed * FNV_PRIME_64) ^ (*ptr++);

	return seed;
}



uint32_t FNV1aDataHash32(uint32_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = (uint8_t *) data;


	while (length--)
		seed = (seed ^ (*ptr++)) * FNV_PRIME_32;

	return seed;
}



uint64_t FNV1aDataHash64(uint64_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = (uint8_t *) data;


	while (length--)
		seed = (seed ^ (*ptr++)) * FNV_PRIME_64;

	return seed;
}



uint32_t FNV1StringHash32(uint32_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;


	while (*ptr)
		seed = (seed * FNV_PRIME_32) ^ (*ptr++);

	return seed;
}



uint64_t FNV1StringHash64(uint64_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;


	while (*ptr)
		seed = (seed * FNV_PRIME_64) ^ (*ptr++);

	return seed;
}



uint32_t FNV1aStringHash32(uint32_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;


	while (*ptr)
		seed = (seed ^ (*ptr++)) * FNV_PRIME_32;

	return seed;
}



uint64_t FNV1aStringHash64(uint64_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;


	while (*ptr)
		seed = (seed ^ (*ptr++)) * FNV_PRIME_64;

	return seed;
}



uint32_t FNV1UppercaseStringHash32(uint32_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;

	while (*ptr)
		seed = (seed * FNV_PRIME_32) ^ toupper(*ptr++);

	return seed;
}



uint64_t FNV1UppercaseStringHash64(uint64_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;

	while (*ptr)
		seed = (seed * FNV_PRIME_64) ^ toupper(*ptr++);
		

	return seed;
}



uint32_t FNV1aUppercaseStringHash32(uint32_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;

	while (*ptr)
		seed = (seed ^ toupper(*ptr++)) * FNV_PRIME_32;

	return seed;
}



uint64_t FNV1aUppercaseStringHash64(uint64_t seed, const char *data)
{
	uint8_t *ptr = (uint8_t *) data;


	while (*ptr)
		seed = (seed ^ toupper(*ptr++)) * FNV_PRIME_64;

	return seed;
}



uint32_t ReverseFNV1DataHash32(uint32_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ (*ptr--)) * FNV_REVERSE_PRIME_32;

	return seed;
}



uint64_t ReverseFNV1DataHash64(uint64_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ (*ptr--)) * FNV_REVERSE_PRIME_64;

	return seed;
}



uint32_t ReverseFNV1aDataHash32(uint32_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_32) ^ (*ptr--);

	return seed;
}



uint64_t ReverseFNV1aDataHash64(uint64_t seed, const void *data, uintmax_t length)
{
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_64) ^ (*ptr--);

	return seed;
}



uint32_t ReverseFNV1StringHash32(uint32_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ (*ptr--)) * FNV_REVERSE_PRIME_32;

	return seed;
}



uint64_t ReverseFNV1StringHash64(uint64_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ (*ptr--)) * FNV_REVERSE_PRIME_64;

	return seed;
}



uint32_t ReverseFNV1aStringHash32(uint32_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_32) ^ (*ptr--);

	return seed;
}



uint64_t ReverseFNV1aStringHash64(uint64_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_64) ^ (*ptr--);

	return seed;
}



uint32_t ReverseFNV1UppercaseStringHash32(uint32_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ toupper(*ptr--)) * FNV_REVERSE_PRIME_32;

	return seed;
}



uint64_t ReverseFNV1UppercaseStringHash64(uint64_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed ^ toupper(*ptr--)) * FNV_REVERSE_PRIME_64;

	return seed;
}



uint32_t ReverseFNV1aUppercaseStringHash32(uint32_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_32) ^ toupper(*ptr--);

	return seed;
}



uint64_t ReverseFNV1aUppercaseStringHash64(uint64_t seed, const char *data)
{
	uintmax_t length = strlen(data);
	uint8_t *ptr = ((uint8_t *) data) + length - 1;


	while (length--)
		seed = (seed * FNV_REVERSE_PRIME_64) ^ toupper(*ptr--);

	return seed;
}
