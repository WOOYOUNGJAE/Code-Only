#include "stdafx.h"
#include "Item_Consumable.h"
#include "ItemDatabase.h"
#include "GameInstance.h"

CItem_Consumable::CItem_Consumable(LPDIRECT3DDEVICE9 pGrahic_Device)
	:CInteractiveObject(pGrahic_Device)
{
}

CItem_Consumable::CItem_Consumable(const CInteractiveObject& rhs)
	: CInteractiveObject(rhs)
{
}



HRESULT CItem_Consumable::Initialize_Prototype()
{
    return S_OK;
}



HRESULT CItem_Consumable::Initialize(void* pArg)
{
    OBJECTDESC* itemdesc = (OBJECTDESC*)pArg;

    HRESULT hr = __super::Initialize(pArg);
    if (FAILED(hr))
        return hr;

    //타입을 받아옴
    m_eItemType = itemdesc->eItemType;

    if (m_eItemType == ITEM_BANANA)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Banana_Outline"),
            TEXT("Com_Texture_Banana_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

  

    else if (m_eItemType == ITEM_APPLE)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Apple_Outline"),
            TEXT("Com_Texture_Food_Apple_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_COLA)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Cola_Outline"),
            TEXT("Com_Texture_Food_Cola_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_MEDIKIT)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Medkit_#7192_Outline"),
            TEXT("Com_Texture_Medkit_#7192_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    Consumable_Info& temp = CItemDatabase::Get_Consumable_Info(itemdesc->eItemType);

   // m_bAlive = temp.bAllive;
    m_iHeal = temp.iHeal;
    m_iThrowingDamage = temp.iThrowingDamage;

    return S_OK;
}

void CItem_Consumable::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (true == m_bPrompt)
    {
       /* if (ITEM_MEDIKIT == m_eItemType)
        {
            CGameInstance* pGameIns = CGameInstance::Get_Instance();
            Safe_AddRef(pGameIns);

            PROMPTER Prompter = PROMPTER_BOTTLE;

            pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);

            Safe_Release(pGameIns);
        }*/
    }
    if (false == m_bEnable)
    {
        CGameInstance* pGameInstance = CGameInstance::Get_Instance();
        Safe_AddRef(pGameInstance);


        if (true == m_bBroken)
        {
            if (ITEM_COLA == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"GlassBreak1.wav", SOUND_ITEM, 0.3f);


                m_bBroken = false;

            }

            else if (ITEM_BANANA == m_eItemType || ITEM_APPLE == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"GrossPropBreak2.wav", SOUND_ITEM, 0.3f);


                m_bBroken = false;

            }

            else if (ITEM_MEDIKIT == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"GrossPropBreak1.wav", SOUND_ITEM, 0.3f);


                m_bBroken = false;

            }
        }
        Safe_Release(pGameInstance);
    }



}

void CItem_Consumable::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);
}

HRESULT CItem_Consumable::Render()
{
    if (true == m_bPrompt)
    {
        if (nullptr == m_pVIBufferCom)
            return E_FAIL;


        // m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

        if (m_eItemType == ITEM_BANANA)
        {
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

            /* 장치에 텍스쳐를 바인딩한다. */
            if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)))
                return E_FAIL;

            if (FAILED(__super::SetUp_RenderState()))
                return E_FAIL;

            m_pVIBufferCom->Render();

            if (FAILED(__super::Release_RenderState()))
                return E_FAIL;
        }

       
        else if (m_eItemType == ITEM_APPLE)
        {
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

            /* 장치에 텍스쳐를 바인딩한다. */
            if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)))
                return E_FAIL;

            if (FAILED(SetUp_RenderState()))
                return E_FAIL;

            m_pVIBufferCom->Render();

            if (FAILED(Release_RenderState()))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_COLA)
        {
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

            /* 장치에 텍스쳐를 바인딩한다. */
            if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)))
                return E_FAIL;

            if (FAILED(SetUp_RenderState()))
                return E_FAIL;

            m_pVIBufferCom->Render();

            if (FAILED(Release_RenderState()))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_MEDIKIT)
        {
            m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

            /* 장치에 텍스쳐를 바인딩한다. */
            if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)))
                return E_FAIL;

            if (FAILED(SetUp_RenderState()))
                return E_FAIL;

            m_pVIBufferCom->Render();

            if (FAILED(Release_RenderState()))
                return E_FAIL;
        }

    }
    else
    {
         __super::Render();
    }

    return S_OK;
}

CItem_Consumable* CItem_Consumable::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItem_Consumable* pInstance = new CItem_Consumable(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CItem_Consumable");
    }

    return pInstance;
}

_bool CItem_Consumable::OnItemPicked(void* pArg)
{
    return _bool();
}

CGameObject* CItem_Consumable::Clone(void* pArg)
{
    CItem_Consumable* pInstance = new CItem_Consumable(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CItem_Consumable");
    }

    return pInstance;
}

void CItem_Consumable::Free()
{
    __super::Free();

    //Safe_Release(m_pTransformCom);
    //Safe_Release(m_pTextureCom);
    //Safe_Release(m_pRendererCom);
    //Safe_Release(m_pVIBufferCom);
}

HRESULT CItem_Consumable::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
   /* if (m_bPrompt)
    {
        if (m_eItemType == ITEM_BANANA)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Banana_Outline"),
                TEXT("Com_Texture_Banana_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_BANANA_PEEL)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BananaPeel_Outline"),
                TEXT("Com_Texture_BananaPeel_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_APPLE)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Apple_Outline"),
                TEXT("Com_Texture_Food_Apple_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_COLA)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Cola_Outline"),
                TEXT("Com_Texture_Food_Cola_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_MEDIKIT)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Medkit_#7192_Outline"),
                TEXT("Com_Texture_Medkit_#7192_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

    }
   
    else*/
    __super::Add_Components(strTextureTag, eMeshType);
    return S_OK;
}
