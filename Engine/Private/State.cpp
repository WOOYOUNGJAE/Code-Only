#include "State.h"
#include "FSM.h"
#include "GameInstance.h"

HRESULT CState::Initialize(void* pArg)
{
	m_pOwner = static_cast<CGameObject*>(pArg);
	return S_OK;
}

void CState::Free()
{
	Safe_Release(m_pGraphic_Device);

	if (m_pInnerFSM)
	{
		Safe_Release(m_pInnerFSM);		
	}
}

HRESULT CState::Add_InnerFSM(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
	{
		MSG_BOX("Failed to Find FSM Component");
		return E_FAIL;
	}

	m_pInnerFSM = static_cast<CFSM*>(pComponent);

	/*for (_uint i = 0; i < m_pInnerFSM.capacity(); ++i)
	{
		m_pInnerFSM.push_back();
	}*/


	Safe_Release(pGameInstance);

	return S_OK;
}
