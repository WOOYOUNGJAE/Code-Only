#include "stdafx.h"
#include "..\Public\Camera_Player.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Player.h"
#include "Transform.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
	CAMERAFREEDESC* pCameraFreeDesc = (CAMERAFREEDESC*)pArg;

	m_iData = pCameraFreeDesc->iData;

	SetCursorPos(g_iWinSizeX / 2, g_iWinSizeY / 2);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pPlayer = static_cast<CPlayer*>(pGameInstance->GetPlayer());
	Safe_AddRef(m_pPlayer);
	m_pPlayer->SetCamera(this);

	Safe_Release(pGameInstance);
	if (FAILED(__super::Initialize(&pCameraFreeDesc->CameraDesc)))
		return E_FAIL;

	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Find_Component(L"Com_Transform"));



	return S_OK;
}

void CCamera_Player::Tick(_float fTimeDelta)
{
	if(m_bFollowPlayer)
	{
		_float3 vBuffer;
		m_pTransform->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vBuffer, &m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT)));
		m_pTransform->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vBuffer, &m_pPlayerTransform->Get_State(CTransform::STATE_UP)));
		m_pTransform->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vBuffer, &m_pPlayerTransform->Get_State(CTransform::STATE_LOOK)));
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) + m_vCameraPos);
	}

	__super::Set_Transform();
}

void CCamera_Player::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

CCamera_Player* CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Player* pInstance = new CCamera_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CCamera_Player");
	}

	return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
	CCamera_Player* pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CCamera_Free");
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pPlayer);
}
