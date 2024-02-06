#include "stdafx.h"
#include "State_Follow.h"
#include "EnemyBase.h"

CState_Follow_Enemy* CState_Follow_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, _float fDist_BattleStart,
	void* pArg)
{
	CState_Follow_Enemy* pInstance = new CState_Follow_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Follow_Enemy");
	}
	pInstance->m_fDist_BattleStart = fDist_BattleStart;
	pInstance->Initialize(pOwnerDetailed);

	return pInstance;
}

HRESULT CState_Follow_Enemy::Initialize(void* pArg)
{
	_float fRandom = rand() % 15;
	fRandom *= 0.1f;
	m_fRandom_MoreWalkDistance = fRandom;

	m_fRandomAddingX = cosf(rand()); // -1 ~ 1

	return CState_Enemy::Initialize(pArg);
}

void CState_Follow_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Follow_Enemy::BeginState(_float fTimeDelta)
{
	//m_vRandomAdding = m_pOwnerDetailed->Get_NearRandomPos(0.1f);
	//m_vRandomAdding = m_pOwnerDetailed->Get_Pos();
	m_pOwnerDetailed->Set_Rest(false);
}

_int CState_Follow_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	_uint iNextState = 1;// 0;
	if (m_pOwnerDetailed->Chase_Target(m_fDist_BattleStart, fTimeDelta, m_fRandomAddingX, m_fRandom_MoreWalkDistance) == true)
	{
		return -1; // Follow
	}
	else // 충분히 따라갔으면 다음 스테이트는 Battle이다 알려주기
	{
		iNextState = 2; // Battle
	}
	
	return iNextState;
}

void CState_Follow_Enemy::CompleteState(_float fTimeDelta)
{
}

_int CState_Follow_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return -1;
}
