/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <libgen.h>
#include <assert.h>
#include <time.h>

#include "legotool.h"

#include "memory.h"
#include "saveItems.h"
#include "objects.h"
#include "mappings.h"
#include "ids.h"
#include "games.h"
#include "questLog.h"



#define LEGOTOOL_VERSION "0.5.1"
#define LEGOTOOL_DATE "31-MAR-2019"
//
//	** Version history: **
//	Version	Date			Comment
//
//                          -s option added
//	0.5.1	31-MAR-2019		CamelCase corrections to movie2/id.c
//                          -g option made mandatory again
//                          completed saveItems format for movie2
//                          updated dump avoidance logic for useless/empty saveItems
//	0.5		30-MAR-2019		added "The Lego Movie 2" support
//							dump header now includes timestamp to ease comparisons
//							-v now increases verbosity level. file offsets and checksums are logged at verbosity level 2.
//							-i now only update item if its quantity is already > 1
//							removed -s option
//							added -C option to ease development
//	0.4		16-NOV-2018		-D argument now a complete specification
//							for batman3, MainCoinTotal now changed in "CoinsVariables" instead of "Coins" saveItem
//							removed overrides
//	0.3.3	14-NOV-2018		renamed chunks to saveItems and minor adjustments
//	0.3.2	04-NOV-2018		small enhancements
//	0.3.1	31-OCT-2018		minor corrections
//							corrected -x functionality
//							corrected completion or discoveries count display
//	0.3		29-OCT-2018		completed "Villains" support
//							various bug corrections
//							enhanced world VOCollectables display
//							added some missing mappings
//							corrected some saveItems formats
//	0.2		27-OCT-2018		added "batman3", "jurassic", "forceawakens" support
//							added -s option to legotool
//							corrected load checksum computation
//							mapping now use game instead of fileVersion
//	0.1		16-OCT-2018		added verbose mode, baseFileName/ prefix in -Z (zap)
//							corrected and completed avengers game01 dump
//	0.0		10-OCT-2018		First somewhat polished version
//

uint32_t checksum(const void *data, uintmax_t size)
{
	return ~FNV1DataHash32(0xffffffff, data, size);
}



void updateFileChecksums(context_t *context)
{
	uint32_t newChecksum;
	uint32_t checksumOnFile;

	if (context->fileSize < 0x14) return;	// invalid data

	// 1 - fileVersion checksum
	newChecksum = checksum(context->fileData + 0x00, sizeof(uint32_t));
	checksumOnFile = get32(context, 0x04);
	if (checksumOnFile != newChecksum)
		{
		context->fileModified++;
		if (context->verbose >= 2)
			printf("fileVersionChecksum: %s updated from 0x%08jx to 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile, (uintmax_t) newChecksum);
		set32(context, 0x04, newChecksum);
		}
	else
		if (context->verbose >= 2)
			printf("fileVersionChecksum: %s up to date, 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile);

	// 2 - percentage checksum
	newChecksum = checksum(context->fileData + 0x08, sizeof(float));
	checksumOnFile = get32(context, 0x0c);
	if (checksumOnFile != newChecksum)
		{
		context->fileModified++;
		if (context->verbose >= 2)
			printf("percentageChecksum:  %s updated from 0x%08jx to 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile, (uintmax_t) newChecksum);
		set32(context, 0x0c, newChecksum);
		}
	else
		if (context->verbose >= 2)
			printf("percentageChecksum:  %s up to date, 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile);

	// 3 - load checksum
	newChecksum = context->loadChecksumComputed;
	checksumOnFile = get32(context, 0x10);

	if (checksumOnFile != newChecksum)
		{
		context->fileModified++;
		if (context->verbose >= 2)
			printf("loadChecksum:        %s updated from 0x%08jx to 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile, (uintmax_t) newChecksum);
		set32(context, 0x10, newChecksum);
		}
	else
		if (context->verbose >= 2)
			printf("loadChecksum:        %s up to date, 0x%08jx\n", context->baseFileName, (uintmax_t) checksumOnFile);
}



void setPercentage(context_t *context, float newPercentage)
{
	float oldPercentage;


	if (context->fileSize < 0x10) return;	// invalid data
	* (uint32_t *) &oldPercentage = get32(context, context->dataOffset + 0x08);
	if (newPercentage != oldPercentage)
		{
		context->fileModified++;
		context->filePercentage = context->willUpdatePercentageValue;
		set32(context, context->dataOffset + 0x08, * (uint32_t *) &context->filePercentage);
		}
}



uintmax_t addSaveItemSizeToDataSize(context_t *context, uintmax_t userData)
{
	context->dataSize += sizeof(uint32_t) + sizeof(uint32_t) + context->saveItemDataSize + sizeof(uint32_t);
	return 0;
}



void printID(const mapping* id)
{
printf("{ ");
printf("0x%08jx", id->key);
printf(", ");
printf("%2jd", id->game);
printf(", ");
if (((intmax_t) id->saveItemID) >= 0) printf("0x%08jx", id->saveItemID); else  printf("-%ju", -id->saveItemID);
printf(", ");
printf("0x%jx", id->flags);
printf(", ");
printf("\"%s\"", id->label ? id->label : "(null)");
printf(", ");
printf("\"%s\"", id->string ? id->string : "(null)");
printf(", ");
printf(id->redirection ? "\"%s\"" : "NULL", id->redirection);
printf(" },");
}



void dumpNonMatchingIDs(context_t *context)
{
uintmax_t key;
char *label;
uintmax_t flags;
int headerPrinted = 0;

for (int i = 0; i < context->knownIDsCount; i++)
	{
	key = context->knownIDs[i].key;
	label = context->knownIDs[i].label;
	flags = context->knownIDs[i].flags;
	if (label)
		{
		if (!(flags & FLAGS_OVERRIDE) && label && (label[0]) && (FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, label) != key))
			{
			if (!headerPrinted)
				{
				printf("non-matching key/non-empty-label pairs:\n");
				printf("*** overrides should have flags |= 0x%jx ***\n", (uintmax_t) FLAGS_OVERRIDE);
				headerPrinted = 1;
				}
			printID(&context->knownIDs[i]);
			printf("\n");
			}
		}
	}

return;
}



void dumpAllIDs(context_t *context)
{
for (int i = 0; i < context->knownIDsCount; i++)
	{
	printID(&context->knownIDs[i]);
	printf("\n");
	}
}



void dumpAllLabels(context_t *context)
{
for (int i = 0; i < context->knownIDsCount; i++)
	{
	printf("%s\n", context->knownIDs[i].label);
	}
}



void dumpDuplicateIDs(context_t *context)
{
int headerPrinted;
int alreadyListed;


for (int i = 0; i < context->knownIDsCount; i++)
	{
	alreadyListed = 0;
	headerPrinted = 0;

	// do not handle already listed duplicates
	for (int j = 0; j < i; j++)
		if (context->knownIDs[i].key == context->knownIDs[j].key)
			{
			alreadyListed = 1;
			}

	if (!alreadyListed)
		{
		for (int j = i + 1; j < context->knownIDsCount; j++)
			{
			if (context->knownIDs[i].key == context->knownIDs[j].key)
				{
				if (!headerPrinted)
					{
					printf("Duplicate ID:\n");
					printID(&context->knownIDs[i]);
					printf("\n");
					headerPrinted = 1;
					}
				printID(&context->knownIDs[j]);
				printf("\n");
				}
			}
		}
	}
}



void usage(context_t *context, int exitCode)
{
const char *myBasename;

myBasename = basename((char *) context->myPath);
if (!myBasename) myBasename = context->myPath;

fprintf(stderr, "usage: %s -g gameName [options] [file]\n", myBasename);
fprintf(stderr, "         or:\n");
fprintf(stderr, "       %s [-A|-B|-C|-N]\n", myBasename);
fprintf(stderr, "           options:\n");
fprintf(stderr, "       -c value            set Coins[Variables]:MainCoinTotal to given value\n");
fprintf(stderr, "       -d                  dump saveItems (after any modification)\n");
fprintf(stderr, "       -D '[baseFileName/]saveItem:object.field=value'\n");
fprintf(stderr, "                           dumps only matching specification.\n");
fprintf(stderr, "                           If any of saveItem, object, field or value is not '*', non-saveItems aren't dumped.\n");
fprintf(stderr, "       -E endianness       sets endianness: 0:native, 1:little, 2:big (default: native)\n");
fprintf(stderr, "                               probably useless\n");
fprintf(stderr, "       -g gameName         select game, as legotool adapts some functionality according to game\n");
fprintf(stderr, "                           The game name may be abbreviated as long as it uniquely identifies a game\n");
fprintf(stderr, "                           Supported game names are:\n");
fprintf(stderr, "                               batman3         for Lego Batman 3: Beyond Gotham\n");
fprintf(stderr, "                               jurassic        for Lego Jurassic World\n");
fprintf(stderr, "                               avengers        for Lego Marvel's Avengers\n");
fprintf(stderr, "                               theforceawakens for Lego Star Wars: The Force Awakens\n");
fprintf(stderr, "                                               also: forceawakens or tfa\n");
fprintf(stderr, "                               worlds          for Lego Worlds\n");
fprintf(stderr, "                               ninjago         for The Lego Ninjago Movie Videogame\n");
fprintf(stderr, "                               incredibles     for Lego The Incredibles\n");
fprintf(stderr, "                               superheroes2    for Lego Marvel Super Heroes 2\n");
fprintf(stderr, "                                               also: sh2\n");
fprintf(stderr, "                               villains        for Lego DC Super-Villains\n");
fprintf(stderr, "                               movie2          for The Lego Movie 2\n");
fprintf(stderr, "       -i quantity         update LEGO Worlds items that are unlocked and present in 2+ quantity to given quantity\n");
fprintf(stderr, "                           Depleted items are updated as one item availabe\n");
fprintf(stderr, "                           Updated items will be marked by a star\n");
fprintf(stderr, "                           Beware that some quests require some items to be absent from you inventory!\n");
fprintf(stderr, "                           If multiple -i are given, only the last is executed.\n");
fprintf(stderr, "       -I quantity         same as -i but for ALL inventory items, even ones you didn't discover yet\n");
fprintf(stderr, "                           Quests leading to discovery of new items are NOT updated.\n");
fprintf(stderr, "       -k                  force checksums update (default is off but on if file is modified)\n");
fprintf(stderr, "       -n                  do not convert IDs to names (default: convert IDs to names)\n");
fprintf(stderr, "       -s                  print save item name in front of values (default: no)\n");
fprintf(stderr, "       -v                  verbose mode (default: off)\n");
fprintf(stderr, "                           repeating -v adds a level of verbosity\n");
fprintf(stderr, "                           checksums change are displayed at verbosity level 2+\n");
fprintf(stderr, "       -V                  display %s version\n", myBasename);
fprintf(stderr, "       -x value            set DebugSaveItem byte value\n");
fprintf(stderr, "                           If multiple -x are given, only the last is executed.\n");
fprintf(stderr, "       -z                  also dump empty or useless saveItems (default: off)\n");
fprintf(stderr, "                              requires -d ou -D\n");
fprintf(stderr, "       -Z '[baseFileName/]saveItem:object.field=value'\n");
fprintf(stderr, "                           set saveItem:object.field = value\n");
fprintf(stderr, "                           if baseFileName is present, this only applies to a file if its basename\n");
fprintf(stderr, "                              matches the given one. The baseFileName is the filename without\n");
fprintf(stderr, "                              directory nor extension. baseFileName takes no wildcard\n");
fprintf(stderr, "                           quotes around 'baseFileName/saveItem:object.field=value' seems unavoidable\n");
fprintf(stderr, "                           the saveItem, object, field you dont specify is considered wildcard\n");
fprintf(stderr, "                              decimal numbers must be preceded by '%%' e.g. %%999\n");
fprintf(stderr, "                              hex numbers must be preceded by '%%0x' e.g. %%0xdeadbeef\n");
fprintf(stderr, "                              float numbers must be preceded by '%%%%' e.g. %%%%0.25\n");
fprintf(stderr, "                              '*' is a wildcard that stands for 'match any'\n");
fprintf(stderr, "                           If multiple -Z are given, only the last one is executed.\n");
fprintf(stderr, "       -%% floatNumber      update completion percentage/discoveries count\n");
fprintf(stderr, "\n");
fprintf(stderr, "           options that do not operate on a file:\n");
fprintf(stderr, "       -A                  dump all known IDs\n");
fprintf(stderr, "       -B                  list duplicates IDs\n");
fprintf(stderr, "       -C                  dump all known ID labels\n");
fprintf(stderr, "       -N                  dump non matching known keys/label (except empty and overridden ones)\n");
fprintf(stderr, "                           (useful to check mappings)\n");


if (exitCode) exit(exitCode);
}



int main(int argc, char **argv)
{
context_t context;
int displayVersion = 0;
int willDumpAllIDs = 0;
int willDumpAllLabels = 0;
int willDumpDuplicateIDs = 0;
int willDumpNonMatchingIDs = 0;
int ch;
int loadDataAtEnd;
int splitResult;
intmax_t i, j, k, l;

assert(sizeof(uintmax_t) > sizeof(uint32_t));
// setup context
context.myPath = argv[0];
context.knownIDs = globalKnownIDs;
context.knownIDsCount = globalKnownIDsCount;
context.game = gameNotDefined;

context.defaultID               = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Default");
context.unknownFlagID           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Unknown");
context.collectedFlagID         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Collected");
context.discoveryStateID        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "DiscoveryState");
context.numberOfItemsID         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "NumberOfItems");
context.collectedID             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Collected");
context.lockedID                = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Locked");
context.unlockedDepletedID      = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "UnlockedDepleted");
context.cLoadSaveManager_DataID = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "cLoadSaveManager_Data");
context.endMarkerID     = 123456789;	// = 0x075bcd15 in hex, not a FNV-based ID

context.fileName = NULL;
context.fileModified = 0;
context.shellVersion = 0;
context.filePercentage = 0;
context.fileData = NULL;
context.fileSize = 0;
context.dataOffset = 0;
context.fileVersion = 0;
context.saveItemCount = 0;

context.saveItemOffset = 0;
context.saveItemDataOffset = 0;
context.saveItemDataSize = 0;
context.saveItemID1 = ID_WILDCARD;
context.saveItemID2 = ID_WILDCARD;

// defaults
context.endianness = 0;	// 0:native, 1:little, 2: big
context.verbose = 0;
context.willRecomputeChecksums = 0;
context.willDump = 0;
context.willRefillAll = 0;
context.willRefillMineOnly = 0;
context.willRefillQuantity = 1;
context.willPrintSaveItemName = 0;
context.convertIDs = 1;
context.willZapValue = 0;
context.willZapString = NULL;
context.dumpOnlyUsefulAndNonEmptySaveItems = 1;
context.willUpdatePercentage = 0;
context.willUpdatePercentageValue = 0;
context.willSetDebugSave = 0;
context.willSetDebugSaveValue = 0;
context.willSetCoins = 0;
context.willSetCoinsValue = 0;

qsort(context.knownIDs, context.knownIDsCount, sizeof(*context.knownIDs), hashCompareFunction);

while ((ch = getopt(argc, argv, "%:ABCc:dD:E:g:i:I:knNsSvVx:zZ:?")) != -1)
	{
	switch (ch)
		{
	case '%':
		context.willUpdatePercentage = 1;
		context.willUpdatePercentageValue = strtof(optarg, NULL);
		break;
	case 'A':
		willDumpAllIDs = 1;
		break;
	case 'B':
		willDumpDuplicateIDs = 1;
		break;
	case 'C':
		willDumpAllLabels = 1;
		break;
	case 'c':
		context.willSetCoins = 1;
		context.willSetCoinsValue = strtoumax(optarg, NULL, 0);
		break;
	case 'd':
		context.willDump = 1;
		context.willDumpBaseFilename = NULL;
		context.willDumpSaveItemID = ID_WILDCARD;
		context.willDumpObjectID = ID_WILDCARD;
		context.willDumpFieldID = ID_WILDCARD;
		context.willDumpValue = ID_WILDCARD;
		break;
	case 'D':
		context.willDump = 1;
		splitResult = splitSaveItemObjectFieldValueString(&context, optarg, &context.willDumpBaseFilename, &context.willDumpSaveItemID, &context.willDumpObjectID, &context.willDumpFieldID, &context.willDumpValue);
		if (splitResult != 0) usage(&context, 1);
		break;
	case 'E':
		context.endianness = strtoumax(optarg, NULL, 0);
		if (context.endianness < 0 || context.endianness > 2) usage(&context, 1);
		break;
	case 'g':
		context.game = nameToGameIdentification(optarg);
		if (context.game == gameNotDefined)
			{
			fprintf(stderr, "error: %s is not a valid game name.\n", optarg);
			exit(1);
			}
		break;
	case 'i':
		context.willRefillMineOnly = 1;
		context.willRefillAll = 0;
		context.willRefillQuantity = strtoumax(optarg, NULL, 0);
		break;
	case 'I':
		context.willRefillAll = 1;
		context.willRefillMineOnly = 0;
		context.willRefillQuantity = strtoumax(optarg, NULL, 0);
		break;
	case 'k':
		context.willRecomputeChecksums = 1;
		break;
	case 'n':
		context.convertIDs = 0;
		break;
	case 'N':
		willDumpNonMatchingIDs = 1;
		break;
	case 's':
		context.willPrintSaveItemName = 1;
		break;
	case 'v':
		context.verbose++;
		break;
	case 'V':
		displayVersion = 1;
		break;
    case 'x':
		context.willSetDebugSave = 1;
		context.willSetDebugSaveValue = strtoumax(optarg, NULL, 0);
		break;
	case 'z':
		context.dumpOnlyUsefulAndNonEmptySaveItems = 0;
		break;
	case 'Z':
		context.willZapValue = 1;
		context.willZapString = optarg;
		break;
	case '?':
	default:
		usage(&context, 1);
		break;
		}
	}
argc -= optind;
argv += optind;
// remember: remaining args are now in argv[0] and up

if (displayVersion) printf("legotool version %s, %s\n", LEGOTOOL_VERSION, LEGOTOOL_DATE);

// no-file operations
if (willDumpAllIDs || willDumpAllLabels || willDumpDuplicateIDs || willDumpNonMatchingIDs)
	{
	if (argc) usage(&context, 1);
	if (willDumpAllIDs) dumpAllIDs(&context);
	if (willDumpAllLabels) dumpAllLabels(&context);
	if (willDumpDuplicateIDs) dumpDuplicateIDs(&context);
	if (willDumpNonMatchingIDs) dumpNonMatchingIDs(&context);

	return 0;
	}

if (displayVersion && argc == 0) return 0;		// -V and no file: that's OK
if (argc != 1) usage(&context, 1);				// otherwise, require a single file name

// file operations

if (context.game == gameNotDefined)
	{
	fprintf(stderr, "error: you must use -g gameName\n");
	exit(1);
	}

// set up context.shellVersion according to game
switch (context.game)
	{
case gameBatman3:
case gameJurassic:
	context.shellVersion = 1;	// without loadSize
	break;
default:
	context.shellVersion = 2;	// with loadSize
	break;
	}

if ((context.willRefillAll || context.willRefillMineOnly) && context.game != gameWorlds)
	usage(&context, 1); // willRefill is for worlds only

if (!context.willDump && !context.willRecomputeChecksums && !context.willUpdatePercentage && !context.willSetDebugSave && !context.willRefillAll && !context.willRefillMineOnly && !context.willZapValue && !context.willSetCoins)
	{ // nothing to do? just dump
	context.willDump = 1;
	context.willDumpBaseFilename = NULL;
	context.willDumpSaveItemID = ID_WILDCARD;
	context.willDumpObjectID = ID_WILDCARD;
	context.willDumpFieldID = ID_WILDCARD;
	context.willDumpValue = ID_WILDCARD;
	}

context.fileName = argv[0];

// get basename
l = strlen(context.fileName);
i = 0;
while ((i < l) && (context.fileName[l - 1 - i] != '/')) i++;

// remove extension
j = 0;
while ((j < i) && (context.fileName[l - 1 - j] != '.')) j++;
if (j == i)
{
	j = -1;
	context.fileExtension = malloc(1);
	if (context.fileExtension == NULL) { perror(NULL); exit(1); }
	context.fileExtension[0] = 0;
}
else
{
	context.fileExtension = malloc(j + 1);
	if (context.fileExtension == NULL) { perror(NULL); exit(1); }
	strcpy(context.fileExtension, context.fileName + l - j);
}

context.baseFileName = malloc(i - j + 1);
if (context.baseFileName == NULL) { perror(NULL); exit(1); }
for (k = 0; k < i - j - 1; k++) context.baseFileName[k] = context.fileName[l - i + k];
context.baseFileName[k] = 0;

context.fileData = mapFileAtPath(context.fileName, &context.mappedFile, MAPPEDFILE_RDWR);
if (context.fileData == NULL) { perror(context.fileName); exit(2); }
context.fileSize = context.mappedFile.size;
context.fileVersion = get32(&context, 0x0);
* (uint32_t *) &context.filePercentage = get32(&context, 0x8);
context.fileModified = 0;
if (context.shellVersion == 1)
	context.loadOffset = 0x14;
else
	context.loadOffset = 0x18;
context.loadSize = 0;	// will be updated with each load block


if (context.game == gameWorlds && context.willDump && !strcasecmp(context.baseFileName, "autosave") && !strcasecmp(context.fileExtension, "gamesave"))
	{
	uint8_t *data;
	uintmax_t dataSize;

	data = context.fileData;
	dataSize = context.fileSize;
	parseQuestLog(&context, &data, &dataSize);

	return 0;
	}

if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
	{
	time_t date;
	char dateString[256];
	
	printf("******************** dump of %s ********************\n", context.fileName);
	date = time(NULL);
	if (ctime_r(&date, dateString) != NULL)
		{
		while (dateString[strlen(dateString) - 1] == '\n')
			dateString[strlen(dateString) - 1] = '0';
		printf("dumped on: %s\n", dateString);
		}
	printf("game: %s\n", gameIdentificationToName(context.game));
	printf("fileVersion: %2ju\n", context.fileVersion);
	printf("completion percentage (or discoveries count): %f\n", context.filePercentage);
	printf("\n");
	}

// whole file or header actions
if (context.willUpdatePercentage) setPercentage(&context, context.willUpdatePercentageValue);

// now loop over all game file load blocks
switch (context.shellVersion)
	{
case 1:
	context.loadChecksumComputed = 0xffffffff;
	context.dataOffset = 0x14;
	break;
case 2: default:
	context.loadChecksumComputed = 0xffffffff;
	context.dataOffset = 0x18;
	break;
	}

for (loadDataAtEnd = 0; loadDataAtEnd == 0; )
	{
	if (context.dataOffset >= context.fileSize)
		{
		fprintf(stderr, "error: past EOF.\n");
		exit(1);
		}
	// loop over load data blocks
	if (context.verbose >= 2 && context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
		printf("at file offset 0x%jx:\n", context.dataOffset);

	// current load data block is at context.dataOffset, you must at least:
	//   - set context.dataSize (should be > 0)
	//   - set loadDataAtEnd (0 if this is not the last block, 1 if this is the last)
	//   - update context.loadChecksumComputed
	// operation principle:
	//   - identify load block
	//   - set context.dataSize
	//   - set loadDataAtEnd
	//   - modify block if needed
	//   - update computedChecksum
	//   - dump block if needed
	context.dataSize = 0;

	if ((context.shellVersion == 1) && (get32(&context, context.dataOffset + 0x00) == context.endMarkerID))
		{	// shellVersion 1 endMarker
		loadDataAtEnd = 1;
		if (context.game == gameBatman3 && !strncmp(context.baseFileName, "dlc", 3))
			context.dataSize = sizeof(uint32_t);			// batman3 && dlc*
		else
			context.dataSize = sizeof(uint32_t) + 0x400;	// game*

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: end marker\n");
			printf("dataSize: %ju (0x%jx)\n", context.dataSize, context.dataSize);
			}

//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);
		}
	else if ((context.shellVersion == 2) && (get32(&context, context.dataOffset + 0x00) == context.endMarkerID))
		{	// shellVersion 2 endMarker
		loadDataAtEnd = 1;
		if (context.dataOffset + 4 < context.fileSize)
			context.dataSize = get32(&context, 0x14) + context.loadOffset - context.dataOffset;	// up to end of loadDataSize as indicated in header
		else
			{
			fprintf(stderr, "error: reading past EOF.\n");
			exit(1);
			}

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: end marker\n");
			printf("dataSize: %ju (0x%jx)\n", context.dataSize, context.dataSize);
			}

//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);
		}
	else if ((context.game == gameAvengers) && (get32(&context, context.dataOffset + 0x08) != context.cLoadSaveManager_DataID))
		{	// avenger additional block
		loadDataAtEnd = 0;
		context.dataSize = sizeof(uint32_t) + sizeof(float) + sizeof(uint32_t) + 1024; // hard coded size, it seems

//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);

		context.unknown1 = get32(&context, context.dataOffset + 0x00);
		* ((uint32_t *) &context.playTime) = get32(&context, context.dataOffset + 0x04);
		context.playTimeHour = context.playTime / 3600;
		context.playTimeMinute = (context.playTime - context.playTimeHour * 3600) / 60;
		context.playTimeSecond = context.playTime - context.playTimeHour * 3600 - context.playTimeMinute * 60;
		context.dataType = get32(&context, context.dataOffset + 0x08);

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: playTime block for avengers\n");
			printf("dataSize %ju (0x%jx)\n", context.dataSize, context.dataSize);
			printf("unknown: 0x%08jx, playTime: %juh %jum %fs\n", context.unknown1, context.playTimeHour, context.playTimeMinute, context.playTimeSecond);
			printf("(%ju bytes not dumped)\n", context.dataSize - 4 - 4 - 4);
			printf("\n");
			}
		}
	else if ((context.game == gameBatman3) && (get32(&context, context.dataOffset + 0x08) != context.cLoadSaveManager_DataID))
		{	// batman3 additional block
		loadDataAtEnd = 0;
		context.dataSize = 2 + sizeof(uint16_t) + get16(&context, context.dataOffset + 0x02) + 2;

//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: non-saveItem block for batman3\n");
			printf("dataSize %ju (0x%jx)\n", context.dataSize, context.dataSize);
			printf("(not dumped)\n");
			printf("\n");
			}
		}
	else if ((context.game == gameJurassic) && (get32(&context, context.dataOffset + 0x08) != context.cLoadSaveManager_DataID))
		{	// batman3 additional block
		loadDataAtEnd = 0;
		context.dataSize = 0x438;

//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: non-saveItem block for jurassic\n");
			printf("dataSize %ju (0x%jx)\n", context.dataSize, context.dataSize);
			printf("(not dumped)\n");
			printf("\n");
			}
		}
	else if (get32(&context, context.dataOffset + 0x08) == context.cLoadSaveManager_DataID)
		{	// 'standard' saveItem block
		loadDataAtEnd = 0;
		context.dataSize = sizeof(uint32_t) + sizeof(float) + sizeof(uint32_t) + sizeof(uint32_t) ;

		if (forEachSaveItem(&context, addSaveItemSizeToDataSize, 0) == INVALID_DATA)
			{
			fprintf(stderr, "error: file format not as expected.\n");
			exit(1);
			}

		// saveItem modifying actions
		if (context.willSetDebugSave)     if (forEachSaveItemWithName(&context, setDebugSaveItem, context.willSetDebugSaveValue, "DebugSaveItem") == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }
		// (most automatic first, giving precedence to most manual ones)
		if (context.willRefillAll)        if (forEachSaveItemWithName(&context, refillAllProps,  context.willRefillQuantity,     "Items") == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }
		if (context.willRefillMineOnly)   if (forEachSaveItemWithName(&context, refillMyProps,   context.willRefillQuantity,     "Items") == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }
		if (context.willSetCoins)         if (forEachSaveItemWithName(&context, setCoins,        context.willSetCoinsValue,      (context.game == gameBatman3) ? "CoinsVariables" : "Coins") == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }
		if (context.willZapValue)         if (forEachSaveItem(&context, zap, (uintmax_t) context.willZapString) == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }

		// actions after all modifications have been done
//		printf("checksum offset 0x%jx length 0x%jx end 0x%jx\n", context.dataOffset, context.dataSize, context.dataOffset + context.dataSize); //***********
		context.loadChecksumComputed = FNV1DataHash32(context.loadChecksumComputed, context.fileData + context.dataOffset, context.dataSize);

		context.unknown1 = get32(&context, context.dataOffset + 0x00);
		* ((uint32_t *) &context.playTime) = get32(&context, context.dataOffset + 0x04);
		context.playTimeHour = context.playTime / 3600;
		context.playTimeMinute = (context.playTime - context.playTimeHour * 3600) / 60;
		context.playTimeSecond = context.playTime - context.playTimeHour * 3600 - context.playTimeMinute * 60;
		context.dataType = get32(&context, context.dataOffset + 0x08);

		if (context.willDump && (!context.willDumpBaseFilename || !strcasecmp(context.willDumpBaseFilename, context.baseFileName)))
			{
			printf("dataType: %s\n", textForID(&context, context.dataType, "0x%08jx"));
			printf("dataSize %ju (0x%jx)\n", context.dataSize, context.dataSize);
			printf("unknown: 0x%08jx, playTime: %juh %jum %fs\n", context.unknown1, context.playTimeHour, context.playTimeMinute, context.playTimeSecond);
			printf("saveItemCount: %jd\n", context.saveItemCount);
			printf("\n");
			if (forEachSaveItemWithID(&context, dumpSaveItem, 0, context.willDumpSaveItemID) == INVALID_DATA) { fprintf(stderr, "error: file format not as expected.\n"); exit(1); }
			}
		}

	else
		{ // unknown block. Abort
		fprintf(stderr, "error: unknown load block, can't proceed\n");
		exit(1);
		}

	context.loadSize += context.dataSize;
	context.dataOffset += context.dataSize;
	}

context.loadChecksumComputed = context.loadChecksumComputed ^ 0xffffffff;
if (!context.fileModified && context.loadChecksumComputed != get32(&context, 0x10))
	{
	printf("warning: computed load checksum mismatch, computed 0x%08jx, file 0x%08jx\n", context.loadChecksumComputed, (uintmax_t) get32(&context, 0x10));
	}

if (context.willRecomputeChecksums || context.fileModified)
	{
	updateFileChecksums(&context);
	}

unmapFile(&context.mappedFile, context.fileModified ? MAPPEDFILE_WRITE : MAPPEDFILE_DISCARD);

return 0;
}



