#include "stdafx.h"
#include "Text.h"

#include "GameInstance.h"


CText::CText(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CFont(pGraphic_Device)
{

}

CText::CText(const CText& rhs)
    :CFont(rhs)
{
    
}

HRESULT CText::Initialize_Prototype()
{
    if(FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CText::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    return S_OK;
}

void CText::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CText::Late_Tick(_float fTimeDelta)
{
    //__super::Late_Tick(fTimeDelta);
    if (m_bIsRender)
    {
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
    }
}

HRESULT CText::Render()
{
    __super::Render();
    return S_OK;
}

CText* CText::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CText* pInstance = new CText(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CText");
    }
    return pInstance;
}

CUI* CText::Clone(void* pArg)
{
    CText* pInstance = new CText(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CText");
    }
    return pInstance;
}

void CText::Free()
{
    __super::Free();
    //Safe_Release(m_pDevice);
    //Safe_Release(m_pSprite);
    
}
