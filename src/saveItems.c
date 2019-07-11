/* this file is part of legotool, by xvi */
#include "saveItems.h"
#include "fnv.h"
#include "mappings.h"
#include "memory.h"
#include "games.h"
#include "objects.h"




void saveItemInitialize(void);



static int saveItemInitialized = 0;

objectRule_t rules[] =
	{
// these rules are scanned from top to bottom and first matching rule applies.

	{ gameBatman3,      &saveItemCharacters,                        objectVersion42244, "X" },
	{ gameBatman3,      &saveItemCharactersVariables,               objectVersion488,   "?" },
	{ gameBatman3,      &saveItemCharacterTokens,                   objectVersion42244, "d" },
	{ gameBatman3,      &saveItemCharacterTokensVariables,          objectVersion488,   "?" },
	{ gameBatman3,      &saveItemCheats,                            objectVersion42244, "X" },
	{ gameBatman3,      &saveItemCheatsVariables,                   objectVersion488,   "?" },
	{ gameBatman3,      &saveItemCheckpoints,                       objectVersion42244, "X" },
	{ gameBatman3,      &saveItemCheckpointsVariables,              objectVersion488,   "?" },
	{ gameBatman3,      &saveItemCoins,                             objectVersion42244, "d" },
	{ gameBatman3,      &saveItemCoinsVariables,                    objectVersion488,   "d" },
	{ gameBatman3,      &saveItemCustomiser,                        objectVersion42244, "d" },
	{ gameBatman3,      &saveItemCustomiserSave,                    objectVersion42244, "Stream" },
	{ gameBatman3,      &saveItemCustomiserVariables,               objectVersion488,   "?" },
	{ gameBatman3,      &saveItemCutSave,                           objectVersion42244, "?" },
	{ gameBatman3,      &saveItemDebugSaveItem,                     objectVersion42244, "1" },
	{ gameBatman3,      &saveItemGoldBricks,                        objectVersion42244, "d" },
	{ gameBatman3,      &saveItemGoldBricksVariables,               objectVersion488,   "?" },
	{ gameBatman3,      &saveItemLegoAchievements,                  objectVersion42244, "?" },
	{ gameBatman3,      &saveItemMinikits,                          objectVersion42244, "d" },
	{ gameBatman3,      &saveItemMinikitsVariables,                 objectVersion488,   "?" },
	{ gameBatman3,      &saveItemPerils,                            objectVersion42244, "d" },
	{ gameBatman3,      &saveItemPerilsVariables,                   objectVersion488,   "?" },
	{ gameBatman3,      &saveItemPlanetsUnlockSaveLoad,             objectVersion42244, "2" },
	{ gameBatman3,      &saveItemRedBricks,                         objectVersion42244, "d" },
	{ gameBatman3,      &saveItemRedBricksVariables,                objectVersion488,   "?" },
	{ gameBatman3,      &saveItemSaveStation,                       objectVersion42244, "4" },
	{ gameBatman3,      &saveItemSpaceShooterSystem,                objectVersion42244, "?" },
	{ gameBatman3,      &saveItemStoryComplete,                     objectVersion42244, "X" },
	{ gameBatman3,      &saveItemStoryCompleteVariables,            objectVersion488,   "?" },
	{ gameBatman3,      &saveItemSuitTokens,                        objectVersion42244, "d" },
	{ gameBatman3,      &saveItemSuitTokensVariables,               objectVersion488,   "?" },
	{ gameBatman3,      &saveItemTrueHero,                          objectVersion42244, "d" },
	{ gameBatman3,      &saveItemTrueHeroVariables,                 objectVersion488,   "?" },
	{ gameBatman3,      &saveItemVRHighScore,                       objectVersion42244, "d" },
	{ gameBatman3,      &saveItemVRHighScoreVariables,              objectVersion488,   "?" },

	{ gameJurassic,     &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameJurassic,     &saveItemAmberBricks,                       objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemAmberHubBonusLevelMarkers,         objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemAmberHubFreePlayMarkers,           objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemAmberHubMapPoints,                 objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemCheckpointRace,                    objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameJurassic,     &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameJurassic,     &saveItemCustomiser,                        objectVersion4488,  "d" },
	{ gameJurassic,     &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameJurassic,     &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameJurassic,     &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameJurassic,     &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemHealDino,                          objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemItems,                             objectVersion4488,  "?" },
	{ gameJurassic,     &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameJurassic,     &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemPerils,                            objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemPhoto,                             objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameJurassic,     &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameJurassic,     &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameForceAwakens, &saveItemAchievements_Main,                 objectVersion4488,  "4" },
	{ gameForceAwakens, &saveItemBounty,                            objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemBridgeLevels,                      objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCarbonite,                         objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemChargeChallenge,                   objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCheckpointRace,                    objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameForceAwakens, &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameForceAwakens, &saveItemCustomiser,                        objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameForceAwakens, &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameForceAwakens, &saveItemFirstOrder,                        objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemGalaxyMapDestination,              objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemGeneral,                           objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemItems,                             objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameForceAwakens, &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameForceAwakens, &saveItemMicroFighter,                      objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemRace,                              objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemResistance,                        objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameForceAwakens, &saveItemScavenger,                         objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemTranslate,                         objectVersion4488,  "X" },
	{ gameForceAwakens, &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameAvengers,     &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemAutoHintSaveLoadManager,           objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemCharacterTokens,                   objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemCheckpointRace,                    objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameAvengers,     &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameAvengers,     &saveItemCustomiser,                        objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameAvengers,     &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameAvengers,     &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemItems,                             objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameAvengers,     &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameAvengers,     &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemPeril,                             objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemQuest,                             objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameAvengers,     &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameAvengers,     &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameWorlds,       &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemBricks,                            objectVersion4488,  "X" },
	{ gameWorlds,       &saveItemCellTypes,                         objectVersion4488,  "X" },
	{ gameWorlds,       &saveItemCharacters,                        objectVersion4488,  "Xd" },
	{ gameWorlds,       &saveItemCharacterTokens,                   objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemCheats,                            objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameWorlds,       &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameWorlds,       &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameWorlds,       &saveItemCustomiser,                        objectVersion4488,  "X" },
	{ gameWorlds,       &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameWorlds,       &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameWorlds,       &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameWorlds,       &saveItemItems,                             objectVersion4488,  "Xdd" },
	{ gameWorlds,       &saveItemLXFMLModels,                       objectVersion4488,  "Xd" },
	{ gameWorlds,       &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameWorlds,       &saveItemMinikits,                          objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemProps,                             objectVersion4488,  "Xd" },
	{ gameWorlds,       &saveItemRedBricks,                         objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemStoryComplete,                     objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemThemes,                            objectVersion4488,  "Xd" },
	{ gameWorlds,       &saveItemTrueHero,                          objectVersion4488,  "?" },
	{ gameWorlds,       &saveItemVOCollectables,                    objectVersion4488,  "t" }, // last time each of the VOCollectables has been played

	{ gameNinjago,      &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameNinjago,      &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemCharacterTokens,                   objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemCheckpointRace,                    objectVersion4488,  "Xf" },
	{ gameNinjago,      &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameNinjago,      &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameNinjago,      &saveItemCustomiser,                        objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameNinjago,      &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameNinjago,      &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameNinjago,      &saveItemDojoMedals,                        objectVersion4488,  "Xd" },
	{ gameNinjago,      &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemItems,                             objectVersion4488,  "?" },
	{ gameNinjago,      &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameNinjago,      &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameNinjago,      &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameNinjago,      &saveItemQuest,                             objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemRenovation,                        objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameNinjago,      &saveItemSpinjitzu,                         objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemTalisman,                          objectVersion4488,  "X" },
	{ gameNinjago,      &saveItemWisdom,                            objectVersion4488,  "Xdd" },
	{ gameNinjago,      &saveItemWisdomWeapons,                     objectVersion4488,  "X" },

	{ gameIncredibles,  &saveItemAchievements_DLC1,                 objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemBlindBags,                         objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemChallengeGoal,                     objectVersion4488,  "Xfx" },
	{ gameIncredibles,  &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemCharacterTokens,                   objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemCheckpointRace,                    objectVersion4488,  "Xf" },
	{ gameIncredibles,  &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameIncredibles,  &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameIncredibles,  &saveItemCustomiser,                        objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameIncredibles,  &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameIncredibles,  &saveItemFamilyBuild,                       objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemIncredibricks,                     objectVersion4488,  "D" }, // 6 first aren't hash, hence "D" instead of "X"
	{ gameIncredibles,  &saveItemItems,                             objectVersion4488,  "Xdd" },
	{ gameIncredibles,  &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameIncredibles,  &saveItemLevelVisits,                       objectVersion4488,  "d" },
	{ gameIncredibles,  &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameIncredibles,  &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemQuest,                             objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameIncredibles,  &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameIncredibles,  &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameSuperHeroes2, &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemChallengeGoal,                     objectVersion4488,  "Xfx" },
	{ gameSuperHeroes2, &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemCharacterTokens,                   objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemCheckpointRace,                    objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameSuperHeroes2, &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameSuperHeroes2, &saveItemCustomiser,                        objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameSuperHeroes2, &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameSuperHeroes2, &saveItemGoldBrickMonumentCollectableDef,   objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemItems,                             objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameSuperHeroes2, &saveItemLXFMLModels,                       objectVersion4488,  "Xd" },
	{ gameSuperHeroes2, &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameSuperHeroes2, &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemPeril,                             objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemPhotoGallery,                      objectVersion4488,  "Xx" }, // x stands for the hash of S[1-4]-SYSGLOBAL:PICS/LMSH2_${DD}-${MM}-${YYYY}_${hh}-${mm}-${ss}.JPEG (the picture file reference)
	{ gameSuperHeroes2, &saveItemProps,                             objectVersion4488,  "Xd" },
	{ gameSuperHeroes2, &saveItemQuest,                             objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameSuperHeroes2, &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameSuperHeroes2, &saveItemThemes,                            objectVersion4488,  "Xd" },
	{ gameSuperHeroes2, &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameVillains,     &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameVillains,     &saveItemChallengeGoal,                     objectVersion4488,  "Xfd" },
	{ gameVillains,     &saveItemCharacters,                        objectVersion4488,  "X" },
	{ gameVillains,     &saveItemCharacterTokens,                   objectVersion4488,  "X" },
	{ gameVillains,     &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameVillains,     &saveItemCheckpointRace,                    objectVersion4488,  "X" },
	{ gameVillains,     &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameVillains,     &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameVillains,     &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameVillains,     &saveItemCustomiser,                        objectVersion4488,  "?" },
	{ gameVillains,     &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameVillains,     &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameVillains,     &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameVillains,     &saveItemGoldBrickMonumentCollectableDef,   objectVersion4488,  "X" },
	{ gameVillains,     &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameVillains,     &saveItemGraffitiPosters,                   objectVersion4488,  "X" },
	{ gameVillains,     &saveItemItems,                             objectVersion4488,  "?" },
	{ gameVillains,     &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameVillains,     &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameVillains,     &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameVillains,     &saveItemMinikits,                          objectVersion4488,  "X" },
	{ gameVillains,     &saveItemPeril,                             objectVersion4488,  "?" },
	{ gameVillains,     &saveItemPhotoGallery,                      objectVersion4488,  "Xx" }, // x stands for the hash of S[1-4]-SYSGLOBAL:PICS/VILLAINS_${DD}-${MM}-${YYYY}_${hh}-${mm}-${ss}.JPEG (the picture file reference)
	{ gameVillains,     &saveItemQuest,                             objectVersion4488,  "X" },
	{ gameVillains,     &saveItemRedBricks,                         objectVersion4488,  "X" },
	{ gameVillains,     &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameVillains,     &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameVillains,     &saveItemTrueHero,                          objectVersion4488,  "X" },

	{ gameHobbit,       &saveItemSchematic,                         objectVersion44,    "?" },	// strange looking, to be verified
	{ gameHobbit,       &saveItemTreasure,                          objectVersion44,    "?" },	// strange looking, to be verified
	{ gameHobbit,       &saveItemCustomiser,                        objectVersion44,    "?" },	// strange looking, to be verified
	{ gameHobbit,       &saveItemLegoAchievements,                  objectVersion44,    "1" },
	{ gameHobbit,       &saveItemAutoHintSaveLoadManager,           objectVersion44,    "4" },
	{ gameHobbit,       &saveItemMechLootLoadSaveManager,           objectVersion44,    "d" },
	{ gameHobbit,       &saveItemcCheckpointMidAreaManagerSaveLoad, objectVersion44,    "?" },
	{ gameHobbit,       &saveItemcGlobalBrickPickupManager,         objectVersion44,    "?" },
	{ gameHobbit,       &saveItemcHubMapSaveData,                   objectVersion44,    "?" },
	{ gameHobbit,       &saveItemcCheckpointManagerSaveLoad,        objectVersion44,    "f" },

	{ gameMovie1,       &saveItemAchievements_Main,                 objectVersion44,    "?" },
	{ gameMovie1,       &saveItemcCheckpointManagerSaveLoad,        objectVersion44,    "f" },
	{ gameMovie1,       &saveItemcCheckpointMidAreaManagerSaveLoad, objectVersion44,    "?" },

	{ gameMovie2,       &saveItemAchievements_Main,                 objectVersion4488,  "?" },
	{ gameMovie2,       &saveItemBricks,                            objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemBuildBricks,                       objectVersion4488,  "d" },
	{ gameMovie2,       &saveItemCellTypes,                         objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemCharacters,                        objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemCheats,                            objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemCheckpoints,                       objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemCoins,                             objectVersion4488,  "d" },
	{ gameMovie2,       &saveItemCollectablePadding,                objectVersion4488,  "-" },
	{ gameMovie2,       &saveItemCustomiser,                        objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemCustomiserSave,                    objectVersion4488,  "Stream" },
	{ gameMovie2,       &saveItemCutSave,                           objectVersion4488,  "?" },
	{ gameMovie2,       &saveItemDebugSaveItem,                     objectVersion4488,  "1" },
	{ gameMovie2,       &saveItemGoldBrickMachine,                  objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemGoldBricks,                        objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemItems,                             objectVersion4488,  "Xdd" },
	{ gameMovie2,       &saveItemJumpToSystem,                      objectVersion4488,  "JumpTo" },
	{ gameMovie2,       &saveItemLXFMLModels,                       objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemMechHintsSaveManager,              objectVersion4488,  "?" },
	{ gameMovie2,       &saveItemMetrics,                           objectVersion4488,  "d" },
	{ gameMovie2,       &saveItemProps,                             objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemSaturnQuest,                       objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemSaveStation,                       objectVersion4488,  "?" },
	{ gameMovie2,       &saveItemStickers,                          objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemStoryComplete,                     objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemThemes,                            objectVersion4488,  "Xd" },
	{ gameMovie2,       &saveItemTraderBrick,                       objectVersion4488,  "X" },
	{ gameMovie2,       &saveItemVOCollectables,                    objectVersion4488,  "t" },	// not sure wether it's "t" or "d". We'll see.

//	{ gameXXXXXXXXXXXX, &saveItemXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX,   objectVersionXXXXX, "?" },

	// catch-all rule: objectVersion4488, unknown format. may help for unknown future game files analysis
	{ gameAny,          &saveItemAny,                               objectVersion4488,  "?"},
	};


void saveItemInitialize(void)
{

	if (saveItemInitialized) return;

	saveItemAny                               = ID_WILDCARD;
	saveItemAchievements_DLC1                 = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Achievements_DLC1");
	saveItemAchievements_Main                 = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Achievements_Main");
	saveItemAmberBricks                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "AmberBricks");
	saveItemAmberHubBonusLevelMarkers         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "AmberHubBonusLevelMarkers");
	saveItemAmberHubFreePlayMarkers           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "AmberHubFreePlayMarkers");
	saveItemAmberHubMapPoints                 = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "AmberHubMapPoints");
	saveItemAutoHintSaveLoadManager           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "AutoHintSaveLoadManager");
	saveItemBlindBags                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "BlindBags");
	saveItemBounty                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Bounty");
	saveItemBricks                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Bricks");
	saveItemBridgeLevels                      = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "BridgeLevels");
	saveItemBuildBricks                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "BuildBricks");
	saveItemCarbonite                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Carbonite");
	saveItemcCheckpointManagerSaveLoad        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "cCheckpointManagerSaveLoad");
	saveItemcCheckpointMidAreaManagerSaveLoad = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "cCheckpointMidAreaManagerSaveLoad");
	saveItemCellTypes                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CellTypes");
	saveItemcGlobalBrickPickupManager         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "cGlobalBrickPickupManager");
	saveItemChallengeGoal                     = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "ChallengeGoal");
	saveItemCharacters                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Characters");
	saveItemCharactersVariables               = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CharactersVariables");
	saveItemCharacterTokens                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CharacterTokens");
	saveItemCharacterTokensVariables          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CharacterTokensVariables");
	saveItemChargeChallenge                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "ChargeChallenge");
	saveItemCheats                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Cheats");
	saveItemCheatsVariables                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CheatsVariables");
	saveItemCheckpointRace                    = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CheckpointRace");
	saveItemCheckpoints                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Checkpoints");
	saveItemCheckpointsVariables              = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CheckpointsVariables");
	saveItemcHubMapSaveData                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "cHubMapSaveData");
	saveItemCoins                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Coins");
	saveItemCoinsVariables                    = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CoinsVariables");
	saveItemCollectablePadding                = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CollectablePadding");
	saveItemCustomiser                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Customiser");
	saveItemCustomiserSave                    = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CustomiserSave");
	saveItemCustomiserVariables               = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CustomiserVariables");
	saveItemCutSave                           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "CutSave");
	saveItemDebugSaveItem                     = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "DebugSaveItem");
	saveItemDojoMedals                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "DojoMedals");
	saveItemFamilyBuild                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "FamilyBuild");
	saveItemFirstOrder                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "FirstOrder");
	saveItemGalaxyMapDestination              = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GalaxyMapDestination");
	saveItemGeneral                           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "General");
	saveItemGoldBrickMachine                  = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GoldBrickMachine");
	saveItemGoldBrickMonumentCollectableDef   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GoldBrickMonumentCollectableDef");
	saveItemGoldBricks                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GoldBricks");
	saveItemGoldBricksVariables               = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GoldBricksVariables");
	saveItemGraffitiPosters                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "GraffitiPosters");
	saveItemHealDino                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "HealDino");
	saveItemIncredibricks                     = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Incredibricks");
	saveItemItems                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Items");
	saveItemJumpToSystem                      = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "JumpToSystem");
	saveItemLegoAchievements                  = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "LegoAchievements");
	saveItemLevelVisits                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "LevelVisits");
	saveItemLXFMLModels                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "LXFMLModels");
	saveItemMechHintsSaveManager              = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "MechHintsSaveManager");
	saveItemMechLootLoadSaveManager           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "MechLootLoadSaveManager");
	saveItemMetrics                           = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Metrics");
	saveItemMicroFighter                      = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "MicroFighter");
	saveItemMinikits                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Minikits");
	saveItemMinikitsVariables                 = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "MinikitsVariables");
	saveItemPeril                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Peril");
	saveItemPerils                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Perils");
	saveItemPerilsVariables                   = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "PerilsVariables");
	saveItemPhoto                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Photo");
	saveItemPhotoGallery                      = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "PhotoGallery");
	saveItemPlanetsUnlockSaveLoad             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "PlanetsUnlockSaveLoad");
	saveItemProps                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Props");
	saveItemQuest                             = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Quest");
	saveItemRace                              = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Race");
	saveItemRedBricks                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "RedBricks");
	saveItemRedBricksVariables                = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "RedBricksVariables");
	saveItemRenovation                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Renovation");
	saveItemResistance                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Resistance");
	saveItemSaturnQuest                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "SaturnQuest");
	saveItemSaveStation                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "SaveStation");
	saveItemScavenger                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Scavenger");
	saveItemSchematic                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Schematic");
	saveItemSpaceShooterSystem                = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "SpaceShooterSystem");
	saveItemSpinjitzu                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Spinjitzu");
	saveItemStickers                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Stickers");
	saveItemStoryComplete                     = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "StoryComplete");
	saveItemStoryCompleteVariables            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "StoryCompleteVariables");
	saveItemSuitTokens                        = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "SuitTokens");
	saveItemSuitTokensVariables               = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "SuitTokensVariables");
	saveItemTalisman                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Talisman");
	saveItemThemes                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Themes");
	saveItemTraderBrick                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "TraderBrick");
	saveItemTranslate                         = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Translate");
	saveItemTreasure                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Treasure");
	saveItemTrueHero                          = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "TrueHero");
	saveItemTrueHeroVariables                 = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "TrueHeroVariables");
	saveItemVOCollectables                    = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "VOCollectables");
	saveItemVRHighScore                       = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "VRHighScore");
	saveItemVRHighScoreVariables              = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "VRHighScoreVariables");
	saveItemWisdom                            = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "Wisdom");
	saveItemWisdomWeapons                     = FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, "WisdomWeapons");

	saveItemInitialized = 1;
}

// these ForEach... functions made code nicer and easier to write.
uintmax_t forEachSaveItemWithID(context_t *context, forEachSaveItemFunction f, uintmax_t userData, uintmax_t saveItemID)
{
	int match;
	int returnedValue;


	saveItemInitialize();

	if (context->fileSize < 0x14) return INVALID_DATA;
	if (context->fileSize < (context->dataOffset + 0x10)) return INVALID_DATA;

	context->saveItemCount = get32(context, context->dataOffset + 0x0c);

	// skip over cLoadSaveManager_Data header + saveItem count
	context->saveItemOffset = context->dataOffset + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	for (intmax_t i = 0; i < context->saveItemCount; i++)
		{
		if (context->fileSize < (context->saveItemOffset + 0x08)) return INVALID_DATA;
		context->saveItemID1 = get32(context, context->saveItemOffset + 0x00);
		context->saveItemDataSize = get32(context, context->saveItemOffset + 0x04);
		if (context->saveItemDataSize > 0xfffffffc)
			{
			// this logic found in superheroes2
			context->saveItemDataSize = 0;
			context->saveItemOffset += sizeof(uint32_t) + sizeof(uint32_t) + context->saveItemDataSize + 0;	// there's no saveItemID2!
			}
		else
			{
			// handle real sizes that differ from announced sizes
			if (context->saveItemID1 == saveItemCustomiserSave)
				context->saveItemDataSize = context->saveItemDataSize / 8000 * 8000;
			else if (context->game == gameMovie1 && context->saveItemID1 == saveItemLegoAchievements)
				context->saveItemDataSize = 0x10;	// unsure: specified size is 0x14 but really is 0x10
			else if (context->game == gameMovie1 && context->saveItemID1 == saveItemcCheckpointMidAreaManagerSaveLoad)
				context->saveItemDataSize = 0x11e;	// unsure: specified size is 0x150 but really is 0x11e
			else if (context->game == gameHobbit && context->saveItemID1 == saveItemcCheckpointMidAreaManagerSaveLoad)
				context->saveItemDataSize = 0x58;	// unsure: specified size is 0xb0 but really is 0x58

			context->saveItemDataOffset = context->saveItemOffset + sizeof(uint32_t) + sizeof(uint32_t);
			if (context->fileSize < (context->saveItemOffset + 0x0c + context->saveItemDataSize))
				return INVALID_DATA;

			context->saveItemID2 = get32(context, context->saveItemOffset + 0x08 + context->saveItemDataSize);
			if (context->saveItemID1 != context->saveItemID2)
				return INVALID_DATA; // context->saveItemDataSize is probably wrong

			// set context->saveItemObjectVersion according to rules
			for (uintmax_t i = 0; i < sizeof(rules)/sizeof(*rules); i++)
				{
				if ((rules[i].game == context->game || rules[i].game == gameAny) && (*rules[i].saveItem == context->saveItemID1 || (*rules[i].saveItem == saveItemAny)))
					{
					context->saveItemObjectVersion = rules[i].objectVersion;
					context->saveItemObjectFormat = rules[i].format;
					break;
					}
				}

			match = (context->saveItemID1 == saveItemID) || (saveItemID == ID_WILDCARD);	// match is based on saveItemID1 only
			if (match)
				{
				returnedValue = f(context, userData);
				if (returnedValue != 0)
					return returnedValue;
				}
			context->saveItemOffset += sizeof(uint32_t) + sizeof(uint32_t) + context->saveItemDataSize + sizeof(uint32_t);
			}
		}

	context->saveItemID1 = ID_WILDCARD;
	context->saveItemID2 = ID_WILDCARD;

	return 0;
}



uintmax_t forEachSaveItemWithName(context_t *context, forEachSaveItemFunction f, uintmax_t userData, char *saveItemName)
{
	saveItemInitialize();

	return forEachSaveItemWithID(context, f, userData, FNV1UppercaseStringHash32(FNV1_INITIAL_SEED_32, saveItemName));
}



uintmax_t forEachSaveItem(context_t *context, forEachSaveItemFunction f, uintmax_t userData)
{
	saveItemInitialize();

	return forEachSaveItemWithID(context, f, userData, ID_WILDCARD);
}
