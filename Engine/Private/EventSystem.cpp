#include "EventSystem.h"

IMPLEMENT_SINGLETON(CEventSystem)

CEventSystem::CEventSystem()
{
}

HRESULT CEventSystem::Reserve_Manager(_uint iNumEvents)
{
	if (nullptr != m_pEvents)
	{
		return E_FAIL;
	}

	m_iNumEvents = iNumEvents;

	m_pEvents = new EVENT[iNumEvents];

	return S_OK;
}

HRESULT CEventSystem::Subscribe(_uint iEventIndex, std::function<_bool(void*)> fpCallback)
{
	if (iEventIndex < 0 || iEventIndex >= m_iNumEvents)
	{
		return E_FAIL;
	}

	m_pEvents[iEventIndex].push_back(fpCallback);

	return S_OK;
}

void CEventSystem::OnEvent(_uint iEventIndex, void* pArg)
{
	auto iter = m_pEvents[iEventIndex].begin();
	while (iter != m_pEvents[iEventIndex].end())
	{
		if ((*iter)(pArg) == false) 
		{
			iter = m_pEvents[iEventIndex].erase(iter);
		}
		else {
			++iter;
		}
	}
}

void CEventSystem::Clear()
{
	for (_uint i = 0; i < m_iNumEvents; ++i)
	{
		m_pEvents[i].clear();
	}
}

void CEventSystem::Free()
{
	Safe_Delete_Array(m_pEvents);
}
