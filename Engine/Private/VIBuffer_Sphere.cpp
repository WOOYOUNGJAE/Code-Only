#include "VIBuffer_Sphere.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs)
    : CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Sphere::Initialize_Prototype()
{
	const float radius = 1.f;
	const int numSlices = 15;
	const int numStacks = 15;

	const float dTheta = -(D3DX_PI * 2) / float(numSlices);
	const float dPhi = -D3DX_PI / float(numStacks);

	m_iNumVertices = (numSlices + 1) * (numStacks + 1);
	m_iStride = sizeof(VTXPOSTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumIndices = numSlices * numStacks * 6;
	m_iNumPrimitive = m_iNumIndices / 3;
	m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
	m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int j = 0; j <= numStacks; j++)
	{
		D3DXVECTOR3 stackStartPoint;
		D3DXVec3TransformCoord(&stackStartPoint, &D3DXVECTOR3(0.f, -radius, 0.f), D3DXMatrixRotationZ(&D3DXMATRIX(), dPhi * j));

		for (int i = 0; i <= numSlices; i++)
		{
			VTXPOSTEX& Vertex = pVertices[i + j * (numSlices + 1)];

			D3DXVec3TransformCoord(
				&Vertex.vPosition,
				&stackStartPoint,
				D3DXMatrixRotationY(&D3DXMATRIX(), dTheta * float(i)));

			m_pVerticesPos[i + j * (numSlices + 1)] = Vertex.vPosition;

			Vertex.vTexcoord = D3DXVECTOR2(float(i) / numSlices, 1.0f - float(j) / numStacks);
		}
	}

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort* pIndices = { nullptr };
		m_pIndices = new _ushort[m_iNumIndices];
		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		for (int j = 0; j < numStacks; j++) {
			const int offset = (numSlices + 1) * j;

			for (int i = 0; i < numSlices; i++) {
				pIndices[(j * numSlices + i) * 6 + 0] = offset + i;
				pIndices[(j * numSlices + i) * 6 + 1] = offset + i + numSlices + 1;
				pIndices[(j * numSlices + i) * 6 + 2] = offset + i + 1 + numSlices + 1;

				pIndices[(j * numSlices + i) * 6 + 3] = offset + i;
				pIndices[(j * numSlices + i) * 6 + 4] = offset + i + 1 + numSlices + 1;
				pIndices[(j * numSlices + i) * 6 + 5] = offset + i + 1;
			}
		}

		memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong* pIndices = { nullptr };
		m_pIndices = new _ulong[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		for (int j = 0; j < numStacks; j++) {
			const int offset = (numSlices + 1) * j;

			for (int i = 0; i < numSlices; i++) {
				pIndices[(j * numSlices + i) * 6 + 0] = offset + i;
				pIndices[(j * numSlices + i) * 6 + 1] = offset + i + numSlices + 1;
				pIndices[(j * numSlices + i) * 6 + 2] = offset + i + 1 + numSlices + 1;

				pIndices[(j * numSlices + i) * 6 + 3] = offset + i;
				pIndices[(j * numSlices + i) * 6 + 4] = offset + i + 1 + numSlices + 1;
				pIndices[(j * numSlices + i) * 6 + 5] = offset + i + 1;
			}
		}

		memcpy(m_pIndices, pIndices, sizeof(_ulong) * m_iNumIndices);
	}

	m_pIB->Unlock();

	m_pVB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Sphere::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Sphere* CVIBuffer_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Sphere");
	}

	return pInstance;
}

CComponent* CVIBuffer_Sphere::Clone(void* pArg)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Sphere");
	}

	return pInstance;
}

void CVIBuffer_Sphere::Free()
{
	__super::Free();
}
