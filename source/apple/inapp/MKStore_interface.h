#ifndef MKSTORE_INTERFACE
#define MKSTORE_INTERFACE

enum
{
	MK_ACTION_LOAD=0,
	MK_ACTION_ISBOUGHT_PACK1,
	MK_ACTION_ISBOUGHT_PACK2,
//	MK_ACTION_ISBOUGHT_PACK3,
//	MK_ACTION_ISBOUGHT_PACK4,
//	MK_ACTION_ISBOUGHT_PACK5,
	MK_ACTION_PROVIDEDCONTENT,
	MK_ACTION_CLEARPROVIDEDCONTENT,
	MK_ACTION_GETFAILEDTRANS,
	MK_ACTION_RESTORE,
	MK_ACTION_BUY_PACK1,
	MK_ACTION_BUY_PACK2
//	MK_ACTION_BUY_PACK3,
//	MK_ACTION_BUY_PACK4,
//	MK_ACTION_BUY_PACK5

};

enum
{
	MK_ACTION2_GETPRODUCTPRICE
};

extern "C" int MKStoreAction (int action, int parameter=-1);
extern "C" char * MKStoreAction2 (int action, int parameter=-1);


#endif
