/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_H_
#define _LEGOTOOL_H_	1

#include "legotoolContext.h"

// functionalities
#include "dumpSaveItem.h"
#include "refillProps.h"
#include "zap.h"
#include "setDebugSaveItem.h"
#include "setCoins.h"

// generic includes
#include "fnv.h"

#include <stdint.h>


// functions that operates on the whole file
void setPercentage(context_t *context, float newPercentage);
void updateFileChecksums(context_t *context);	// requires context->loadOffset and context->loadSize


// functions that operates on NO file
void dumpAllIDs(context_t *context);
void dumpNonMatchingIDs(context_t *context);
void generateSynalysis(context_t *context);

// printing IDs require printID()
void printID(const mapping* id);

// utility
uint32_t checksum(const void *data, uintmax_t size);

// you need this one, of course
void usage(context_t *context, int exitCode);

#endif // _LEGOTOOL_H_
