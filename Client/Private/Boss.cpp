#include "stdafx.h"
#include "Boss.h"
#include <GameInstance.h>
#include "AwarenessGauge.h"
#include "EnemyPhysics.h"
#include "Enemy_HealthSystem.h"
#include "State_Idle.h"
#include "Player.h"
#include "State_Attacked.h"
#include "State_Aware.h"
#include "State_Cautious.h"
#include "Layer.h"

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoss* pInstance = new CBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CBoss");
	}

	return pInstance;
}

CGameObject* CBoss::Clone(void* pArg)
{
	CGameObject* pInstance = new CBoss(*this);

	if (FAILED(dynamic_cast<CEnemyBase*>(pInstance)->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CBoss");
	}

	return pInstance;
}

HRESULT CBoss::Initialize_Prototype()
{
	return CEnemyBase::Initialize_Prototype();
}

HRESULT CBoss::Initialize(void* pArg)
{

	m_eEnemy_Type = ENEMY_TYPE_MALONE;

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
	/* For.Com_Transform */
	/* °¢ °´Ã¼µéÀÌ Æ®·£½ºÆûÀ» º¹Á¦ÇÏ¿© °´Ã¼¾È¿¡ º¸°üÇÒ ¶§! °´Ã¼°¡ ¿òÁ÷, È¸ÀüÇØ¾ßÇÑ¤¤ ¼Óµµ¸¦ ÀúÀåÇÒ °ÍÀÌ´Ù. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 0.8f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pTransformCom->Rotation(_float3(0, 1, 0), D3DXToRadian(225));


	// FSM Ãß°¡ÇÏ°í
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
	D3DXMatrixScaling(&matScale, 0.23f, 1.f, 0.23f);
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
	m_pPhysics->Set_Height(1.1f);

#pragma region Set_EventFunc
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Subscribe(EVENT_PLAYER_ATTACK,
		std::bind(&CBoss::On_CheckHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_EXECUTION_TRIGGER,
		std::bind(&CBoss::On_CheckTobeExecuted, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_PLAY_EXECUTION,
		std::bind(&CBoss::On_GetExecutionHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_THROW_TO_ENEMY,
		std::bind(&CBoss::On_CheckThrownHit, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_PLAYER_GUNFIRE,
		std::bind(&CBoss::On_CheckGunFire, this, std::placeholders::_1));

	Safe_Release(pGameInstance);
#pragma endregion Set_EventFunc
#pragma endregion
	m_fSizeX = 0.5f;
	m_fSizeY = 1.8f;
	/*m_fSizeX = 1.3f;
	m_fSizeY = 1.3f;*/


	if (FAILED(Add_Components())) // Texture
	{
		return E_FAIL;
	}
	// ÀÚ½Ä¸¸ÀÇ State Çª½¬
	if (FAILED(Add_StateOnFSM()))
	{
		return E_FAIL;
	}

	// ¾Ö´Ï¸ÞÀÌ¼Ç º°·Î ÃÖ´ë ÇÁ·¹ÀÓ ¼³Á¤
	Set_AnimProperty();

	// Sound Channel
	m_iSound_Channel = CHANNELID::SOUND_ENEMY2;
	

	return S_OK;
}

void CBoss::Tick(_float fTimeDelta)
{
	if (m_bRedStart)
	{
		m_fRedTime += fTimeDelta;
		if (m_fRedTime > 0.f && m_fRedTime < 0.2f)
		{
			Set_Ratio(Get_Ratio() + 0.01f);
		}
		else if (m_fRedTime >= 0.2f && m_fRedTime < 0.4f)
		{
			Set_Ratio(Get_Ratio() - 0.01f);
		}
		else
		{
			Set_Ratio(0.f);
			m_bRedStart = false;
		}
	}

	if (GetAsyncKeyState('K') & 0x8000)
	{
		Make_Aware();
	}

	
	if (m_bTriggered == false && m_fDist_BtwTarget < 8.8f)
	{
		Make_Aware();
	}


#pragma region Override
	if (!m_bIsAlive)
		m_fIsDeadTime += fTimeDelta;

	if (m_fIsDeadTime > 1.f)
		m_bIsNotTick = true;

	if (m_bIsNotTick)
		return;


	CGameObject::Tick(fTimeDelta);
	m_pTransformCom->SetNextPos(_float3(0, 0, 0));

	//Health
	if (m_bIsAlive == true)
	{
		_bool bMadeDazed = false;

		if (m_pHealthSystem->Tick(fTimeDelta, bMadeDazed) == false) // If Dead
		{
			Set_NextAction(ACTION_BOSS_FALLING_START);
			m_pHealthSystem->Make_Dead();
			Set_Dead();
			Play_DyingSound();
			m_bRegularSize = true;

			// TODO: DEMO CODE
			{
				// 총 떨구기
				OBJECTDESC tObjectDesc;
				tObjectDesc.strTextureTag = TEXT("AutoPistol");
				tObjectDesc.eItemType = ITEM_AUTOMATIC_PISTOL;
				tObjectDesc.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				tObjectDesc.vPosition.y -= 0.3f;
				tObjectDesc.vQuaternion = D3DXQUATERNION(0.f, 0.f, 0.f, 0.f);
				tObjectDesc.vScale = { 0.2f, 0.15f, 0.2f };
				tObjectDesc.bAlphaTesting = true;
				tObjectDesc.eMeshType = MESH_BILLBOARD;
				tObjectDesc.eObjectType = OBJECT_BILLBOARD;
				CGameInstance::Get_Instance()->Add_Section_GameObject(LEVEL_OUTDOOR, TEXT("Layer_Interactive"), TEXT("Prototype_Item_Equipment"), TEXT("Section_2F"), &tObjectDesc);
			}
		}
	}

	// Animation Frame
	m_fCurFrame += static_cast<_float>(m_iBossAnimFrameMaxArr[m_iCurAction]) *
		m_fBossAnimSpeedArr[m_iCurAction] * fTimeDelta;

	if (m_fCurFrame >= m_iBossAnimFrameMaxArr[m_iCurAction])
	{
		m_fCurFrame = 0.f;
		++m_iAnimPlayedCount;
	}
#pragma endregion

	m_pFSMCom->Tick(fTimeDelta);
	m_pTransformCom->SetNextPos(m_pTransformCom->GetNextPos() + m_pPhysics->Tick(fTimeDelta));
}

void CBoss::Late_Tick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);


	if (m_fIsDeadTime > 1.f)
		m_bIsNotTick = true;

	if (m_bIsNotTick)
		return;

	CGameObject::Late_Tick(fTimeDelta);

	if (m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) == false)
	{
		m_pPhysics->GroundPhysics(m_pTransformCom, LEVEL_OUTDOOR, TEXT("Layer_Floor"), fTimeDelta);
	}

	//if (m_bInAir)
	//if (m_bInAir)
	//{

	//	// Landing
	//	if ()
	//		== true) // ¶¥¿¡ ´ê¾ÒÀ¸¸é
	//	{
	//		if (m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE))
	//		{
	//			m_bInAir = false;
	//		}
	//	}
	//}

	m_pFSMCom->Late_Tick(fTimeDelta);

	if (m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) == false)
	{
		// OBB Collision
		_float4x4 matScale, matTrans;
		D3DXMatrixIdentity(&m_matNotRot);
		//D3DXMatrixScaling(&matScale, 0.23f, 1.f, 0.3f);
		D3DXMatrixScaling(&matScale, 0.25f, 0.5f, 0.25f);
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y + 0.15f, vPos.z);
		m_matNotRot *= matScale * matTrans;
		m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_matNotRot);

		CGameObject* pGameObject;
		_bool bCollided = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Environment"), &pGameObject);
		if (!bCollided)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
		}

	}
	else //IdleÀÏ ¶§
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->GetNextPos());
	}


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->GetNextPos());
#pragma endregion

	Update_ToTarget(); // ÀÚ½Å -> ÇÃ·¹ÀÌ¾î º¤ÅÍ ¾÷µ¥ÀÌÆ®
	Set_TextureDir();

	//if (true == m_bIsAlive)
	//{
	//	_float4x4 MatPlayerWorldInverse = m_pTargetTransform->Get_WorldMatrix_Inverse();
	//	_float3 EnemyWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//	_float3 EnemyLocalPos = *D3DXVec3TransformCoord(&EnemyLocalPos, &EnemyWorldPos, &MatPlayerWorldInverse);
	//	//³ªÁß¿¡ ´Ù½Ã È°¼ºÈ­
	//	EnemyLocalPos.y = 0.0f;

	//	_float Rad = D3DXToRadian(50.f);
	//	_float fDotProduct = D3DXVec3Dot(&_float3(0.0f, 0.0f, 1.0f), D3DXVec3Normalize(&EnemyLocalPos, &EnemyLocalPos));
	//	if (m_fDamageRange >= m_fDist_BtwTarget && 0.f <= EnemyLocalPos.z && fDotProduct >= cosf(Rad))
	//	{
	//		HITDISTANCEDESC hitDesc;
	//		hitDesc.pEnemy = this;
	//		hitDesc.bInDistance = true;
	//		m_pGameInstance->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
	//		return;
	//	}
	//	else
	//	{
	//		HITDISTANCEDESC hitDesc;
	//		hitDesc.pEnemy = this;
	//		hitDesc.bInDistance = false;
	//		m_pGameInstance->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
	//		return;
	//	}
	//}


}

HRESULT CBoss::Render()
{
	if (m_bHit_byTrain == true)
		return S_OK;

	if (m_bExecuting) // Ã³Çü ¾À
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;


#pragma region Render Character
	Make_LookCam(); // BillBoard, TextureMat Àü¿ë

	_float4x4 finalMat;
	memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	finalMat = m_TextureMat;

	_float fTempHeight = -0.15f;
	switch (m_iCurAction)
	{
	case ACTION_BOSS_FALLING:
		fTempHeight += -0.3f;
		break;
	case ACTION_BOSS_FALLING_END:
		fTempHeight += -0.3f;
		break;
	case ACTION_BOSS_DEAD_LYING:
		fTempHeight += -0.17f;
		break;
	default:
		break;
	}
	//fTempHeight = m_iCurAction == ACTION_BOSS_FALLING? -0.3f : 0.f;
	/*if (!m_bIsAlive)
		fTempHeight -= 0.2f;*/

	finalMat.m[3][1] += fTempHeight;

	// ÅØ½ºÃÄ ½ºÄÉÀÏ Á¶Á¤
	_float4x4 matScaling;

	_float fSizeX =  1.8f;

	_float fSizeY = fSizeX;

	D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	finalMat = matScaling * finalMat; // S R T ¼ø¼­¶ó matScaling¸ÕÀú


	if (FAILED(SetUp_RenderState()))
		return E_FAIL;


	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &finalMat);

	/* ÀåÄ¡¿¡ ÅØ½ºÃÄ¸¦ ¹ÙÀÎµùÇÑ´Ù. */
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
	
	return S_OK;
}

void CBoss::Free()
{
	CGameObject::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferCom_Awareness);
	Safe_Release(m_pFSMCom);
	Safe_Release(m_pPhysicsCom);
	Safe_Release(m_pTarget);
	Safe_Release(m_pGauge);
	Safe_Release(m_pHealthSystem);
	Safe_Release(m_pPhysics);
	Safe_Release(m_pCubeVIBufferCom);
	Safe_Release(m_pDebugTextureCom);

	for (int i = 0; i < BOSS_ONLY_SOUTH_ACTION_END; ++i)
	{
		Safe_Release(m_BossOnlySouthTexturesArr[i]);
	}

	for (int i = 0; i < ENEMY_TEXTURE_DIR_END; ++i)
	{
		for (int j = 0; j < BOSS_EIGHT_DIR_ACTION_END; ++j)
		{
			Safe_Release(m_BossEightDirTexturesArr[i][j]);
		}
	}
}

HRESULT CBoss::Add_Components()
{
	// ÀÚ½Ä¸¸ÀÇ Com¸¸
	/* For.Com_Texture */
	/*if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;*/

	// Texture Arr Test
#pragma region Legacy
	/*if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PipeGuy_North"),
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
		return E_FAIL;*/
#pragma endregion Legacy

#pragma region Eight Dir
	// South
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_South"),
		TEXT("Com_Texture_Stand_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_South"),
		TEXT("Com_Texture_Strip_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_South"),
		TEXT("Com_Texture_BasicAttack_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_South"),
		TEXT("Com_Texture_BattleIdle_Near_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_South"),
		TEXT("Com_Texture_BattleIdle_Far_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_South"),
		TEXT("Com_Texture_BattleIdle_Fire_South"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH][BOSS_FIRE])))
		return E_FAIL;
	// North
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_North"),
		TEXT("Com_Texture_Stand_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_North"),
		TEXT("Com_Texture_Strip_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_North"),
		TEXT("Com_Texture_BasicAttack_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_North"),
		TEXT("Com_Texture_BattleIdle_Near_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_North"),
		TEXT("Com_Texture_BattleIdle_Far_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_North"),
		TEXT("Com_Texture_BattleIdle_Fire_North"), (CComponent**)&m_BossEightDirTexturesArr[NORTH][BOSS_FIRE])))
		return E_FAIL;

	// NorthEast
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_NorthEast"),
		TEXT("Com_Texture_Stand_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_NorthEast"),
		TEXT("Com_Texture_Strip_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_NorthEast"),
		TEXT("Com_Texture_BasicAttack_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_NorthEast"),
		TEXT("Com_Texture_BattleIdle_Near_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_NorthEast"),
		TEXT("Com_Texture_BattleIdle_Far_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_NorthEast"),
		TEXT("Com_Texture_BattleIdle_Fire_NorthEast"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_EAST][BOSS_FIRE])))
		return E_FAIL;

	// East
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_East"),
		TEXT("Com_Texture_Stand_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_East"),
		TEXT("Com_Texture_Strip_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_East"),
		TEXT("Com_Texture_BasicAttack_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_East"),
		TEXT("Com_Texture_BattleIdle_Near_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_East"),
		TEXT("Com_Texture_BattleIdle_Far_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_East"),
		TEXT("Com_Texture_BattleIdle_Fire_East"), (CComponent**)&m_BossEightDirTexturesArr[EAST][BOSS_FIRE])))
		return E_FAIL;

	// SouthEast
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_SouthEast"),
		TEXT("Com_Texture_Stand_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_SouthEast"),
		TEXT("Com_Texture_BasicAttack_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_SouthEast"),
		TEXT("Com_Texture_BattleIdle_Near_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_SouthEast"),
		TEXT("Com_Texture_BattleIdle_Far_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_SouthEast"),
		TEXT("Com_Texture_BattleIdle_Fire_SouthEast"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_EAST][BOSS_FIRE])))
		return E_FAIL;

	// SouthWest
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_SouthWest"),
		TEXT("Com_Texture_Stand_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_SouthWest"),
		TEXT("Com_Texture_BasicAttack_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_SouthWest"),
		TEXT("Com_Texture_BattleIdle_Near_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_SouthWest"),
		TEXT("Com_Texture_BattleIdle_Far_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_SouthWest"),
		TEXT("Com_Texture_BattleIdle_Fire_SouthWest"), (CComponent**)&m_BossEightDirTexturesArr[SOUTH_WEST][BOSS_FIRE])))
		return E_FAIL;

	// West
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_West"),
		TEXT("Com_Texture_Stand_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_West"),
		TEXT("Com_Texture_Strip_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_West"),
		TEXT("Com_Texture_BasicAttack_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_West"),
		TEXT("Com_Texture_BattleIdle_Near_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_West"),
		TEXT("Com_Texture_BattleIdle_Far_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_West"),
		TEXT("Com_Texture_BattleIdle_Fire_West"), (CComponent**)&m_BossEightDirTexturesArr[WEST][BOSS_FIRE])))
		return E_FAIL;

	// NorthWest
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_NorthWest"),
		TEXT("Com_Texture_Stand_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Strip_NorthWest"),
		TEXT("Com_Texture_Strip_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BasicAttack_NorthWest"),
		TEXT("Com_Texture_BasicAttack_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Near_NorthWest"),
		TEXT("Com_Texture_BattleIdle_Near_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][BATTLE_IDLE_NEAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BattleIdle_Far_NorthWest"),
		TEXT("Com_Texture_BattleIdle_Far_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][BATTLE_IDLE_FAR])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Fire_NorthWest"),
		TEXT("Com_Texture_BattleIdle_Fire_NorthWest"), (CComponent**)&m_BossEightDirTexturesArr[NORTH_WEST][BOSS_FIRE])))
		return E_FAIL;

#pragma endregion Eight Dir

#pragma region OnlySouth
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_BodyPunched"),
		TEXT("Com_Texture_BodyPunched"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::BODY_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Dazed"),
		TEXT("Com_Texture_Dazed"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::DAZED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FacePunched"),
		TEXT("Com_Texture_FacePunched"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::FACE_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FallingEnd"),
		TEXT("Com_Texture_FallingEnd"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::FALLING_END])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_FallingStart"),
		TEXT("Com_Texture_FallingStart"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::FALLING_START])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_GetUp"),
		TEXT("Com_Texture_GetUp"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::GET_UP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_LowPunched"),
		TEXT("Com_Texture_LowPunched"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::LOW_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Reloading"),
		TEXT("Com_Texture_Reloading"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::RELOADING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollEast"),
		TEXT("Com_Texture_RollEast"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::ROLL_EAST])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollWest"),
		TEXT("Com_Texture_RollWest"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::ROLL_WEST])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollEastReverse"),
		TEXT("Com_Texture_RollEastReverse"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::ROLL_EAST_REVERSE])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_RollWestReverse"),
		TEXT("Com_Texture_RollWestReverse"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::ROLL_WEST_REVERSE])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Run_South"),
		TEXT("Com_Texture_Run"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Taunt"),
		TEXT("Com_Texture_Taunt"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::TAUNT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Dead"),
		TEXT("Com_Texture_Dead"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::DEAD_LYING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Boss_Falling"),
		TEXT("Com_Texture_Falling"), (CComponent**)&m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION::FALLING])))
		return E_FAIL;

#pragma endregion OnlySouth
	return S_OK;
}

HRESULT CBoss::Add_StateOnFSM()
{
	// ºÎ¸ð ÇÔ¼ö´Â ºÎ¸ðInit¿¡¼­ È£ÃâÇÏ±â ¶§¹®¿¡ º°µµ·Î È£Ãâ ¾ÈÇÔ
	list<CState*> tmpStateList;
	tmpStateList.emplace_back(CState_Idle_Boss::Create(m_pGraphic_Device, this));
	tmpStateList.emplace_back(CState_Cautious_Boss::Create(m_pGraphic_Device, this));

	AWAREDESC tAwareArguDesc{ 2.f, 0.2f, 4.f, 5.f, 10 };
	m_pState_Aware = CState_Aware_Boss::Create(m_pGraphic_Device, this, &tAwareArguDesc);
	tmpStateList.emplace_back(m_pState_Aware);
	tmpStateList.emplace_back(CState_Attacked_Boss::Create(m_pGraphic_Device, this));

	return m_pFSMCom->Add_NewState(&tmpStateList, this);
}

_int CBoss::LateTick_NotAware(_float fTimeDelta)
{
	// °¢µµ °è»ê ÈÄ ÅØ½ºÃÄ ¹æÇâ Á¤ÇÏ°í °¡µµ °ÔÀÌÁö Áõ°¡ ½ºÇÇµå °áÁ¤
	Set_TextureDir();

	return FSM_NO_EVENT;
}
	
void CBoss::Set_AnimProperty()
{
	m_iBossAnimFrameMaxArr[ACTION_BOSS_STAND] = 15;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_WALK] = 29;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BATTLE_IDLE_NEAR] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BATTLE_IDLE_FAR] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BASIC_ATTACK] = 7;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BOSS_FIRE] = 9;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_FACE_PUNCHED] = 8;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BODY_PUNCHED] = 3;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_LOW_PUNCHED] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_BATTLE_DEFAULT] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_TAUNT] = 18;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_FALLING_START] = 3;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_FALLING] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_FALLING_END] = 2;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_DEAD_LYING] = 1;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_DAZED] = 14;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_GET_UP] = 7;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_RELOADING] = 18;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_ROLL_EAST] = 20;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_ROLL_WEST] = 20;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_ROLL_EAST_REVERSE] = 20;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_ROLL_WEST_REVERSE] = 20;
	m_iBossAnimFrameMaxArr[ACTION_BOSS_RUN] = 10;

	m_fBossAnimSpeedArr[ACTION_BOSS_STAND] = 0.8f;
	m_fBossAnimSpeedArr[ACTION_BOSS_WALK] = 1.f;
	m_fBossAnimSpeedArr[ACTION_BOSS_BATTLE_IDLE_NEAR] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_BATTLE_IDLE_FAR] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_BASIC_ATTACK] = 2.5f;
	m_fBossAnimSpeedArr[ACTION_BOSS_BOSS_FIRE] = 2.f;
	m_fBossAnimSpeedArr[ACTION_BOSS_FACE_PUNCHED] = 2.5f;//1.4f;
	m_fBossAnimSpeedArr[ACTION_BOSS_BODY_PUNCHED] = 2.5f;//1.4f;
	m_fBossAnimSpeedArr[ACTION_BOSS_LOW_PUNCHED] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_BATTLE_DEFAULT] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_TAUNT] = 1.f;
	m_fBossAnimSpeedArr[ACTION_BOSS_FALLING_START] = 1.2f;
	m_fBossAnimSpeedArr[ACTION_BOSS_FALLING] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_FALLING_END] = 1.5f;
	m_fBossAnimSpeedArr[ACTION_BOSS_DEAD_LYING] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_DAZED] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_GET_UP] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_RELOADING] = 1.f;
	m_fBossAnimSpeedArr[ACTION_BOSS_ROLL_EAST] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_ROLL_WEST] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_ROLL_EAST_REVERSE] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_ROLL_WEST_REVERSE] = 1;
	m_fBossAnimSpeedArr[ACTION_BOSS_RUN] = 2.f;
}

_bool CBoss::Check_AttackSuccess()
{
	// Á¤¸é && °Å¸® ³»
	if((m_eTextureDir == ENEMY_TEXTURE_DIR::SOUTH || m_eTextureDir == ENEMY_TEXTURE_DIR::SOUTH_EAST || m_eTextureDir == ENEMY_TEXTURE_DIR::SOUTH_WEST)
		&& CEnemyBase::Check_AttackSuccess())
	{
		return true;
	}
	else
	{
		return false;		
	}

}

_bool CBoss::Chase_Target(_float fMinDistance, _float fWalkMaxDistance, _float fTimeDelta)
{
	_float fSpeed = 1.f;

	// °Èµç ¶Ùµç µû¶ó°¡¾ß ÇÒ ¶§
	if (m_fDist_BtwTarget >= fMinDistance)
	{
		if (m_fDist_BtwTarget > fWalkMaxDistance) // ¶Ù±â
		{
			Set_NextAction(ACTION_BOSS_RUN);
			fSpeed = 5.f;
		}
		else // °È±â
		{
			Set_NextAction(ACTION_WALK);
			fSpeed = 2.5f;
		}
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		m_pTransformCom->Go_Straight(fTimeDelta * fSpeed);
		return true;
	}
	else // ±×¸¸ µû¶ó°¡µµ µÇ´Â °Å¸®¶ó¸é
	{
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		return false;
	}
}

CTexture* CBoss::Get_CurTexture(ENEMY_TEXTURE_DIR eTextureDir, _uint curAction)
{
	_bool bOnlySouth = curAction > BOSS_FIRE;
	if (bOnlySouth == true)
	{
		return m_BossOnlySouthTexturesArr[curAction - ACTION_BOSS_FACE_PUNCHED]; // 8¹æÇâ ÀÌ¹ÌÁö °³¼ö¸¸Å­ ÀÎµ¦½º ¶¯±â±â
	}
	else // 8¹æÇâ
	{
		return m_BossEightDirTexturesArr[eTextureDir][curAction];
	}
}

_bool CBoss::On_CheckHit(void* pArg)
{
	if (m_bIsAlive == false)
	{
		return false;
	}

	// ÀÏÁ¤ °Å¸®º¸´Ù ¸Ö¸é return nullptr
	if (m_fDist_BtwTarget > m_fDamageRange) // 1.5fÀÌ³»°¡ Àû´ç
		return true;

	if (m_iCurAction == ACTION_BOSS_FALLING_START || m_iCurAction == ACTION_BOSS_FALLING || m_iCurAction == ACTION_BOSS_FALLING_END
		|| m_iCurAction == ACTION_BOSS_GET_UP)
	{
		return true;
	}


	_float3 vThisPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vCamPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vCamLook;
	_float4x4 viewMat;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMat);
	vCamLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vCamLook, &vCamLook);

#pragma region Ray
	_float3 vRayHitPosOut;
	CGameObject* pGameObjOut = nullptr;


	_float4x4 TempWorldMatrix = m_pTransformCom->Get_WorldMatrix();


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&m_TextureMat.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&m_TextureMat.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&m_TextureMat.m[2][0]);

	if (m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, L"Layer_Enemy", &vRayHitPosOut, &pGameObjOut) == false)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
		return true;
	}
	else if (pGameObjOut != this)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
		return true;
	}
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
#pragma endregion
	// ÇÇ°Ý È®Á¤ ÀÌÈÄ
	PLAYERATTACKDESC* at = static_cast<PLAYERATTACKDESC*>(pArg);
	_float3 vToTarget = vCamPos - vThisPos;
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	_float3 vToTargetXZ = { vToTarget.x, 0, vToTarget.z };
	D3DXVec3Normalize(&vToTargetXZ, &vToTargetXZ);
	// Y¹«½ÃÇÑ Ä«¸Þ¶óÀÇ ¹æÇâ
	_float3 vCamLookXZ = { vCamLook.x, 0, vCamLook.z };
	D3DXVec3Normalize(&vCamLookXZ, &vCamLookXZ);


	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))
		&& m_pThisLayer) // Idle »óÅÂ¶ó¸é µ¿·á¿¡°Ô ¾Ë¸®±â
	{
		list<class CGameObject*> enemyList = m_pThisLayer->Get_GameObjectList();

		_float fDistance = 0.f;
		_float3 vThisToAnother{};
		for (auto& iter : enemyList)
		{
			CEnemyBase* pAnother = static_cast<CEnemyBase*>(iter);
			if (pAnother->Is_Alive() == false)
			{
				continue;
			}
			vThisToAnother = pAnother->Get_Pos() - Get_Pos();
			if (D3DXVec3Length(&vThisToAnother) < 10) // 10 °Å¸® ¾È¿¡ ÀÖ´Ù¸é
			{
				pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // ¾Ë¸®±â
			}
		}
	}

	m_pFSMCom->Set_NextState(ATTACKED); // ÇÇ°Ý ½ºÅ×ÀÌÆ® ÀüÈ¯

	m_bRedStart = true;
	m_fRedTime = 0.f;

	m_pGameInstance->StopSound(SOUND_EFFECT);
	if (at->bHeavy == true) // ¸¸¾à °­ÆÝÄ¡¿´´Ù¸é
	{
		m_bInAir = true;
		m_pHealthSystem->Damaged(15);
		Send_HPInfo(m_pHealthSystem->Get_PrevHP(), m_pHealthSystem->Get_CurHP());
		m_pFSMCom->Change_Next_Immediately(ENEMY_BASE_STATE::ATTACKED, 0.02f);
		m_bRegularSize = true;
		Set_NextAction(ACTION_BOSS_FALLING_START);

		_float3 vForceDir;
		vForceDir = -vToTargetXZ + _float3(0, 0.2f, 0);
		D3DXVec3Normalize(&vForceDir, &vForceDir);

		m_pPhysics->Add_Force(vForceDir, 40, 0.9f);

		EFFECTDESC effect;
		effect.vPos = vRayHitPosOut;
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &effect);
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &effect);
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_POW, &effect);
		CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_ATTACKED);
		CGameInstance::Get_Instance()->StopSound(SOUND_ENEMY_HIT);
		switch (at->eType)
		{
		case ITEM_PIPE:
			CGameInstance::Get_Instance()->StopSound(SOUND_ENEMY_HIT);
			CGameInstance::Get_Instance()->Play_Sound(L"PipeHitFlesh3.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
			break;
		default:
			CGameInstance::Get_Instance()->StopSound(SOUND_ENEMY_HIT);
			CGameInstance::Get_Instance()->Play_Sound(L"HitGeneric1.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
			break;
		}
		CGameInstance::Get_Instance()->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		_tchar szSound[MAX_PATH] = TEXT("HurtMore%d.wav");
		wsprintf(szSound, szSound, rand() % 4 + 1);
		CGameInstance::Get_Instance()->Play_Sound(szSound, static_cast<CHANNELID>(m_iSound_Channel), m_fSoundVolume);
		return true;
	}
	CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_ATTACKED);
	CGameInstance::Get_Instance()->StopSound(SOUND_ENEMY_HIT);
	switch (at->eType)
	{
	case ITEM_BOTTLE:
		CGameInstance::Get_Instance()->Play_Sound(L"BottleHit1.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
		break;
	case ITEM_PIPE:
		CGameInstance::Get_Instance()->Play_Sound(L"PipeHitFlesh1.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
		break;
	case ITEM_BOTTLE_BROKEN:
		CGameInstance::Get_Instance()->Play_Sound(L"Bottlestab1.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
		break;
	default:
		CGameInstance::Get_Instance()->Play_Sound(L"HitGeneric1.wav", SOUND_ENEMY_HIT, m_fSoundVolume);
		break;
	}

	CGameInstance::Get_Instance()->StopSound(static_cast<CHANNELID>(m_iSound_Channel)); // ½ÅÀ½¼Ò¸®

	m_pHealthSystem->Damaged(5);
	Send_HPInfo(m_pHealthSystem->Get_PrevHP(), m_pHealthSystem->Get_CurHP());

#pragma region Hit Point
	_float fHitMinusCenter = vRayHitPosOut.y - vThisPos.y;
	if (fHitMinusCenter <= -0.f) //»çÅ¸±¸´Ï
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_BOSS_LOW_PUNCHED;

		if (m_iCurAction != ACTION_BOSS_DAZED)
		{
			_tchar* szSoundKey = TEXT("");
			switch (m_iSound_Channel)
			{
			case CHANNELID::SOUND_ENEMY0:
				szSoundKey = TEXT("CrotchHit4.wav");
				break;
			case CHANNELID::SOUND_ENEMY1:
				szSoundKey = TEXT("CrotchHit3.wav");
				break;
			case CHANNELID::SOUND_ENEMY2:
				szSoundKey = TEXT("CrotchHit1.wav");
				break;
			case CHANNELID::SOUND_ENEMY3:
				szSoundKey = TEXT("CrotchHit5.wav");
				break;
			case CHANNELID::SOUND_ENEMY4:
				szSoundKey = TEXT("CrotchHit2.wav");
				break;
			}
			m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
			m_pGameInstance->Play_Sound(szSoundKey, static_cast<CHANNELID>(m_iSound_Channel), 1.f);
		}

	}
	else if (fHitMinusCenter < m_fSizeY * 0.3f) // ¸öÅë
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_BOSS_BODY_PUNCHED;

		if (m_iCurAction != ACTION_BOSS_DAZED)
		{
			_int iRandom = rand() % 17;
			_tchar* szSoundKey = TEXT("");
			switch (iRandom)
			{
			case 0:
				szSoundKey = TEXT("Hurt1 #5091.wav");
				break;
			case 1:
				szSoundKey = TEXT("Hurt1 #5623.wav");
				break;
			case 2:
				szSoundKey = TEXT("Hurt1.wav");
				break;
			case 3:
				szSoundKey = TEXT("Hurt2 #5493.wav");
				break;
			case 4:
				szSoundKey = TEXT("Hurt2.wav");
				break;
			case 5:
				szSoundKey = TEXT("Hurt3 #5090.wav");
				break;
			case 6:
				szSoundKey = TEXT("Hurt3 #5549.wav");
				break;
			case 7:
				szSoundKey = TEXT("Hurt3.wav");
				break;
			case 8:
				szSoundKey = TEXT("Hurt4 #5238.wav");
				break;
			case 9:
				szSoundKey = TEXT("Hurt4.wav");
				break;
			case 10:
				szSoundKey = TEXT("Hurt5 #5653.wav");
				break;
			case 11:
				szSoundKey = TEXT("Hurt5.wav");
				break;
			case 12:
				szSoundKey = TEXT("Hurt6 #5550.wav");
				break;
			case 13:
				szSoundKey = TEXT("Hurt6.wav");
				break;
			case 14:
				szSoundKey = TEXT("Hurt7.wav");
				break;
			case 15:
				szSoundKey = TEXT("Hurt9.wav");
				break;
			case 16:
				szSoundKey = TEXT("Hurt10.wav");
				break;
			default:
				szSoundKey = TEXT("Hurt1 #5091.wav");
				break;
			}
			m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
			m_pGameInstance->Play_Sound(szSoundKey, static_cast<CHANNELID>(m_iSound_Channel), 1.f);
		}
	}
	else // ¸Ó¸®
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_BOSS_FACE_PUNCHED;
		_int iRandom = rand() % 7;
		_tchar* szSoundKey = TEXT("");
		switch (iRandom)
		{
		case 0:
			szSoundKey = TEXT("FaceHit1.wav");
			break;
		case 1:
			szSoundKey = TEXT("FaceHit2.wav");
			break;
		case 2:
			szSoundKey = TEXT("FaceHit3.wav");
			break;
		case 3:
			szSoundKey = TEXT("FaceHit4.wav");
			break;
		case 4:
			szSoundKey = TEXT("FaceHit5.wav");
			break;
		case 5:
			szSoundKey = TEXT("FaceHit6.wav");
			break;
		case 6:
			szSoundKey = TEXT("FaceHit7.wav");
			break;
		}
		m_pGameInstance->StopSound(SOUND_ENEMY0);
		m_pGameInstance->Play_Sound(szSoundKey, SOUND_ENEMY0, 1.f);
	}

	m_pPhysics->Add_Force(vCamLookXZ, 15);
#pragma endregion


#pragma region Effect

	EFFECTDESC effect;
	effect.vPos = vRayHitPosOut;

	for (int i = 0; i < 10; ++i)
	{
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &effect);
	}

	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &effect);
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_POW, &effect);

#pragma endregion


	return true;

}

_bool CBoss::On_GetExecutionHit(void* pArg)
{
	EXECUTIONDESC* _pArg = static_cast<EXECUTIONDESC*>(pArg);

	if (_pArg->pExecutionTarget != this)
	{
		return true;
	}

	m_bExecuting = false;
	m_bExecuted = true;
	m_pFSMCom->Change_Next_Immediately(ENEMY_BASE_STATE::ATTACKED, 0.02f);
	m_bRegularSize = true;

	EFFECTDESC effect;
	effect.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	//m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &effect);
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &effect);
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_POW, &effect);

	

	m_bInAir = true;
	Set_NextAction(ACTION_BOSS_FALLING_START);

	_float3 vForceDir;
	if (*static_cast<_int*>(pArg) == 3) // ¾îÆÛÄÆ
	{
		vForceDir = -m_vToTargetXZ + _float3(0, 2.f, 0);
	}
	else
	{
		vForceDir = -m_vToTargetXZ + _float3(0, 0.2f, 0);
	}
	D3DXVec3Normalize(&vForceDir, &vForceDir);

	m_pPhysics->Add_Force(vForceDir, 40, 0.9f);

	return true;
}

_bool CBoss::On_CheckThrownHit(void* pArg)
{
	// ³»°¡ Ãæµ¹ÇÑ °Ô ¾Æ´Ï¸é return
	if (static_cast<CGameObject*>(pArg) != this)
	{
		return true;
	}

	m_bRedStart = true;
	m_fRedTime = 0.f;
	m_pHealthSystem->Damaged(10);
	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))
		&& m_pThisLayer) // Idle »óÅÂ¶ó¸é µ¿·á¿¡°Ô ¾Ë¸®±â
	{
		list<class CGameObject*> enemyList = m_pThisLayer->Get_GameObjectList();

		_float fDistance = 0.f;
		_float3 vThisToAnother{};
		for (auto& iter : enemyList)
		{
			CEnemyBase* pAnother = static_cast<CEnemyBase*>(iter);
			if (pAnother->Is_Alive() == false)
			{
				continue;
			}
			vThisToAnother = pAnother->Get_Pos() - Get_Pos();
			if (D3DXVec3Length(&vThisToAnother) < 9) // 9 °Å¸® ¾È¿¡ ÀÖ´Ù¸é
			{
				pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // ¾Ë¸®±â
			}
		}
	}
	m_pFSMCom->Change_Next_Immediately(ENEMY_BASE_STATE::ATTACKED, 0.02f);
	m_bRegularSize = true;
	Set_NextAction(ACTION_BOSS_FALLING_START);

	_float3 vForceDir;
	vForceDir = -m_vToTargetXZ; // ±¦Âú°ÚÁö
	D3DXVec3Normalize(&vForceDir, &vForceDir);

	m_pPhysics->Add_Force(vForceDir, 40, 0.9f);
	return true;
}

_bool CBoss::On_CheckGunFire(void* pArg)
{
	if (m_bIsAlive == false)
	{
		return false;
	}


	if (m_fDist_BtwTarget > 25.f) // 1.5fÀÌ³»°¡ Àû´ç
		return true;

#pragma region Ray
	_float3 vRayHitPosOut;
	CGameObject* pGameObjOut = nullptr;
	_float4x4 TempWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&m_TextureMat.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&m_TextureMat.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&m_TextureMat.m[2][0]);

	if (m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, L"Layer_Enemy", &vRayHitPosOut, &pGameObjOut) == false)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
		return true;
	}
	else if (pGameObjOut != this)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
		return true;
	}
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);

	_float3 vThisPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vCamPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vCamLook;
	_float4x4 viewMat;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMat);
	vCamLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	// Y¹«½ÃÇÑ Ä«¸Þ¶óÀÇ ¹æÇâ
	_float3 vCamLookXZ = { vCamLook.x, 0, vCamLook.z };
	D3DXVec3Normalize(&vCamLookXZ, &vCamLookXZ);
	// ÇÇ°Ý ÀÌÈÄ ÀÌº¥Æ®µé
	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))
		&& m_pThisLayer) // Idle »óÅÂ¶ó¸é µ¿·á¿¡°Ô ¾Ë¸®±â
	{
		list<class CGameObject*> enemyList = m_pThisLayer->Get_GameObjectList();

		_float fDistance = 0.f;
		_float3 vThisToAnother{};
		for (auto& iter : enemyList)
		{
			CEnemyBase* pAnother = static_cast<CEnemyBase*>(iter);
			if (pAnother->Is_Alive() == false)
			{
				continue;
			}
			vThisToAnother = pAnother->Get_Pos() - Get_Pos();
			if (D3DXVec3Length(&vThisToAnother) < 10) // 10 °Å¸® ¾È¿¡ ÀÖ´Ù¸é
			{
				pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // ¾Ë¸®±â
			}
		}
	}

#pragma region HitPoint
	// ÇÇ°Ý È®Á¤ ÀÌÈÄ
	_float3 vToTarget = vCamPos - vThisPos;
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	_float3 vToTargetXZ = { vToTarget.x, 0, vToTarget.z };
	D3DXVec3Normalize(&vToTargetXZ, &vToTargetXZ);
	_float fHitMinusCenter = vRayHitPosOut.y - vThisPos.y;


	m_pGameInstance->StopSound(SOUND_ENEMY_HIT);

#pragma endregion

	m_bRedStart = true;
	m_fRedTime = 0.f;
	m_pFSMCom->Set_NextState(ATTACKED); // ÇÇ°Ý ½ºÅ×ÀÌÆ® ÀüÈ¯
	//CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_ATTACKED);

	m_iCurAction = ACTION_BOSS_BODY_PUNCHED;

	if (m_iCurAction != ACTION_BOSS_DAZED)
	{
		CGameInstance::Get_Instance()->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		_tchar szSound[MAX_PATH] = TEXT("HurtMore%d.wav");
		wsprintf(szSound, szSound, rand() % 4 + 1);
		CGameInstance::Get_Instance()->Play_Sound(szSound, static_cast<CHANNELID>(m_iSound_Channel), m_fSoundVolume);
	}
	m_pPhysics->Add_Force(vCamLookXZ, 15);

	m_pHealthSystem->Damaged(10);
	Send_HPInfo(m_pHealthSystem->Get_PrevHP(), m_pHealthSystem->Get_CurHP());
#pragma region Effect

	EFFECTDESC effect;
	effect.vPos = vRayHitPosOut;
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &effect);
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &effect);
	effect.vScale = _float3(0.3f, 0.3f, 0.1f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_POW, &effect);

#pragma endregion


	return true;
}

void CBoss::Make_Aware()
{
	if (m_bTriggered == true)
		return;

	m_bTriggered = true;
	m_pGameInstance->StopBGM(SOUND_BGM1);
	if (m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) && m_pThisLayer) // Idle »óÅÂ¶ó¸é µ¿·á¿¡°Ô ¾Ë¸®±â
	{
		list<class CGameObject*> enemyList = m_pThisLayer->Get_GameObjectList();

		_float fDistance = 0.f;
		_float3 vThisToAnother{};
		for (auto& iter : enemyList)
		{
			CEnemyBase* pAnother = static_cast<CEnemyBase*>(iter);
			if (pAnother->Is_Alive() == false)
			{
				continue;
			}
			vThisToAnother = pAnother->Get_Pos() - Get_Pos();
			if (D3DXVec3Length(&vThisToAnother) < 9) // 9 °Å¸® ¾È¿¡ ÀÖ´Ù¸é
			{
				pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // ¾Ë¸®±â
			}
		}
	}
	m_pFSMCom->Set_NextState(ENEMY_BASE_STATE::AWARE);
	//m_pGameInstance->StopSound()
	m_pGameInstance->StopSound(SOUND_ENEMY_BOSS);
	_tchar szSound[MAX_PATH] = TEXT("PistolGuyReload.wav");
	m_pGameInstance->Play_Sound(szSound, SOUND_ENEMY_BOSS, 1.f);

	Set_NextAction(ACTION_BOSS_TAUNT);
}

void CBoss::Restart_AwareState()
{
	static_cast<CState_Aware_Boss*>(m_pState_Aware)->Restart();
}
