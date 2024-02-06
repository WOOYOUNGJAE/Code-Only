#include "stdafx.h"
#include "State_Rush.h"
#include "State_Battle.h"
#include "GameInstance.h"
#include <EnemyBase.h>

CState_Rush_Enemy* CState_Rush_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Rush_Enemy* pInstance = new CState_Rush_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Rush_Enemy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Rush_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CState_Rush_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Rush_Enemy::BeginState(_float fTimeDelta)
{
}

_int CState_Rush_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	_uint iNextState = 0;
	//if (m_pOwnerDetailed->Chase_Target(m_fDist_Attackable, m_fRushSpeed * fTimeDelta, TODO, TODO) == true)
	//{
	//	return FSM_NO_EVENT; // Rush
	//}
	//else // 충분히 따라갔으면 다음 스테이트는 Battle이다 알려주기
	//{
	//	*static_cast<_uint*>(pArg) = (1 << 1); // Rush Attack
	//	//*static_cast<_uint*>(pArg) = (1 << 1); // Rush Attack
	//	m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BASIC_ATTACK);
	//	//m_pOwnerDetailed->Set_NextAction(actionrush)
	//	iNextState = 3; // Attack
	//}

	return iNextState;
}

void CState_Rush_Enemy::CompleteState(_float fTimeDelta)
{
	

}

_int CState_Rush_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}