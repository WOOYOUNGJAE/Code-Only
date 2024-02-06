#include "stdafx.h"
#include "EventDatabase.h"

std::unordered_map<TOOLTIP, float> CEventDatabase::m_TooltipMap
{
	// Section 1
	{TOOLTIP_KICK, 5.f},
	{TOOLTIP_SUSPICION, 5.f},
	{TOOLTIP_HEAL, 5.f},
	{TOOLTIP_KEY, 5.f},

	// Section 2
	{TOOLTIP_STEALTH, 5.f},
	{TOOLTIP_ZIPPO, 5.f},
	{TOOLTIP_ATTACK, 5.f},

	// Section 3
	{TOOLTIP_DODGE, 5.f},

	// Section 4
	{TOOLTIP_THROW, 5.f},

	// Section 5

	// Section 6
	{TOOLTIP_TABULATOR, 5.f},
};

std::unordered_map<int, float> CEventDatabase::m_RadioMap
{
	{1, 9.81f},
	{2, 7.62f},
	{3, 6.08f},
	{4, 6.17f},
	{5, 3.17f},
	{6, 9.48f},
	{9, 8.37f},
};

float& CEventDatabase::Get_Tooltip_Event_ActiveTime(TOOLTIP eTooltip)
{
	return m_TooltipMap[eTooltip];
}

float& CEventDatabase::Get_Radio_Event_ActiveTime(int radioIndex)
{
	return m_RadioMap[radioIndex];
}
