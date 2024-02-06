#include "stdafx.h"
#include "..\Public\Event_Sphere.h"

#include "GameInstance.h"
#include "Client_Struct.h"

#include "EventDatabase.h"
#include "Sound_Manager.h"

#include "Player.h"

CEvent_Sphere::CEvent_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEvent_Sphere::CEvent_Sphere(const CEvent_Sphere& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEvent_Sphere::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEvent_Sphere::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;

	OBJECTDESC* tObjectDesc = (OBJECTDESC*)pArg;

	m_eObjectType = tObjectDesc->eObjectType;
	m_eTooltipType = tObjectDesc->eTooltipType;
	m_iRadioIndex = tObjectDesc->iRadioIndex;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tObjectDesc->vPosition);

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

		_float3 vScale = tObjectDesc->vScale;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

		m_fThresholdDist = vScale.x;
	}

	m_strEventTag = tObjectDesc->strSoundKey;

	CGameObject* pPlayer = CGameInstance::Get_Instance()->GetPlayer();
	if (nullptr != pPlayer)
	{
		m_pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")));
	}

	CGameInstance::Get_Instance()->Subscribe(EVENT_DEAD, std::bind(&CEvent_Sphere::On_Revive, this, std::placeholders::_1));
	CGameInstance::Get_Instance()->Subscribe(EVENT_SMOKE_ENABLE, std::bind(&CEvent_Sphere::On_SmokeEnable, this, std::placeholders::_1));
	
	return S_OK;
}

void CEvent_Sphere::Tick(_float fTimeDelta)
{
	if (false == m_bEnable)
	{
		return;
	}

	__super::Tick(fTimeDelta);

}

void CEvent_Sphere::Late_Tick(_float fTimeDelta)
{
	if (false == m_bEnable)
	{
		return;
	}

	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pPlayerTransform)
	{
		_float3 vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		_float fDist = D3DXVec3Length(&(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - vPlayerPos));

		if (fDist <= m_fThresholdDist)
		{
			switch (m_eObjectType)
			{
			case Client::OBJECT_EVENT_SECTION:
				CGameInstance::Get_Instance()->OnEvent(EVENT_SECTION);
				m_bEnable = false;
				break;
			case Client::OBJECT_TOOLTIP_EVENT:
			{
				TOOLTIP_RADIO_DESC tDesc;
				
				tDesc.fActiveTime = CEventDatabase::Get_Tooltip_Event_ActiveTime(m_eTooltipType);
				tDesc.iTexIndex = m_eTooltipType;

				CGameInstance::Get_Instance()->OnEvent(EVENT_TOOLTIP_ACTIVE, &tDesc);
				m_bEnable = false;
			}
				break;
			case Client::OBJECT_RADIO_EVENT:
			{
				TOOLTIP_RADIO_DESC tDesc;
				tDesc.fActiveTime = CEventDatabase::Get_Radio_Event_ActiveTime(m_iRadioIndex);
				tDesc.iTexIndex = m_iRadioIndex;

				CGameInstance::Get_Instance()->OnEvent(EVENT_RADIO_ACTIVE, &tDesc);
				m_bEnable = false;
			}
				break;
			case Client::OBJECT_BGM_EVENT:
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_BGM, &m_iRadioIndex);
				m_bEnable = false;
				break;
			case Client::OBJECT_ETC_EVENT:
				if (m_strEventTag == TEXT("Fall"))
				{
					ENEMYATTACKDESC desc;
					desc.pEnemyTransform = m_pTransformCom;
					desc.iDmg = -23;
					CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_ATTACK, &desc);
					m_bEnable = false;
				}
				break;
			}
		}
	}

	if (Client::OBJECT_ETC_EVENT == m_eObjectType && m_bSmokeEnable)
	{
		m_fAccEffectInterval += fTimeDelta;
		if ((m_fAccEffectInterval >= m_fEffectThreshold) && m_strEventTag == TEXT("Smoke"))
		{
			MOVEABLEEFFECTDESC effectDesc;
			effectDesc.bActive = true;
			effectDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, -1.f, 0.f);
			effectDesc.vScale = _float3(1.f, 1.f, 1.f);

			for (int i = 0; i < 10; ++i)
			{
				D3DXMATRIX mRot;
				D3DXMatrixRotationYawPitchRoll(&mRot, (((float)rand() / (float)RAND_MAX) * (D3DX_PI * 0.25f)) - (D3DX_PI * 0.25f) + (D3DX_PI * 0.5f), ((float)rand() / (float)RAND_MAX) * D3DX_PI * 0.5f + D3DX_PI * 0.5f, 0.f);
				
				_float3 vLookDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.1f;
				D3DXVec3TransformNormal(&effectDesc.vDir, &vLookDir, &mRot);
				CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_FIRE_STRIP, &effectDesc);
			}
			
			m_fAccEffectInterval = 0.f;
		}
	}

#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
#endif
}


HRESULT CEvent_Sphere::Render()
{
#ifdef _DEBUG // Debug 빌드일 때만 렌더링

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CEvent_Sphere::Add_Components()
{
#ifdef _DEBUG
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sphere"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
#endif

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

HRESULT CEvent_Sphere::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CEvent_Sphere::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

_bool CEvent_Sphere::On_Revive(void* pArg)
{
	if (m_eObjectType == Client::OBJECT_EVENT_SECTION || m_strEventTag == TEXT("Smoke"))
	{
		return false;
	}

	m_bEnable = true;
	return true;
}

_bool CEvent_Sphere::On_SmokeEnable(void* pArg)
{
	m_bSmokeEnable = *(_bool*)pArg;
	return true;
}

CEvent_Sphere* CEvent_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_Sphere* pInstance = new CEvent_Sphere(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CEvent_Sphere");
	}

	return pInstance;
}

CGameObject* CEvent_Sphere::Clone(void* pArg)
{
	CEvent_Sphere* pInstance = new CEvent_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CEvent_Sphere");
	}

	return pInstance;
}

void CEvent_Sphere::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
#endif
}
