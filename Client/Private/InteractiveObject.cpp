#include "stdafx.h"
#include "InteractiveObject.h"
#include "GameInstance.h"
#include "Picking.h"
#include "Client_Struct.h"
#include "Item_Equipment.h"
#include "Item_Interactive.h"
#include "Item_Consumable.h"
#include "Item_ThrowOnly.h"

CInteractiveObject::CInteractiveObject(LPDIRECT3DDEVICE9 pGrahic_Device)
	:CGameObject(pGrahic_Device)
{
}

CInteractiveObject::CInteractiveObject(const CInteractiveObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CInteractiveObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteractiveObject::Initialize(void* pArg)
{
	OBJECTDESC* pObjDesc = (OBJECTDESC*)pArg;
	
	if (FAILED(Add_Components(pObjDesc->strTextureTag, pObjDesc->eMeshType)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pObjDesc->vPosition);

	// Set Rotation
	{
		D3DXQUATERNION vQuat;
		D3DXQuaternionNormalize(&vQuat, &pObjDesc->vQuaternion);

		D3DXMATRIX Rotation;
		D3DXMatrixRotationQuaternion(&Rotation, &vQuat);

		D3DXVECTOR3 vRight, vUp, vLook;
		D3DXVec3TransformNormal(&vRight, &_float3(1.f, 0.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vUp, &_float3(0.f, 1.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vLook, &_float3(0.f, 0.f, 1.f), &Rotation);

		_float3 vScale = pObjDesc->vScale;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	}

	// Set AlphaTesting
	{
		m_bAlphaTesting = pObjDesc->bAlphaTesting;
		m_iAlphaRef = pObjDesc->iAlphaRef;
	}

	// Set Billboard
	if (pObjDesc->eObjectType == OBJECT_BILLBOARD)
	{
		m_bBillboard = true;
	}

	m_pPhysicsCom->SetGravityForce(1.f);

	return S_OK;
}

void CInteractiveObject::Tick(_float fTimeDelta)
{
	if (true == m_bEnable)
	{
		__super::Tick(fTimeDelta);

		if (CGameInstance::Get_Instance()->Key_Down('Z'))
		{
			m_bDebugCollider = !m_bDebugCollider;
		}

		if (m_bThrow)
		{
			_float4x4 matScale, matTrans;
			D3DXMatrixIdentity(&m_matNotRot);
			D3DXMatrixScaling(&matScale, 0.2f, 0.3f, 0.2f);
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
			m_matNotRot *= matScale * matTrans;

			m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_matNotRot);

			CGameObject* pGameObject;
			_bool bCollided = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Environment"), &pGameObject);
			_bool bCollided2 = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Floor"), &pGameObject);
			CGameObject* pGameObjectEnemy;
			_bool bCollidedEnemy = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Enemy"), &pGameObjectEnemy);

			EFFECTDESC effect;
			PARTICLEEVENTDESC particle;
			effect.vScale = { 0.6f,0.6f,0.6f };
			effect.vPos = vPos;
			particle.targetPos = vPos;
			
			if (bCollided || bCollided2 || bCollidedEnemy)
			{
				m_bEnable = false;
				m_bThrow = false;
				//m_bItemCollideSound = true;
				CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_POW, &effect);
				if (m_eItemType == ITEM_TABLE_PLANT || m_eItemType == ITEM_POT_PLANT || m_eItemType == ITEM_TUMBLER_GLASS || (m_eItemType >= ITEM_POOL_BALL_0 && m_eItemType <= ITEM_POOL_BALL_15))
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
					CGameInstance::Get_Instance()->Play_Sound(L"GlassBreak3.wav", SOUND_ITEM, m_fSoundVolume);
					particle.ParticleType = particle.ParticleType = PARTICLE_BOARD;
					CGameInstance::Get_Instance()->OnEvent(EVENT_PARTICLE,&particle);
				}
				else if (m_eItemType >= ITEM_POOL_BALL_0 && m_eItemType <= ITEM_POOL_BALL_15)
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
					CGameInstance::Get_Instance()->Play_Sound(L"GlassBottleBounce.wav", SOUND_ITEM, m_fSoundVolume);
					particle.ParticleType = particle.ParticleType = PARTICLE_BOARD;
					CGameInstance::Get_Instance()->OnEvent(EVENT_PARTICLE, &particle);
				}
				else if (m_eItemType == ITEM_WOODEN_BARREL || m_eItemType == ITEM_TOOL_BOX || m_eItemType == ITEM_WOODEN_STOOL || m_eItemType == ITEM_UMBRELLA_STAND || m_eItemType == ITEM_COAT_HANGER || m_eItemType == ITEM_BAR_STOOL)
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
					CGameInstance::Get_Instance()->Play_Sound(L"WoodSmash1.wav", SOUND_ITEM, m_fSoundVolume);
					particle.ParticleType = particle.ParticleType = PARTICLE_BOARD;
					CGameInstance::Get_Instance()->OnEvent(EVENT_PARTICLE, &particle);
				}
				else if (m_eItemType == ITEM_SCRUNCHED_PAPER)
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
					CGameInstance::Get_Instance()->Play_Sound(L"HitGeneric1.wav", SOUND_ITEM, m_fSoundVolume);
					particle.ParticleType = particle.ParticleType = PARTICLE_BOARD;
					CGameInstance::Get_Instance()->OnEvent(EVENT_PARTICLE, &particle);
				}
				else
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_ITEM); 
					CGameInstance::Get_Instance()->Play_Sound(L"LandOnMetal.wav", SOUND_ITEM, m_fSoundVolume);
					particle.ParticleType = particle.ParticleType = PARTICLE_BOARD;
					CGameInstance::Get_Instance()->OnEvent(EVENT_PARTICLE, &particle);
				}
				
				if (bCollidedEnemy)
				{
					PLAYERATTACKDESC at;
					at.bHeavy = false;
					at.pPlayerTransform = m_pTransformCom;
					at.vPickPos = vPos;
					ObjectHit(&at);
					CGameInstance::Get_Instance()->OnEvent(EVENT_THROW_TO_ENEMY, pGameObjectEnemy);
				}
			
			}

			_float3 vPlayerLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&vPlayerLook ,&vPlayerLook);
			m_pPhysicsCom->ThrowPhysics(m_pTransformCom, vPlayerLook,fTimeDelta);
		}
		
	}
}

void CInteractiveObject::Late_Tick(_float fTimeDelta)
{
	if (true == m_bEnable)
	{
		__super::Late_Tick(fTimeDelta);

		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

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
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
}

HRESULT CInteractiveObject::Render()
{
	if (true == m_bEnable)
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
	
}

_bool CInteractiveObject::IsPicked(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOut)
{
	m_bPicked = m_pVIBufferCom->Intersect_Polygons_ForRay(WorldMatrix, _ray, _rayPos, pOut);

	return m_bPicked;
}

//OnEvent(EVENT_PLAYER_ATTACK)하면 OnItemPicked가 콜백되게 해야됨
//OnItemPicked는 아이템이 크로스헤어에서 쏘는 레이랑 맞았는지 확인하는거
_bool CInteractiveObject::OnItemPicked(void* pArg)
{
	//_float4x4* pOurWorldMatrix = (_float4x4*)pArg;
	_float3 vItemRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	_float3 vItemUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_float3 vItemLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vItemPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//아이템의 월드매트릭스
	_float4x4 ItemWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	// 카메라의 위치와 crosshair의 방향을 이용해 레이를 구성
	_float3 rayPos = { 0.f, 0.f, 0.f };
	_float3 rayDir = { 0.f, 0.f, 1.f };
	_float3 outPoint;

	_float4x4 CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	rayPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };
	rayDir = { CameraWorldMatrix._31, CameraWorldMatrix._32, CameraWorldMatrix._33 };


	if (m_pVIBufferCom->Intersect_Polygons_ForRay(ItemWorldMatrix, rayDir, rayPos, &outPoint))
	{
		m_bPicked = true;

		_float3 vDistVector = outPoint - rayPos;
		_float fDist = D3DXVec3Length(&vDistVector);
		m_fDistance = fDist;
	}
	

	


	/*if (fDist < 2.f)
		m_bPicked = true;

	else
		m_bPicked = false;*/

	return true;
}

void CInteractiveObject::Ready_ForThrow(CTransform* _transform)
{
	m_bThrow = true;
	m_bEnable = true;
	m_pPlayerTransform = _transform;
	_float3 vPlayerPos = _transform->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPlayerPos);
}

_bool CInteractiveObject::ObjectHit(void* pArg)
{
	PLAYERATTACKDESC* pAt = static_cast<PLAYERATTACKDESC*>(pArg);
	_float3 vSourcePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vDestPos = pAt->pPlayerTransform->Get_State(CTransform::STATE_POSITION);
 	_float fDist = D3DXVec3Length(&(vDestPos - vSourcePos));
	if (fDist < 3.f && m_bEnable)
	{
		EFFECTDESC effect;
		effect.vScale = { 0.5f,0.5f,0.5f };
		effect.vPos = pAt->vPickPos;
		CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_STRIP, &effect);
		m_bEnable = false;
		m_bBroken = true;
	}

	return true;
}

void CInteractiveObject::Item_Hited(const _float3& vPickedPosition, CInteractiveObject* pTarget)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (nullptr == pTarget)
	{
		Safe_Release(pGameInstance);
		return;
	}

	ITEM_TYPE TargetType = pTarget->Get_Item_Type();
	_float3 vTargetPosition = vPickedPosition;
	_float4x4 TargetMatrix = pTarget->m_pTransformCom->Get_WorldMatrix();

	//equipment picked
	if (0 <= TargetType && 100 >= TargetType)
	{
	//	dynamic_cast<CItem_Equipment*>(pTarget)->
	}

	//consumable picked
	if (101 <= TargetType && 200 >= TargetType)
	{
		//pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &vTargetPosition);
		//pGameInstance->OnEvent(EVENT_EFFECT_SMASHED, &vTargetPosition);
	}

	//interactive picked
	if (201 <= TargetType && 300 >= TargetType)
	{
		//dynamic_cast<CItem_Interactive*>(pTarget)->Set_Enable_Item(false);
		CGameInstance* pGameIns = CGameInstance::Get_Instance();
		Safe_AddRef(pGameIns);
		

		Safe_Release(pGameIns);
		//dynamic_cast<CItem_Interactive*>(pTarget)->Set_Effect_Item(true);
	}

	//throwonly picked
	if (301 <= TargetType && 900 >= TargetType)
	{

	}
	Safe_Release(pGameInstance);
}

void CInteractiveObject::Item_Picked(CInteractiveObject* pTarget)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (nullptr == pTarget)
	{
		Safe_Release(pGameInstance);
		return;
	}

	if (nullptr != pTarget)
	{
		ITEM_TYPE ItemType =  pTarget->Get_Item_Type();
		if (0 <= ItemType && ItemType <= 100)
		{
			CItem_Equipment* pEquipment = dynamic_cast<CItem_Equipment*>(pTarget);
			if (pEquipment != nullptr)
				pEquipment->Set_Prompter(true);
		}

		if (101 <= ItemType && ItemType <= 200)
		{
			CItem_Consumable* pConsum = dynamic_cast<CItem_Consumable*>(pTarget);
			if (pConsum != nullptr)
				pConsum->Set_Prompter(true);
		}
			

		if (201 <= ItemType && ItemType <= 300)
		{
			CItem_Interactive* pConsum = dynamic_cast<CItem_Interactive*>(pTarget);
			if (pConsum != nullptr)
				pConsum->Set_Prompter(true);
		}
		if (301 <= ItemType && ItemType <= 900)
		{
			CItem_ThrowOnly* pThrow = dynamic_cast<CItem_ThrowOnly*>(pTarget);
			if (pThrow != nullptr)
				pThrow->Set_Prompter(true);
		}
		
		

	}

	Safe_Release(pGameInstance);

}



HRESULT CInteractiveObject::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	switch (eMeshType)
	{
	case Client::MESH_QUAD:
	case Client::MESH_BILLBOARD:
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Billboard"),
			TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case Client::MESH_CUBE:
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
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

	/* For.Com_Physics */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_CubeVIBuffer"), (CComponent**)&m_pCubeVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInteractiveObject::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (m_bAlphaTesting)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, m_iAlphaRef);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/
	}


	if (m_bDebugCollider)
	{
		if (m_bThrow)
		{
			if (FAILED(m_pDebugTextureCom->Bind_OnGraphicDevice(1)))
				return E_FAIL;
			m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matNotRot);
			m_pCubeVIBufferCom->Render();
			m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

	return S_OK;
}

HRESULT CInteractiveObject::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (m_bAlphaTesting)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	return S_OK;
}

CInteractiveObject* CInteractiveObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInteractiveObject* pInstance = new CInteractiveObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CInteractiveObject");
	}

	return pInstance;
}

CGameObject* CInteractiveObject::Clone(void* pArg)
{
	CInteractiveObject* pInstance = new CInteractiveObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CInteractiveObject");
	}

	return pInstance;
}

void CInteractiveObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pPhysicsCom);
	Safe_Release(m_pCubeVIBufferCom);
	Safe_Release(m_pDebugTextureCom);
	Safe_Release(m_pTextureCom_Outline);
	Safe_Release(m_pTextureCom_Dead);
	 
}
