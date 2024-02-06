#pragma once
#include "PlayerState.h"
class CPlayerExecuteState :
    public CPlayerState
{
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
private:
	_bool m_bEnd = false;
	_float m_fSpinReadyTime = 0.17f;
	PLAYER_TEX eEnemyTex = GRAB1;
};

