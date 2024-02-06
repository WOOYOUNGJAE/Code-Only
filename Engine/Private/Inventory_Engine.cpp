#include "Inventory_Engine.h"

IMPLEMENT_SINGLETON(CInventory_Engine);

CInventory_Engine::CInventory_Engine(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CInventory_Engine::CInventory_Engine(const CInventory_Engine& rhs)
    :CGameObject(rhs)
{
}

HRESULT CInventory_Engine::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInventory_Engine::Initialize(void* pArg)
{
    return S_OK;
}

void CInventory_Engine::Tick(_float fTimeDelta)
{
}

void CInventory_Engine::Late_Tick(_float fTimeDelta)
{
}

CInventory_Engine* CInventory_Engine::Clone(void* pArg)
{
    return nullptr;
}

void CInventory_Engine::Free()
{
    __super::Free();
}
