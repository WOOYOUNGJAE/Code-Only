#include "stdafx.h"
#include "Enemy_HealthSystem.h"
#include "EnemyBase.h"

CEnemy_HealthSystem* CEnemy_HealthSystem::Create(CEnemyBase* pOwner)
{
	CEnemy_HealthSystem* pInstance = new CEnemy_HealthSystem();

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CEnemy_HealthSystem");
	}

	pInstance->m_pOwner = pOwner;

	return pInstance;
}

void CEnemy_HealthSystem::Initialize()
{
}

_bool CEnemy_HealthSystem::Tick(_float fTimeDelta, _bool& refMakeDazed)
{
	Tick_CalculateHP(fTimeDelta);

	if (0 < m_iCurHP && m_iCurHP <= 45)
	{
		refMakeDazed = true;
	}
	else if (m_pOwner->Get_CurAction() == CEnemyBase::DAZED)
	{
		m_pOwner->Erase_DazedEffect();
	}
	return m_iCurHP > 0; // Alive?
}

#pragma region MyRegion
void CEnemy_HealthSystem::LateTick(_float fTimeDelta)
{
}

void CEnemy_HealthSystem::Free()
{
}
#pragma endregion

void CEnemy_HealthSystem::Tick_CalculateHP(_float fTimeDelta)
{
	if (0 < m_iCurHP && m_iCurHP < 50)
	{
		Tick_Recover(fTimeDelta);
	}
	else
	{
		m_fRecoverAccTime = 0.f;
	}
	
}

void CEnemy_HealthSystem::Tick_Recover(_float fTimeDelta)
{	
	m_fRecoverAccTime += fTimeDelta;
	if (m_fRecoverAccTime > 1.6f)
	{
		m_iPrevHP = m_iCurHP;
		m_iCurHP += 5;
		if (m_iCurHP <= 0)
			int a = 1;
		//m_pOwner->Send_HPInfo(m_iPrevHP, m_iCurHP);
		if (m_iCurHP > 50)
		{
			m_iCurHP = 50;
		}
		m_fRecoverAccTime = 0.f;
	}
}

void CEnemy_HealthSystem::Damaged(_int iDamage)
{
	// 이미 기절 hp 미만인 상태에서 한번 더 맞았을 때 진짜 기절로
	if (0 < m_iCurHP && m_iCurHP < m_iDazeHP && m_pOwner->Get_EnemyType() != ENEMY_TYPE_MALONE/*보스는 기절 안함*/) // 기절 상태
	{
		m_pOwner->Create_DazedEffect();
		m_bMakeDazed = true;// Play Sound
		m_pOwner->Set_NextAction(CEnemyBase::ACTION_DAZED);
		m_pOwner->Play_MySound(TEXT("Dazed1.wav"));
	}

	m_iPrevHP = m_iCurHP;
	m_iCurHP -= iDamage;

	if (m_iCurHP <= 0) // 음수 제한
	{
		m_iCurHP = 0;
	}
}
