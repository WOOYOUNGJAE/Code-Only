#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)



private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev(void) { return m_pGraphicDev; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, GRAPHICDESC::WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppOut);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9			m_pSDK = { nullptr } ;
	LPDIRECT3DDEVICE9	m_pGraphicDev = { nullptr };

private:
	virtual void	Free(void)override;
};

END
