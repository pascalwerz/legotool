#ifndef _LEGOTOOL_QUESTLOG_H
#define _LEGOTOOL_QUESTLOG_H 1

#include <stdint.h>

#include "legotoolContext.h"



int parseQuestLog(context_t *context, uint8_t **dataPtr, uintmax_t *dataSize);

#endif // _LEGOTOOL_QUESTLOG_H