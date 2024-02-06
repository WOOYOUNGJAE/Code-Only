#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	m_iNumVertices = 24;
	m_iStride = sizeof(VTXNORTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;
	m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-1.0f, -1.0f, -1.0f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[0].vNormal = _float3(0.f, 0.f, -1.f);
	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(-1.0f, 1.0f, -1.0f);
	pVertices[1].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[1].vNormal = _float3(0.f, 0.f, -1.f);
	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(1.0f, 1.0f, -1.0f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[2].vNormal = _float3(0.f, 0.f, -1.f);
	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(1.0f, -1.0f, -1.0f);
	pVertices[3].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[3].vNormal = _float3(0.f, 0.f, -1.f);

	pVertices[4].vPosition = m_pVerticesPos[4] = _float3(-1.0f, -1.0f, 1.0f);
	pVertices[4].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[4].vNormal = _float3(0.f, 0.f, 1.f);
	pVertices[5].vPosition = m_pVerticesPos[5] = _float3(1.0f, -1.0f, 1.0f);
	pVertices[5].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[5].vNormal = _float3(0.f, 0.f, 1.f);
	pVertices[6].vPosition = m_pVerticesPos[6] = _float3(1.0f, 1.0f, 1.0f);
	pVertices[6].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[6].vNormal = _float3(0.f, 0.f, 1.f);
	pVertices[7].vPosition = m_pVerticesPos[7] = _float3(-1.0f, 1.0f, 1.0f);
	pVertices[7].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[7].vNormal = _float3(0.f, 0.f, 1.f);

	pVertices[8].vPosition = m_pVerticesPos[8] = _float3(-1.0f, 1.0f, -1.0f);
	pVertices[8].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[8].vNormal = _float3(0.f, 1.f, 0.f);
	pVertices[9].vPosition = m_pVerticesPos[9] = _float3(-1.0f, 1.0f, 1.0f);
	pVertices[9].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[9].vNormal = _float3(0.f, 1.f, 0.f);
	pVertices[10].vPosition = m_pVerticesPos[10] = _float3(1.0f, 1.0f, 1.0f);
	pVertices[10].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[10].vNormal = _float3(0.f, 1.f, 0.f);
	pVertices[11].vPosition = m_pVerticesPos[11] = _float3(1.0f, 1.0f, -1.0f);
	pVertices[11].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[11].vNormal = _float3(0.f, 1.f, 0.f);

	pVertices[12].vPosition = m_pVerticesPos[12] = _float3(-1.0f, -1.0f, -1.0f);
	pVertices[12].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[12].vNormal = _float3(0.f, -1.f, 0.f);
	pVertices[13].vPosition = m_pVerticesPos[13] = _float3(1.0f, -1.0f, -1.0f);
	pVertices[13].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[13].vNormal = _float3(0.f, -1.f, 0.f);
	pVertices[14].vPosition = m_pVerticesPos[14] = _float3(1.0f, -1.0f, 1.0f);
	pVertices[14].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[14].vNormal = _float3(0.f, -1.f, 0.f);
	pVertices[15].vPosition = m_pVerticesPos[15] = _float3(-1.0f, -1.0f, 1.0f);
	pVertices[15].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[15].vNormal = _float3(0.f, -1.f, 0.f);

	pVertices[16].vPosition = m_pVerticesPos[16] = _float3(-1.0f, -1.0f, 1.0f);
	pVertices[16].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[16].vNormal = _float3(-1.f, 0.f, 0.f);
	pVertices[17].vPosition = m_pVerticesPos[17] = _float3(-1.0f, 1.0f, 1.0f);
	pVertices[17].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[17].vNormal = _float3(-1.f, 0.f, 0.f);
	pVertices[18].vPosition = m_pVerticesPos[18] = _float3(-1.0f, 1.0f, -1.0f);
	pVertices[18].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[18].vNormal = _float3(-1.f, 0.f, 0.f);
	pVertices[19].vPosition = m_pVerticesPos[19] = _float3(-1.0f, -1.0f, -1.0f);
	pVertices[19].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[19].vNormal = _float3(-1.f, 0.f, 0.f);

	pVertices[20].vPosition = m_pVerticesPos[20] = _float3(1.0f, -1.0f, -1.0f);
	pVertices[20].vTexcoord = _float2(0.0f, 0.0f);
	pVertices[20].vNormal = _float3(1.f, 0.f, 0.f);
	pVertices[21].vPosition = m_pVerticesPos[21] = _float3(1.0f, 1.0f, -1.0f);
	pVertices[21].vTexcoord = _float2(0.0f, 1.0f);
	pVertices[21].vNormal = _float3(1.f, 0.f, 0.f);
	pVertices[22].vPosition = m_pVerticesPos[22] = _float3(1.0f, 1.0f, 1.0f);
	pVertices[22].vTexcoord = _float2(1.0f, 1.0f);
	pVertices[22].vNormal = _float3(1.f, 0.f, 0.f);
	pVertices[23].vPosition = m_pVerticesPos[23] = _float3(1.0f, -1.0f, 1.0f);
	pVertices[23].vTexcoord = _float2(1.0f, 0.0f);
	pVertices[23].vNormal = _float3(1.f, 0.f, 0.f);

	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };
	m_pIndices = new _ushort[m_iNumIndices];

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;
	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	pIndices[12] = 8;
	pIndices[13] = 9;
	pIndices[14] = 10;
	pIndices[15] = 8;
	pIndices[16] = 10;
	pIndices[17] = 11;

	pIndices[18] = 12;
	pIndices[19] = 13;
	pIndices[20] = 14;
	pIndices[21] = 12;
	pIndices[22] = 14;
	pIndices[23] = 15;

	pIndices[24] = 16;
	pIndices[25] = 17;
	pIndices[26] = 18;
	pIndices[27] = 16;
	pIndices[28] = 18;
	pIndices[29] = 19;

	pIndices[30] = 20;
	pIndices[31] = 21;
	pIndices[32] = 22;
	pIndices[33] = 20;
	pIndices[34] = 22;
	pIndices[35] = 23;

	memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Cube");
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Cube");
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
