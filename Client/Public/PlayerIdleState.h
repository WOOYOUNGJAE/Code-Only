#pragma once
#include "PlayerState.h"
class CPlayerIdleState :
    public CPlayerState
{
public:
	//_b->AnimToIdleEnd : idle��ȯ �ִϸ��̼��� ��������
	CPlayerIdleState(_bool _b);
	~CPlayerIdleState() = default;
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
};

