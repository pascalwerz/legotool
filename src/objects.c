/* this file is part of legotool, by xvi */
#include "objects.h"
#include "mappings.h"
#include "memory.h"
#include "fnv.h"

#include <stdio.h>

// version 1:
// typedef struct { uint32_t object; uint16_t unknown; uint16_t collected; uint32_t probationalValue; uint32_t permanentValue; } object_t;
// version 2:
// typedef struct { uint32_t object; uint64_t probationalValue; uint64_t permanentValue; } object_t;
// version 3:
// typedef struct { uint32_t object; uint32_t field; uint64_t probationalValuealue1; uint64_t permanentValue; } object_t;

// While analyzing worlds saved game, I suspected most data was structured like version 3
// ...and this was the key to many, many, many files...
// object and field are mapping keys, values can be mapping keys or any uint32_t, uint64_t and even float represented as uint32_t
// some variants exists (e.g. batman3)


// As variants exist, object_t is nothing but an opaque structure.
// getters and setters take care of all variants



uintmax_t objectSizeOf(context_t *context)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return 0x10;
	case objectVersion488:
		return 0x14;
	case objectVersion4488: default:
		return 0x18;
		}
}



uint32_t objectObject(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return read32(((uint8_t *)field) + 0x00, context->endianness);
	case objectVersion488:
		return read32(((uint8_t *)field) + 0x00, context->endianness);
	case objectVersion4488: default:
		return read32(((uint8_t *)field) + 0x00, context->endianness);
		}
}



uint32_t objectField(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return context->defaultID; // fake fieldID
	case objectVersion488:
		return context->defaultID; // fake fieldID
	case objectVersion4488: default:
		return read32(((uint8_t *)field) + 0x04, context->endianness);
		}
}



uint16_t objectUnknownFlag(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return read16(((uint8_t *)field) + 0x04, context->endianness);
	case objectVersion488:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
	case objectVersion4488: default:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		}
}



uint16_t objectCollectedFlag(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return read16(((uint8_t *)field) + 0x06, context->endianness);
	case objectVersion488:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
	case objectVersion4488: default:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		}
}



uint64_t objectProbationalValue(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return read32(((uint8_t *)field) + 0x08, context->endianness);
	case objectVersion488:
		return read64(((uint8_t *)field) + 0x04, context->endianness);
	case objectVersion4488: default:
		return read64(((uint8_t *)field) + 0x08, context->endianness);
		}
}



uint64_t objectPermanentValue(context_t *context, object_t *field)
{
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		return read32(((uint8_t *)field) + 0x0c, context->endianness);
	case objectVersion488:
		return read64(((uint8_t *)field) + 0x0c, context->endianness);
	case objectVersion4488: default:
		return read64(((uint8_t *)field) + 0x10, context->endianness);
		}
}



void setObjectObject(context_t *context, object_t *field, uint32_t objectID)
{
	context->fileModified++;

	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		write32(((uint8_t *)field) + 0x00, context->endianness, objectID);
		break;
	case objectVersion488:
		write32(((uint8_t *)field) + 0x00, context->endianness, objectID);
		break;
	case objectVersion4488: default:
		write32(((uint8_t *)field) + 0x00, context->endianness, objectID);
		break;
		}
}



void setObjectField(context_t *context, object_t *field, uint32_t fieldID)
{
	context->fileModified++;

	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
	case objectVersion488:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
	case objectVersion4488: default:
		write32(((uint8_t *)field) + 0x04, context->endianness, fieldID);
		break;
		}
}



void setObjectUnknownFlag(context_t *context, object_t *field, uint16_t value)
{
	context->fileModified++;

	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		write32(((uint8_t *)field) + 0x04, context->endianness, value);
		break;
	case objectVersion488:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
	case objectVersion4488: default:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
		}
}



void setObjectCollectedFlag(context_t *context, object_t *field, uint16_t value)
{
	context->fileModified++;

	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		write32(((uint8_t *)field) + 0x06, context->endianness, value);
		break;
	case objectVersion488:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
	case objectVersion4488: default:
		fprintf(stderr, "internal error: invalid operation for object version\n");
		exit(1);
		break;
		}
}



void setObjectProbationalValue(context_t *context, object_t *field, uint64_t value)
{
	context->fileModified++;

	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		write32(((uint8_t *)field) + 0x08, context->endianness, value);
		break;
	case objectVersion488:
		write64(((uint8_t *)field) + 0x04, context->endianness, value);
		break;
	case objectVersion4488: default:
		write64(((uint8_t *)field) + 0x08, context->endianness, value);
		break;
		}
}



void setObjectPermanentValue(context_t *context, object_t *field, uint64_t value)
{
	context->fileModified++;
	switch(context->saveItemObjectVersion)
		{
	case objectVersion42244:
		write32(((uint8_t *)field) + 0x0c, context->endianness, value);
		break;
	case objectVersion488:
		write64(((uint8_t *)field) + 0x0c, context->endianness, value);
		break;
	case objectVersion4488: default:
		write64(((uint8_t *)field) + 0x10, context->endianness, value);
		break;
		}
}



uintmax_t forAllObjectsWithObjectNameAndFieldName(context_t *context, forAllFunction f, uintmax_t userData, char *objectName, char *fieldName)
{
	return forAllObjectsWithObjectIDAndFieldID(context, f, userData, FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, objectName), FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, fieldName));
}



uintmax_t forAllObjectsWithObjectIDAndFieldID(context_t *context, forAllFunction f, uintmax_t userData, uintmax_t objectID, uintmax_t fieldID)
{
	int result;
	object_t *field;
	uintmax_t fieldCount;


	fieldCount = context->saveItemDataSize / objectSizeOf(context);

	for (uintmax_t i = 0; i < fieldCount; i++)
		{
		field = (object_t *) (context->fileData + context->saveItemDataOffset + i * objectSizeOf(context));
		if (objectObject(context, field) == objectID || objectID == ID_WILDCARD)
			{
			switch (context->saveItemObjectVersion)
				{
			case objectVersion42244:
				if (fieldID == ID_WILDCARD) fieldID = context->defaultID; // * will match the fake Default field
				if (objectField(context, field) == context->defaultID || objectField(context, field) == context->unknownFlagID || objectField(context, field) == context->collectedFlagID)
					{
					result = f(context, field, fieldID, userData);
					if (result) return result;
					}
				break;
			case objectVersion488:
				if (fieldID == ID_WILDCARD) fieldID = context->defaultID; // * will match the fake Default field
				if (objectField(context, field) == context->defaultID)
					{
					result = f(context, field, fieldID, userData);
					if (result) return result;
					}
				break;
			case objectVersion4488:
				if (fieldID == ID_WILDCARD) fieldID = objectField(context, field); // * will match all fields
				if (objectField(context, field) == fieldID)
					{
					result = f(context, field, fieldID, userData);
					if (result) return result;
					}
				break;
				}
			}
		}

	return 0;
}



uintmax_t forAllObjects(context_t *context, forAllFunction f, uintmax_t userData)
{
	return forAllObjectsWithObjectIDAndFieldID(context, f, userData, ID_WILDCARD, ID_WILDCARD);
}



// find objectID.fieldID in saveItem data. On failure, returns NULL
object_t *findObjectForObjectIDAndFieldID(context_t *context, uintmax_t objectID, uintmax_t fieldID)
{
	uintmax_t fieldCount;
	object_t *field;


	fieldCount = context->saveItemDataSize / objectSizeOf(context);
	for (uintmax_t i = 0; i < fieldCount; i++)
		{
		field = (object_t *) (context->fileData + context->saveItemDataOffset + i * objectSizeOf(context));
		if (objectObject(context, field) == objectID && objectField(context, field) == fieldID)
			return field;
		}

	return NULL;
}



int findObjectForObjectIDAndFieldIDAndNotValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value)
{
	object_t *field;


	field = findObjectForObjectIDAndFieldID(context, objectID, fieldID);
	if (!field)
		return 0; // no update needed: object.field not found

	return (objectProbationalValue(context, field) != value) || (objectPermanentValue(context, field) != value);
}



int findObjectForObjectIDAndFieldIDAndCheckValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value, int absentResult)
{
	object_t *field;


	field = findObjectForObjectIDAndFieldID(context, objectID, fieldID);
	if (!field)
		return absentResult; // object.field not found

	return (objectProbationalValue(context, field) == value) && (objectPermanentValue(context, field) == value);
}



void findObjectForObjectIDAndFieldIDAndSetValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value)
{
	object_t *field;


	field = findObjectForObjectIDAndFieldID(context, objectID, fieldID);
	if (field)
		setObjectValue(context, field, fieldID, value);
}



void setObjectValue(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t value)
{
			switch (context->saveItemObjectVersion)
				{
			case objectVersion42244:
				if (fieldID == context->defaultID)
					{
					setObjectProbationalValue(context, field, value);
					setObjectPermanentValue(context, field, value);
					}
				else if (fieldID == context->unknownFlagID)
					{
					setObjectUnknownFlag(context, field, value);
					}
				else if (fieldID == context->collectedFlagID)
					{
					setObjectCollectedFlag(context, field, value);
					}
				else
					{
					fprintf(stderr, "internal error: illegal setObjectValue() request\n");
					exit(1);
					}
				break;
			case objectVersion488:
				if (fieldID != context->defaultID)
					{
					fprintf(stderr, "internal error: illegal setObjectValue() request\n");
					exit(1);
					}
				setObjectProbationalValue(context, field, value);
				setObjectPermanentValue(context, field, value);
				break;
			case objectVersion4488:
				if (objectField(context, field) != fieldID)
					{
					fprintf(stderr, "internal error: illegal setObjectValue() request\n");
					exit(1);
					}
				setObjectProbationalValue(context, field, value);
				setObjectPermanentValue(context, field, value);
				break;
				}
}



