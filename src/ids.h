/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_IDS_H_
#define _LEGOTOOL_IDS_H_	1

#include <stdlib.h>
#include <stdint.h>

#include "mappings.h"


extern mapping globalKnownIDs[];
extern uintmax_t globalKnownIDsCount;


// mapping related
int hashCompareFunction(const void *h1, const void *h2);
mapping *keyFind(context_t *context, uintmax_t key, uintmax_t fileVersion, uintmax_t saveItemID);



#endif // _LEGOTOOL_IDS_H_
