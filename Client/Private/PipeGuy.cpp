#include "stdafx.h"
#include "PipeGuy.h"
#include <GameInstance.h>
#include "AwarenessGauge.h"
#include "EnemyPhysics.h"
#include "Enemy_HealthSystem.h"
#include "State_Idle.h"
#include "Player.h"
#include "State_Attacked.h"
#include "State_Aware.h"
#include "State_Cautious.h"

CPipeGuy* CPipeGuy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPipeGuy* pInstance = new CPipeGuy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CPipeGuy");
	}

	return pInstance;
}

CGameObject* CPipeGuy::Clone(void* pArg)
{
	CGameObject* pInstance = new CPipeGuy(*this);

	if (FAILED(dynamic_cast<CEnemyBase*>(pInstance)->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CPipeGuy");
	}

	return pInstance;
}

HRESULT CPipeGuy::Initialize_Prototype()
{
	return CEnemyBase::Initialize_Prototype();
}
static _int iCloneCount = 0;
HRESULT CPipeGuy::Initialize(void* pArg)
{
	m_eEnemy_Type = ENEMY_TYPE_PIPE_MAN;

#pragma region Init override
	if (FAILED(CGameObject::Initialize(pArg)))
	{
		return E_FAIL;
	}

#pragma region Add Components Override
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Blink"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* For.Com_VIBuffer_Awareness */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Awareness"), (CComponent**)&m_pVIBufferCom_Awareness)))
		return E_FAIL;

	//cube
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_CubeVIBuffer"), (CComponent**)&m_pCubeVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	/* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 0.8f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pTransformCom->Rotation(_float3(0, 1, 0), D3DXToRadian(225));

#pragma region Texture
#pragma region AwarenessIcon
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Alert"),
		TEXT("Com_Texture_AwarenessIconAlert"), (CComponent**)&m_AwarenessIcon_Alert)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Spooked"),
		TEXT("Com_Texture_AwarenessIconSpooked"), (CComponent**)&m_AwarenessIcon_Spooked)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Awareness_Cautious"),
		TEXT("Com_Texture_AwarenessIconCautious"), (CComponent**)&m_AwarenessIcon_Cautious)))
		return E_FAIL;
#pragma endregion AwarenessIcon
#pragma endregion Texture

	// FSM 추가하고
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FSM"),
		TEXT("Com_FSM"), (CComponent**)&m_pFSMCom, this)))
		return E_FAIL;

	

	//Physics
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
		return E_FAIL;


#pragma endregion Add Components Override


	Set_SpawnPos();

	Set_InitMatrix(static_cast<_float4x4*>(pArg));

	_float4x4 matScale, matTrans;
	D3DXMatrixIdentity(&m_matNotRot);
	D3DXMatrixScaling(&matScale, 0.23f, 0.5f, 0.23f);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	m_matNotRot *= matScale * matTrans;

	m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_matNotRot);
	// GameInstance
	m_pGameInstance = CGameInstance::Get_Instance();

	CGameObject* pTmpPlayer = nullptr;
	pTmpPlayer = CGameInstance::Get_Instance()->GetPlayer();
	if (pTmpPlayer)
	{
		Set_Target(dynamic_cast<CPlayer*>(pTmpPlayer));
		m_pTargetTransform =
			dynamic_cast<CTransform*>(m_pTarget->Find_Component(TEXT("Com_Transform")));
	}

	// Awareness Gauge
	m_pGauge = CAwarenessGauge::Create();
	// Health System
	m_pHealthSystem = CEnemy_HealthSystem::Create(this);
	// Physics
	m_pPhysics = CEnemyPhysics::Create(this);
	m_pPhysics->Set_Height(0.9f);

#pragma region Set_EventFunc
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Subscribe(EVENT_PLAYER_ATTACK,
		std::bind(&CPipeGuy::On_CheckHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_EXECUTION_TRIGGER,
		std::bind(&CPipeGuy::On_CheckTobeExecuted, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_PLAY_EXECUTION,
		std::bind(&CPipeGuy::On_GetExecutionHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_THROW_TO_ENEMY,
		std::bind(&CPipeGuy::On_CheckThrownHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_PLAYER_GUNFIRE,
		std::bind(&CPipeGuy::On_CheckGunFire, this, std::placeholders::_1));

	Safe_Release(pGameInstance);
#pragma endregion Set_EventFunc
#pragma endregion
	m_fSizeX = 0.5f;
	m_fSizeY = 1.5f;
	/*m_fSizeX = 1.3f;
	m_fSizeY = 1.3f;*/


	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}
	// 자식만의 State 푸쉬
	if (FAILED(Add_StateOnFSM()))
	{
		return E_FAIL;
	}

	// 애니메이션 별로 최대 프레임 설정
	Set_AnimProperty();

	
	// Sound Channel
	m_iSound_Channel = (++iCloneCount) % 5 + CHANNELID::SOUND_ENEMY0;

	// Find Layer
	m_pThisLayer = m_pGameInstance->Find_Layer(LEVEL_OUTDOOR, TEXT("Layer_Enemy"));
	if (m_pThisLayer == nullptr)
	{
		//MSG_BOX("Can't Find Layer");
		return S_OK;
	}

	return S_OK;
}

void CPipeGuy::Tick(_float fTimeDelta)
{
	CEnemyBase::Tick(fTimeDelta);

	// Temp
	_int pNextState = m_pFSMCom->Tick(fTimeDelta);

	m_pTransformCom->SetNextPos(m_pTransformCom->GetNextPos() + m_pPhysics->Tick(fTimeDelta));

	if (pNextState != FSM_NO_EVENT) // 다음 스테이트 정보가 비지 않았다면
	{
		//m_pFSMCom->Set_NextState(*static_cast<_uint*>(pNextState));
		//m_pFSMCom->Set_NextState(ENEMY_BASE_STATE::IDLE);
	}
}

void CPipeGuy::Late_Tick(_float fTimeDelta)
{
	CEnemyBase::Late_Tick(fTimeDelta);


}

HRESULT CPipeGuy::Render()
{
	if (m_bHit_byTrain == true)
		return S_OK;

	if (m_bExecuting) // 처형 씬
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;


#pragma region Render Character
	Make_LookCam(); // BillBoard, TextureMat 전용

	_float4x4 finalMat;
	memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	finalMat = m_TextureMat;

	_float fTempHeight = 0.f;
	fTempHeight = (m_iCurAction == ACTION_HEAVY_ATTACK || m_iCurAction == ACTION_HEAVY_ATTACK_WINDUP) ? 0.2f: 0.f;
	if (!m_bIsAlive)
		fTempHeight -= 0.2f;
	if (m_iCurAction == ACTION_HEADLESS)
	{
		fTempHeight = -0.4f;
	}

	finalMat.m[3][1] += fTempHeight;

	// 텍스쳐 스케일 조정
	_float4x4 matScaling;

	_float fSizeX = (m_iCurAction == ACTION_HEAVY_ATTACK || m_iCurAction == ACTION_HEAVY_ATTACK_WINDUP) ? 2.f : 1.3f;
	if (m_iCurAction == ACTION_HEADLESS)
	{
		fSizeX = 1.f;
	}
	_float fSizeY = fSizeX;

	D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	finalMat = matScaling * finalMat; // S R T 순서라 matScaling먼저

	//// Texture Matrix
	//if (m_bRegularSize == false)
	//{
	//	memcpy(&finalMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	//}
	//else // 정사각형
	//{
	//	if (m_bIsChopped == true)
	//	{
	//		D3DXMatrixIdentity(&matScaling);
	//		fSizeX = 1.6f;
	//		fSizeY = 1.6f;
	//		D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	//		finalMat = matScaling * finalMat; // S R T 순서라 matScaling먼저
	//		finalMat.m[3][0] += 0.2f;
	//		finalMat.m[3][1] += 0.2f;
	//		finalMat.m[3][2] += 0.2f;
	//	}
	//	_float3 vLandPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0, -0.4f, 0};
	//	memcpy(&finalMat.m[CTransform::STATE_POSITION][0], &vLandPos, sizeof(_float3));
	//}


	if (FAILED(SetUp_RenderState()))
		return E_FAIL;


	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &finalMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(Get_CurTexture(m_eTextureDir, m_iCurAction)->Bind_OnGraphicDevice(static_cast<_uint>(m_fCurFrame))))
		return E_FAIL;

	m_pVIBufferCom->SetWorldMatrix(finalMat);
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pVIBufferCom->SetViewMatrix(m_ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	m_pVIBufferCom->SetProjectionMatrix(m_ProjMatrix);
	m_pVIBufferCom->Set_Texture(Get_CurTexture(m_eTextureDir, m_iCurAction)->Get_Texture(static_cast<_uint>(m_fCurFrame)));

	m_pVIBufferCom->Render();


	if (FAILED(Release_RenderState()))
		return E_FAIL;
#pragma endregion Render Character

#pragma region Render AwarenessIcon
	if (m_bIsAlive == false || m_fDist_BtwTarget > 6.f || m_pGauge->Is_Acting() == false)
		return S_OK;

	if (m_pVIBufferCom_Awareness == nullptr)
		return E_FAIL;

	_float4x4 matScaling0{};
	D3DXMatrixScaling(&matScaling0, m_fIconSizeX, m_fIconSizeY, m_fIconSizeX);
	m_IconMat = matScaling0 * m_IconMat; // S R T 순서라 matScaling먼저



	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_IconMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (m_pFSMCom->Is_State(AWARE))
	{
		if (FAILED(m_AwarenessIcon_Alert->Bind_OnGraphicDevice(0)))
		{
			return E_FAIL;
		}
	}
	else if (m_pFSMCom->Is_State(CAUTIOUS))
	{
		if (FAILED(m_AwarenessIcon_Cautious->Bind_OnGraphicDevice(m_pGauge->Get_TextureFrame())))
		{
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_AwarenessIcon_Spooked->Bind_OnGraphicDevice(m_pGauge->Get_TextureFrame())))
		{
			return E_FAIL;
		}
	}

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom_Awareness->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
#pragma endregion Render AwarenessIcon
	return S_OK;
}

void CPipeGuy::Free()
{
	CEnemyBase::Free();

	for (int i = 0; i < ENEMY_TEXTURE_DIR_END; ++i)
	{
		Safe_Release(m_pTextureComArr[i]);
	}
	for (int i = 0; i < ONLY_SOUTH_ACTION_END; ++i)
	{
		Safe_Release(m_OnlySouthTexturesArr[i]);
	}

	for (int i = 0; i < ENEMY_TEXTURE_DIR_END; ++i)
	{
		for (int j = 0; j < EIGHT_DIR_ACTION_END; ++j)
		{
			Safe_Release(m_EightDirTexturesArr[i][j]);			
		}
	}

}

HRESULT CPipeGuy::Add_Components()
{
	// 자식만의 Com만
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// Texture Arr Test
#pragma region Legacy
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
		TEXT("Com_Texture_North"), (CComponent**)&m_pTextureComArr[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthEast"),
		TEXT("Com_Texture_NorthEast"), (CComponent**)&m_pTextureComArr[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthWest"),
		TEXT("Com_Texture_NorthWest"), (CComponent**)&m_pTextureComArr[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_East"),
		TEXT("Com_Texture_East"), (CComponent**)&m_pTextureComArr[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_West"),
		TEXT("Com_Texture_West"), (CComponent**)&m_pTextureComArr[4])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_South"),
		TEXT("Com_Texture_South"), (CComponent**)&m_pTextureComArr[5])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthEast"),
		TEXT("Com_Texture_SouthEast"), (CComponent**)&m_pTextureComArr[6])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthWest"),
		TEXT("Com_Texture_SouthWest"), (CComponent**)&m_pTextureComArr[7])))
		return E_FAIL; 
#pragma endregion Legacy

#pragma region Renewed
	// South
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_South"),
		TEXT("Com_Texture_Stand_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_South"),
		TEXT("Com_Texture_Strip_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][WALK])))
		return E_FAIL;	
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][RUN])))
		return E_FAIL;	
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FacePunched"),
		TEXT("Com_Texture_FacePunched"), (CComponent**)&m_OnlySouthTexturesArr[FACE_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BodyPunched"),
		TEXT("Com_Texture_BodyPunched"), (CComponent**)&m_OnlySouthTexturesArr[BODY_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_LowPunched"),
		TEXT("Com_Texture_LowPunched"), (CComponent**)&m_OnlySouthTexturesArr[LOW_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BattleDefault"),
		TEXT("Com_Texture_BattleDefault"), (CComponent**)&m_OnlySouthTexturesArr[BATTLE_DEFAULT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_InchForward"),
		TEXT("Com_Texture_InchForward"), (CComponent**)&m_OnlySouthTexturesArr[INCH_FORWARD])))
		return E_FAIL;;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Taunt"),
		TEXT("Com_Texture_Taunt"), (CComponent**)&m_OnlySouthTexturesArr[TAUNT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BasicAttack"),
		TEXT("Com_Texture_BasicAttack"), (CComponent**)&m_OnlySouthTexturesArr[BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeavyAttackWindUp"),
		TEXT("Com_Texture_HeavyAttackWindUp"), (CComponent**)&m_OnlySouthTexturesArr[HEAVY_ATTACK_WINDUP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeavyAttack"),
		TEXT("Com_Texture_HeavyAttack"), (CComponent**)&m_OnlySouthTexturesArr[HEAVY_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_AttackDefault"),
		TEXT("Com_Texture_AttackDefault"), (CComponent**)&m_OnlySouthTexturesArr[ATTACK_DEFAULT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_BattleIdle"),
		TEXT("Com_Texture_BattleIdle"), (CComponent**)&m_OnlySouthTexturesArr[BATTLE_IDLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strafing"),
		TEXT("Com_Texture_Strafing"), (CComponent**)&m_OnlySouthTexturesArr[STRAFING])))
		return E_FAIL;

	// Other
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
		TEXT("Com_Texture_Stand_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthEast"),
		TEXT("Com_Texture_Stand_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_NorthWest"),
		TEXT("Com_Texture_Stand_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_East"),
		TEXT("Com_Texture_Stand_East"), (CComponent**)&m_EightDirTexturesArr[EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_West"),
		TEXT("Com_Texture_Stand_West"), (CComponent**)&m_EightDirTexturesArr[WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthEast"),
		TEXT("Com_Texture_Stand_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_SouthWest"),
		TEXT("Com_Texture_Stand_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][STAND])))
		return E_FAIL;

#pragma region Walk
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_West"), (CComponent**)&m_EightDirTexturesArr[WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_East"), (CComponent**)&m_EightDirTexturesArr[EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Strip_South"),
		TEXT("Com_Texture_Strip_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][WALK])))
		return E_FAIL;
#pragma endregion Walk

#pragma region Run

	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_West"), (CComponent**)&m_EightDirTexturesArr[WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_East"), (CComponent**)&m_EightDirTexturesArr[EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Run_South"),
		TEXT("Com_Texture_Run_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][RUN])))
		return E_FAIL;

#pragma endregion Run

#pragma region suspicious
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_South"),
		TEXT("Com_Texture_Suspicious_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_SouthWest"),
		TEXT("Com_Texture_Suspicious_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_SouthEast"),
		TEXT("Com_Texture_Suspicious_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_East"),
		TEXT("Com_Texture_Suspicious_East"), (CComponent**)&m_EightDirTexturesArr[EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_West"),
		TEXT("Com_Texture_Suspicious_West"), (CComponent**)&m_EightDirTexturesArr[WEST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_North"),
		TEXT("Com_Texture_Suspicious_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_NorthEast"),
		TEXT("Com_Texture_Suspicious_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Suspicious_NorthWest"),
		TEXT("Com_Texture_Suspicious_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][SUSPICIOUS])))
		return E_FAIL;
#pragma endregion suspicious

#pragma region FallingStart - End
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FallingStart"),
		TEXT("Com_Texture_FallingStart"), (CComponent**)&m_OnlySouthTexturesArr[FALLING_START])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Falling"),
		TEXT("Com_Texture_Falling"), (CComponent**)&m_OnlySouthTexturesArr[FALLING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_FallingEnd"),
		TEXT("Com_Texture_FallingEnd"), (CComponent**)&m_OnlySouthTexturesArr[FALLING_END])))
		return E_FAIL;
#pragma endregion

	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Dead"),
		TEXT("Com_Texture_Dead"), (CComponent**)&m_OnlySouthTexturesArr[DEAD_LYING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Dazed"),
		TEXT("Com_Texture_Dazed"), (CComponent**)&m_OnlySouthTexturesArr[DAZED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_GetUp"),
		TEXT("Com_Texture_GetUp"), (CComponent**)&m_OnlySouthTexturesArr[GET_UP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Jump"),
		TEXT("Com_Texture_Jump"), (CComponent**)&m_OnlySouthTexturesArr[JUMP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_KarateChopped"),
		TEXT("Com_Texture_KarateChopped"), (CComponent**)&m_OnlySouthTexturesArr[KARATE_CHOPPED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_Headless_South"),
		TEXT("Com_Texture_Headless"), (CComponent**)&m_OnlySouthTexturesArr[HEADLESS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_HeadlessEnd_South"),
		TEXT("Com_Texture_HeadlessEnd"), (CComponent**)&m_OnlySouthTexturesArr[HEADLESS_END])))
		return E_FAIL;
#pragma endregion Renewed
	return S_OK;
}

HRESULT CPipeGuy::Add_StateOnFSM()
{
	// 부모 함수는 부모Init에서 호출하기 때문에 별도로 호출 안함
	list<CState*> tmpStateList;
	tmpStateList.emplace_back(CState_Idle_Enemy::Create(m_pGraphic_Device, this));
	tmpStateList.emplace_back(CState_Cautious_Enemy::Create(m_pGraphic_Device, this));

	AWAREDESC tAwareArguDesc{ 2.f, 0.2f, 4.f, 5.f, 10 };
	tmpStateList.emplace_back(CState_Aware_PipeGuy::Create(m_pGraphic_Device, this, &tAwareArguDesc));
	tmpStateList.emplace_back(CState_Attacked_Enemy::Create(m_pGraphic_Device, this));

	return m_pFSMCom->Add_NewState(&tmpStateList, this);
}

void CPipeGuy::Set_AnimProperty()
{
	m_iAnimFrameMaxArr[ACTION_STAND] = 1;
	m_iAnimFrameMaxArr[ACTION_WALK] = 19;
	m_iAnimFrameMaxArr[ACTION_RUN] = 21;
	m_iAnimFrameMaxArr[ACTION_SUSPICIOUS] = 1;
	m_iAnimFrameMaxArr[ACTION_FACE_PUNCHED] = 8;
	m_iAnimFrameMaxArr[ACTION_BODY_PUNCHED] = 3;
	m_iAnimFrameMaxArr[ACTION_LOW_PUNCHED] = 1;
	m_iAnimFrameMaxArr[ACTION_BATTLE_DEFAULT] = 1;
	m_iAnimFrameMaxArr[ACTION_INCH_FORWARD] = 5;
	m_iAnimFrameMaxArr[ACTION_TAUNT] = 11;
	m_iAnimFrameMaxArr[ACTION_BASIC_ATTACK] = 9;
	m_iAnimFrameMaxArr[ACTION_HEAVY_ATTACK_WINDUP] = 1;
	m_iAnimFrameMaxArr[ACTION_HEAVY_ATTACK] = 14;
	m_iAnimFrameMaxArr[ACTION_BATTLE_IDLE] = 2;
	m_iAnimFrameMaxArr[ACTION_STRAFING] = 7;
	m_iAnimFrameMaxArr[ACTION_FALLING_START] = 3;
	m_iAnimFrameMaxArr[ACTION_FALLING] = 1;
	m_iAnimFrameMaxArr[ACTION_FALLING_END] = 2;
	m_iAnimFrameMaxArr[ACTION_DEAD_LYING] = 1;
	m_iAnimFrameMaxArr[ACTION_DAZED] = 14;
	m_iAnimFrameMaxArr[ACTION_GET_UP] = 7;
	m_iAnimFrameMaxArr[ACTION_JUMP] = 8;
	m_iAnimFrameMaxArr[ACTION_HEADLESS] = 18;


	m_fAnimSpeedArr[ACTION_STAND] = 1;
	m_fAnimSpeedArr[ACTION_WALK] = 1.f;
	m_fAnimSpeedArr[ACTION_RUN] = 1.8f;
	m_fAnimSpeedArr[ACTION_SUSPICIOUS] = 1.f;
	m_fAnimSpeedArr[ACTION_FACE_PUNCHED] = 1.4f;
	m_fAnimSpeedArr[ACTION_BODY_PUNCHED] = 1.4f;
	m_fAnimSpeedArr[ACTION_LOW_PUNCHED] = 1;
	m_fAnimSpeedArr[ACTION_BATTLE_DEFAULT] = 1;
	m_fAnimSpeedArr[ACTION_INCH_FORWARD] = 0.73f;
	m_fAnimSpeedArr[ACTION_TAUNT] = 1.5f;
	m_fAnimSpeedArr[ACTION_BASIC_ATTACK] = 1.4f;
	m_fAnimSpeedArr[ACTION_HEAVY_ATTACK_WINDUP] = 1;
	m_fAnimSpeedArr[ACTION_HEAVY_ATTACK] = 1.1f;
	m_fAnimSpeedArr[ACTION_BATTLE_IDLE] = 1;
	m_fAnimSpeedArr[ACTION_STRAFING] = 1;
	m_fAnimSpeedArr[ACTION_FALLING_START] = 1;
	m_fAnimSpeedArr[ACTION_FALLING] = 1;
	m_fAnimSpeedArr[ACTION_FALLING_END] = 1.5f;
	m_fAnimSpeedArr[ACTION_DEAD_LYING] = 1;
	m_fAnimSpeedArr[ACTION_DAZED] = 1;
	m_fAnimSpeedArr[ACTION_GET_UP] = 1;
	m_fAnimSpeedArr[ACTION_JUMP] = 1;
	m_fAnimSpeedArr[ACTION_HEADLESS] = 0.8f;

	memcpy(m_fAnimDefaultSpeedArr, m_fAnimSpeedArr, sizeof _uint * ALL_ACTION_STATE_END);
}
