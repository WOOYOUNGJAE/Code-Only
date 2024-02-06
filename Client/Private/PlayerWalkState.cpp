#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerPunchState.h"
#include "PlayerGuardState.h"
#include "PlayerKickState.h"
#include "PlayerEvadeState.h"
#include "PlayerCrouchState.h"
#include "Player.h"
#include "Physics.h"
#include "InteractiveObject.h"
#include "Item_Equipment.h"


CPlayerWalkState::CPlayerWalkState(_bool _b)
{
	m_bAnimToIdleEnd = _b;
}

CPlayerState* CPlayerWalkState::Transition(CPlayer& _player)
{


		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (_player.GetPlayerDesc()->fStamina > 10.f)
			{
				CItem_Equipment* eq = dynamic_cast<CItem_Equipment*>(_player.GetHoldItem());
				_bool b = eq != nullptr && eq->GetBulletCount() <= 0 && ((eq->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL && !_player.GetPistolMeelee())
					|| eq->Get_Item_Type() == ITEM_TOMMY_GUN);
				if (!b)
					return new CPlayerPunchState();
				else
				{
					CGameInstance::Get_Instance()->Play_Sound(L"EmptyFirearm.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
				}
			}
			else
			{
				_uint idx = NOTI_NOT_ENOUGH_STAMINA;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
				if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
					CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, _player.GetSoundVolume());
			}
		}

		if (m_pGameInstance->Key_Down('Q') && !_player.GetDeliaHold())
		{
			if (_player.GetPlayerDesc()->fStamina > 10.f)
			{
				return new CPlayerKickState();
			}
			else
			{
				_uint idx = NOTI_NOT_ENOUGH_STAMINA;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
				if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
					CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, _player.GetSoundVolume());
			}
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !_player.GetCrouch())
		{
			if (_player.GetPlayerDesc()->fStamina > 10.f)
			{
				return new CPlayerJumpState(false);
			}
			else
			{
				_uint idx = NOTI_NOT_ENOUGH_STAMINA;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
				if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
					CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, _player.GetSoundVolume());
			}
		}

		if (GetAsyncKeyState('W') & 0x8000 && !_player.GetCrouch() && !_player.GetDeliaHold())
		{
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				if (_player.GetPlayerDesc()->fStamina > 10.f)
				{
					if (_player.GetHoldItem() != nullptr && _player.GetHoldItem()->Get_Item_Type() > 100)
						return nullptr;
					_player.SetPlayerTex(RUN);
					_player.SetTime(0.f);
					return new CPlayerRunState(0.f);
				}
				else
				{
					_uint idx = NOTI_NOT_ENOUGH_STAMINA;
					CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
					if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
						CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, _player.GetSoundVolume());
				}
			}
		}
	

	if (m_pGameInstance->Key_Down('C'))
	{
		return new CPlayerCrouchState();
	}

	if (m_pGameInstance->Key_Down(VK_RBUTTON))
	{
		if (_player.GetHoldItem() == nullptr || _player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE || _player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
			return new CPlayerGuardState();
	}

	if (GetAsyncKeyState('S') & 0x8000 || 
		GetAsyncKeyState('A') & 0x8000 || 
		GetAsyncKeyState('D') & 0x8000)
	{
        return nullptr;
	}
    return new CPlayerIdleState(m_bAnimToIdleEnd);
}

void CPlayerWalkState::Entry(CPlayer& _player)
{
    __super::Entry(_player);
	_player.SetSpeed(_player.GetSpeedInfo(SPEED_WALK));
	//m_pGameInstance->StopSound(SOUND_PLAYER_WALK);
}

void CPlayerWalkState::Update(CPlayer& _player, float fTimeDelta)
{
	_player.Move(fTimeDelta);
    
	if (!m_bAnimToIdleEnd&&_player.AnimToIdle(fTimeDelta))
		m_bAnimToIdleEnd = true;

	if(m_bAnimToIdleEnd)
		_player.AnimWalk(fTimeDelta);

    m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	m_pGameInstance->Play_Sound(L"FootstepConcrete1.wav", SOUND_PLAYER_WALK, _player.GetSoundVolume());
}

void CPlayerWalkState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
}
