#include "VIBuffer_Cylinder.h"

CVIBuffer_Cylinder::CVIBuffer_Cylinder(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cylinder::CVIBuffer_Cylinder(const CVIBuffer_Cylinder& rhs)
    : CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cylinder::Initialize_Prototype(const _uint iSliceCount, const float fHeight, const float fRadius)
{
	const float fTheta = -(D3DX_PI * 2.f) / float(iSliceCount);

	// Init Vertex/Index Description
	{
		m_iNumVertices = iSliceCount * 2 + 2;
		m_iStride = sizeof(VTXNORTEX);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
		m_PrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumPrimitive = iSliceCount * 2;
		m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
		m_iNumIndices = m_iNumPrimitive * 3;
		m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	}

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i <= iSliceCount; ++i)
	{
		pVertices[i].vPosition = m_pVerticesPos[i] = _float3(sinf(fTheta * i) * fRadius, -fHeight * 0.5f,
			cosf(fTheta * i) * fRadius);
		pVertices[i].vNormal = _float3(sinf(fTheta * i), 0.f, cosf(fTheta * i));
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
		pVertices[i].vTexcoord = _float2(float(i) / iSliceCount, 1.0f);
	}

	size_t iStep = 0;
	for (size_t i = iSliceCount + 1; i <= iSliceCount * 2 + 1; ++i)
	{
		pVertices[i].vPosition = m_pVerticesPos[i] = _float3(sinf(fTheta * iStep) * fRadius, fHeight * 0.5f,
			cosf(fTheta * iStep) * fRadius);
		pVertices[i].vNormal = _float3(sinf(fTheta * iStep), 0.f, cosf(fTheta * iStep));
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
		pVertices[i].vTexcoord = _float2(float((i - 1) - iSliceCount) / iSliceCount, 0.0f);

		++iStep;
	}

	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort* pIndices = { nullptr };
		m_pIndices = new _ushort[m_iNumIndices];
		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		_uint		iNumIndices = 0;

		_uint iIndexCount = 0;
		for (_uint i = 0; i < iSliceCount; i++)
		{
			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + iSliceCount + 1;
			pIndices[iIndexCount++] = i + 1 + iSliceCount + 1;

			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + 1 + iSliceCount + 1;
			pIndices[iIndexCount++] = i + 1;
		}

		memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong* pIndices = { nullptr };
		m_pIndices = new _ulong[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		_uint		iNumIndices = 0;

		_uint iIndexCount = 0;
		for (_uint i = 0; i < iSliceCount; i++)
		{
			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + iSliceCount + 1;
			pIndices[iIndexCount++] = i + 1 + iSliceCount + 1;

			pIndices[iIndexCount++] = i;
			pIndices[iIndexCount++] = i + 1 + iSliceCount + 1;
			pIndices[iIndexCount++] = i + 1;
		}

		memcpy(m_pIndices, pIndices, sizeof(_ulong) * m_iNumIndices);
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cylinder::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Cylinder* CVIBuffer_Cylinder::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint iSliceCount, const float fHeight, const float fRadius)
{
	CVIBuffer_Cylinder* pInstance = new CVIBuffer_Cylinder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iSliceCount, fHeight, fRadius)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Cylinder");
	}

	return pInstance;
}

CComponent* CVIBuffer_Cylinder::Clone(void* pArg)
{
	CVIBuffer_Cylinder* pInstance = new CVIBuffer_Cylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Cylinder");
	}

	return pInstance;
}

void CVIBuffer_Cylinder::Free()
{
	__super::Free();
}
