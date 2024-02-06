#include "stdafx.h"
#include "..\Public\Environment.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CEnvironment::CEnvironment(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEnvironment::CEnvironment(const CEnvironment& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEnvironment::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnvironment::Initialize(void* pArg)
{
	OBJECTDESC* tObjectDesc = (OBJECTDESC*)pArg;

	if (FAILED(Add_Components(tObjectDesc->strTextureTag, tObjectDesc->eMeshType)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tObjectDesc->vPosition);
	_float3 vScale = tObjectDesc->vScale;

	// Set Rotation
	{
		D3DXQUATERNION vQuat;
		D3DXQuaternionNormalize(&vQuat, &tObjectDesc->vQuaternion);

		D3DXMATRIX Rotation;
		D3DXMatrixRotationQuaternion(&Rotation, &vQuat);

		D3DXVECTOR3 vRight, vUp, vLook;
		D3DXVec3TransformNormal(&vRight, &_float3(1.f, 0.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vUp, &_float3(0.f, 1.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vLook, &_float3(0.f, 0.f, 1.f), &Rotation);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	}

	// Set Projection Space Culling
	/*{
		m_bProjectionCulling = true;
		if (vScale.x >= m_fCullingThreshold || vScale.y >= m_fCullingThreshold || vScale.z >= m_fCullingThreshold)
		{
			m_bProjectionCulling = false;
		}
	}*/

	// Set AlphaTesting
	{
		m_bAlphaTesting = tObjectDesc->bAlphaTesting;
		m_iAlphaRef = tObjectDesc->iAlphaRef;
	}

	// Set Billboard
	if (tObjectDesc->eObjectType == OBJECT_BILLBOARD)
	{
		m_bBillboard = true;
	}

	_float4x4 s, t;
	_float4x4 mat;
	if (m_eMeshType == MESH_QUAD)
	{
		m_WorldMatrix = *D3DXMatrixScaling(&s, 1.f, 1.f, 0.1f) * *D3DXMatrixTranslation(&t, 0.f, 0.f, -1.f) * m_pTransformCom->Get_WorldMatrix();
	}
	else if (m_eMeshType == MESH_GRID)
	{
		m_WorldMatrix = *D3DXMatrixScaling(&s, 4.5f, 0.1f, 4.5f) * *D3DXMatrixTranslation(&t, 4.5f, 0.f, 4.5f) * m_pTransformCom->Get_WorldMatrix();
	}
	else
	{
		m_WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(),&m_WorldMatrix);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//구독함수
	pGameInstance->Subscribe(EVENT_PLAYER_GUNFIRE,
		std::bind(&CEnvironment::OnCheckGunfireEffect, this, std::placeholders::_1));

	Safe_Release(pGameInstance);


	return S_OK;
}

void CEnvironment::Tick(_float fTimeDelta)
{
	/*if (false == m_bEnable)
	{
		return;
	}*/

	__super::Tick(fTimeDelta);

	if (m_bBillboard)
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
}

void CEnvironment::Late_Tick(_float fTimeDelta)
{
	/*if (false == m_bEnable)
	{
		return;
	}*/

	__super::Late_Tick(fTimeDelta);

	/*if (m_bProjectionCulling)
	{
		_float4x4 ProjectionSpaceMatrix;

		_float4x4		ViewMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		_float4x4 ProjectionMatrix;
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjectionMatrix);

		if (m_pVIBufferCom->Is_Vertex_In_Projection_Space(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_WorldMatrix(), ViewMatrix, ProjectionMatrix) == false)
		{
			return;
		}
	}*/

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}


HRESULT CEnvironment::Render()
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

_bool CEnvironment::OnCheckGunfireEffect(void* pArg)
{
	//_float3 pOut = _float3(0.f, 0.f, 0.f);
	//CGameObject* pPickItem = nullptr;
	//m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, TEXT("Layer_Environment"), &pOut, &pPickItem);
	//CGameInstance* pGameIns = CGameInstance::Get_Instance();
	//_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_float fDist = D3DXVec3Length(&(pOut - vPos));

	//if (pPickItem != nullptr )
	//{	
	//	pGameIns->OnEvent(EVENT_EFFECT_SPARKSTRIP, &pOut);
	//	
	//	return true;
	//	
	//}
	////마우스 피킹에 아이템이 안들어왔을때 
	//else
	//{
	//	
	//	
	//
	//}

	/*_float3 vRayHitPosOut;
	CGameObject* pGameObjOut = nullptr;
	_float4x4 TempWorldMatrix = m_pTransformCom->Get_WorldMatrix();


	if (m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, L"Layer_Environment", &vRayHitPosOut, &pGameObjOut) == true)
	{
		CGameInstance* pGameIns = CGameInstance::Get_Instance();
		Safe_AddRef(pGameIns);

		EFFECTDESC tEffectdesc;
		tEffectdesc.vScale = { 0.1f,0.1f,0.1f };
		tEffectdesc.vPos = vRayHitPosOut;
		tEffectdesc.bActive = true;
		pGameIns->OnEvent(EVENT_EFFECT_SPARKSTRIP, &tEffectdesc);
		Safe_Release(pGameIns);
		return true;
	}*/

	if (false == m_bEnable)
	{
		return true;
	}

	_float4x4 ObjectWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	// 카메라의 위치와 crosshair의 방향을 이용해 레이를 구성
	_float3 rayPos = { 0.f, 0.f, 0.f };
	_float3 rayDir = { 0.f, 0.f, 1.f };
	_float3 outPoint;

	_float4x4 CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	rayPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };
	rayDir = { CameraWorldMatrix._31, CameraWorldMatrix._32, CameraWorldMatrix._33 };
	

	if (m_pVIBufferCom->Intersect_Polygons_ForRay(ObjectWorldMatrix, rayDir, rayPos, &outPoint))
	{
		_float3 diff = outPoint - rayPos;
		_float distance = D3DXVec3Length(&diff);

		CGameInstance* pGameIns = CGameInstance::Get_Instance();
		Safe_AddRef(pGameIns);

		EFFECTDESC tEffectdesc;
		tEffectdesc.vScale = { 0.1f,0.1f,0.1f };
		tEffectdesc.vPos = outPoint;
		tEffectdesc.bActive = true;



		pGameIns->OnEvent(EVENT_EFFECT_SPARKSTRIP, &tEffectdesc);
		tEffectdesc.vScale = { 0.05f,0.05f,0.05f };
		pGameIns->OnEvent(EVENT_EFFECT_BULLET_HOLES, &tEffectdesc);

		Safe_Release(pGameIns);
	}

	


	return true;
}

HRESULT CEnvironment::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	m_eMeshType = eMeshType;

	switch (eMeshType)
	{
	case Client::MESH_QUAD:
		/* For.Com_VIBuffer_Quad */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Quad"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		
		break;
	case Client::MESH_GRID:
		/* For.Com_VIBuffer_Grid */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Grid"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case Client::MESH_CUBE:
		/* For.Com_VIBuffer_Cube */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case Client::MESH_CYLINDER:
		/* For.Com_VIBuffer_Cylinder */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cylinder_8"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case Client::MESH_CIRCLE:
		/* For.Com_VIBuffer_Circle */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Circle_8"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case Client::MESH_NONE:
		break;
	default:
		break;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_" + strTextureTag),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
		return E_FAIL;

	/* For.Com_Transform */
	/* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
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

HRESULT CEnvironment::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (m_bAlphaTesting)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, m_iAlphaRef);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

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

HRESULT CEnvironment::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (m_bAlphaTesting)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

CEnvironment* CEnvironment::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnvironment* pInstance = new CEnvironment(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CEnvironment");
	}

	return pInstance;
}

CGameObject* CEnvironment::Clone(void* pArg)
{
	CEnvironment* pInstance = new CEnvironment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CEnvironment");
	}

	return pInstance;
}

void CEnvironment::Free()
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
