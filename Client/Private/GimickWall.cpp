#include "stdafx.h"
#include "..\Public\GimickWall.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CGimickWall::CGimickWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CGimickWall::CGimickWall(const CGimickWall& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGimickWall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGimickWall::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	// Set Position
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-2.f, 2.5f, -18.f));
	}

	// Set Rotation
	{
		D3DXMATRIX Rotation;
		D3DXMatrixRotationYawPitchRoll(&Rotation, D3DX_PI, 0.f, 0.f);

		D3DXVECTOR3 vRight, vUp, vLook;
		D3DXVec3TransformNormal(&vRight, &_float3(1.f, 0.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vUp, &_float3(0.f, 1.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vLook, &_float3(0.f, 0.f, 1.f), &Rotation);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * 1.5f);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	}

	m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_pTransformCom->Get_WorldMatrix());

	CGameInstance::Get_Instance()->Subscribe(EVENT_SECRET_BUTTON, std::bind(&CGimickWall::OnButtonClickedEvent, this, std::placeholders::_1));

	return S_OK;
}

void CGimickWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGimickWall::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_bEventTriggered)
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->SetNextPos(_float3(0.f, 0.f, -fTimeDelta));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition + m_pTransformCom->GetNextPos());
		m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_pTransformCom->Get_WorldMatrix());
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}


HRESULT CGimickWall::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (m_pTextureCom->Bind_OnGraphicDevice())
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_bool CGimickWall::OnButtonClickedEvent(void* pArg)
{
	m_bEventTriggered = true;

	CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT2);
	CGameInstance::Get_Instance()->Play_Sound(TEXT("SlidingStone.wav"), SOUND_ENVIRONMENT2, 1.f);

	return false;
}

HRESULT CGimickWall::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Quad"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Wall"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

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

HRESULT CGimickWall::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (m_pDebugTextureCom->Bind_OnGraphicDevice())
		return E_FAIL;

	if (m_bDebugCollider)
	{
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
		m_pCubeVIBufferCom->Render();
	}
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, true);

	return S_OK;
}

HRESULT CGimickWall::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

CGimickWall* CGimickWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGimickWall* pInstance = new CGimickWall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CGimickWall");
	}

	return pInstance;
}

CGameObject* CGimickWall::Clone(void* pArg)
{
	CGimickWall* pInstance = new CGimickWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CGimickWall");
	}

	return pInstance;
}

void CGimickWall::Free()
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
