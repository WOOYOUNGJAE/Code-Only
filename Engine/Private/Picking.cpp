#include "..\Public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}


HRESULT CPicking::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
 	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	m_hWnd = hWnd;

	return S_OK;
}

void CPicking::Tick()
{
	POINT		ptMouse;
	
	GetCursorPos(&ptMouse);

	/* 뷰포트 상의 마우스 좌표를 구한다. */
	ScreenToClient(m_hWnd, &ptMouse);

	/* 투영스페이스 상의 마우스 좌표를 구한다. */
	_float3		vMousePos;
	/* x : 0 -> -1, WINSIZEX -> 1 */
	/* y : 0 -> 1, WINSIZEY -> -1 */
	RECT		rcClient;
	GetClientRect(m_hWnd, &rcClient);

	vMousePos.x = ptMouse.x / (rcClient.right * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (rcClient.bottom * 0.5f) + 1.f;
	vMousePos.z = 0.f; /* near평면을 클릭하였기 때문에. */

	/* 뷰스페이스상의 마우스 Ray, 마우스 RayPos을 구한다. */
	_float4x4		ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);	

	/* 뷰스페이스 상의 마우스 위치. */
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrix);

	m_vMouseRayPos = _float3(0.f, 0.f, 0.f);
	m_vMouseRay = vMousePos - m_vMouseRayPos;

	/* 월드 스페이스 상의 마우스 Ray, RayPos */
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformCoord(&m_vMouseRayPos, &m_vMouseRayPos, &ViewMatrix);
	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &ViewMatrix);

	D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay);
}

void CPicking::Transform_ToLocalSpace(const _float4x4 & WorldMatrix)
{
	_float4x4		WorldMatrixInv;
	D3DXMatrixInverse(&WorldMatrixInv, nullptr, &WorldMatrix);
	D3DXVec3TransformCoord(&m_vMouseRayPos_Local, &m_vMouseRayPos, &WorldMatrixInv);
	D3DXVec3TransformNormal(&m_vMouseRay_Local, &m_vMouseRay, &WorldMatrixInv);

	D3DXVec3Normalize(&m_vMouseRay_Local, &m_vMouseRay_Local);
}

void CPicking::Transform_ToLocalSpace_ForRay(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOutVec, _float3* pOutPos)
{
	_float4x4		WorldMatrixInv;
	D3DXMatrixInverse(&WorldMatrixInv, nullptr, &WorldMatrix);
	D3DXVec3TransformCoord(pOutPos, &_rayPos, &WorldMatrixInv);
	D3DXVec3TransformNormal(pOutVec, &_ray, &WorldMatrixInv);

	D3DXVec3Normalize(pOutVec, pOutVec);
}

_bool CPicking::Intersect_Triangle_InWorld(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3* pOut)
{
	_float		fU, fV, fDist;

	_bool		isColl = { false };

	isColl = (_bool)D3DXIntersectTri(pPointA, pPointB, pPointC, &m_vMouseRayPos, &m_vMouseRay, &fU, &fV, &fDist);

	if (true == isColl)
	{
		*pOut = m_vMouseRay * fDist + m_vMouseRayPos;
	}

	return isColl;
}

_bool CPicking::Intersect_Triangle_InLocal(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3 * pOut)
{
	_float		fU, fV, fDist;

	_bool		isColl = { false };

	isColl = (_bool)D3DXIntersectTri(pPointA, pPointB, pPointC, &m_vMouseRayPos_Local, &m_vMouseRay_Local, &fU, &fV, &fDist);

	if (true == isColl)
	{
		*pOut = m_vMouseRay_Local * fDist + m_vMouseRayPos_Local;
	}

	return isColl;
}

_bool CPicking::Intersect_Triangle_ForRay(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, const _float3& _ray, const _float3& _rayPos, _float3* pOut)
{
	_float		fU, fV, fDist;

	_bool		isColl = { false };

	isColl = (_bool)D3DXIntersectTri(pPointA, pPointB, pPointC, &_rayPos, &_ray, &fU, &fV, &fDist);
	
	if (true == isColl)
	{
		*pOut = _ray * fDist + _rayPos;
	}
	else
	{
		int a = 1;
	}

	return isColl;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}
