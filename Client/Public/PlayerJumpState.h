#pragma once
#include "PlayerState.h"
class CPlayerJumpState :
    public CPlayerState
{
public:
	CPlayerJumpState(_bool run);
	~CPlayerJumpState() = default;
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;

private:
	_bool m_bRun = false;
};

