#include "..\Public\VIBuffer.h"
#include "Picking.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_PrimitiveType(rhs.m_PrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVerticesPos(rhs.m_pVerticesPos)
	, m_pIndices(rhs.m_pIndices)
	
{	
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 어떤 소스(자원, 정점을) 를 이용해서 그릴껀K ? */
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);	
	m_pGraphic_Device->SetIndices(m_pIB);
	
	m_pGraphic_Device->DrawIndexedPrimitive(m_PrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);
	
	return S_OK;
}

_bool CVIBuffer::Intersect_Polygons(const _float4x4& WorldMatrix, _float3* pOut)
{
	CPicking*		pPicking = CPicking::Get_Instance();

	pPicking->Transform_ToLocalSpace(WorldMatrix);

	for (size_t i = 0; i < m_iNumPrimitive; i++)
	{
		if (D3DFMT_INDEX16 == m_eIndexFormat)
		{
			_ushort iIndices[3];

			memcpy(iIndices, (_ubyte*)m_pIndices + (i * m_iIndexStride * 3), m_iIndexStride * 3);

			if (true == pPicking->Intersect_Triangle_InLocal(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], pOut))
			{
				D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
				return true;
			}
		}
		else
		{
			_ulong iIndices[3];

			memcpy(iIndices, (_ubyte*)m_pIndices + (i * m_iIndexStride * 3), m_iIndexStride * 3);

			if (true == pPicking->Intersect_Triangle_InLocal(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], pOut))
			{
				D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
				return true;
			}
		}
	}

	return false;
	
}

_bool CVIBuffer::Intersect_Polygons_ForRay(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOut)
{
	CPicking* pPicking = CPicking::Get_Instance();

	_float3 vLocalRay, vLocalRayPos;
	pPicking->Transform_ToLocalSpace_ForRay(WorldMatrix,_ray,_rayPos,&vLocalRay,&vLocalRayPos);

	for (size_t i = 0; i < m_iNumPrimitive; i++)
	{
		if (D3DFMT_INDEX16 == m_eIndexFormat)
		{
			_ushort iIndices[3];

			memcpy(iIndices, (_ubyte*)m_pIndices + (i * m_iIndexStride * 3), m_iIndexStride * 3);

			if (true == pPicking->Intersect_Triangle_ForRay(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], vLocalRay, vLocalRayPos, pOut))
			{
				D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
				return true;
			}
		}
		else
		{
			_ulong iIndices[3];

			memcpy(iIndices, (_ubyte*)m_pIndices + (i * m_iIndexStride * 3), m_iIndexStride * 3);

			if (true == pPicking->Intersect_Triangle_ForRay(&m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]], vLocalRay, vLocalRayPos, pOut))
			{
				D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
				return true;
			}
		}
	}

	return false;
}

_bool CVIBuffer::Is_Vertex_In_Projection_Space(const _float3 vPosition, const _float4x4& WorldMatrix, const _float4x4& ViewMatrix, const _float4x4& ProjectionMatrix)
{
	const _float4x4 ProjectionSpaceMatrix = WorldMatrix * ViewMatrix * ProjectionMatrix;

	_float3 vProjectedPosition;
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		D3DXVec3TransformCoord(&vProjectedPosition, &m_pVerticesPos[i], &ProjectionSpaceMatrix);

		if (vProjectedPosition.x < 1.0f && vProjectedPosition.x > -1.0f
			&& vProjectedPosition.y < 1.0f && vProjectedPosition.y > -1.0f)
			return true;
	}

	D3DXVec3TransformCoord(&vProjectedPosition, &vPosition, &ProjectionSpaceMatrix);

	if (vProjectedPosition.x < 1.0f && vProjectedPosition.x > -1.0f
		&& vProjectedPosition.y < 1.0f && vProjectedPosition.y > -1.0f)
		return true;

	return false;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	/* 정점(Vertex) 배열을 할당한다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 인덱스(정점의 Index) 배열을 할당한다. */
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;

	
}

CComponent* CVIBuffer::Clone(void* pArg)
{
	return nullptr;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pIndices);
	}
  
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}
