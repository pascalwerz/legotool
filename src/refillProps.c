/* this file is part of legotool, by xvi */
#include "refillProps.h"
#include "objects.h"
#include <stdio.h>



static uintmax_t legoWorldsPropCanBeRefilled[] =
	{
	0x0152d637, 0x01ec7665, 0x01f350ee, 0x0527a2ee, 0x05e2aac1, 0x093e57b4, 0x0a2d62b6, 0x0be27ec9, 0x0c9dc0c0, 0x0cb5c43b, 
	0x0d896e91, 0x0e01476d, 0x0f179691, 0x11ce4268, 0x12cb806f, 0x12cedbea, 0x12dd7fbb, 0x12f0b824, 0x14af2d87, 0x14f83cc2, 
	0x1a8070fe, 0x1ba03c4e, 0x1e5545a3, 0x1f83149a, 0x1f877bfc, 0x208b128b, 0x21060d4d, 0x22277839, 0x22624fc5, 0x22a4a85c, 
	0x24356cef, 0x2633e1cb, 0x29565c31, 0x2fe3c13f, 0x303c13a8, 0x31cfdc4d, 0x345ef2dc, 0x3490975c, 0x3a610801, 0x3ad9b759, 
	0x3b00b6a4, 0x3b05d272, 0x3b2c1954, 0x3df2b344, 0x3ebfff66, 0x404a39af, 0x4609eea6, 0x4976bd91, 0x4c6afe34, 0x4d4c8d43, 
	0x4d5339de, 0x4fa0019d, 0x51054e89, 0x52360e3b, 0x523c107c, 0x52c34d01, 0x5430019e, 0x54b5add2, 0x551a4825, 0x558732a4, 
	0x55946a38, 0x568258d6, 0x58aaa499, 0x58cc8eca, 0x59c0723d, 0x5b43fea9, 0x5c4dd50b, 0x640a2ed5, 0x64a563f4, 0x6652c9b8, 
	0x6972cf88, 0x6a3230c3, 0x6a3ebbc2, 0x6d533f49, 0x6fb21b66, 0x738f5371, 0x766a9618, 0x7685d562, 0x76f2a0ae, 0x77975f36, 
	0x796eece1, 0x79adace2, 0x7aa4101a, 0x7e045913, 0x7e8dd4fa, 0x82fc3de0, 0x85023fee, 0x878e8bf6, 0x87d4e560, 0x8961e850, 
	0x8d31724a, 0x8ddf0d8e, 0x8ec211c3, 0x8f14ade2, 0x9007f810, 0x9084a14b, 0x91b9c48f, 0x949ce705, 0x95076ae5, 0x950ca51b, 
	0x9785f869, 0x984c5bf4, 0x9992e012, 0x9d6a690b, 0x9e51f093, 0xa3a2451f, 0xa49e2a4f, 0xa7f9c9ab, 0xa81de3db, 0xa8a78f2f, 
	0xaa553419, 0xaa821f45, 0xab52cb63, 0xabbb0d86, 0xac7a2ffc, 0xae49a1b2, 0xae57462f, 0xb13bb514, 0xb18cf470, 0xb4ec8d1e, 
	0xb7034240, 0xb79776ce, 0xb7b5310a, 0xb84cf951, 0xbc476b51, 0xbe110734, 0xc1bd69f0, 0xc2eb4c02, 0xc31827e7, 0xc3ae1eb7, 
	0xc4e7b04d, 0xc5858ec7, 0xc76142fc, 0xc83c94d1, 0xc8f1fc76, 0xcc22bdf3, 0xcc4258a7, 0xcca1ed5d, 0xcdeab876, 0xce4103f3, 
	0xcfa0ade4, 0xd00e12c0, 0xd06544c5, 0xd465c022, 0xd49afe94, 0xd8e6b0c5, 0xda2c5713, 0xdd849b27, 0xde47cd09, 0xdf7a5035, 
	0xe32b9705, 0xe37a33cf, 0xe5632ad7, 0xe9d8d7ad, 0xea69f082, 0xeaac660c, 0xead8d9c7, 0xeb664ab2, 0xebe8747a, 0xedee450e, 
	0xeeae1e18, 0xeef261c4, 0xefc06aa3, 0xf0befd8a, 0xf34fbf23, 0xf3b9a3d9, 0xf504691c, 0xf591f275, 0xf68f4f00, 0xf8c3e0b9, 
	0xf8e97eb8, 0xfb66a4cd, 0xfcd67851, 0xff7afed4,
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
		if (
		    !findObjectForObjectIDAndFieldIDAndCheckValue(context, legoWorldsPropCanBeRefilled[i], context->defaultID, context->lockedID, 1) &&
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



