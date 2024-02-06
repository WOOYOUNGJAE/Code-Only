#include "FSM.h"
#include "GameObject.h"

CFSM* CFSM::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFSM* pInstance = new CFSM(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFSM");
	}

	return pInstance;
}

CComponent* CFSM::Clone(void* pArg)
{
	CFSM* pInstance = new CFSM(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CFSM");
	}

	return pInstance;
}

HRESULT CFSM::Initialize_Prototype()
{
	return CComponent::Initialize_Prototype();
}

HRESULT CFSM::Initialize(void* pArg)
{
	if (FAILED(CComponent::Initialize(pArg)))
	{
		return E_FAIL;
	}
	m_pOwner = static_cast<CGameObject*>(pArg); // 더 안전한 방법?
	return S_OK;
}

_int CFSM::Tick(_float fTimeDelta, void* pArg)
{
	if (m_vecState.size() <= 0)
		return FSM_NO_EVENT;

	// 새로운 스테이트라면
	if (m_iNextState != STATE_EMPTY && m_iNextState != m_iCurState)
	{
		m_vecState[m_iCurState]->CompleteState(fTimeDelta);

		m_iPrevState = m_iCurState;
		m_iCurState = m_iNextState;
		m_iNextState = STATE_EMPTY;

		m_vecState[m_iCurState]->BeginState(fTimeDelta);
	}

	// 현 스테이트 기본 함수
	return m_vecState[m_iCurState]->Tick_State(fTimeDelta, pArg);
}


_int CFSM::Late_Tick(_float fTimeDelta, void* pArg)
{
	if (m_vecState.size() <= 0)
		return FSM_NO_EVENT;
	
	return m_vecState[m_iCurState]->LateTick_State(fTimeDelta, pArg);
}

void CFSM::Free()
{
	CComponent::Free();
	for (int i = 0; i < m_vecState.size(); ++i)
	{
		Safe_Release(m_vecState[i]);
	}
	m_vecState.clear();
}

void CFSM::Change_Next_Immediately(_uint iNextState, _float fTimeDelta)
{
	if (m_vecState.size() <= 0)
		return;

	// 새로운 스테이트라면
	if (iNextState != STATE_EMPTY && iNextState != m_iCurState)
	{
		m_vecState[m_iCurState]->CompleteState(fTimeDelta);

		m_iPrevState = m_iCurState;
		m_iCurState = iNextState;
		m_iNextState = STATE_EMPTY;

		m_vecState[m_iCurState]->BeginState(fTimeDelta);
	}
}

void CFSM::Go_PrevState(_float fTimeDelta)
{
	if (m_iPrevState == STATE_EMPTY)
		return;
	
	m_vecState[m_iCurState]->CompleteState(fTimeDelta);

	// swap cur and prev
	_int iTmpState = m_iPrevState;
	m_iPrevState = m_iCurState;
	m_iCurState = iTmpState;
	
	m_vecState[m_iCurState]->BeginState(fTimeDelta);
}

HRESULT CFSM::Add_NewState(list<CState*>* pStatesList, CGameObject* pOwner)
{
	if (pStatesList == nullptr || pStatesList->size() == 0)
		return S_OK;

	for (auto& iter : *pStatesList)
	{
		if (iter == nullptr)
		{
			MSG_BOX("CFSM::Add_NewState\nElement of StateList is Empty");
			Safe_Release(iter);
			return E_FAIL;
		}

		m_vecState.push_back(iter);
	}
	return S_OK;
}

HRESULT CFSM::Add_NewState(CState* pState, CGameObject* pOwner)
{
	if (pState == nullptr)
		return S_OK;

	if (pState == nullptr)
	{
		MSG_BOX("CFSM::Add_NewState\nArgument is Empty");
		Safe_Release(pState);
		return E_FAIL;
	}
	if (pOwner)
	{
		pState->Set_Owner(pOwner);
	}
	m_vecState.push_back(pState);

	return S_OK;
}
