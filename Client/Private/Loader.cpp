#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Camera_Player.h"
#include "Terrain.h"
#include "Player.h"
#include "Monster.h"
#include "UI_Fixed.h"
#include "Text.h"
#include "UI_Unfixed.h"
#include "FSM.h"
#include "Environment.h"
#include "InteractiveObject.h"
#include "Item_Interactive.h"
#include "Item_Equipment.h"
#include "Item_Consumable.h"
#include "Item_ThrowOnly.h"
#include "Inventory.h"
#include "Effect.h"
#include "PipeGuy.h"
#include "ProgressableUI.h"
#include "Gimmick_Object.h"
#include "BookShelf.h"
#include "GimickWall.h"
#include "Train.h"
#include "Gore_Body.h"
#include "Boss.h"
#include "Particle.h"

#pragma region COMPONENT

#pragma endregion

#pragma region GAMEOBJECT
#include "BackGround.h"
#include "Goon.h"
#include "BackgroundCylinder.h"
#include "Cloud.h"
#include "Smoke.h"
#include "CircleMask.h"
#include "Event_Sphere.h"
#include "Delia.h"
#pragma endregion

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadEntry(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->LoadingForNextLevel()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//Safe_AddRef(pGameInstance);

	////배경화면 UI를 그리기 위한 고정 UI.
	//if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Fixed"),
	//	CUI_Fixed::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	////배경화면 진행도를 그리기 위한 프로그레스 바.
	//if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Progress"),
	//	CProgressableUI::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	////로딩화면 배경텍스쳐
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_Background"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene.png")))))
	//	return E_FAIL;

	////로딩화면 프로그레스바 텍스쳐
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Texture_LoadingScene_ProgressBar"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene_ProgressBar.png")))))
	//	return E_FAIL;

	////프로그레스바
	//CUI::UIDESC UIDesc;
	//UIDesc.iLevelIndex = LEVEL_LOADING;
	//UIDesc.iLevelStatic = LEVEL_STATIC;
	//UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_LoadingScene_ProgressBar");
	//UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	//UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	//UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	//UIDesc.fPaddingX = 0.0f;
	//UIDesc.fPaddingY = 0.0f;
	//UIDesc.fSizeX = 1920.f;
	//UIDesc.fSizeY = 1080.f;
	//UIDesc.fPosZ = 0.0f;
	//UIDesc.bIsRender = false;
	//UIDesc.bIsFixed = true;
	//UIDesc.eAnchor = CUI::CENTER;
	//UIDesc.eRender = CUI::RENDER_TEST;
	//UIDesc.iWindowSizeX = g_iWinSizeX;
	//UIDesc.iWindowSizeY = g_iWinSizeY;
	//UIDesc.bIsAnimation = false;
	//UIDesc.fAnimationRatio = 1.0f;

	//if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING, UI_LOADING_PROGRESSBAR,
	//	TEXT("Prototype_UI_Progress"), &UIDesc)))
	//	return E_FAIL;


	////배경
	//UIDesc.iLevelIndex = LEVEL_LOADING;
	//UIDesc.iLevelStatic = LEVEL_STATIC;
	//UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_LoadingScene_Background");
	//UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	//UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	//UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	//UIDesc.fPaddingX = 0.0f;
	//UIDesc.fPaddingY = 0.0f;
	//UIDesc.bIsFixed = true;
	//UIDesc.fSizeX = 1920.f;
	//UIDesc.fSizeY = 1080.f;
	//UIDesc.fPosZ = 0.01f;
	//UIDesc.bIsRender = false;
	//UIDesc.eAnchor = CUI::CENTER;
	//UIDesc.iWindowSizeX = g_iWinSizeX;
	//UIDesc.iWindowSizeY = g_iWinSizeY;
	//UIDesc.bIsAnimation = false;
	//UIDesc.fAnimationRatio = 1.0f;
	//UIDesc.eRender = CUI::RENDER_TEST;

	//if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING, UI_LOADING_BACKGROUND,
	//	TEXT("Prototype_UI_Fixed"), &UIDesc)))
	//	return E_FAIL;

	//Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::LoadingForNextLevel()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = { 0 };

   	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_ForLogo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_ForGamePlay();
		break;
	case LEVEL_WYJ:
		hr = Loading_ForWYJ();
		break;
	case LEVEL_OUTDOOR:
		hr = Loading_ForOutdoor();
		break;
	case LEVEL_2F:
		hr = Loading_For2F();
		break;
	}

	if (FAILED(hr))
		goto except;

	LeaveCriticalSection(&m_CriticalSection);
	return S_OK;

except:
	LeaveCriticalSection(&m_CriticalSection);
	return E_FAIL;
}

HRESULT CLoader::Loading_ForLogo()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	LOADINGDESC LoadingDesc;
	LoadingDesc.Level_Loading = LEVEL_LOADING;
	LoadingDesc.Loading_Success_Count = 0;
	LoadingDesc.Loading_All_Count = 660;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	
  	lstrcpy(m_szLoading, TEXT("텍스쳐를 로딩 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 2;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//StartScene 배경
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartScene_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Start/Background/%d.jpg"), 603))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 603;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//StartScene 배경
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartScene_Logo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Start/Logo.png")))))
		return E_FAIL;
	
	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("모델을 로딩 중입니다."));


	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("셰이더를 로딩 중입니다."));


	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("객체원형 로딩 중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 54;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	

	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlay()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로딩 중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Player/Player%d.png"), 103))))
		return E_FAIL;

	
	lstrcpy(m_szLoading, TEXT("모델을 로딩 중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	
	lstrcpy(m_szLoading, TEXT("셰이더를 로딩 중입니다."));

	lstrcpy(m_szLoading, TEXT("객체원형 로딩 중입니다."));
	/* For.Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	
	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다."));

	Safe_Release(pGameInstance);

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_ForWYJ()
{	return S_OK;}

HRESULT CLoader::Loading_ForOutdoor()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	LOADINGDESC LoadingDesc;
	LoadingDesc.Level_Loading = LEVEL_LOADING;

	LoadingDesc.Loading_All_Count = 1606;

	LoadingDesc.Loading_Success_Count = 0;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로딩 중입니다."));

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section1.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section2.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section3.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section4.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section5.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section6.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section_2F.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section_1F.lik"), LEVEL_OUTDOOR);
	LoadingDesc.Loading_Success_Count = 50;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Gore Textures */
	if (FAILED(Loading_Gore_Textures()))
	{
		return E_FAIL;
	}

	// 임시
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Ending"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Demo_End_Screen.png")))))
		return E_FAIL;
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ending"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Player/Player%d.png"), 103))))
		return E_FAIL;
	LoadingDesc.Loading_Success_Count = 77;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Sky/skybox.png")))))
		return E_FAIL;
	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Sky/cloud.png")))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_BookShelf_Move */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BookShelf_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Furniture/bookshelf_move.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Gimmick_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Wall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Wall/Gimmick_Wall.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Train_Front */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Train_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Train/streetfront.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Train_Side */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Train_Side"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Train/streetcarside.png")))))
		return E_FAIL;

	// For.Prototype_Component_Texture_UI_HudBox
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/PlayerHudBox_Main.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	//430


	// For.Prototype_Component_Texture_UI_HealthBar_Player_Brown
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HealthBar_Player_Brown"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Player/HealthBar_Player_Brown.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HealthBar_Player_Red
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HealthBar_Player_Red"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Player/HealthBar_Player_Red.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Player_BackGround
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Player_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Player/Background_Player.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HealthBar_Enemy_DarkRed
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HealthBar_Enemy_DarkRed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Enemy/HealthBar_Enemy_DarkRed.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HealthBar_Enemy_Red
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HealthBar_Enemy_Red"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Enemy/HealthBar_Enemy_Red.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Goon_BackGround
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Goon_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Enemy/Background_Goon.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Mook_BackGround
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Mook_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Enemy/Background_Pipe.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Malone_BackGround
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Malone_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/Enemy/Background_Malone.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//// For.Prototype_Component_Texture_UI_Text_Transparent_BackGround
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Text_Transparent_BackGround"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ProgressBar/TextEmptyBackGround.png")))))
	//	return E_FAIL;

	//438

	// For.Prototype_Component_Texture_UI_Tooltip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Tooltip"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Tooltip/Tooltip%d.png"), 16))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 16;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//454
	
	// For.Prototype_Component_Texture_UI_RadioBase
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_RadioBase"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Radio/RadioBase/RadioBase%d.png"), 40))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 40;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//494

	// For.Prototype_Component_Texture_UI_HudBox_HPNum
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_HPNum"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HPNum/Num%d.png"), 10))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 10;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//504

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Healthy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Healthy"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/Healthy/MikeFace_Healthy%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Slightly_Hurt
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Slightly_Hurt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/SlightlyHurt/MikeFace_SlightlyHurt%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Hurt
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hurt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/Hurt/MikeFace_Hurt%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Moderately_Hurt
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Moderately_Hurt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/ModeratelyHurt/MikeFace_ModeratelyHurt%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Badly_Hurt
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Badly_Hurt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/BadlyHurt/MikeFace_BadlyHurt%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//524

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Pain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Pain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/Pain/MikeFace_Pain%d.png"), 5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Dead
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Dead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/MikeFace_Dead.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_PartialHidden
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_PartialHidden"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/MikeFace_PartialHidden.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Mike_Hidden
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hidden"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/HudBox_Mike/MikeFace_Hidden.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_SelectBox
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_SelectBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/Item/Etc/PlayerHudBox_Select.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Inventory
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Inventory"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/Item/InventoryIcon%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	
	//548
	
	// For.Prototype_Component_Texture_UI_HudBox_Inventory_Broken
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/Item/Etc/InventoryIcons_broken.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Item_HealthBar
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/Item/Etc/HealthBar_Item.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_HudBox_Item_HealthBar_BackGround
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HudBox/Item/Etc/HealthBar_ItemBackground.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Buff_Heal
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Buff_Heal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Buff/Heal/Heal%d.png"), 5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Buff_Stamina
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Buff_Stamina"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Buff/Stamina/Stamina%d.png"), 20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_CrossHair_Default
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_CrossHair_Default"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Crosshair/Crosshair.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_CrossHair_Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_CrossHair_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Crosshair/Crosshair_Attack/CrosshairSpinning%d.png"), 12))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 12;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//589

	// For.Prototype_Component_Texture_UI_ObjectiveComplete
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_ObjectiveComplete"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/ObjectiveComplete.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_EnemyArrow
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_EnemyArrow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/EnemyArrow/LosIndicator_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_CrossHair_InDistance
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_CrossHair_InDistance"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Crosshair/CrosshairHitDistanceIndicator.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Text
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Text"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Talk/Talk%d.png"),15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//614
	

	// For.Prototype_Component_Texture_UI_Prompter
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Prompter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Prompter/Prompter%d.png"),64))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 64;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//678

	// For.Prototype_Component_Texture_UI_CrossHair_Charging
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_CrossHair_Charging"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Crosshair/Charging/Crosshair_Charging%d.png"), 34))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 34;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//712

	// For.Prototype_Component_Texture_UI_Map
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Map"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Map/Map%d.png"), 11))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 10;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//// For.Prototype_Component_Texture_UI_CrossHair_HeavyReady
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_CrossHair_HeavyReady"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Crosshair/CrosshairHeavyReady.png")))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BloodImpactStrip"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/BloodImpactStrip/BloodImpactStrip_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//736

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Pow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/Pow/Pow_%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Strip"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/Strip/Strip_256x256_%d.png"), 17))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 17;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smoke_Strip"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/smoke/Strip%d.png"), 17))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 17;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smashed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/Smashed/Smashed.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_DazeSwirl"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/DazeSwirl/DazeSwirl_%d.png"),4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	//762
	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_GrossDebris"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/GrossDebris/GrossDebris_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_SmokeImpactUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/SmokeImpactUp/SmokeImpactUp_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 18;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smoking"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/Smoking/Smoking_%d.png"), 16))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 16;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_SparkStrip"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/SparkStrip/SparkStrip_256x256_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/WoodDebrisSprites/WoodDebrisSprites_%d.png"), 11))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 11;


	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BloodMist"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/BloodMist/BloodMist_154x151_%d.png"), 17))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 17;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BulletHoles"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Effect/BulletHoles/BulletHoles_5.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// For.Prototype_Component_Texture_UI_Weapon_AutoPistol
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_AutoPistol"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/AutoPistol.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_Bottle
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_Bottle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Bottle.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_Bottle_Broken
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Broken"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Bottle_Broken.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_LeadPipe
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/LeadPipe.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_LeadPipe_Broken
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe_Broken"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/LeadPipe_Broken.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_TommyGun
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_TommyGun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/TommyGun.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_Bottle_Background
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Background/Bottle_Vertical_Background.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_Bottle_Broken_Background
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Broken_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Background/Bottle_Horizontal_Background.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_Pistol_Bullet
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_Pistol_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Bullet/PistolBullet.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_TommyGun_Bullet
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_TommyGun_Bullet"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Bullet/TommyGunBullet.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Weapon_LeadPipe_Background
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Weapon/Background/LeadPipe_Background.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Scene_Change
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Scene_Change"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/SceneChange/SceneChange%d.png"),5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Loading_Scene_Background
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Loading_Scene_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_UI_Loading_Scene_ProgressBar
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Loading_Scene_ProgressBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/LoadingScene/LoadingScene_ProgressBar.png")))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// For.Prototype_Component_Texture_UI_Loading_Scene_ProgressBar
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_DeadSceneOnce"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/DeadOnce/Blood%d.png"),99))))
		return E_FAIL;

	// For.Prototype_Component_Texture_UI_Loading_Scene_ProgressBar
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_DeadSceneLoop"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/DeadLoop/Dead%d.png"), 19))))
		return E_FAIL;

	//780

	// For.Prototype_Component_Texture_UI_Notification
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UI_Notification"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Notification/Noti%d.png"),21))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 21;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Guard_Bar
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Guard_Bar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Guard/Guard_Bar.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Guard_Background
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Guard_Background"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/Guard/Guard_Background.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Green
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Green"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HP/Green_Effect%d.png"),20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Red
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Red"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/UI/HP/Red_Effect%d.png"),20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_Dead
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodDebris"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Item_Dead/WoodDebris.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AutoPistol_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/AutoPistol_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TommyGun_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/TommyGun_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Banana_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Banana_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BananaPeel_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/BananaPeel_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bottle_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Bottle_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_CardboardBoxes_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/CardboardBoxes_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Apple_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Food_Apple_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Food_Cola_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Food_Cola_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Keys_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Keys_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Medkit_#7192_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Medkit_#7192_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_ScrunchedPaper_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/ScrunchedPaper_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_ScrunchedPaper_1_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/ScrunchedPaper_1_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Toolbox_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Toolbox_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashbin_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/trashbin_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_trashcan_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/trashcan_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_wallholepipe_pipe_H_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/wallholepipe_pipe_H_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_wallholepipe_pipe_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/wallholepipe_pipe_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodenBarrel_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/WoodenBarrel_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_2_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_2_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_4_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_4_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_5_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_5_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_6_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_6_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_7_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_7_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_8_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_8_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_9_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_9_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_10_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_10_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_13_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_13_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PoolBall_14_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PoolBall_14_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BloodyHandPrint_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/BloodyHandPrint_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Red_Button_0_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Red_Button_0_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PadLockChain_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PadLockChain_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_OilBarrel_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/OilBarrel_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PaintCans_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/PaintCans_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TablePlant2_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/TablePlant2_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TumblerGlass_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/TumblerGlass_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Potplant_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/Potplant_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_UmbrellaStand_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/UmbrellaStand_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_CoatHanger_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/CoatHanger_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BarStool_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/BarStool_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_TableLamp_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/TableLamp_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodenStool_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/WoodenStool_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_HandBag_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/HandBag_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Item_OutLine
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_AshtrayBinFallen_Outline"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Outline/AshtrayBinFallen_Outline.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// For.Prototype_Component_Texture_Fountain
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Fountain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Fountain/WaterFountain_289x176_%d.png"),10))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 10;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bloodpool"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/BloodPool.png")))))
		return E_FAIL;


	// For.Prototype_Component_Texture_Particle_Metal
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Particle_Metal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Particles/Metal/MetalShards_%d.png"), 9))))
		return E_FAIL;

	// For.Prototype_Component_Texture_Particle_Glass
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Particle_Glass"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Particles/Glass/GlassShards_%d.png"), 8))))
		return E_FAIL;

	// For.Prototype_Component_Texture_Particle_Wooden
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Particle_Wooden"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Particles/WoodenBarrel/WoodPlankDebris%d.png"), 5))))
		return E_FAIL;

	// For.Prototype_Component_Texture_Particle_Board
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Particle_Board"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Particles/Board/WoodDebrisSprites_%d.png"), 7))))
		return E_FAIL;

	//832

	lstrcpy(m_szLoading, TEXT("모델을 로딩 중입니다."));

	
	lstrcpy(m_szLoading, TEXT("셰이더를 로딩 중입니다."));


	
	lstrcpy(m_szLoading, TEXT("객체원형 로딩 중입니다."));

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Effect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
	//	CParticle::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Environment */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Environment"),
		CEnvironment::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_BackgroundCylinder */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackgroundCylinder"),
		CBackgroundCylinder::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Cloud */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cloud"),
		CCloud::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Smoke */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Smoke"),
		CSmoke::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CircleMask */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CircleMask"),
		CCircleMask::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_GameObject_Interactive */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Interactive"),
		CInteractiveObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Event_Sphere */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EventCollider"),
		CEvent_Sphere::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Item_Interactive */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Item_Interactive"),
		CItem_Interactive::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Item_Equipment*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Item_Equipment"),
		CItem_Equipment::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Item_Consumable */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Item_Consumable"),
		CItem_Consumable::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Item_ThrowOnly */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Item_ThrowOnly"),
		CItem_ThrowOnly::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_Inventory */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Inventory"),
		CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	/* For.Prototype_BookShelf */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_BookShelf"),
		CBookShelf::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GimmickWall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GimmickWall"),
		CGimickWall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Train"),
		CTrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Gore"),
		CGore_Body::Create(m_pGraphic_Device))))
		return E_FAIL;


#pragma region WYJ
	
// WYJ
	//GOON
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/North/Goon_Standing_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/NorthEast/Goon_Standing_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/NorthWest/Goon_Standing_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/East/Goon_Standing_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//850

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/West/Goon_Standing_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Goon_Standing_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthEast/Goon_Standing_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthWest/Goon_Standing_SouthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/North/PipeGuy_Standing_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/NorthEast/PipeGuy_Standing_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/NorthWest/PipeGuy_Standing_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/East/PipeGuy_Standing_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/West/PipeGuy_Standing_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/PipeGuy_Standing_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/SouthEast/PipeGuy_Standing_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/SouthWest/PipeGuy_Standing_SouthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Delia 
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_North.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH_EAST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_NorthEast.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH_WEST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_NorthWest.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_EAST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_East.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_WEST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_West.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_South.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH_EAST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_SouthEast.png")))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH_WEST"),
			CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Delia/Delia_SouthWest.png")))))
			return E_FAIL;

		LoadingDesc.Loading_Success_Count = 8;
		pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	}

	//BOSS 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Boss_Standing_North_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma region OnlySouth
	// Goon
	// Face Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FacePunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/FacePunched/Goon_FacePunched_South_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Body Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BodyPunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/BodyPunched/Goon_BodyPunched_South_%d.png"), 6))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 6;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	
	// Low Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_LowPunched"), // 1장짜리
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/LowPunched/Goon_LowPunched_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Default
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BattleDefault"), // 1장짜리
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Goon_BattleDefault.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// InchForward
 	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_InchForward"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/InchForward/Goon_InchForward_%d.png"), 6))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 6;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Taunt (도발)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Taunt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Taunt/Goon_Taunt_%d.png"), 6))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 6;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//890

	// South Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Strip/Goon_Strip_South_%d.png"), 24))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 24;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BasicAttack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/BasicAttack/BasicAttack_South_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Heavy Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeavyAttack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/HeavyAttack/HeavyAttack_South_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//930

	// Heavy Attack WindUp
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeavyAttackWindUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/HeavyAttack_Windup.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Attack Default
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_AttackDefault"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/BasicAttack/BasicAttack_South_%d.png"), 5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BattleIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/BattleIdle/BattleIdle_%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strafing
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strafing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Strafing/Strafing_%d.png"), 6))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 6;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingStart
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FallingStart"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/FallingStart/FallingStart_%d.png"), 5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Falling
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Falling"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Falling.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingEnd
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FallingEnd"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/FallingEnd/FallingEnd_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Dead (Dead_Lying)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Dead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Dead.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//960

	// Dazed
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Dazed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Dazed/Dazed_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Get Up
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_GetUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/GetUp/GetUp_%d.png"), 6))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 6;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//  Jump
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Jump/Jump_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// KarateChopped
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_KarateChopped"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/KarateChopped/KarateChopped_%d.png"), 18))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 18;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1006

	// PipeGuy
	// Face Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FacePunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/FacePunched/PipeGuy_FacePunched_South_%d.png"), 8))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// Body Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BodyPunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/BodyPunched/PipeGuy_BodyPunched_South_%d.png"), 3))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 3;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Low Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_LowPunched"), // 1장짜리
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/LowPunched/PipeGuy_LowPunched_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Default
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BattleDefault"), // 1장짜리
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/PipeGuy_BattleDefault.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// InchForward
 	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_InchForward"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/InchForward/PipeGuy_InchForward_%d.png"), 5))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// Taunt (도발)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Taunt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Taunt/PipeGuy_Taunt_%d.png"), 11))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 11;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// South Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Strip/PipeGuy_Strip_South_%d.png"), 19))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 19;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BasicAttack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/BasicAttack/BasicAttack_South_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Heavy Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeavyAttack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/HeavyAttack/HeavyAttack_South_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Heavy Attack WindUp
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeavyAttackWindUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/HeavyAttack_Windup.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Attack Default
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_AttackDefault"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/BasicAttack/BasicAttack_South_%d.png"), 5))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 5;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BattleIdle"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/BattleIdle/BattleIdle_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 2;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1085

	// Strafing
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strafing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Strafing/Strafing_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingStart
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FallingStart"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/FallingStart/FallingStart_%d.png"), 3))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 3;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Falling
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Falling"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Falling.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingEnd
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FallingEnd"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/FallingEnd/FallingEnd_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 2;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Dead (Dead_Lying)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Dead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Dead.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Dazed
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Dazed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Dazed/Dazed_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1113

	// Get Up
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_GetUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/GetUp/GetUp_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//  Jump
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Jump"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Jump/Jump_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// KarateChopped
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_KarateChopped"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/KarateChopped/KarateChopped_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 18;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion OnlySouth

#pragma region South
	//Goon
	// Run
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Run/Goon_Run_South_%d.png"), 21))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 21;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1167

	// South Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Goon_Cautious_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Headless
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Headless_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Headless/Headless_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 16;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeadlessEnd_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/South/Headless/Headless_15.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	// Run
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Run/PipeGuy_Run_South_%d.png"), 21))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 21;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// South Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/PipeGuy_Cautious_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// Headless
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Headless_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Headless/Headless_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 16;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeadlessEnd_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/South/Headless/Headless_17.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion South

#pragma region SouthEast
	// Goon
	// SouthEast Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthEast/Strip/Goon_Strip_SouthEast_%d.png"), 24))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 24;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// SouthEast Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthEast/Goon_Cautious_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	// SouthEast Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthEast/Strip/Goon_Strip_SouthEast_%d.png"), 24))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 24;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// SouthEast Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/SouthEast/PipeGuy_Cautious_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	//1240
#pragma endregion SouthEast

#pragma region SouthWest
	//Goon
	// SouthWest Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthWest/Strip/Goon_Strip_SouthWest_%d.png"), 24))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 24;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// SouthWest Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthWest/Goon_Cautious_SouthWest.png")))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// PipeGuy
	// SouthWest Strip (Walking)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/SouthWest/Strip/Goon_Strip_SouthWest_%d.png"), 24))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 24;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// SouthWest Suspicious
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/SouthWest/PipeGuy_Cautious_SouthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	//1290
#pragma endregion SouthWest
	
#pragma region East
	// East Suspicious
	// Goon
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/East/Goon_Cautious_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);


	// PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/East/PipeGuy_Cautious_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion East

#pragma region West
	// West Suspicious
	// Goon
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/West/Goon_Cautious_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/West/PipeGuy_Cautious_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion West

#pragma region NorthEast
	// Suspicious
	// Goon
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/NorthEast/Goon_Cautious_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/NorthEast/PipeGuy_Cautious_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion NorthEast

#pragma region NorthWest
	// Suspicious
	// Goon
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/NorthWest/Goon_Cautious_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/NorthWest/PipeGuy_Cautious_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

#pragma endregion NorthWest

#pragma region North
	// Suspicious
	// Goon
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Goon/North/Goon_Cautious_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// PipeGuy
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/PipeGuy/North/PipeGuy_Cautious_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1300
#pragma endregion North

#pragma region Indicator
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Alert"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/AwarenessIcon/AwarenessIcon_Alert.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Spooked"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/AwarenessIcon/Spooked/AwarenessIcon_Spooked_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Cautious"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/AwarenessIcon/Cautious/AwarenessIcon_Cautious_%d.png"), 15))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 15;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bubble"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/AwarenessIcon/SpeechBubble/Speech___%d.png"), 4))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);



#pragma endregion Indicator

#pragma region GimmickObject
	// Gimmick Fan
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Fan"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Fan/Gimmick_Fan_%d.png"), 3))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 3;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Entrance Key
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Key"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Key/Gimmick_Key_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 2;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Fence"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Gimmick_Fence.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);*/

	//// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Fence"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Gimmick_Fence_%d.png"),2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Wooden_Door"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Wooden_Door_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Wooden_Door_R"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Wooden_Door_R_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Metal_Door"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Metal_Door_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Metal_Door_R"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Metal_Door_R_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Fancy_Door"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Fancy_Door_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Fancy_Door_R"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Fancy_Door_R_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Club_Door"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Club_Door_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fence (Door)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Club_Door_R"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Door/Club_Door_R_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);



	// Blood Hand Paint
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Bloody_Hand_Paint"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/BloodyHandPrint/BloodyHandPrint_%d.png"),2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Blood Hand Paint
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Gimmick_Button"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gimmick/Button/Red_Button_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1337
#pragma endregion GimmickObject

#pragma region Boss
#pragma region BossEast
	//East
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/East/BasicAttack/BasicAttack_East_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/East/BattleIdle/Boss_Battle_Idle_Near_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/East/BattleIdle/Boss_Battle_Idle_Far_East.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/East/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_East"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/East/Strip/EastStrip_534x852_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion BossEast

#pragma region BossNorth
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/BasicAttack/BasicAttack_North_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/BattleIdle/Boss_Battle_Idle_Near_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/BattleIdle/Boss_Battle_Idle_Far_North.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_North"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/North/Strip/NorthStrip_351x876_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion BossNorth

#pragma region Boss_NorthEast
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthEast/BasicAttack/BasicAttack_NorthEast_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthEast/BattleIdle/Boss_Battle_Idle_Near_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthEast/BattleIdle/Boss_Battle_Idle_Far_NorthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthEast/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_NorthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthEast/Strip/NorthEastStrip_485x920_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_NorthEast

#pragma region Boss_NorthWest
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthWest/BasicAttack/BasicAttack_NorthWest_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthWest/BattleIdle/Boss_Battle_Idle_Near_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthWest/BattleIdle/Boss_Battle_Idle_Far_NorthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthWest/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_NorthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/NorthWest/Strip/NorthWestStrip_438_920_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_NorthWest

#pragma region Boss_SouthEast
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthEast/BasicAttack/BasicAttack_SouthEast_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthEast/BattleIdle/Boss_Battle_Idle_Near_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthEast/BattleIdle/Boss_Battle_Idle_Far_SouthEast.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthEast/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_SouthEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthEast/Strip/SouthEastStrip_451x940_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_SouthEast

#pragma region Boss_SouthWest
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthWest/BasicAttack/BasicAttack_SouthWest_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthWest/BattleIdle/Boss_Battle_Idle_Near_SouthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthWest/BattleIdle/Boss_Battle_Idle_Far_SouthWest.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthWest/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_SouthWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/SouthWest/Strip/SouthWest_503x940_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_SouthWest

#pragma region Boss_West
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/West/BasicAttack/BasicAttack_West_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/West/BattleIdle/Boss_Battle_Idle_Near_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/West/BattleIdle/Boss_Battle_Idle_Far_West.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/West/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_West"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/West/Strip/WestStrip_495x852_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_West

#pragma region Boss_OnlySouth
	// Basic Attack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/BasicAttack/BasicAttack_South_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Near
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/BattleIdle/Boss_Battle_Idle_Near_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Battle Idle Far
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/BattleIdle/Boss_Battle_Idle_Far_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Fire
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Fire/Fire_%d.png"), 9))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 9;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Strip
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Strip/SouthStrip_424x936_%d.png"), 29))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 29;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Body Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BodyPunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/BodyPunched/Boss_BodyPunched_South_%d.png"), 3))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 3;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Dazed
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Dazed"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Dazed/Dazed_484x890_%d.png"), 14))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 14;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Face Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FacePunched"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/FacePunched/FacePunched_585x915_%d.png"), 8))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 8;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingEnd
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FallingEnd"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/FallingEnd/FallingEnd_%d.png"), 2))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 2;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// FallingStart
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FallingStart"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/FallingStart/FallingStart_%d.png"), 3))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 3;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// GetUp
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_GetUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/GetUp/GetUp_%d.png"), 7))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 7;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Low Punched
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_LowPunched"), // 1장짜리
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/LowPunched/Boss_LowPunched_South.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Reloading
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Reloading"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Reloading/Reloading_502x889_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 18;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// RollEast
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollEast"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/RollEast/Roll_%d.png"), 20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// RollEast Reverse
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollEastReverse"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/RollEast_Reverse/RollReverse_%d.png"), 20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// RollWest
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollWest"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/RollWest/Roll_%d.png"), 20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// RollWest Reverse
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollWestReverse"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/RollWest_Reverse/RollReverse_%d.png"), 20))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 20;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Run
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Run_South"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Run/Running_377x849_%d.png"), 10))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 10;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Taunt
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Taunt"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Reloading/Reloading_502x889_%d.png"), 18))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 18;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Dead (Dead_Lying)
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Dead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Dead.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	// Falling
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Falling"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Enemy/Boss/South/Falling.png")))))
		return E_FAIL;

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
#pragma endregion Boss_OnlySouth


#pragma endregion



	// Enemy
	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Goon"), CGoon::Create(m_pGraphic_Device));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PipeGuy"), CPipeGuy::Create(m_pGraphic_Device));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss"), CBoss::Create(m_pGraphic_Device));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gimmick_Object"), CGimmick_Object::Create(m_pGraphic_Device));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Delia"), CDelia::Create(m_pGraphic_Device));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);

	//1340
#pragma endregion WYJ


	////For.Prototype_GameObject_UI_Fixed
	//if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Fixed"),
	//	CUI_Fixed::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	// TODO: Test Code~!~!~!~!~ 여기야 여기~~ 여기가 범인임!!
	//For.Prototype_GameObject_UI_Progress
	/*if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_Progress"),
		CProgressableUI::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Fixed"),
		CUI_Fixed::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	//�������
	//For.Prototype_GameObject_UI_HealthBar
	
	//체력바 원형 주석처리 디버그때 확인할 것.
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HealthBar"),
		CUI_Fixed::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	////For.Prototype_GameObject_UI_Text
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Text"),
	//	CText::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	//For.Prototype_GameObject_UI_Unfixed
	if (FAILED(pGameInstance->Add_Prototype_UI(TEXT("Prototype_UI_UnFixed"),
		CUI_Unfixed::Create(m_pGraphic_Device))))
		return E_FAIL;


	LoadingDesc.Loading_Success_Count = 122;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	//1500
	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다."));
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Unfixed"),
		CUI_Unfixed::Create(m_pGraphic_Device))))
		return E_FAIL;*/



	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For2F()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	LOADINGDESC LoadingDesc;
	LoadingDesc.Level_Loading = LEVEL_LOADING;
	LoadingDesc.Loading_Success_Count = 0;
	LoadingDesc.Loading_All_Count = 4;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("텍스쳐를 로딩 중입니다."));

	Loading_Textures(TEXT("../Bin/Resources/Maps/Section1.lik"), LEVEL_2F);

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("모델을 로딩 중입니다."));

	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("셰이더를 로딩 중입니다."));


	LoadingDesc.Loading_Success_Count = 1;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("객체원형 로딩 중입니다."));

	LoadingDesc.Loading_Success_Count =1 ;
	pGameInstance->OnEvent(EVENT_UPDATE_LOADING, &LoadingDesc);
	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다."));


	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Gore_Textures()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	/* For.Prototype_Component_Texture_BookShelf_Move */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Gore"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gore/Goon_Gore/Goon Gore_%d.png"), 21))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Gimmick_Wall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Pipeguy_Gore"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, TEXT("../Bin/Resources/Textures/Gore/Pipeguy_Gore/Pipeguy_Gore_%d.png"), 22))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Textures(const std::wstring& strPath, LEVEL eLevel)
{
	std::ifstream file(strPath, ios_base::in | ios_base::binary);

	if (file.is_open())
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		size_t iObjectCount;
		file.read((char*)&iObjectCount, sizeof(size_t));

		for (size_t i = 0; i < iObjectCount; ++i)
		{
			TCHAR strPrototypeTag[MAX_PATH] = L"";
			TCHAR strTexturePath[MAX_PATH] = L"";

			// Load Texture Path
			{
				size_t iPathLength;

				file.read((char*)&iPathLength, sizeof(size_t));
				file.read((char*)strTexturePath, iPathLength + 2);
			}

			// Load Tag
			{
				TCHAR strName[MAX_PATH];
				_int iNameLength;

				file.read((char*)&iNameLength, sizeof(_int));
				file.read((char*)strName, iNameLength + 2);

				swprintf_s(strPrototypeTag, MAX_PATH, TEXT("Prototype_Component_Texture_%s"), strName);
			}

			CTexture* pTexutre = CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, strTexturePath);
			if (FAILED(pGameInstance->Add_Prototype(eLevel, strPrototypeTag, pTexutre)))
			{
				Safe_Release(pTexutre);
				continue;
			}
		}

		file.close();
		Safe_Release(pGameInstance);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLoader");
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGraphic_Device);
}
