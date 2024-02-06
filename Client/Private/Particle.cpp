#include "Particle.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Physics.h"
#include "GameInstance.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CParticle::CParticle(const CParticle& rhs)
    :CGameObject(rhs)
{
}

HRESULT CParticle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
    PARTICLEDESC* pParticleDesc = (PARTICLEDESC*)pArg;

    m_TextureTag = pParticleDesc->strParticleTexTag;
    m_iIndex = pParticleDesc->iIndex;
    m_ParticleType = pParticleDesc->ParticleType;
    m_iSpread = pParticleDesc->iSpread;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_Dir = _float3(_float(rand() % m_iSpread - m_iSpread / 2), float(rand() % m_iSpread - m_iSpread / 2), float(rand() % m_iSpread - m_iSpread / 2));
    m_pTransformCom->SetNextPos(_float3{ 0.0f, 0.0f, 0.0f });
    return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
    if (false == m_pPhysicsCom->GetGround() && true == m_bRender)
    {
        m_pTransformCom->SetNextPos(m_pTransformCom->GetNextPos() + m_Dir * fTimeDelta);
        m_pPhysicsCom->GroundPhysics(m_pTransformCom, LEVEL_OUTDOOR, TEXT("Layer_Floor"), fTimeDelta);
    }
}

void CParticle::Late_Tick(_float fTimeDelta)
{

    _float4x4		CameraWorldMatrix;

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
    D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);


    D3DXVECTOR3 vScale = m_pTransformCom->Get_Scaled();
    _float3 vRight, vUp, vLook;
    vRight = *(_float3*)&CameraWorldMatrix.m[0][0] * vScale.x;
    vLook = *(_float3*)&CameraWorldMatrix.m[2][0] * vScale.y;
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    //m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&CameraWorldMatrix.m[1][0]);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

}



HRESULT CParticle::Render()
{

    if (true == m_bRender)
    {
        m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());
        if (FAILED())
            return E_FAIL;
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
        SetUp_RenderState();
        m_pVIBufferCom->Render();
        Release_RenderState();
    }
    return S_OK;
}

void CParticle::Set_Position(_float3 TargetPos)
{
    m_pTransformCom->SetNextPos(TargetPos);
    m_bRender = true;
}

void CParticle::Set_Texture(_uint iTexIndex)
{
    m_pTextureCom->Bind_OnGraphicDevice(iTexIndex);
}

HRESULT CParticle::Add_Components()
{
    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
        TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_LeftVIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, m_TextureTag,
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    CTransform::TRANSFORMDESC		TransformDesc;
    TransformDesc.fSpeedPerSec = 10.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
        return E_FAIL;

    /* For.Com_Physics */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
        TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
        return E_FAIL;
    return S_OK;
}

HRESULT CParticle::SetUp_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    return S_OK;
}

HRESULT CParticle::Release_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

    return S_OK;
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CParticle* pInstance = new CParticle(pGraphic_Device);
    if (nullptr == pInstance)
        return nullptr;
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created: Particle");
    }

    return pInstance;
}

CGameObject* CParticle::Clone(void* pArg)
{
    CParticle* pInstance = new CParticle(*this);
    if (nullptr == pInstance)
        return nullptr;
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned: Particle");
    }
    return nullptr;
}

void CParticle::Free()
{
    __super::Free();
    Safe_Release(m_pPhysicsCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}