#include "stdafx.h"
#include "ProgressableUI.h"
#include "VIBuffer_ProgressableRect.h"
#include "GameInstance.h"

CProgressableUI::CProgressableUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUI(pGraphic_Device)
{
}

CProgressableUI::CProgressableUI(const CProgressableUI& rhs)
    : CUI(rhs)
{
}

HRESULT CProgressableUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CProgressableUI::Initialize(void* pArg)
{
    UIDESC* pUIDesc = (UIDESC*)pArg;

    if (FAILED(__super::Initialize(pUIDesc)))
        return E_FAIL;

    m_pVIBufferProgressableRectCom = (CVIBuffer_ProgressableRect*)m_pVIBufferRectCom;

    m_pVIBufferProgressableRectCom->SetWorldMatrix(m_pTransformCom->Get_WorldMatrix());
    m_pVIBufferProgressableRectCom->SetViewMatrix(m_ViewMatrix);
    m_pVIBufferProgressableRectCom->SetProjectionMatrix(m_ProjMatrix);

    m_pVIBufferProgressableRectCom->Set_Texture(m_pTextureCom->Get_Texture(0));

    return S_OK;
}

void CProgressableUI::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

   /* if (CGameInstance::Get_Instance()->Key_Down('N'))
    {
        m_fRatio -= fTimeDelta;
    }
    else if (CGameInstance::Get_Instance()->Key_Down('M'))
    {
        m_fRatio += fTimeDelta;
    }*/
    if (m_bSetChanged)
    {
        m_fProgressAccTime += fTimeDelta;
        if (m_fProgressDuration < m_fProgressAccTime)
        {
            m_bSetChanged = false;
            m_fProgressAccTime = 0.0f;
        }
        if(m_fProgressAccTime>m_fProgressDuration*0.5f)
            Change_Ratio_while(fTimeDelta);
    }
}

void CProgressableUI::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);


}

HRESULT CProgressableUI::Render()
{
    if (nullptr == m_pVIBufferProgressableRectCom)
    {
        return E_FAIL;
    }

    m_pVIBufferProgressableRectCom->Set_Ratio(m_fRatio);

    //SetUp_RenderState();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 115);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    m_pVIBufferProgressableRectCom->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //Release_RenderState();

    return S_OK;
}

void CProgressableUI::Set_Changed(_float fDest)
{
    if(false==m_bSetChanged)
        m_bSetChanged = true;
    else {
        m_fProgressAccTime = m_fProgressDuration * 0.5f;
    }
    m_fDiff = fDest - m_fRatio;
}

void CProgressableUI::Change_Ratio_while(_float fTimeDelta)
{
    m_fRatio += m_fDiff * fTimeDelta;
}



CProgressableUI* CProgressableUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CProgressableUI* pInstance = new CProgressableUI(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created: CProgressableUI");
    }
    return pInstance;
}

CUI* CProgressableUI::Clone(void* pArg)
{
    CProgressableUI* pInstance = new CProgressableUI(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CProgressableUI");
    }

    return pInstance;
}

void CProgressableUI::Free()
{
    __super::Free();
}
