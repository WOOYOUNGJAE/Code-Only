#include "stdafx.h"
#include "Item_ThrowOnly.h"
#include "ItemDatabase.h"
#include "GameInstance.h"

CItem_ThrowOnly::CItem_ThrowOnly(LPDIRECT3DDEVICE9 pGrahic_Device)
    :CInteractiveObject(pGrahic_Device)
{
}

CItem_ThrowOnly::CItem_ThrowOnly(const CInteractiveObject& rhs)
    : CInteractiveObject(rhs)
{
}

HRESULT CItem_ThrowOnly::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_ThrowOnly::Initialize(void* pArg)
{

    OBJECTDESC* itemdesc = (OBJECTDESC*)pArg;

    HRESULT hr = __super::Initialize(pArg);
    if (FAILED(hr))
        return hr;

    //타입을 받아옴
    m_eItemType = itemdesc->eItemType;

    if (FAILED(Add_Components(itemdesc->strTextureTag, itemdesc->eMeshType)))
        return E_FAIL;

    if (m_eItemType == ITEM_SCRUNCHED_PAPER)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_ScrunchedPaper_0_Outline"),
            TEXT("Com_Texture_ScrunchedPaper_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TOOL_BOX)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Toolbox_Outline"),
            TEXT("Com_Texture_Toolbox_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TRASH_BIN)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashbin_0_Outline"),
            TEXT("Com_Texture_trashbin_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TRASH_CAN)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashcan_0_Outline"),
            TEXT("Com_Texture_trashcan_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_WOODEN_BARREL)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodenBarrel_Outline"),
            TEXT("Com_Texture_WoodenBarrel_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_1)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_0_Outline"),
            TEXT("Com_Texture_PoolBall_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_3)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_2_Outline"),
            TEXT("Com_Texture_PoolBall_2_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_5)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_4_Outline"),
            TEXT("Com_Texture_PoolBall_4_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_6)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_5_Outline"),
            TEXT("Com_Texture_PoolBall_5_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_7)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_6_Outline"),
            TEXT("Com_Texture_PoolBall_6_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_8)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_7_Outline"),
            TEXT("Com_Texture_PoolBall_7_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_9)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_8_Outline"),
            TEXT("Com_Texture_PoolBall_8_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_10)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_9_Outline"),
            TEXT("Com_Texture_PoolBall_9_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_11)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_10_Outline"),
            TEXT("Com_Texture_PoolBall_10_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_14)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_13_Outline"),
            TEXT("Com_Texture_PoolBall_13_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_POOL_BALL_15)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_14_Outline"),
            TEXT("Com_Texture_PoolBall_14_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_OIL_BARREL)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_OilBarrel_Outline"),
            TEXT("Com_Texture_OilBarrel_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    else if (m_eItemType == ITEM_PAINT_CAN)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PaintCans_Outline"),
            TEXT("Com_Texture_PaintCans_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    else if (m_eItemType == ITEM_TABLE_PLANT)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TablePlant2_Outline"),
            TEXT("Com_Texture_TablePlant2_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TUMBLER_GLASS)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TumblerGlass_Outline"),
            TEXT("Com_Texture_TumblerGlass_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    else if (m_eItemType == ITEM_POT_PLANT)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Potplant_Outline"),
            TEXT("Com_Texture_Potplant_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    else if (m_eItemType == ITEM_UMBRELLA_STAND)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UmbrellaStand_Outline"),
            TEXT("Com_Texture_UmbrellaStand_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_COAT_HANGER)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_CoatHanger_Outline"),
            TEXT("Com_Texture_CoatHanger_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_BAR_STOOL)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BarStool_Outline"),
            TEXT("Com_Texture_BarStool_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TABLE_LAMP)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TableLamp_Outline"),
            TEXT("Com_Texture_TableLamp_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_WOODEN_STOOL)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodenStool_Outline"),
            TEXT("Com_Texture_WoodenStool_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_HAND_BAG)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_HandBag_Outline"),
            TEXT("Com_Texture_HandBag_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    else if (m_eItemType == ITEM_ASHTRAY_BIN)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AshtrayBinFallen_Outline"),
            TEXT("Com_Texture_AshtrayBinFallen_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    ThrowOnly_Info& temp = CItemDatabase::Get_ThrowOnly_Info(itemdesc->eItemType);

    m_bAlive = temp.bAlive;
    m_iThrowingDamage = temp.iThrowingDamage;

    return S_OK;
}

void CItem_ThrowOnly::Tick(_float fTimeDelta)
{
    CGameInstance* pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(pGameInstance);

    if (true == m_bEnable)
    {
        __super::Tick(fTimeDelta);
    }

    if (false == m_bEnable)
    {
        CGameInstance* pGameInstance = CGameInstance::Get_Instance();
        Safe_AddRef(pGameInstance);


        if (true == m_bBroken)
        {
              if (m_eItemType == ITEM_TABLE_PLANT || m_eItemType == ITEM_POT_PLANT || m_eItemType == ITEM_TUMBLER_GLASS)
              {
                  CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
                  CGameInstance::Get_Instance()->Play_Sound(L"GlassBreak3.wav", SOUND_ITEM, m_fSoundVolume);
                  m_bBroken = false;
              }
              else if (m_eItemType ==ITEM_SCRUNCHED_PAPER)
              {
                  CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
                  CGameInstance::Get_Instance()->Play_Sound(L"ScrunchedPaperPickup.wav", SOUND_ITEM, m_fSoundVolume);
                  m_bBroken = false;
              }

              else if (m_eItemType == ITEM_WOODEN_BARREL)
              {
                  CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
                  CGameInstance::Get_Instance()->Play_Sound(L"WoodHitGeneric2.wav", SOUND_ITEM, m_fSoundVolume);
                  m_bBroken = false;
              }

              else if (m_eItemType == ITEM_NONE)
              {
                  CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
                  CGameInstance::Get_Instance()->Play_Sound(L"GrossPropBreak1.wav", SOUND_ITEM, m_fSoundVolume);
                  m_bBroken = false;
              }
              
              else
              {
                  m_eItemType;
                  CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
                  CGameInstance::Get_Instance()->Play_Sound(L"GenericSmash2.wav", SOUND_ITEM, m_fSoundVolume);
                  m_bBroken = false;
              }
            
              
   
        }

        if (true == m_bItemCollideSound)
        {
            if (ITEM_WOODEN_BARREL == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"WoodSmash1.wav", SOUND_ITEM, 0.3f);
                m_bItemCollideSound = false;
            }
        }

        Safe_Release(pGameInstance);

    }
    Safe_Release(pGameInstance);
}

void CItem_ThrowOnly::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);
}

HRESULT CItem_ThrowOnly::Render()
{
    if (true == m_bPrompt)
    {
        if (nullptr == m_pVIBufferCom)
            return E_FAIL;


        // m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

        if (m_eItemType == ITEM_SCRUNCHED_PAPER)
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

        else if (m_eItemType == ITEM_TOOL_BOX)
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

        else if (m_eItemType == ITEM_TRASH_BIN)
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

        else if (m_eItemType == ITEM_TRASH_CAN)
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

        else if (m_eItemType == ITEM_WOODEN_BARREL)
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

        else if (m_eItemType == ITEM_POOL_BALL_1)
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

        else if (m_eItemType == ITEM_POOL_BALL_3)
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
        else if (m_eItemType == ITEM_POOL_BALL_5)
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
        else if (m_eItemType == ITEM_POOL_BALL_6)
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
        else if (m_eItemType == ITEM_POOL_BALL_7)
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
        else if (m_eItemType == ITEM_POOL_BALL_8)
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
        else if (m_eItemType == ITEM_POOL_BALL_9)
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
        else if (m_eItemType == ITEM_POOL_BALL_10)
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
        else if (m_eItemType == ITEM_POOL_BALL_11)
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
        else if (m_eItemType == ITEM_POOL_BALL_14)
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
        else if (m_eItemType == ITEM_POOL_BALL_15)
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
        else if (m_eItemType == ITEM_OIL_BARREL)
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
        else if (m_eItemType == ITEM_PAINT_CAN)
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
        else if (m_eItemType == ITEM_TABLE_PLANT)
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
        else if (m_eItemType == ITEM_TUMBLER_GLASS)
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
        else if (m_eItemType == ITEM_POT_PLANT)
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
        else if (m_eItemType == ITEM_UMBRELLA_STAND)
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
        else if (m_eItemType == ITEM_COAT_HANGER)
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
        else if (m_eItemType == ITEM_BAR_STOOL)
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
        else if (m_eItemType == ITEM_TABLE_LAMP)
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
        else if (m_eItemType == ITEM_WOODEN_STOOL)
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
        else if (m_eItemType == ITEM_HAND_BAG)
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
        else if (m_eItemType == ITEM_ASHTRAY_BIN)
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
        else
        {
            __super::Render();
        }

    }
    else
    {
        __super::Render();
    }

    return S_OK;
}

HRESULT CItem_ThrowOnly::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
    ///* For.Com_Renderer */
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
    //    TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
    //    return E_FAIL;

    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Billboard"),
    //    TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
    //    return E_FAIL;

    ///* For.Com_Texture */
    //if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_" + strTextureTag),
    //    TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
    //    return E_FAIL;

    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
    //    TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
    //    return E_FAIL;

    ///* For.Com_Transform */
    ///* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
    //CTransform::TRANSFORMDESC		TransformDesc;
    //TransformDesc.fSpeedPerSec = 10.f;
    //TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
    //    TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
    //    return E_FAIL;

    ///* For.Com_Physics */
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
    //    TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
    //    return E_FAIL;

    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
    //    TEXT("Com_CubeVIBuffer"), (CComponent**)&m_pCubeVIBufferCom)))
    //    return E_FAIL;


    //if (m_bPrompt)
    //{
    //    if (m_eItemType == ITEM_SCRUNCHED_PAPER)
    //    {
    //        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_ScrunchedPaper_0_Outline"),
    //            TEXT("Com_Texture_ScrunchedPaper_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
    //            return E_FAIL;
    //    }

    //    else if (m_eItemType == ITEM_TOOL_BOX)
    //    {
    //        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Toolbox_Outline"),
    //            TEXT("Com_Texture_Toolbox_Outline"), (CComponent**)&m_pTextureCom_Outline)))
    //            return E_FAIL;
    //    }

    //    else if (m_eItemType == ITEM_TRASH_BIN)
    //    {
    //        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashbin_0_Outline"),
    //            TEXT("Com_Texture_trashbin_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
    //            return E_FAIL;
    //    }

    //    else if (m_eItemType == ITEM_TRASH_CAN)
    //    {
    //        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashcan_0_Outline"),
    //            TEXT("Com_Texture_trashcan_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
    //            return E_FAIL;
    //    }

    //    else if (m_eItemType == ITEM_WOODEN_BARREL)
    //    {
    //        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodenBarrel_Outline"),
    //            TEXT("Com_Texture_WoodenBarrel_Outline"), (CComponent**)&m_pTextureCom_Outline)))
    //            return E_FAIL;
    //    }
    //}

    //else
    __super::Add_Components(strTextureTag, eMeshType);
    return S_OK;
}

CItem_ThrowOnly* CItem_ThrowOnly::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItem_ThrowOnly* pInstance = new CItem_ThrowOnly(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CItem_ThrowOnly");
    }

    return pInstance;
}

CGameObject* CItem_ThrowOnly::Clone(void* pArg)
{
    CItem_ThrowOnly* pInstance = new CItem_ThrowOnly(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CItem_ThrowOnly");
    }

    return pInstance;
}

void CItem_ThrowOnly::Free()
{
    __super::Free();

    //Safe_Release(m_pTransformCom);
    //Safe_Release(m_pTextureCom);
    //Safe_Release(m_pRendererCom);
    //Safe_Release(m_pVIBufferCom);
}

_bool CItem_ThrowOnly::OnItemPicked(void* pArg)
{
    return _bool();
}
