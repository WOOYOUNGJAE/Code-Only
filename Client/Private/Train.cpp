#include "stdafx.h"
#include "..\Public\Train.h"

#include "GameInstance.h"
#include "Client_Struct.h"

#include "Layer.h"
#include "EnemyBase.h"

CTrain::CTrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTrain::CTrain(const CTrain& rhs)
	: CGameObject(rhs)
{

}

HRESULT CTrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrain::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	CGameInstance::Get_Instance()->Subscribe(EVENT_TRAIN_COME, std::bind(&CTrain::OnTrainActivate, this, std::placeholders::_1));

	// Set Position
	{
		m_vPivotPosition = _float3(-26.f, -23.f, 121.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPivotPosition);
	}

	// Set Rotation
	{
		D3DXMATRIX Rotation;
		D3DXMatrixRotationYawPitchRoll(&Rotation, 0.f, 0.f, 0.f);

		D3DXVECTOR3 vRight, vUp, vLook;
		D3DXVec3TransformNormal(&vRight, &_float3(1.f, 0.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vUp, &_float3(0.f, 1.f, 0.f), &Rotation);
		D3DXVec3TransformNormal(&vLook, &_float3(0.f, 0.f, 1.f), &Rotation);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	}

	// Pivots
	{
		// Front
		{
			D3DXMatrixIdentity(&m_PivotTransform[0]);

			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot, D3DXToRadian(-40.f));

			m_vDirection = *(_float3*)mRot.m[2];
			m_vDirection *= -1.f;

			D3DXMATRIX mScale;
			D3DXMatrixScaling(&mScale, 2.5f, 3.f, 1.f);

			m_PivotTransform[0] = mScale * mRot;
		}

		// Back
		{
			D3DXMatrixIdentity(&m_PivotTransform[1]);

			D3DXMATRIX mScale;
			D3DXMatrixScaling(&mScale, 2.5f, 3.f, 1.f);
			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot, D3DXToRadian(140.f));
			D3DXMATRIX mTranslation;
			D3DXMatrixTranslation(&mTranslation, -8.98f, 0.f, 10.7f);

			m_PivotTransform[1] = mScale * mRot * mTranslation;
		}

		// Left
		{
			D3DXMatrixIdentity(&m_PivotTransform[2]);

			D3DXMATRIX mScale;
			D3DXMatrixScaling(&mScale, 8.f, 3.f, 1.f);
			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot, D3DXToRadian(50.f));
			D3DXMATRIX mTranslation;
			D3DXMatrixTranslation(&mTranslation, -5.64f, 0.f, 4.39f);

			m_PivotTransform[2] = mScale * mRot * mTranslation;
		}

		// Right
		{
			D3DXMatrixIdentity(&m_PivotTransform[3]);

			D3DXMATRIX mScale;
			D3DXMatrixScaling(&mScale, 8.f, 3.f, 1.f);
			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot, D3DXToRadian(230.f));
			D3DXMATRIX mTranslation;
			D3DXMatrixTranslation(&mTranslation, -3.35f, 0.f, 6.326f);

			m_PivotTransform[3] = mScale * mRot * mTranslation;
		}
	}

	// Set Position
	{
		m_vPivotPosition -= m_vDirection * 20.0f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPivotPosition);
	}

	// Update Light Pos
	{
		D3DLIGHT9 LightDesc;
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = m_vPivotPosition;
		LightDesc.Range = 10.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(7, &LightDesc);
		m_pGraphic_Device->LightEnable(7, TRUE);
	}

	m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_pTransformCom->Get_WorldMatrix());

	return S_OK;
}

void CTrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		m_fAccRunTime += fTimeDelta;

		if (m_fAccRunTime > m_fRespawnThreshold)
		{ 
			m_fAccRunTime = 0.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPivotPosition);

			//CGameInstance::Get_Instance()->StopSound(SOUND_ENVIRONMENT1);
			//CGameInstance::Get_Instance()->Play_Sound(TEXT("TrainLoop2.wav"), SOUND_ENVIRONMENT1, 1.f);

			//임시로 주석
			/*BGMINFO BGMInfo;
			BGMInfo.pSoundKey = L"TrainLoop2.wav";
			BGMInfo.ChannelId = SOUND_BGM3;
			BGMInfo.fVolume = 0.5f;
			CGameInstance::Get_Instance()->OnEvent(EVENT_PLAY_BGM, &BGMInfo);*/
			
			
			CGameInstance::Get_Instance()->PlayBGM(L"TrainLoop2.wav", SOUND_BGM3, 0.5f);
		}

		if (m_fAccRunTime > m_fGoneThreshold)
		{
			/*_uint ChannelID = SOUND_BGM3;
			CGameInstance::Get_Instance()->OnEvent(EVENT_STOP_BGM, &ChannelID);*/
			//임시 주석
			CGameInstance::Get_Instance()->StopBGM(SOUND_BGM3);
		}

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OUTDOOR, TEXT("Layer_Enemy"));

		_float3 vTrainPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		for (auto pEnemy : pLayer->Get_GameObjectList())
		{
			// ÀÌ¹Ì Ä¡¿´À¸¸é
			if (static_cast<CEnemyBase*>(pEnemy)->Get_Hit_byTrain() == true)
			{
				continue;
			}

			CTransform* pEnemyTransform = (CTransform*)pEnemy->Find_Component(TEXT("Com_Transform"));

			_float3 vEnemyPosition = pEnemyTransform->Get_State(CTransform::STATE_POSITION);

			if (D3DXVec3Length(&(vEnemyPosition - vTrainPosition)) < m_fAccidentThreshold)
			{
				static_cast<CEnemyBase*>(pEnemy)->Set_Hit_byTrain();

				/*GoreDesc tGoreDesc;
				tGoreDesc.vPosition = vEnemyPosition;
				tGoreDesc.eEnemyType = ((CEnemyBase*)pEnemy)->Get_EnemyType();
				if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Gore"), TEXT("Prototype_Gore"), TEXT("Section_Outdoor"), &tGoreDesc)))
				{
					Safe_Release(pGameInstance);
				}*/
			}
		}

		// Update Light Pos
		{
			D3DLIGHT9 LightDesc;
			ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

			LightDesc.Type = D3DLIGHT_POINT;
			LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
			LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			LightDesc.Position = vTrainPosition;
			LightDesc.Range = 10.0f;
			LightDesc.Attenuation0 = 1.0f;
			LightDesc.Attenuation1 = 0.0f;
			LightDesc.Attenuation2 = 0.0f;
			m_pGraphic_Device->SetLight(7, &LightDesc);
			m_pGraphic_Device->LightEnable(7, TRUE);
		}

		Safe_Release(pGameInstance);
	}
}

void CTrain::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_bActive)
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->SetNextPos(m_vDirection * fTimeDelta * m_fSpeed);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition + m_pTransformCom->GetNextPos());
		m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_pTransformCom->Get_WorldMatrix());
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}


HRESULT CTrain::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	// Front / Back
	{
		for (int i = 0; i < 2; ++i)
		{
			_float4x4 matRenderState = m_PivotTransform[i] * m_pTransformCom->Get_WorldMatrix();
			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matRenderState);

			if (m_pFrontTextureCom->Bind_OnGraphicDevice())
				return E_FAIL;

			m_pVIBufferCom->Render();
		}
	}
	// Side
	{
		for (int i = 2; i < 4; ++i)
		{
			_float4x4 matRenderState = m_PivotTransform[i] * m_pTransformCom->Get_WorldMatrix();
			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matRenderState);

			if (m_pSideTextureCom->Bind_OnGraphicDevice())
				return E_FAIL;

			m_pVIBufferCom->Render();
		}
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_bool CTrain::OnTrainActivate(void* pArg)
{
	m_bActive = true;
	return false;
}

HRESULT CTrain::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer_Quad */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Quad"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Front_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Train_Front"),
		TEXT("Com_Front_Texture"), (CComponent**)&m_pFrontTextureCom)))
		return E_FAIL;

	/* For.Com_Side_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Train_Side"),
		TEXT("Com_Side_Texture"), (CComponent**)&m_pSideTextureCom)))
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

HRESULT CTrain::SetUp_RenderState()
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

HRESULT CTrain::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

CTrain* CTrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain* pInstance = new CTrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CTrain");
	}

	return pInstance;
}

CGameObject* CTrain::Clone(void* pArg)
{
	CTrain* pInstance = new CTrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CTrain");
	}

	return pInstance;
}

void CTrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pFrontTextureCom);
	Safe_Release(m_pSideTextureCom);
	Safe_Release(m_pDebugTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCubeVIBufferCom);
	Safe_Release(m_pPhysicsCom);
}
