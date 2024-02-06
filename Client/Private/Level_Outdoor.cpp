#include "stdafx.h"
#include "..\Public\Level_Outdoor.h"

#include "GameInstance.h"
#include "Camera_Free.h"


#include "Engine_Defines.h"
#include "Engine_Struct.h"

#include "UI.h"
#include "Font.h"
#include "UI_Unfixed.h"
#include "UI_Fixed.h"

#include "InteractiveObject.h"
#include "Item_Equipment.h"
#include "Item_Consumable.h"
#include "Item_Interactive.h"
#include "Inventory.h"
#include "Effect.h"

#include "Level_Loading.h"

#include <iostream>
#include <fstream>

CLevel_Outdoor::CLevel_Outdoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}


HRESULT CLevel_Outdoor::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Enemy(TEXT("Layer_Enemy")))) // TEMP
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Player(TEXT("Layer_Camera"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;*/

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section1.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section2.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section3.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section4.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section5.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section6.map"), TEXT("Section_Outdoor"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section_2F.map"), TEXT("Section_2F"), true)))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("../Bin/Resources/Maps/Section_1F.map"), TEXT("Section_1F"))))
		return E_FAIL;

	// Create Gimmick Objects
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Environment"), TEXT("Prototype_BookShelf"), TEXT("Section_2F"))))
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Environment"), TEXT("Prototype_Train"), TEXT("Section_Outdoor"))))
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Environment"), TEXT("Prototype_GimmickWall"), TEXT("Section_2F"))))
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		Safe_Release(pGameInstance);
	}

	// Ready Section Ending
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Background"), TEXT("Prototype_GameObject_Ending"), TEXT("Section_Ending"))))
		{
			return E_FAIL;
		}

		Safe_Release(pGameInstance);
	}

	// Init Map Settings
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Background"), TEXT("Prototype_GameObject_BackgroundCylinder"))))
		{
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Background"), TEXT("Prototype_GameObject_Cloud"))))
		{
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Background"), TEXT("Prototype_GameObject_CircleMask"))))
		{
			return E_FAIL;
		}

		/*if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Background"), TEXT("Prototype_GameObject_Smoke"), TEXT("Section_Outdoor"))))
		{
			return E_FAIL;
		}*/



		Safe_Release(pGameInstance);

		CGameInstance::Get_Instance()->Enable_Section(LEVEL_OUTDOOR, TEXT("Section_Outdoor"));
		CGameInstance::Get_Instance()->Disable_Section(LEVEL_OUTDOOR, TEXT("Section_2F"));
		CGameInstance::Get_Instance()->Disable_Section(LEVEL_OUTDOOR, TEXT("Section_1F"));
		CGameInstance::Get_Instance()->Disable_Section(LEVEL_OUTDOOR, TEXT("Section_Ending"));

		Ready_Lights_Section_1();
	}

	// Init Effect Manager
	{
		CGameInstance::Get_Instance()->Reserve_Manager(LEVEL_OUTDOOR);
	}




	if (FAILED(Ready_Layer_UI_HudBox()))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_UI_Player_Stamina()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_UI_RadioBase()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Objective_Complete()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Tooltip()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_CrossHair()))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_UI_Buff()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_UI_Text()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Map()))
		return E_FAIL;

	// TODO: Progress Test
	if (FAILED(Ready_Layer_UI_Progress()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	////아이템
	//if (FAILED(Ready_Layer_Object_Interactive(TEXT("Layer_Object_Interactive"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UI_Weapon()))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Notification()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Dead()))
		return E_FAIL;

	if (FAILED(Ready_Particle()))
		return E_FAIL;
	
	CGameInstance::Get_Instance()->Subscribe(EVENT_SECTION, std::bind(&CLevel_Outdoor::On_Section_Trigger, this, std::placeholders::_1));
	CGameInstance::Get_Instance()->Subscribe(EVENT_RESPAWN_2F, std::bind(&CLevel_Outdoor::On_Respawn_Event, this, std::placeholders::_1));
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	_uint uiCurrentLevel = LEVEL_OUTDOOR;
	pGameInstance->OnEvent(EVENT_UPDATE_LEVEL, &uiCurrentLevel);
	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_HP_NUM0, true, 1);
	//pGameInstance->PlayBGM(L"Music_MysteriousRevelationsShort.wav", SOUND_BGM0, 0.3f);
	//pGameInstance->Play_Sound(L"Music_MysteriousRevelationsShort.wav", SOUND_BGM0, 0.3f);
	pGameInstance->Set_Render_Timer(LEVEL_OUTDOOR, UI_TEXT, true, false, 2.0f, 0);
	pGameInstance->Play_Sound(L"MIKE_MothNFlame_Jig.wav", SOUND_PLAYER_TALK, 2.0f);
	Safe_Release(pGameInstance);



	return S_OK;
} 

void CLevel_Outdoor::Tick(_float fTimeDelta)
{
	////아이템 셀렉트 박스 전환 
	//if (GetAsyncKeyState('1') & 0x8000)
	//{
	////	CInventory* pInventory = new CInventory();




	//	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT0, true);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT1, false);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT2, false);
	//	Safe_Release(pGameInstance);*/
	//}
	//if (GetAsyncKeyState('2') & 0x8000)
	//{
	//	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT0, false);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT1, true);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT2, false);
	//	Safe_Release(pGameInstance);*/
	//}
	//if (GetAsyncKeyState('3') & 0x8000)
	//{
	///*	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT0, false);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT1, false);
	//	pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT2, true);
	//	Safe_Release(pGameInstance);*/
	//}

	// TODO: TEST CODE
	// TODO: 여기서부터 하셈. 제발 까먹지 말고~~
	//{
	//	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);
	//	if (pGameInstance->Key_Down('P'))
	//	{
	//		MOVEABLEEFFECTDESC effectDesc;
	//		effectDesc.bActive = true;
	//		effectDesc.vDir = _float3(-0.01f, 0.f, 0.f);
	//		effectDesc.vPos = _float3(2.f, 2.f, 2.f);
	//		effectDesc.vScale = _float3(1.f, 1.f, 1.f);

	//		pGameInstance->OnEvent(EVENT_EFFECT_FIRE_STRIP, &effectDesc);
	//	}
	//	Safe_Release(pGameInstance);
	//}
	

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Set_Render_Timer(LEVEL_OUTDOOR, UI_CROSSHAIR, false, false, 0.4f );
		pGameInstance->Set_Render_Timer(LEVEL_OUTDOOR, UI_CROSSHAIR_ATTACK, true, true, 0.4f);
		Safe_Release(pGameInstance);
	}

	if (GetAsyncKeyState('7') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Set_PartiallyHidden(LEVEL_OUTDOOR);
		Safe_Release(pGameInstance);
	}

	if (GetAsyncKeyState('8') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Set_Hidden(LEVEL_OUTDOOR);
		Safe_Release(pGameInstance);
	}

	if (GetAsyncKeyState('9') & 0x8000)
	{
		/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->OnEvent(EVENT_MAP, nullptr);
		Safe_Release(pGameInstance);*/
	}
	
	__super::Tick(fTimeDelta);
}

void CLevel_Outdoor::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down('J'))
	{
		pGameInstance->OnEvent(EVENT_SECTION);
		pGameInstance->OnEvent(EVENT_CLEAR_PARTICLE, nullptr);
	}

	if (pGameInstance->Key_Down('L'))
	{
		pGameInstance->OnEvent(EVENT_RESPAWN_2F);
	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("°ÔÀÓÇÃ·¹ÀÌ·¹º§ÀÔ´Ï´Ù."));
}

HRESULT CLevel_Outdoor::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Outdoor::Ready_Lights_Section_1()
{
	Reset_Light_Settings();

	D3DLIGHT9			LightDesc;

	// Directional Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.Type = D3DLIGHT_DIRECTIONAL;
		LightDesc.Direction = _float3(-1.f, -1.f, -1.f);
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_pGraphic_Device->SetLight(0, &LightDesc);

		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	// Spot Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.Type = D3DLIGHT_SPOT;
		LightDesc.Position = _float3(-13.f, -4.f, 9.0f);
		LightDesc.Direction = _float3(0.f, -1.f, 0.f);
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Range = 100.f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Phi = D3DXToRadian(45.0f);
		LightDesc.Theta = D3DXToRadian(30.0f);
		LightDesc.Falloff = 1.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Turnel Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-55.790928f, -17.131190f, 8.812697f);
		LightDesc.Range = 10.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}
	

	D3DMATERIAL9		MtrlDesc;
	ZeroMemory(&MtrlDesc, sizeof MtrlDesc);
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pGraphic_Device->SetMaterial(&MtrlDesc);
}

void CLevel_Outdoor::Ready_Lights_Section_2()
{
	Reset_Light_Settings();

	D3DLIGHT9			LightDesc;

	// Directional Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);
		LightDesc.Type = D3DLIGHT_DIRECTIONAL;
		LightDesc.Direction = _float3(-1.f, -1.f, -1.f);
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_pGraphic_Device->SetLight(0, &LightDesc);

		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	// Turnel Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-55.790928f, -17.131190f, 8.812697f);
		LightDesc.Range = 10.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Market Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-86.590950f, -18.330009f, 18.811647f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}
}

void CLevel_Outdoor::Ready_Lights_Section_3()
{
	Reset_Light_Settings();

	D3DLIGHT9			LightDesc;

	// Directional Light
	{
		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	// Fire Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);
		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Position = _float3(-83.f, -18.f, 30.f);
		LightDesc.Range = 12.f;
		LightDesc.Attenuation0 = 0.3f;
		LightDesc.Diffuse = D3DXCOLOR(1.f, 0.0f, 0.f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

		m_pGraphic_Device->SetLight(1, &LightDesc);

		m_pGraphic_Device->LightEnable(1, TRUE);
	}
}

void CLevel_Outdoor::Ready_Lights_Section_4()
{
	Reset_Light_Settings();

	// Directional Light
	{
		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	D3DLIGHT9			LightDesc;

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-51.f, -15.8f, 57.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-46.f, -15.8f, 63.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}

	// Neon Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-39.834461f, - 13.0f, 56.240356f);
		LightDesc.Range = 3.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(3, &LightDesc);
		m_pGraphic_Device->LightEnable(3, TRUE);
	}
}

void CLevel_Outdoor::Ready_Lights_Section_5()
{
	Reset_Light_Settings();

	D3DLIGHT9			LightDesc;

	// Directional Light
	{
		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-51.f, -15.8f, 57.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-46.f, -15.8f, 63.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-40.709846f, - 14.4f, 73.626923f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(3, &LightDesc);
		m_pGraphic_Device->LightEnable(3, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-38.064079f, -15.7f,  64.128105f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(4, &LightDesc);
		m_pGraphic_Device->LightEnable(4, TRUE);
	}

	// Neon Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-39.978779f, -13.0f, 56.678532f);
		LightDesc.Range = 3.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(5, &LightDesc);
		m_pGraphic_Device->LightEnable(5, TRUE);
	}
}

void CLevel_Outdoor::Ready_Lights_Section_6()
{
	Reset_Light_Settings();

	// Directional Light
	{
		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	D3DLIGHT9			LightDesc;

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-18.7f, -22.4f, 101.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-11.0f, -22.4f, 92.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(3.f, -22.6f, 107.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(3, &LightDesc);
		m_pGraphic_Device->LightEnable(3, TRUE);
	}

	// Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(10.0f, -22.6f, 98.5f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(4, &LightDesc);
		m_pGraphic_Device->LightEnable(4, TRUE);
	}

	// Yellow Neon Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 0.8f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-5.5f, -22.f, 116.6f);
		LightDesc.Range = 15.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(5, &LightDesc);
		m_pGraphic_Device->LightEnable(5, TRUE);
	}

	// Pink Neon Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.f, 1.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(1.f, -11.5f, 121.f);
		LightDesc.Range = 3.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(6, &LightDesc);
		m_pGraphic_Device->LightEnable(6, TRUE);
	}
}

void CLevel_Outdoor::Ready_Lights_Section_2F()
{
	Reset_Light_Settings();

	D3DLIGHT9 LightDesc;

	// Directional Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.Type = D3DLIGHT_DIRECTIONAL;
		LightDesc.Direction = _float3(0.f, -1.f, 0.f);
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_pGraphic_Device->SetLight(0, &LightDesc);

		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	D3DMATERIAL9		MtrlDesc;
	ZeroMemory(&MtrlDesc, sizeof MtrlDesc);
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pGraphic_Device->SetMaterial(&MtrlDesc);
}

void CLevel_Outdoor::Ready_Lights_Section_1F()
{
	Reset_Light_Settings();

	D3DLIGHT9 LightDesc;

	// Directional Light
	{
		ZeroMemory(&LightDesc, sizeof LightDesc);

		LightDesc.Type = D3DLIGHT_DIRECTIONAL;
		LightDesc.Direction = _float3(0.f, 0.f, 0.f);
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 1.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
		LightDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_pGraphic_Device->SetLight(0, &LightDesc);

		m_pGraphic_Device->LightEnable(0, TRUE);
	}

	// Chandelier Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(5.f, 3.5f, 10.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 1.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(1, &LightDesc);
		m_pGraphic_Device->LightEnable(1, TRUE);
	}

	// Chandelier Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(8.f, 3.5f, 15.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 1.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(2, &LightDesc);
		m_pGraphic_Device->LightEnable(2, TRUE);
	}

	// Stage Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(12.9f, 3.f, 9.7f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(3, &LightDesc);
		m_pGraphic_Device->LightEnable(3, TRUE);
	}

	// Neon Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 1.f, 0.8f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(-3.5f, 2.42f, 10.366f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 1.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(4, &LightDesc);
		m_pGraphic_Device->LightEnable(4, TRUE);
	}

	// Golden Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.84f, 0.f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(9.4f, 4.8f, 29.f);
		LightDesc.Range = 15.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(5, &LightDesc);
		m_pGraphic_Device->LightEnable(5, TRUE);
	}

	// Gamble Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(0.f, 4.0f, 0.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(6, &LightDesc);
		m_pGraphic_Device->LightEnable(6, TRUE);
	}

	// Gamble Point Light
	{
		ZeroMemory(&LightDesc, sizeof(D3DLIGHT9));

		LightDesc.Type = D3DLIGHT_POINT;
		LightDesc.Diffuse = D3DXCOLOR(1.0f, 0.7f, 0.5f, 1.f);
		LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		LightDesc.Position = D3DXVECTOR3(5.f, 4.0f, -1.f);
		LightDesc.Range = 6.0f;
		LightDesc.Attenuation0 = 2.0f;
		LightDesc.Attenuation1 = 0.0f;
		LightDesc.Attenuation2 = 0.0f;

		m_pGraphic_Device->SetLight(7, &LightDesc);
		m_pGraphic_Device->LightEnable(7, TRUE);
	}

	D3DMATERIAL9		MtrlDesc;
	ZeroMemory(&MtrlDesc, sizeof MtrlDesc);
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlDesc.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pGraphic_Device->SetMaterial(&MtrlDesc);
}

void CLevel_Outdoor::Reset_Light_Settings()
{
	for (int i = 0; i < 8; ++i)
	{
		m_pGraphic_Device->LightEnable(i, FALSE);
	}
}

HRESULT CLevel_Outdoor::Ready_Layer_Camera(const wstring& strLayerTag)
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
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Camera_Free"), &CameraFreeDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_Enemy(const wstring& strLayerTag)
{
  	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	  Safe_AddRef(pGameInstance);

 // 	/* For.Goon */
	//if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, strLayerTag,
	//	TEXT("Prototype_GameObject_Goon"),TEXT("Section1"))))
	//{
	//	return E_FAIL;
	//}

	/* For PipeGuy */
	/*if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_PipeGuy"), TEXT("Section1"))))
	{
		return E_FAIL;
	}*/

	/* For Boss */
	if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Boss"), TEXT("Section_Outdoor"))))
	{
		return E_FAIL;
	}

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag, TEXT("Prototype_GameObject_Delia"), TEXT("Section_Outdoor"))))
	//{
	//	return E_FAIL;
	//}

	//// Gimmick
	//OBJECTDESC gimmickDesc{};
	//gimmickDesc.bAlphaTesting = true;
	//gimmickDesc.eObjectType = OBJECT_INTERATIVE;
	//gimmickDesc.strTextureTag = TEXT("Gimmick_Fan");
	//gimmickDesc.eItemType = ITEM_FAN;
	////wstring strGimmick = TEXT("Layer_Interactive");
	//// FAN
	//if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Interactive"),
	//	TEXT("Prototype_GameObject_Gimmick_Object"), TEXT("Section1"), &gimmickDesc)))
	//{
	//	return E_FAIL;
	//}
	//gimmickDesc.strTextureTag = TEXT("Gimmick_Key");
	//gimmickDesc.eItemType = ITEM_ENTRANCE_KEY;
	//// ENTRANCE KEY
	//if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Interactive"),
	//	TEXT("Prototype_GameObject_Gimmick_Object"), TEXT("Section1"), &gimmickDesc)))
	//{
	//	return E_FAIL;
	//}
	//// Fence
	//gimmickDesc.strTextureTag = TEXT("Gimmick_Fence");
	//gimmickDesc.eItemType = ITEM_DOOR;
	//if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Interactive"),
	//	TEXT("Prototype_GameObject_Gimmick_Object"), TEXT("Section1"), &gimmickDesc)))
	//{
	//	return E_FAIL;
	//}

    Safe_Release(pGameInstance);

    return S_OK;
}
    

HRESULT CLevel_Outdoor::Ready_Layer_Camera_Player(const wstring& strLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Camera_Player"), &CameraFreeDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_Player(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Outdoor::Ready_Layer_UI_HudBox()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUI::UIDESC		UIDesc;

	//HUDBox
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;


	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 체력 백의 자리
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 60.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_HP_NUM0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 체력 십의 자리
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 85.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_HP_NUM1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 체력 일의 자리
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_HPNum");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 110.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.fSizeX = 25.f;
	UIDesc.fSizeY = 49.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_HP_NUM2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 건강한 얼굴
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Healthy");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 30.0f;
	UIDesc.fPaddingY = 76.0f;
	UIDesc.fSizeX = 142.f;
	UIDesc.fSizeY = 168.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 0.2f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_HEALTHY,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 약간 아픈 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Slightly_Hurt");
	UIDesc.bIsRender = false;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_SLIGHTLYHURT,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 아픈 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hurt");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_HURT,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 좀 아픈 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Moderately_Hurt");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_MODERATELYHURT,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 많이 아픈 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Badly_Hurt");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_BADLYHURT,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 고통스러운 얼굴
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Pain");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_PAIN,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 죽은 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Dead");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_DEAD,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 약간 숨겨진 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_PartialHidden");
	UIDesc.fPaddingX = 27.0f;
	UIDesc.fPaddingY = 74.0f;
	UIDesc.fSizeX = 148.f;
	UIDesc.fSizeY = 168.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_PARTIALLYHIDDEN,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HUDBox 마이크 숨겨진 얼굴
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Mike_Hidden");
	////횇횞쩍쨘횈짰
	//UIDesc.bIsRender = true;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_MIKE_HIDDEN,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	// HUDBox 인벤토리 1번째 칸
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// HUDBox 인벤토리 1번째 칸 부러진 이미지
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.2f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_BROKEN0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// HUDBox 인벤토리 2번째 칸
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory");
	UIDesc.fPosZ = 0.3f;
	UIDesc.fPaddingY = 98.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// HUDBox 인벤토리 2번째 칸 부러진 이미지
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.2f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_BROKEN1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// HUDBox 인벤토리 3번째 칸
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory");
	UIDesc.fPosZ = 0.3f;
	UIDesc.fPaddingY = 30.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	// HUDBox 인벤토리 3번째 칸 부러진 이미지
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Inventory_Broken");
	UIDesc.fPosZ = 0.2f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_BROKEN2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//HudBox 셀렉트 박스1
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//HudBox 셀렉트 박스2
	UIDesc.fPaddingY = 96.0f;
	UIDesc.bIsRender = false;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//HudBox 셀렉트 박스3
	UIDesc.fPaddingY = 28.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_SELECT2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//아이템 1 체력바 배경
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR_BACKGROUND0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//아이템2 체력바 배경
	UIDesc.fPaddingY = 102.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR_BACKGROUND1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//아이템3 체력바 배경
	UIDesc.fPaddingY = 34.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR_BACKGROUND2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//아이템 1 체력바
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress"); 
	UIDesc.fPaddingY = 171.0f;
	UIDesc.fSizeX = 102.f;
	UIDesc.fSizeY = 4.f;
	UIDesc.fPosZ = 0.025f;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR0,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//아이템 2 체력바
	UIDesc.fPaddingY = 103.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR1,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//아이템 3 체력바
	UIDesc.fPaddingY = 35.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_HPBAR2,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Player_Stamina()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	//스태미나바 배경
	CUI::UIDESC		UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.fPosZ = 0.3f;
	UIDesc.bIsRender = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;
	UIDesc.eRender = CUI::RENDER_TEST;


	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_STAMINA_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//스태미나 바
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIDesc.fPosZ = 0.2f;
	UIDesc.bIsRender = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_STAMINA_BAR,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	
	////Text
	//CFont::TEXTDESC TextDesc;
	//TextDesc.strText = L"HP\nMP\nSP";
	//TextDesc.Height = 10;
	//TextDesc.Width = 10;
	//TextDesc.Weight = FW_NORMAL;
	//TextDesc.CharSet = HANGUL_CHARSET;
	//lstrcpy(TextDesc.FaceName, L"궁서");
	//TextDesc.TextColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	//TextDesc.UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	//TextDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	//TextDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Text_Transparent_BackGround");//TEXT("Prototype_Component_Texture_UI_Text_Transparent_BackGround");
	//TextDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	//TextDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	//TextDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	//TextDesc.UIDesc.bIsFixed = true;
	//TextDesc.UIDesc.fPaddingX = 0.0f;
	//TextDesc.UIDesc.fPaddingY = 57.0f;
	//TextDesc.UIDesc.fSizeX = 308.f;
	//TextDesc.UIDesc.fSizeY = 34.f;
	//TextDesc.UIDesc.fPosZ = 0.1f;
	//TextDesc.UIDesc.bIsRender = true;
	//TextDesc.UIDesc.eAnchor = CUI::DOWN;
	//TextDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	//TextDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	//TextDesc.UIDesc.bIsAnimation = false;
	//TextDesc.UIDesc.fAnimationDelay = 0.0f;
	//TextDesc.UIDesc.eRender = CUI::RENDER_TEST;


	//if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, strLayerTag,
	//	TEXT("Prototype_GameObject_UI_Text"), &TextDesc)))
	//	return E_FAIL;

	

	Safe_Release(pGameInstance);
	return S_OK;

}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Tooltip()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//Tooltip
	CUI_Unfixed::UIUNFIXEDDESC UIUnfixedDesc;
	UIUnfixedDesc.eDir = CUI_Unfixed::DIR_DOWN;
	UIUnfixedDesc.fRotationPerSec = 1.0f;
	UIUnfixedDesc.fSpeedPerSec = 500.f;
	UIUnfixedDesc.UIDesc.iLevelIndex = LEVEL_OUTDOOR;
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
	UIUnfixedDesc.UIDesc.fAnimationRatio = 0.0f;
	UIUnfixedDesc.UIDesc.eRender = CUI::RENDER_TEST;


	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOOLTIP,
		TEXT("Prototype_UI_UnFixed"), &UIUnfixedDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_RadioBase()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//라디오
	CUI_Unfixed::UIUNFIXEDDESC UIUnfixedDesc;
	UIUnfixedDesc.eDir = CUI_Unfixed::DIR_LEFT;
	UIUnfixedDesc.fRotationPerSec = 1.0f;
	UIUnfixedDesc.fSpeedPerSec = 500.f;
	UIUnfixedDesc.UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIUnfixedDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	UIUnfixedDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_RadioBase");
	UIUnfixedDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIUnfixedDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIUnfixedDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIUnfixedDesc.UIDesc.bIsFixed = false;
	UIUnfixedDesc.UIDesc.fPaddingX = 84.0f;
	UIUnfixedDesc.UIDesc.fPaddingY = 33.f;//33.0f;
	UIUnfixedDesc.UIDesc.fSizeX = 178.f;
	UIUnfixedDesc.UIDesc.fSizeY = 382.f;
	UIUnfixedDesc.UIDesc.fPosZ = 0.1f;
	UIUnfixedDesc.UIDesc.bIsRender = false;
	UIUnfixedDesc.UIDesc.eAnchor = CUI::RIGHTUP;
	UIUnfixedDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	UIUnfixedDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	UIUnfixedDesc.UIDesc.bIsAnimation = true;
	UIUnfixedDesc.UIDesc.eRender = CUI::RENDER_TEST;
	UIUnfixedDesc.UIDesc.fAnimationRatio = 0.66f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_RADIOBASE,
		TEXT("Prototype_UI_UnFixed"), &UIUnfixedDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_CrossHair()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUI::UIDESC		UIDesc;

	//CrossHair
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_CrossHair_Default");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 51.f;
	UIDesc.fSizeY = 51.f;
	UIDesc.fPosZ = 0.1f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_CROSSHAIR,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//공격시 보여주는 UI.
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_CrossHair_Attack");
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 4.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_CROSSHAIR_ATTACK,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//범위안인지 표시하는 UI.
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_CrossHair_InDistance");
	UIDesc.fPosZ = 0.05f; 
	UIDesc.fSizeX = 55.f;
	UIDesc.fSizeY = 55.f;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_CROSSHAIR_INDISTANCE,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//프롬프터
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Prompter");
	UIDesc.fPosZ = 0.05f;
	UIDesc.fSizeX = 300.f;
	UIDesc.fSizeY = 75.f;
	UIDesc.fPaddingX = UIDesc.fSizeX*0.5f + 25.f;
	UIDesc.fPaddingY = -UIDesc.fSizeY*0.5f-25.5f;
	UIDesc.bIsRender = true;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PROMPTER,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	////차징공격시 보여주는 UI.
	//UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	//UIDesc.iLevelStatic = LEVEL_STATIC;
	//UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_CrossHair_Charging");
	//UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	//UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	//UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	//UIDesc.fPaddingX = 0.0f;
	//UIDesc.fPaddingY = 0.0f;
	//UIDesc.fSizeX = 100.f;
	//UIDesc.fSizeY = 100.f;
	//UIDesc.fPosZ = 0.1f;
	//UIDesc.bIsRender = true;
	//UIDesc.bIsFixed = true;
	//UIDesc.eAnchor = CUI::CENTER;
	//UIDesc.iWindowSizeX = g_iWinSizeX;
	//UIDesc.iWindowSizeY = g_iWinSizeY;
	//UIDesc.bIsAnimation = true;
	//UIDesc.fAnimationRatio = 0.8f;
	//UIDesc.eRender = CUI::RENDER_TEST;

	//if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_CROSSHAIR_CHARGING,
	//	TEXT("Prototype_UI_Fixed"), &UIDesc)))
	//	return E_FAIL;


	////차징공격 준비 완료
	//UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	//UIDesc.iLevelStatic = LEVEL_STATIC;
	//UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_CrossHair_HeavyReady");
	//UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	//UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	//UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	//UIDesc.fPaddingX = 0.0f;
	//UIDesc.fPaddingY = 0.0f;
	//UIDesc.fSizeX = 51.f;
	//UIDesc.fSizeY = 51.f;
	//UIDesc.fPosZ = 0.1f;
	//UIDesc.bIsRender = true;
	//UIDesc.bIsFixed = true;
	//UIDesc.eAnchor = CUI::CENTER;
	//UIDesc.iWindowSizeX = g_iWinSizeX;
	//UIDesc.iWindowSizeY = g_iWinSizeY;
	//UIDesc.bIsAnimation = false;
	//UIDesc.fAnimationRatio = 1.0f;
	//UIDesc.eRender = CUI::RENDER_TEST;

	//if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_CROSSHAIR_HEAVY_READY,
	//	TEXT("Prototype_UI_Fixed"), &UIDesc)))
	//	return E_FAIL;

	//적화살표 준비 완료
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_EnemyArrow");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 100.f;
	UIDesc.fSizeY = 50.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_ARROW0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_ARROW1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_ARROW2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_ARROW3,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_ARROW4,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_Guard_Background");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = -110.0f;
	UIDesc.fSizeX = 230.f;
	UIDesc.fSizeY = 22.f;
	UIDesc.fPosZ = 0.1f;
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_GUARD_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_Guard_Bar");
	UIDesc.fPosZ = 0.05f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_GUARD_BAR,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//체력회복시 초록화면

	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_Green");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.0005f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 3.0f;
	UIDesc.eRender = CUI::RENDER_BLEND;


	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HP_UP,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//체력감소시 빨간화면

	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_Red");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.0005f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 3.0f;
	UIDesc.eRender = CUI::RENDER_BLEND;


	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_HP_DOWN,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Buff()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUI::UIDESC		UIDesc;

	//Buff 1번째칸 1층 스태미나
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Stamina");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 4.0f;
	UIDesc.fPaddingY = 257.0f;
	UIDesc.fSizeX = 59.f;
	UIDesc.fSizeY = 80.f;
	UIDesc.fPosZ = 0.1f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::LEFTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.eRender = CUI::RENDER_BLEND;
	UIDesc.fAnimationRatio = 1/20.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_1L_1F_STAMINA,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//Buff 1번째칸 1층 힐
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Heal");
	UIDesc.bIsRender = false;
	UIDesc.fAnimationRatio = 1/5.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_1L_1F_HEAL,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Buff 1번째칸 2층 스태미나
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Stamina");
	UIDesc.fPaddingY = 277.0f;
	UIDesc.fAnimationRatio = 1 / 20.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_1L_2F_STAMINA,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Buff 1번째칸 2층 힐
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Heal");
	UIDesc.fAnimationRatio = 1 / 5.f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_1L_2F_HEAL,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//Buff 2번째칸 1층 스태미나
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Stamina");
	UIDesc.fPaddingX = 63.0f;
	UIDesc.fPaddingY = 257.0f;
	UIDesc.bIsRender = false;
	UIDesc.fAnimationRatio = 1 / 20.f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_2L_1F_STAMINA,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;


	//Buff 2번째칸 1층 힐
	UIDesc.bIsRender = true;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Heal"); 
	UIDesc.fAnimationRatio = 1 / 5.f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_2L_1F_HEAL,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	

	//Buff 2번째칸 2층 스태미나
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Stamina");
	UIDesc.fPaddingY = 277.0f;
	UIDesc.bIsRender = false;
	UIDesc.fAnimationRatio = 1 / 20.f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_2L_2F_STAMINA,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//Buff 2번째칸 2층 힐
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Buff_Heal");
	UIDesc.fAnimationRatio = 1 / 5.f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_BUFF_2L_2F_HEAL,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Objective_Complete()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Unfixed::UIUNFIXEDDESC UIUnfixedDesc;
	UIUnfixedDesc.eDir = CUI_Unfixed::DIR_RIGHT;
	UIUnfixedDesc.fRotationPerSec = 1.0f;
	UIUnfixedDesc.fSpeedPerSec = 500.f;
	UIUnfixedDesc.UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIUnfixedDesc.UIDesc.iLevelStatic = LEVEL_STATIC;
	UIUnfixedDesc.UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_ObjectiveComplete");
	UIUnfixedDesc.UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIUnfixedDesc.UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIUnfixedDesc.UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIUnfixedDesc.UIDesc.bIsFixed = false;
	UIUnfixedDesc.UIDesc.fPaddingX = 26.0f;
	UIUnfixedDesc.UIDesc.fPaddingY = 21.0f;
	UIUnfixedDesc.UIDesc.fSizeX = 322.f;
	UIUnfixedDesc.UIDesc.fSizeY = 76.f;
	UIUnfixedDesc.UIDesc.fPosZ = 0.002f;
	UIUnfixedDesc.UIDesc.bIsRender = false;
	UIUnfixedDesc.UIDesc.eAnchor = CUI::LEFTUP;
	UIUnfixedDesc.UIDesc.iWindowSizeX = g_iWinSizeX;
	UIUnfixedDesc.UIDesc.iWindowSizeY = g_iWinSizeY;
	UIUnfixedDesc.UIDesc.bIsAnimation = true;
	UIUnfixedDesc.UIDesc.eRender = CUI::RENDER_TEST;
	UIUnfixedDesc.UIDesc.fAnimationRatio = 0.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_OBJECTIVE_COMPLETE,
		TEXT("Prototype_UI_UnFixed"), &UIUnfixedDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Text()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Text");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 168.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 720.f;
	UIDesc.fSizeY = 80.f;
	UIDesc.fPosZ = 0.005f;
	UIDesc.bIsRender = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 0.0f;
	UIDesc.eRender = CUI::RENDER_BLEND;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TEXT,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Map()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Map");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.00001f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_MAP,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Weapon()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//권총
	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_AutoPistol");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.fPaddingX = 146.0f;
	UIDesc.fPaddingY = 71.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 127.f;
	UIDesc.fSizeY = 86.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::RIGHTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_AUTOPISTOL,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;
	
	//맥주병
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_Bottle");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.001f;
	UIDesc.fSizeX = 30.0f;
	UIDesc.fSizeY = 112.0f;
	UIDesc.bIsRender = false;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_BOTTLE,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;
	
	//맥주병 배경
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Background");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.fSizeX = 30.0f;
	UIDesc.fSizeY = 112.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_BOTTLE_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//맥주병깨진것
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Broken");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.001f;
	UIDesc.fSizeX = 95.f;
	UIDesc.fSizeY = 29.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_BOTTLE_BROKEN,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//맥주병깨진것 배경
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_Bottle_Broken_Background");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.fSizeX = 95.f;
	UIDesc.fSizeY = 29.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_BOTTLE_BROKEN_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//쇠파이프
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.01f;
	UIDesc.fSizeX = 221.0f;
	UIDesc.fSizeY = 35.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_LEADPIPE,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//쇠파이프 배경
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe_Background");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 141.f;
	UIDesc.fPaddingY = 60.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.fSizeX = 221.0f;
	UIDesc.fSizeY = 35.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_LEADPIPE_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//쇠파이프 망가진것
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_LeadPipe_Broken");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 170.0f;
	UIDesc.fPaddingY = 74.0f;
	UIDesc.fSizeX = 163.f;
	UIDesc.fSizeY = 63.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_LEADPIPE_BROKEN,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//토미건
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_TommyGun");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.fPaddingX = 150.0f;
	UIDesc.fPaddingY = 105.0f;
	UIDesc.fSizeX = 343.f;
	UIDesc.fSizeY = 111.f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_WEAPON_TOMMYGUN,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//권총탄알
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_Pistol_Bullet");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 27.0f;
	UIDesc.fPaddingY = 47.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 61.f;
	UIDesc.fSizeY = 25.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::RIGHTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 60.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 73.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 86.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET3,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 99.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET4,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 112.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET5,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 125.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET6,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 138.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET7,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 151.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET8,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 164.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET9,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 177.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET10,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 190.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET11,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 203.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET12,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 216.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PISTOL_BULLET13,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//기관총탄알
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Weapon_TommyGun_Bullet");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 20.0f;
	UIDesc.fPaddingY = 51.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 37.f;
	UIDesc.fSizeY = 17.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::RIGHTDOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET0,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 60.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET1,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 69.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET2,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 78.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET3,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 87.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET4,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 96.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET5,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 105.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET6,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 114.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET7,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 123.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET8,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 132.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET9,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 141.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET10,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 150.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET11,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 159.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET12,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 168.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET13,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 177.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET14,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 186.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET15,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 195.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET16,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 204.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET17,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 213.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET18,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 222.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET19,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 231.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET20,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 240.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET21,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 249.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET22,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 258.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET23,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 267.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET24,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 276.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET25,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 285.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET26,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 294.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET27,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 303.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET28,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 312.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET29,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 321.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET30,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 330.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET31,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 339.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET32,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 348.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET33,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 357.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET34,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 366.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET35,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 375.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET36,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 384.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET37,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 393.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET38,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 402.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET39,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 411.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET40,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 420.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET41,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 429.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET42,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 438.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET43,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 447.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET44,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 456.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET45,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 465.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET46,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 474.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET47,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 483.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET48,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	UIDesc.fPaddingY = 492.0f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_TOMMYGUN_BULLET49,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Notification()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//기관총탄알
	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Notification");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 20.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 600.f;
	UIDesc.fSizeY = 60.f;
	UIDesc.fPosZ = 0.05f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::UP;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_NOTIFICATION,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_UI_Dead()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//한번 보여주는 UI
	CUI::UIDESC UIDesc;
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_DeadSceneOnce");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 0.25f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_DEADONCE,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//여러번 반복되는 UI
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_DeadSceneLoop");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 0.0f;
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 1920.f;
	UIDesc.fSizeY = 1080.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = true;
	UIDesc.fAnimationRatio = 0.5f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_DEADLOOP,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Particle()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//GOON
	PARTICLEDESC particleDesc;
	particleDesc.iCurrentLevel = LEVEL_OUTDOOR;
	particleDesc.fSpread = _float3(9.f, 10.f, 9.f);
	particleDesc.fDirStr = 3.f;
	particleDesc.iStatic = LEVEL_STATIC;
	particleDesc.ParticleType = PARTICLE_GOON;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Goon_Gore");
	particleDesc.iIndex = 0;
	particleDesc.fGravityStr = 10.0f;
	particleDesc.vec3ParticleSize = _float3(0.2f, 0.2f, 0.2f);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
			
		}
		particleDesc.iIndex++;
	}

	//PIPE
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Pipeguy_Gore");
	particleDesc.iIndex = 0;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.ParticleType = PARTICLE_PIPE;
	particleDesc.fGravityStr = 10.0f;
	particleDesc.vec3ParticleSize = _float3(0.2f, 0.2f, 0.2f);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
		}
		particleDesc.iIndex++;
	}

	//BLOOD
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Bloodpool");
	particleDesc.iIndex = 0;
	particleDesc.ParticleType = PARTICLE_BLOOD;
	particleDesc.fHeight = 0.1f;//0.865;
	particleDesc.fGravityStr = 10.0f;
	particleDesc.vec3ParticleSize = _float3(0.8f, 0.8f, 0.8f);
	for (int k = 0; k < 100; k++)
	{
		pGameInstance->Add_BloodPool(&particleDesc);
	}

	
	//메탈
	particleDesc.iCurrentLevel = LEVEL_OUTDOOR;
	particleDesc.fSpread = _float3(9.f, 10.f, 9.f);
	particleDesc.fDirStr = 3.f;
	particleDesc.iStatic = LEVEL_STATIC;
	particleDesc.ParticleType = PARTICLE_METAL;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Particle_Metal");
	particleDesc.iIndex = 0;
	particleDesc.fGravityStr = 20.0f;
	particleDesc.vec3ParticleSize = _float3(0.2f, 0.2f, 0.2f);


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
		}
		particleDesc.iIndex++;
	}

	//나무상자
	particleDesc.iCurrentLevel = LEVEL_OUTDOOR;
	particleDesc.fSpread = _float3(9.f, 10.f, 9.f);
	particleDesc.fDirStr = 3.f;
	particleDesc.iStatic = LEVEL_STATIC;
	particleDesc.ParticleType = PARTICLE_WOODEN;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Particle_Wooden");
	particleDesc.iIndex = 0;
	particleDesc.fGravityStr = 20.0f;
	particleDesc.vec3ParticleSize = _float3(0.2f, 0.2f, 0.2f);


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
		}
		particleDesc.iIndex++;
	}

	//나무판자
	particleDesc.iCurrentLevel = LEVEL_OUTDOOR;
	particleDesc.fSpread = _float3(9.f, 10.f, 9.f);
	particleDesc.fDirStr = 3.f;
	particleDesc.iStatic = LEVEL_STATIC;
	particleDesc.ParticleType = PARTICLE_BOARD;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Particle_Board");
	particleDesc.iIndex = 0;
	particleDesc.fGravityStr = 10.0f;
	particleDesc.vec3ParticleSize = _float3(0.05f, 0.05f, 0.05f);


	for (int i = 0; i < 80; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
		}
		particleDesc.iIndex++;
	}

	//유리
	particleDesc.iCurrentLevel = LEVEL_OUTDOOR;
	particleDesc.fSpread = _float3(9.f, 10.f, 9.f);
	particleDesc.fDirStr = 3.f;
	particleDesc.iStatic = LEVEL_STATIC;
	particleDesc.ParticleType = PARTICLE_GLASS;
	particleDesc.fHeight = 0.5f;//0.865;
	particleDesc.strParticleTexTag = TEXT("Prototype_Component_Texture_Particle_Glass");
	particleDesc.iIndex = 0;
	particleDesc.fGravityStr = 20.0f;
	particleDesc.vec3ParticleSize = _float3(0.2f, 0.2f, 0.2f);


	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			pGameInstance->Add_Particle(&particleDesc);
		}
		particleDesc.iIndex++;
	}
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_Inventory(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	/* For.Inventory */ 
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Inventory"))))
		return E_FAIL;
	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Outdoor::Ready_Layer_Effect(const wstring& strLayerTag)
{
	


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	Safe_Release(pGameInstance);
	//CEffect::EFFECT_TYPE
	/* For.Effect */
	/*CEffect::EFFECT_TYPE eEffect_Type;
	for(int i = 0; i<10; i++)
	{
		eEffect_Type = CEffect::EFFECT_TYPE::EFFECT_BLOOD_IMPACTSTRIP;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
			TEXT("Prototype_GameObject_Effect"), &eEffect_Type)))
			return E_FAIL;
	}

	eEffect_Type = CEffect::EFFECT_TYPE::EFFECT_POW;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Effect"), & eEffect_Type)))
		return E_FAIL;

	for (int i = 0; i < 10; i++)
	{
		eEffect_Type = CEffect::EFFECT_TYPE::EFFECT_STRIP;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
			TEXT("Prototype_GameObject_Effect"), &eEffect_Type)))
		return E_FAIL;
	}
	eEffect_Type = CEffect::EFFECT_TYPE::EFFECT_SMASHED;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
		TEXT("Prototype_GameObject_Effect"), &eEffect_Type)))
		return E_FAIL;

	eEffect_Type = CEffect::EFFECT_TYPE::EFFECT_DAZESWIRL;
	for (int i = 0; i < 8; i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_OUTDOOR, strLayerTag,
			TEXT("Prototype_GameObject_Effect"), &eEffect_Type)))
			return E_FAIL;
	}
	Safe_Release(pGameInstance);*/

	
	return S_OK;
}



HRESULT CLevel_Outdoor::Ready_Layer_UI_Progress()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//참고용 
	/*CUI::UIDESC		UIDesc;

	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HudBox_Item_HealthBar");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 200.0f;
	UIDesc.fSizeX = 500.f;
	UIDesc.fSizeY = 500.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = true;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::CENTER;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PROGRESS,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;*/

#pragma region 플레이어 스태미나	
	//플레이어 스태미나바
	CUI::UIDESC		UIDesc;

	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_Player_Brown");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 32.0f;
	UIDesc.fSizeX = 300.f;
	UIDesc.fSizeY = 17.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PLAYER_STAMINA,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//플레이어 뒷 스태미나 바
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_Player_Red");
	UIDesc.fPosZ = 0.005f;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PLAYER_STAMINA_BACK,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;


	//플레이어 스태미나 배경
	
	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Player_BackGround");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 26.0f;
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 308.f;
	UIDesc.fSizeY = 70.f;
	UIDesc.fPosZ = 0.01f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::DOWN;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_PLAYER_STAMINA_BACKGROUND,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

#pragma region 적 체력
	//적 체력바


	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_Enemy_Red");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 119.0f;
	UIDesc.fSizeX = 300.f;
	UIDesc.fSizeY = 17.f;
	UIDesc.fPosZ = 0.0f;
	UIDesc.bIsRender = false;
	UIDesc.bIsFixed = true;
	UIDesc.eAnchor = CUI::UP;
	UIDesc.eRender = CUI::RENDER_TEST;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_HP,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Progress");
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_HealthBar_Enemy_DarkRed");
	UIDesc.fPosZ = 0.005f;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_HP_BACK,
		TEXT("Prototype_UI_Progress"), &UIDesc)))
		return E_FAIL;

	//GOON 체력 배경


	UIDesc.iLevelIndex = LEVEL_OUTDOOR;
	UIDesc.iLevelStatic = LEVEL_STATIC;
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Goon_BackGround");
	UIDesc.strRendererTag = TEXT("Prototype_Component_Renderer");
	UIDesc.strVIBuffer_RectTag = TEXT("Prototype_Component_VIBuffer_Rect");
	UIDesc.strTransformTag = TEXT("Prototype_Component_Transform");
	UIDesc.fPaddingX = 0.0f;
	UIDesc.fPaddingY = 80.0f;
	UIDesc.bIsFixed = true;
	UIDesc.fSizeX = 308.f;
	UIDesc.fSizeY = 60.f;
	UIDesc.fPosZ = 0.01f;
	UIDesc.bIsRender = false;
	UIDesc.eAnchor = CUI::UP;
	UIDesc.iWindowSizeX = g_iWinSizeX;
	UIDesc.iWindowSizeY = g_iWinSizeY;
	UIDesc.bIsAnimation = false;
	UIDesc.fAnimationRatio = 1.0f;
	UIDesc.eRender = CUI::RENDER_TEST;

	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_HP_BACKGROUND_GOON,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//MOOK 체력 배경
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Mook_BackGround");
	UIDesc.bIsRender = false;
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_HP_BACKGROUND_MOOK,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	//MALONE 체력 배경
	UIDesc.strTextureTag = TEXT("Prototype_Component_Texture_UI_Malone_BackGround");
	if (FAILED(pGameInstance->Add_UI(LEVEL_OUTDOOR, UI_ENEMY_HP_BACKGROUND_MALONE,
		TEXT("Prototype_UI_Fixed"), &UIDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Outdoor::Load_Map(const wstring& strFilePath, const wstring& strSectionTag, _bool bRespawnEnemy)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	std::ifstream file(strFilePath, ios_base::in | ios_base::binary);

	if (file.is_open())
	{
		size_t iObjectCount;
		file.read((char*)&iObjectCount, sizeof(size_t));

		for (size_t i = 0; i < iObjectCount; ++i)
		{
			OBJECTDESC tObjectDesc;

			std::wstring strPrototypeTag;
			std::wstring strLayerTag;
			// Load MeshType
			{
				MESH_TYPE eMeshType;
				file.read((char*)&eMeshType, sizeof(MESH_TYPE));

				tObjectDesc.eMeshType = eMeshType;
			}

			// Load ObjectType
			{
				OBJECT_TYPE eObjectType; 
				file.read((char*)&eObjectType, sizeof(OBJECT_TYPE));

				tObjectDesc.eObjectType = eObjectType;
				
				switch (eObjectType)
				{
				case Client::OBJECT_STATIC:
					strPrototypeTag = TEXT("Prototype_GameObject_Environment");
					strLayerTag = TEXT("Layer_Environment");
					break;
				case Client::OBJECT_FLOOR:
					strPrototypeTag = TEXT("Prototype_GameObject_Environment");
					strLayerTag = TEXT("Layer_Floor");
					break;
				case Client::OBJECT_INTERATIVE:
					strLayerTag = TEXT("Layer_Interactive");
					break;
				case Client::OBJECT_BILLBOARD:
					strLayerTag = TEXT("Layer_Interactive");
					break;
				case Client::OBJECT_EVENT_SECTION:
					strPrototypeTag = TEXT("Prototype_GameObject_EventCollider");
					strLayerTag = TEXT("Layer_Event_Collider");
					break;
				case Client::OBJECT_ENEMY:
					strLayerTag = TEXT("Layer_Enemy");
					break;
				case Client::OBJECT_TOOLTIP_EVENT:
				case Client::OBJECT_RADIO_EVENT:
				case Client::OBJECT_BGM_EVENT:
				case Client::OBJECT_ETC_EVENT:
					strPrototypeTag = TEXT("Prototype_GameObject_EventCollider");
					strLayerTag = TEXT("Layer_Event_Collider");
					break;
				default:
					break;
				}
			}

			// Load ItemType
			{
				ITEM_TYPE eItemType;
				file.read((char*)&eItemType, sizeof(ITEM_TYPE));

				tObjectDesc.eItemType = eItemType;

				if (tObjectDesc.eObjectType == OBJECT_INTERATIVE || tObjectDesc.eObjectType == OBJECT_BILLBOARD)
				{
					_int eItemtype = (_int)tObjectDesc.eItemType;
					//equipment
					if( 0 <= eItemtype && 100 >= eItemtype)
						strPrototypeTag = TEXT("Prototype_Item_Equipment");

					//consumable
					if (101 <= eItemtype && 200 >= eItemtype)
						strPrototypeTag = TEXT("Prototype_Item_Consumable");

					//interactive
					if (201 <= eItemtype && 300 >= eItemtype)
						strPrototypeTag = TEXT("Prototype_Item_Interactive");

					//throwonly
					if (301 <= eItemtype && 400 >= eItemtype)
						strPrototypeTag = TEXT("Prototype_Item_ThrowOnly");
					//throwonly
					if (999 == eItemtype )
						strPrototypeTag = TEXT("Prototype_Item_ThrowOnly");
				}

				// ... ㅋㅋ
				if (eItemType == ITEM_FAN || eItemType == ITEM_ENTRANCE_KEY || eItemType == ITEM_DOOR
					|| eItemType == ITEM_CLUB_DOOR || eItemType == ITEM_WOODEN_DOOR
					|| eItemType == ITEM_BUTTON || eItemType == ITEM_BLOODY_HAND_PRINT
					|| eItemType == ITEM_CLUB_DOOR_R || eItemType == ITEM_WOODEN_DOOR_R
					|| eItemType == ITEM_METAL_DOOR || eItemType == ITEM_METAL_DOOR_R
					|| eItemType == ITEM_METAL_CLUB_DOOR
					|| eItemType == ITEM_FANCY_DOOR || eItemType == ITEM_FANCY_DOOR_R)
				{
					strPrototypeTag = TEXT("Prototype_GameObject_Gimmick_Object");					
				}
			}

			ENEMY_TYPE2 eEnemyType;
			// Load EnemyType
			{
				file.read((char*)&eEnemyType, sizeof(ENEMY_TYPE2));

				if (tObjectDesc.eObjectType == OBJECT_ENEMY)
				{
					switch (eEnemyType)
					{
					case ENEMY_TYPE2::ENEMY_TYPE_GOON:
						strPrototypeTag = TEXT("Prototype_GameObject_Goon");
						break;
					case ENEMY_TYPE2::ENEMY_TYPE_PIPE_MAN:
						strPrototypeTag = TEXT("Prototype_GameObject_PipeGuy");
						break;
					case ENEMY_TYPE2::ENEMY_TYPE_MALONE:
						strPrototypeTag = TEXT("Prototype_GameObject_Boss");
						break;
					case ENEMY_TYPE2::ENEMY_TYPE_DELIA:
						strPrototypeTag = TEXT("Prototype_GameObject_Delia");
						strLayerTag = TEXT("Layer_Delia");
						break;
					default:
						break;
					}
				}
			}

			// Save Tooltip Type
			{
				TOOLTIP eTooltip;
				file.read((char*)& eTooltip, sizeof(TOOLTIP));

				tObjectDesc.eTooltipType = eTooltip;
			}

			// Save Radio Index
			{
				int iRadioIndex;
				file.read((char*)& iRadioIndex, sizeof(int));

				tObjectDesc.iRadioIndex = iRadioIndex;
			}

			// Load ObjectName
			{
				TCHAR strName[MAX_PATH];
				_int iNameLength;

				file.read((char*)&iNameLength, sizeof(_int));
				file.read((char*)strName, iNameLength + 2);
			}

			// Load Texture Path
			{
				TCHAR strTexturePath[MAX_PATH];
				size_t iPathLength;

				file.read((char*)&iPathLength, sizeof(size_t));
				file.read((char*)strTexturePath, iPathLength + 2);

				tObjectDesc.strTexturePath = strTexturePath;


			}

			// Load Transform
			{
				D3DXVECTOR3 vPosition;
				D3DXQUATERNION vQuaternion;
				D3DXVECTOR3 vScale;

				file.read((char*)&vPosition, sizeof(D3DXVECTOR3));
				file.read((char*)&vQuaternion, sizeof(D3DXQUATERNION));
				file.read((char*)&vScale, sizeof(D3DXVECTOR3));

				tObjectDesc.vPosition = vPosition;
				tObjectDesc.vQuaternion = vQuaternion;
				tObjectDesc.vScale = vScale;
			}

			// Load AlphaTesting
			{
				_bool bAlphaTesting;
				_int iAlphaRef;

				file.read((char*)&bAlphaTesting, sizeof(_bool));
				file.read((char*)&iAlphaRef, sizeof(_int));

				tObjectDesc.bAlphaTesting = bAlphaTesting;
				tObjectDesc.iAlphaRef = iAlphaRef;
			}

			// Load Tag
			{
				TCHAR strTag[MAX_PATH];
				_int iTagLength;

				file.read((char*)&iTagLength, sizeof(_int));
				file.read((char*)strTag, iTagLength + 2);
				
				tObjectDesc.strTextureTag = strTag;
			}

			// Load Sound Key
			{
				TCHAR strSoundKey[MAX_PATH];
				int iKeyLength;

				file.read((char*)&iKeyLength, sizeof(_int));
				file.read((char*)strSoundKey, iKeyLength + 2);

				tObjectDesc.strSoundKey = strSoundKey;
			}
			
			if (tObjectDesc.eObjectType == OBJECT_ENEMY)
			{
				// Init Enemy Matrix
				ENEMYOBJECTDESC tEnemyDesc;
				_float4x4 EnemyMatrix;
				D3DXMatrixIdentity(&EnemyMatrix);
				{
					// Set Position
					{
						EnemyMatrix._41 = tObjectDesc.vPosition.x;
						EnemyMatrix._42 = tObjectDesc.vPosition.y;
						EnemyMatrix._43 = tObjectDesc.vPosition.z;
					}

					// Set Rotation
					{
						D3DXQUATERNION vQuat;
						D3DXQuaternionNormalize(&vQuat, &tObjectDesc.vQuaternion);

						D3DXMATRIX Rotation;
						D3DXMatrixRotationQuaternion(&Rotation, &vQuat);

						D3DXVECTOR3 vRight, vUp, vLook;
						D3DXVec3TransformNormal(&vRight, &_float3(1.f, 0.f, 0.f), &Rotation);
						D3DXVec3TransformNormal(&vUp, &_float3(0.f, 1.f, 0.f), &Rotation);
						D3DXVec3TransformNormal(&vLook, &_float3(0.f, 0.f, 1.f), &Rotation);

						D3DXVec3Normalize(&vRight, &vRight);
						D3DXVec3Normalize(&vUp, &vUp);
						D3DXVec3Normalize(&vLook, &vLook);

						EnemyMatrix._11 = vRight.x; EnemyMatrix._12 = vRight.y; EnemyMatrix._13 = vRight.z;
						EnemyMatrix._21 = vUp.x; EnemyMatrix._22 = vUp.y; EnemyMatrix._23 = vUp.z;
						EnemyMatrix._31 = vLook.x; EnemyMatrix._32 = vLook.y; EnemyMatrix._33 = vLook.z;
					}
				}

				// Enemy Respawn Points
				if (bRespawnEnemy)
				{
					ENEMYRESPAWNDESC tRespawnDest;
					tRespawnDest.EnemyMatrix = EnemyMatrix;
					tRespawnDest.strEnemyPrototype = strPrototypeTag;
					
					if (eEnemyType == ENEMY_TYPE_GOON || eEnemyType == ENEMY_TYPE_PIPE_MAN)
					{
						m_vecEnemyRespawnPoints.push_back(tRespawnDest);
					}
				}

				tEnemyDesc.EnemyMatrix = EnemyMatrix;
				tEnemyDesc.strSoundKey = tObjectDesc.strSoundKey;
				if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, strLayerTag,
					strPrototypeTag, strSectionTag, &tEnemyDesc)))
				{
					return E_FAIL;
				}
			}
			else
			{
				if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, strLayerTag, strPrototypeTag, strSectionTag, &tObjectDesc)))
				{
					Safe_Release(pGameInstance);
					return E_FAIL;
				}
			}
		}

		file.close();
	}
	else
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

_bool CLevel_Outdoor::On_Section_Trigger(void* pArg)
{
	// TODO: Test Code
	static int iSectionIndex = 0;

	if (iSectionIndex == 0)
	{
		Ready_Lights_Section_2();

		_bool bEnable = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_SMOKE_ENABLE, &bEnable);

		++iSectionIndex;
	}
	else if (iSectionIndex == 1)
	{
		Ready_Lights_Section_3();

		++iSectionIndex;
	}
	else if (iSectionIndex == 2)
	{
		Ready_Lights_Section_4();

		_bool bEnable = false;
		CGameInstance::Get_Instance()->OnEvent(EVENT_SMOKE_ENABLE, &bEnable);

		++iSectionIndex;
	}
	else if (iSectionIndex == 3)
	{
		Ready_Lights_Section_5();

		++iSectionIndex;
	}
	else if (iSectionIndex == 4)
	{
		Ready_Lights_Section_6();
		CGameInstance::Get_Instance()->OnEvent(EVENT_TRAIN_COME);

		++iSectionIndex;
	}
	else if (iSectionIndex == 5)
	{
		Ready_Lights_Section_2F();

		FLOORCHANGEDESC desc;
		desc.pFrom = TEXT("Section_Outdoor");
		desc.pTo = TEXT("Section_2F");
		desc.vNewPosition = _float3(0.5f, 2.5f, 0.f);

		CGameInstance::Get_Instance()->OnEvent(EVENT_FLOOR_CHANGE, &desc);

		++iSectionIndex;
	}
	else if (iSectionIndex == 6)
	{
		Ready_Lights_Section_1F();

		FLOORCHANGEDESC desc;
		desc.pFrom = TEXT("Section_2F");
		desc.pTo = TEXT("Section_1F");
		desc.vNewPosition = _float3(0.f, 2.5f, -3.f);

		CGameInstance::Get_Instance()->OnEvent(EVENT_FLOOR_CHANGE, &desc);

		++iSectionIndex;
	}
	else if (iSectionIndex == 7)
	{
		FLOORCHANGEDESC desc;
		desc.pFrom = TEXT("Section_1F");
		desc.pTo = TEXT("Section_Ending");
		desc.vNewPosition = _float3(0.f, 0.f, 0.f);

		CGameInstance::Get_Instance()->OnEvent(EVENT_FLOOR_CHANGE, &desc);
		CGameInstance::Get_Instance()->Set_LateTick(false);


		CGameInstance::Get_Instance()->StopAll();
		CGameInstance::Get_Instance()->PlayBGM(TEXT("Music_SmokeAndMirrors.wav"), SOUND_BGM0, 1.f);

		++iSectionIndex;
	}
	
	return true;
}

_bool CLevel_Outdoor::On_Respawn_Event(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
#pragma region 웅성거리는 소리
	pGameInstance->StopSound(SOUND_ENEMY0);
	pGameInstance->StopSound(SOUND_ENEMY1);
	pGameInstance->StopSound(SOUND_ENEMY2);
	pGameInstance->StopSound(SOUND_ENEMY3);
	pGameInstance->StopSound(SOUND_ENEMY4);
	pGameInstance->Play_Sound(TEXT("FindDeadAlly4.wav") , SOUND_ENEMY0, 0.7f);
	pGameInstance->Play_Sound(TEXT("HisDead1 #5542.wav") , SOUND_ENEMY1, 0.7f);
	pGameInstance->Play_Sound(TEXT("ItsEverywhere2.wav") , SOUND_ENEMY2, 0.7f);
	pGameInstance->Play_Sound(TEXT("Shocked2.wav") , SOUND_ENEMY3, 0.7f);
	pGameInstance->Play_Sound(TEXT("ThatWasDrinkingBuddy.wav") , SOUND_ENEMY4, 0.7f);
#pragma endregion 웅성거리는 소리

	for (auto& mSpawnPoint : m_vecEnemyRespawnPoints)
	{
		if (FAILED(pGameInstance->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Enemy"),
			mSpawnPoint.strEnemyPrototype, TEXT("Section_2F"), &mSpawnPoint.EnemyMatrix)))
		{
			Safe_Release(pGameInstance);

			return false;
		}
	}

	Safe_Release(pGameInstance);

	return false;
}

CLevel_Outdoor* CLevel_Outdoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Outdoor* pInstance = new CLevel_Outdoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLevel_Outdoor");
	}

	return pInstance;
}

void CLevel_Outdoor::Free()
{
	__super::Free();

	//CGameInstance::Get_Instance()->Clear_Events();
}
