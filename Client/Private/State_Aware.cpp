#include "stdafx.h"
#include "State_Aware.h"
#include "GameInstance.h"
#include "EnemyBase.h"
#include "State_Battle.h"
#include "State_Follow.h"
#include "State_Aware_Ready.h"
#include "Boss.h"
#include "Client_Struct.h"

CState_Aware_Enemy* CState_Aware_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, void* pArg)
{
	CState_Aware_Enemy* pInstance = new CState_Aware_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Aware_Enemy");
	}
	ARGUDESC tmpDesc{ pOwnerDetailed, (AWAREDESC*)pArg};
	pInstance->Initialize(&tmpDesc); // 주인과 정보 구조체를 묶어서 전달
	return pInstance;
}

HRESULT CState_Aware_Enemy::Initialize(void* pArg)
{
	// Set Owner - Detailed
	CEnemyBase* pOwnerDetailed = static_cast<ARGUDESC*>(pArg)->pOwnerDetailed;
	if (FAILED(CState_Enemy::Initialize(pOwnerDetailed)))
	{
		return E_FAIL;
	}

	// Set Desc
	m_tDesc = *static_cast<ARGUDESC*>(pArg)->pDesc;

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	// Add States : Ready - Follow - Battle
	list<CState*> tmpStateList;

	// Ready State
	tmpStateList.emplace_back(CState_Aware_Ready::Create(m_pGraphic_Device, m_pOwnerDetailed));
	tmpStateList.emplace_back(CState_Follow_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed, m_tDesc.fDist_BattleStart)); // Aware 안에서
	tmpStateList.emplace_back(CState_Battle_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed)); // Aware 안에서

	return 	m_pInnerFSM->Add_NewState(&tmpStateList, m_pOwnerDetailed);
}

void CState_Aware_Enemy::Free()
{
	CState_Enemy::Free();
	
}

void CState_Aware_Enemy::BeginState(_float fTimeDelta)
{
	m_pOwnerDetailed->Reset_Gauge();
}

_int CState_Aware_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	if (m_pOwnerDetailed->Is_TargetAlive() == false)
		return FSM_NO_EVENT;

	// 거리 판단 -> Enemy Late Tick 에서
	_float fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	if (m_pInnerFSM->Is_State(READY)) // for ready State
	{
		m_pInnerFSM->Set_NextState(AWARE_STATE::READY);
	}
	else if (fDistance < m_tDesc.fDist_BattleStart)
	{
		// Battles
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_DEFAULT);
		//m_pInnerFSM->Tick(fTimeDelta, pArg); // Battle State
		m_pInnerFSM->Set_NextState(AWARE_STATE::BATTLE);
	}
	else if (fDistance < m_tDesc.fDist_AlwaysFollow) // 거리 + moving이 아닐 때  && m_pInnerFSM->Is_State(1) == false
	{
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_RUN);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else if (fDistance < m_tDesc.fDist_CheckPrtHidden) // 6
	{
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_RUN);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else // 추적 못 할 정도가 되면
	{
		_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::CAUTIOUS;
		m_pInnerFSM->Change_Next_Immediately(READY, fTimeDelta);
		//m_pInnerFSM->Set_NextState(READY);/
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_SUSPICIOUS);
		m_pOwnerDetailed->Set_GaugeActive(true);
		m_pOwnerDetailed->Set_NextFSMState(iNextState);
		return iNextState;
	}

	iResult = m_pInnerFSM->Tick(fTimeDelta, pArg); // Ready or Follow or Battle

	if (iResult != FSM_NO_EVENT)
	{
		m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
	}

	iResult = FSM_NO_EVENT;
	//m_pInnerFSM->Set_NextState(*(_uint*)pResult);
	// 스테이트 실행5
	return iResult; // 다음 스테이트도 Aware
}

void CState_Aware_Enemy::CompleteState(_float fTimeDelta)
{
	//m_pInnerFSM->Set_NextState(READY);
	m_pOwnerDetailed->Reset_AwareState();
}

_int CState_Aware_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	m_pOwnerDetailed->Set_TextureDir(); // 각도 계산 + TextureDir 세팅
	m_pOwnerDetailed->Make_Look_Target();
	return FSM_NO_EVENT;
}


#pragma region CState_Aware_PipeGuy


CState_Aware_PipeGuy* CState_Aware_PipeGuy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed,
	void* pArg)
{
	CState_Aware_PipeGuy* pInstance = new CState_Aware_PipeGuy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Aware_PipeGuy");
	}
	ARGUDESC tmpDesc{ pOwnerDetailed, (AWAREDESC*)pArg };
	pInstance->Initialize(&tmpDesc); // 주인과 정보 구조체를 묶어서 전달
	return pInstance;
}

HRESULT CState_Aware_PipeGuy::Initialize(void* pArg)
{
	// Set Owner - Detailed
	CEnemyBase* pOwnerDetailed = static_cast<ARGUDESC*>(pArg)->pOwnerDetailed;
	if (FAILED(CState_Enemy::Initialize(pOwnerDetailed)))
	{
		return E_FAIL;
	}

	// Set Desc
	m_tDesc = *static_cast<ARGUDESC*>(pArg)->pDesc;

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	// Add States : Ready - Follow - Battle
	list<CState*> tmpStateList;

	// Ready State
	tmpStateList.emplace_back(CState_Aware_Ready::Create(m_pGraphic_Device, m_pOwnerDetailed));
	tmpStateList.emplace_back(CState_Follow_Enemy::Create(m_pGraphic_Device, m_pOwnerDetailed, m_tDesc.fDist_BattleStart)); // Aware 안에서
	tmpStateList.emplace_back(CState_Battle_PipeGuy::Create(m_pGraphic_Device, m_pOwnerDetailed)); // Aware 안에서

	return 	m_pInnerFSM->Add_NewState(&tmpStateList, m_pOwnerDetailed);
}

void CState_Aware_PipeGuy::Free()
{
	CState_Aware_Enemy::Free();
}

void CState_Aware_PipeGuy::BeginState(_float fTimeDelta)
{
	CState_Aware_Enemy::BeginState(fTimeDelta);
}

_int CState_Aware_PipeGuy::Tick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;
	if (m_pOwnerDetailed->Is_TargetAlive() == false)
		return FSM_NO_EVENT;
	// 거리 판단 -> Enemy Late Tick 에서
	_float fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	if (m_pInnerFSM->Is_State(READY)) // for ready State
	{
		m_pInnerFSM->Set_NextState(AWARE_STATE::READY);
	}
	else if (fDistance < m_tDesc.fDist_BattleStart) // 1.0f
	{
		// Battles
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_DEFAULT);
		//m_pInnerFSM->Tick(fTimeDelta, pArg); // Battle State
		m_pInnerFSM->Set_NextState(AWARE_STATE::BATTLE);
	}
	else if (fDistance < m_tDesc.fDist_AlwaysFollow) // 거리 + moving이 아닐 때  && m_pInnerFSM->Is_State(1) == false
	{
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else if (fDistance < m_tDesc.fDist_CheckPrtHidden) // 6
	{
		// TODO : player Hidden 인지 Partially Hidden인지 판별
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else // 추적 못 할 정도가 되면
	{
		_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::CAUTIOUS;
		m_pInnerFSM->Change_Next_Immediately(READY, fTimeDelta);
		//m_pInnerFSM->Set_NextState(READY);/
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_SUSPICIOUS);
		m_pOwnerDetailed->Set_GaugeActive(true);
		m_pOwnerDetailed->Set_NextFSMState(iNextState);
		return iNextState;
	}

	iResult = m_pInnerFSM->Tick(fTimeDelta, pArg); // Ready or Follow or Battle

	if (iResult != FSM_NO_EVENT)
	{
		m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
	}

	iResult = FSM_NO_EVENT;
	//m_pInnerFSM->Set_NextState(*(_uint*)pResult);
	// 스테이트 실행5
	return iResult; // 다음 스테이트도 Aware
}

void CState_Aware_PipeGuy::CompleteState(_float fTimeDelta)
{
	CState_Aware_Enemy::CompleteState(fTimeDelta);
}

_int CState_Aware_PipeGuy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return CState_Aware_Enemy::LateTick_State(fTimeDelta, pArg);
}

#pragma endregion CState_Aware_PipeGuy


#pragma region BOSS

CState_Aware_Boss* CState_Aware_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed,
	void* pArg)
{
	CState_Aware_Boss* pInstance = new CState_Aware_Boss(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Aware_Boss");
	}
	ARGUDESC tmpDesc{ pOwnerDetailed, (AWAREDESC*)pArg };
	pInstance->Initialize(&tmpDesc); // 주인과 정보 구조체를 묶어서 전달
	return pInstance;
}

HRESULT CState_Aware_Boss::Initialize(void* pArg)
{
	// Set Owner - Detailed
	CEnemyBase* pOwnerDetailed = static_cast<ARGUDESC*>(pArg)->pOwnerDetailed;
	if (FAILED(CState_Enemy::Initialize(pOwnerDetailed)))
	{
		return E_FAIL;
	}

	// Set Desc
	m_tDesc = *static_cast<ARGUDESC*>(pArg)->pDesc;

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	// Add States : Ready - Follow - Battle
	list<CState*> tmpStateList;
	
	return 	m_pInnerFSM->Add_NewState(&tmpStateList, m_pOwnerDetailed);
}

void CState_Aware_Boss::Free()
{
	CState_Aware_Enemy::Free();
}

void CState_Aware_Boss::BeginState(_float fTimeDelta)
{
	CState_Aware_Enemy::BeginState(fTimeDelta);
}

_int CState_Aware_Boss::Tick_State(_float fTimeDelta, void* pArg)
{
	_int iResult = FSM_NO_EVENT;

	if (m_pOwnerDetailed->Is_TargetAlive() == false)
		return FSM_NO_EVENT;

#pragma region DIstance
	m_fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	if (m_fDistance < m_fNearMax)
	{
		m_eDistanceState = DISTANCE_NEAR;
	}
	else if (m_fDistance < m_fNormalMax)
	{
		m_eDistanceState = DISTANCE_NORMAL;
	}
	else
	{
		m_eDistanceState = DISTANCE_FAR;
	}

	static_cast<CBoss*>(m_pOwnerDetailed)->Set_DistanceState(m_eDistanceState);
#pragma endregion

	switch (m_eCurState)
	{
	case BOSS_AWARE_READY:
		Tick_Ready(fTimeDelta);
		break;
	case BOSS_AWARE_FOLLOW:
		Tick_Follow(fTimeDelta);
		break;
	case BOSS_AWARE_FIRE:
		Tick_Fire(fTimeDelta);
		break;
	case BOSS_AWARE_SHORT_WALK:
		Tick_ShortWalk(fTimeDelta);
		break;
	case BOSS_AWARE_NEAR_ATTACK:
		Tick_NearAttack(fTimeDelta);
		break;
	case BOSS_AWARE_ROLL:
		Tick_Roll(fTimeDelta);
		break;
	case BOSS_AWARE_RELOAD:
		Tick_Reload(fTimeDelta);
		break;
	default:
		break;
	}
	
#pragma region PipeGuy


	if (m_pInnerFSM->Is_State(READY)) // for ready State
	{
		m_pInnerFSM->Set_NextState(AWARE_STATE::READY);
	}
	else if (m_fDistance < m_tDesc.fDist_BattleStart) // 1.0f
	{
		// Battles
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_DEFAULT);
		//m_pInnerFSM->Tick(fTimeDelta, pArg); // Battle State
		m_pInnerFSM->Set_NextState(AWARE_STATE::BATTLE);
	}
	else if (m_fDistance < m_tDesc.fDist_AlwaysFollow) // 거리 + moving이 아닐 때  && m_pInnerFSM->Is_State(1) == false
	{
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else if (m_fDistance < m_tDesc.fDist_CheckPrtHidden) // 6
	{
		// TODO : player Hidden 인지 Partially Hidden인지 판별
		//m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_WALK);
		m_pInnerFSM->Set_NextState(AWARE_STATE::FOLLOW);
	}
	else // 추적 못 할 정도가 되면
	{
		_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::CAUTIOUS;
		m_pInnerFSM->Change_Next_Immediately(READY, fTimeDelta);
		//m_pInnerFSM->Set_NextState(READY);/
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_SUSPICIOUS);
		m_pOwnerDetailed->Set_GaugeActive(true);
		m_pOwnerDetailed->Set_NextFSMState(iNextState);
		return iNextState;
	}

	iResult = m_pInnerFSM->Tick(fTimeDelta, pArg); // Ready or Follow or Battle

	if (iResult != FSM_NO_EVENT)
	{
		m_pInnerFSM->Change_Next_Immediately(iResult, fTimeDelta);
	}

	iResult = FSM_NO_EVENT;
	//m_pInnerFSM->Set_NextState(*(_uint*)pResult);
	// 스테이트 실행5
	return iResult; // 다음 스테이트도 Aware
#pragma endregion
	return FSM_NO_EVENT;
}

void CState_Aware_Boss::CompleteState(_float fTimeDelta)
{
	CState_Aware_Enemy::CompleteState(fTimeDelta);
}

_int CState_Aware_Boss::LateTick_State(_float fTimeDelta, void* pArg)
{
	//return CState_Aware_Enemy::LateTick_State(fTimeDelta, pArg);
	return FSM_NO_EVENT;
}
#pragma region Tick_State


void CState_Aware_Boss::Tick_Ready(_float fTimeDelta)
{
	// 도발 애니메이션이 끝나면
	if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_TAUNT
		&& m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
	{
		RefreshDir();
		switch (m_eDistanceState)
		{
		case DISTANCE_NEAR:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
			m_eCurState = BOSS_AWARE_NEAR_ATTACK;
			return;
		case DISTANCE_NORMAL:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
			m_eCurState = BOSS_AWARE_FIRE;
			return;
		case DISTANCE_FAR:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
			m_eCurState = BOSS_AWARE_FOLLOW;
			return;
		}
	}
}

void CState_Aware_Boss::Tick_Follow(_float fTimeDelta)
{
	RefreshDir(); // 따라갈 때는 항상 타겟 바라보기

	if (static_cast<CBoss*>(m_pOwnerDetailed)->Chase_Target(m_fChaseMinDistance, m_fWalkMaxDistance, fTimeDelta) == false)
	{
		// 그만 따라가도 된다면 
		switch (m_eDistanceState)
		{
		case DISTANCE_NEAR:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
			m_eCurState = BOSS_AWARE_NEAR_ATTACK;
			break;

		default:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
			m_eCurState = BOSS_AWARE_FIRE;
			break;
		}
	}
}

void CState_Aware_Boss::Tick_Fire(_float fTimeDelta)
{
	m_fFireAccTime += fTimeDelta;

	if (m_bFireRestComplete == false && m_fFireAccTime > m_fFireTerm)
	{
		m_fFireAccTime = 0.f;
		m_bFireRestComplete = true;
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BOSS_FIRE);
	}

	if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_BOSS_FIRE)
	{
		// 실제로 발사해야하는 타이밍
		if (static_cast<_int>(m_pOwnerDetailed->Get_CurFrame()) == 1 && m_bRealFired == false)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			m_bRealFired = true;
			++m_iFireCount;
			// Sound
			pGameInstance->StopSound(SOUND_ENEMY_BOSS);
			_tchar szSound[MAX_PATH] = TEXT("EnemyPistolFire%d.wav");
			wsprintf(szSound, szSound, rand() % 4 + 1);
			pGameInstance->Play_Sound(szSound, SOUND_ENEMY_BOSS, 1.f);

			if (Check_ShootSuccess(m_pOwnerDetailed->Get_ToTargetXZ()))
			{
				ENEMYATTACKDESC at;
				at.pEnemyTransform = static_cast<CTransform*>(m_pOwnerDetailed->Find_Component(TEXT("Com_Transform")));
				at.iDmg = m_iFireDamage;
				pGameInstance->OnEvent(EVENT_ENEMY_ATTACK, &at);

			}
			else // 빗나갔을 때 사운드
			{
				pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
				_tchar szSound[MAX_PATH] = TEXT("ric%d.wav");
				wsprintf(szSound, szSound, rand() % 4 + 1);
				pGameInstance->Play_Sound(szSound, SOUND_ENEMY_ATTACK, 1.f);
			}
			Safe_Release(pGameInstance);
		}
		else if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1) // 애니메이션 완료 되면
		{			
			if (m_iFireCount >= 5) // Reload
			{
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_RELOADING);
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);
				pGameInstance->StopSound(SOUND_ENEMY2);
				_tchar szSound[MAX_PATH] = TEXT("PistolGuyReload.wav");
				pGameInstance->Play_Sound(szSound, SOUND_ENEMY2, 1.f);
				Safe_Release(pGameInstance);
				m_eCurState = BOSS_AWARE_RELOAD;
			}
			else
			{
				switch (m_eDistanceState)
				{
				case DISTANCE_NEAR:
					m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
					m_eCurState = BOSS_AWARE_NEAR_ATTACK;
					break;
				case DISTANCE_NORMAL:
					m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
					m_eCurState = BOSS_AWARE_FIRE;
					break;
				case DISTANCE_FAR:
					m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
					m_eCurState = BOSS_AWARE_FOLLOW;
					break;
				}

			}

			RefreshDir();
			m_fFireAccTime = 0.f;
			m_bFireRestComplete = false;
			m_bRealFired = false;
		}
	}
}

void CState_Aware_Boss::Tick_ShortWalk(_float fTimeDelta)
{
}

void CState_Aware_Boss::Tick_NearAttack(_float fTimeDelta)
{
	m_fNearAttackAccTime += fTimeDelta;

	if (m_bNearAttackRestComplete == false && m_fNearAttackAccTime > 0.8f)
	{
		m_fNearAttackAccTime = 0.f;
		m_bNearAttackRestComplete = true;
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BASIC_ATTACK);
	}

	if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_BASIC_ATTACK
		&& m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if(static_cast<CBoss*>(m_pOwnerDetailed)->Check_AttackSuccess())
		{
			// TODO 근접 공격 성공
			// Sound
			pGameInstance->StopSound(SOUND_ENEMY_ATTACK);
			_tchar szSound[MAX_PATH] = TEXT("PistolWhip3.wav");
			pGameInstance->Play_Sound(szSound, SOUND_ENEMY_BOSS, 1.f);

			ENEMYATTACKDESC at;
			at.pEnemyTransform = static_cast<CTransform*>(m_pOwnerDetailed->Find_Component(TEXT("Com_Transform")));
			at.iDmg = m_iFireDamage;
			pGameInstance->OnEvent(EVENT_ENEMY_ATTACK, &at);

			m_pOwnerDetailed->Add_Force(-m_vDirXZ, 15);
		}

		Safe_Release(pGameInstance);

		
		RefreshDir();

		// Decide Next State
		//iRandom = 3;
		//if (iRandom < 4) // TODO ROLL 확률 조정
		_int iRandom = rand() % 4;
		Set_RollDir(iRandom);
		RefreshDir(m_vRollDir);
		m_vRollActionIndex = CBoss::ACTION_BOSS_ROLL_WEST;
		switch (iRandom)
		{
		case 0:
			m_vRollActionIndex = CBoss::ACTION_BOSS_ROLL_WEST_REVERSE;
			break;
		case 1:
			m_vRollActionIndex = CBoss::ACTION_BOSS_ROLL_WEST_REVERSE;
			break;
		case 2:
			m_vRollActionIndex = CBoss::ACTION_BOSS_ROLL_EAST_REVERSE;
			break;
		case 3:
			m_vRollActionIndex = CBoss::ACTION_BOSS_ROLL_EAST_REVERSE;
			break;
		}
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_STAND);
		m_eCurState = BOSS_AWARE_ROLL;
		/*{
		}
		else
		{
			switch (m_eDistanceState)
			{
			case DISTANCE_NEAR:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
				m_eCurState = BOSS_AWARE_NEAR_ATTACK;
				break;
			case DISTANCE_NORMAL:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
				m_eCurState = BOSS_AWARE_FIRE;
				break;
			case DISTANCE_FAR:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
				m_eCurState = BOSS_AWARE_FOLLOW;
				break;
			}
		}*/

		m_fNearAttackAccTime = 0.f;
		m_bNearAttackRestComplete = false;
	}
}

void CState_Aware_Boss::Tick_Roll(_float fTimeDelta)
{
	m_fRollAccTime += fTimeDelta;

	if (m_bRollRestComplete == false && m_fRollAccTime > 0.3f)
	{
		m_fRollAccTime = 0.f;
		m_bRollRestComplete = true;
		m_pOwnerDetailed->Set_NextAction(m_vRollActionIndex);
	}

	if (m_bRollRestComplete == true)
	{
		m_pOwnerDetailed->Go_Straight(fTimeDelta * 3.f); // TODO 구르는 속도 조정

		// 구르고 있고 다 굴렀으면
		if ((m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_ROLL_EAST ||
			m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_ROLL_WEST ||
			m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_ROLL_EAST_REVERSE ||
			m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_ROLL_WEST_REVERSE)
			&& m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			RefreshDir();

			switch (m_eDistanceState)
			{
			case DISTANCE_NEAR:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
				m_eCurState = BOSS_AWARE_NEAR_ATTACK;
				break;
			case DISTANCE_NORMAL:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
				m_eCurState = BOSS_AWARE_FIRE;
				break;
			case DISTANCE_FAR:
				m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
				m_eCurState = BOSS_AWARE_FOLLOW;
				break;
			}

			m_fRollAccTime = 0.f;
			m_bRollRestComplete = false;
		}
	}
}

void CState_Aware_Boss::Tick_Reload(_float fTimeDelta)
{
	// 리로드 애니메이션이 끝나면
	if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_RELOADING
		&& m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
	{
		m_iFireCount = 0;
		RefreshDir();
		switch (m_eDistanceState)
		{
		case DISTANCE_NEAR:
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
			m_eCurState = BOSS_AWARE_NEAR_ATTACK;
			return;
		case DISTANCE_NORMAL:
			m_eCurState = BOSS_AWARE_FIRE;
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
			return;
		case DISTANCE_FAR:
			m_eCurState = BOSS_AWARE_FOLLOW;
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
			return;
		}
	}
}

#pragma endregion Tick_State

void CState_Aware_Boss::RefreshDir()
{
	m_vDirXZ = m_pOwnerDetailed->Get_ToTargetXZ();
	m_pOwnerDetailed->Set_Look(m_vDirXZ);
}

void CState_Aware_Boss::RefreshDir(const _float3& vNewDir)
{
	m_vDirXZ = vNewDir;
	m_pOwnerDetailed->Set_Look(m_vDirXZ);
}

void CState_Aware_Boss::Set_RollDir(_int iRandom)
{
	_float fRad = 0.f;
	switch (iRandom)
	{
	case 0:
		fRad = D3DXToRadian(135); // west
		break;
	case 1:
		fRad = D3DXToRadian(135); // west
		break;
	case 2:
		fRad = D3DXToRadian(225); // east
		break;
	case 3:
		fRad = D3DXToRadian(225); // east
		break;
	default:
		fRad = D3DXToRadian(225);
		break;
	}

	_float fCos = cosf(fRad);
	_float fSin = sinf(fRad);

	m_vRollDir = {
		m_vDirXZ.x * fCos - m_vDirXZ.z * fSin,
		0.f,
		m_vDirXZ.x * fSin + m_vDirXZ.z * fCos
	};

}

_bool CState_Aware_Boss::Check_ShootSuccess(_float3 vRealToTargetDirXZ)
{
	//m_vDir(총구 방향) vs 타겟까지 실제 방향 각도 비교
	D3DXVec3Normalize(&vRealToTargetDirXZ, &vRealToTargetDirXZ);	

	return D3DXVec3Dot(&m_vDirXZ, &vRealToTargetDirXZ) > cosf(D3DXToRadian(20)); // TODO 각도 조정
}

void CState_Aware_Boss::Restart()
{
	// NearAttack
	m_fNearAttackAccTime = 0.f;
	m_bNearAttackRestComplete = false;
	// Roll
	m_fRollAccTime = 0.f;
	m_bRollRestComplete = false;
	// Fire
	m_fFireAccTime = 0.f;
	m_bFireRestComplete = false;
	m_bRealFired = false;

	m_fDistance = m_pOwnerDetailed->Get_Distance_ToTarget();

	if (m_fDistance < m_fNearMax)
	{
		m_eDistanceState = DISTANCE_NEAR;
	}
	else if (m_fDistance < m_fNormalMax)
	{
		m_eDistanceState = DISTANCE_NORMAL;
	}
	else
	{
		m_eDistanceState = DISTANCE_FAR;
	}

	static_cast<CBoss*>(m_pOwnerDetailed)->Set_DistanceState(m_eDistanceState);

	RefreshDir();

	switch (m_eDistanceState)
	{
	case DISTANCE_NEAR:
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_NEAR);
		m_eCurState = BOSS_AWARE_NEAR_ATTACK;
		break;
	case DISTANCE_NORMAL:
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
		m_eCurState = BOSS_AWARE_FIRE;
		break;
	case DISTANCE_FAR:
		m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR);
		m_eCurState = BOSS_AWARE_FOLLOW;
		break;
	}
}

#pragma endregion
