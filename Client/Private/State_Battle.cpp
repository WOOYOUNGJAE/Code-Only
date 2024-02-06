#include "stdafx.h"
#include "State_Fight.h"
#include "State_Battle.h"
#include "GameInstance.h"
#include "State_Moving.h"
#include "State_AttackReady.h"
#include "State_Attack.h"
#include "State_Rush.h"
#include <EnemyBase.h>


CState_Battle_Enemy* CState_Battle_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Battle_Enemy* pInstance = new CState_Battle_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Battle_Enemy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Battle_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	list<CState*> tmpStateList;

	// Add Inner States
	tmpStateList.emplace_back(CState_AttackReady_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed));
	tmpStateList.emplace_back(CState_Moving_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed)); 
	tmpStateList.emplace_back(CState_Rush_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed));
	tmpStateList.emplace_back(CState_Attack_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed)); 

	return 	m_pInnerFSM->Add_NewState(&tmpStateList, m_pOwnerDetailed);
}

void CState_Battle_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Battle_Enemy::BeginState(_float fTimeDelta)
{

}

_int CState_Battle_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	_float fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	if (m_pInnerFSM->Is_State(RUSH) == false && fDistance > m_fDist_AttackReady)
	{
		m_pInnerFSM->Set_NextState(MOVING);
	}

	iResult = m_pInnerFSM->Tick(fTimeDelta, &m_iArg);

	if (iResult != FSM_NO_EVENT)
	{
		m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
	}

	iResult = FSM_NO_EVENT;
	return iResult;
}

void CState_Battle_Enemy::CompleteState(_float fTimeDelta)
{
	

}

_int CState_Battle_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	

	return m_pInnerFSM->Late_Tick(fTimeDelta);
}

#pragma region CState_Battle_PipeGuy


CState_Battle_PipeGuy* CState_Battle_PipeGuy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Battle_PipeGuy* pInstance = new CState_Battle_PipeGuy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Battle_PipeGuy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Battle_PipeGuy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	list<CState*> tmpStateList;

	// Add Inner States
	tmpStateList.emplace_back(CState_AttackReady_PipeGuy::Create(m_pGraphic_Device, m_pOwnerDetailed));
	tmpStateList.emplace_back(CState_Attack_PipeGuy::Create(m_pGraphic_Device, m_pOwnerDetailed));
	//tmpStateList.emplace_back(CState_Moving_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed));
	//tmpStateList.emplace_back(CState_Rush_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed));

	return 	m_pInnerFSM->Add_NewState(&tmpStateList, m_pOwnerDetailed);
}

void CState_Battle_PipeGuy::BeginState(_float fTimeDelta)
{
	CState_Battle_Enemy::BeginState(fTimeDelta);
}

_int CState_Battle_PipeGuy::Tick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	_float fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	// 공격중이면 리턴
	/*if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_BASIC_ATTACK ||
		m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_HEAVY_ATTACK)*/
	if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_BASIC_ATTACK ||
		m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_HEAVY_ATTACK)
	{
		m_fRestAfterAttackAccTime = 0.f;
		iResult = m_pInnerFSM->Tick(fTimeDelta, &m_iArg);

		if (iResult != FSM_NO_EVENT)
		{
			m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
		}

		iResult = FSM_NO_EVENT;
		return iResult;
	}
	else if (m_pOwnerDetailed->Get_CurAction() != CEnemyBase::ACTION_WALK)
	{
		if (fDistance > m_fDist_AttackReady) // 따라가야 하는 거리
		{
			m_fRestAfterAttackAccTime += m_fRestAfterAttackMaxTime;
			if (m_fRestAfterAttackAccTime > m_fRestAfterAttackMaxTime)
			{
				m_fRestAfterAttackAccTime = 0.f;
				m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
			}
		}
		else // 가까운 거리
		{
			m_fRestAfterAttackAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
			m_pInnerFSM->Set_NextState(ATTACK_READY);			
		}
		
	}
	// 완전히 따라갈 수 있는 상태
	else // Action Walk
	{
		m_fRestAfterAttackAccTime = 0.f;
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		if (m_pOwnerDetailed->Chase_Target(m_fDist_AttackReady, fTimeDelta, 0.f, 0.f) == false) // 가까우면 공격 줁비
		{
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
			m_pInnerFSM->Set_NextState(ATTACK_READY);
		}
		else
		{
			// 더 따라가야 한다면 state실행 안하고 다시
			return iResult;
		}
	}

	iResult = m_pInnerFSM->Tick(fTimeDelta, &m_iArg);

	if (iResult != FSM_NO_EVENT)
	{
		m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
	}

	iResult = FSM_NO_EVENT;
	return iResult;
}

void CState_Battle_PipeGuy::CompleteState(_float fTimeDelta)
{
	m_fRestAfterAttackAccTime = 0.f;
	CState_Battle_Enemy::CompleteState(fTimeDelta);
}

_int CState_Battle_PipeGuy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return CState_Battle_Enemy::LateTick_State(fTimeDelta, pArg);
}

void CState_Battle_PipeGuy::Free()
{
	CState_Battle_Enemy::Free();
}


#pragma endregion CState_Battle_PipeGuy