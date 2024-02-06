#pragma once
#include "PlayerState.h"
class CPlayerWalkState :
    public CPlayerState
{
public:
	CPlayerWalkState(_bool _b);
	~CPlayerWalkState() = default;
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
};

