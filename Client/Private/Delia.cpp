#include "stdafx.h"
#include "Delia.h"
#include <GameInstance.h>

#include "Player.h"

CDelia* CDelia::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDelia* pInstance = new CDelia(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CDelia");
	}

	return pInstance;
}

CGameObject* CDelia::Clone(void* pArg)
{
	CGameObject* pInstance = new CDelia(*this);

	if (FAILED(dynamic_cast<CEnemyBase*>(pInstance)->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CDelia");
	}

	return pInstance;
}

HRESULT CDelia::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDelia::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}

	m_pGameInstance = CGameInstance::Get_Instance();

	m_fSizeX = 0.55f;
	m_fSizeY = 1.2f;

	Set_SpawnPos();
	Set_InitMatrix(static_cast<_float4x4*>(pArg));

	CGameObject* pTmpPlayer = nullptr;
	pTmpPlayer = m_pGameInstance->GetPlayer();
	if (pTmpPlayer)
	{
		Set_Target(dynamic_cast<CPlayer*>(pTmpPlayer));
		m_pTargetTransform =
			dynamic_cast<CTransform*>(m_pTarget->Find_Component(TEXT("Com_Transform")));
	}

	m_pGameInstance->Subscribe(EVENT_EXECUTION_TRIGGER,
		std::bind(&CDelia::Interactive, this, std::placeholders::_1));

	Set_AnimProperty();

	return S_OK;
}

void CDelia::Tick(_float fTimeDelta)
{
}

void CDelia::Late_Tick(_float fTimeDelta)
{
	if (m_bEnable == false)
	{
		return;
	}

	Update_ToTarget();
	Set_TextureDir();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CDelia::Render()
{
	if (m_bEnable == false)
	{
		return S_OK;
	}
	Make_LookCam();

#pragma region Render Character

	_float4x4 finalMat;
	memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	finalMat = m_TextureMat;

	// 텍스쳐 스케일 조정
	_float4x4 matScaling;
	_float fSizeX = m_bRegularSize ? 1.f : m_fSizeX;
	_float fSizeY = m_bRegularSize ? 1.f : m_fSizeY;
	fSizeX = 0.55f;
	fSizeY = 1.2f;
	D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	finalMat = matScaling * finalMat; // S R T 순서라 matScaling먼저

	// Texture Matrix
	if (m_bRegularSize == false)
	{
		memcpy(&finalMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			sizeof(_float3));
	}

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	finalMat.m[3][1] -= 0.26f;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &finalMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(Get_CurTexture(m_eTextureDir, m_iCurAction)->Bind_OnGraphicDevice(static_cast<_uint>(m_fCurFrame))))
		return E_FAIL;

	m_pVIDeliaBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
#pragma endregion Render Character

	return S_OK;
}

void CDelia::Free()
{
	CGameObject::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIDeliaBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTarget);

	Safe_Release(m_EightDirTexturesArr[NORTH][STAND]);
	Safe_Release(m_EightDirTexturesArr[NORTH_EAST][STAND]);
	Safe_Release(m_EightDirTexturesArr[NORTH_WEST][STAND]);
	Safe_Release(m_EightDirTexturesArr[EAST][STAND]);
	Safe_Release(m_EightDirTexturesArr[WEST][STAND]);
	Safe_Release(m_EightDirTexturesArr[SOUTH_EAST][STAND]);
	Safe_Release(m_EightDirTexturesArr[SOUTH_WEST][STAND]);
	Safe_Release(m_EightDirTexturesArr[SOUTH][STAND]);

}

_bool CDelia::Interactive(void* pArg)
{
	if (m_fDist_BtwTarget < 1.5f)
	{
		CGameInstance::Get_Instance()->OnEvent(EVENT_DELIA);
		CGameInstance::Get_Instance()->OnEvent(EVENT_RESPAWN_2F);

		m_bEnable = false;

		return false;
	}

	return true;
}

HRESULT CDelia::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIDeliaBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	// Textures
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH"),
		TEXT("Com_Texture_Stand_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH_EAST"),
		TEXT("Com_Texture_Stand_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_NORTH_WEST"),
		TEXT("Com_Texture_Stand_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_EAST"),
		TEXT("Com_Texture_Stand_East"), (CComponent**)&m_EightDirTexturesArr[EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_WEST"),
		TEXT("Com_Texture_Stand_West"), (CComponent**)&m_EightDirTexturesArr[WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH"),
		TEXT("Com_Texture_Stand_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH_EAST"),
		TEXT("Com_Texture_Stand_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Delia_SOUTH_WEST"),
		TEXT("Com_Texture_Stand_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][STAND])))
		return E_FAIL;

	return S_OK;
}