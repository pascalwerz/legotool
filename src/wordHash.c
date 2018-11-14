/* this file is part of legotool, by xvi */
#include <stdlib.h>
#include <stdint.h>

typedef struct { uintmax_t key; uintmax_t game; uintmax_t saveItemID; uintmax_t flags; char *label; char *string; char *redirection; } mapping;
#define gameAny ((uintmax_t)-1)

mapping wordHash[] =
	{
// if you get some error similar to
//		make: *** No rule to make target `dictionary.c', needed by `src/wordHash.o'.  Stop.
// or
// 		fatal error: '../dictionary.c' file not found
// that's normal,  see src/ids/00-how-to.txt how to generate dictionary.c.
#include "../dictionary.c"
	};
int wordHashCount = sizeof(wordHash)/sizeof(*wordHash);
