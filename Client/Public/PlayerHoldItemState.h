#pragma once
#include "PlayerState.h"
class CPlayerHoldItemState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
};

