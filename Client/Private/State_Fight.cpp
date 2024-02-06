#include "stdafx.h"
#include "State_Fight.h"
#include "GameInstance.h"

CState_Fight* CState_Fight::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CState_Fight* pInstance = new CState_Fight(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Fight");
	}

	pInstance->Initialize();
	return pInstance;
}

HRESULT CState_Fight::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Fight::BeginState(_float fTimeDelta)
{
	m_eFight_State = FIGHT_STATE::MOVING;
}

_int CState_Fight::Tick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}

void CState_Fight::CompleteState(_float fTimeDelta)
{
	m_eFight_State = FIGHT_STATE::MOVING;
}

_int CState_Fight::LateTick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}

void CState_Fight::Free()
{
	CState::Free();
}
