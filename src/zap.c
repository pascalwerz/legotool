/* this file is part of legotool, by xvi */
#include "zap.h"
#include "objects.h"
#include "legotool.h" // for usage()

#include <string.h>
#include <stdio.h>



uintmax_t zapFunction(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t userData);



uintmax_t zapFunction(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t userData)
{
	setObjectValue(context, field, fieldID, userData);

	return 0;
}



uintmax_t zap(context_t *context, uintmax_t userData)
{
	const char *targetBaseFileName;
	uintmax_t targetSaveItem;
	uintmax_t targetObject;
	uintmax_t targetField;
	uintmax_t targetValue;
	int result;


	// context->willZapString is re-evaluated for each saveItem as we want the ID lookup to honor the current saveItemID
	result = splitSaveItemObjectFieldValueString(context, context->willZapString, &targetBaseFileName, &targetSaveItem, &targetObject, &targetField, &targetValue);
	if (result != 0)
		{
		fprintf(stderr, "error: zap argument must be [baseFileName/]saveItem:object.field=value");
		usage(context, 1);
		}

	if (targetBaseFileName && strcasecmp(targetBaseFileName, context->baseFileName))
		return 0;	// a baseFileName was specified and this is not the current one, don't handle it

	if (context->saveItemID1 != targetSaveItem && targetSaveItem != ID_WILDCARD)
		return 0;	// not an adequate saveItem, proceed to next

	return forAllObjectsWithObjectIDAndFieldID(context, zapFunction, targetValue, targetObject, targetField);
}



