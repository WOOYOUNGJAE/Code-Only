#include "stdafx.h"
#include "State_Enemy.h"
#include "EnemyBase.h"

HRESULT CState_Enemy::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("pOwner is Empty");
		return E_FAIL;
	}

	if (FAILED(CState::Initialize(pArg)))
	{
		return E_FAIL;
	}

	m_pOwnerDetailed = static_cast<CEnemyBase*>(pArg);
	return S_OK;
}

HRESULT CState_Enemy::Set_OwnerDetailed(CGameObject* pOwner_Original)
{
	m_pOwnerDetailed = static_cast<CEnemyBase*>(pOwner_Original);
	return S_OK;
}
