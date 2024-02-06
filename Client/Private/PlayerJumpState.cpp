#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerRunState.h"
#include "PlayerPunchState.h"
#include "PlayerKickState.h"
#include "Physics.h"
#include "Transform.h"
#include "Player.h"

CPlayerJumpState::CPlayerJumpState(_bool run) : m_bRun(run)
{
}

CPlayerState* CPlayerJumpState::Transition(CPlayer& _player)
{
	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		return new CPlayerPunchState();
	}

	if (m_pGameInstance->Key_Down('Q'))
	{
		return new CPlayerKickState();
	}

	if (m_pPhysics->GetGround())
	{
		if (m_bRun)
 			return new CPlayerRunState(_player.GetTime());
		else
		{
			return new CPlayerWalkState(true);
		}
	}

	return nullptr;
}

void CPlayerJumpState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	m_pPhysics->SetJumpStart();
	_float fSpeed = m_bRun ? _player.GetSpeedInfo(SPEED_RUN) : _player.GetSpeedInfo(SPEED_WALK);
	_player.SetSpeed(fSpeed);
	_player.SetJump(true);
	_float fUse = -10.f;
	_player.ChangeStamina(&fUse);
	_player.GetPlayerDesc()->bStaminaChanged = true;
	m_pGameInstance->StopSound(SOUND_PLAYER_WALK);
	m_pGameInstance->Play_Sound(L"Windup2.wav", SOUND_PLAYER_WALK, _player.GetSoundVolume());
}

void CPlayerJumpState::Update(CPlayer& _player, float fTimeDelta)
{
	m_pPhysics->JumpPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	_player.Move(fTimeDelta);

	if (m_bRun)
		_player.AnimRun(fTimeDelta);
	else
		_player.AnimWalk(fTimeDelta);
}

void CPlayerJumpState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	m_pTransform->SetDir({ 0.f,0.f,0.f });
}
