#include "BlendObject.h"
#include "Transform.h"

CBlendObject::CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CBlendObject::CBlendObject(const CBlendObject& rhs)
    : CGameObject(rhs)
{
}

HRESULT CBlendObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlendObject::Initialize(void* pArg)
{
    return S_OK;
}

void CBlendObject::Tick(_float fTimeDelta)
{
}

void CBlendObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBlendObject::Render()
{
    return S_OK;
}

void CBlendObject::Compute_CamDistance(CTransform* pTransform)
{
    _float4x4		ViewMatrix;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    _float3			vCamPos = *((_float3*)&ViewMatrix.m[3][0]);

    m_fCamDistance = D3DXVec3Length(&(vCamPos - pTransform->Get_State(CTransform::STATE_POSITION)));
}

void CBlendObject::Free()
{
    __super::Free();
}
