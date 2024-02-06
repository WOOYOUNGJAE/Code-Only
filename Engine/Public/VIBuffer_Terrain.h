#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize_Prototype(const wstring& strHeightMap);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_float Compute_Height(const _float3& vWorldPos, const _float4x4& OwnerWorldMatrix);

private:
	_uint			m_iNumVerticesX = { 0 };
	_uint			m_iNumVerticesZ = { 0 };



public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring& strHeightMap);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END