#pragma once

#include "Client_Struct.h"
#include <unordered_map>

class CItemDatabase
{
private:
	CItemDatabase() = delete;

public:
	static Equipment_Info& Get_Equipment_Info(ITEM_TYPE eItemType);

	static Consumable_Info& Get_Consumable_Info(ITEM_TYPE eItemType);

	static Interactive_Info& Get_Interactive_Info(ITEM_TYPE eItemType);

	static ThrowOnly_Info& Get_ThrowOnly_Info(ITEM_TYPE eItemType);

private:
	static std::unordered_map<ITEM_TYPE, Equipment_Info> m_MapEquipment;
	static std::unordered_map<ITEM_TYPE, Consumable_Info> m_MapConsumable;
	static std::unordered_map<ITEM_TYPE, Interactive_Info> m_MapInteractive;
	static std::unordered_map<ITEM_TYPE, ThrowOnly_Info> m_MapThrowOnly;
};