#pragma once
#include "PlayerState.h"
class CPlayerKickState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;

private:
	_float m_fTime = 0.f;
	_float m_fTiltAngle = 0.f;
};

