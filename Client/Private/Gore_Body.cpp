#include "stdafx.h"
#include "..\Public\Gore_Body.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CGore_Body::CGore_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CGore_Body::CGore_Body(const CGore_Body& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGore_Body::Initialize_Prototype()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	return S_OK;
}

HRESULT CGore_Body::Initialize(void* pArg)
{
	GoreDesc* pDesc = (GoreDesc*)pArg;
	m_eEnemyType = pDesc->eEnemyType;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_iTextureCount = m_pTextureCom->Get_TextureCount();
	m_pBodyPositionPivots = new _float3[m_iTextureCount];

	for (_uint i = 0; i < m_iTextureCount; ++i)
	{
		m_pBodyPositionPivots[i].x = -2.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.f - (-2.f))));
		m_pBodyPositionPivots[i].y = -0.45f;
		m_pBodyPositionPivots[i].z = -2.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.f - (-2.f))));
	}

	// Set Position
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
	}

	// Set Scale
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * 0.2f);
		m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 1.f, 0.f) * 0.2f);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0.f, 0.f, 1.f) * 0.2f);
	}

	return S_OK;
}

void CGore_Body::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGore_Body::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	_float4x4		CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	D3DXVECTOR3 vScale = m_pTransformCom->Get_Scaled();
	_float3 vRight, vUp, vLook;
	vRight = *(_float3*)&CameraWorldMatrix.m[0][0] * vScale.x;
	vLook = *(_float3*)&CameraWorldMatrix.m[2][0] * vScale.y;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}


HRESULT CGore_Body::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	for (_uint i = 0; i < m_iTextureCount; ++i)
	{
		_float4x4 WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		WorldMatrix._41 += m_pBodyPositionPivots[i].x;
		WorldMatrix._42 += m_pBodyPositionPivots[i].y;
		WorldMatrix._43 += m_pBodyPositionPivots[i].z;

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(i)))
			return E_FAIL;

		m_pVIBufferCom->Render();
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGore_Body::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Billboard"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (m_eEnemyType == ENEMY_GOON)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Gore"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_eEnemyType == ENEMY_PIPEMAN)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Pipeguy_Gore"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Cube*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_CubeVIBuffer"), (CComponent**)&m_pCubeVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGore_Body::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);



	return S_OK;
}

HRESULT CGore_Body::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

CGore_Body* CGore_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGore_Body* pInstance = new CGore_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CGore_Body");
	}

	return pInstance;
}

CGameObject* CGore_Body::Clone(void* pArg)
{
	CGore_Body* pInstance = new CGore_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CGore_Body");
	}

	return pInstance;
}

void CGore_Body::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pDebugTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCubeVIBufferCom);
	Safe_Release(m_pPhysicsCom);
}
