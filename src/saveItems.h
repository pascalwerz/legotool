/* this file is part of legotool, by xvi */
#ifndef _LEGOTOOL_SAVEITEMS_H_
#define _LEGOTOOL_SAVEITEMS_H_	1

#include "legotoolContext.h"
#include <stdint.h>



typedef struct
	{
	gameIdentification game; 	// gameAny to match any
	uintmax_t *saveItem;		// &saveItemAny to match all
	int objectVersion;			// object version
	char *format;				// display format
	} objectRule_t;



uintmax_t saveItemAchievements_DLC1;
uintmax_t saveItemAchievements_Main;
uintmax_t saveItemAmberBricks;
uintmax_t saveItemAmberHubBonusLevelMarkers;
uintmax_t saveItemAmberHubFreePlayMarkers;
uintmax_t saveItemAmberHubMapPoints;
uintmax_t saveItemAny;
uintmax_t saveItemAutoHintSaveLoadManager;
uintmax_t saveItemBlindBags;
uintmax_t saveItemBounty;
uintmax_t saveItemBricks;
uintmax_t saveItemBridgeLevels;
uintmax_t saveItemBuildBricks;
uintmax_t saveItemCarbonite;
uintmax_t saveItemcCheckpointManagerSaveLoad;
uintmax_t saveItemcCheckpointMidAreaManagerSaveLoad;
uintmax_t saveItemCellTypes;
uintmax_t saveItemChallengeGoal;
uintmax_t saveItemCharacters;
uintmax_t saveItemCharactersVariables;
uintmax_t saveItemCharacterTokens;
uintmax_t saveItemCharacterTokensVariables;
uintmax_t saveItemChargeChallenge;
uintmax_t saveItemCheats;
uintmax_t saveItemCheatsVariables;
uintmax_t saveItemCheckpointRace;
uintmax_t saveItemCheckpoints;
uintmax_t saveItemCheckpointsVariables;
uintmax_t saveItemCoins;
uintmax_t saveItemCoinsVariables;
uintmax_t saveItemCollectablePadding;
uintmax_t saveItemCustomiser;
uintmax_t saveItemCustomiserSave;
uintmax_t saveItemCustomiserVariables;
uintmax_t saveItemCutSave;
uintmax_t saveItemDebugSaveItem;
uintmax_t saveItemDojoMedals;
uintmax_t saveItemFamilyBuild;
uintmax_t saveItemFirstOrder;
uintmax_t saveItemGalaxyMapDestination;
uintmax_t saveItemGeneral;
uintmax_t saveItemGoldBrickMachine;
uintmax_t saveItemGoldBrickMonumentCollectableDef;
uintmax_t saveItemGoldBricks;
uintmax_t saveItemGoldBricksVariables;
uintmax_t saveItemGraffitiPosters;
uintmax_t saveItemHealDino;
uintmax_t saveItemIncredibricks;
uintmax_t saveItemItems;
uintmax_t saveItemJumpToSystem;
uintmax_t saveItemLegoAchievements;
uintmax_t saveItemLevelVisits;
uintmax_t saveItemLXFMLModels;
uintmax_t saveItemMechHintsSaveManager;
uintmax_t saveItemMetrics;
uintmax_t saveItemMicroFighter;
uintmax_t saveItemMinikits;
uintmax_t saveItemMinikitsVariables;
uintmax_t saveItemPeril;
uintmax_t saveItemPerils;
uintmax_t saveItemPerilsVariables;
uintmax_t saveItemPhoto;
uintmax_t saveItemPhotoGallery;
uintmax_t saveItemPlanetsUnlockSaveLoad;
uintmax_t saveItemProps;
uintmax_t saveItemQuest;
uintmax_t saveItemRace;
uintmax_t saveItemRedBricks;
uintmax_t saveItemRedBricksVariables;
uintmax_t saveItemRenovation;
uintmax_t saveItemResistance;
uintmax_t saveItemSaturnQuest;
uintmax_t saveItemSaveStation;
uintmax_t saveItemScavenger;
uintmax_t saveItemSpaceShooterSystem;
uintmax_t saveItemSpinjitzu;
uintmax_t saveItemStickers;
uintmax_t saveItemStoryComplete;
uintmax_t saveItemStoryCompleteVariables;
uintmax_t saveItemSuitTokens;
uintmax_t saveItemSuitTokensVariables;
uintmax_t saveItemTalisman;
uintmax_t saveItemThemes;
uintmax_t saveItemTraderBrick;
uintmax_t saveItemTranslate;
uintmax_t saveItemTrueHero;
uintmax_t saveItemTrueHeroVariables;
uintmax_t saveItemVOCollectables;
uintmax_t saveItemVRHighScore;
uintmax_t saveItemVRHighScoreVariables;
uintmax_t saveItemWisdom;
uintmax_t saveItemWisdomWeapons;



// function prototype for forEachSaveItem...()
// if the return value is not 0, the saveItem cycling functions (forEachSaveItem...) stops processing remaining saveItems and returns that returned value.
typedef uintmax_t (*forEachSaveItemFunction)(context_t *context, uintmax_t userData);

// forEachSaveItem...: saveItem cycling functions
// if the value returned by f is not 0, the saveItem cycling functions (forEach...) stops processing remaining saveItems and returns that returned value.
// if the data is invalid, the saveItem cycling functions process as much saveItems as possible and returns INVALID_DATA
#ifndef INVALID_DATA
#define INVALID_DATA	(INTMAX_MIN)
#endif

uintmax_t forEachSaveItem(context_t *context, forEachSaveItemFunction f, uintmax_t userData);
uintmax_t forEachSaveItemWithID(context_t *context, forEachSaveItemFunction f, uintmax_t userData, uintmax_t saveItemID);
uintmax_t forEachSaveItemWithName(context_t *context, forEachSaveItemFunction f, uintmax_t userData, char *saveItemName);

#endif // _LEGOTOOL_SAVEITEMS_H_
