#include "Font.h"
#include "Component_Manager.h"


CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUI(pGraphic_Device)
{
    //m_pDevice = pGraphic_Device;
    //Safe_AddRef(m_pDevice);
}

CFont::CFont(const CFont& rhs)
    :CUI(rhs)
{
   // m_pDevice = rhs.m_pDevice;
    m_pSprite = rhs.m_pSprite;

    //Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pSprite);
}

HRESULT CFont::Initialize_Prototype()
{
    if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
    {
        MSG_BOX("D3DXCreateSprite Failed");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CFont::Initialize(void* pArg)
{
    TEXTDESC* pTextDesc = (TEXTDESC*)pArg;
    D3DXFONT_DESCW tFontInfo;
    ZeroMemory(&tFontInfo, sizeof(tFontInfo));

    tFontInfo.Height = pTextDesc->Height;
    tFontInfo.Width = pTextDesc->Width;
    tFontInfo.Weight = pTextDesc->Weight;
    tFontInfo.CharSet = pTextDesc->CharSet;
    lstrcpy(tFontInfo.FaceName, pTextDesc->FaceName);

    m_iWinSizeX = pTextDesc->UIDesc.iWindowSizeX;
    m_iWinSizeY = pTextDesc->UIDesc.iWindowSizeY;
    m_strText = pTextDesc->strText;
    m_TextColor = pTextDesc->TextColor;
    if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pFont)))
    {
        MSG_BOX("Font Create Failed");
        return E_FAIL;
    }


    if (FAILED(__super::Initialize(&pTextDesc->UIDesc)))
    {
        MSG_BOX("UI_Text Parent Initialize Failed");
        return E_FAIL;
    }
    _float4x4 WorldMatrix;// , ViewMatrix;
    D3DXMatrixTranslation(&WorldMatrix, /*m_fTransX +*/ m_iWinSizeX * 0.5f/*-m_fSizeX*0.5f*/,-m_fTransY /*+ m_fSizeY * 0.5f*/ + m_iWinSizeY * 0.5f, 0.0f);
    //D3DXMatrixIdentity(&ViewMatrix);
    m_pSprite->SetTransform(&WorldMatrix);
    
    return S_OK;
}

void CFont::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CFont::Late_Tick(_float fTimeDelta)
{
    //__super::Late_Tick(fTimeDelta);
    if (m_bIsRender)
    {
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
    }
}

HRESULT CFont::Render()
{
    
    __super::Render();

    RECT UIRect;
    SetRect(&UIRect, _long(-m_fSizeX * 0.5f), _long(-m_fSizeY * 0.5f), _long(m_fSizeX * 0.5f), _long(m_fSizeY * 0.5f));
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
    _int iSizeY = m_pFont->DrawTextW(m_pSprite, m_strText.c_str(), (_int)m_strText.length(), &UIRect, DT_CALCRECT, m_TextColor);
    SetRect(&UIRect, _long(-m_fSizeX * 0.5f) , _long(-iSizeY /2), _long(m_fSizeX * 0.5f), _long(iSizeY / 2));
    m_pFont->DrawTextW(m_pSprite, m_strText.c_str(), (_int)m_strText.length(), &UIRect, DT_CENTER, m_TextColor);
    m_pSprite->End();
    return S_OK;
}

void CFont::Free()
{
    Safe_Release(m_pFont);
    Safe_Release(m_pSprite);
    /*Safe_Release(m_pDevice);*/
    __super::Free();
}
