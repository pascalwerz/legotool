/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_OBJECTS_H_
#define _LEGOTOOL_OBJECTS_H_	1

#include "legotoolContext.h"
#include <stdint.h>



typedef enum
	{
	objectVersion42244,
	objectVersion488,
	objectVersion4488,
	} objectVersion;

// opaque because you shouldn't access members directly
typedef struct _object_t object_t;
// and because it's opaque you can't sizeof() it, so use this instead (ugly indeed):
uintmax_t objectSizeOf(context_t *context);


// getters/setters
uint32_t objectObject(context_t *context, object_t *field);
uint32_t objectField(context_t *context, object_t *field);
uint16_t objectUnknownFlag(context_t *context, object_t *field);
uint16_t objectCollectedFlag(context_t *context, object_t *field);
uint64_t objectProbationalValue(context_t *context, object_t *field);
uint64_t objectPermanentValue(context_t *context, object_t *field);
// these setters WON'T print a message even if context->verbose is set
// verbose mode must be handled in upper level functions
void setObjectObject(context_t *context, object_t *field, uint32_t objectID);
void setObjectField(context_t *context, object_t *field, uint32_t fieldID);
void setObjectUnknownFlag(context_t *context, object_t *field, uint16_t value);
void setObjectCollectedFlag(context_t *context, object_t *field, uint16_t value);
void setObjectProbationalValue(context_t *context, object_t *field, uint64_t value);
void setObjectPermanentValue(context_t *context, object_t *field, uint64_t value);

// function prototype for forAll...()
// if the return value is not 0, the saveItem cycling functions (forAll...) stops processing remaining saveItems and returns that returned value.
typedef uintmax_t (*forAllFunction)(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t userData);

// forAll...: field cycling functions
// if the value returned by f is not 0, the saveItem cycling functions (forAll...) stops processing remaining saveItems and returns that returned value.
uintmax_t forAllObjects                          (context_t *context, forAllFunction f, uintmax_t userData);
uintmax_t forAllObjectsWithObjectIDAndFieldID    (context_t *context, forAllFunction f, uintmax_t userData, uintmax_t objectID, uintmax_t fieldID);
uintmax_t forAllObjectsWithObjectNameAndFieldName(context_t *context, forAllFunction f, uintmax_t userData, char *objectName, char *fieldName);

// fields area in saveItem data helpers
object_t *findObjectForObjectIDAndFieldID(context_t *context, uintmax_t objectID, uintmax_t fieldID);
int findObjectForObjectIDAndFieldIDAndNotValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value);
int findObjectForObjectIDAndFieldIDAndCheckValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value, int absentResult);
// won't display update message even in verbose mode
// verbose mode must be handled in upper level functions
void findObjectForObjectIDAndFieldIDAndSetValue(context_t *context, uintmax_t objectID, uintmax_t fieldID, uintmax_t value);

// sets both probationalValue and permanentValue, if any of them is not the target value
// won't display update message even in verbose mode
// verbose mode must be handled in upper level functions
void setObjectValue(context_t *context, object_t *field, uintmax_t fieldID, uintmax_t value);

#endif // _LEGOTOOL_OBJECTS_H_
