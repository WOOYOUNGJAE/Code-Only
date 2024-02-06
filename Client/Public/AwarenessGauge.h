#pragma once
#include "Client_Defines.h"
#include "EnemyBase.h"


BEGIN(Client)
	class CAwarenessGauge : public CBase
{
private:
	CAwarenessGauge() = default;
	virtual ~CAwarenessGauge() override = default;

public:
	static CAwarenessGauge* Create();
	virtual void Free() override {};
public:
	_float Get_Gauge() { return m_fCurAwarenessGauge; }
	_uint Get_TextureFrame();
	_bool Is_GaugeMax() { return m_fCurAwarenessGauge >= m_fAwarenessGaugeMax; }
	_bool Is_Acting() { return m_bIsActing; } // �ʹ� �־ �������� ������ �Ÿ��� ������ false->�Ⱥ���,
	// ���� : ȭ��ǥ,����ǥ �߳� O vs ����ǥ ä������ X
	// �긦 ����ǥ�� ä�����ķ� �ٲٰ� ����ǥ �߳Ĵ� �Ÿ���
	void Set_AwarenessGauge(_float fTimeDelta);
	void Set_Speed_Angle(CEnemyBase::ENEMY_TEXTURE_DIR eTextureDir);
	void Set_Speed_Distance(_float fDistanceToTarget);
	void Set_Active(_bool _b) { m_bIsActing = _b; }
	void Reset_Gauge();
private:
	_bool m_bIsActing = true;

	// for Awareness Gauge
	_float m_fAccTime_Gauge = 0.f;
	_float m_fAccTime_Gauge_Decrease = 0.f;
	_float m_fCurAwarenessGauge = 0.f; // ���� ������
	_float m_fAwarenessGaugeMax = 1000000.f;
	_float m_fSpeed_Angle = 0.f;
	_float m_fGaugeSpeed_0Angle = 6.25f * 50 * 3     ;
	_float m_fGaugeSpeed_45Angle = 6.25f * 20    * 2 ;
	_float m_fGaugeSpeed_90Angle = 6.25f * 20 * 2;
	// ������ �Ÿ� �ݺ�ʷ�
	_float m_fSpeed_Distance = 0.f;
	_float m_fGaugeSpeedDelta_VeryNear = 200.f;
	_float m_fGaugeSpeedDelta_Near = 200.f;
	_float m_fGaugeSpeedDelta_Middle = 50.f;
	_float m_fGaugeSpeedDelta_Far = 0.75f;
	_float m_fGaugeSpeedDelta_VeryFar = 0.5f;
	// �Ÿ� ���� //TODO �ӽ�
	_float m_fVeryNearMax = 1.f;
	_float m_fNearMax = 3.f;
	_float m_fMiddleMax = 5.f;
	_float m_fFarMax = 10.f;
};

END
