#pragma once

#include "UI.h"
#include "Renderer.h"

BEGIN(Engine)
class ENGINE_DLL CFont abstract : public CUI
{
public:
	typedef struct tagUI_Text
	{
		wstring		strText;
		_int		Height;
		_uint		Width;
		_uint		Weight;
		byte		CharSet;
		_tchar		FaceName[LF_FACESIZE];
		D3DCOLOR    TextColor;
		CUI::UIDESC UIDesc;
	}TEXTDESC;

protected:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(const CFont& rhs);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	//LPDIRECT3DDEVICE9 m_pDevice = { nullptr };
	LPD3DXSPRITE m_pSprite = { nullptr };
	LPD3DXFONT m_pFont = { nullptr };
	D3DCOLOR    m_TextColor;
	wstring m_strText;
	_uint m_iWinSizeX, m_iWinSizeY;


public:
	virtual void Free() override;

};

END