#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "PlayerPunchState.h"
#include "PlayerGuardState.h"
#include "PlayerKickState.h"
#include "PlayerExecuteState.h"
#include "PlayerSwapState.h"
#include "PlayerCrouchState.h"
#include "PlayerSwapState.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Physics.h"
#include "Transform.h"
#include "InteractiveObject.h"
#include "Inventory.h"
#include "Item_Equipment.h"


CPlayerIdleState::CPlayerIdleState(_bool _b)
{
	m_bAnimToIdleEnd = _b;
}

CPlayerState* CPlayerIdleState::Transition(CPlayer& _player)
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

	if (CGameInstance::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		if (_player.GetHoldItem() == nullptr || _player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE || _player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
			return new CPlayerGuardState();
	}

	if (CGameInstance::Get_Instance()->Key_Down('Q') && !_player.GetDeliaHold())
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

	if (GetAsyncKeyState('E') & 0x8000)
	{
		if (_player.GetPlayerDesc()->fStamina > 30.f)
		{
			SendEvent_Directly(EVENT_EXECUTION_TRIGGER); // m_pExecutionTarget ¼³Á¤^
			if (_player.Get_ExecutionTarget())
			{
				return new CPlayerExecuteState();
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

	if (GetAsyncKeyState('C') & 0x8000)
	{
		return new CPlayerCrouchState();
	}

	if (CGameInstance::Get_Instance()->Key_Down('E') && _player.GetPickItem() != nullptr)
	{

		if (_player.GetPickItem()->Get_Item_Type() > 200 && _player.GetPickItem()->Get_Item_Type() <= 300)
		{
			PLAYERATTACKDESC at;
			at.pPlayerTransform = m_pTransform;
			_player.GetPickItem()->ObjectHit(&at);
			if(_player.GetPickItem()->Get_Item_Type()==ITEM_ENTRANCE_KEY || 
				_player.GetPickItem()->Get_Item_Type() == ITEM_BASEMENT_KEY ||
				_player.GetPickItem()->Get_Item_Type() == ITEM_MALONES_OFFIC_KEY ||
				_player.GetPickItem()->Get_Item_Type() == ITEM_UPSTAIRS_KEY)
				CGameInstance::Get_Instance()->Play_Sound(L"PickupKey1.wav", SOUND_ITEM, _player.GetSoundVolume());
			return nullptr;
		}
		else if (_player.GetPickItem() != nullptr && _player.GetInventory()->GetInvenSize() <3)
		{
			CGameInstance::Get_Instance()->OnEvent(EVENT_ADD_ITEM_IN_INVENTORY, _player.GetPickItem());

			WEAPONUIDESC we;
			if (_player.GetHoldItem() != nullptr)
			{
				switch (_player.GetHoldItem()
					->Get_Item_Type())
				{
				case ITEM_BOTTLE:
					we.uiUITag = UI_WEAPON_BOTTLE;
					we.uiBulletCount = 0;
					break;
				case ITEM_PIPE:
					we.uiUITag = UI_WEAPON_LEADPIPE;
					we.uiBulletCount = 0;
					break;
				case ITEM_BOTTLE_BROKEN:
					we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
					we.uiBulletCount = 0;
					break;
				case ITEM_TOMMY_GUN:
					we.uiUITag = UI_WEAPON_TOMMYGUN;
					we.uiBulletCount = 40;
					break;
				case ITEM_AUTOMATIC_PISTOL:
					we.uiUITag = UI_WEAPON_AUTOPISTOL;
					we.uiBulletCount = 12;
					break;
				}
				if(_player.GetHoldItem()->Get_Item_Type()<=100)
				{
					we.fWeaponHP = 1.f;
					we.bIsRender = false;
					CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
				}
			}
			_player.SetHoldItem(_player.GetPickItem());

			switch (_player.GetHoldItem()->Get_Item_Type())
			{
			case ITEM_PIPE:
			{
				CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
				CGameInstance::Get_Instance()->Play_Sound(L"PickupPipe.wav", SOUND_ITEM, _player.GetSoundVolume());
				we.uiUITag = UI_WEAPON_LEADPIPE;
				we.fWeaponHP = 1.f;
				we.uiBulletCount = 0;
				_uint idx = NOTI_PICK_LEADPIPE;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			}
				break;
			case ITEM_BOTTLE:
			{
				CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
				CGameInstance::Get_Instance()->Play_Sound(L"BottlePickup.wav", SOUND_ITEM, _player.GetSoundVolume());
				we.uiUITag = UI_WEAPON_BOTTLE;
				we.fWeaponHP = 1.f;
				we.uiBulletCount = 0;
				_uint idx = NOTI_PICK_BOTTLE;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			}
				break;
			case ITEM_TOMMY_GUN:
			{
				CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
				CGameInstance::Get_Instance()->Play_Sound(L"Nice4.wav", SOUND_ITEM, _player.GetSoundVolume());
				we.uiUITag = UI_WEAPON_TOMMYGUN;
				we.fWeaponHP = 1.f;
				we.uiBulletCount = 40;
				_uint idx = NOTI_PICK_TOMMYGUN;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			}
				break;
			case ITEM_AUTOMATIC_PISTOL:
				CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
				{
					CGameInstance::Get_Instance()->Play_Sound(L"Nice4.wav", SOUND_ITEM, _player.GetSoundVolume());
					we.uiUITag = UI_WEAPON_AUTOPISTOL;
					we.fWeaponHP = 1.f;
					we.uiBulletCount = 12;
					_uint idx = NOTI_PICK_AUTOPISTOL;
					CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
				}
				break;
			}
			if (_player.GetHoldItem()->Get_Item_Type() <= 100)
			{
				we.bIsRender = true;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
			}
			return new CPlayerSwapState();
		}


		if (_player.GetInventory()->GetInvenSize() == 3)
		{
			_uint idx = NOTI_INVENTORY_FULL;
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			CGameInstance::Get_Instance()->StopSound(SOUND_NOTI_ERROR);
			CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, _player.GetSoundVolume());
		}
	}

	if (CGameInstance::Get_Instance()->Key_Down('F'))
	{
		if (_player.GetHoldItem() != nullptr)
		{
			CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
			WEAPONUIDESC we;
			if (_player.GetHoldItem() != nullptr)
			{
				switch (_player.GetHoldItem()
					->Get_Item_Type())
				{
				case ITEM_BOTTLE:
					we.uiUITag = UI_WEAPON_BOTTLE;
					we.uiBulletCount = 0;
					break;
				case ITEM_PIPE:
					we.uiUITag = UI_WEAPON_LEADPIPE;
					we.uiBulletCount = 0;
					break;
				case ITEM_BOTTLE_BROKEN:
					we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
					we.uiBulletCount = 0;
					break;
				case ITEM_TOMMY_GUN:
					we.uiUITag = UI_WEAPON_TOMMYGUN;
					we.uiBulletCount = 40;
					break;
				case ITEM_AUTOMATIC_PISTOL:
					we.uiUITag = UI_WEAPON_AUTOPISTOL;
					we.uiBulletCount = 12;
					break;
				}
				if (_player.GetHoldItem()->Get_Item_Type() <= 100)
				{
					we.fWeaponHP = 1.f;
					we.bIsRender = false;
					CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
					_player.SetHoldItem(nullptr);
				}
			}
			return new CPlayerSwapState();
		}
	}

	if (GetAsyncKeyState('W') & 0x8000 ||
		GetAsyncKeyState('S') & 0x8000 || 
		GetAsyncKeyState('A') & 0x8000 || 
		GetAsyncKeyState('D') & 0x8000 )
	{
		CPlayerWalkState* pState = new CPlayerWalkState(m_bAnimToIdleEnd);
		return pState;
	}
	return nullptr;
}

void CPlayerIdleState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
}

void CPlayerIdleState::Update(CPlayer& _player, float fTimeDelta)
{
	if (!_player.GetCamera())
		return;

	if (!m_bAnimToIdleEnd && _player.AnimToIdle(fTimeDelta))
		m_bAnimToIdleEnd = true;

	if (m_bAnimToIdleEnd)
		_player.AnimIdle(fTimeDelta);

	if(_player.GetSpeed()>0.f)
		_player.SetSpeed(_player.GetSpeed() - fTimeDelta*2.f);
	if (_player.GetSpeed() < 0.f)
		_player.SetSpeed(0.f);

	m_pTransform->Go_Dir(fTimeDelta*_player.GetSpeed());

	if (!_player.GetJump())
		m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);
	else
		m_pPhysics->JumpPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	_player.ResetCamera(fTimeDelta);
}

void CPlayerIdleState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
}
