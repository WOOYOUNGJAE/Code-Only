#pragma once

#include "Client_Struct.h"
#include <unordered_map>

class CEventDatabase
{
private:
	CEventDatabase() = delete;

public:
	static float& Get_Tooltip_Event_ActiveTime(TOOLTIP eTooltip);
	static float& Get_Radio_Event_ActiveTime(int radioIndex);

private:
	static std::unordered_map<TOOLTIP, float> m_TooltipMap;
	static std::unordered_map<int, float> m_RadioMap;
};

