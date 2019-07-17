#include <stdio.h>
#include <string.h>
#include <time.h>

#include "questLog.h"
#include "memory.h"
#include "expandableMemory.h"



// INCOMPLETE IMPLEMENTATION: no file modification allowed

int parseQuestLog(context_t *context, uint8_t **dataPtr, uintmax_t *dataSize)
	{
	uintmax_t blockLength;
	uintmax_t blockNameLength;
	uintmax_t unknown1;
	uintmax_t unknown2;
	uintmax_t count1;
	uintmax_t count2;
	uintmax_t n;
	uintmax_t quest;
	uintmax_t date1;
	uintmax_t date2;
	uintmax_t date3;
	uintmax_t questType;
	uintmax_t printed;


	staticExpandableMemory(char *, blockName, blockNameAllocation);
	time_t t;
	struct tm tm;
	char timeString[64];
	
	printf("******************** dump of %s ********************\n", context->fileName);
	t = time(NULL);
	if (ctime_r(&t, timeString) != NULL)
		{
		while (timeString[strlen(timeString) - 1] == '\n')
			timeString[strlen(timeString) - 1] = '\0';
		printf("dumped on: %s\n", timeString);
		}
	printf("game: %s\n", gameIdentificationToName(context->game));

	while (*dataSize)
		{
		if (*dataSize < 4)	// 'physical' end of data
			return -2;

		blockLength = read32(*dataPtr, context->endianness);
		*dataPtr += 4;
		*dataSize -= 4;

		if (blockLength == 0)	// 'logical' end of data
			return -1;

		if (*dataSize < 4)
			return 1;
		blockNameLength = read32(*dataPtr, context->endianness);
		*dataPtr += 4;
		*dataSize -= 4;

		expandMemory(blockName, blockNameAllocation, blockNameLength + 1);

		if (*dataSize < blockNameLength)
			return 1;
		memcpy(blockName, *dataPtr, blockNameLength + 1);	// add an extra byte for a trailing 0 to ensure alls trings are terminated
		*dataPtr += blockNameLength;
		*dataSize -= blockNameLength;
		blockName[blockNameLength] = 0;

		if (!strcmp(blockName, "StreamInfo"))
			{
			if (*dataSize < 4)
				return 1;
			unknown1 = read32(*dataPtr, context->endianness);
			*dataPtr += 4;
			*dataSize -= 4;
			}
		else if (!strcmp(blockName, "QuestLogMeta"))
			{
			if (*dataSize < 4)
				return 1;
			unknown1 = read32(*dataPtr, context->endianness);
			*dataPtr += 4;
			*dataSize -= 4;

			if (*dataSize < 4)
				return 1;
			unknown2 = read32(*dataPtr, context->endianness);
			*dataPtr += 4;
			*dataSize -= 4;
			}
		else if (!strcmp(blockName, "QuestLogData"))
			{
			if (*dataSize < 4)
				return 1;
			count1 = read32(*dataPtr, context->endianness);
			*dataPtr += 4;
			*dataSize -= 4;

			printed = 0;
			for (uintmax_t i = 0; i < count1; i++)
				{
				if (*dataSize < 4)
					return 1;
				n = read32(*dataPtr, context->endianness);
				*dataPtr += 4;
				*dataSize -= 4;

				if (*dataSize < 4)
					return 1;
				count2 = read32(*dataPtr, context->endianness);
				*dataPtr += 4;
				*dataSize -= 4;

				if (n == 0)
					{
					printf("\n");
					printf("// %ju activated but not started quests\n", count2);
					printf("\n");
					printed = 0;
					}
				else if (n == 1)
					{
					printf("\n");
					printf("// %ju started but not completed quests\n", count2);
					printf("\n");
					printed = 0;
					}
				else if (n == 2)
					{
					printf("\n");
					printf("// %ju completed quests\n", count2);
					printf("\n");
					printed = 0;
					}
				else
					{
					printf("\n");
					printf("// %ju unknown state %ju quests\n", count2, n);
					printf("\n");
					printed = 0;
					}

				for (uintmax_t j = 0; j < count2; j++)
					{
					if (printed) printf("\n");
					printed++;

					if (*dataSize < 4)
						return 1;
					quest = read32(*dataPtr, context->endianness);
					*dataPtr += 4;
					*dataSize -= 4;

					if (*dataSize < 4)
						return 1;
					n = read32(*dataPtr, context->endianness);
					*dataPtr += 4;
					*dataSize -= 4;

					if (n >= 0)
						{
						if (*dataSize < 8)
							return 1;
						date1 = read64(*dataPtr, context->endianness);
						*dataPtr += 8;
						*dataSize -= 8;
						}
					if (n >= 1)
						{
						if (*dataSize < 8)
							return 1;
						date2 = read64(*dataPtr, context->endianness);
						*dataPtr += 8;
						*dataSize -= 8;
						}
					if (n >= 2)
						{
						if (*dataSize < 8)
							return 1;
						date3 = read64(*dataPtr, context->endianness);
						*dataPtr += 8;
						*dataSize -= 8;
						}

					if (*dataSize < 4)
						return 1;
					questType = read32(*dataPtr, context->endianness);
					*dataPtr += 4;
					*dataSize -= 4;

					printf("%s.Default %s\n", textForID(context, quest, "0x%08jx"), textForID(context, questType, "0x%08jx"));
					if (n >= 0)
						{
						// Quest activated, e.g. big ! displayed
						printf("%s.Activated ", textForID(context, quest, "0x%08jx"));	// fake state
						t = date1;
						if (t && gmtime_r(&t, &tm))
							{
							// gmtime required, NOT localtime, even if this is NOT an UTC time_t
							// the displayed time will then be the local time *where* and *when* game timestamp was taken
							asctime_r(&tm, timeString);
							if (timeString[strlen(timeString) - 1] == '\n') timeString[strlen(timeString) - 1] = 0;
							printf("0x%08jx [%s] \n", date1, timeString);
							}
						else
							printf("0x%08jx \n", date1);
						}
					if (n >= 1)
						{
						// Quest accepted, e.g. started by player
						printf("%s.Started ", textForID(context, quest, "0x%08jx"));	// fake state
						t = date2;
						if (t && gmtime_r(&t, &tm))
							{
							// gmtime required, NOT localtime, even if this is NOT an UTC time_t
							// the displayed time will then be the local time *where* and *when* game timestamp was taken
							asctime_r(&tm, timeString);
							if (timeString[strlen(timeString) - 1] == '\n') timeString[strlen(timeString) - 1] = 0;
							printf("0x%08jx [%s] \n", date2, timeString);
							}
						else
							printf("0x%08jx \n", date2);
						}
					if (n >= 2)
						{
						// Quest completed
						printf("%s.Completed ", textForID(context, quest, "0x%08jx"));	// fake state
						t = date3;
						if (t && gmtime_r(&t, &tm))
							{
							// gmtime required, NOT localtime, even if this is NOT an UTC time_t
							// the displayed time will then be the local time *where* and *when* game timestamp was taken
							asctime_r(&tm, timeString);
							if (timeString[strlen(timeString) - 1] == '\n') timeString[strlen(timeString) - 1] = 0;
							printf("0x%08jx [%s] \n", date3, timeString);
							}
						else
							printf("0x%08jx \n", date3);

						}
					}
				}

			}
		}

	return 0;
}
