/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dumpSaveItem.h"
#include "saveItems.h"
#include "objects.h"
#include "memory.h"
#include "objects.h"
#include "legotool.h"

#include "fnv.h"



// Note: the logic that 'knows' which object version/format applies for a kind of saveItem in a game is in saveItems.c

uintmax_t dumpFormatted   (context_t *context);
uintmax_t dumpJumpToSystem(context_t *context);
uintmax_t dumpStream      (context_t *context);
uintmax_t dump24          (context_t *context);
uintmax_t dump4           (context_t *context);
uintmax_t dump2           (context_t *context);
uintmax_t dump1           (context_t *context);
uintmax_t dumpUnknown     (context_t *context);
uintmax_t dumpDont        (context_t *context);
void      dumpField       (context_t *context, object_t *data, const char *valueFormat, int shouldLookup);



uintmax_t dumpSaveItem(context_t *context, uintmax_t userData)
{
	char *format;


	format = context->saveItemObjectFormat;

	if (0) {}
	else if (format[0] == '-')
		dumpDont(context);
	else if (!strcasecmp(format, "?"))
		dumpUnknown(context);
	else if (!strcasecmp(format, "1"))
		dump1(context);
	else if (!strcasecmp(format, "2"))
		dump2(context);
	else if (!strcasecmp(format, "4"))
		dump4(context);
	else if (!strcasecmp(format, "JumpTo"))
		dumpJumpToSystem(context);
	else if (!strcasecmp(format, "Stream"))
		dumpStream(context);
	else // all other strings are formats for dumpFormatted()
		dumpFormatted(context);

	return 0;
}



void dumpField(context_t *context, object_t *data, const char *valueFormat, int shouldLookup)
{
	char tmp[256];
	float f32;
	time_t t;
	struct tm tm;
	char timeString[64];

	if (context->willPrintSaveItemName)
		printf("%s:", textForID(context, context->saveItemID1 & 0xffffffff, "%%%08jx"));
	printf("%s", textForID(context, objectObject(context, data) & 0xffffffff, "%08jx"));
	printf(".%s=", textForID(context, objectField(context, data) & 0xffffffff, "%08jx"));

	if (shouldLookup)
		strcpy(tmp, textForID(context, objectProbationalValue(context, data) & 0xffffffff, valueFormat));
	else
		{
		if (!strcmp(valueFormat, "%t"))
			{
			t = objectProbationalValue(context, data);
			if (t && gmtime_r(&t, &tm))
				{
				// gmtime required, NOT localtime, even if this is NOT an UTC time_t
				// the displayed time will then be the local time *where* and *when* game timestamp was taken
				asctime_r(&tm, timeString);
				if (timeString[strlen(timeString) - 1] == '\n') timeString[strlen(timeString) - 1] = 0;
				sprintf(tmp, "0x%08jx [%s]", (uintmax_t) objectProbationalValue(context, data), timeString);
				}
			else
				sprintf(tmp, "0x%08jx", (uintmax_t) objectProbationalValue(context, data));
			}
		else if (!strcmp(valueFormat, "%f"))
			{
			* (uint32_t *) &f32 = objectProbationalValue(context, data);
			sprintf(tmp, valueFormat, f32);
			}
		else
			sprintf(tmp, valueFormat, objectProbationalValue(context, data));
		}
	printf("%s ", tmp);

	if (objectProbationalValue(context, data) != objectPermanentValue(context, data))
		{
		if (shouldLookup)
			strcpy(tmp, textForID(context, objectPermanentValue(context, data) & 0xffffffff, valueFormat));
		else
			{
			if (!strcmp(valueFormat, "%f"))
				{
				* (uint32_t *) &f32 = objectPermanentValue(context, data);
				sprintf(tmp, valueFormat, f32);
				}
			else
				sprintf(tmp, valueFormat, objectPermanentValue(context, data));
			}
		printf("[\u2260 %s] ", tmp);
		}

	if (context->saveItemObjectVersion == objectVersion42244)
		{
		printf(".Unknown %ju .Collected %ju ", (uintmax_t) objectUnknownFlag(context, data), (uintmax_t) objectCollectedFlag(context, data));
		}
}



uintmax_t dumpFormatted(context_t *context)
{
	char *title;
	object_t *field;
	uintmax_t count;
	int formatLength;
	char *format;
	int hasTarget;
	uintmax_t printedCount;


	if (context->saveItemDataSize == 0 && context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	count = context->saveItemDataSize / (objectSize(context) * 1);

	hasTarget = 0;
	for (uintmax_t i = 0; i < count; i++)
		{
		field = (object_t *)(context->fileData + context->saveItemDataOffset + (1 * i + 0) * objectSize(context));

		if (objectObject(context, field) == context->willDumpObjectID || context->willDumpObjectID == ID_WILDCARD)
			{
			if (objectField(context, field) == context->willDumpFieldID || context->willDumpFieldID == ID_WILDCARD)
				{
				if (objectProbationalValue(context, field) == context->willDumpValue || context->willDumpValue == ID_WILDCARD)
					hasTarget = 1;
				}
			}
		}

	if (!hasTarget)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	format = context->saveItemObjectFormat;
	formatLength = strlen(format);

	printedCount = 0;
	for (uintmax_t i = 0; i < count; i++)
		{
		field = (object_t *)(context->fileData + context->saveItemDataOffset + (1 * i + 0) * objectSize(context));

		if (objectObject(context, field) != context->willDumpObjectID && context->willDumpObjectID != ID_WILDCARD)
			continue;

		if (objectField(context, field) != context->willDumpFieldID && context->willDumpFieldID != ID_WILDCARD)
			continue;

		if (objectProbationalValue(context, field) != context->willDumpValue && context->willDumpValue != ID_WILDCARD)
			continue;

		if ((i % formatLength) == 0 && (printedCount != 0) && (formatLength > 1))
			printf("\n");

		printf("\t");

		switch (format[i % formatLength])
			{
		case 'x':
			dumpField(context, field, "0x%08jx", 0);
			break;
		case 'X':
			dumpField(context, field, "0x%08jx", 1);
			break;
		case 'd':
			dumpField(context, field, "%jd", 0);
			break;
		case 'D':
			dumpField(context, field, "%jd", 1);
			break;
		case 'f':
			dumpField(context, field, "%f", 0);
			break;
		case 't':
			dumpField(context, field, "%t", 0);	// fake format string
			break;
			}

		printf("\n");
		printedCount++;
		}

	printf("}\n\n");

	return 0;
}




uintmax_t dumpJumpToSystem(context_t *context)
{
	char *title;
	uintmax_t count;
	uintmax_t i;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	count = get16(context, context->saveItemDataOffset + 0x00);
	if (sizeof(uint16_t) + count * (sizeof(uint16_t) + 3 * sizeof(uint32_t)) < context->saveItemDataSize)
		return INVALID_DATA;
	for (i = 0; i < count; i++)
		{
		printf("\t0x%04jx 0x%08jx 0x%08jx 0x%08jx\n", get16(context, context->saveItemDataOffset + 0x02 + i * 14 + 0x00), get32(context, context->saveItemDataOffset + 0x02 + i * 14 + 0x02), get32(context, context->saveItemDataOffset + 0x02 + i * 14 + 0x06), get32(context, context->saveItemDataOffset + 0x02 + i * 14 + 0x0a));
		}

	printf("}\n\n");

	return 0;
}



uintmax_t dumpStream(context_t *context)
{
	char *title;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	printf("\t(stream(s) not dumped, size in saveItem header is %ju (0x%jx))\n", get32(context, context->saveItemOffset + 0x04), get32(context, context->saveItemOffset + 0x04));

	printf("}\n\n");

	return 0;
}



uintmax_t dumpDont(context_t *context)
{
	char *title;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)	// even if size is non-zero, don't dump if empty saveItems are not dumped
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	printf("\t(useless data not dumped)\n");

	printf("}\n\n");

	return 0;
}



uintmax_t dump24(context_t *context)
{
	char *title;
	uintmax_t count;
	uintmax_t i;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	count = context->saveItemDataSize / 4;
	for (i = 0; i < count; i++)
		{
		if ( (i % 6) == 0) printf("\t");
		printf("0x%08jx ", get32(context, context->saveItemDataOffset + i * 4));
		if ( (i % 6) == 5) printf("\n");
		}
	if ( (i % 6) != 0) printf("\n");


	printf("}\n\n");

	return 0;
}



uintmax_t dump4(context_t *context)
{
	char *title;
	uintmax_t count;
	uintmax_t i;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	count = context->saveItemDataSize / 4;
	for (i = 0; i < count; i++)
		{
		if ( (i % 4) == 0) printf("\t");
		printf("0x%08jx ", get32(context, context->saveItemDataOffset + i * 4));
		if ( (i % 4) == 3) printf("\n");
		}
	if ( (i % 4) != 0) printf("\n");


	printf("}\n\n");

	return 0;
}



uintmax_t dump2(context_t *context)
{
	char *title;
	uintmax_t count;
	uintmax_t i;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	count = context->saveItemDataSize / 2;
	for (i = 0; i < count; i++)
		{
		if ( (i % 8) == 0) printf("\t");
		printf("0x%04jx ", get16(context, context->saveItemDataOffset + i * 2));
		if ( (i % 8) == 7) printf("\n");
		}
	if ( (i % 8) != 0) printf("\n");


	printf("}\n\n");

	return 0;
}



uintmax_t dump1(context_t *context)
{
	char *title;
	uintmax_t count;
	uintmax_t i;


	if (context->willDumpSaveItemID != ID_WILDCARD || context->willDumpObjectID != ID_WILDCARD || context->willDumpFieldID != ID_WILDCARD || context->willDumpValue != ID_WILDCARD)
		return 0;

	if (context->dumpOnlyUsefulNonEmptySaveItems)
		return 0;

	if (context->verbose >= 2)
		printf("at file offset 0x%jx:\n", context->saveItemOffset);

	title = textForID(context, context->saveItemID1, "0x%08jx");
	printf("saveItem \"%s\" (0x%08jx)\n", title, context->saveItemID1);
	printf("size %ju (0x%jx)\n", context->saveItemDataSize, context->saveItemDataSize);
	printf("{\n");

	count = context->saveItemDataSize;
	for (i = 0; i < count; i++)
		{
		if ( (i % 16) == 0) printf("\t");
		printf("0x%02x ", context->fileData[context->saveItemDataOffset + i]);
		if ( (i % 16) == 15) printf("\n");
		}
	if ( (i % 16) != 0) printf("\n");


	printf("}\n\n");

	return 0;
}



uintmax_t dumpUnknown(context_t *context)
{
	if ((context->saveItemDataSize % 24) == 0)
		return dump24(context);
	else if ((context->saveItemDataSize % 4) == 0)
		return dump4(context);
	else if ((context->saveItemDataSize % 2) == 0)
		return dump2(context);
	else
		return dump1(context);
}
