#include "stdafx.h"
#include "State_Aware_Ready.h"
#include "GameInstance.h"
#include "EnemyBase.h"

CState_Aware_Ready* CState_Aware_Ready::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Aware_Ready* pInstance = new CState_Aware_Ready(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Aware_Ready");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Aware_Ready::Initialize(void* pArg)
{
	return CState_Enemy::Initialize(pArg);
}

void CState_Aware_Ready::Free()
{
	CState_Enemy::Free();
}

void CState_Aware_Ready::BeginState(_float fTimeDelta)
{

}

_int CState_Aware_Ready::Tick_State(_float fTimeDelta, void* pArg)
{
	// 애니메이션 두번 끝나면 Aware에게
	if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_BATTLE_DEFAULT
		|| m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_BATTLE_IDLE)
	{
		return 2; // battle
	}

	if (m_pOwnerDetailed->Get_AnimPlayedCount() >= m_iAnimMaxCycle)
	{
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
		m_pOwnerDetailed->Set_GaugeActive(false);
		_uint iNextState = 2; // Battle
		return iNextState;
	}
	

	//m_fAnimAccTime += fTimeDelta;
	//if (m_fAnimAccTime >= m_fAnimMaxTime)
	//{
	//	m_fAnimAccTime = 0.f;
	//	m_pOwnerDetailed->Set_CurAction(0);
	//	_uint iNextState = m_pOwnerDetailed->Get_FSM()->Get_PrevState();
	//	return &iNextState;
	//}

	//m_pOwnerDetailed->Set_CurAction()
	return FSM_NO_EVENT;
}

void CState_Aware_Ready::CompleteState(_float fTimeDelta)
{


	m_pOwnerDetailed->Reset_CautiousState();
}

_int CState_Aware_Ready::LateTick_State(_float fTimeDelta, void* pArg)
{

	return FSM_NO_EVENT;
}
