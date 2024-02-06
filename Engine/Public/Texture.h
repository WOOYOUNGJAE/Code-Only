#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	/* TYPE_GENERAL : LPDIRECT3DTEXTURE9 */
	/* TYPE_CUBE : LPDIRECT3DCUBETEXTURE9 */
	enum TYPE { TYPE_GENERAL, TYPE_CUBE, TYPE_END };
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_OnGraphicDevice(_uint iTextureIndex = 0);
	_int Get_TextureCount();

	IDirect3DBaseTexture9* Get_Texture(_uint iTextureIndex = 0) { return m_Textures[iTextureIndex]; }

private:
	_uint											m_iNumTextures = { 0 };
	vector<IDirect3DBaseTexture9*>					m_Textures;
	TYPE											m_eType = { TYPE_END };

public:
	/* Texture_0.png, 1 */
	/* Texture_%d.png, 2 */
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END