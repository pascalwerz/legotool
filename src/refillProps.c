/* this file is part of legotool, by xvi */
#include "refillProps.h"
#include "objects.h"
#include <stdio.h>



static uintmax_t legoWorldsPropCanBeRefilled[] =
	{
	0x87d4e560, // AaronBow
	0xc2eb4c02, // AgentsPistol
	0xc83c94d1, // Ammonite
	0x6972cf88, // Anacondrai_Hammer
	0x0152d637, // Apple
	0xeef261c4, // AppleDarkGreen
	0x345ef2dc, // AppleGolden
	0x3df2b344, // AppleRed
	0xefc06aa3, // AppleYellow
	0x4d4c8d43, // Axe
	0xe37a33cf, // Axe_Black
	0x8961e850, // Backpack_EggBlaster
	0x9007f810, // Bagpipes
	0xbe110734, // Baguette
	0x54b5add2, // Banana
	0x8ec211c3, // BattleGoddessSpear
	0xb4ec8d1e, // Bazooka
	0xb18cf470, // Binocular
	0xf8c3e0b9, // BlowPipe
	0x949ce705, // BlueTorch
	0xcdeab876, // Blunderbuss
	0xe9d8d7ad, // Bomb
	0x85023fee, // Bomb_Crystal
	0x7685d562, // BombSmall
	0xead8d9c7, // Bone
	0x404a39af, // Bow
	0xf504691c, // BowCompound
	0x24356cef, // Brush
	0xc1bd69f0, // CandyZooka
	0x093e57b4, // Carrot
	0x303c13a8, // CarrotZooka
	0x6fb21b66, // CavemanClub
	0xeb664ab2, // CherryBlue
	0xf34fbf23, // CherryGreen
	0x558732a4, // CherryPurple
	0xdd849b27, // CherryRed
	0x0be27ec9, // ChickenEgg
	0x05e2aac1, // ClaySword
	0x95076ae5, // ClubTwoHanded
	0x766a9618, // ColeHammer
	0x950ca51b, // Croissant
	0xb84cf951, // Crossbow
	0x3b05d272, // Crystal_Short
	0xb79776ce, // Crystal_ShortBoulderax
	0x984c5bf4, // Crystal_ShortFirox
	0xeeae1e18, // Crystal_ShortGlaciator
	0x14af2d87, // Crystal_ShortMeltrox
	0x79adace2, // Crystal_ShortSulfurix
	0x796eece1, // Crystal_Small
	0xab52cb63, // Crystal_SmallBoulderax
	0x22624fc5, // Crystal_SmallFirox
	0x4976bd91, // Crystal_SmallGlaciator
	0x568258d6, // Crystal_SmallMeltrox
	0x59c0723d, // Crystal_SmallSulfurix
	0xde47cd09, // Crystal_Tall
	0x9d6a690b, // Crystal_TallBoulderax
	0x21060d4d, // Crystal_TallFirox
	0x3ad9b759, // Crystal_TallGlaciator
	0x76f2a0ae, // Crystal_TallMeltrox
	0xdf7a5035, // Crystal_TallSulfurix
	0x9084a14b, // CrystalBow
	0x12f0b824, // Cutlass
	0xd49afe94, // Cutlass_Silver
	0xfcd67851, // DinoTooth
	0xc3ae1eb7, // Doughnut
	0xcc4258a7, // DragonEgg
	0x1e5545a3, // DragonEgg_Black
	0x9992e012, // DragonEgg_Blue
	0x3b00b6a4, // DragonEgg_Gold
	0x6a3230c3, // DragonEgg_Green
	0xedee450e, // DragonEgg_Pink
	0x58aaa499, // DragonEgg_White
	0xb13bb514, // Dynamite
	0x8d31724a, // ElectricTorch
	0x01ec7665, // ExplosiveBowCompound
	0x6652c9b8, // Extinguisher
	0xe5632ad7, // Fish
	0x12cb806f, // Fish_DarkGrey
	0x11ce4268, // Fish_Orange
	0x6d533f49, // Fish_Silver
	0xc8f1fc76, // FlintlockPistol
	0x01f350ee, // Football
	0x4fa0019d, // GlowBallGun
	0xf68f4f00, // GreenTorch
	0x0c9dc0c0, // GuitarRock
	0xf0befd8a, // HalloweenCake
	0x91b9c48f, // Hammer
	0xd8e6b0c5, // IceBallGun
	0x58cc8eca, // IceBow
	0x3b2c1954, // IceCream
	0x3ebfff66, // IceLolly
	0x0f179691, // JayFlail
	0xcca1ed5d, // KaiSword
	0xce4103f3, // Key_Gold
	0xa8a78f2f, // LanceLance
	0xa81de3db, // LaserPistol
	0x0527a2ee, // LavaBalLGun
	0x551a4825, // LavaBow
	0xebe8747a, // LloydSword
	0x738f5371, // MacyMace
	0x523c107c, // MagicBean
	0x7e8dd4fa, // MagicPotion_Blue
	0x52360e3b, // MagicPotion_Green
	0xeaac660c, // MagicPotion_Purple
	0xa49e2a4f, // MagicPotion_Red
	0xbc476b51, // MagicPotion_Silver
	0x12cedbea, // MagicPotion_Yellow
	0xc4e7b04d, // MegaZooka
	0xc5858ec7, // MonsterHunterGun
	0x0a2d62b6, // Nut
	0xd465c022, // NyaSpear
	0x6a3ebbc2, // PaintballGun
	0x3a610801, // PaintBow
	0xaa553419, // PastelBlueEgg
	0x1f83149a, // PastelGreenEgg
	0x640a2ed5, // PastelPinkEgg
	0x9785f869, // PastelYellowEgg
	0x1a8070fe, // PianoPaintGun
	0x4609eea6, // Pickaxe
	0x22277839, // Pie
	0xa3a2451f, // PieZooka
	0xb7034240, // Pistol
	0xe32b9705, // Pitchfork
	0x4c6afe34, // PneumaticDrill
	0x8f14ade2, // Pompom_Right
	0xf3b9a3d9, // PotionBottleBlack
	0x878e8bf6, // PotionBottleOrange
	0x22a4a85c, // PotionBottlePink
	0x0d896e91, // Pretzel
	0xae57462f, // Pumpkin
	0xd06544c5, // PumpkinBucket
	0x2633e1cb, // PurpleTorch
	0x82fc3de0, // RedTorch
	0xfb66a4cd, // Rifle
	0x1ba03c4e, // RollerSkate_Right
	0xc31827e7, // ROmanSpear
	0x7aa4101a, // Saxophone
	0xd00e12c0, // Scimitar_Crystal
	0xabbb0d86, // Seed
	0x7e045913, // SkeletonFishCritter
	0x8ddf0d8e, // SkeletonHead
	0x64a563f4, // SmallGrass
	0x0cb5c43b, // SnowBall
	0x0e01476d, // SpaceGun
	0x3490975c, // Spade
	0xac7a2ffc, // Spanner
	0xf591f275, // SpartanSpear
	0x1f877bfc, // Spear
	0x5430019e, // SpearGold
	0xb7b5310a, // SpookyTeddyBear
	0x52c34d01, // Steak
	0xcfa0ade4, // Sword
	0xaa821f45, // Sword_Wooden
	0xea69f082, // SwordBattle
	0x9e51f093, // SwordEgyptian
	0x5c4dd50b, // SwordGladiator
	0xc76142fc, // SwordGladiator_Wooden
	0x4d5339de, // SwordKatana
	0x14f83cc2, // SwordKatana_Black
	0x55946a38, // SwordKatanaGold
	0xcc22bdf3, // SwordRapier
	0xae49a1b2, // SwordScimitar
	0x2fe3c13f, // Telescope
	0x5b43fea9, // Torch
	0x31cfdc4d, // ToxicBallGun
	0xda2c5713, // TranqGun
	0x12dd7fbb, // Trident
	0xf8e97eb8, // Trumpet
	0x208b128b, // Truncheon
	0x29565c31, // TurkeyDrumstick
	0xff7afed4, // Water_Pistol
	0x51054e89, // WaterBallGun
	0x77975f36, // WhiteTorch
	0xa7f9c9ab, // ZaneBow
	};
static uintmax_t legoWorldsPropCanBeRefilledCount = sizeof(legoWorldsPropCanBeRefilled)/sizeof(*legoWorldsPropCanBeRefilled);



uintmax_t refillAllProps(context_t *context, uintmax_t userData)
{
	for (uintmax_t i = 0; i < legoWorldsPropCanBeRefilledCount; i++)
		{
		if (
		    findObjectForObjectIDAndFieldIDAndNotValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->collectedID) ||
		    findObjectForObjectIDAndFieldIDAndNotValue(context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, context->willRefillQuantity)
		   )
			{
			if (context->verbose)
				printf("updating %s/%s:%s.%s=%%%ju\n", context->baseFileName, textForID(context, context->saveItemID1, "%08jx"), textForID(context, legoWorldsPropCanBeRefilled[i], "%08jx"), textForID(context, context->numberOfItemsID, "%08jx"), userData);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->collectedID);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->discoveryStateID, 0); // mark it with a star
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, context->willRefillQuantity);
			}
		}
	return 0;
}



uintmax_t refillMyProps(context_t *context, uintmax_t userData)
{
	for (uintmax_t i = 0; i < legoWorldsPropCanBeRefilledCount; i++)
		{
		if (findObjectForObjectIDAndFieldIDAndCheckValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->unlockedDepletedID, 0))
			{
			if (context->verbose)
				printf("updating %s/%s:%s.%s=%%%ju\n", context->baseFileName, textForID(context, context->saveItemID1, "%08jx"), textForID(context, legoWorldsPropCanBeRefilled[i], "%08jx"), textForID(context, context->numberOfItemsID, "%08jx"), userData);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->collectedID);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->discoveryStateID, 0); // mark it with a star
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, 1);
			}
		else if (
		    !findObjectForObjectIDAndFieldIDAndCheckValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->lockedID, 1) &&
             findObjectForObjectIDAndFieldIDAndNotValue  (context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, 1) && 
             findObjectForObjectIDAndFieldIDAndNotValue  (context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, context->willRefillQuantity)
		   )
			{
			if (context->verbose)
				printf("updating %s/%s:%s.%s=%%%ju\n", context->baseFileName, textForID(context, context->saveItemID1, "%08jx"), textForID(context, legoWorldsPropCanBeRefilled[i], "%08jx"), textForID(context, context->numberOfItemsID, "%08jx"), userData);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->collectedID);
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->discoveryStateID, 0); // mark it with a star
			findObjectForObjectIDAndFieldIDAndSetValue(context, legoWorldsPropCanBeRefilled[i], context->numberOfItemsID, context->willRefillQuantity);
			}
		}
	return 0;
}



