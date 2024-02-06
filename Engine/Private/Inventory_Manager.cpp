#include "Inventory_Manager.h"
#include "Inventory_Engine.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CInventory_Manager);

CInventory_Manager::CInventory_Manager()
{

}

HRESULT CInventory_Manager::Reserve_Inventory_Manager(_uint iNumLevels)
{
    return S_OK;
}

HRESULT CInventory_Manager::Add_Prototype_Inventory(const wstring& strPrototypeUITag, CUI* pUI)
{
    return S_OK;
}

HRESULT CInventory_Manager::Add_Inventory(_uint iLevelIndex, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg)
{
    return S_OK;
}

void CInventory_Manager::Tick(_float fTimeDelta)
{
}

void CInventory_Manager::Late_Tick(_float fTimeDelta)
{
}

void CInventory_Manager::Clear(_uint iLevelIndex)
{
}
