#pragma once

#include "Component.h"

/* Vertex + Index ����(Buffer) �� ������.*/
/* �������� ��(�׸�, ť��, ���� ����) ���� �θ� �Ǵ� ũ������. */
BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

public:
	_bool Intersect_Polygons(const _float4x4& WorldMatrix, _float3* pOut);
	_bool Intersect_Polygons_ForRay(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOut);

	_bool Is_Vertex_In_Projection_Space(const _float3 vPosition,
		const _float4x4& WorldMatrix, const _float4x4& ViewMatrix, const _float4x4& ProjectionMatrix);

protected:
	/* ������ �迭�� ������ �޸� ������ ��ǥ�ϴ� �İ�ü */
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9				m_pIB = { nullptr };

protected:
	_uint								m_iStride = { 0 }; /* ���� �ϳ��� byteũ�� */
	_uint								m_iNumVertices = { 0 };
	_ulong								m_dwFVF = { 0 }; /* ������ �� ������ ��������־�! */
	D3DPRIMITIVETYPE					m_PrimitiveType;
	_uint								m_iNumPrimitive = { 0 };

protected:
	_uint								m_iIndexStride = { 0 }; /* �ε��� ��ĭ�� ũ�� (2 or 4 Byte)*/
	_uint								m_iNumIndices = { 0 };
	D3DFORMAT							m_eIndexFormat;

protected:
	_float3*							m_pVerticesPos = { nullptr };	
	void*								m_pIndices = { nullptr };

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END