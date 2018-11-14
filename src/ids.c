/* this file is part of legotool, by xvi */
#include "ids.h"



int hashCompareFunction(const void *h1, const void *h2)
{
if (((mapping *) h1)->key < ((mapping *) h2)->key) return -1;
if (((mapping *) h1)->key > ((mapping *) h2)->key) return +1;
if (((mapping *) h1)->saveItemID != ID_WILDCARD && ((mapping *) h2)->saveItemID == ID_WILDCARD) return -1;	// specific IDs have precedence
if (((mapping *) h1)->saveItemID == ID_WILDCARD && ((mapping *) h2)->saveItemID != ID_WILDCARD) return +1;	// specific IDs have precedence
if (((mapping *) h1)->saveItemID < ((mapping *) h2)->saveItemID) return -1;
if (((mapping *) h1)->saveItemID > ((mapping *) h2)->saveItemID) return +1;
if (((mapping *) h1)->game != ID_WILDCARD && ((mapping *) h2)->game == ID_WILDCARD) return -1;	// specific IDs have precedence
if (((mapping *) h1)->game == ID_WILDCARD && ((mapping *) h2)->game != ID_WILDCARD) return +1;	// specific IDs have precedence
if (((mapping *) h1)->game < ((mapping *) h2)->game) return -1;
if (((mapping *) h1)->game > ((mapping *) h2)->game) return +1;

return 0;
}



mapping *keyFind(context_t *context, uintmax_t key, uintmax_t game, uintmax_t saveItemID)
{
mapping searchKey;

searchKey.key = key;
searchKey.game = game;
searchKey.saveItemID = saveItemID;
searchKey.label = "(fake search mapping)";
searchKey.string = "(fake search mapping)";
searchKey.redirection = NULL;

return bsearch(&searchKey, context->knownIDs, context->knownIDsCount, sizeof(*context->knownIDs), hashCompareFunction);
}
