#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	CAMERAFREEDESC* pCameraFreeDesc = (CAMERAFREEDESC*)pArg;

	m_iData = pCameraFreeDesc->iData;


	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);



	if (FAILED(__super::Initialize(&pCameraFreeDesc->CameraDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	m_pTransform->SetNextPos(_float3(0, 0, 0));

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransform->Go_Backward(fTimeDelta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransform->Go_Left(fTimeDelta);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransform->Go_Right(fTimeDelta);
	}

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_int		iX = { 0 };
		_int		iY = { 0 };

		if (iX = ptMouse.x - m_ptOldMousePos.x)
		{
			m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * iX * 0.1f);
		}

		if (iY = ptMouse.y - m_ptOldMousePos.y)
		{
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * iY * 0.1f);
		}
	}

	__super::Set_Transform();

	m_ptOldMousePos = ptMouse;

}

void CCamera_Free::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos + m_pTransform->GetNextPos());
}

CCamera_Free* CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free* pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CCamera_Free");
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CCamera_Free");
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}