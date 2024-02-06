#include "stdafx.h"
#include "AwarenessGauge.h"

CAwarenessGauge* CAwarenessGauge::Create()
{
	CAwarenessGauge* pInstance = new CAwarenessGauge();

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CAwarenessGauge");
	}
	return pInstance;
}


_uint CAwarenessGauge::Get_TextureFrame()
{
	return static_cast<_uint>(m_fCurAwarenessGauge / m_fAwarenessGaugeMax * 15);
}

void CAwarenessGauge::Set_AwarenessGauge(_float fTimeDelta)
{
	m_bIsActing = m_fSpeed_Distance == -m_fGaugeSpeed_0Angle ? false : true;

	if (m_fSpeed_Angle == 0) // 계산 할거냐 말거냐
	{
		return;
	}

	int iApproxTime = 0;
	if (m_fSpeed_Angle >= 0)
	{
		m_fAccTime_Gauge += fTimeDelta;
		m_fAccTime_Gauge_Decrease -= fTimeDelta;
		// 소수점 버림으로 연산량 줄이기
		iApproxTime = static_cast<int>(m_fAccTime_Gauge * 1000000.f) / 1000000;
		m_fCurAwarenessGauge += m_fSpeed_Angle * m_fSpeed_Distance
			* (iApproxTime * iApproxTime);
	}
	else
	{
		m_fAccTime_Gauge -= fTimeDelta;
		m_fAccTime_Gauge_Decrease += fTimeDelta;
		iApproxTime = static_cast<int>(m_fAccTime_Gauge_Decrease * 1000000.f) / 1000000;
		m_fCurAwarenessGauge += m_fSpeed_Angle * m_fSpeed_Distance
			* (iApproxTime * iApproxTime );
	}

	

	// Clamp
	if (m_fCurAwarenessGauge < 0.f)
		m_fCurAwarenessGauge = 0.f;
	else if (m_fCurAwarenessGauge > m_fAwarenessGaugeMax)
	{
		m_fCurAwarenessGauge = m_fAwarenessGaugeMax;
	}

	if (m_fAccTime_Gauge < 0.f)
	{
		m_fAccTime_Gauge = 0.f;
	}
	if (m_fAccTime_Gauge_Decrease < 0.f)
	{
		m_fAccTime_Gauge_Decrease = 0.f;
	}
}

void CAwarenessGauge::Set_Speed_Angle(CEnemyBase::ENEMY_TEXTURE_DIR eTextureDir)
{
	switch (eTextureDir)
	{
	case CEnemyBase::ENEMY_TEXTURE_DIR::EAST:
		m_fSpeed_Angle = m_fGaugeSpeed_90Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::WEST:
		m_fSpeed_Angle = m_fGaugeSpeed_90Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::SOUTH_EAST:
		m_fSpeed_Angle = m_fGaugeSpeed_45Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::SOUTH_WEST:
		m_fSpeed_Angle = m_fGaugeSpeed_45Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::SOUTH:
		m_fSpeed_Angle = m_fGaugeSpeed_0Angle;
		break;
	// 감소
	case CEnemyBase::ENEMY_TEXTURE_DIR::NORTH_EAST:
		m_fSpeed_Angle = -m_fGaugeSpeed_45Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::NORTH_WEST:
		m_fSpeed_Angle = -m_fGaugeSpeed_45Angle;
		break;
	case CEnemyBase::ENEMY_TEXTURE_DIR::NORTH:
		m_fSpeed_Angle = -m_fGaugeSpeed_0Angle;
		break;

	default:
		m_fSpeed_Angle = 0.f;
		break;
	}
}

void CAwarenessGauge::Set_Speed_Distance(_float fDistanceToTarget)
{
	if (fDistanceToTarget < m_fVeryNearMax)
	{
		//m_fSpeed_Distance = m_fGaugeSpeedDelta_VeryNear;
		m_fSpeed_Distance = m_fSpeed_Angle >= 0 ? m_fGaugeSpeedDelta_Near : m_fGaugeSpeedDelta_VeryFar;
	}
	else if (fDistanceToTarget < m_fNearMax)
	{
		//m_fSpeed_Distance = m_fGaugeSpeedDelta_Near;
		m_fSpeed_Distance = m_fSpeed_Angle >= 0 ? m_fGaugeSpeedDelta_Middle : m_fGaugeSpeedDelta_Far;
	}
	else if (fDistanceToTarget < m_fMiddleMax)
	{
		//m_fSpeed_Distance = m_fGaugeSpeedDelta_Middle;
		m_fSpeed_Distance = m_fSpeed_Angle >= 0 ? m_fGaugeSpeedDelta_Far : m_fGaugeSpeedDelta_Middle;
	}
	else if (fDistanceToTarget < m_fFarMax)
	{
		//m_fSpeed_Distance = m_fGaugeSpeedDelta_Far;
		m_fSpeed_Distance = m_fSpeed_Angle >= 0 ? m_fGaugeSpeedDelta_VeryFar : m_fGaugeSpeedDelta_VeryNear;
	}
	else
	{
		m_fAccTime_Gauge = 0;
		m_fAccTime_Gauge_Decrease = 0;
		m_fSpeed_Distance = 0.f;
	}
}

void ::CAwarenessGauge::Reset_Gauge()
{
	m_fCurAwarenessGauge = 0.f;
	m_fAccTime_Gauge = 0.f;
}
