#include "stdafx.h"
#include "..\Public\Cloud.h"

#include "GameInstance.h"

CCloud::CCloud(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCloud::CCloud(const CCloud& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCloud::Initialize(void* pArg)
{
	/* 백그라운드가 사용하기위한 컴포넌트들을 추가하낟. */
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 1.f, 0.f));
	m_pTransformCom->Scaling(_float3(1000.f, 1000.f, 0.f));
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DX_PI * 0.5f);

	return S_OK;
}

void CCloud::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCloudPivot += (fTimeDelta * 0.1f);
}

void CCloud::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	_float4x4			ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3 vCameraPos = *(_float3*)&ViewMatrix.m[3][0];
	vCameraPos.y += 100.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}


HRESULT CCloud::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
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

HRESULT CCloud::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flow"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Cloud"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//Prototype_Component_Texture_Cloud
	/* For.Com_Transform */
	/* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCloud::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

HRESULT CCloud::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CCloud* CCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCloud* pInstance = new CCloud(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CCloud");
	}

	return pInstance;
}

CGameObject* CCloud::Clone(void* pArg)
{
	CCloud* pInstance = new CCloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CCloud");
	}

	return pInstance;
}

void CCloud::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
