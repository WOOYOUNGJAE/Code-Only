#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scaled() const
{
	return _float3(
		D3DXVec3Length(&Get_State(STATE_RIGHT)),
		D3DXVec3Length(&Get_State(STATE_UP)),
		D3DXVec3Length(&Get_State(STATE_LOOK)));
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vLook = Get_State(STATE_LOOK);
	m_vDir = vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	m_vDir = -vRight;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	m_vDir = vRight;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vLook = Get_State(STATE_LOOK);
	m_vDir = -vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3 vUp = Get_State(STATE_UP);
	m_vDir = vUp;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Straight_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Left_FixedY(_float fTimeDelta)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	m_vDir = -vRight;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3 vUp = Get_State(STATE_UP);
	m_vDir = -vUp;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Dir(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_DirBack(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE_POSITION);
	D3DXVec3Normalize(&m_vDir, &-m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Right_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	m_vDir = vRight;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_Backward_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = -vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_LeftUp_FixedY(_float fTimeDelta)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = -vRight + vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_RightUp_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = vRight + vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_LeftDown_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = -vRight - vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Go_RightDown_FixedY(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);
	vRight.y = 0.f;
	_float3		vLook = Get_State(STATE_LOOK);
	vLook.y = 0.f;
	m_vDir = vRight - vLook;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vNextPos += m_vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;
}

void CTransform::Turn(const _float3 & vAxis, _float fTimeDelta)
{
	_float4x4		RotationMatrix = *D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	_float3			vRight = Get_State(STATE_RIGHT);
	_float3			vUp = Get_State(STATE_UP);
	_float3			vLook = Get_State(STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(const _float3 & vAxis, float fRadian)
{
	_float3			vScale = Get_Scaled();

	_float4x4		RotationMatrix = *D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	_float3			vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3			vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3			vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Scaling(const _float3 & vScale)
{
	_float3		vRight, vUp, vLook;

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &Get_State(STATE_RIGHT)) * vScale.x);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &Get_State(STATE_UP)) * vScale.y);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &Get_State(STATE_LOOK)) * vScale.z);
}

void CTransform::Scaling_Ratio(const _float3& vScale)
{
	Set_State(STATE_RIGHT, Get_State(STATE_RIGHT) * vScale.x);
	Set_State(STATE_UP, Get_State(STATE_UP) * vScale.y);
	Set_State(STATE_LOOK, Get_State(STATE_LOOK) * vScale.z);
}



void CTransform::LookAt(const _float3 & vAt)
{
	_float3		vScale = Get_Scaled();

	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vLook = *D3DXVec3Normalize(&vLook, &(vAt - vPosition)) * vScale.z;
	_float3		vRight = *D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook)) * vScale.x;
	_float3		vUp = *D3DXVec3Normalize(&vUp, D3DXVec3Cross(&vUp, &vLook, &vRight)) * vScale.y;
	
	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::TurnOneSide(const _float3& vAxis, const _float3& vMove, _float fTimeDelta)
{
	_float3 vCurPos = Get_State(STATE_POSITION);
	_float fDistance = D3DXVec3Length(&vMove);

	_float4x4		TranslationMatrix = *D3DXMatrixTranslation(&TranslationMatrix, vCurPos.x + fDistance, vCurPos.y, vCurPos.z);

	_float4x4		TranslationMatrixInverse;
	D3DXMatrixTranslation(&TranslationMatrixInverse, -vCurPos.x , -vCurPos.y, -vCurPos.z);




	_float4x4		RotationMatrix = *D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	_float4x4		RotationMatrixFinal = *TranslationMatrixInverse * RotationMatrix * TranslationMatrix;


	_float3			vRight = Get_State(STATE_RIGHT);
	_float3			vUp = Get_State(STATE_UP);
	_float3			vLook = Get_State(STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrixFinal);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrixFinal);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrixFinal);
	//D3DXVec3TransformCoord(&vCurPos, &vCurPos, &RotationMatrixFinal);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	//Set_State(STATE_POSITION, vCurPos);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CTransform");
	}


	return pInstance;
}

CComponent * CTransform::Clone(void* pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CTransform");
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
