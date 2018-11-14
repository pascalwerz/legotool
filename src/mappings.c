/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "mappings.h"
#include "ids.h"
#include "fnv.h"
#include "expandableMemory.h"



// on success, returns 0
int IDForText(context_t *context, uintmax_t *resultID, const char *text)
{
	uintmax_t hash;
	mapping * map;
	uint32_t f32;
	char *endPointer;


	// first detect float
	if (strlen(text) >= 3 && text[0] == '%' && text[1] == '%')
		{
		* (float *) &f32 = strtof(text + 2, &endPointer);
		if (text[2] && endPointer[0] == 0) { *resultID = f32; return 0; }

		return -1;
		}

	// then integers
	if (strlen(text) >= 2 && text[0] == '%')
		{
		hash = strtoumax(text + 1, &endPointer, 0);
		if (text[1] && endPointer[0] == 0) { *resultID = hash & 0xffffffff; return 0; }

		return -1;
		}

	hash = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, text);
	map = keyFind(context, hash, context->game, context->saveItemID1);		// find specific mapping
	if (!map) map = keyFind(context, hash, context->game, ID_WILDCARD);	// if not found, find game-specific mapping
	if (!map) map = keyFind(context, hash, ID_WILDCARD, context->saveItemID1);			// if still not found, find saveItem-specific mapping
	if (!map) map = keyFind(context, hash, ID_WILDCARD, ID_WILDCARD);				// if still not found, find generic mapping
	if (map)
		{
		if (map->label  && map->label[0]  && !strcasecmp(map->label, text)) { *resultID = hash; return 0; }
		if (map->string && map->string[0] && !strcasecmp(map->string, text)) { *resultID = hash; return 0; }
		}
	for (uintmax_t i = 0; i < context->knownIDsCount; i++)										// no non-empty match found find first matching label/string
		{
		if (context->knownIDs[i].label  && context->knownIDs[i].label[0]  && !strcasecmp(context->knownIDs[i].label, text)) { *resultID = context->knownIDs[i].key; return 0; }
		if (context->knownIDs[i].string && context->knownIDs[i].string[0] && !strcasecmp(context->knownIDs[i].string, text)) { *resultID = context->knownIDs[i].key; return 0; }
		}

	if (!strcmp(text, "*")) {*resultID = ID_WILDCARD; return 0; }

	// last chance, check if it is a number, as if preceded by '%'
	hash = strtoumax(text, &endPointer, 0);
	if (text[0] && endPointer[0] == 0) { *resultID = hash & 0xffffffff; return 0; }

	return -1;	// text could not be converted
}



// textForID will ALWAYS return some valid text
char *textForID(context_t *context, uintmax_t id, const char *defaultFormat)
{
	mapping * map;
	float f32;
	static char tmp[256];

	if (context->convertIDs)
		{
		map = keyFind(context, id, context->game, context->saveItemID1);			// find specific mapping
		if (!map) map = keyFind(context, id, context->game, ID_WILDCARD);			// if not found, find game-specific mapping
		if (!map) map = keyFind(context, id, ID_WILDCARD, context->saveItemID1);	// if still not found, find saveItem-specific mapping
		if (!map) map = keyFind(context, id, ID_WILDCARD, ID_WILDCARD);				// if still not found, find generic mapping
		if (!map)                                                            		// if still not found, look for key-only matching mapping
			{
			for (uintmax_t i = 0; i < context->knownIDsCount; i++)
				{
				if (context->knownIDs[i].key == id)
					{
					if (context->knownIDs[i].saveItemID == context->saveItemID1)
						map = &context->knownIDs[i];
					else if (context->knownIDs[i].game == context->game)
						map = &context->knownIDs[i];
					else
						map = &context->knownIDs[i];
					break;
					}
				}
			}
		if (map)
			{
			if (map->redirection)
				{
				for (uintmax_t i = 0; i < context->knownIDsCount; i++)
					{
					if (!strcmp(context->knownIDs[i].label, map->redirection))
						{
						if (context->knownIDs[i].saveItemID == context->saveItemID1)
							map = &context->knownIDs[i];
						else if (context->knownIDs[i].game == context->game)
							map = &context->knownIDs[i];
						else
							map = &context->knownIDs[i];
						break;
						}
					}
				}
			if (map->string && map->string[0])
				return map->string;
			if (map->label && map->label[0])
				return map->label;
			}
		}

	// not found, return default
	if (!strcmp(defaultFormat, "%f"))
		{
		* (uint32_t *) &f32 = id;
		sprintf(tmp, defaultFormat, f32);
		}
	else
		sprintf(tmp, defaultFormat, id);

	return tmp;
}



// split [baseFileName/]saveItem:object.field=value string, where each of saveItem/object/field/value can begin with % for a (dec/hex) integer and %% for a float
// for objectVersion488, fake field must be "Default" (* allowed and will match fake field Default)
// for objectVersion42244, fake field must be "Default" for value[12] and Flags (* allowed and will match fake field Default only)
// syntax is [[[saveItem:]object.]field=]value
// note: the rightmost '=' is the field/value separator, so field may contain '=' but value may not
// note: once '=value' removed, the rightmost '.' is the object/field separator, so object and value may contain '.' but field may not
// note: once '.field=value' removed, the rightmost '.' is the object/field separator, so saveItem may contain ':' but object may not
// note: once ':object.field=value' removed, the rightmost '.' is the object/field separator, so object name may contain '.'
// note: if no baseFileName is specified, NULL is returned, else a static copy of it is returned
// returns 0 on success, < 0 on failure.
int splitSaveItemObjectFieldValueString(context_t *context, char *string, const char **baseFileNameRequest, uintmax_t *saveItemRequestedID, uintmax_t *objectRequestedID, uintmax_t *fieldRequestedID, uintmax_t *value)
{
	staticExpandableMemory(char *, stringCopy, stringCopyAllocation);
	char *valueString;
	char *fieldString;
	char *objectString;
	char *saveItemString;


	expandMemoryChunkedDiscardingContent(stringCopy, stringCopyAllocation, 256, strlen(string) + 1);
	if (stringCopy == NULL) return -2;
	strcpy(stringCopy, string);

	*saveItemRequestedID = *objectRequestedID = *fieldRequestedID = *value = ID_WILDCARD;

	valueString = strrchr(stringCopy, '=');
	if (!valueString)
		{
		if (stringCopy[0] == 0)
			return -1;	// there MUST be a value here
		if (IDForText(context, value, stringCopy))
			return -1 ;	// value was invalid

		return 0;
		}
	else
		{
		*valueString = 0;
		valueString++;
		if (valueString[0] == 0)
			return -1;	// there MUST be a value here
		if (IDForText(context, value, valueString))
			return -1 ;	// value was invalid

		fieldString = strrchr(stringCopy, '.');
		if (!fieldString)
			{
			if (stringCopy[0] == 0)
				return -1;	// there MUST be a field here
			if (IDForText(context, fieldRequestedID, stringCopy))
				return -1 ;	// field was invalid

			return 0;
			}
		else
			{
			*fieldString = 0;
			fieldString++;
			if (fieldString[0] == 0)
				return -1;	// there MUST be a field here
			if (IDForText(context, fieldRequestedID, fieldString))
				return -1 ;	// field was invalid

			objectString = strrchr(stringCopy, ':');
			if (!objectString)
				{
				if (stringCopy[0] == 0)
					return -1;	// there MUST be an object here
				if (IDForText(context, objectRequestedID, stringCopy))
					return -1 ;	// object was invalid

				return 0;
				}
			else
				{
				*objectString = 0;
				objectString++;
				if (objectString[0] == 0)
					return -1;	// there MUST be an object here
				if (IDForText(context, objectRequestedID, objectString))
					return -1 ;	// object was invalid

				saveItemString = strrchr(stringCopy, '/');
				if (!saveItemString)
					{
					if (stringCopy[0] == 0)
						return -1;	// there MUST be a saveItem here
					if (IDForText(context, saveItemRequestedID, stringCopy))
						return -1 ;	// saveItem was invalid

					*baseFileNameRequest = NULL;
					return 0;
					}
				else
					{
					*saveItemString = 0;
					saveItemString++;
					if (saveItemString[0] == 0)
						return -1;	// there MUST be a saveItem here
					if (IDForText(context, saveItemRequestedID, saveItemString))
						return -1 ;	// saveItem was invalid

					if (stringCopy[0] == 0)
						return -1;	// there MUST be a baseFileName here

					*baseFileNameRequest = stringCopy;

					return 0;
					}
				}
			}
		}
}



