/* this file is part of legotool, by xvi */

#include "../mappings.h"
#include "../games.h"



mapping globalKnownIDs[] =	// Known IDs are in included files. Much easier to maintain.
	{
// common/*.c are valid and have a meaning for all games
#include "01-common/saveItems.c"
#include "01-common/fields.c"
#include "01-common/states.c"
#include "01-common/savegame.c"
#include "01-common/coins.c"
#include "01-common/metrics.c"
#include "01-common/cheats.c"
#include "01-common/questTypes.c"

// games
#include "batman3/ids.c"
#include "jurassic/ids.c"
#include "avengers/ids.c"
#include "forceawakens/ids.c"
#include "worlds/ids.c"
#include "ninjago/ids.c"
#include "incredibles/ids.c"
#include "superheroes2/ids.c"
#include "villains/ids.c"
#include "movie2/ids.c"
	};

uintmax_t globalKnownIDsCount = sizeof(globalKnownIDs) / sizeof(*globalKnownIDs);
