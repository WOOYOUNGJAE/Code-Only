#include "stdafx.h"
#include "State_AttackReady.h"
#include "State_Battle.h"
#include "GameInstance.h"
#include <EnemyBase.h>

CState_AttackReady_Enemy* CState_AttackReady_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_AttackReady_Enemy* pInstance = new CState_AttackReady_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_AttackReady_Enemy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_AttackReady_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CState_AttackReady_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_AttackReady_Enemy::BeginState(_float fTimeDelta)
{
	// Ready Action
}

_int CState_AttackReady_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	if (*static_cast<_uint*>(pArg) == 1 << 1)
	{
		//MSG_BOX("공격 논리 오류");
		return FSM_NO_EVENT;
	}
	_uint iNextState = 3; // Attack

	m_fChoosingAccTime += fTimeDelta;
	if (m_fChoosingAccTime < m_fChoosingTimeMax)
	{
		return FSM_NO_EVENT;
	}
	m_fChoosingAccTime = 0.f;

	int iRandom = rand() % 10;
	if (iRandom > 3) // TODO : 확률 조정	
	{
		if (rand() % 2 == 0)
		{
			m_pOwnerDetailed->Play_MySound(TEXT("HmRargh!.wav"));				
		}
		else
		{
			m_pOwnerDetailed->Play_MySound(TEXT("hmRargh!H.wav"));				
		}
		
			

		*static_cast<_uint*>(pArg) = 1 << 0;  // Basic Attack
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BASIC_ATTACK);
	}
	else
	{
		*static_cast<_uint*>(pArg) = 1 << 2;// heavy attack
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_HEAVY_ATTACK_WINDUP);
	}
	return iNextState;
}

void CState_AttackReady_Enemy::CompleteState(_float fTimeDelta)
{

}

_int CState_AttackReady_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{

	return m_pInnerFSM->Late_Tick(fTimeDelta);
}

#pragma region MyRegion CState_AttackReady_PipeGuy


CState_AttackReady_PipeGuy* CState_AttackReady_PipeGuy::Create(LPDIRECT3DDEVICE9 pGraphic_Device,
	CEnemyBase* pOwnerDetailed)
{
	CState_AttackReady_PipeGuy* pInstance = new CState_AttackReady_PipeGuy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_AttackReady_PipeGuy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_AttackReady_PipeGuy::Initialize(void* pArg)
{
	return CState_AttackReady_Enemy::Initialize(pArg);
}

void CState_AttackReady_PipeGuy::BeginState(_float fTimeDelta)
{
	CState_AttackReady_Enemy::BeginState(fTimeDelta);
}

_int CState_AttackReady_PipeGuy::Tick_State(_float fTimeDelta, void* pArg)
{
	_uint iNextState = 1; // Attack

	m_fChoosingAccTime += fTimeDelta;
	if (m_fChoosingAccTime < m_fChoosingTimeMax)
	{
		return FSM_NO_EVENT;
	}
	m_fChoosingAccTime = 0.f;

	int iRandom = rand() % 10;
	if (iRandom > 3) // TODO : 확률 조정	
	{
		*static_cast<_uint*>(pArg) = 1 << 0;  // Basic Attack
		m_pOwnerDetailed->Play_MySound(TEXT("Swing2.wav"));
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BASIC_ATTACK);
	}
	else
	{
		*static_cast<_uint*>(pArg) = 1 << 2;// heavy attack
		m_pOwnerDetailed->Play_MySound(TEXT("Swing1.wav"));
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_HEAVY_ATTACK);
	}
	return iNextState;
}

void CState_AttackReady_PipeGuy::CompleteState(_float fTimeDelta)
{
	CState_AttackReady_Enemy::CompleteState(fTimeDelta);
}

_int CState_AttackReady_PipeGuy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return CState_AttackReady_Enemy::LateTick_State(fTimeDelta, pArg);
}

void CState_AttackReady_PipeGuy::Free()
{
	CState_AttackReady_Enemy::Free();
}
#pragma endregion CState_AttackReady_PipeGuy
