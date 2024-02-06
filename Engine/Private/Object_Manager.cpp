#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "EffectManager.h"
#include "Particle_Manager.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
	: m_sectionControlThread(nullptr)
{
}

void CObject_Manager::SectionControlThread()
{
	std::unique_lock<std::mutex> lock(m_sectionMutex);
	while (true)
	{
		while (!m_bSectionChagneEventOccurred) 
		{
			m_sectionChangeEventCV.wait(lock);
		}

		// Kill Thread
		if (m_bThreadEnable == false)
		{
			break;
		}

		// Section 활성화 작업
		CLayer* pNextSection = Find_Section(m_iCurrentLevelIndex, m_strCurrentSectionTag);
		if (nullptr == pNextSection)
		{
			m_bSectionChagneEventOccurred = false;
			continue;
		}

		pNextSection->Enable_GameObject(true);

		// Section 비활성화 작업
		CLayer* pLastSection = Find_Section(m_iCurrentLevelIndex, m_strLastSectionTag);
		if (nullptr == pLastSection)
		{
			m_bSectionChagneEventOccurred = false;
			continue;
		}

		pLastSection->Enable_GameObject(false);

		m_bSectionChagneEventOccurred = false;
	}
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComTag, _uint iIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComTag, iIndex);	
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	if (nullptr != m_pSections)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[iNumLevels];
	m_pSections = new SECTIONS[iNumLevels];

	m_sectionControlThread = new std::thread(&CObject_Manager::SectionControlThread, this);
	m_strStaticSectionTag = TEXT("Section_Static");

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring& strPrototypeTag, CGameObject * pPrototype)
{
	/* 중복된 키로 추가하면 안되니까.*/
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	//CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	//if (nullptr == pPrototype)
	//	return E_FAIL;

	//CGameObject*		pGameObject = pPrototype->Clone(pArg);
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//CLayer*			pLayer = Find_Layer(iLevelIndex, strLayerTag);

	//if (nullptr == pLayer)
	//{
	//	pLayer = CLayer::Create();
	//	if (nullptr == pLayer)
	//		return E_FAIL;

	//	pLayer->Add_GameObject(pGameObject);

	//	m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	//}
	//else
	//	pLayer->Add_GameObject(pGameObject);

	return Add_GameObject(iLevelIndex, strLayerTag, strPrototypeTag, m_strStaticSectionTag, pArg);
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const wstring& strSectionTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	// Set Layer
	{
		CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();
			if (nullptr == pLayer)
				return E_FAIL;

			pLayer->Add_GameObject(pGameObject);

			m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
		}
		else
		{
			pLayer->Add_GameObject(pGameObject);
		}
	}

	// Set Section
	{
		CLayer* pSection = Find_Section(iLevelIndex, strSectionTag);

		if (nullptr == pSection)
		{
			pSection = CLayer::Create();
			if (nullptr == pSection)
			{
				return E_FAIL;
			}

			pSection->Add_GameObject(pGameObject);

			m_pSections[iLevelIndex].emplace(strSectionTag, pSection);
		}
		else
		{
			pSection->Add_GameObject(pGameObject);
		}
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_Effect(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	// Set Layer
	{
		CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();
			if (nullptr == pLayer)
				return E_FAIL;

			pLayer->Add_GameObject(pGameObject); 

			m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
		}
		else
		{
			pLayer->Add_GameObject(pGameObject);
		}
	}

	// Set Section
	{
		CLayer* pSection = Find_Section(iLevelIndex, m_strStaticSectionTag);

		if (nullptr == pSection)
		{
			pSection = CLayer::Create();
			if (nullptr == pSection)
			{
				return E_FAIL;
			}

			pSection->Add_GameObject(pGameObject);

			m_pSections[iLevelIndex].emplace(m_strStaticSectionTag, pSection);
		}
		else
		{
			pSection->Add_GameObject(pGameObject);
		}
	}
	
	//CEffectManager::Get_Instance()->Add_Effect((CBlendObject*)pGameObject, *(_uint*)pArg);
	CEffectManager::Get_Instance()->Add_Effect((CBlendObject*)pGameObject, (EFFECTARG*)pArg);
	
	return S_OK;
}

HRESULT CObject_Manager::Add_Particle(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	// Set Layer
	{
		CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

		if (nullptr == pLayer)
		{
			pLayer = CLayer::Create();
			if (nullptr == pLayer)
				return E_FAIL;

			pLayer->Add_GameObject(pGameObject);

			m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
		}
		else
		{
			pLayer->Add_GameObject(pGameObject);
		}
	}

	// Set Section
	{
		CLayer* pSection = Find_Section(iLevelIndex, m_strStaticSectionTag);

		if (nullptr == pSection)
		{
			pSection = CLayer::Create();
			if (nullptr == pSection)
			{
				return E_FAIL;
			}

			pSection->Add_GameObject(pGameObject);

			m_pSections[iLevelIndex].emplace(m_strStaticSectionTag, pSection);
		}
		else
		{
			pSection->Add_GameObject(pGameObject);
		}
	}

	PARTICLEDESC* pParticleDesc = (PARTICLEDESC*)pArg;
	
	CParticle_Manager::Get_Instance()->Add_Particle(pArg);

	return S_OK;
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _int iObjectIndex)
{
	CLayer* pCLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pCLayer)
		return nullptr;
	return pCLayer->Get_GameObject(iObjectIndex);
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject)
{
	CLayer* pCLayer = Find_Section(iLevelIndex, strLayerTag);
	if (nullptr == pCLayer)
	{
		MSG_BOX("Failed to Find Layer's GameObject");
		return E_FAIL;
	}

	if (nullptr == pGameObject)
		return E_FAIL;

	

	auto& GameObjectList = pCLayer->Get_GameObjectList(); 

	for (auto& GameObject = GameObjectList.begin(); GameObject != GameObjectList.end();)
	{
		if (pGameObject == *GameObject)
		{
			
			GameObject = GameObjectList.erase(GameObject);

		}
		else
		{
			++GameObject;
		}
	}

	return S_OK;
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	if (m_bLevelChanged)
	{
		for (auto layer : m_vecLayerBuffer)
		{
			layer->Clear();
			Safe_Release(layer);
		}
			
		m_vecLayerBuffer.clear();

		m_bLevelChanged = false;
	}

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		auto end = m_pSections[i].end();
		if (m_pSections[i].find(m_strCurrentSectionTag) != end)
			m_pSections[i][m_strCurrentSectionTag]->Tick(fTimeDelta);

		if (m_pSections[i].find(m_strStaticSectionTag) != end)
			m_pSections[i][m_strStaticSectionTag]->Tick(fTimeDelta);
	}
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		auto end = m_pSections[i].end();
		if (m_pSections[i].find(m_strCurrentSectionTag) != end)
			m_pSections[i][m_strCurrentSectionTag]->Late_Tick(fTimeDelta);

		if (m_pSections[i].find(m_strStaticSectionTag) != end)
 			m_pSections[i][m_strStaticSectionTag]->Late_Tick(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);
	m_pLayers[iLevelIndex].clear();

	for (auto& Pair : m_pSections[iLevelIndex])
		m_vecLayerBuffer.push_back(Pair.second);
	m_pSections[iLevelIndex].clear();

	m_bLevelChanged = true;
}

HRESULT CObject_Manager::Change_Section(_uint iLevelIndex, const wstring& strLastSectionTag, const wstring& strNextSectionTag)
{
	m_iCurrentLevelIndex = iLevelIndex;
	m_strLastSectionTag = strLastSectionTag;
	m_strCurrentSectionTag = strNextSectionTag;

	{
		std::lock_guard<std::mutex> lock(m_sectionMutex);
		m_bSectionChagneEventOccurred = true;
	}
	m_sectionChangeEventCV.notify_one();

	return S_OK;
}

HRESULT CObject_Manager::Enable_Section(_uint iLevelIndex, const wstring& strSectionTag)
{
	CLayer* pSection = Find_Section(iLevelIndex, strSectionTag);
	if (nullptr == pSection)
	{
		return E_FAIL;
	}

	m_strLastSectionTag = strSectionTag;
	m_strCurrentSectionTag = strSectionTag;

	pSection->Enable_GameObject(true);

	return S_OK;
}

HRESULT CObject_Manager::Disable_Section(_uint iLevelIndex, const wstring& strSectionTag)
{
	CLayer* pSection = Find_Section(iLevelIndex, strSectionTag);
	if (nullptr == pSection)
	{
		return E_FAIL;
	}

	pSection->Enable_GameObject(false);

	return S_OK;
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CLayer* CObject_Manager::Find_Section(_uint iLevelIndex, const wstring& strSectionTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pSections[iLevelIndex].find(strSectionTag);

	if (iter == m_pSections[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pSections[i])
		{
			Pair.second->Clear();
			Safe_Release(Pair.second);
		}
		m_pSections[i].clear();
	}
	Safe_Delete_Array(m_pSections);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();

	for (auto& layer : m_vecLayerBuffer)
		Safe_Release(layer);

	m_vecLayerBuffer.clear();

	// Kill Section Thread
	{
		m_bThreadEnable = false;
		m_bSectionChagneEventOccurred = true;
		m_sectionChangeEventCV.notify_one();

		m_sectionControlThread->join();
		Safe_Delete(m_sectionControlThread);
	}
}
