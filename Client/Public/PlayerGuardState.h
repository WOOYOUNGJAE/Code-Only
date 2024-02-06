#pragma once
#include "PlayerState.h"
class CPlayerGuardState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
private:
	_bool m_bGuardEnd = false;
	_float m_fTime = 0.f;
	_bool m_bPistolSpinFinish = false;
};

