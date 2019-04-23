/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_GAMES_H_
#define _LEGOTOOL_GAMES_H_ 1

#include <stdint.h>

// to add a supported game:
// 1 - add it to "typedef enum {} gameIdentification" (game.h)
// 2 - add it to "static keyword_t gameKeywords[]" (game.c)
// 3 - add it to "usage()" (legotool.c)
// 4 - add it to "README.md"



typedef enum
	{
	// non specific, hard-coded values required.
	gameAny          = -1,
	// game-specific.
	gameAvengers,
	gameBatman3,
	gameForceAwakens,
	gameHobbit,
	gameIncredibles,
	gameJurassic,
	gameMovie1,
	gameMovie2,
	gameNinjago,
	gameSuperHeroes2,
	gameVillains,
	gameWorlds,
	} gameIdentification;

gameIdentification nameToGameIdentification(const char * name);
const char *gameIdentificationToName(gameIdentification id);

#endif // _LEGOTOOL_GAMES_H_
