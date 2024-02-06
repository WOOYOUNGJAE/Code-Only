#include "..\Public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_Textures(rhs.m_Textures)
	, m_eType(rhs.m_eType)	
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Initialize_Prototype(TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	m_eType = eType;

	m_iNumTextures = iNumTextures;

	_tchar		szFullPath[MAX_PATH] = TEXT("");	
	m_Textures.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		IDirect3DBaseTexture9*		pTexture = { nullptr };

		wsprintf(szFullPath, pTextureFilePath, i);

		HRESULT			hr = { 0 };

		switch (m_eType)
		{
		case TYPE_GENERAL: 
			hr = D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;
		case TYPE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.push_back(pTexture);	
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_OnGraphicDevice(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);	
}

_int CTexture::Get_TextureCount()
{
	return m_Textures.size();
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*	pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, pTextureFilePath, iNumTextures)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CTexture");
	}

	return pInstance;
}

CComponent * CTexture::Clone(void* pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CTexture");
	}

	return pInstance;
}


void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

}
