/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOLCONTEXT_H_
#define _LEGOTOOLCONTEXT_H_	1

#include <stdint.h>

// included files refer to context_t, so we need a forward declaration
typedef struct _context_t context_t;

#include "mappings.h"
#include "mappedFile.h"
#include "games.h"
#include "saveItems.h"



// such a context allows to pass many values without resorting to globals
typedef struct _context_t
	{
// sort of globals
	const char *myPath;
	mapping *knownIDs;
	uintmax_t knownIDsCount;
	gameIdentification game;

// frequently used IDs
	uintmax_t defaultID;
	uintmax_t unknownFlagID;
	uintmax_t collectedFlagID;
	uintmax_t discoveryStateID;
	uintmax_t numberOfItemsID;
	uintmax_t collectedID;
	uintmax_t lockedID;
	uintmax_t cLoadSaveManager_DataID;
	uintmax_t endMarkerID;

// for current file
	char *fileName;				// for current file
	char *baseFileName;			// for current file, no path nor extension
	mappedfile_t mappedFile;	// for current file
	uintmax_t fileModified;		// for current file
	uint8_t *fileData;			// for current file
	uintmax_t shellVersion;		// for current file. This is a personal numbering, nothing official.
	uintmax_t fileSize;			// for current file
	uintmax_t fileVersion;		// for current file
	float filePercentage;		// for current file
	uintmax_t loadOffset;		// for current file
	uintmax_t loadSize;			// for current file
	uintmax_t loadChecksumComputed;	// for current file

// for current data
	uintmax_t dataOffset;		// for current load block
	uintmax_t dataSize;			// for current load block
	uintmax_t unknown1;			// for current load block
	float playTime;				// for current load block
	uintmax_t playTimeHour;		// for current load block
	uintmax_t playTimeMinute;	// for current load block
	float playTimeSecond;		// for current load block
	uintmax_t dataType;			// for current load block
	intmax_t saveItemCount;		// for current load block

// for current saveItem in current data
	uintmax_t saveItemOffset;			// for current saveItem in current file
	uintmax_t saveItemDataOffset;		// for current saveItem in current file
	uintmax_t saveItemDataSize;		// for current saveItem in current file
	uintmax_t saveItemID1;				// for current saveItem in current file
	uintmax_t saveItemID2;				// for current saveItem in current file
	uintmax_t saveItemObjectVersion;	// for current saveItem in current file. This is a personal numbering, nothing official.
	char *saveItemObjectFormat;	// for current saveItem in current file

// user requestable functionalities
	int willRefillAll;
	int willRefillMineOnly;
	uintmax_t willRefillQuantity;
	int willSetCoins;
	uintmax_t willSetCoinsValue;
	int willZapValue;
	char *willZapString;
	int willRecomputeChecksums;
	int willDump;
	const char *willDumpBaseFilename;
	uintmax_t willDumpSaveItemID;
	uintmax_t willDumpObjectID;
	uintmax_t willDumpFieldID;
	uintmax_t willDumpValue;
	int willUpdatePercentage;
	float willUpdatePercentageValue;
	int willSetDebugSave;
	int willSetDebugSaveValue;
// user selectable options
	int endianness;	// 0:native, 1:little, 2: big. Not sure if there's any big endian game version.
	int verbose;
	int convertIDs;
	int dumpEmptySaveItems;
	} context_t;


#endif // _LEGOTOOLCONTEXT_H_
