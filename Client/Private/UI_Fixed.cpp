#include "stdafx.h"
#include "..\Public\UI_Fixed.h"

#include "GameInstance.h"
CUI_Fixed::CUI_Fixed(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{
}

CUI_Fixed::CUI_Fixed(const CUI_Fixed& rhs)
    :CUI(rhs)
{
}

HRESULT CUI_Fixed::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Fixed::Initialize(void* pArg)
{
    UIDESC* pUIDesc = (UIDESC*)pArg;

    if (FAILED(__super::Initialize(pUIDesc)))
        return E_FAIL;

    return S_OK;
}

void CUI_Fixed::Tick(_float fTimeDelta)
{
    //UI 렌더 true, false 조건 테스트코드
   /* if (GetAsyncKeyState('1') & 0x8000)
    {
        if (m_bIsRender)
            m_bIsRender = false;
        else
            m_bIsRender = true;
    }*/
    __super::Tick(fTimeDelta);
}

void CUI_Fixed::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

}

HRESULT CUI_Fixed::Render()
{
    __super::Render();

    return S_OK;
}


CUI_Fixed* CUI_Fixed::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CUI_Fixed* pInstance = new CUI_Fixed(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created: CUI_Fixed");
    }
    return pInstance;
}

CUI* CUI_Fixed::Clone(void* pArg)
{
    CUI_Fixed* pInstance = new CUI_Fixed(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CUI_Fixed");
    }

    return pInstance;
}

void CUI_Fixed::Free()
{
    __super::Free();
}
