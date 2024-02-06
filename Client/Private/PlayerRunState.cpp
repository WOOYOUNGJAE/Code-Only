#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerRunState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "Player.h"
#include "Physics.h"
#include "Transform.h"

CPlayerRunState::CPlayerRunState(_float fTime) : m_fTime(fTime)
{
}

CPlayerState* CPlayerRunState::Transition(CPlayer& _player)
{
	if (_player.GetPlayerDesc()->fStamina <= 0.f)
	{
		_player.SetPlayerTex(IDLE);
		_player.SetAnimPosIdle();
		return new CPlayerWalkState(false);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		return new CPlayerJumpState(true);
	}
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('W')&0x8000))
	{
		return nullptr;
	}
	if ((GetAsyncKeyState('W') & 0x8000))
	{
		_player.SetPlayerTex(IDLE);
		_player.SetAnimPosIdle();
		return new CPlayerWalkState(false);
	}
	_player.SetPlayerTex(IDLE);
	_player.SetAnimPosIdle();
	return new CPlayerIdleState(false);
}

void CPlayerRunState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	_player.SetSpeed(0.7f);
	//m_pGameInstance->StopSound(SOUND_PLAYER_WALK);
}

void CPlayerRunState::Update(CPlayer& _player, float fTimeDelta)
{
	m_pTransform->SetDir(m_pTransform->Get_State(CTransform::STATE_LOOK));
	m_pTransform->Go_Dir(fTimeDelta);
	_player.AnimRun(fTimeDelta);

	m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	_float fUse = -0.5f;
	_player.ChangeStamina(&fUse);
	_player.GetPlayerDesc()->bStaminaChanged = true;

	m_pGameInstance->Play_Sound(L"FootstepConcrete2.wav", SOUND_PLAYER_WALK, _player.GetSoundVolume());
}

void CPlayerRunState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
}
