/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_MAPPINGS_H_
#define _LEGOTOOL_MAPPINGS_H_	1

#include <stdint.h>


typedef struct _mapping mapping;
#include "legotoolContext.h"

#define ID_WILDCARD			((uintmax_t)-1)	// no collision possible as IDs are 32 bits only and uintmax_t > 32 bits

#define FLAGS_FINAL		(1 << 0)
#define FLAGS_OVERRIDE	(1 << 1)

// mapping between a LABEL and a KEY.
// label is the label name
// key is the hash, normally key = FNV1UppercaseStringHash32(label), BUT might be overriden so key is not completely predictable.
// string, if not empty, is a human readable text, mostly extracted from TEXT.CSV and maybe other data files (probably extracted by quickBMS)
// game, saveItemID specify where this mapping is used
// flags, FLAGS_FINAL, means that the human readable string shouldn't not be updated by automated tools.
//        FLAGS_OVERRIDE, means this is a required override, i.e. key != FNV1UppercaseStringHash32(label)
//        flags can be ORed
// redirection, if not NULL redirects human readable string to specified label (1 redirection level ONLY).
typedef struct _mapping { uintmax_t key; uintmax_t game; uintmax_t saveItemID; uintmax_t flags; char *label; char *string; char *redirection; } mapping;



// text <==> ID
int IDForText(context_t *context, uintmax_t *resultID, const char *text);
char *textForID(context_t *context, uintmax_t id, const char *defaultFormat);
// split saveItem:object.field=value string, where each of saveItem/object/field/value can begin with % for a (dec/hex) integer and %% for a float
int splitSaveItemObjectFieldValueString(context_t *context, char *string, const char **baseFileNameRequest, uintmax_t *saveItemRequestedID, uintmax_t *objectRequestedID, uintmax_t *fieldRequestedID, uintmax_t *value);



#endif // _LEGOTOOL_MAPPINGS_H_
