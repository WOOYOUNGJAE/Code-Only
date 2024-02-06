#include "stdafx.h"
#include "..\Public\Smoke.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CSmoke::CSmoke(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSmoke::CSmoke(const CSmoke& rhs)
	: CGameObject(rhs)
{

}

HRESULT CSmoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSmoke::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 3.f, 3.f));
	m_pTransformCom->Scaling({ 6.f, 6.f, 6.f });

	return S_OK;
}

void CSmoke::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCloudPivot += (fTimeDelta * 0.1f);
}

void CSmoke::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	_float4x4		CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	D3DXVECTOR3 vScale = m_pTransformCom->Get_Scaled();
	_float3 vRight, vUp, vLook;
	vRight = *(_float3*)&CameraWorldMatrix.m[0][0] * vScale.x;
	vUp = *(_float3*)&CameraWorldMatrix.m[1][0] * vScale.y;
	vLook = *(_float3*)&CameraWorldMatrix.m[2][0] * vScale.z;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
}


HRESULT CSmoke::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	m_pVIBufferCom->Set_Amount(m_fCloudPivot);
	m_pVIBufferCom->SetWorldMatrix(m_pTransformCom->Get_WorldMatrix());

	D3DXMATRIX ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMATRIX ProjectionMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
	m_pVIBufferCom->SetViewMatrix(ViewMatrix);
	m_pVIBufferCom->SetProjectionMatrix(ProjectionMatrix);

	m_pVIBufferCom->Set_Texture(m_pTextureCom->Get_Texture(0));

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSmoke::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flow"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Cloud"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSmoke::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 115);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CSmoke::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSmoke* CSmoke::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSmoke* pInstance = new CSmoke(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CSmoke");
	}

	return pInstance;
}

CGameObject* CSmoke::Clone(void* pArg)
{
	CSmoke* pInstance = new CSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CSmoke");
	}

	return pInstance;
}

void CSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
