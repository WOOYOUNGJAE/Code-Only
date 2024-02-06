#include "stdafx.h"
#include "Item_Equipment.h"
#include "GameInstance.h"
#include "ItemDatabase.h"

CItem_Equipment::CItem_Equipment(LPDIRECT3DDEVICE9 pGrahic_Device)
    :CInteractiveObject(pGrahic_Device)
{
}

CItem_Equipment::CItem_Equipment(const CInteractiveObject& rhs)
    : CInteractiveObject(rhs)
{
}

HRESULT CItem_Equipment::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_Equipment::Initialize(void* pArg)
{
    OBJECTDESC* itemdesc = (OBJECTDESC*)pArg;

   
   // m_bPrompt = true;

   
    HRESULT hr = __super::Initialize(pArg);
    if (FAILED(hr))
        return hr;

    //타입을 받아옴
    m_eItemType = itemdesc->eItemType;

    if (m_eItemType == ITEM_BOTTLE)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bottle_0_Outline"),
            TEXT("Com_Texture_Bottle_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_AUTOMATIC_PISTOL)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AutoPistol_Outline"),
            TEXT("Com_Texture_AutoPistol_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_PIPE)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_wallholepipe_pipe_H_Outline"),
            TEXT("Com_Texture_wallholepipe_pipe_H_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }

    else if (m_eItemType == ITEM_TOMMY_GUN)
    {
        if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TommyGun_Outline"),
            TEXT("Com_Texture_TommyGun_Outline"), (CComponent**)&m_pTextureCom_Outline)))
            return E_FAIL;
    }
    Equipment_Info& temp = CItemDatabase::Get_Equipment_Info(itemdesc->eItemType);



  //  m_bAlive = temp.bAllive;
    m_iAttack = temp.iAttack;
    m_iMaxHP = temp.fDurability;
    m_iHP = m_iMaxHP;
    m_iThrowingDamage =temp.iThrowingDamage;
    m_iBulletCount = temp.iBulletCount;

    CGameInstance* pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(pGameInstance);

    ////장비 내구도 줄어듦 동준테스트
  /*  pGameInstance->Subscribe(EVENT_PLAYER_ATTACK,
        std::bind(&CItem_Equipment::OnUsedEquipment, this, std::placeholders::_1));*/

   
    Safe_Release(pGameInstance);
    
    return S_OK;
}

void CItem_Equipment::Tick(_float fTimeDelta)
{
    if (m_bEnable)
    {
        __super::Tick(fTimeDelta);
    }

    if (false == m_bEnable)
    {
        CGameInstance* pGameInstance = CGameInstance::Get_Instance();
        Safe_AddRef(pGameInstance);


        if (true == m_bBroken)
        {
            if (ITEM_BOTTLE == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"GlassBreak1.wav", SOUND_ITEM, 0.3f);


                m_bBroken = false;

            }

            else if (ITEM_AUTOMATIC_PISTOL == m_eItemType || ITEM_TOMMY_GUN == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"MetalPropBounce.wav", SOUND_ITEM, 0.3f);
               

                m_bBroken = false;

            }
            else if (ITEM_PIPE == m_eItemType || ITEM_FRYING_PAN == m_eItemType)
            {
                pGameInstance->StopSound(SOUND_ITEM);
                pGameInstance->Play_Sound(L"PipeBreak.wav", SOUND_ITEM, 0.3f);
                

                    m_bBroken = false;

            }
        }
        Safe_Release(pGameInstance);
    }
}

void CItem_Equipment::Late_Tick(_float fTimeDelta)
{
    if (m_bEnable)
    {
        __super::Late_Tick(fTimeDelta);

       
        //if (true == m_bPrompt)
        //{
        //    CGameInstance* pGameIns = CGameInstance::Get_Instance();
        //    Safe_AddRef(pGameIns);
        //    PROMPTER Prompter;
        //    switch (m_eItemType)
        //    {
        //    case(ITEM_BOTTLE):
        //        Prompter = PROMPTER_BOTTLE;
        //        // pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;
        //    case(ITEM_AUTOMATIC_PISTOL):
        //        Prompter = PROMPTER_AUTOMATIC_PISTOL;
        //        // pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;
        //    case(ITEM_TOMMY_GUN):
        //        Prompter = PROMPTER_TOMMY_GUN;
        //        // pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;
        //    case(ITEM_PIPE):
        //        Prompter = PROMPTER_PIPE_WIELDING_MOOK_UNCONSCIOUS;
        //        // pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;
        //    case(ITEM_FRYING_PAN):
        //        Prompter = PROMPTER_FRYINGPAN;
        //        //pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;
        //    case(ITEM_BOTTLE_BROKEN):
        //        Prompter = PROMPTER_BOTTLE;
        //        //pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //        break;

        //    }
        //    pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
        //    Safe_Release(pGameIns);




        //}
     /*   else if (m_bPrompt == false)
        {
            CGameInstance* pGameIns = CGameInstance::Get_Instance();
            Safe_AddRef(pGameIns);

            PROMPTER Prompter = PROMPTER_END;

            pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);

            Safe_Release(pGameIns);
        }*/
      
    }
}

HRESULT CItem_Equipment::Render()
{
    if (m_bEnable)
    {
        
        if (true == m_bPrompt)
        {
          if (nullptr == m_pVIBufferCom)
                return E_FAIL;


         // m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

            if (m_eItemType == ITEM_BOTTLE)
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

            else if (m_eItemType == ITEM_AUTOMATIC_PISTOL)
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

            else if (m_eItemType == ITEM_PIPE)
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
          
            else if (m_eItemType == ITEM_TOMMY_GUN)
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
            return __super::Render();
        }
    }
    
    return S_OK;
}

CItem_Equipment* CItem_Equipment::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItem_Equipment* pInstance = new CItem_Equipment(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CItem_Equipment");
    }

    return pInstance;
}

CGameObject* CItem_Equipment::Clone(void* pArg)
{
    CItem_Equipment* pInstance = new CItem_Equipment(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CItem_Equipment");
    }

    return pInstance;
}

void CItem_Equipment::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pVIBufferCom);
}

_bool CItem_Equipment::OnItemPicked(void* pArg)
{
    //_float4x4* pOurWorldMatrix = (_float4x4*)pArg;
    _float3 vItemRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
    _float3 vItemUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
    _float3 vItemLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _float3 vItemPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    //아이템의 월드매트릭스
    _float4x4 ItemWorldMatrix = m_pTransformCom->Get_WorldMatrix();

    // 카메라의 위치와 crosshair의 방향을 이용해 레이를 구성
    _float3 rayPos = { 0.f, 0.f, 0.f };
    _float3 rayDir = { 0.f, 0.f, 1.f };
    _float3 outPoint;

    _float4x4 CameraWorldMatrix;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
    D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

    rayPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };
    rayDir = { CameraWorldMatrix._31, CameraWorldMatrix._32, CameraWorldMatrix._33 };

    if (m_pVIBufferCom->Intersect_Polygons_ForRay(ItemWorldMatrix, rayDir, rayPos, &outPoint))
    {
        _float3 vDistVector = outPoint - rayPos;
        _float fDist = D3DXVec3Length(&vDistVector);

        if (fDist < 100.f)
            m_bPicked = true;
            
        else
            m_bPicked = false;
    }

    return true;
}

_bool CItem_Equipment::OnUsedEquipment(void* pArg)
{
    
    if (true == m_bPicked && 0 < m_iHP) //동준테스트
        m_iHP -= 1;
    
    if (0 == m_iHP)
        m_bEnable = false;

    return true;
}
HRESULT CItem_Equipment::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{

    /* if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AutoPistol_Outline"),
         TEXT("Com_Texture_AutoPistol_Outline"), (CComponent**)&m_pTextureCom_Outline)))
         return E_FAIL;


     if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bottle_0_Outline"),
         TEXT("Com_Texture_Bottle_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
         return E_FAIL;
    */
    /*if (m_bPrompt)
    {
        if (m_eItemType == ITEM_BOTTLE)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bottle_0_Outline"),
                TEXT("Com_Texture_Bottle_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_AUTOMATIC_PISTOL)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AutoPistol_Outline"),
                TEXT("Com_Texture_AutoPistol_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }

        else if (m_eItemType == ITEM_PIPE)
        {
            if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_wallholepipe_pipe_H_Outline"),
                TEXT("Com_Texture_wallholepipe_pipe_H_Outline"), (CComponent**)&m_pTextureCom_Outline)))
                return E_FAIL;
        }
    }
   
    else */
    __super::Add_Components(strTextureTag, eMeshType);
    return S_OK;
}
//
//HRESULT CItem_Equipment::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
//{
//   
//    return S_OK;
//}
//
//HRESULT CItem_Equipment::SetUp_RenderState()
//{
//    return S_OK;
//}
//
//HRESULT CItem_Equipment::Release_RenderState()
//{
//    return S_OK;
//}
