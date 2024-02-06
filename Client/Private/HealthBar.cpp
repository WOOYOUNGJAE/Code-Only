#include "stdafx.h"
#include "..\Public\HealthBar.h"

#include "GameInstance.h"

CHealthBar::CHealthBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CHealthBar::CHealthBar(const CHealthBar& rhs)
	:CUI(rhs)
{
}

HRESULT CHealthBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHealthBar::Initialize(void* pArg)
{
	HEALTHBARDESC* pHealthBarDesc = (HEALTHBARDESC*)pArg;

	m_fConsumptionSpeed = pHealthBarDesc->fConsumptionSpeed;
	m_fRecoverSpeed = pHealthBarDesc->fRecoverSpeed;

	if (FAILED(__super::Initialize(&pHealthBarDesc->UIDesc)))
		return E_FAIL;

	return S_OK;
}

void CHealthBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHealthBar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CHealthBar::Render()
{
	__super::Render();

	return S_OK;
}

CHealthBar* CHealthBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHealthBar* pInstance = new CHealthBar(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CHealthBar");
	}
	return pInstance;
}

CGameObject* CHealthBar::Clone(void* pArg)
{
	CHealthBar* pInstance = new CHealthBar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CHealthBar");
	}
	return pInstance;
}

void CHealthBar::Free()
{
	__super::Free();
}
