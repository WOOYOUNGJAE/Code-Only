#include "stdafx.h"
#include "State_Idle.h"
#include "GameInstance.h"
#include "EnemyBase.h"

#pragma region Idle_Walk
CState_Idle_Enemy::CState_Idle_Enemy_Walk* CState_Idle_Enemy::CState_Idle_Enemy_Walk::Create(
	LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CState_Idle_Enemy_Walk* pInstance = new CState_Idle_Enemy_Walk(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Idle_Enemy_Walk");
	}

	pInstance->Initialize();
	return pInstance;
}

void CState_Idle_Enemy::CState_Idle_Enemy_Walk::BeginState(_float fTimeDelta)
{
}

_int CState_Idle_Enemy::CState_Idle_Enemy_Walk::Tick_State(_float fTimeDelta, void* pArg)
{
	return S_OK;
}

void CState_Idle_Enemy::CState_Idle_Enemy_Walk::CompleteState(_float fTimeDelta)
{
}

_int CState_Idle_Enemy::CState_Idle_Enemy_Walk::LateTick_State(_float fTimeDelta, void* pArg)
{
	return S_OK;
}
#pragma endregion Idle_Walk

CState_Idle_Enemy* CState_Idle_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Idle_Enemy* pInstance = new CState_Idle_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Idle_Enemy");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Idle_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}
	 
	m_bIsDynamic = m_pOwnerDetailed->Is_Dynamic();
	//if (m_bIsDynamic)
	//{
	//	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
	//		return E_FAIL;

	//	// Add Walking States into Inner FSM
	//	
	//}
	return S_OK;
}

void CState_Idle_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Idle_Enemy::BeginState(_float fTimeDelta)
{

}

_int CState_Idle_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{


	if (m_bIsDynamic) // 돌아다니는 자식
	{
		
	}
	else // 가만히 있는 적
	{
		
	}
	return FSM_NO_EVENT;
}

void CState_Idle_Enemy::CompleteState(_float fTimeDelta)
{

	m_pOwnerDetailed->Reset_IdleState();
}

_int CState_Idle_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{

	_int iResult = FSM_NO_EVENT;
	if (m_bIsDynamic)
	{
		iResult = FSM_NO_EVENT;

	}
	else // 가만히 있는 적
	{
		iResult = m_pOwnerDetailed->LateTick_NotAware(fTimeDelta);
	}

	return iResult;
}

// --boss--
CState_Idle_Boss* CState_Idle_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Idle_Boss* pInstance = new CState_Idle_Boss(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Idle_Boss");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Idle_Boss::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CState_Idle_Boss::Free()
{
	CState_Enemy::Free();
}

void CState_Idle_Boss::BeginState(_float fTimeDelta)
{
	//cout << "Enemy Idle State Begin" << endl;
}

_int CState_Idle_Boss::Tick_State(_float fTimeDelta, void* pArg)
{

	return FSM_NO_EVENT;
}

void CState_Idle_Boss::CompleteState(_float fTimeDelta)
{
	m_pOwnerDetailed->Reset_IdleState();
}

_int CState_Idle_Boss::LateTick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	iResult = m_pOwnerDetailed->LateTick_NotAware(fTimeDelta);

	return iResult;
}