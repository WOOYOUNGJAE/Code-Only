#include "..\Public\UI.h"

#include "GameInstance.h"
CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CUI::CUI(const CUI& rhs)
    :CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
    UIDESC* pUIDesc = (UIDESC*)pArg;

    CComponent_Manager* pComponent_Manager = CComponent_Manager::Get_Instance();
    Safe_AddRef(pComponent_Manager);

    m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(pUIDesc->iLevelStatic, pUIDesc->strRendererTag, nullptr);
    if (nullptr == m_pRendererCom)
        return E_FAIL;
    m_pVIBufferRectCom = (CVIBuffer_Rect*)pComponent_Manager->Clone_Component(pUIDesc->iLevelStatic, pUIDesc->strVIBuffer_RectTag, nullptr);
    //Safe_AddRef(m_pVIBufferRectCom);
    if (nullptr == m_pVIBufferRectCom)
        return E_FAIL;
    m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(pUIDesc->iLevelIndex, pUIDesc->strTextureTag, nullptr);
    //Safe_AddRef(m_pTextureCom);
    if (nullptr == m_pTextureCom)
        return E_FAIL;
    m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(pUIDesc->iLevelStatic, pUIDesc->strTransformTag, nullptr);
    //Safe_AddRef(m_pTransformCom);
    if (nullptr == m_pTransformCom)
        return E_FAIL;

    Safe_Release(pComponent_Manager);

    m_fSizeX = pUIDesc->fSizeX;
    m_fSizeY = pUIDesc->fSizeY;
    //각 위치
    m_fPaddingX = pUIDesc->fPaddingX;
    m_fPaddingY = pUIDesc->fPaddingY;

    m_bIsRender = pUIDesc->bIsRender;

    _uint iWindowSizeX = pUIDesc->iWindowSizeX;
    _uint iWindowSizeY = pUIDesc->iWindowSizeY;
   
    m_eAnchor = pUIDesc->eAnchor;

    m_eRender = pUIDesc->eRender;

    m_fAccTime = 0.0f;
    m_fDuration = -1.0f;

    m_bIsSetTimer = false;

    //애니메이션 설정
    m_fTexIndex = 0.0f;
    m_bIsAnimation = pUIDesc->bIsAnimation;
    m_iTexCnt = m_pTextureCom->Get_TextureCount();
    m_fAnimationRatio = pUIDesc->fAnimationRatio;
    //

    //고정된 UI의 경우 처음 넘겨준 Padding, Anchor를 기준으로 transform.

    switch (pUIDesc->eAnchor)
    {
    case LEFT:
        m_fTransX = iWindowSizeX * -0.5f + m_fSizeX * 0.5f + m_fPaddingX;
        m_fTransY = -m_fPaddingY;
        break;
    case LEFTUP:
        m_fTransX = iWindowSizeX * -0.5f + m_fSizeX * 0.5f + m_fPaddingX;
        m_fTransY = +iWindowSizeY * 0.5f - m_fSizeY * 0.5f - m_fPaddingY;

        break;
    case UP:
        m_fTransX = m_fPaddingX;
        m_fTransY = +iWindowSizeY * 0.5f - m_fSizeY * 0.5f - m_fPaddingY;
        break;
    case RIGHTUP:
        m_fTransX = +iWindowSizeX * 0.5f - m_fSizeX * 0.5f - m_fPaddingX;
        m_fTransY = +iWindowSizeY * 0.5f - m_fSizeY * 0.5f - m_fPaddingY;

        break;
    case RIGHT:
        m_fTransX = +iWindowSizeX * 0.5f - m_fSizeX * 0.5f - m_fPaddingX;
        m_fTransY = -m_fPaddingY;
        break;
    case RIGHTDOWN:
        m_fTransX = +iWindowSizeX * 0.5f - m_fSizeX * 0.5f - m_fPaddingX;
        m_fTransY = iWindowSizeY * -0.5f + m_fSizeY * 0.5f + m_fPaddingY;
        break;
    case DOWN:
        m_fTransX = m_fPaddingX;
        m_fTransY = iWindowSizeY * -0.5f + m_fSizeY * 0.5f + m_fPaddingY;
        break;
    case LEFTDOWN:
        m_fTransX = iWindowSizeX * -0.5f + m_fSizeX * 0.5f + m_fPaddingX;
        m_fTransY = iWindowSizeY * -0.5f + m_fSizeY * 0.5f + m_fPaddingY;
        break;
    case CENTER:
        m_fTransX = m_fPaddingX;
        m_fTransY = m_fPaddingY;
        break;
    }
    m_fTransZ =  pUIDesc->fPosZ;


    m_pTransformCom->Scaling(_float3(m_fSizeX, m_fSizeY, 1.0f));
    //고정된 UI이면 부모의 Initiailize에서 Translation, Scale 세팅.
    if (true == pUIDesc->bIsFixed)
    {
        _float3 fTranslation = { m_fTransX, m_fTransY, m_fTransZ };
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, fTranslation);
    }
    m_ViewMatrix = *D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)iWindowSizeX, (_float)iWindowSizeY, 0.0f, 1.0f);


    return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
    //애니메이션 켜져있는 경우
    if (true == m_bIsAnimation)
    {

        m_fTexIndex += (_float)m_iTexCnt * fTimeDelta*m_fAnimationRatio;

        if (m_fTexIndex >= (_float)m_iTexCnt)
            m_fTexIndex = 0.0f;
    }
    //몇초동안 보이거나, 안보이는 세팅인 경우
    if (true == m_bIsSetTimer)
    {
        m_fAccTime += fTimeDelta;
        if (m_fDuration < m_fAccTime)
        {
            m_bIsRender = !m_bIsRender;
            m_fAccTime = 0.f;
            m_fDuration = 0.f;
            m_bIsSetTimer = false;
            m_fTexIndex = 0.0f;
            if(m_bIsAnimation==true)
                m_bIsAnimation = !m_bIsAnimation;
        }
        
    }
    __super::Tick(fTimeDelta);
}

void CUI::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    if (m_bIsRender)
    {
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
    }
    
}

HRESULT CUI::Render()
{
    if (nullptr == m_pVIBufferRectCom)
        return E_FAIL;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_PreViewMatirx);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_PreProjMatrix);
    

    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

    if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fTexIndex)))
        return E_FAIL;

    //UI화질 개선을 위한 설정
    //m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16); D3DTEXF_ANISOTROPIC
    
    if(FAILED(SetUp_RenderState()))
        return E_FAIL;
    m_pVIBufferRectCom->Render();
    if (FAILED(Release_RenderState()))
        return E_FAIL;
 
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_PreViewMatirx);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_PreProjMatrix);

    return S_OK;
}

HRESULT CUI::SetUp_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;
    if (RENDER_TEST == m_eRender)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF,115);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    }
    else if (RENDER_BLEND == m_eRender)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    /*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);*/
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    return S_OK;
}

HRESULT CUI::Release_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;
    if (RENDER_TEST == m_eRender)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    }
    else if (RENDER_BLEND == m_eRender)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
    /*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);*/
    return S_OK;
}

void CUI::SetActive_Unfixed(_float fActiveTime, _int iTexIndex)
{
}

_float CUI::Get_Ratio()
{
    return _float();
}

void CUI::Set_Ratio(_float fRatio)
{
}

void CUI::Set_Changed(_float fDest)
{
}

void CUI::Change_Ratio_while(_float fTimeDelta)
{
}

void CUI::Set_Render(_bool bInput, _int iTexIndex)
{
    m_bIsRender = bInput;
    if (0 <= iTexIndex)
        Set_TexIndex(iTexIndex);
}

void CUI::Set_Render_Timer(_bool bIsRender, _bool bIsAnimation, _int iTexIndex, _float fDuration)
{
    if (!m_bIsSetTimer)
    {
        m_bIsSetTimer = true;
        
    }
    else
    {
        m_fAccTime = 0.0f;
    }
    m_bIsRender = bIsRender;
    m_fDuration = fDuration;
    m_bIsAnimation = bIsAnimation;
    if (0 <= iTexIndex)
        Set_TexIndex(iTexIndex);
   
}


void CUI::Set_TexIndex(_uint iInput)
{
 m_fTexIndex = (_float)iInput;
}

_uint CUI::GetTexIndex()
{
    return (_uint)m_fTexIndex;
}

void CUI::Rotate_UI(const _float3& vAxis, _float fRadian)
{
    if (nullptr == m_pTransformCom)
        return;
    m_pTransformCom->Rotation(vAxis, fRadian);
}

void CUI::Set_Position(_float length, _float fRadian)
{
    _float fRadianAdd = D3DXToRadian(90.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, { length * cosf(fRadian+fRadianAdd), length * sinf(fRadian+fRadianAdd), 0.0f });
}

void CUI::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferRectCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
}
