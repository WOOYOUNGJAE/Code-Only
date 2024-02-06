#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerGuardState.h"
#include "InteractiveObject.h"
#include "Player.h"

CPlayerState* CPlayerGuardState::Transition(CPlayer& _player)
{
	if (_player.GetHoldItem() != nullptr && _player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
	{
		if (!m_bPistolSpinFinish)
			return nullptr;
		else
		{
			_player.SetPlayerTex(IDLE);
			return new CPlayerIdleState(true);
		}
	}
	if ((m_bGuardEnd && m_fTime > 0.15f ) ||_player.GetPlayerDesc()->iGuardGauge < 1 )
	{
		_player.SetPlayerTex(IDLE);
		_player.SetAnimPosIdle();
		return new CPlayerIdleState(false);
	}
	return nullptr;
}

void CPlayerGuardState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	if (_player.GetHoldItem() == nullptr)
	{
		_player.SetPlayerTex(GUARDL);
		_player.SetPlayerTex2(GUARDR);
	}
	else
	{
		switch (_player.GetHoldItem()->Get_Item_Type())
		{
		case ITEM_PIPE:
			_player.SetPlayerTex(HEAVYREADY);
			_player.SetPlayerTex2(PIPEGUARD);
			break;
		case ITEM_AUTOMATIC_PISTOL:
			_player.SetPlayerTex(HEAVYREADY);
			_player.SetPlayerTex2(PISTOLIDLE);
			break;
		}
	}
	_player.SetSpeed(_player.GetSpeedInfo(IDLE));
	_player.SetGuard(true);
	m_pGameInstance->StopSound(SOUND_PLAYER_SWING);
}

void CPlayerGuardState::Update(CPlayer& _player, float fTimeDelta)
{
	m_fTime += fTimeDelta;
	m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	if (_player.GetHoldItem() != nullptr && _player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
	{
		if (m_fTime < 0.05f && m_fTime > 0.f)
		{
			m_pGameInstance->Play_Sound(L"GunSpin.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
			_player.SetPlayerOnlyTex2(PISTOLSPIN1);
			_player.AddRightX(fTimeDelta*500.f);
			_player.AddRightY(fTimeDelta*-500.f);
			_player.AddRightAngle(-fTimeDelta);
		}
		else if (m_fTime >= 0.05f && m_fTime < 0.1f)
		{
			_player.SetPlayerOnlyTex2(PISTOLSPIN2);
			_player.AddRightX(fTimeDelta * 500.f);
			_player.AddRightY(fTimeDelta * -500.f);
			_player.AddRightAngle(-fTimeDelta);
		}
		else if (m_fTime < 0.15f && m_fTime >= 0.1f)
		{
			_player.SetPlayerOnlyTex2(PISTOLSPIN3);
			_player.AddRightX(fTimeDelta * -500.f);
			_player.AddRightY(fTimeDelta * 500.f);
			_player.AddRightAngle(fTimeDelta);
		}
		else if (m_fTime >= 0.15f && m_fTime < 0.2f)
		{
			_player.SetPlayerOnlyTex2(PISTOLSPIN4);
			_player.AddRightX(fTimeDelta * -500.f);
			_player.AddRightY(fTimeDelta * 500.f);
			_player.AddRightAngle(fTimeDelta);
		}
		else if (m_fTime >= 0.2f)
		{
			_player.SetPistolMeelee(!_player.GetPistolMeelee());
			m_bPistolSpinFinish = true;
		}
		return;
	}

	if (!m_pGameInstance->Key_Pressing(VK_RBUTTON) && m_fTime > 0.15f&&!m_bGuardEnd)
	{
		m_bGuardEnd = true;
		m_fTime = 0.f;
	}
	if (_player.Move(fTimeDelta))
		_player.AnimWalk(fTimeDelta);
	else
		_player.AnimIdle(fTimeDelta);



	if (_player.GetHoldItem() == nullptr)
	{
		_player.AnimGuard(m_bGuardEnd, m_fTime, fTimeDelta);
	}
	else
	{
		switch (_player.GetHoldItem()->Get_Item_Type())
		{
		case ITEM_PIPE:
			if (!m_bGuardEnd)
			{
				if (m_fTime > 0.f && m_fTime < 0.15f)
				{
					if(!_player.GetDeliaHold())
					{
						_player.AddLeftX(-fTimeDelta * 500.f);
						_player.AddLeftY(fTimeDelta * 1500.f);
					}
					_player.AddRightX(-fTimeDelta * 400.f);
					_player.AddRightY(-fTimeDelta * 4000.f);
				}
			}
			break;
		}
	}
}

void CPlayerGuardState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.SetSpeed(0.f);
	_player.SetGuard(false);
	_player.SetRightAngle(0.f);
}
