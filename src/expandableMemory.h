/* by xvi */
#ifndef _EXPANDABLEMEMORY_H_
#define _EXPANDABLEMEMORY_H_	1

// This is a simple wrapper to declare and use variables for malloc()ated memory that only grows.

// to be used as variable declaration
#define expandableMemory(TYPE,NAME,ALLOCATIONNAME) \
	unsigned long long ALLOCATIONNAME = 0; \
	TYPE NAME = NULL

// to be used as variable declaration
#define staticExpandableMemory(TYPE,NAME,ALLOCATIONNAME) \
	static unsigned long long ALLOCATIONNAME = 0; \
	static TYPE NAME = NULL


// for strings, don't forget to add 1 to strlen() in NEWSIZE
// once done, you have check for failure, i.e. check if NAME == NULL
#define expandMemory(NAME,ALLOCATIONNAME,NEWSIZE) \
	do { if (NEWSIZE > ALLOCATIONNAME) { ALLOCATIONNAME = NEWSIZE; NAME = reallocf(NAME, ALLOCATIONNAME); } } while (0)

#define expandMemoryDiscardingContent(NAME,ALLOCATIONNAME,NEWSIZE) \
	do { if (NEWSIZE > ALLOCATIONNAME) { free(NAME); ALLOCATIONNAME = NEWSIZE; NAME = malloc(ALLOCATIONNAME); } } while (0)

#define expandMemoryChunked(NAME,ALLOCATIONNAME,CHUNKSIZE,NEWSIZE) \
	do { if (NEWSIZE > ALLOCATIONNAME) { ALLOCATIONNAME = ((NEWSIZE + CHUNKSIZE - 1) / CHUNKSIZE) * CHUNKSIZE; NAME = reallocf(NAME, ALLOCATIONNAME); } } while (0)

#define expandMemoryChunkedDiscardingContent(NAME,ALLOCATIONNAME,CHUNKSIZE,NEWSIZE) \
	do { if (NEWSIZE > ALLOCATIONNAME) { free(NAME); ALLOCATIONNAME = ((NEWSIZE + CHUNKSIZE - 1) / CHUNKSIZE) * CHUNKSIZE; NAME = malloc(ALLOCATIONNAME); } } while (0)

#endif // _EXPANDABLEMEMORY_H_
