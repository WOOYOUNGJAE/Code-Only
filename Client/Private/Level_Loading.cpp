#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#pragma region ENGINE_HEADER
#include "GameInstance.h"
#pragma endregion

#pragma region CLIENT_HEADER
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_WYJ.h"
#include "Level_Outdoor.h"
#include "Level_2F.h"
#include "UI_Fixed.h"
#include "ProgressableUI.h"
#pragma endregion


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	////배경화면 UI를 그리기 위한 고정 UI.
	//if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Fixed"),
	//	CUI_Fixed::Create(m_pGraphic_Device))))
	//{
	//	return E_FAIL;
	//}
	//	

	////배경화면 진행도를 그리기 위한 프로그레스 바.
	//if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Progress"),
	//	CProgressableUI::Create(m_pGraphic_Device))))
	//{
	//	Safe_Release(pGameInstance);
	//	return E_FAIL;
	//}


	////로딩화면 배경텍스쳐
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_Background"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene.png")))))
	//{
	//	Safe_Release(pGameInstance);
	//	return E_FAIL;
	//}
	////로딩화면 프로그레스바 텍스쳐
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_ProgressBar"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene_ProgressBar.png")))))
	//{
	//	Safe_Release(pGameInstance);
	//	return E_FAIL;
	//}
	//프로그레스바
	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_LOADING;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_LoadingScene_ProgressBar");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING, UI_LOADING_PROGRESSBAR,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	//배경
	UIDesc.iLevelIndex = LEVEL_LOADING;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_LoadingScene_Background");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.01f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING, UI_LOADING_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	
	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (nullptr == m_pLoader)

		return E_FAIL;

	m_eNextLevel = eNextLevel;

	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	

}

void CLevel_Loading::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
 		if (true != m_pLoader->is_Finished())
			return;

		//CGameInstance::Get_Instance()->Clear
		CLevel* pLevel = { nullptr };

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			pLevel = CLevel_Logo::Create(m_pGraphic_Device);
			break;
		case LEVEL_GAMEPLAY:
			pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
		case LEVEL_WYJ:
			pLevel = CLevel_WYJ::Create(m_pGraphic_Device);
			break;
		case LEVEL_OUTDOOR:
			pLevel = CLevel_Outdoor::Create(m_pGraphic_Device);
			break;
		case LEVEL_2F:
			pLevel = CLevel_2F::Create(m_pGraphic_Device);
			break;
		}

		if (nullptr == pLevel)
			return;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		//주석처리
		/*LOADINGDESC loadingdesc;
		loadingdesc.Level_Loading = LEVEL_LOADING;
		loadingdesc.IsFinished = true;
		loadingdesc.Loading_Success_Index = 0;
		pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &loadingdesc);*/

 		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
		{
			Safe_Release(pGameInstance);
			return;
		}

		Safe_Release(pGameInstance);
	}

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);	

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLevel_Loading");
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
