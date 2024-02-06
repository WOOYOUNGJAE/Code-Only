#include "stdafx.h"
#include "State_Attacked.h"
#include "GameInstance.h"
#include "EnemyBase.h"
#include "Boss.h"

CState_Attacked_Enemy* CState_Attacked_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Attacked_Enemy* pInstance = new CState_Attacked_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Attacked_Enemy");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Attacked_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	CState_Enemy::Set_OwnerDetailed(static_cast<CGameObject*>(pArg));
	
	return S_OK;
}

void CState_Attacked_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Attacked_Enemy::BeginState(_float fTimeDelta)
{
	//m_pOwnerDetailed.
}

_int CState_Attacked_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_HEADLESS)
	{
		return FSM_NO_EVENT;
	}


	if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_LOW_PUNCHED)
	{
		m_fAnimAccTime += fTimeDelta;
		if (m_fAnimAccTime >= m_fAnimMaxTime)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE); //
			_uint iNextState = m_pOwnerDetailed->Get_FSM()->Get_PrevState();
			m_pOwnerDetailed->Set_NextFSMState(iNextState);
			return iNextState;
		}		
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_FACE_PUNCHED ||
		m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_BODY_PUNCHED)
	{
		if (m_pOwnerDetailed->Get_FrameLeft() <= 1)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE); //
			//_uint iNextState = m_pOwnerDetailed->Get_FSM()->Get_PrevState();
			_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::AWARE;
			m_pOwnerDetailed->Set_NextFSMState(iNextState);
			return iNextState;
		}
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_DAZED)
	{
		// 이벤트 Call?
		if (m_pOwnerDetailed->Is_Dazed() == false)
		{
			m_pOwnerDetailed->Erase_DazedEffect();
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE); //
			//_uint iNextState = m_pOwnerDetailed->Get_FSM()->Get_PrevState();
			_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::AWARE;
			m_pOwnerDetailed->Set_NextFSMState(iNextState);
			return iNextState;
		}
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_GET_UP)
	{
		if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE); //
			_uint iNextState = CEnemyBase::ENEMY_BASE_STATE::AWARE;
			m_pOwnerDetailed->Set_NextFSMState(iNextState);
			m_pOwnerDetailed->Set_TextureSize_Regular(false);
			return iNextState;
		}
	}
	else if (m_pOwnerDetailed->Is_Alive() == false) // Dead
	{
		_uint iNextAction = CEnemyBase::ACTION_DEAD_LYING;

		switch (m_pOwnerDetailed->Get_CurAction())
		{
		case CEnemyBase::ACTION_FALLING_START:
			iNextAction = CEnemyBase::ACTION_FALLING_END;
			break;
		case CEnemyBase::ACTION_FALLING_END:
			iNextAction = CEnemyBase::ACTION_DEAD_LYING;
			break;

		}
		if (m_pOwnerDetailed->Get_FrameLeft() <= 2)
		{
			m_pOwnerDetailed->Set_NextAction(iNextAction);
		}
	}
	else if (m_pOwnerDetailed->Is_Alive() == true) // 죽진 않고 떨어지는 상태
	{
		_uint iNextAction = CEnemyBase::ACTION_DEAD_LYING;

		if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_FALLING_START)
		{
			iNextAction = CEnemyBase::ACTION_FALLING;
		}
		else if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_FALLING_START
			&& m_pOwnerDetailed->Has_Magnitude() == false)
		{
			iNextAction = CEnemyBase::ACTION_DEAD_LYING;
		}
		else if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_DEAD_LYING)
		{
			if (m_pOwnerDetailed->Is_Executed())
			{
				m_pOwnerDetailed->Set_Chopped(false);
				//m_pOwnerDetailed->Enable(false);
				m_pOwnerDetailed->Make_Dead();
				return FSM_NO_EVENT;
			}
			else // 처형이 아닌 강펀치 등으로 인해 날라간거라면
			{
				m_fAnimAccTime += fTimeDelta;
				if (m_fAnimAccTime > 2.f)
				{
					m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_GET_UP);	
					m_pOwnerDetailed->Set_InAir(true);
				}
			}
		}

		if (m_pOwnerDetailed->Get_FrameLeft() <= 2)
		{
			m_pOwnerDetailed->Set_NextAction(iNextAction);
			if (m_pOwnerDetailed->Get_CurAction() == CEnemyBase::ACTION_GET_UP)
			{
				//m_pOwnerDetailed->Set_InAir(false);
			}
		}
	}
	
	//m_pOwnerDetailed->Set_CurAction()
	return FSM_NO_EVENT;
}

void CState_Attacked_Enemy::CompleteState(_float fTimeDelta)
{
	m_pOwnerDetailed->Reset_CautiousState();
}

_int CState_Attacked_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}

// -------------boss

CState_Attacked_Boss* CState_Attacked_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Attacked_Boss* pInstance = new CState_Attacked_Boss(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Attacked_Boss");
	}
	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Attacked_Boss::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	CState_Enemy::Set_OwnerDetailed(static_cast<CGameObject*>(pArg));

	m_pOwnerBoss = static_cast<CBoss*>(m_pOwnerDetailed);

	return S_OK;
}

void CState_Attacked_Boss::Free()
{
	CState_Enemy::Free();
}

void CState_Attacked_Boss::BeginState(_float fTimeDelta)
{
	CState_Enemy::BeginState(fTimeDelta);
}

_int CState_Attacked_Boss::Tick_State(_float fTimeDelta, void* pArg)
{
	if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_LOW_PUNCHED)
	{
		m_fAnimAccTime += fTimeDelta;
		if (m_fAnimAccTime >= 0.3f)
		{
			m_fAnimAccTime = 0.f;

			m_pOwnerDetailed->Set_NextFSMState(CEnemyBase::ENEMY_BASE_STATE::AWARE);
			m_pOwnerBoss->Restart_AwareState();
			return FSM_NO_EVENT;
		}
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FACE_PUNCHED ||
		m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_BODY_PUNCHED)
	{
		if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextFSMState(CEnemyBase::ENEMY_BASE_STATE::AWARE);
			m_pOwnerBoss->Restart_AwareState();
			return FSM_NO_EVENT;
		}
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_DAZED) // 안씀
	{
		// 이벤트 Call?
		if (m_pOwnerDetailed->Is_Dazed() == false)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_BATTLE_IDLE_FAR); //
			m_pOwnerDetailed->Set_NextFSMState(CEnemyBase::ENEMY_BASE_STATE::AWARE);
			m_pOwnerBoss->Restart_AwareState();
			return FSM_NO_EVENT;
		}
	}
	else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_GET_UP)
	{
		if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			m_fAnimAccTime = 0.f;
			m_pOwnerDetailed->Set_NextFSMState(CEnemyBase::ENEMY_BASE_STATE::AWARE);
			m_pOwnerBoss->Restart_AwareState();
			return FSM_NO_EVENT;
		}
	}
	else if (m_pOwnerDetailed->Is_Alive() == false) // Dead
	{
		_uint iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;

		switch (m_pOwnerDetailed->Get_CurAction())
		{
		case CBoss::ACTION_BOSS_FALLING_START:
			iNextAction = CBoss::ACTION_BOSS_FALLING_END;
			break;
		case CBoss::ACTION_BOSS_FALLING_END:
			iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;
			break;

		}
		if (m_pOwnerDetailed->Get_FrameLeft() <= 2)
		//if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			m_pOwnerDetailed->Set_NextAction(iNextAction);
		}
	}
	else if (m_pOwnerDetailed->Is_Alive() == true) // 죽진 않고 떨어지는 상태
	{
		_uint iNextAction = m_pOwnerDetailed->Get_CurAction();

		if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING_START)
		{
			if (m_pOwnerDetailed->Has_Magnitude() == true)
			{
				iNextAction = CBoss::ACTION_BOSS_FALLING;
			}
			else
			{
				iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;
			}
		}
		else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING)
		{
			if (m_pOwnerDetailed->Has_Magnitude() == true)
			{
				iNextAction = CBoss::ACTION_BOSS_FALLING_END;
			}
			else
			{
				iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;
			}
		}
		/*else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING_START
			&& m_pOwnerDetailed->Has_Magnitude() == false)
		{
			iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;
		}*/
		else if ((m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING_START || m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING ||
			m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_FALLING_END)
			&& m_pOwnerDetailed->Has_Magnitude() == false)
		{
			iNextAction = CBoss::ACTION_BOSS_DEAD_LYING;
		}
		else if (m_pOwnerDetailed->Get_CurAction() == CBoss::ACTION_BOSS_DEAD_LYING)
		{
			if (m_pOwnerDetailed->Is_Executed())
			{
				m_pOwnerDetailed->Set_Chopped(false);
				//m_pOwnerDetailed->Enable(false);
				m_pOwnerDetailed->Make_Dead();
				return FSM_NO_EVENT;
			}
			else // 처형이 아닌 강펀치 등으로 인해 날라간거라면
			{
				m_fAnimAccTime += fTimeDelta;
				if (m_fAnimAccTime > 2.f)
				{
					m_pOwnerDetailed->Set_NextAction(CBoss::ACTION_BOSS_GET_UP);
					m_pOwnerDetailed->Set_InAir(true);
				}
			}
		}

		//if (m_pOwnerDetailed->Get_FrameLeft() <= 1)
		if (m_pOwnerDetailed->Get_AnimPlayedCount() >= 1)
		{
			m_pOwnerDetailed->Set_NextAction(iNextAction);
		}
	}

	//m_pOwnerDetailed->Set_CurAction()
	return FSM_NO_EVENT;
}

void CState_Attacked_Boss::CompleteState(_float fTimeDelta)
{
	CState_Enemy::CompleteState(fTimeDelta);
}

_int CState_Attacked_Boss::LateTick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}
