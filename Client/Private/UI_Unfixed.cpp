#include "stdafx.h"
#include "UI_Unfixed.h"

#include "GameInstance.h"

CUI_Unfixed::CUI_Unfixed(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{

}

CUI_Unfixed::CUI_Unfixed(const CUI_Unfixed& rhs)
    :CUI(rhs)
{
}

HRESULT CUI_Unfixed::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Unfixed::Initialize(void* pArg)
{
    UIUNFIXEDDESC* pUIUnfixedDesc = (UIUNFIXEDDESC*)pArg;

    CTransform::TRANSFORMDESC TransFormDesc;
    TransFormDesc.fRotationPerSec = pUIUnfixedDesc->fRotationPerSec;
    TransFormDesc.fSpeedPerSec = pUIUnfixedDesc->fSpeedPerSec;

    m_eDir = pUIUnfixedDesc->eDir;

    if (FAILED(__super::Initialize(&pUIUnfixedDesc->UIDesc)))
        return E_FAIL;

    if(FAILED(m_pTransformCom->Initialize(&TransFormDesc)))
        return E_FAIL;

    //m_fDuration = 0.0f;

    

    if (DIR_DOWN == m_eDir)
    {
        m_fTransHiddenX = m_fTransX;
        m_fTransHiddenY = m_fTransY + m_fSizeY + m_fPaddingY;
        
    }
    else if (DIR_LEFT == m_eDir)
    {
        m_fTransHiddenX = m_fTransX + m_fSizeX + m_fPaddingX;
        m_fTransHiddenY = m_fTransY;
    }
    else if (DIR_RIGHT == m_eDir)
    {
        m_fTransHiddenX = m_fTransX - m_fSizeX - m_fPaddingX;
        m_fTransHiddenY = m_fTransY;
    }
    else if (DIR_UP == m_eDir)
    {
        m_fTransHiddenX = m_fTransX;
        m_fTransHiddenY = m_fTransY + m_fSizeY + m_fPaddingY;
    }
    else if (DIR_END == m_eDir)
    {
        m_fTransHiddenX = 0.0f;
        m_fTransHiddenY = 0.0f;
    }
   /* else if (CENTER == m_eAnchor)
    {
        m_fTransHiddenX = 0.0f;
        m_fTransHiddenY = 0.0f;
    }*/

    m_fTransHiddenZ = m_fTransZ;
    
    _float3 fTranslation = { m_fTransHiddenX, m_fTransHiddenY, m_fTransHiddenZ };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, fTranslation);
    m_fAccTime = 0.0f;
    return S_OK;
}

void CUI_Unfixed::Tick(_float fTimeDelta)
{
    //if (true == m_bIsRender)
    //{
    //    switch(m_eDir)
    //    {//asdf
    //    case DIR_DOWN:
    //        if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > m_fTransY&& m_fAccTime < m_fDuration)
    //        {
    //            m_pTransformCom->Go_Down(fTimeDelta);
    //        }
    //        else if(m_fAccTime < m_fDuration) {
    //            m_fAccTime += fTimeDelta;
    //        }
    //        else if(m_fAccTime>m_fDuration)
    //        {
    //            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < m_fTransHiddenY)
    //            {
    //                m_pTransformCom->Go_Up(fTimeDelta);
    //            }
    //            else
    //            {
    //                Set_Render(false, -1);
    //                m_fAccTime = 0.0f;
    //            }
    //        }
    //        break;
    //    case DIR_LEFT:
    //        if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > m_fTransX && m_fAccTime < m_fDuration)
    //        {
    //            m_pTransformCom->Go_Left(fTimeDelta);
    //        }
    //        else if (m_fAccTime < m_fDuration) {
    //            m_fAccTime += fTimeDelta;
    //        }
    //        else if (m_fAccTime > m_fDuration)
    //        {
    //            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < m_fTransHiddenX)
    //            {
    //                m_pTransformCom->Go_Right(fTimeDelta);

    //            }
    //            else
    //            {
    //                m_bIsRender = false;
    //                m_fAccTime = 0.0f;
    //            }
    //        }
    //        break;
    //    case DIR_RIGHT:
    //        if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < m_fTransX && m_fAccTime < m_fDuration)
    //        {
    //            m_pTransformCom->Go_Right(fTimeDelta);
    //        }
    //        else if (m_fAccTime < m_fDuration) {
    //            m_fAccTime += fTimeDelta;
    //        }
    //        else if (m_fAccTime > m_fDuration)
    //        {
    //            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > m_fTransHiddenX)
    //            {
    //                m_pTransformCom->Go_Left(fTimeDelta);

    //            }
    //            else
    //            {
    //                m_bIsRender = false;
    //                m_fAccTime = 0.0f;
    //            }
    //        }
    //        break;
    //    }
    //}

    m_pTransformCom->SetNextPos(_float3(0.f, 0.f, 0.f));

    if (true == m_bIsRender)
    {
        switch (m_eDir)
        {//asdf
        case DIR_DOWN:
            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > m_fTransY && m_fAccTime < m_fDuration)
            {
                m_pTransformCom->Go_Down(fTimeDelta);
                _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
            }
            else if (m_fAccTime < m_fDuration) {
                m_fAccTime += fTimeDelta;
            }
            else if (m_fAccTime > m_fDuration)
            {
                if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < m_fTransHiddenY)
                {
                    m_pTransformCom->Go_Up(fTimeDelta);
                    _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
                }
                else
                {
                    Set_Render(false, -1);
                    m_fAccTime = 0.0f;
                }
            }
            break;
        case DIR_LEFT:
            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > m_fTransX && m_fAccTime < m_fDuration)
            {
                m_pTransformCom->Go_Left(fTimeDelta);
                _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
            }
            else if (m_fAccTime < m_fDuration) {
                m_fAccTime += fTimeDelta;
            }
            else if (m_fAccTime > m_fDuration)
            {
                if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < m_fTransHiddenX)
                {
                    m_pTransformCom->Go_Right(fTimeDelta);
                    _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());

                }
                else
                {
                    m_bIsRender = false;
                    m_fAccTime = 0.0f;
                }
            }
            break;
        case DIR_RIGHT:
            if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < m_fTransX && m_fAccTime < m_fDuration)
            {
                m_pTransformCom->Go_Right(fTimeDelta);
                _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
            }
            else if (m_fAccTime < m_fDuration) {
                m_fAccTime += fTimeDelta;
            }
            else if (m_fAccTime > m_fDuration)
            {
                if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > m_fTransHiddenX)
                {
                    m_pTransformCom->Go_Left(fTimeDelta);
                    _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());

                }
                else
                {
                    m_bIsRender = false;
                    m_fAccTime = 0.0f;
                }
            }
            break;
        }
    }
   /* _float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());*/
    __super::Tick(fTimeDelta);
}

void CUI_Unfixed::Late_Tick(_float fTimeDelta)
{
  /*  if (GetAsyncKeyState('C') & 0x8000)
    {
        Set_Render(true, -1);
    }*/
    
    __super::Late_Tick(fTimeDelta);
    
}

HRESULT CUI_Unfixed::Render()
{
    __super::Render();
    return S_OK;
}

void CUI_Unfixed::SetActive_Unfixed(_float fActiveTime, _int iTexIndex)
{
    if (0.0f > fActiveTime||-1>iTexIndex)
        return;
    //m_fDuration = fActiveTime;
    m_fDuration = fActiveTime;
    if (-1 != iTexIndex && false==m_bIsAnimation)
    {
        Set_TexIndex(iTexIndex);
    }
    Set_Render(TRUE, -1);
}

CUI_Unfixed* CUI_Unfixed::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_Unfixed* pInstance = new CUI_Unfixed(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CUI_Unfixed");
    }

    return pInstance;
}

CUI* CUI_Unfixed::Clone(void* pArg)
{
    CUI_Unfixed* pInstance = new CUI_Unfixed(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned: CUI_Unfixed");
    }
    return pInstance;
}

void CUI_Unfixed::Free()
{
    __super::Free();
}
