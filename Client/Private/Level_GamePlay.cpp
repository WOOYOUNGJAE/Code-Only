#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "UI.h"
#include "Font.h"
#include "UI_Unfixed.h"
#include "UI_Fixed.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI_HudBox(TEXT("Layer_UI_HudBox"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI_Player_Stamina(TEXT("Layer_UI_Player_Stamina"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI_Tooltip(TEXT("Layer_UI_Tooltip"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI_RadioBase(TEXT("Layer_UI_RadioBase"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	/*if (GetAsyncKeyState('1') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CUI_Fixed* pSelect1 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 18);
		CUI_Fixed* pSelect2 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 19);
		CUI_Fixed* pSelect3 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 20);

		pSelect1->Set_Render(true);
		pSelect2->Set_Render(false);
		pSelect3->Set_Render(false);
		Safe_Release(pGameInstance);
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CUI_Fixed* pSelect1 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 18);
		CUI_Fixed* pSelect2 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 19);
		CUI_Fixed* pSelect3 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 20);

		pSelect1->Set_Render(false);
		pSelect2->Set_Render(true);
		pSelect3->Set_Render(false);
		Safe_Release(pGameInstance);
	}
	if (GetAsyncKeyState('3') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CUI_Fixed* pSelect1 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 18);
		CUI_Fixed* pSelect2 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 19);
		CUI_Fixed* pSelect3 = (CUI_Fixed*)pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HudBox"), 20);

		pSelect1->Set_Render(false);
		pSelect2->Set_Render(false);
		pSelect3->Set_Render(true);
		Safe_Release(pGameInstance);
	}*/

	__super::Tick(fTimeDelta);
}

void CLevel_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("°Ô?ÓÇÃ·¹?Ì·¹º§?Ô´Ï´Ù."));
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Free::CAMERAFREEDESC		CameraFreeDesc;

	CameraFreeDesc.iData = 10;
	CameraFreeDesc.CameraDesc.iLevelIndex = LEVEL_STATIC;
	CameraFreeDesc.CameraDesc.strTranformTag = TEXT("Prototype_Component_Transform");
	CameraFreeDesc.CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraFreeDesc.CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraFreeDesc.CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraFreeDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraFreeDesc.CameraDesc.fNear = 0.2f;
	CameraFreeDesc.CameraDesc.fFar = 300.f;
	CameraFreeDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.0f;
	CameraFreeDesc.CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.Camera */
	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
	//	TEXT("Prototype_GameObject_Camera_Free"), &CameraFreeDesc)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_Camera_Player"), &CameraFreeDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Monster */
	for (size_t i = 0; i < 20; i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
			TEXT("Prototype_GameObject_Monster"))))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_UI_HudBox(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUI::UIDESC		UIDesc;

	//HUDBox
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 5.0f;
	UIDesc.fPaddingY = 14.0f;
	UIDesc.fSizeX = 314.f;
	UIDesc.fSizeY = 233.f;
	UIDesc.fPosZ = 0.4f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Ã¼·Â ¹é?Ç ?Ú¸®
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 60.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Ã¼·Â ½Ê?Ç ?Ú¸®
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 85.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Ã¼·Â ?Ï?Ç ?Ú¸®
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 110.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//ÁÖ?Î°ø ¾ó±¼µé
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Healthy");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 30.0f;
	UIDesc.fPaddingY = 76.0f;
	UIDesc.fSizeX = 142.f;
	UIDesc.fSizeY = 168.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//¾à°£ ¾ÆÇÂ ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Slightly_Hurt");
	UIDesc.bIsRender = false;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// ¾ÆÇÂ ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hurt");
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//²Ï ¾ÆÇÂ ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Moderately_Hurt");
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//½É°¢ÇÏ°Ô ¾ÆÇÂ ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Badly_Hurt");
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//°íÅë½º·¯¿î ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Pain");
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//¹ÝÂë ¼û?º ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_PartialHidden");
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//¿Ï?üÈ÷ ¼û?º ¾ó±¼
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hidden");
	////Å×½ºÆ®
	//UIDesc.bIsRender = true;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	// 1¹øÂ° ?Îº¥Åä¸®
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 192.0f;
	UIDesc.fPaddingY = 166.0f;
	UIDesc.fSizeX = 113.f;
	UIDesc.fSizeY = 63.f;
	UIDesc.fPosZ = 0.3f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.001f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//2¹øÂ° ?Îº¥Åä¸®
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory");
	UIDesc.fPosZ = 0.3f;
	UIDesc.fPaddingY = 98.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.001f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//3¹øÂ° ?Îº¥Åä¸®
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory");
	UIDesc.fPosZ = 0.3f;
	UIDesc.fPaddingY = 30.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.001f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//¼¿·ºÆ®¹Ú½º1

	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_SelectBox");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 190.0f;
	UIDesc.fPaddingY = 164.0f;
	UIDesc.fSizeX = 117.f;
	UIDesc.fSizeY = 66.f;
	UIDesc.fPosZ = 0.1f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//¼¿·ºÆ®¹Ú½º2
	UIDesc.fPaddingY = 96.0f;
	UIDesc.bIsRender = false;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//¼¿·ºÆ®¹Ú½º3
	UIDesc.fPaddingY = 28.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//������ 1 ü�¹� ���
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar_BackGround");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 198.0f;
	UIDesc.fPaddingY = 170.0f;
	UIDesc.fSizeX = 102.f;
	UIDesc.fSizeY = 6.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//������2 ü�¹� ���
	UIDesc.fPaddingY = 102.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//������3 ü�¹� ���
	UIDesc.fPaddingY = 34.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//������ 1 ü�¹�
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar");
	UIDesc.fPaddingY = 171.0f;
	UIDesc.fSizeX = 102.f;
	UIDesc.fSizeY = 4.f;
	UIDesc.fPosZ = 0.025f;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//������ 2 ü�¹�
	UIDesc.fPaddingY = 103.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//������ 3 ü�¹�
	UIDesc.fPaddingY = 35.0f;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;




	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Player_Stamina(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	//HealthBar_BackGround
	CUI::UIDESC		UIDesc;
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_BackGround");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 26.0f;
	UIDesc.fSizeX = 308.f;
	UIDesc.fSizeY = 31.f;
	UIDesc.fPosZ = 0.1f;
	UIDesc.bIsRender = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//¼öÁ¤»çÇ×
	UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_Player_Brown");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 34.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 300.f;
	UIDesc.fSizeY = 15.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_HealthBar"), &UIDesc)))
		return E_FAIL;


	//Text
	CFont::TEXTDESC TextDesc;
	TextDesc.strText = L"HP\nMP\nSP";
	TextDesc.Height = 10;
	TextDesc.Width = 10;
	TextDesc.Weight = FW_NORMAL;
	TextDesc.CharSet = HANGUL_CHARSET;
	lstrcpy(TextDesc.FaceName, L"±Ã¼­");
	TextDesc.TextColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	TextDesc.UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	TextDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	TextDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Text_Transparent_BackGround");//TEXT("Prototype_Component_Texture_UI_Text_Transparent_BackGround");
	TextDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	TextDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	TextDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	TextDesc.UIDesc.bIsFixed = true;
	TextDesc.UIDesc.fPaddingX = 0.0f;
	TextDesc.UIDesc.fPaddingY = 57.0f;
	TextDesc.UIDesc.fSizeX = 308.f;
	TextDesc.UIDesc.fSizeY = 34.f;
	TextDesc.UIDesc.fPosZ = 0.1f;
	TextDesc.UIDesc.bIsRender = true;
	TextDesc.UIDesc.eAnchor = CUI::DOWN;
	TextDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	TextDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	TextDesc.UIDesc.bIsAnimation = false;



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Text"), &TextDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Tooltip(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//Tooltip
	CUI_Unfixed::UIUNFIXEDDESC UIUnfixedDesc;
	UIUnfixedDesc.fRotationPerSec = 1.0f;
	UIUnfixedDesc.fSpeedPerSec = 500.f;

	UIUnfixedDesc.UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIUnfixedDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	UIUnfixedDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Tooltip");
	UIUnfixedDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIUnfixedDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIUnfixedDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIUnfixedDesc.UIDesc.bIsFixed = false;
	UIUnfixedDesc.UIDesc.fPaddingX = 21.0f;
	UIUnfixedDesc.UIDesc.fPaddingY = 20.0f;
	UIUnfixedDesc.UIDesc.fSizeX = 527.f;
	UIUnfixedDesc.UIDesc.fSizeY = 249.f;
	UIUnfixedDesc.UIDesc.fPosZ = 0.1f;
	UIUnfixedDesc.UIDesc.bIsRender = false;
	UIUnfixedDesc.UIDesc.eAnchor = CUI::LEFTUP;
	UIUnfixedDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	UIUnfixedDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	UIUnfixedDesc.UIDesc.bIsAnimation = false;



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Unfixed"), &UIUnfixedDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_RadioBase(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Unfixed::UIUNFIXEDDESC UIUnfixedDesc;
	UIUnfixedDesc.fRotationPerSec = 1.0f;
	UIUnfixedDesc.fSpeedPerSec = 500.f;
	UIUnfixedDesc.UIDesc.iLevelIndex = LEVEL_GAMEPLAY;
	UIUnfixedDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	UIUnfixedDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_RadioBase");
	UIUnfixedDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIUnfixedDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIUnfixedDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIUnfixedDesc.UIDesc.bIsFixed = false;
	UIUnfixedDesc.UIDesc.fPaddingX = 92.0f;
	UIUnfixedDesc.UIDesc.fPaddingY = 33.0f;
	UIUnfixedDesc.UIDesc.fSizeX = 178.f;
	UIUnfixedDesc.UIDesc.fSizeY = 382.f;
	UIUnfixedDesc.UIDesc.fPosZ = 0.1f;
	UIUnfixedDesc.UIDesc.bIsRender = false;
	UIUnfixedDesc.UIDesc.eAnchor = CUI::RIGHTUP;
	UIUnfixedDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	UIUnfixedDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	UIUnfixedDesc.UIDesc.bIsAnimation = true;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, strLayerTag,
		TEXT("Prototype_GameObject_UI_Unfixed"), &UIUnfixedDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLevel_GamePlay");
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
