#pragma once
#include "PlayerState.h"
class CPlayerHeavyPunchState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;

private:
	_float m_fTime = 0.f;
	_bool m_bCharge = true;
	_bool m_bPunch = false;
	_float m_fForce = 1.f;
	_bool  m_bPipeHeavyAtk1 = false;
};
