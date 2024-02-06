#include "stdafx.h"
#include "..\Public\BackgroundCylinder.h"

#include "GameInstance.h"

CBackgroundCylinder::CBackgroundCylinder(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBackgroundCylinder::CBackgroundCylinder(const CBackgroundCylinder& rhs)
	: CGameObject(rhs)
{

}

HRESULT CBackgroundCylinder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackgroundCylinder::Initialize(void* pArg)
{
	/* 백그라운드가 사용하기위한 컴포넌트들을 추가하낟. */
	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackgroundCylinder::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



}

void CBackgroundCylinder::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	_float4x4			ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3 vCameraPos = *(_float3*)&ViewMatrix.m[3][0];
	vCameraPos.y += 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}


HRESULT CBackgroundCylinder::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());



	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackgroundCylinder::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cylinder_64"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Sky"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

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

HRESULT CBackgroundCylinder::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

HRESULT CBackgroundCylinder::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CBackgroundCylinder* CBackgroundCylinder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackgroundCylinder* pInstance = new CBackgroundCylinder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CBackgroundCylinder");
	}

	return pInstance;
}

CGameObject* CBackgroundCylinder::Clone(void* pArg)
{
	CBackgroundCylinder* pInstance = new CBackgroundCylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CBackgroundCylinder");
	}

	return pInstance;
}

void CBackgroundCylinder::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
