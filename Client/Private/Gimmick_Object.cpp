#include "stdafx.h"
#include "Gimmick_Object.h"
#include "GameInstance.h"
#include "Client_Struct.h"
#include "Item_Equipment.h"
#include "Item_Interactive.h"
#include "GimmickManager.h"

CGimmick_Object* CGimmick_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGimmick_Object* pInstance = new CGimmick_Object(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CGimmick_Object");
	}

	return pInstance;
}

CGameObject* CGimmick_Object::Clone(void* pArg)
{
	CGimmick_Object* pInstance = new CGimmick_Object(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CGimmick_Object");
	}

	return pInstance;
}

HRESULT CGimmick_Object::Initialize_Prototype()
{
	return CInteractiveObject::Initialize_Prototype();
}

HRESULT CGimmick_Object::Initialize(void* pArg)
{
	OBJECTDESC* pObjDesc = (OBJECTDESC*)pArg;

	m_pGimmickMgrInstance = CGimmickManager::Get_Instance();
	m_eItemType = pObjDesc->eItemType;
	switch (m_eItemType)
	{
	case ITEM_FAN:
		pObjDesc->strTextureTag = TEXT("Gimmick_Fan");
		break;
	case ITEM_ENTRANCE_KEY:
		pObjDesc->strTextureTag = TEXT("Gimmick_Key");
		m_pGimmickMgrInstance->Set_KeyInstance(this);
		break;
	case ITEM_DOOR:
		pObjDesc->strTextureTag = TEXT("Gimmick_Fence");
		break;
	case ITEM_BUTTON:
		pObjDesc->strTextureTag = TEXT("Gimmick_Button");
		break;
	case ITEM_BLOODY_HAND_PRINT:
		pObjDesc->strTextureTag = TEXT("Gimmick_Bloody_Hand_Paint");
		break;
	case ITEM_WOODEN_DOOR:
		pObjDesc->strTextureTag = TEXT("Gimmick_Wooden_Door");
		break;
	case ITEM_METAL_DOOR:
		pObjDesc->strTextureTag = TEXT("Gimmick_Metal_Door");
		break;
	case ITEM_FANCY_DOOR:
		m_bRotationPivotRight = false;
		pObjDesc->strTextureTag = TEXT("Gimmick_Fancy_Door");
		break;
	case ITEM_CLUB_DOOR:
		m_bRotationPivotRight = false;
		pObjDesc->strTextureTag = TEXT("Gimmick_Club_Door");
		break;
	case ITEM_METAL_CLUB_DOOR:
		m_bRotationPivotRight = false;
		pObjDesc->strTextureTag = TEXT("Gimmick_Metal_Door");
		break;
	case ITEM_WOODEN_DOOR_R:
		m_bRotationPivotRight = true;
		pObjDesc->strTextureTag = TEXT("Gimmick_Wooden_Door_R");
		break;
	case ITEM_METAL_DOOR_R:
		m_bRotationPivotRight = true;
		pObjDesc->strTextureTag = TEXT("Gimmick_Metal_Door_R");
		break;
	case ITEM_FANCY_DOOR_R:
		m_bRotationPivotRight = true;
		pObjDesc->strTextureTag = TEXT("Gimmick_Fancy_Door_R");
		break;
	case ITEM_CLUB_DOOR_R:
		m_bRotationPivotRight = true;
		pObjDesc->strTextureTag = TEXT("Gimmick_Club_Door_R");
		break;
	default:
		break;
	}

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

		m_vScale = pObjDesc->vScale;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * m_vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * m_vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * m_vScale.z);
	}

	m_vRotationPivot = pObjDesc->vPosition;
	m_vRightPivot = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	if (m_bRotationPivotRight)
	{
		m_vRotationPivot += m_vRightPivot;
	}
	else
	{
		m_vRotationPivot -= m_vRightPivot;
	}

	

#pragma region Fan

#pragma endregion
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

void CGimmick_Object::Tick(_float fTimeDelta)
{
	CInteractiveObject::Tick(fTimeDelta);

	if (m_bPlayAnimation == true) // 애니메이션 플레이 상태
	{
		// animSpeed 1이면 1초에 모든 프레임
		m_fCurFrame += static_cast<_float>(m_iAnimFrameMax) * m_fAnimSpeed * fTimeDelta;
		if (m_fCurFrame >= m_iAnimFrameMax + 1)
		{
			m_fCurFrame = 0.f;
		}
	}


	//if (m_eItemType == ITEM_DOOR)
	if ((m_eItemType == ITEM_DOOR || m_eItemType == ITEM_WOODEN_DOOR)
		|| ((m_eItemType >= ITEM_WOODEN_DOOR_R) && (m_eItemType <= ITEM_FANCY_DOOR_R)) && m_bDoorOpening == true)
	{
		m_fRotateAmount += m_bRotateClockWise ? -fTimeDelta * m_fDoorSpeed : fTimeDelta * m_fDoorSpeed;
		if ((m_bRotateClockWise) && (m_fRotateAmount < m_fRotateThreshold))
		{
			return;
		}
		else if ((m_bRotateClockWise == false) && (m_fRotateAmount > m_fRotateThreshold))
		{
			return;
		}

		_float4x4 MatrixForTurning;
		D3DXMatrixIdentity(&MatrixForTurning);

		_float4x4 matTranslation;
		MatrixForTurning._41 = m_bRotationPivotRight ? -abs(m_vScale.x) : abs(m_vScale.x);

		_float4x4 matRotation;
		_float3 vUpOne = { 0,1,0 };
		
		D3DXMatrixRotationAxis(&matRotation, &vUpOne, -m_fRotateAmount);
		MatrixForTurning = MatrixForTurning * matRotation;

		D3DXMatrixIdentity(&matTranslation);
		D3DXMatrixTranslation(&matTranslation, m_vRotationPivot.x, m_vRotationPivot.y, m_vRotationPivot.z);
		MatrixForTurning = MatrixForTurning * matTranslation;
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&MatrixForTurning.m[0][0] * m_vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&MatrixForTurning.m[1][0] * m_vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&MatrixForTurning.m[2][0] * m_vScale.z);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)&MatrixForTurning.m[3][0]);
	}
}

void CGimmick_Object::Late_Tick(_float fTimeDelta)
{
	CInteractiveObject::Late_Tick(fTimeDelta);
}

HRESULT CGimmick_Object::Render()
{
	if (m_bEnable == true && m_bRenderAfterUsed == true)
	{
		if (nullptr == m_pVIBufferCom)
			return E_FAIL;

		_uint iFinalFrame = static_cast<_uint>(m_fCurFrame);

		if (m_eItemType == ITEM_ENTRANCE_KEY && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_BUTTON && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_BLOODY_HAND_PRINT && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_WOODEN_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_WOODEN_DOOR_R && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_METAL_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_METAL_DOOR_R && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_FANCY_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_FANCY_DOOR_R && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_CLUB_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_CLUB_DOOR_R && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		else if (m_eItemType == ITEM_METAL_CLUB_DOOR && m_bPrompt == true)
		{
			iFinalFrame = 1;
		}

		

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

		/* 장치에 텍스쳐를 바인딩한다. */
		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(iFinalFrame)))
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pVIBufferCom->Render();
		m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


		if (FAILED(Release_RenderState()))
			return E_FAIL;

		return S_OK;
	}
}


HRESULT CGimmick_Object::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Billboard"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

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

	// 아이템 타입에 따라 달라질 것들
	// Animation
	if (strTextureTag == TEXT("Gimmick_Fan"))
	{
		m_iAnimFrameMax = 1;
		m_fAnimSpeed = 6.f;
	}
	else if (m_eItemType == ITEM_ENTRANCE_KEY)
	{
		m_iAnimFrameMax = 0;
	}

	return S_OK;
}

_bool CGimmick_Object::ObjectHit(void* pArg)
{
	PLAYERATTACKDESC* pAt = static_cast<PLAYERATTACKDESC*>(pArg);
	_float3 vSourcePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vDestPos = pAt->pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vRayDir = vDestPos - vSourcePos;
	_float fDist = D3DXVec3Length(&vRayDir);
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	if (fDist < 3.f && m_bEnable)
	{
		//CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_STRIP, pAt->vPickPos);
		
		switch (m_eItemType)
		{
		case ITEM_FAN:
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				pGameInstance->StopSound(SOUND_EFFECT);
				pGameInstance->Play_Sound(TEXT("GenericSmash2.wav"), SOUND_EFFECT,1.f);				
			}
			m_bPlayAnimation = false;
			m_fCurFrame = 2.f; // 깨져있는 이미지
			m_fAnimSpeed = 6.f;
			m_bRenderAfterUsed = true;
			CGameObject::m_bPickable = false;
			break;
		case ITEM_ENTRANCE_KEY:
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			pGameInstance->StopSound(SOUND_EFFECT);
			pGameInstance->Play_Sound(TEXT("PickupKey2.wav"), SOUND_EFFECT, 1.f);
		}
			m_pGimmickMgrInstance->Set_EntranceKeyUsed(true);
			m_bRenderAfterUsed = false;
			CGameObject::m_bPickable = false;
			break;
		case ITEM_DOOR:
			m_bRenderAfterUsed = true;
			{
				// Noti
				_uint idx = NOTI_UNLOCK_IT;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			}
			if (m_pGimmickMgrInstance->Get_EntranceKeyUsed() == true)
			{
				m_pGimmickMgrInstance->Set_EntranceKeyUsed(false);
				m_bDoorOpening = true;
				m_bDoorLocked = false;
				CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT1);
				CGameInstance::Get_Instance()->Play_Sound(TEXT("MetalGateOpen.wav"), SOUND_ENVIRONMENT1, 1.f);

				_float3 vLookNormal = *D3DXVec3Normalize(&vLookNormal, &m_pTransformCom->Get_State(CTransform::STATE_LOOK));
				m_bRotateClockWise = (D3DXVec3Dot(&vRayDir, &vLookNormal) > 0.f);

				m_fRotateAmount = atan2(vLookNormal.z, vLookNormal.x) - (D3DX_PI * 0.5f);

				if (m_bRotateClockWise)
					m_fRotateThreshold = m_fRotateAmount - D3DX_PI * 0.4f;
				else
					m_fRotateThreshold = m_fRotateAmount + D3DX_PI * 0.4f;
			}
			else
			{
				_uint idx = NOTI_LOCKED;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
				if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
					CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, m_fSoundVolume);
			}
			break;
		case ITEM_CLUB_DOOR:
		case ITEM_CLUB_DOOR_R:
		case ITEM_METAL_CLUB_DOOR:
			CGameInstance::Get_Instance()->OnEvent(EVENT_SECTION);
			break;
		case ITEM_WOODEN_DOOR:
		case ITEM_WOODEN_DOOR_R:
		case ITEM_METAL_DOOR:
		case ITEM_METAL_DOOR_R:
		case ITEM_FANCY_DOOR:
		case ITEM_FANCY_DOOR_R:
		{
			m_bDoorOpening = true;

			CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT1);
			CGameInstance::Get_Instance()->Play_Sound(TEXT("WoodDoorOpen.wav"), SOUND_ENVIRONMENT1, 1.f);

			_float3 vLookNormal = *D3DXVec3Normalize(&vLookNormal, &m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			m_bRotateClockWise = (D3DXVec3Dot(&vRayDir, &vLookNormal) > 0.f);

			m_fRotateAmount = atan2(vLookNormal.z, vLookNormal.x) - (D3DX_PI * 0.5f);

			if (m_bRotationPivotRight)
			{
				m_bRotateClockWise = !m_bRotateClockWise;
			}

			if (m_bRotateClockWise)
				m_fRotateThreshold = m_fRotateAmount - D3DX_PI * 0.4f;
			else
				m_fRotateThreshold = m_fRotateAmount + D3DX_PI * 0.4f;
		}
		break;
		case ITEM_BUTTON:
			CGameInstance::Get_Instance()->OnEvent(EVENT_BUTTON);
			
			CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT1);
			CGameInstance::Get_Instance()->Play_Sound(TEXT("ButtonClick.wav"), SOUND_ENVIRONMENT1, 1.f);
			break;
		case ITEM_BLOODY_HAND_PRINT:
			CGameInstance::Get_Instance()->OnEvent(EVENT_SECRET_BUTTON);

			CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT1);
			CGameInstance::Get_Instance()->Play_Sound(TEXT("ButtonClick.wav"), SOUND_ENVIRONMENT1, 1.f);
			break;
		default:
			break;
		}
	}


	return true;
}

_bool CGimmick_Object::IsPicked(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos,
	_float3* pOut)
{
	if (m_bPickable == false)
	{
		return false;
	}

	return CInteractiveObject::IsPicked(WorldMatrix, _ray, _rayPos, pOut);
}

void CGimmick_Object::Set_Position(_float3 vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

//void CGimmick_Object::Item_Picked(CInteractiveObject* pTarget)
//{
//	CInteractiveObject::Item_Picked(pTarget);
//
//
//}
