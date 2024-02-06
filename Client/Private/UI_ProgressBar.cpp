#include "stdafx.h"
#include "UI_ProgressBar.h"

#include "GameInstance.h"

CUI_ProgressBar::CUI_ProgressBar(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{
}

CUI_ProgressBar::CUI_ProgressBar(const CUI_ProgressBar& rhs)
    :CUI(rhs)
{
}

HRESULT CUI_ProgressBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_ProgressBar::Initialize(void* pArg)
{
    UIPROGRESSBARDESC* pUIProgressBarDesc = (UIPROGRESSBARDESC*)pArg;

    m_eBar = pUIProgressBarDesc->eBar;
    m_fTotal = pUIProgressBarDesc->fTotal;
    m_fCurrent = m_fTotal;
    m_fDecrease = pUIProgressBarDesc->fDecrease;
    m_fIncrease = pUIProgressBarDesc->fIncrease;

    if (FAILED(__super::Initialize(&pUIProgressBarDesc->UIDesc)))
        return E_FAIL;
    return S_OK;
}

void CUI_ProgressBar::Tick(_float fTimeDelta)
{

    __super::Tick(fTimeDelta);
}

void CUI_ProgressBar::Late_Tick(_float fTimeDelta)
{
    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        m_fCurrent -= m_fDecrease * fTimeDelta;
        if (0.0f > m_fCurrent)
        {
            m_fCurrent = 0.0f;
        }
    }

    else
    {
        if (m_fTotal > m_fCurrent)
        {
            m_fCurrent += m_fIncrease * fTimeDelta;
            if (m_fTotal < m_fCurrent)
            {
                m_fCurrent = m_fTotal;
            }
        }
    }
    __super::Late_Tick(fTimeDelta);
}

HRESULT CUI_ProgressBar::Render()
{
    __super::Render();
    return S_OK;
}

CUI_ProgressBar* CUI_ProgressBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_ProgressBar* pInstance = new CUI_ProgressBar(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CUI_ProgressBar");   
    }
    return pInstance;
}

CGameObject* CUI_ProgressBar::Clone(void* pArg)
{
    CUI_ProgressBar* pInstance = new CUI_ProgressBar(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        if (BAR_PLAYER == m_eBar)
        {
            MSG_BOX("Failed to Cloned : CUI_ProgressBar_Player");
        }
        else if (BAR_ENEMY == m_eBar)
        {
            MSG_BOX("Failed to Cloned : CUI_ProgressBar_Enemy");
        }
        else if (BAR_LOADING == m_eBar)
        {
            MSG_BOX("Failed to Cloned : CUI_ProgressBar_Loading");
        }
    }
    return pInstance;
}

void CUI_ProgressBar::Free()
{

}
