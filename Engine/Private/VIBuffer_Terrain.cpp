#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)

{

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iStride = sizeof(VTXPOSTEX);	
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);	
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;



	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX*			pVertices = { nullptr };

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 30.f, i / (m_iNumVerticesZ - 1.f) * 30.f);
		}
	}
	

	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort*		pIndices = { nullptr };

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
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong*		pIndices = { nullptr };

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;
		pIndices[3] = 0;
		pIndices[4] = 2;
		pIndices[5] = 3;
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const wstring & strHeightMap)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	HANDLE		hFile = CreateFile(strHeightMap.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong					dwByte = { 0 };

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_ulong*			pPixel = new _ulong[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iStride = sizeof(VTXPOSTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;



	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX*			pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3((_float)j, (pPixel[iIndex] & 0x000000ff) / 15.f, (_float)i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 30.f, i / (m_iNumVerticesZ - 1.f) * 30.f);
		}
	}


	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort*		pIndices = { nullptr };
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
		_ulong*		pIndices = { nullptr };

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

		memcpy(m_pIndices, pIndices, sizeof(_ulong) * m_iNumIndices);
	}

	m_pIB->Unlock();

	Safe_Delete_Array(pPixel);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vWorldPos, const _float4x4& OwnerWorldMatrix)
{
	/* 비교하고자하는 객체의 월드위치르 지형 정점의 로컬로 옮긴다. */
	_float3			vLocalPos;

	_float4x4		OwnerWorldMatrixInv = *D3DXMatrixInverse(&OwnerWorldMatrixInv, nullptr, &OwnerWorldMatrix);
	D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &OwnerWorldMatrixInv);

	/* 어떤 넼모안에 있는지 구하낟. */
	_uint		iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	/* 네모의 네개 코너 인덱스를 다 구한다. */
	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1, 
		iIndex
	};

	/* 오른쪽위 or 왼쪽아래 삼각형이냐를 판단하낟. */
	_float		fWidth = vLocalPos.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vLocalPos.z;

	D3DXPLANE			Plane;

	/*& 오느쪽 위ㅣ. */
	if (fWidth > fDepth)
	{
		//D3DXPlaneFromPointNormal();
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], 
			&m_pVerticesPos[iIndices[1]],
			&m_pVerticesPos[iIndices[2]]);
	}
	/* 외;ㄴ쪽 하단. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]],
			&m_pVerticesPos[iIndices[2]],
			&m_pVerticesPos[iIndices[3]]);
	}	

	// ax + by + cz + d == 0

	// y = (-ax - cz - d) / b;

	vLocalPos.y = (-Plane.a * vLocalPos.x - Plane.c * vLocalPos.z - Plane.d) / Plane.b;

	return (*D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, &OwnerWorldMatrix)).y;
	
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring & strHeightMap)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(strHeightMap)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();



}
