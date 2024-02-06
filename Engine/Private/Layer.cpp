#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const wstring & strComTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)	
		++iter;

	return (*iter)->Find_Component(strComTag);	
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

CGameObject* CLayer::Get_GameObject(_int iObjectIndex)
{
	auto iter = m_GameObjects.begin();
	for (_int i = 0; i < iObjectIndex; i++)
	{
		iter++;
	}
	return *iter;
}

void CLayer::Enable_GameObject(_bool bEnable)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Enable(bEnable);
	}
}

void CLayer::Clear()
{
	m_GameObjects.clear();
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
 		if (nullptr != pGameObject)
			pGameObject->Tick(fTimeDelta);
	}
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(fTimeDelta);
	}
}

CLayer * CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}


