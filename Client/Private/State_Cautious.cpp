#include "stdafx.h"
#include "State_Cautious.h"
#include "GameInstance.h"
#include "EnemyBase.h"

CState_Cautious_Enemy* CState_Cautious_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Cautious_Enemy* pInstance = new CState_Cautious_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Cautious_Enemy");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Cautious_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CState_Cautious_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Cautious_Enemy::BeginState(_float fTimeDelta)
{
	m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_SUSPICIOUS);
}

_int CState_Cautious_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	m_fCautiousAccTime += fTimeDelta;

	if (m_fCautiousAccTime <= m_fStayTurnMaxTime)
	{
		// 제자리에서 방향 바꾸기
		m_fRemainCurDirAccTime += fTimeDelta;
		if (m_fRemainCurDirAccTime > m_fRemainCurDirMaxTime)
		{
			m_pOwnerDetailed->Make_Look_Target();
			m_fRemainCurDirAccTime = 0.f;
			// Change Look
			m_fTurnAngle += 135.f;
			m_pOwnerDetailed->StayTurn(m_fTurnAngle);
		}

	}
	else if (m_fCautiousAccTime <= m_fCautiousMaxTime)
	{
		//m_pOwnerDetailed

	}
	else
	{
		m_fRemainCurDirAccTime = 0.f;
		m_fCautiousAccTime = 0.f;
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_STAND);
		_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::IDLE;
		m_pOwnerDetailed->Set_NextFSMState(iNextState);
		return iNextState;
	}

	return FSM_NO_EVENT;
}

void CState_Cautious_Enemy::CompleteState(_float fTimeDelta)
{
	m_fCautiousAccTime = 0.f;
	m_fRemainCurDirAccTime = 0.f;
	
	m_pOwnerDetailed->Reset_CautiousState();
}

_int CState_Cautious_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	m_pOwnerDetailed->LateTick_NotAware(fTimeDelta);

	return FSM_NO_EVENT;
}



// --boss--
CState_Cautious_Boss* CState_Cautious_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Cautious_Boss* pInstance = new CState_Cautious_Boss(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Cautious_Boss");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Cautious_Boss::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CState_Cautious_Boss::Free()
{
	CState_Enemy::Free();
}

void CState_Cautious_Boss::BeginState(_float fTimeDelta)
{
	//cout << "Enemy Idle State Begin" << endl;
}

_int CState_Cautious_Boss::Tick_State(_float fTimeDelta, void* pArg)
{

	return FSM_NO_EVENT;
}

void CState_Cautious_Boss::CompleteState(_float fTimeDelta)
{
	m_pOwnerDetailed->Reset_IdleState();
}

_int CState_Cautious_Boss::LateTick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	iResult = m_pOwnerDetailed->LateTick_NotAware(fTimeDelta);

	return iResult;
}