#include "stdafx.h"
#include "GimmickManager.h"
#include "Gimmick_Object.h"
IMPLEMENT_SINGLETON(CGimmickManager)

void CGimmickManager::Free()
{
}

void CGimmickManager::Set_KeyInstance(CGimmick_Object* pKeyInstance)
{
	if (m_pKeyInstance == nullptr)
	{
		m_pKeyInstance = pKeyInstance;
	}
}

void CGimmickManager::DropKey(_float3 vDropPos)
{
	m_pKeyInstance->Set_Pickable(true);
	m_pKeyInstance->Set_RenderAfterUsed(true);
	m_pKeyInstance->Set_Position(vDropPos);
}

