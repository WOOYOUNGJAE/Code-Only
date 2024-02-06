#include "..\Public\CVIBuffer_Enemy.h"

CCVIBuffer_Enemy::CCVIBuffer_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CCVIBuffer_Enemy::CCVIBuffer_Enemy(const CCVIBuffer_Enemy & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CCVIBuffer_Enemy::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iNumVertices = 4;
	m_iStride = sizeof(VTXNORTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_PrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;
	m_iIndexStride = m_iNumVertices > 65000 ? sizeof(_ulong) : sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = m_iIndexStride == sizeof(_ushort) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;



	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX*			pVertices = { nullptr };
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);
	pVertices[0].vNormal = _float3(0.f, 0.f, -1.f);

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);
	pVertices[1].vNormal = _float3(0.f, 0.f, -1.f);

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.f);
	pVertices[2].vNormal = _float3(0.f, 0.f, -1.f);
	
	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.f);
	pVertices[3].vNormal = _float3(0.f, 0.f, -1.f);

	m_pVB->Unlock();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_eIndexFormat)
	{
		_ushort*		pIndices = { nullptr };
		m_pIndices = new _ushort[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;
		pIndices[3] = 0;
		pIndices[4] = 2;
		pIndices[5] = 3;

		memcpy(m_pIndices, pIndices, sizeof(_ushort) * m_iNumIndices);
	}

	else if (D3DFMT_INDEX32 == m_eIndexFormat)
	{
		_ulong*		pIndices = { nullptr };
		m_pIndices = new _ulong[m_iNumIndices];

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;
		pIndices[3] = 0;
		pIndices[4] = 2;
		pIndices[5] = 3;

		memcpy(m_pIndices, pIndices, sizeof(_ulong) * m_iNumIndices);
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CCVIBuffer_Enemy::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CCVIBuffer_Enemy * CCVIBuffer_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCVIBuffer_Enemy*	pInstance = new CCVIBuffer_Enemy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CCVIBuffer_Enemy");
	}

	return pInstance;
}

CComponent * CCVIBuffer_Enemy::Clone(void* pArg)
{
	CCVIBuffer_Enemy*	pInstance = new CCVIBuffer_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CCVIBuffer_Enemy");
	}

	return pInstance;
}

void CCVIBuffer_Enemy::Free()
{
	__super::Free();

}
