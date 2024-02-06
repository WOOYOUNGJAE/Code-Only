#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerKickState.h"
#include "Player.h"
#include "Physics.h"
#include "InteractiveObject.h"

CPlayerState* CPlayerKickState::Transition(CPlayer& _player)
{
	if (m_fTime > 0.6f)
	{
		_player.SetPlayerTex(IDLE);
		_player.SetAnimPosIdle();
		return new CPlayerIdleState(false);
	}
	return nullptr;
}

void CPlayerKickState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	_player.SetSpeed(_player.GetSpeedInfo(SPEED_WALK));
	_player.SetPlayerTex(KICK1);
	_float fUse = -10.f;
	_player.ChangeStamina(&fUse);
	_player.GetPlayerDesc()->bStaminaChanged = true;
}

void CPlayerKickState::Update(CPlayer& _player, float fTimeDelta)
{
	m_fTime += fTimeDelta;
	_player.Move(fTimeDelta);


	m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);
	

	_player.ResetCamera(fTimeDelta);

	_player.AnimKick(m_fTime,fTimeDelta);

	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);


	if (m_fTime > 0.2f)
	{
		PLAYERATTACKDESC at;
		at.bHeavy = false;
		at.bKick = true;
		at.pPlayerTransform = m_pTransform;
		at.vPickPos = _player.GetPickItemCollidedPos();
		at.eType = ITEM_NONE;
		at.iDmg = 30;
		SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
		m_pGameInstance->Play_Sound(L"Kick.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
	}

	if (m_fTime > 0.5f)
	{
		
		if (m_fTiltAngle > 0.f)
		{
			m_fTiltAngle -= fTimeDelta;
			m_pTransform->Turn(-vRight, fTimeDelta*0.5f);
		}
	}
	else 
	{
		if (m_fTiltAngle < 0.1f)
		{
			m_fTiltAngle += fTimeDelta;
			m_pTransform->Turn(vRight, fTimeDelta * 0.5f);
		}
	}
}

void CPlayerKickState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.SetSpeed(0.f);
}
