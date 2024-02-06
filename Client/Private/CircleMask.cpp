#include "stdafx.h"
#include "..\Public\CircleMask.h"
#include "VIBuffer_Mask.h"

#include "Client_Struct.h"
#include "GameInstance.h"

CCircleMask::CCircleMask(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCircleMask::CCircleMask(const CCircleMask& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCircleMask::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCircleMask::Initialize(void* pArg)
{
	/* 백그라운드가 사용하기위한 컴포넌트들을 추가하낟. */
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	_float4x4		Scaling, Translation;

	m_WorldMatrix = *D3DXMatrixScaling(&Scaling, m_fSizeX, m_fSizeY, 1.f) *
		*D3DXMatrixTranslation(&Translation, m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	m_ViewMatrix = *D3DXMatrixIdentity(&m_ViewMatrix);

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);

	m_pVIBufferCom->Set_ViewportSize((_float)g_iWinSizeX, (_float)g_iWinSizeY);

	m_bEnable = false;

	CGameInstance::Get_Instance()->Subscribe(EVENT_FLOOR_CHANGE, std::bind(&CCircleMask::Floor_Change_Trigger, this, std::placeholders::_1));

	return S_OK;
}

void CCircleMask::Tick(_float fTimeDelta)
{
	if (m_bEnable == false)
		return;

	__super::Tick(fTimeDelta);
}

void CCircleMask::Late_Tick(_float fTimeDelta)
{
	if (m_bEnable == false)
		return;

	if (m_bFadeOut)
	{
		m_fCircleRadius -= fTimeDelta * m_fInoutSpeed;
		m_pVIBufferCom->Set_Radius(m_fCircleRadius);

		if (m_fCircleRadius < 0.f)
		{
			if (m_bChangeSectionTrigger == false)
			{
				CGameInstance::Get_Instance()->Change_Section(LEVEL_OUTDOOR, m_strCurrentSection, m_strNextSection);
				CTransform* pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->GetPlayer()->Find_Component(TEXT("Com_Transform"));
				pPlayerTransform->Set_State(CTransform::STATE_POSITION, m_vNewPosition);

				if (m_strNextSection == TEXT("Section_Ending"))
				{
					EFFECTDESC effect;
					effect.vScale = { 1.f,1.f,1.f };
					effect.vPos = { g_iWinSizeX / 2,g_iWinSizeY / 2, 0.f };
					effect.bActive = true;
					CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_SMOKING, &effect);
				}

				m_bChangeSectionTrigger = true;
			}
			m_fAccDelayTime += fTimeDelta;
			if (m_fAccDelayTime > m_fDelayThreshold)
			{
				m_bFadeOut = false;
				m_fAccDelayTime = 0.f;

				m_bChangeSectionTrigger = false;
			}
		}
	}
	else
	{
		m_fCircleRadius += fTimeDelta * m_fInoutSpeed;
		m_pVIBufferCom->Set_Radius(m_fCircleRadius);

		if (m_fCircleRadius > m_fCircleThreshold)
		{
			m_bEnable = false;
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_POST_PROCESSING, this);
}


HRESULT CCircleMask::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pVIBufferCom->SetWorldMatrix(m_WorldMatrix);
	m_pVIBufferCom->SetViewMatrix(m_ViewMatrix);
	m_pVIBufferCom->SetProjectionMatrix(m_ProjMatrix);

	//m_pVIBufferCom->Set_Radius(300.f);

	m_pVIBufferCom->Render();
	
	return S_OK;
}

_bool CCircleMask::Floor_Change_Trigger(void* pArg)
{
	m_bEnable = true;

	FLOORCHANGEDESC* desc = (FLOORCHANGEDESC*)pArg;
	m_strCurrentSection = desc->pFrom;
	m_strNextSection = desc->pTo;
	m_vNewPosition = desc->vNewPosition;

	m_fCircleRadius = m_fCircleThreshold;
	m_bFadeOut = true;
	CGameInstance::Get_Instance()->OnEvent(EVENT_CLEAR_PARTICLE, nullptr);

	return true;
}

HRESULT CCircleMask::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Mask"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CCircleMask* CCircleMask::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCircleMask* pInstance = new CCircleMask(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CCircleMask");
	}

	return pInstance;
}

CGameObject* CCircleMask::Clone(void* pArg)
{
	CCircleMask* pInstance = new CCircleMask(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CCircleMask");
	}

	return pInstance;
}

void CCircleMask::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
