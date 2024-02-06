#include "VIBuffer_Grid.h"

CVIBuffer_Grid::CVIBuffer_Grid(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Grid::CVIBuffer_Grid(const CVIBuffer_Grid& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Grid::Initialize_Prototype()
{
	m_iNumVerticesX = 10;
	m_iNumVerticesZ = 10;

	// Init Vertex/Index Description
	{
		m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
		m_iStride = sizeof(VTXNORTEX);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
		m_PrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
		m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
		m_iNumIndices = m_iNumPrimitive * 3;
		m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	}

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 5.f, i / (m_iNumVerticesZ - 1.f) * 5.f);
			pVertices[iIndex].vNormal = _float3(0.f, 1.f, 0.f);
		}
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

		for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
		{
			for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
			{
				_uint		iIndex = i * m_iNumVerticesX + j;

				_uint		iIndices[4] = {
					iIndex + m_iNumVerticesX,
					iIndex + m_iNumVerticesX + 1,
					iIndex + 1,
					iIndex
				};

				pIndices[iNumIndices++] = iIndices[0];
				pIndices[iNumIndices++] = iIndices[1];
				pIndices[iNumIndices++] = iIndices[2];

				pIndices[iNumIndices++] = iIndices[0];
				pIndices[iNumIndices++] = iIndices[2];
				pIndices[iNumIndices++] = iIndices[3];
			}
		}

		memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong* pIndices = { nullptr };
		m_pIndices = new _ulong[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		_uint		iNumIndices = 0;

		for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
		{
			for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
			{
				_uint		iIndex = i * m_iNumVerticesX + j;

				_uint		iIndices[4] = {
					iIndex + m_iNumVerticesX,
					iIndex + m_iNumVerticesX + 1,
					iIndex + 1,
					iIndex
				};

				pIndices[iNumIndices++] = iIndices[0];
				pIndices[iNumIndices++] = iIndices[1];
				pIndices[iNumIndices++] = iIndices[2];

				pIndices[iNumIndices++] = iIndices[0];
				pIndices[iNumIndices++] = iIndices[2];
				pIndices[iNumIndices++] = iIndices[3];
			}
		}

		memcpy(m_pIndices, pIndices, sizeof(_ulong)* m_iNumIndices);
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Grid::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Grid* CVIBuffer_Grid::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Grid");
	}

	return pInstance;
}

CComponent* CVIBuffer_Grid::Clone(void* pArg)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Grid");
	}

	return pInstance;
}

void CVIBuffer_Grid::Free()
{
	__super::Free();
}
