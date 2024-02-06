#include "stdafx.h"
#include "State_Test.h"
#include "GameInstance.h"
#include "FSM.h"

#pragma region CEnemy0State_Test
CEnemy0State_Test* CEnemy0State_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy0State_Test* pInstance = new CEnemy0State_Test(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CEnemy0State_Test");
	}

	pInstance->Initialize();
	return pInstance;
}

void* CEnemy0State_Test::BeginState(_float fTimeDelta, void* pArg)
{
	//cout << "Enemy0 Test State Begin" << endl;
	return S_OK;
}

void* CEnemy0State_Test::Tick_State(_float fTimeDelta, void* pArg)
{
	//cout << "Enemy0 Test Tick" << endl;
	return S_OK;
}

void* CEnemy0State_Test::CompleteState(_float fTimeDelta, void* pArg)
{
	//cout << "Enemy0 Test State Complete" << endl;
	return S_OK;
}

void* CEnemy0State_Test::LateTick_State(_float fTimeDelta, void* pArg)
{
	//cout << "Enemy0 Test Late Tick" << endl;
	return S_OK;
}
#pragma endregion CEnemy0State_Test

#pragma region CEnemy0State_TestDetailed
CEnemy0State_TestDetailed* CEnemy0State_TestDetailed::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy0State_TestDetailed* pInstance = new CEnemy0State_TestDetailed(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CEnemy0State_TestDetailed");
	}

	pInstance->Initialize();
	return pInstance;
}

HRESULT CEnemy0State_TestDetailed::Initialize(void* pArg)
{
	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
	{
		return E_FAIL;
	}

	// Add States
	list<CState*> tmpStateList;
	tmpStateList.emplace_back(CEnemy0State_TestDetailed_0::Create(m_pGraphic_Device));
	tmpStateList.emplace_back(CEnemy0State_TestDetailed_1::Create(m_pGraphic_Device));

	return m_vecInnerFSM[0]->Add_NewState(&tmpStateList);

	return S_OK;
}

void CEnemy0State_TestDetailed::Free()
{
	CState::Free();
}

void* CEnemy0State_TestDetailed::BeginState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed::BeginState" << endl;
	for (_uint i = 0; i < m_vecInnerFSM.size(); ++i)
	{
		m_vecInnerFSM[i]->Set_NextState(0);
	}

	return S_OK;
}

void* CEnemy0State_TestDetailed::Tick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed::Tick_State" << endl;
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_vecInnerFSM[0]->Set_NextState(ENEMY0BATTLE_STATE::STATE_FOLLOW);
	}
	for (_uint i = 0; i < m_vecInnerFSM.size(); ++i)
	{
		if (FAILED(m_vecInnerFSM[i]->Tick(fTimeDelta)))
			return E_FAIL;
	}

	return S_OK;
}

void* CEnemy0State_TestDetailed::CompleteState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed::CompleteState" << endl;
	for (_uint i = 0; i < m_vecInnerFSM.size(); ++i)
	{
		m_vecInnerFSM[i]->Set_NextState(0);
	}

	return S_OK;
}

void* CEnemy0State_TestDetailed::LateTick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed::LateTick" << endl;
	for (_uint i = 0; i < m_vecInnerFSM.size(); ++i)
	{
		if (FAILED(m_vecInnerFSM[i]->Late_Tick(fTimeDelta)))
			return E_FAIL;
	}

	return S_OK;
}

#pragma region CEnemy0State_TestDetailed_0
CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0::
Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy0State_TestDetailed_0* pInstance = new CEnemy0State_TestDetailed_0(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CEnemy0State_TestDetailed_0");
	}

	pInstance->Initialize();
	return pInstance;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0::BeginState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_0 Begin" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0::Tick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_0 Tick" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0::CompleteState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_0 Complete" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_0::LateTick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_0 LateTick" << endl;
	return S_OK;
}
#pragma endregion CEnemy0State_TestDetailed_0

#pragma region Enemy0BattleState_1

CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1::Create(
	LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnemy0State_TestDetailed_1* pInstance = new CEnemy0State_TestDetailed_1(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CEnemy0State_TestDetailed_1");
	}

	pInstance->Initialize();
	return pInstance;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1::BeginState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_1 Begin" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1::Tick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_1 Tick" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1::CompleteState(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_1 Complete" << endl;
	return S_OK;
}

void* CEnemy0State_TestDetailed::CEnemy0State_TestDetailed_1::LateTick_State(_float fTimeDelta, void* pArg)
{
	//cout << "CEnemy0State_TestDetailed_1 LateTick" << endl;
	return S_OK;
}
#pragma endregion Enemy0BattleState_1

#pragma endregion CEnemy0State_TestDetailed