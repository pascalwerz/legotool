/* by xvi */
#ifndef _MAPPEDFILE_H_
#define _MAPPEDFILE_H_	1

#include <stdlib.h>



// this functions allow for easier file read/modify/write
typedef struct { void *data; size_t size; size_t allocatedSize; int flags; int file; } mappedfile_t;

typedef enum
	{
	MAPPEDFILE_RDONLY = 0,
	MAPPEDFILE_RDWR = 1,
	} mapFileFlag_t;

typedef enum
	{
	MAPPEDFILE_DISCARD = 0,
	MAPPEDFILE_WRITE = 1,
	} unmapFileFlag_t;

void *mapFileAtPath(const char *path, mappedfile_t *mappedFile, mapFileFlag_t flags);
void *resizeMappedFile(mappedfile_t *mappedFile, size_t newSize);
int unmapFile(mappedfile_t *mappedFile, unmapFileFlag_t flags);

#endif // _MAPPEDFILE_H_
