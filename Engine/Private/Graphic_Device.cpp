#include "../Public/Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device() 
	: m_pSDK { nullptr }
	, m_pGraphicDev { nullptr } 
{

}

HRESULT	CGraphic_Device::Ready_GraphicDev(HWND hWnd, GRAPHICDESC::WINMODE eMode, _uint iSizeX, _uint iSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	_ulong	dwFlag = 0;

	/* VERTEXPROCESSING : 정점의 변환, 조명. */

	if (DeviceCaps.DevCaps &D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iSizeX;
	d3dpp.BackBufferHeight = iSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
		return E_FAIL;

	*ppOut = m_pGraphicDev;

	Safe_AddRef(m_pGraphicDev);


	return S_OK;
}

void CGraphic_Device::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		Color, 1.f, 0);
	m_pGraphicDev->BeginScene();
}

void CGraphic_Device::Render_End(void)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

void CGraphic_Device::Free(void)
{
	if (0 != Safe_Release(m_pGraphicDev))
	{
		MSG_BOX("Failed to Released");
	}
	Safe_Release(m_pSDK);
}