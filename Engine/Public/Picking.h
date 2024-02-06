#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);

public: /* 뷰포트상에 마우스위치를 구하고. 투영까지 변환하고, 뷰까지 변환한다, 월드까지 가자. */
	void Tick();
	void Transform_ToLocalSpace(const _float4x4& WorldMatrix);
	void Transform_ToLocalSpace_ForRay(const _float4x4& WorldMatrix,const _float3& _ray, const _float3& _rayPos,_float3* pOutVec, _float3*pOutPos);


public:
	_bool Intersect_Triangle_InWorld(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3* pOut);
	_bool Intersect_Triangle_InLocal(const _float3 * pPointA, const _float3 * pPointB, const _float3 * pPointC, _float3* pOut);

	/* 다른 ray도 계산하기 위해 오버로딩 */
	_bool Intersect_Triangle_ForRay(const _float3* pPointA, const _float3* pPointB, const _float3* pPointC, const _float3& _ray, const _float3& _rayPos, _float3* pOut);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device;
	HWND				m_hWnd;

private:
	_float3				m_vMouseRay, m_vMouseRayPos;
	_float3				m_vMouseRay_Local, m_vMouseRayPos_Local;



public:
	virtual void Free() override;

	_float3 GetMouseRayPos() { return m_vMouseRayPos; }
};

END