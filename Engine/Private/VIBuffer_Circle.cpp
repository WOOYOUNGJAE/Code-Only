#include "VIBuffer_Circle.h"

CVIBuffer_Circle::CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Circle::CVIBuffer_Circle(const CVIBuffer_Circle& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Circle::Initialize_Prototype(const _uint iSliceCount)
{
	const float fTheta = -(D3DX_PI * 2.f) / float(iSliceCount);
	const float fRadius = 1.f;

	// Init Vertex/Index Description
	{
		m_iNumVertices = iSliceCount + 1;
		m_iStride = sizeof(VTXPOSTEX);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
		m_PrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumIndices = iSliceCount * 3;
		m_iNumPrimitive = iSliceCount;
		m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
		m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	}

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// Circle Center
	{
		pVertices[0].vPosition = m_pVerticesPos[0] = _float3(0.f, 0.f, 0.f);
		pVertices[0].vTexcoord = _float2(0.f, 0.0f);
	}

	float iStep = 0;
	for (size_t i = 1; i <= iSliceCount; ++i)
	{
		pVertices[i].vPosition = m_pVerticesPos[i] = _float3(cosf(iStep) * fRadius, 0.f, sinf(iStep) * fRadius);
		pVertices[i].vTexcoord = _float2(float(iStep) / iSliceCount, 1.0f);

		iStep += fTheta;
	}

	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort* pIndices = { nullptr };
		m_pIndices = new _ushort[m_iNumIndices];
		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		_uint iIndexCount = 0;
		for (_uint i = 1; i < iSliceCount; i++)
		{
			pIndices[iIndexCount++] = 0;
			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + 1;
		}

		// Last Piece
		{
			pIndices[iIndexCount++] = 0;
			pIndices[iIndexCount++] = iSliceCount;
			pIndices[iIndexCount++] = 1;
		}

		memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong* pIndices = { nullptr };
		m_pIndices = new _ulong[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		_uint iIndexCount = 0;
		for (_uint i = 1; i < iSliceCount; i++)
		{
			pIndices[iIndexCount++] = 0;
			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + 1;
		}

		// Last Piece
		{
			pIndices[iIndexCount++] = 0;
			pIndices[iIndexCount++] = iSliceCount;
			pIndices[iIndexCount++] = 1;
		}

		memcpy(m_pIndices, pIndices, sizeof(_ulong) * m_iNumIndices);
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Circle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Circle* CVIBuffer_Circle::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint iSliceCount)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iSliceCount)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Circle");
	}

	return pInstance;
}

CComponent* CVIBuffer_Circle::Clone(void* pArg)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Circle");
	}

	return pInstance;
}

void CVIBuffer_Circle::Free()
{
	__super::Free();
}
