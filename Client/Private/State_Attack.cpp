#include "stdafx.h"
#include "State_Attack.h"
#include "State_Battle.h"
#include "GameInstance.h"
#include <EnemyBase.h>

CState_Attack_Enemy* CState_Attack_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Attack_Enemy* pInstance = new CState_Attack_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Attack_Enemy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Attack_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CState_Attack_Enemy::Free()
{
	CState_Enemy::Free();
}


void CState_Attack_Enemy::BeginState(_float fTimeDelta)
{
	// Ready Action
}

_int CState_Attack_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	if ((*static_cast<_uint*>(pArg) & (1 << 2)) > 0) // heavyAttack 이면
	{
		if (m_bHeavyAttackWindUpFinished == false)
		{
			// WindUp 애니메이션 실행
			m_fWindUpAccTime += fTimeDelta;
			if (m_fWindUpAccTime < m_fWindUpMaxTime)
			{
				return FSM_NO_EVENT;
			}
			// 1번만 실행
			m_pOwnerDetailed->Play_MySound(TEXT("HmRargh!.wav"));
			/*if (m_pOwnerDetailed->Get_EnemyType() == ENEMY_TYPE_GOON)
			{
			}*/
			/*else if (m_pOwnerDetailed->Get_EnemyType() == ENEMY_TYPE_PIPE_MAN)
			{
				m_pOwnerDetailed->Play_MySound(TEXT("Swing1.wav"));				
			}*/
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_HEAVY_ATTACK);
			m_fWindUpAccTime = 0.f;
			m_bHeavyAttackWindUpFinished = true;
		}
	}


	if (m_pOwnerDetailed->Get_FrameLeft() <= 3)
	{
		//m_pOwnerDetailed->Set_CurAnimSpeedMultiplied(0.2f);
		m_bHeavyAttackWindUpFinished = false;
		_uint iNextState = 1; //Moving

		//*static_cast<_uint*>(pArg) = (1 << 3);// KnockBack True

		m_pOwnerDetailed->Set_KnockBackDir();

		m_pOwnerDetailed->Add_Force(-m_pOwnerDetailed->Get_ToTargetXZ(), 15);
		//return  nullptr;

		// Check Success And Trigger Event
		if (m_pOwnerDetailed->Check_AttackSuccess())
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			_uint iDamage = -7;
			ENEMYATTACKDESC at;
			at.pEnemyTransform = static_cast<CTransform*>(m_pOwnerDetailed->Find_Component(TEXT("Com_Transform")));
			at.iDmg = iDamage;
			pGameInstance->OnEvent(EVENT_ENEMY_ATTACK, &at);

			Safe_Release(pGameInstance);
		}

		if (rand() % 10 > 7)
		{
			m_pOwnerDetailed->Set_Stepable(true);
		}

		m_pOwnerDetailed->Set_Rest(true);
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
		return iNextState;
	}
	
	
	// 애니메이션 다 찼으면 어택 트리거

	return  FSM_NO_EVENT;
}

void CState_Attack_Enemy::CompleteState(_float fTimeDelta)
{
}

_int CState_Attack_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return FSM_NO_EVENT;
}


#pragma region CState_Attack_PipeGuy
CState_Attack_PipeGuy* CState_Attack_PipeGuy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Attack_PipeGuy* pInstance = new CState_Attack_PipeGuy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Attack_PipeGuy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Attack_PipeGuy::Initialize(void* pArg)
{
	return CState_Attack_Enemy::Initialize(pArg);
}

void CState_Attack_PipeGuy::BeginState(_float fTimeDelta)
{
	CState_Attack_Enemy::BeginState(fTimeDelta);
}

_int CState_Attack_PipeGuy::Tick_State(_float fTimeDelta, void* pArg)
{
	//if ((*static_cast<_uint*>(pArg) & (1 << 2)) > 0) // heavyAttack 이면
	//{
	//	if (m_bHeavyAttackWindUpFinished == false)
	//	{
	//		// WindUp 애니메이션 실행
	//		m_fWindUpAccTime += fTimeDelta;
	//		if (m_fWindUpAccTime < m_fWindUpMaxTime)
	//		{
	//			return FSM_NO_EVENT;
	//		}
	//		// 1번만 실행
	//		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_HEAVY_ATTACK);
	//		m_fWindUpAccTime = 0.f;
	//		m_bHeavyAttackWindUpFinished = true;
	//	}
	//}


	if (m_pOwnerDetailed->Get_FrameLeft() <= 1)
	{
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);


		//m_pOwnerDetailed->Set_CurAnimSpeedMultiplied(0.2f);
		m_bHeavyAttackWindUpFinished = false;
		_uint iNextState = 0; // Attack Ready

		//*static_cast<_uint*>(pArg) = (1 << 3);// KnockBack True

		m_pOwnerDetailed->Set_KnockBackDir();

		m_pOwnerDetailed->Add_Force(-m_pOwnerDetailed->Get_ToTargetXZ(), 15);
		//return  nullptr;

		// Check Success And Trigger Event
		if (m_pOwnerDetailed->Check_AttackSuccess())
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			_int iDamage = -7;
			ENEMYATTACKDESC at;
			at.pEnemyTransform = static_cast<CTransform*>(m_pOwnerDetailed->Find_Component(TEXT("Com_Transform")));
			at.iDmg = iDamage;
			pGameInstance->OnEvent(EVENT_ENEMY_ATTACK, &at);

			Safe_Release(pGameInstance);
		}
		return iNextState;
	}


	// 애니메이션 다 찼으면 어택 트리거

	return  FSM_NO_EVENT;
}

void CState_Attack_PipeGuy::CompleteState(_float fTimeDelta)
{
	CState_Attack_Enemy::CompleteState(fTimeDelta);
}

_int CState_Attack_PipeGuy::LateTick_State(_float fTimeDelta, void* pArg)
{
	return CState_Attack_Enemy::LateTick_State(fTimeDelta, pArg);
}

void CState_Attack_PipeGuy::Free()
{
	CState_Attack_Enemy::Free();
}
#pragma endregion CState_Attack_PipeGuy
