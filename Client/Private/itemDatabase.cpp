#include "stdafx.h"
#include "ItemDatabase.h"
#include <unordered_map>

#include "../Public/ItemDatabase.h"

std::unordered_map<ITEM_TYPE, Equipment_Info> CItemDatabase::m_MapEquipment
{
	//생명,공격력,내구도,던지기데미지,총알수
	{ITEM_TYPE::ITEM_BOTTLE, Equipment_Info{true,  5, 30,10,0}},
	{ITEM_TYPE::ITEM_AUTOMATIC_PISTOL, Equipment_Info{true, 8, 50,5,12}},
	{ITEM_TYPE::ITEM_TOMMY_GUN, Equipment_Info{true,  2, 50,100, 40}},
	{ITEM_TYPE::ITEM_PIPE, Equipment_Info{true,10, 80,0, 0}},
	{ITEM_TYPE::ITEM_FRYING_PAN, Equipment_Info{true, 12, 17,6, 0}},
};

std::unordered_map<ITEM_TYPE, Consumable_Info> CItemDatabase::m_MapConsumable
{
	//생명,힐링밸류,던지기데미지
	{ITEM_TYPE::ITEM_MEDIKIT, Consumable_Info{true,35,0}},
	{ITEM_TYPE::ITEM_BANANA, Consumable_Info{true,10,0}},
	{ITEM_TYPE::ITEM_APPLE, Consumable_Info{true,10,0}},
	{ITEM_TYPE::ITEM_COLA, Consumable_Info{true,10,0}}
};

std::unordered_map<ITEM_TYPE, Interactive_Info> CItemDatabase::m_MapInteractive
{
	//생명,블로킹여부
	{ITEM_TYPE::ITEM_BOARD, Interactive_Info{true,  true}},
	{ITEM_TYPE::ITEM_DOOR, Interactive_Info{true, true}},
	{ITEM_TYPE::ITEM_COMBOLOCK, Interactive_Info{true, true}},
	{ITEM_TYPE::ITEM_FAN, Interactive_Info{true,  false}},
	{ITEM_TYPE::ITEM_ENTRANCE_KEY, Interactive_Info{true,  true}},
	{ITEM_TYPE::ITEM_CARDBOARD_BOX, Interactive_Info{true,  true}},

	
};

std::unordered_map<ITEM_TYPE, ThrowOnly_Info> CItemDatabase::m_MapThrowOnly
{
	//생명,던지기데미지
	{ITEM_TYPE::ITEM_SCRUNCHED_PAPER, ThrowOnly_Info{true,  0}},
	{ITEM_TYPE::ITEM_WOODEN_BARREL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_OIL_BARREL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_PAINT_CAN, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TRASH_CAN, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TRASH_BIN, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TRAFFIC_CONE, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TOOL_BOX, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_UMBRELLA_STAND, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TABLE_LAMP, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_POT_PLANT, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_WOODEN_STOOL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TABLE_PLANT, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_MARTINI_GLASS, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TUMBLER_GLASS, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_POOL_BALL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_ASHTRAY_BIN, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_GAS_CANISTER, ThrowOnly_Info{true,  100}},
	{ITEM_TYPE::ITEM_METAL_BARREL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_HAIR_BRUSH, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_BOTTLE_ELSE, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_BAR_STOOL, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_JAR, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_CAN, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_ESPRESSO_MACHINE, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_CANTAINER, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_CHAMPAGNE_BUCKET, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_COAT_HANGER, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_HAND_BAG, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_TRUTH_SERUM, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_BUCKET, ThrowOnly_Info{true,  10}},
	{ITEM_TYPE::ITEM_NONE, ThrowOnly_Info{true, 0}},
};


Equipment_Info& CItemDatabase::Get_Equipment_Info(ITEM_TYPE eItemType)
{
	return m_MapEquipment[eItemType];
}

Consumable_Info& CItemDatabase::Get_Consumable_Info(ITEM_TYPE eItemType)
{
	return m_MapConsumable[eItemType];
}

Interactive_Info& CItemDatabase::Get_Interactive_Info(ITEM_TYPE eItemType)
{
	return m_MapInteractive[eItemType];
}

ThrowOnly_Info& CItemDatabase::Get_ThrowOnly_Info(ITEM_TYPE eItemType)
{
	return m_MapThrowOnly[eItemType];
}

//Interactive_Info& CItemDatabase::Get_ThrowOnly_Info(ITEM_TYPE eItemType)
//{
//	return m_MapThrowOnly[eItemType];
//}