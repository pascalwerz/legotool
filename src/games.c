/* this file is part of legotool, by xvi */
#include "games.h"
#include "keywords.h"
#include <string.h>



// keyword_t's userFlags
#define SYNOMYM	(1 << 0)	// keyword is a synonym, not to be returned by gameIdentificationToName()



static keyword_t gameKeywords[] =	// this array will automatically be sorted and completed
	{
		{ gameAvengers,           0, 1,  8, "avengers"},
		{ gameBatman3,            0, 1,  7, "batman3"},
		{ gameForceAwakens, SYNOMYM, 1, 12, "forceawakens"},
		{ gameIncredibles,        0, 1, 11, "incredibles"},
		{ gameJurassic,           0, 1,  8, "jurassic"},
		{ gameNinjago,            0, 1,  7, "ninjago"},
		{ gameSuperHeroes2, SYNOMYM, 2,  3, "sh2"},
		{ gameSuperHeroes2,       0, 2, 12, "superheroes2"},
		{ gameForceAwakens, SYNOMYM, 2,  3, "tfa"},
		{ gameForceAwakens,       0, 2, 15, "theforceawakens"},
		{ gameVillains,           0, 1,  8, "villains"},
		{ gameWorlds,             0, 1,  6, "worlds"},
	};
static uintmax_t gameKeywordsCount = sizeof(gameKeywords)/sizeof(gameKeywords[0]);
static int gameKeywordsPrepared = 0;


gameIdentification nameToGameIdentification(const char * name)
{
const keyword_t *k;


if (!gameKeywordsPrepared)
	{
	prepareKeywords(gameKeywords, gameKeywordsCount, 0);
	gameKeywordsPrepared = 1;
	}

k = keywordLookup(name, gameKeywords, gameKeywordsCount, 0);
if (k)
	return k->userData;

return gameNotDefined; // no match
}



const char *gameIdentificationToName(gameIdentification id)
{
// no need to check gameKeywordsPrepared as we are doing a linear search anyway.
for (uintmax_t i = 0; i < gameKeywordsCount; i++)
	{
	if (gameKeywords[i].userData == id && !(gameKeywords[i].userFlags & SYNOMYM))
		return gameKeywords[i].fullName;
	}

return NULL; // no match
}
