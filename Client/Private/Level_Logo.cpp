#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI.h"


CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{


}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//잠시 비활성화
	///* ������ �����Ͽ� �纻�� �����. */
	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//Start 배경
	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_LOGO;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_StartScene_Background");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.3f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 0.1f;


	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, UI_START_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Start 로고
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_StartScene_Logo");
	UIDesc.fPosZ = 0.2f;
	UIDesc.bIsAnimation = false;

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, UI_START_LOGO,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	pGameInstance->Play_Sound(L"Ambience_Room6.wav", SOUND_BGM0, 0.3f);
	pGameInstance->Play_Sound(L"CarDriveby.wav", SOUND_BGM1, 0.3f);
	pGameInstance->Play_Sound(L"RainInside.wav", SOUND_BGM2, 0.3f);
	pGameInstance->Play_Sound(L"ThunderStrike.wav", SOUND_BGM3, 0.3f);
	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CLevel_Logo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (GetAsyncKeyState(VK_BACK) & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
		{
			Safe_Release(pGameInstance);
			return;
		}

		Safe_Release(pGameInstance);
	}
	else if (GetAsyncKeyState(VK_LBUTTON)  & 0x8000)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd,  &pt);
		if (1744 > pt.x && 1472 < pt.x && 481 < pt.y && 531 > pt.y)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			_uint Level = LEVEL_LOGO;
			pGameInstance->OnEvent(EVENT_START_OFF, &Level);
			//pGameInstance->Clear_Section(LEVEL_LOGO);
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_OUTDOOR))))
			{
				Safe_Release(pGameInstance);
				return;
			}
			
			Safe_Release(pGameInstance);
		}
		
	}

	else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		_uint Level = LEVEL_LOGO;
		pGameInstance->OnEvent(EVENT_START_OFF, &Level);
		//pGameInstance->Clear_Section(LEVEL_LOGO);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_OUTDOOR))))
		{
			Safe_Release(pGameInstance);
			return;
		}
		Safe_Release(pGameInstance);
	}

	//SetWindowText(g_hWnd, TEXT("�ΰ������Դϴ�."));
}

HRESULT CLevel_Logo::Render()
{
	//잠시 비활성화
	/*if (FAILED(__super::Render()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.BackGround */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, strLayerTag,
		TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);	

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLevel_Logo");
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->StopAll();
	Safe_Release(pGameInstance);
	__super::Free();


}
