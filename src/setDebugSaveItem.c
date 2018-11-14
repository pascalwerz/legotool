/* this file is part of legotool, by xvi */
#include "setDebugSaveItem.h"
#include <stdio.h>



uintmax_t setDebugSaveItem(context_t *context, uintmax_t userData)
{
	if (context->saveItemDataSize < 1) return 0;	// ooh that's bad, saveItem is too short

	if (context->verbose)
		printf("updating %s/%s=%ju\n", context->baseFileName, textForID(context, context->saveItemID1, "%08jx"), (uintmax_t) userData);

	context->fileModified++;
	context->fileData[context->saveItemDataOffset] = userData;

	return 0;
}



