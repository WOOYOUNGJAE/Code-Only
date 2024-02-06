#include "stdafx.h"
#include "GameInstance.h"
#include "EnemyBase.h"

#include <Layer.h>

#include "State_Idle.h"
#include "State_Aware.h"
#include "State_Cautious.h"
#include "State_Attacked.h"
#include "Player.h"
#include "AwarenessGauge.h"
#include "DirectXMath.h"
#include "Enemy_HealthSystem.h"
#include "EnemyPhysics.h"
#include "GimmickManager.h"

#pragma region Defines

const _float CEnemyBase::PI = 3.141592f;
const _float CEnemyBase::HALF_QRT_PI = PI / 4;

const _float CEnemyBase::NORTH_MIN_RAD = -HALF_QRT_PI * 0.5f;
const _float CEnemyBase::NORTH_MAX_RAD = HALF_QRT_PI * 0.5f;

const _float CEnemyBase::NORTH_EAST_MAX_RAD = NORTH_MAX_RAD + HALF_QRT_PI;
const _float CEnemyBase::EAST_MAX_RAD = NORTH_EAST_MAX_RAD + HALF_QRT_PI;
const _float CEnemyBase::SOUTH_EAST_MAX_RAD = EAST_MAX_RAD + HALF_QRT_PI;

const _float CEnemyBase::SOUTH_MAX_RAD = -SOUTH_EAST_MAX_RAD;
const _float CEnemyBase::SOUTH_WEST_MAX_RAD = -EAST_MAX_RAD ;
const _float CEnemyBase::WEST_MAX_RAD = -NORTH_EAST_MAX_RAD;
const _float CEnemyBase::NORTH_WEST_MAX_RAD = NORTH_MIN_RAD;

#pragma endregion Defines

#pragma region CEnemyBase
HRESULT CEnemyBase::Initialize_Prototype()
 {
	HRESULT dwResult = CGameObject::Initialize_Prototype();

	
	return dwResult;
}

static _int iCloneCount = 0;
HRESULT CEnemyBase::Initialize(void* pArg)
{
	if (FAILED(CGameObject::Initialize(pArg)))
	{
		return E_FAIL;
	}  
	if (FAILED(CEnemyBase::Add_Components()))
	{
		MSG_BOX("Failed to Initailize CEnemyBase");
		return E_FAIL;
	}
		
	ENEMYOBJECTDESC* pEnemyDesc = static_cast<ENEMYOBJECTDESC*>(pArg);

	Set_SpawnPos();

	Set_InitMatrix(&pEnemyDesc->EnemyMatrix);

	m_strCautiousSoundKey = pEnemyDesc->strSoundKey;


	_float4x4 matScale, matTrans;
	D3DXMatrixIdentity(&m_matNotRot);
	D3DXMatrixScaling(&matScale, 0.25f, 0.5f, 0.25f);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	m_matNotRot *= matScale * matTrans;

	m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_matNotRot);

	// GameInstance
	m_pGameInstance = CGameInstance::Get_Instance();
	//Safe_AddRef(m_pGameInstance);
	CGameObject* pTmpPlayer = nullptr;
	pTmpPlayer = m_pGameInstance->GetPlayer();
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

	m_pGameInstance->Subscribe(EVENT_PLAYER_ATTACK,
		std::bind(&CEnemyBase::On_CheckHit, this, std::placeholders::_1));

	m_pGameInstance->Subscribe(EVENT_EXECUTION_TRIGGER,
		std::bind(&CEnemyBase::On_CheckTobeExecuted, this, std::placeholders::_1));

	m_pGameInstance->Subscribe(EVENT_PLAY_EXECUTION,
		std::bind(&CEnemyBase::On_GetExecutionHit, this, std::placeholders::_1));

	m_pGameInstance->Subscribe(EVENT_THROW_TO_ENEMY,
		std::bind(&CEnemyBase::On_CheckThrownHit, this, std::placeholders::_1));

	m_pGameInstance->Subscribe(EVENT_PLAYER_GUNFIRE,
		std::bind(&CEnemyBase::On_CheckGunFire, this, std::placeholders::_1));

#pragma endregion Set_EventFunc
	// Sound Channel
	m_iSound_Channel = (++iCloneCount) % 5 + CHANNELID::SOUND_ENEMY0;

	if (m_strCautiousSoundKey.empty() == false)
	{
		m_bTalkingEnemy = true;		
	}

	// Find Layer
	m_pThisLayer = m_pGameInstance->Find_Layer(LEVEL_OUTDOOR, TEXT("Layer_Enemy"));
	if (m_pThisLayer == nullptr)
	{
		//MSG_BOX("Can't Find Layer");
		return S_OK;
	}
	
	return S_OK;
}	

void CEnemyBase::Tick(_float fTimeDelta)
{
	if(m_bRedStart)
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

	if (CGameInstance::Get_Instance()->Key_Down('Z'))
	{
		m_bDebugCollider = !m_bDebugCollider;
	}

	if(!m_bIsAlive)
	{
		m_fIsDeadTime += fTimeDelta;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				m_matNotRot.m[i][j] = 0;
		}
	}

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
			if (m_iCurAction != ACTION_HEADLESS)
			{
				Set_NextAction(ACTION_FALLING_START);				
			}
			Erase_DazedEffect();
			m_pHealthSystem->Make_Dead();
			Set_Dead();
			Play_DyingSound();
			m_bRegularSize = true;
		}

		if (m_bExecutePrompt == false 
			&& (m_iCurAction == ACTION_DAZED || ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) == true) && m_eTextureDir == NORTH))
			&& m_fDist_BtwTarget <= 1.7f)
		{
			_float3 vTargetLookXZ = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
			vTargetLookXZ.y = 0;
			D3DXVec3Normalize(&vTargetLookXZ, &vTargetLookXZ);
			_float fDot = D3DXVec3Dot(&vTargetLookXZ, &-m_vToTargetXZ);

			if (fDot > cosf(D3DXToRadian(60)))
			{
				m_bExecutePrompt = true;
				_uint iArg = PROMPTER_END;
				m_pGameInstance->OnEvent(EVENT_SET_PROMPTER, &iArg);
				iArg = m_iCurAction == ACTION_DAZED ? PROMPTER_FINISH : PROMPTER_CHOP;
				m_pGameInstance->OnEvent(EVENT_SET_PROMPTER, &iArg);
			}
		}
		else if (m_bExecutePrompt == true) // 켜 있는 상태에서
		{
			_float3 vTargetLookXZ = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
			vTargetLookXZ.y = 0;
			D3DXVec3Normalize(&vTargetLookXZ, &vTargetLookXZ);
			_float fDot = D3DXVec3Dot(&vTargetLookXZ, &-m_vToTargetXZ);
			if (m_iCurAction != ACTION_DAZED && (((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) == true) && m_eTextureDir == NORTH) == false)
				|| m_fDist_BtwTarget > 1.7f || fDot < cosf(D3DXToRadian(60)))
			{
				m_bExecutePrompt = false;
				_uint iArg = PROMPTER_END;
				m_pGameInstance->OnEvent(EVENT_SET_PROMPTER, &iArg);
				iArg = PROMPTER_FINISH_END;
				m_pGameInstance->OnEvent(EVENT_SET_PROMPTER, &iArg);
			}
		}
		
	}
	//Prompt

	

	// Animation Frame
	m_fCurFrame += static_cast<_float>(m_iAnimFrameMaxArr[m_iCurAction]) * 
		m_fAnimSpeedArr[m_iCurAction] * fTimeDelta;
	if (m_fCurFrame >= m_iAnimFrameMaxArr[m_iCurAction] && m_iCurAction != ACTION_HEADLESS)
	{
		m_fCurFrame = 0.f;
		++m_iAnimPlayedCount;
	}


	

}

void CEnemyBase::Late_Tick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);


	if (m_fIsDeadTime > 1.f)
		m_bIsNotTick = true;

	if (m_bIsNotTick)
		return;

	CGameObject::Late_Tick(fTimeDelta);

	m_pPhysicsCom->AddForceTick(m_pTransformCom, fTimeDelta);


	//if (m_bInAir)
	if (m_bInAir)
	{

		// Landing
		if (m_pPhysics->GroundPhysics(m_pTransformCom, LEVEL_OUTDOOR, TEXT("Layer_Floor"), fTimeDelta)
			== true) // 땅에 닿았으면
		{
			if (m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE))
			{
				m_bInAir = false;				
			}
		}
	}

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
		_bool bCollided = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Environment"),&pGameObject);
		if (!bCollided)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
		}

	}
	else //Idle일 때
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->GetNextPos());
	}
	

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->GetNextPos());

	
	Update_ToTarget(); // 자신 -> 플레이어 벡터 업데이트

	if (m_bSendArrow == true)
	{
		if (m_bIsAlive == false)
		{
			m_bSendArrow = false;
		}
		_bool bArrowActive = (m_fDist_BtwTarget < 10.f) & m_bIsAlive; // mPGauge비활성화면 false, 또는 m_bAlive false 면 false
		ENEMYARROWDESC Arg{ m_pGauge->Get_Gauge() / 10.f, m_pTargetTransform, m_pTransformCom, bArrowActive };
		m_pGameInstance->OnEvent(EVENT_ENEMY_ARROW, &Arg);		
	}

	if (true == m_bIsAlive)
	{
		_float4x4 MatPlayerWorldInverse = m_pTargetTransform->Get_WorldMatrix_Inverse();
		_float3 EnemyWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 EnemyLocalPos = *D3DXVec3TransformCoord(&EnemyLocalPos, &EnemyWorldPos, &MatPlayerWorldInverse);
		//나중에 다시 활성화
		EnemyLocalPos.y = 0.0f;
		
		_float Rad = D3DXToRadian(50.f);
		_float fDotProduct = D3DXVec3Dot(&_float3(0.0f, 0.0f, 1.0f), D3DXVec3Normalize(&EnemyLocalPos, &EnemyLocalPos));
		if (m_fDamageRange >= m_fDist_BtwTarget && 0.f <= EnemyLocalPos.z && fDotProduct>=cosf(Rad))
		{
			HITDISTANCEDESC hitDesc;
			hitDesc.pEnemy = this;
			hitDesc.bInDistance = true;
			m_pGameInstance->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
			return;
		}
		else
		{
			HITDISTANCEDESC hitDesc;
			hitDesc.pEnemy = this;
			hitDesc.bInDistance = false;
			m_pGameInstance->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
			return;
		}
	}

	


	/*if (m_fDamageRange >= m_fDist_BtwTarget)
	{
		HITDISTANCEDESC hitDesc;
		hitDesc.pEnemy = this;
		hitDesc.bInDistance = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
	}
	else
	{
		HITDISTANCEDESC hitDesc;
		hitDesc.pEnemy = this;
		hitDesc.bInDistance = false;
		CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);
	}*/

}

HRESULT CEnemyBase::Render()
{
	Make_LookCam(); // BillBoard, TextureMat 전용

	// 텍스쳐 스케일 조정
	_float4x4 matScaling;
	_float fSizeX = m_bRegularSize ? 1.f : m_fSizeX;
	_float fSizeY = m_bRegularSize ? 1.f : m_fSizeY;
	D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	m_TextureMat = matScaling * m_TextureMat; // S R T 순서라 matScaling먼저

	// Texture Matrix
	if (m_bRegularSize == false)
	{
		memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	}
	else
	{
		_float3 vLandPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0, -0.4f, 0};
		memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], &vLandPos, sizeof(_float3));
	}

	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;
	D3DXMATRIX mCurWorld = m_pTransformCom->Get_WorldMatrix();
	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &mCurWorld);
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_TextureMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemyBase::Update_ToTarget()
{
	//if (m_pTarget == nullptr)
	//	return E_FAIL;
	_float4x4 viewMat;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMat);
	memcpy(&m_vCamLook, &viewMat.m[2][0], sizeof _float3);
	D3DXVec3Normalize(&m_vCamLook, &m_vCamLook);
	 
	m_vCamPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	m_vToTarget = vTargetPos - m_vCamPos;

	m_fDist_BtwTarget = D3DXVec3Length(&m_vToTarget);
	m_vToTarget = *D3DXVec3Normalize(&m_vToTarget, &m_vToTarget);

	m_vToTargetXZ = { m_vToTarget.x, 0, m_vToTarget.z }; // Y위치는 고려 안한 방향
	D3DXVec3Normalize(&m_vToTargetXZ, &m_vToTargetXZ);

	return S_OK;
}

_float CEnemyBase::Calculate_Angle() // 매 프레임 호출
{
	if (m_pTarget == nullptr)
	{
		MSG_BOX("Target Empty");
		return FLT_MAX;
	}

	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float3 vLookXZ = { vLook.x, 0, vLook.z };
	D3DXVec3Normalize(&vLookXZ, &vLookXZ);
	/*m_fRadian_BtwTarget =
		abs(D3DXVec3Dot(&m_vToTargetXZ, &vLookXZ));*/

	_float fDot =
		D3DXVec3Dot(&-m_vToTargetXZ/*반대*/, &vLookXZ);

	// 예외 처리
	if (fDot > 1.f)
		fDot = 1.f;
	else if (fDot < -1)
		fDot = -1.f;

	// fDot = cos(사이각)
	_float fRad = acosf(fDot);

	_float3 vCrossOut{};
	if (D3DXVec3Cross(&vCrossOut, &vLookXZ, &m_vToTargetXZ)->y < 0)
	{
		fRad *= -1;
	}
	

	return fRad;
}

void CEnemyBase::Func_AwarenessGauge(_float fTimeDelta)
{
}

CEnemyBase::ENEMY_TEXTURE_DIR CEnemyBase::Get_Texture_Dir(_float fRad)
{
	if (m_pTarget == nullptr)
		return ENEMY_TEXTURE_DIR::ENEMY_TEXTURE_DIR_END;


#pragma region CheckAngle
	if (NORTH_MIN_RAD <= fRad && fRad < NORTH_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::NORTH;
	}
	else if (NORTH_MAX_RAD <= fRad && fRad < NORTH_EAST_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::NORTH_EAST;
	}
	else if (NORTH_EAST_MAX_RAD <= fRad && fRad < EAST_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::EAST;
	}
	else if (EAST_MAX_RAD <= fRad && fRad < SOUTH_EAST_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::SOUTH_EAST;
	}
	else if ((SOUTH_EAST_MAX_RAD <= fRad) ||// && fRad <= PI) ||
		fRad < SOUTH_MAX_RAD) //(-PI - 0.00001f <= fRad && fRad < SOUTH_MAX_RAD))
	{
		return ENEMY_TEXTURE_DIR::SOUTH;
	}
	else if (SOUTH_MAX_RAD <= fRad && fRad < SOUTH_WEST_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::SOUTH_WEST;
	}
	else if (SOUTH_WEST_MAX_RAD <= fRad && fRad < WEST_MAX_RAD)
	{
		return ENEMY_TEXTURE_DIR::WEST;
	}
	else if (WEST_MAX_RAD <= fRad && fRad < NORTH_MIN_RAD)
	{
		return ENEMY_TEXTURE_DIR::NORTH_WEST;
	}
	else
	{
		// failed
		return ENEMY_TEXTURE_DIR_END;
	}
	
#pragma endregion CheckAngle
}

void CEnemyBase::Make_LookCam()
{
	_float4x4 CameraMat{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraMat);
	D3DXMatrixInverse(&CameraMat, nullptr, &CameraMat);
	m_IconMat = CameraMat;
//TODO
	if (m_fDist_BtwTarget > m_fDamageRange)
	{
		m_TextureMat = CameraMat;
	}

	// Right
	memcpy(&m_TextureMat.m[0][0], &CameraMat.m[0][0], sizeof _float3);
	// Up
	memcpy(&m_TextureMat.m[1][0], &m_pTransformCom->Get_State(CTransform::STATE_UP), sizeof _float3);
	// Look
	memcpy(&m_TextureMat.m[2][0], &CameraMat.m[2][0], sizeof _float3);

	//m_TextureMat = CameraMat;

	// Icon
	_float3 vIconPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0, 1.0f, 0);
	if (m_bIsTalking)
		vIconPos.y += 0.1f;
	memcpy(&m_IconMat.m[CTransform::STATE_POSITION][0], &vIconPos, sizeof(_float3));
}

_int CEnemyBase::LateTick_NotAware(_float fTimeDelta) // 각도 계산, 
{	
	// Check Current Gauge
#pragma region Check Current Gauge, Change State
	if (m_pGauge->Is_GaugeMax())
	{
		Stop_Talking();
		if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))) // Idle 상태라면 동료에게 알리기
		{
			//m_pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
			m_pGameInstance->Play_Sound(TEXT("SpottedStinger.wav"), SOUND_ENEMY_ATTACK, 1.f);

			if (m_pThisLayer)
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
					if (D3DXVec3Length(&vThisToAnother) < m_fCollectiveRange) // 10 거리 안에 있다면
					{
						pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // 알리기

						if (pAnother->Is_State(ENEMY_BASE_STATE::AWARE) == false) // 원래부터 몰랐던 애는
						{
							pAnother->Stop_MySound();
							pAnother->Stop_Talking();
						}
					}
				}
			}
		}
		m_pGauge->Reset_Gauge();
		m_pFSMCom->Set_NextState(ENEMY_BASE_STATE::AWARE);
		Set_NextAction(ACTION_TAUNT);
		return FSM_NO_EVENT;
	}
	else if (m_bIsSensitive) // 예민한 녀석이면 소리 듣고 Cautious로
	{
		// 들을 수 있는 거리에 있을 때
		if (m_fDist_BtwTarget <= m_fDist_Hearable)
		{
			_bool bPlayerStanding = false;
			 //bPlayerStanding = m_pTarget->Is_Standing()
			if (bPlayerStanding /*|| 깨지는 소리들었을 때*/)
			{
				m_pGauge->Reset_Gauge();
				m_pFSMCom->Set_NextState(ENEMY_BASE_STATE::CAUTIOUS);
				return FSM_NO_EVENT;
			}
		}
	}
#pragma endregion Check Current Gauge, Change State


	// 각도 계산 후 텍스쳐 방향 정하고 가도 게이지 증가 스피드 결정
	Set_TextureDir();

	if (Can_See_Target())
	{
		m_pGauge->Set_Speed_Angle(m_eTextureDir);
		m_pGauge->Set_Speed_Distance(m_fDist_BtwTarget);
	}

	// y = 6.25 * x^3 에서 계수 6.25만 조절
	m_pGauge->Set_AwarenessGauge(fTimeDelta); // 게이지 증가 TODO: 안보일 때 감소

	return FSM_NO_EVENT;
}

void CEnemyBase::Tick_Moving(_float fTimeDelta, _uint* eStateOut, _uint eStateReadyToAttack, _uint eStateRush)
{
	if (m_pTarget == nullptr)
		return;
	_uint iReadyToAttack = eStateReadyToAttack;
	_uint iRush = eStateRush;

	// 전 프레임의 LateTick에서 거리판별, 각도판별 완료된 상태
	if (m_fDist_BtwTarget <= m_fDist_Attack) // 공격 사정거리 이내에 있을 때
	{
		*eStateOut = iReadyToAttack;
		return;
	}

	// cases : cautiously stay, change State to Rush, Come, Move Left or Right, Move Back
	//if (rand() )
	// Randomly Move - 

}

void CEnemyBase::Tick_Attack(_float fTimeDelta)
{
}

_bool CEnemyBase::Chase_Target(_float fMinDistance, _float fTimeDelta, _float vRandomAddingX, _float fMoreWalkDistance)
{
	If_Close_EachOther();// 너무 가까우면 우회 켜기

	_float fSpeed = 1.f;
	if (m_bDetour == true) // 우회 중
	{
		ALL_ACTION_STATE eNextAction
			= m_fDist_BtwTarget > 2.2f + fMoreWalkDistance ? ACTION_RUN : ACTION_WALK;
		Set_NextAction(eNextAction);
		fSpeed = 2.5f;
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vDetourDir);
		m_pTransformCom->Go_Straight(fTimeDelta * fSpeed);
		m_fDetourAccDistance += fTimeDelta * fSpeed * 0.8f/*TransformSpeed*/;
		if (m_fDetourAccDistance > m_fDetourMaxDistance)
		{
			m_fDetourAccDistance = 0.f;
			m_bDetour = false;
		}
	}
	// 걷든 뛰든 따라가야 할 때
	if (m_fDist_BtwTarget > fMinDistance)
	{
		_float3 vMoveDirXZ;

		// 따라갈 때는 무조건 타겟만 바라보기
		if (m_fDist_BtwTarget > 2.2f + fMoreWalkDistance)
		{
			Set_NextAction(ACTION_RUN);
			fSpeed = 5.f;
			_float3 vPlayerRandomPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
			_float3 vPlayerRight = m_pTargetTransform->Get_State(CTransform::STATE_RIGHT);
			D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
			vPlayerRandomPos += vPlayerRight * vRandomAddingX;
			vMoveDirXZ = vPlayerRandomPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vMoveDirXZ.y = 0.f;
			D3DXVec3Normalize(&vMoveDirXZ, &vMoveDirXZ);
		}
		else
		{
			Set_NextAction(ACTION_WALK);			
			fSpeed = 2.5f;
			vMoveDirXZ = m_vToTargetXZ;
		}
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMoveDirXZ);
		m_pTransformCom->Go_Straight(fTimeDelta * fSpeed);
		return true;
	}
	else // 그만 따라가도 되는 거리라면
	{
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		return false;
	}

}

_bool CEnemyBase::ApproachtoTarget(_float fMinDistance, _float fTimeDelta)
{
	if (m_fDist_BtwTarget > fMinDistance)
	{
		_float3 vMoveDirXZ;
		_float fSpeed = 1.f;
		if (m_fDist_BtwTarget > fMinDistance)
		{
			Set_NextAction(ACTION_WALK);
			fSpeed = 2.5f;
			vMoveDirXZ = m_vToTargetXZ;
		}
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMoveDirXZ);
		m_pTransformCom->Go_Straight(fTimeDelta * fSpeed);
		return true;
	}
	else // 그만 따라가도 되는 거리라면
	{
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		return false;
	}
}

_bool CEnemyBase::Chase_Target_Lerp(_bool& bStepable, _float3* pStartDir, _float& fCurStep, _float fOneStep, _float fAttackable, _float fLerpSpeed, _float fTimeDelta)
{
	// 공격 사정거리보다 멀리 있으면 따라감
	_float3& vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vTargetPos = vCurPos;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ); // TODO : 일단은 무조건 타겟 바라보게, 좌우 무빙 생기면 바뀔수도
	_float3 vLook = m_vToTargetXZ;
	if (m_fDist_BtwTarget >= fAttackable) // Lerp로 움직여야 할 때
	{
		if (Get_FrameLeft() <= 1)
		{
			bStepable = false;
			fCurStep = 0.f;
			*pStartDir = m_vToTargetXZ; // 새로운 방향
			Set_NextAction(ACTION_BATTLE_IDLE);
			return true;
		}
		

		//if (fCurStep >= fOneStep) // Lerp 충분히 했다면 새로운 한 발짝
		//{
		//	bStepable = false;
		//	fCurStep = 0.f;
		//	*pStartDir = m_vToTargetXZ; // 새로운 방향
		//	Set_NextAction(ACTION_BATTLE_IDLE);
		//	return true;
		//}
		Set_NextAction(ACTION_INCH_FORWARD);

		D3DXVec3Lerp(&vTargetPos, &vTargetPos, &(vTargetPos + (*pStartDir * fOneStep)), fLerpSpeed * fTimeDelta);
		fCurStep += D3DXVec3Length(&(vCurPos - vTargetPos));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);
		// 따라갈 때는 무조건 타겟만 바라보기
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);

		return true;
	}
	else // 그만 따라가도 되는 거리라면
	{
		Set_NextAction(ACTION_BATTLE_IDLE);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		return false;
	}
}

_bool CEnemyBase::Strafing_Lerp(_bool& bStepable, _float3* pStartDir, _float& fCurStep,
                                _float fOneStep, _float fLerpSpeed, _float fTimeDelta)
{
	// 한 발짝() 다할때까지 럴프
	if (Get_FrameLeft() > 2) // Strafing
	{
		_float3& vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = vCurPos;

		D3DXVec3Lerp(&vTargetPos, &vTargetPos, &(vTargetPos + (*pStartDir * fOneStep)), fLerpSpeed * fTimeDelta);
		//fCurStep += D3DXVec3Length(&(vCurPos - vTargetPos));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);


		// 따라갈 때는 무조건 타겟만 바라보기
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
		return true;
		
	}
	//else if (Get_FrameLeft() <= 0) // Strafing 끝
	else if (Get_FrameLeft() <= 2) // Strafing 끝
	{
		bStepable = false;
		//fCurStep = 0.f;
		Set_NextAction(ACTION_BATTLE_IDLE);
		return false;
	}
	else // Not Strafing but Playing Motion
	{
		//fCurStep = 0.f;
		return true;
	}

	
	//else // 그만 따라가도 되는 거리라면
	//{
	//	Set_NextAction(ACTION_BATTLE_IDLE);
	//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
	//	return false;
	//}
	return true;
}

_bool CEnemyBase::Simple_Move(_float& refAccDistance, _float fMaxDistance, _float fKnockBackSpeed)
{
	refAccDistance += fKnockBackSpeed;
	if (refAccDistance > fMaxDistance) // 충분히 밀려났으면
	{
		refAccDistance = 0.f;
		return false;
	}

	_float3 vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vCurPos += m_vKnockBackDir * fKnockBackSpeed;
	//D3DXVec3Lerp(&vCurPos, &vCurPos, &m_vKnockBackPos, fKnockBackSpeed);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPos);
	return true;
}

void CEnemyBase::Set_KnockBackDir()
{	
	const _float3& refLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vLookXZ = _float3(-refLook.x, 0, -refLook.z);
	m_vKnockBackDir = *(D3DXVec3Normalize(&vLookXZ, &vLookXZ));
}

void CEnemyBase::Make_Look_Target()
{
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vToTargetXZ);
}

_bool CEnemyBase::Check_AttackSuccess()
{
	if (m_fDist_BtwTarget < 4.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CEnemyBase::StayTurn(_float fDegree)
{
	const _float3 vAxisY = m_pTransformCom->Get_State(CTransform::STATE_UP);
	m_pTransformCom->Rotation(vAxisY, D3DXToRadian(fDegree));
}

_bool CEnemyBase::Is_Dazed()
{
	return m_pHealthSystem->Is_Dazed();
}

void CEnemyBase::Add_Force(_float3 vForceDirNormalized, _float fMagnitude, _float fDecelerationFactor)
{
	m_pPhysics->Add_Force(vForceDirNormalized, fMagnitude, fDecelerationFactor);
}

_bool CEnemyBase::Has_Magnitude()
{
	return m_pPhysics->Has_Magnitude();
}

void CEnemyBase::Reset_Gauge()
{
	m_pGauge->Reset_Gauge();
}

_float3 CEnemyBase::Get_Pos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

HRESULT CEnemyBase::Set_Target(CPlayer* pTarget)
{
	m_pTarget = pTarget;
	Safe_AddRef(m_pTarget);
	return S_OK;
}

_float3 CEnemyBase::Get_TargetRight()
{
	return m_pTargetTransform->Get_State(CTransform::STATE_RIGHT);
}

_float3 CEnemyBase::Get_NearRandomPos(_float fMulti)
{
	_float3 vResult = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fRandomDistanceX = cosf(rand()); // -1 ~ 1
	_float fRandomDistanceZ = sinf(rand()+1); // -1 ~ 1
	_float3 vAdd = fMulti * _float3(fRandomDistanceX, 0.f, fRandomDistanceZ);

	return vResult + vAdd;
}

_float3 CEnemyBase::Get_Look()
{
	return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

_float3 CEnemyBase::Get_Up()
{
	return m_pTransformCom->Get_State(CTransform::STATE_UP);
}

_bool CEnemyBase::Is_State(_uint iState)
{
	{ return m_pFSMCom->Is_State(iState); }
}

_bool CEnemyBase::Is_TargetAlive()
{
	return m_pTarget->GetAlive();
}

void CEnemyBase::Set_NextAction(_uint iNextAction)
{
	if (iNextAction != m_iCurAction)
	{
		Set_AnimSpeedDefault();

		m_iCurAction = iNextAction;
		m_iAnimPlayedCount = 0;
		m_fCurFrame = 0.f;
	}
}

void CEnemyBase::Set_AnimSpeedDefault()
{
	memcpy(m_fAnimSpeedArr, m_fAnimDefaultSpeedArr, sizeof _uint * ALL_ACTION_STATE_END);
}

void CEnemyBase::Set_GaugeActive(_bool _b)
{
	m_pGauge->Set_Active(_b);
}

void CEnemyBase::Set_NextFSMState(_uint iNextState)
{
	if (m_pFSMCom == nullptr)
	{
		return;
	}

	m_pFSMCom->Set_NextState(iNextState);
}

void CEnemyBase::Make_Dead()
{
	m_pHealthSystem->Make_Dead();
	Set_Dead();
}

void CEnemyBase::Set_Dead()
{
	if (m_bIsAlive == true)
	{
		m_bIsAlive = false;
		m_bPickable = false;
		m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		Erase_DazedEffect();
		if (m_eEnemy_Type == ENEMY_TYPE_GOON)
		{
			CGimmickManager::Get_Instance()->Goon_KillPlus();		
		}
		else if (m_eEnemy_Type == ENEMY_TYPE_PIPE_MAN)
		{
			CGimmickManager::Get_Instance()->PipeGuy_KillPlus();
			if (CGimmickManager::Get_Instance()->Get_PipeGuy_KillCount() == 6) // 첫번째 죽인 파이프가이는
			{
				_float3 vDropPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _float3(0.f, 0.4f, 0.f);
				CGimmickManager::Get_Instance()->DropKey(vDropPos);
			}
		}


		//나중에 다시 활성화
		HITDISTANCEDESC hitDesc;
 		hitDesc.pEnemy = this;
		hitDesc.bInDistance = false;
		m_pGameInstance->OnEvent(EVENT_ENEMY_IN_DISTANCE, &hitDesc);

		_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		fPos.y -= 0.7f;
		m_pGameInstance->OnEvent(EVENT_BLOODPOOL, &fPos);
	}
}

void CEnemyBase::Set_SpawnPos(_float _x, _float _y, _float _z)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_x, _y, _z));
}

void CEnemyBase::Set_InitMatrix(_float4x4* pMatrix)
{
	if (m_pTransformCom == nullptr || pMatrix == nullptr)
		return;

	_float3 vRight{};
	_float3 vUp{};
	_float3 vLook{};
	_float3 vPos{};

	memcpy(&vRight, &(*pMatrix).m[CTransform::STATE_RIGHT][0], sizeof _float3);
	memcpy(&vUp, &(*pMatrix).m[CTransform::STATE_UP][0], sizeof _float3);
	memcpy(&vLook, &(*pMatrix).m[CTransform::STATE_LOOK][0], sizeof _float3);
	memcpy(&vPos, &(*pMatrix).m[CTransform::STATE_POSITION][0], sizeof _float3);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

_bool CEnemyBase::Can_See_Target()
{
	return true;
}

CTexture* CEnemyBase::Get_CurTexture(ENEMY_TEXTURE_DIR eTextureDir, _uint curAction)
{
	_bool bOnlySouth = curAction > ACTION_SUSPICIOUS;
	if (bOnlySouth == true)
	{
		return m_OnlySouthTexturesArr[curAction - ACTION_FACE_PUNCHED]; // 8방향 이미지 개수만큼 인덱스 땡기기
	}
	else // 8방향
	{
		return m_EightDirTexturesArr[eTextureDir][curAction];
	}
}

_bool CEnemyBase::On_CheckHit(void* pArg)
{
	if (m_bIsAlive == false)
	{
		return false;
	}
	// 일정 거리보다 멀면 return nullptr
	if (m_fDist_BtwTarget > m_fDamageRange) // 1.5f이내가 적당
		return true;

	if (m_iCurAction == ACTION_FALLING_START || m_iCurAction == ACTION_FALLING || m_iCurAction == ACTION_FALLING_END
		|| m_iCurAction == ACTION_GET_UP)
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
	PLAYERATTACKDESC* at = static_cast<PLAYERATTACKDESC*>(pArg);

	
	_float3 vToTarget = vCamPos - vThisPos;
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	_float3 vToTargetXZ = { vToTarget.x, 0, vToTarget.z };
	D3DXVec3Normalize(&vToTargetXZ, &vToTargetXZ);
	// Y무시한 카메라의 방향
	_float3 vCamLookXZ = { vCamLook.x, 0, vCamLook.z };
	D3DXVec3Normalize(&vCamLookXZ, &vCamLookXZ);

	CTransform* PlayerPos = static_cast<CTransform*>(CGameInstance::Get_Instance()->GetPlayer()->Find_Component(L"Com_Transform"));
	_float3 vPlayerPos = PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 vDir = vThisPos - vPlayerPos;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	if (at->bKick)
		m_pPhysicsCom->SetAddForce(vDir, 4.f);
#pragma region

	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))) // Idle 상태라면 동료에게 알리기
	{
		//m_pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
		Stop_Talking();
		m_pGameInstance->Play_Sound(TEXT("SpottedStinger.wav"), SOUND_ENEMY_ATTACK, 1.f);

		if (m_pThisLayer)
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
				if (D3DXVec3Length(&vThisToAnother) < m_fCollectiveRange) // 10 거리 안에 있다면
				{
					pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // 알리기
					if (pAnother->Is_State(ENEMY_BASE_STATE::AWARE) == false) // 원래부터 몰랐던 애는
					{
						pAnother->Stop_MySound();
						pAnother->Stop_Talking();
					}
				}
			}
		}
	}

	m_pFSMCom->Set_NextState(ATTACKED); // 피격 스테이트 전환

	m_bRedStart = true;
	m_fRedTime = 0.f;

	m_pGameInstance->StopSound(SOUND_EFFECT);
	if (at->bHeavy == true) // 만약 강펀치였다면
	{
		m_bInAir = true;
		m_pHealthSystem->Damaged(at->iDmg);
		Send_HPInfo(m_pHealthSystem->Get_PrevHP(), m_pHealthSystem->Get_CurHP());
		m_pFSMCom->Change_Next_Immediately(ENEMY_BASE_STATE::ATTACKED, 0.02f);
		m_bRegularSize = true;
		Set_NextAction(ACTION_FALLING_START);

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
		CGameInstance::Get_Instance()->StopSound( static_cast<CHANNELID>(m_iSound_Channel));
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

	CGameInstance::Get_Instance()->StopSound(static_cast<CHANNELID>(m_iSound_Channel)); // 신음소리
	//_tchar szSound[MAX_PATH] = TEXT("Hurt%d.wav");
	//wsprintf(szSound, szSound, rand() % 10 + 1);
	//CGameInstance::Get_Instance()->Play_Sound(szSound, static_cast<CHANNELID>(m_iSound_Channel), m_fSoundVolume);
	

#pragma region Hit Point
	_float fHitMinusCenter = vRayHitPosOut.y - vThisPos.y;
	if (fHitMinusCenter <= -0.f) //사타구니
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_LOW_PUNCHED;
		m_pHealthSystem->Damaged(at->iDmg);
		_int iCurHP = m_pHealthSystem->Get_PrevHP();
		Send_HPInfo(iCurHP, m_pHealthSystem->Get_CurHP());
		if (m_iCurAction != ACTION_DAZED && iCurHP > 0)
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
	else if (fHitMinusCenter < m_fSizeY * 0.3f) // 몸통
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_BODY_PUNCHED;
		m_pHealthSystem->Damaged(at->iDmg);
		_int iCurHP = m_pHealthSystem->Get_PrevHP();
		Send_HPInfo(iCurHP, m_pHealthSystem->Get_CurHP());
		if (m_iCurAction != ACTION_DAZED && iCurHP > 0)
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
	else // 머리
	{
		m_bRegularSize = false;
		m_iCurAction = ACTION_FACE_PUNCHED;
		m_pHealthSystem->Damaged(at->iDmg);
		_int iCurHP = m_pHealthSystem->Get_PrevHP();
		Send_HPInfo(iCurHP, m_pHealthSystem->Get_CurHP());
		if (m_iCurAction != ACTION_DAZED && iCurHP > 0)
		{

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

_bool CEnemyBase::On_CheckArrow(void* pArg)
{

	return true;
}

_bool CEnemyBase::On_CheckTobeExecuted(void* pArg)
{
	_bool bSuccess = true;
	CGameObject* pOut = this;
	EXECUTECHECKDESC tExecuteDesc{};
	if (m_bIsAlive == false)
	{
		bSuccess = false;
		pOut = nullptr;
		return true;
	}

	// 기절이 아니고 방향이 south아닐때 -> 처형 못한다
	if (Is_Dazed() == false && m_eTextureDir != ENEMY_TEXTURE_DIR::NORTH)
	{
		pOut = nullptr;
		return true;		
	}

	if (m_fDist_BtwTarget > 1.7f) // 1.5f이내가 적당
	{
		bSuccess = false;
		pOut = nullptr;
		return true;
	}
	_float4x4 TempWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&m_TextureMat.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&m_TextureMat.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&m_TextureMat.m[2][0]);
	_float3 vHitPos;
	if (m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, TEXT("Layer_Enemy"), &vHitPos, &pOut) && pOut != this)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
		pOut = nullptr;
		return true;
	}
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);

	
	/*tExecuteDesc.pTarget = pOut;
	if(m_eTextureDir == ENEMY_TEXTURE_DIR::NORTH)
	{
		tExecuteDesc.bToBeChopped = true;
	}
	else
	{
		tExecuteDesc.bToBeChopped = false;
	}*/

	//m_pGameInstance->OnEvent(EVENT_EXECUTION_SUCCESS, &tExecuteDesc);
	EXECUTECHECKDESC ex;
	ex.pExecutionTarget = pOut;
	ex.bToBeChopped = (m_eTextureDir == ENEMY_TEXTURE_DIR::NORTH);
	ex.eType = m_eEnemy_Type;
	// Execution 확정
	m_pGameInstance->OnEvent(EVENT_EXECUTION_SUCCESS, &ex); // 처형 성공했다 알려줘서 플레이어 애니메이션 시키도록

	if (pOut == this && m_eTextureDir != ENEMY_TEXTURE_DIR::NORTH)
	{
		m_bExecuting = true;
		Erase_DazedEffect();
	}

	return true;
}

_bool CEnemyBase::On_GetExecutionHit(void* pArg) // 힘 받기
{
	EXECUTIONDESC* _pArg = static_cast<EXECUTIONDESC*>(pArg);

	if(_pArg->pExecutionTarget != this)
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

	// TODO : 처형 타입 구분하기
	if (_pArg->iExecutionIndex == 4) // karateChopped
	{
		m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))) // Idle 상태라면 동료에게 알리기
		{
			//m_pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
			Stop_Talking();
			m_pGameInstance->Play_Sound(TEXT("SpottedStinger.wav"), SOUND_ENEMY_ATTACK, 1.f);

			if (m_pThisLayer)
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
					if (D3DXVec3Length(&vThisToAnother) < m_fCollectiveRange) // 10 거리 안에 있다면
					{
						pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // 알리기
						if (pAnother->Is_State(ENEMY_BASE_STATE::AWARE) == false) // 원래부터 몰랐던 애는
						{
							pAnother->Stop_MySound();
							pAnother->Stop_Talking();
						}
					}
				}
			}
		}
		m_bIsChopped = true;
		Set_NextAction(ACTION_KARATE_CHOPPED);
		return true;
	}


	m_bInAir = true;
	Set_NextAction(ACTION_FALLING_START);

	_float3 vForceDir;
	if (*static_cast<_int*>(pArg) == 3) // 어퍼컷
	{
		vForceDir = -m_vToTargetXZ + _float3(0, 2.f, 0);
	}
	else
	{
		vForceDir = -m_vToTargetXZ + _float3(0, 0.2f, 0);
	}
	D3DXVec3Normalize(&vForceDir, &vForceDir);
	//m_pPhysics->Add_Force(vForceDir, 60, 0.9f);
	m_pPhysics->Add_Force(vForceDir, 40, 0.9f);
	
	return true;
}

_bool CEnemyBase::On_CheckThrownHit(void* pArg)
{
	// 내가 충돌한 게 아니면 return
	if (static_cast<CGameObject*>(pArg) != this || m_bIsAlive == false)
	{
		return true;		
	}
	
	m_bRedStart = true;
	m_fRedTime = 0.f;
	m_pHealthSystem->Damaged(50);
	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))) // Idle 상태라면 동료에게 알리기
	{
		//m_pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
		Stop_Talking();
		m_pGameInstance->Play_Sound(TEXT("SpottedStinger.wav"), SOUND_ENEMY_ATTACK, 1.f);

		if (m_pThisLayer)
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
				if (D3DXVec3Length(&vThisToAnother) < m_fCollectiveRange) // 10 거리 안에 있다면
				{
					pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // 알리기
					if (pAnother->Is_State(ENEMY_BASE_STATE::AWARE) == false) // 원래부터 몰랐던 애는
					{
						pAnother->Stop_MySound();
						pAnother->Stop_Talking();
					}
				}
			}
		}
	}
	m_pFSMCom->Change_Next_Immediately(ENEMY_BASE_STATE::ATTACKED, 0.02f);
	m_bRegularSize = true;
	Set_NextAction(ACTION_FALLING_START);

	_float3 vForceDir;
	vForceDir = -m_vToTargetXZ; // 괜찮겠지
	D3DXVec3Normalize(&vForceDir, &vForceDir);

	m_pPhysics->Add_Force(vForceDir, 40, 0.9f);
	return true;
}

_bool CEnemyBase::On_CheckGunFire(void* pArg)
{
	if (m_bIsAlive == false)
	{
		return false;
	}


	if (m_fDist_BtwTarget > 25.f) // 1.5f이내가 적당
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
	// Y무시한 카메라의 방향
	_float3 vCamLookXZ = { vCamLook.x, 0, vCamLook.z };
	D3DXVec3Normalize(&vCamLookXZ, &vCamLookXZ);
	// 피격 이후 이벤트들
	if ((m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE) || m_pFSMCom->Is_State(ENEMY_BASE_STATE::CAUTIOUS))) // Idle 상태라면 동료에게 알리기
	{
		Stop_Talking();
		//m_pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
		m_pGameInstance->Play_Sound(TEXT("SpottedStinger.wav"), SOUND_ENEMY_ATTACK, 1.f);

		if (m_pThisLayer)
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
				if (D3DXVec3Length(&vThisToAnother) < m_fCollectiveRange) // 10 거리 안에 있다면
				{
					pAnother->Set_NextFSMState(ENEMY_BASE_STATE::AWARE); // 알리기
					if (pAnother->Is_State(ENEMY_BASE_STATE::AWARE) == false) // 원래부터 몰랐던 애는
					{
						pAnother->Stop_MySound();
						pAnother->Stop_Talking();
					}
				}
			}
		}
	}

#pragma region HitPoint
	// 피격 확정 이후
	_float3 vToTarget = vCamPos - vThisPos;
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	_float3 vToTargetXZ = { vToTarget.x, 0, vToTarget.z };
	D3DXVec3Normalize(&vToTargetXZ, &vToTargetXZ);
	_float fHitMinusCenter = vRayHitPosOut.y - vThisPos.y;


	m_pGameInstance->StopSound(SOUND_ENEMY_HIT);
	if (fHitMinusCenter > m_fSizeY * 0.3f) // 헤드샷
	{
		m_iCurAction = ACTION_HEADLESS;
		m_pFSMCom->Set_NextState(ATTACKED); // 피격 스테이트 전환
		m_pHealthSystem->Damaged(100);
		Send_HPInfo(m_pHealthSystem->Get_PrevHP(), m_pHealthSystem->Get_CurHP());

		m_pGameInstance->Play_Sound(TEXT("GoreCrush2.wav"), SOUND_ENEMY_HIT, 0.3f);

		EFFECTDESC effect;
		effect.vPos = vRayHitPosOut;
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &effect);
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_STRIP, &effect);
		effect.vScale = _float3(0.3f, 0.3f, 0.1f);
		m_pGameInstance->OnEvent(EVENT_EFFECT_POW, &effect);
		return true;
	}
#pragma endregion
	m_pHealthSystem->Damaged(50);

	m_bRedStart = true;
	m_fRedTime = 0.f;
	m_pFSMCom->Set_NextState(ATTACKED); // 피격 스테이트 전환
	//CGameInstance::Get_Instance()->OnEvent(EVENT_ENEMY_ATTACKED);

	m_iCurAction = ACTION_BODY_PUNCHED;

	if (m_iCurAction != ACTION_DAZED)
	{
		CGameInstance::Get_Instance()->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		_tchar szSound[MAX_PATH] = TEXT("HurtMore%d.wav");
		wsprintf(szSound, szSound, rand() % 4 + 1);
		CGameInstance::Get_Instance()->Play_Sound(szSound, static_cast<CHANNELID>(m_iSound_Channel), m_fSoundVolume);
	}
	m_pPhysics->Add_Force(vCamLookXZ, 15);

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

void CEnemyBase::Set_AnimProperty()
{
	// TODO : 텍스처 리소스 다 넣으면 주석 풀기
	//for (int i = 0; i < ONLY_SOUTH_ACTION_END + EIGHT_DIR_ACTION_END; ++i)
	//{
	//	if (i < action_face_punched) // 8방향 애니메이션
	//	{
	//		for (int j = 0; j < ENEMY_TEXTURE_DIR_END; ++j)
	//		{
	//			m_iAnimFrameMaxArr[i] = m_EightDirTexturesArr[j][i]->Get_TextureCount();
	//		}
	//	}
	//	else // only south animation
	//	{
	//		m_iAnimFrameMaxArr[i]
	//		= m_OnlySouthTexturesArr[i - action_face_punched]->Get_TextureCount();
	//	}
	//}

	m_iAnimFrameMaxArr[ACTION_STAND] = 1;
	m_iAnimFrameMaxArr[ACTION_WALK] = 24;
	m_iAnimFrameMaxArr[ACTION_RUN] = 21;
	m_iAnimFrameMaxArr[ACTION_SUSPICIOUS] = 1;
	m_iAnimFrameMaxArr[ACTION_FACE_PUNCHED] = 8;
	m_iAnimFrameMaxArr[ACTION_BODY_PUNCHED] = 6;
	m_iAnimFrameMaxArr[ACTION_LOW_PUNCHED] = 1;
	m_iAnimFrameMaxArr[ACTION_BATTLE_DEFAULT] = 1;
	m_iAnimFrameMaxArr[ACTION_INCH_FORWARD] = 6;
	m_iAnimFrameMaxArr[ACTION_TAUNT] = 6;
	m_iAnimFrameMaxArr[ACTION_BASIC_ATTACK] = 8;
	m_iAnimFrameMaxArr[ACTION_HEAVY_ATTACK_WINDUP] = 1;
	m_iAnimFrameMaxArr[ACTION_HEAVY_ATTACK] = 8;
	m_iAnimFrameMaxArr[ACTION_BATTLE_IDLE] = 4;
	m_iAnimFrameMaxArr[ACTION_STRAFING] = 6;
	m_iAnimFrameMaxArr[ACTION_FALLING_START] = 5;
	m_iAnimFrameMaxArr[ACTION_FALLING] = 1;
	m_iAnimFrameMaxArr[ACTION_FALLING_END] = 7;
	m_iAnimFrameMaxArr[ACTION_DEAD_LYING] = 1;
	m_iAnimFrameMaxArr[ACTION_DAZED] = 14;
	m_iAnimFrameMaxArr[ACTION_GET_UP] = 6;
	m_iAnimFrameMaxArr[ACTION_JUMP] = 8;
	m_iAnimFrameMaxArr[ACTION_KARATE_CHOPPED] = 18;
	m_iAnimFrameMaxArr[ACTION_HEADLESS] = 18;
	m_iAnimFrameMaxArr[ACTION_HEADLESS_END] = 1;


	m_fAnimSpeedArr[ACTION_STAND] = 1;
	m_fAnimSpeedArr[ACTION_WALK] = 1.2f;
	m_fAnimSpeedArr[ACTION_RUN] = 1.8f;
	m_fAnimSpeedArr[ACTION_SUSPICIOUS] = 1.f;
	m_fAnimSpeedArr[ACTION_FACE_PUNCHED] = 1.4f;
	m_fAnimSpeedArr[ACTION_BODY_PUNCHED] = 1.2f;
	m_fAnimSpeedArr[ACTION_LOW_PUNCHED] = 1;
	m_fAnimSpeedArr[ACTION_BATTLE_DEFAULT] = 1;
	m_fAnimSpeedArr[ACTION_INCH_FORWARD] = 0.73f;
	m_fAnimSpeedArr[ACTION_TAUNT] = 1.5f;
	m_fAnimSpeedArr[ACTION_BASIC_ATTACK] = 1.7f;
	m_fAnimSpeedArr[ACTION_HEAVY_ATTACK_WINDUP] = 1;
	m_fAnimSpeedArr[ACTION_HEAVY_ATTACK] = 2.3f;
	m_fAnimSpeedArr[ACTION_BATTLE_IDLE] = 1;
	m_fAnimSpeedArr[ACTION_STRAFING] = 1;
	m_fAnimSpeedArr[ACTION_FALLING_START] = 1;
	m_fAnimSpeedArr[ACTION_FALLING] = 1;
	m_fAnimSpeedArr[ACTION_FALLING_END] = 1.5f;
	m_fAnimSpeedArr[ACTION_DEAD_LYING] = 1;
	m_fAnimSpeedArr[ACTION_DAZED] = 1;
	m_fAnimSpeedArr[ACTION_GET_UP] = 1;
	m_fAnimSpeedArr[ACTION_JUMP] = 1;
	m_fAnimSpeedArr[ACTION_KARATE_CHOPPED] = 0.6f;
	m_fAnimSpeedArr[ACTION_HEADLESS] = 0.8f;
	m_fAnimSpeedArr[ACTION_HEADLESS_END] = 1.f;


	memcpy(m_fAnimDefaultSpeedArr, m_fAnimSpeedArr, sizeof _uint * ALL_ACTION_STATE_END);
}

void CEnemyBase::Play_DyingSound()
{
#pragma region Sound
	_tchar* szSoundKey = TEXT("");
	switch (m_iSound_Channel)
	{
	case CHANNELID::SOUND_ENEMY0:
		szSoundKey = TEXT("Death1 #5117.wav");
		break;
	case CHANNELID::SOUND_ENEMY1:
		szSoundKey = TEXT("Death1.wav");
		break;
	case CHANNELID::SOUND_ENEMY2:
		szSoundKey = TEXT("Death3.wav");
		break;
	case CHANNELID::SOUND_ENEMY3:
		szSoundKey = TEXT("Death3 #5441.wav");
		break;
	case CHANNELID::SOUND_ENEMY4:
		szSoundKey = TEXT("Death2.wav");
		break;
	}
	m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
	m_pGameInstance->Play_Sound(szSoundKey, static_cast<CHANNELID>(m_iSound_Channel), 1.f);
#pragma endregion

}

void CEnemyBase::Create_DazedEffect()
{
	if (m_bEffectOn == true)
		return;

	m_bEffectOn = true;
	m_tEffectDesc2.vScale = { 0.3f,0.3f,0.3f };
	m_tEffectDesc2.vPos = Get_Pos();
	m_tEffectDesc2.bActive = true;
	m_tEffectDesc2.pTargetTransform = m_pTransformCom;
	CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_DAZESWIRL, &m_tEffectDesc2);
}

void CEnemyBase::Erase_DazedEffect()
{
	if (m_bEffectOn == false)
		return;

	m_bEffectOn = false;

	CGameInstance::Get_Instance()->TurnOff_Effect(EFFECT_DAZESWIRL, m_tEffectDesc2.iIndex);
}
//
//_bool CEnemyBase::Texture_SelfMouseRay(_uint iLevelIndex, const wstring& _strLayerTag, _float3* pOut)
//{
//	_float3 vRayHitPosOut;
//	CGameObject* pGameObjOut = nullptr;
//
//	_float4x4 TempWorldMatrix = m_pTransformCom->Get_WorldMatrix();
//
//	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&m_TextureMat.m[0][0]);
//	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&m_TextureMat.m[1][0]);
//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&m_TextureMat.m[2][0]);
//
//	if (m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, L"Layer_Enemy", &vRayHitPosOut, &pGameObjOut) == false)
//	{
//		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
//		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
//		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
//		return false;
//	}
//	else if (pGameObjOut != this)
//	{
//		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
//		m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
//		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
//		return false;
//	}
//	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&TempWorldMatrix.m[0][0]);
//	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&TempWorldMatrix.m[1][0]);
//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&TempWorldMatrix.m[2][0]);
//}

void CEnemyBase::Set_Hit_byTrain()
{
	m_bHit_byTrain = true;
	m_pHealthSystem->Make_Dead();
	m_bIsAlive = false;
	_bool bArrowActive = false; // mPGauge비활성화면 false, 또는 m_bAlive false 면 false
	ENEMYARROWDESC Arg{ m_pGauge->Get_Gauge() / 10.f, m_pTargetTransform, m_pTransformCom, bArrowActive };
	m_pGameInstance->OnEvent(EVENT_ENEMY_ARROW, &Arg);

	PARTICLEEVENTDESC ParticleEventDesc;
	if (m_eEnemy_Type == ENEMY_TYPE_GOON)
		ParticleEventDesc.ParticleType = PARTICLE_GOON;
	else if (m_eEnemy_Type == ENEMY_TYPE_PIPE_MAN)
		ParticleEventDesc.ParticleType = PARTICLE_PIPE;
	_float3 fPosTemp = ParticleEventDesc.targetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ParticleEventDesc.targetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pGameInstance->OnEvent(EVENT_PARTICLE, &ParticleEventDesc);
	m_pGameInstance->OnEvent(EVENT_BLOODPOOL, &fPosTemp);

	EFFECTDESC effect;
	effect.vPos = Get_Pos();
	effect.vScale = _float3(0.5f, 0.5f, 0.5f);
	m_pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_MIST, &effect);

	_int iRandom = rand() % 2;
	_tchar* szSoundKey = TEXT("");
	switch (iRandom)
	{
	case 0:
		szSoundKey = TEXT("GoreCrush1.wav");
		break;
	case 1:
		szSoundKey = TEXT("GoreCrush2.wav");
		break;
	}
	m_pGameInstance->StopSound(SOUND_ENEMY_HIT);
	m_pGameInstance->Play_Sound(szSoundKey, SOUND_ENEMY_HIT, 1.f);

}

void CEnemyBase::Set_Look(const _float3& vLookDir)
{
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLookDir);
}

void CEnemyBase::Send_HPInfo(_uint iPrevHP, _uint iCurHP)
{
	m_tHP_Desc = { static_cast<_uint>(m_eEnemy_Type), iPrevHP, iCurHP };

	m_pGameInstance->OnEvent(EVENT_UPDATE_ENEMY_HP, &m_tHP_Desc);
}

void CEnemyBase::Play_MySound(_tchar* strKey)
{
	m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
	m_pGameInstance->Play_Sound(strKey, static_cast<CHANNELID>(m_iSound_Channel), 1.f);
}

void CEnemyBase::Stop_MySound()
{
	m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
}

void CEnemyBase::Go_Straight(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
}

_bool CEnemyBase::If_Close_EachOther()
{
	if (m_pThisLayer == nullptr)
	{
		return false;
	}

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
		if (D3DXVec3Length(&vThisToAnother) < 0.6f) //거리 안에 있다면
		{
			_float3 vAnotherLook = pAnother->Get_Look();
			_float3 vAnotherUp = pAnother->Get_Up();
			D3DXVec3Normalize(&vAnotherLook, &vAnotherLook);
			D3DXVec3Normalize(&vAnotherUp, &vAnotherUp);
			D3DXVec3Normalize(&vThisToAnother, &vThisToAnother);
			_float3 vTempNormal;
			_float3 vNormal; // 동료 Look의 법선
			D3DXVec3Cross(&vTempNormal, &vAnotherLook , &vThisToAnother);
			D3DXVec3Cross(&vNormal,&vAnotherLook, &vAnotherUp); // Look의 왼쪽
			D3DXVec3Normalize(&vNormal, &vNormal);
			if (vTempNormal.y < 0) // vthisToOther이 오른쪽에서
			{
				vNormal *= -1;
			}
			_float fDot = D3DXVec3Dot(&vNormal, &-vThisToAnother);

			m_vDetourDir = vNormal * fDot * 2 + vThisToAnother;
			D3DXVec3Normalize(&m_vDetourDir, &m_vDetourDir);

			m_fDetourAccDistance = 0.f;
			m_bDetour = true;
			return true;
		}
	}

	return false;
}

HRESULT CEnemyBase::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);*/


	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_bDebugCollider)
	{
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matNotRot);
		if (FAILED(m_pDebugTextureCom->Bind_OnGraphicDevice(0)))
			return E_FAIL;
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pCubeVIBufferCom->Render();
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}



	return S_OK;
}

HRESULT CEnemyBase::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	return S_OK;
}

HRESULT CEnemyBase::Add_Components()
{
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

	 //부모 Enemy만이 알고 있는 State 추가
	if (FAILED(CEnemyBase::Add_StateOnFSM()))
	{
		return E_FAIL;
	}

	//Physics
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemyBase::Add_StateOnFSM() // Idle, Cautious, Aware
{
	list<CState*> tmpStateList;
	tmpStateList.emplace_back(CState_Idle_Enemy::Create(m_pGraphic_Device, this));
	tmpStateList.emplace_back(CState_Cautious_Enemy::Create(m_pGraphic_Device, this));

	AWAREDESC tAwareArguDesc{ 2.5f, 0.2f, 4.f, 5.f, 10.f};
	tmpStateList.emplace_back(CState_Aware_Enemy::Create(m_pGraphic_Device, this, &tAwareArguDesc));
	tmpStateList.emplace_back(CState_Attacked_Enemy::Create(m_pGraphic_Device, this));
	//tmpStateList.emplace_back(CState_Battle_Enemy::Create(m_pGraphic_Device, this)); // Aware 안에서
	return m_pFSMCom->Add_NewState(&tmpStateList, this);
}

void CEnemyBase::Free()
{
	CGameObject::Free();
	// Release Components
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_AwarenessIcon_Alert);
	Safe_Release(m_AwarenessIcon_Spooked);
	Safe_Release(m_AwarenessIcon_Cautious);
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
}

#pragma endregion CEnemyBase
