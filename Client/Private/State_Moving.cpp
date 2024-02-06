#include "stdafx.h"
#include "State_Moving.h"
#include "State_Battle.h"
#include "GameInstance.h"
#include <EnemyBase.h>

CState_Moving_Enemy* CState_Moving_Enemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed)
{
	CState_Moving_Enemy* pInstance = new CState_Moving_Enemy(pGraphic_Device);

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Instantiate CState_Moving_Enemy");
	}

	pInstance->Initialize(pOwnerDetailed);
	return pInstance;
}

HRESULT CState_Moving_Enemy::Initialize(void* pArg)
{
	if (FAILED(CState_Enemy::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Add_InnerFSM(LEVEL_STATIC, TEXT("Prototype_Component_FSM"))))
		return E_FAIL;

	return S_OK;
}

void CState_Moving_Enemy::Free()
{
	CState_Enemy::Free();
}

void CState_Moving_Enemy::BeginState(_float fTimeDelta)
{
	m_vStepStartDir = m_pOwnerDetailed->Get_ToTargetXZ();
	m_pOwnerDetailed->Set_KnockBackDir();


	//m_bStepable = false;
}

_int CState_Moving_Enemy::Tick_State(_float fTimeDelta, void* pArg)
{
	if (m_pOwnerDetailed->Get_Rest() == true)
	{
		m_fRestAccTime += fTimeDelta;
		if (m_fRestAccTime > m_fRestMaxTime)
		{
			m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
			m_pOwnerDetailed->Set_Rest(false);
		}
		else
		{
			return FSM_NO_EVENT;
		}
	}

	_uint iNextState = 0;

	if (m_bIsStrafing == true )
	{
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_STRAFING);
		if (m_pOwnerDetailed->Has_Magnitude() == true)
		{
			return FSM_NO_EVENT; // 스텝중이면 그대로 두			
		}
		else // 다 움직였으면
		{
			m_bIsStrafing = false;
			m_pOwnerDetailed->Set_Stepable(false);
			//m_bStepable = false;
		}
	}
	// 스텝 할 예정인데 움직이고 있지 않으면 움직이게 하기
	if (m_pOwnerDetailed->Is_Stepable() && m_bIsStrafing == false)
	{
		m_pOwnerDetailed->Set_Stepable(false);
		m_bIsStrafing = true;
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_STRAFING);
		_float fIfRight = 1;
		if (rand() % 2 == 0)
		{
			fIfRight *= -1;
		}
		//m_pOwnerDetailed->Add_Force(fIfRight * m_pOwnerDetailed->Get_Right(), m_fStrafingMagnitude);
		_float3 vCurRight = m_pOwnerDetailed->Get_TargetRight();
		m_pOwnerDetailed->Add_Force(fIfRight * *D3DXVec3Normalize(&vCurRight, &vCurRight), m_fStrafingMagnitude, 0.9);
		

		return FSM_NO_EVENT;
	}
	else if (m_pOwnerDetailed->ApproachtoTarget(m_fMinApproachDistance, fTimeDelta) == false)
	{
		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
		return CState_Battle_Enemy::BATTLE_STATE::ATTACK_READY;
	}
#pragma region reb
	//else // 쉬는 시간
	//{
	//	if (m_pOwnerDetailed->Has_Magnitude() == true) // 아직 이동중이라면
	//	{
	//		return FSM_NO_EVENT;
	//	}
	//	// If Not Moving
	//	// If Ready To Attack
	//	//_float f = m_pOwnerDetailed->Get_CurFrame();
	//	if (m_pOwnerDetailed->Get_Distance_ToTarget() < m_fDist_AttackReady && m_bIsStrafing) 
	//	{
	//		m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);
	//		iNextState = 0; // AttackReady
	//		return iNextState;
	//	}

	//	// Should Move
	//	m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_BATTLE_IDLE);

	//	m_fRestAccTime += fTimeDelta;
	//	if (m_fRestAccTime >= m_fRestMaxTime) // 다시 Stepable
	//	{
	//		m_fRestAccTime = 0.f;
	//		m_bStepable = true;

	//		// 다음 쉬는 시간 랜덤, 최대 1초
	//		m_fRestMaxTime = static_cast<_float>(rand() % 100) * 0.01f;
	//		if (m_fRestMaxTime < 0.7f)
	//		{
	//			m_fRestMaxTime = 0.7f;
	//		}

	//		int iRandom = rand() % 10;

	//		if (iRandom > 7) // Strafing Start
	//		{
	//			m_bIsStrafing = true;
	//		}
	//		/*else if (iRandom >= 9)
	//		{
	//			m_bIsJumpingBack = true;
	//		}*/
	//		//else if (iRandom < 1) // TODO : 확률 조정. 랜덤으로 돌진
	//		//{
	//		//	m_pOwnerDetailed->Set_NextAction(CEnemyBase::ACTION_RUN);
	//		//	iNextState = 2; // Rush
	//		//	return &iNextState;
	//		//}
	//	}
	//	return FSM_NO_EVENT;
	//}
#pragma endregion
	return iNextState;
}

void CState_Moving_Enemy::CompleteState(_float fTimeDelta)
{
	m_fCurrentStep = 0.f;
	m_pOwnerDetailed->Set_Stepable(false);
	m_fRestAccTime = 0.f;
	m_fKnockBackAccDistance = 0.f;
	m_bIsStrafing = false;
}

_int CState_Moving_Enemy::LateTick_State(_float fTimeDelta, void* pArg)
{	
	return FSM_NO_EVENT;
}