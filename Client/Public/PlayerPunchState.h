#pragma once
#include "PlayerState.h"
class CPlayerPunchState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
private:
	_bool m_bAttackEnd = false;
	_int m_ePunchState = PUNCH_FORWARD;
	_bool m_bHeavyPunch = true;
	_float m_fHeavyPunchTime = 0.f;
	_float m_fTiltAngle = 0.f;
	_bool m_bInitPos = true;
	_bool m_bTex3 = false;
	_int m_iPipeIndex = 1;
	_bool m_bBottleAtk4 = false;
	_bool m_bThrow = false;
	_bool m_bGuardable = false;
};

