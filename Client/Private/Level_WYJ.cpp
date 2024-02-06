#include "stdafx.h"
#include "..\Public\Level_WYJ.h"

#include "GameInstance.h"
#include "Camera_Free.h"

CLevel_WYJ::CLevel_WYJ(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_WYJ::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;


	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Enemy(TEXT("Layer_Enemy"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_WYJ::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CLevel_WYJ::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("WYJ 레벨입니다."));
}

HRESULT CLevel_WYJ::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WYJ::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
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
 	if (FAILED(pGameInstance->Add_GameObject(LEVEL_WYJ, strLayerTag,
		TEXT("Prototype_GameObject_Camera_Free"), &CameraFreeDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WYJ::Ready_Layer_Player(const wstring & strLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Player */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_WYJ, strLayerTag,
		TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WYJ::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Terrain */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_WYJ, strLayerTag,
		TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;

}

HRESULT CLevel_WYJ::Ready_Layer_Enemy(const wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Enemy */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_WYJ, strLayerTag,
		TEXT("Prototype_GameObject_Goon"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_WYJ * CLevel_WYJ::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_WYJ*	pInstance = new CLevel_WYJ(pGraphic_Device);	

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CLevel_WYJ");
	}

	return pInstance;
}

void CLevel_WYJ::Free()
{
	__super::Free();


}
