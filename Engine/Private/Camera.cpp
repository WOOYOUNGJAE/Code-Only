#include "..\Public\Camera.h"
#include "Component_Manager.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return	S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	CAMERADESC*		pCameraDesc = (CAMERADESC*)pArg;

	CComponent_Manager*	pComponent_Manager = CComponent_Manager::Get_Instance();
	Safe_AddRef(pComponent_Manager);

	m_pTransform = (CTransform*)pComponent_Manager->Clone_Component(pCameraDesc->iLevelIndex, pCameraDesc->strTranformTag, &pCameraDesc->TransformDesc);
	if (nullptr == m_pTransform)
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	m_pTransform->Set_State(CTransform::STATE_POSITION, pCameraDesc->vEye);
	m_pTransform->LookAt(pCameraDesc->vAt);
	
	m_fFovy = pCameraDesc->fFovy;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;

	return	S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}


HRESULT CCamera::Set_Transform()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransform->Get_WorldMatrix_Inverse());

	_float4x4		ProjMatrix;
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));


	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}
