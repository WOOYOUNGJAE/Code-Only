#pragma once
#include "PlayerState.h"
class CPlayerEvadeState :
    public CPlayerState
{
	enum EVADE_DIR{EVADE_A, EVADE_SA, EVADE_D, EVADE_SD, EVADE_S};
public:
	virtual CPlayerState* Transition(class CPlayer& _player) override;
	virtual void Entry(class CPlayer& _player) override;
	virtual void Update(class CPlayer& _player, float fTimeDelta) override;
	virtual void Exit(class CPlayer& _player) override;
private:
	_float3 vInitPos;
	class CTransform* m_pCameraTransform = nullptr;
	_float fMoveY = 0.f;
	EVADE_DIR m_eDir;
};

