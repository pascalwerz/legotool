/* this file is part of legotool, by xvi */
#include "setCoins.h"
#include "objects.h"
#include <stdio.h>



uintmax_t setCoinsFunction(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t userData);



uintmax_t setCoinsFunction(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t userData)
{
	if (objectProbationalValue(context, field) != userData || objectPermanentValue(context, field) != userData)
		{
		if (context->verbose)
			printf("updating %s/%s:%s.%s=%%%ju\n", context->baseFileName, textForID(context, context->saveItemID1, "%08jx"), textForID(context, objectObject(context, field), "%08jx"), textForID(context, objectField(context, field), "%08jx"), userData);
		setObjectValue(context, field, fieldID, userData);
		}

	return 0;
}



uintmax_t setCoins(context_t *context, uintmax_t userData)
{
	return forAllObjectsWithObjectNameAndFieldName(context, setCoinsFunction, context->willSetCoinsValue, "MainCoinTotal", "Default");
}



