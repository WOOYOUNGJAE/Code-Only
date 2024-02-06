#include "stdafx.h"
#include "..\Public\MainApp.h"

#include <FSM.h>

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Fixed.h"
#include "ProgressableUI.h"
#include "GimmickManager.h"
#include "EffectManager.h"
CMainApp::CMainApp()	
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
#ifdef _DEBUG

#endif

	GRAPHICDESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iSizeX = g_iWinSizeX;
	GraphicDesc.iSizeY = g_iWinSizeY;
	GraphicDesc.eWinMode = GRAPHICDESC::WINMODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Event_System(EVENT_END)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_ForStatic()))
		return E_FAIL;

	if (FAILED(Ready_Loading_Scene()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_OUTDOOR)))
		return E_FAIL;

	m_pGimmickManager = CGimmickManager::Get_Instance();
	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Render_Begin(D3DXCOLOR(0.0f, 0.f, 0.f, 0.f));

	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Render_End();
	
	return S_OK;
}


HRESULT CMainApp::Ready_Default_Setting()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMainApp::Ready_Loading_Scene()
{
	//배경화면 UI를 그리기 위한 고정 UI.
	if (FAILED(m_pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Fixed"),
		CUI_Fixed::Create(m_pGraphic_Device))))
		return E_FAIL;

	//배경화면 진행도를 그리기 위한 프로그레스 바.
	if (FAILED(m_pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Progress"),
		CProgressableUI::Create(m_pGraphic_Device))))
		return E_FAIL;


	//로딩화면 배경텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene.png")))))
		return E_FAIL;

	//로딩화면 프로그레스바 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_ProgressBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene_ProgressBar.png")))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eNextLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eNextLevel))))
		return E_FAIL;

	return S_OK;
}


HRESULT CMainApp::Ready_Prototype_Component_ForStatic()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Quad */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Quad"),
		CVIBuffer_Quad::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Billboard */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Billboard"),
		CVIBuffer_Billboard::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Grid */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Grid"),
		CVIBuffer_Grid::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cylinder_64 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cylinder_64"),
		CVIBuffer_Cylinder::Create(m_pGraphic_Device, 64, 4.f, 1.f))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cylinder_8 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cylinder_8"),
		CVIBuffer_Cylinder::Create(m_pGraphic_Device, 8, 1.f, 1.f))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Circle_8 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Circle_8"),
		CVIBuffer_Circle::Create(m_pGraphic_Device, 8))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sphere"),
		CVIBuffer_Sphere::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_ProgressableRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Progress"),
		CVIBuffer_ProgressableRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_FlowableRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flow"),
		CVIBuffer_FlowableRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Mask"),
		CVIBuffer_Mask::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_BlinkableRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Blink"),
		CVIBuffer_BlinkableRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_FSM */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FSM"),
		CFSM::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Physics */
  	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		CPhysics::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Debug */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Misc/Debug%d.png"),2))))
		return E_FAIL;

	

	

	Safe_AddRef(m_pRenderer);
	
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CMainApp");
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pGameInstance);

	
	Safe_Release(m_pGimmickManager);
	
	CGameInstance::Release_Engine();
}
