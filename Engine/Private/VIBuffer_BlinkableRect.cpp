#include "VIBuffer_BlinkableRect.h"

CVIBuffer_BlinkableRect::CVIBuffer_BlinkableRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer_Rect(pGraphic_Device), m_hBaseTexHandle{}, m_hBaseTexDesc{}, m_hRatio{}
{
}

CVIBuffer_BlinkableRect::CVIBuffer_BlinkableRect(const CVIBuffer_BlinkableRect& rhs)
	: CVIBuffer_Rect(rhs), m_hBaseTexHandle{ rhs.m_hBaseTexHandle }, m_hBaseTexDesc{ rhs.m_hBaseTexDesc }
	, m_hRatio(rhs.m_hRatio)
{
}

HRESULT CVIBuffer_BlinkableRect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_BlinkableRect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Create_VertexShader();
	Create_PixelShader();

	Initialize_ConstantTable();

	return S_OK;
}

HRESULT CVIBuffer_BlinkableRect::Render()
{
	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformWorldHandle,
		&m_WorldMatrix);

	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformViewHandle,
		&m_ViewMatrix);

	m_pVertexConstantTable->SetMatrix(
		m_pGraphic_Device,
		m_hTransformProjectionHandle,
		&m_ProjectionMatrix);

	m_pGraphic_Device->SetTexture(m_hBaseTexDesc.RegisterIndex, m_pTexture);

	m_pPixelConstantTable->SetFloat(
		m_pGraphic_Device,
		m_hRatio,
		m_fRatio
	);

	// Vertex
	m_pGraphic_Device->SetVertexShader(m_pVertexShader);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);

	// Pixel
	m_pGraphic_Device->SetPixelShader(m_pPixelShader);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	m_pGraphic_Device->SetVertexShader(nullptr);
	m_pGraphic_Device->SetPixelShader(nullptr);

	return S_OK;
}

HRESULT CVIBuffer_BlinkableRect::Create_VertexShader()
{
	ID3DXBuffer* pVertexShader = nullptr;
	ID3DXBuffer* errorBuffer = nullptr;

	HRESULT hr = D3DXCompileShaderFromFile(
		L"../Shader/UIVertexShader.hlsl",
		0,
		0,
		"main",
		"vs_3_0",
		D3DXSHADER_DEBUG,
		&pVertexShader,
		&errorBuffer,
		&m_pVertexConstantTable);

	// output any error messages
	if (errorBuffer)
	{
		::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return E_FAIL;
	}

	hr = m_pGraphic_Device->CreateVertexShader(
		(DWORD*)pVertexShader->GetBufferPointer(),
		&m_pVertexShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return E_FAIL;
	}

	pVertexShader->Release();


	return E_NOTIMPL;
}

HRESULT CVIBuffer_BlinkableRect::Create_PixelShader()
{
	ID3DXBuffer* pPixelShader = nullptr;
	ID3DXBuffer* errorBuffer = nullptr;

	HRESULT hr = D3DXCompileShaderFromFile(
		L"../Shader/BlinkPixelShader.hlsl",
		0,
		0,
		"main",
		"ps_3_0",
		D3DXSHADER_DEBUG,
		&pPixelShader,
		&errorBuffer,
		&m_pPixelConstantTable);

	// output any error messages
	if (errorBuffer)
	{
		::MessageBox(0, (wchar_t*)errorBuffer->GetBufferPointer(), 0, 0);
		errorBuffer->Release();
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return E_FAIL;
	}

	hr = m_pGraphic_Device->CreatePixelShader(
		(DWORD*)pPixelShader->GetBufferPointer(),
		&m_pPixelShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreatePixelShader - FAILED", 0, 0);
		return E_FAIL;
	}

	pPixelShader->Release();


	return E_NOTIMPL;
}

void CVIBuffer_BlinkableRect::Initialize_ConstantTable()
{
	m_hTransformWorldHandle = m_pVertexConstantTable->GetConstantByName(0, "worldMatrix");
	m_hTransformViewHandle = m_pVertexConstantTable->GetConstantByName(0, "viewMatrix");
	m_hTransformProjectionHandle = m_pVertexConstantTable->GetConstantByName(0, "projectionMatrix");

	m_hBaseTexHandle = m_pPixelConstantTable->GetConstantByName(0, "g_Sampler");

	m_hRatio = m_pPixelConstantTable->GetConstantByName(0, "g_Ratio");

	UINT count;
	m_pPixelConstantTable->GetConstantDesc(m_hBaseTexHandle, &m_hBaseTexDesc, &count);

	m_pVertexConstantTable->SetDefaults(m_pGraphic_Device);
	m_pPixelConstantTable->SetDefaults(m_pGraphic_Device);
}

CVIBuffer_BlinkableRect* CVIBuffer_BlinkableRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BlinkableRect* pInstance = new CVIBuffer_BlinkableRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CVIBuffer_BlinkableRect");
	}

	return pInstance;
}

CComponent* CVIBuffer_BlinkableRect::Clone(void* pArg)
{
	CVIBuffer_BlinkableRect* pInstance = new CVIBuffer_BlinkableRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CVIBuffer_BlinkableRect");
	}

	return pInstance;
}

void CVIBuffer_BlinkableRect::Free()
{
	__super::Free();

	Safe_Release(m_pVertexShader);
	Safe_Release(m_pPixelShader);

	Safe_Release(m_pVertexConstantTable);
	Safe_Release(m_pPixelConstantTable);
}
