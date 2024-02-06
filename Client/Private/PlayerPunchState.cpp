#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerPunchState.h"
#include "PlayerHeavyPunchState.h"
#include "PlayerGuardState.h"
#include "Player.h"
#include "Physics.h"
#include "InteractiveObject.h"
#include "Item_Consumable.h"
#include "Item_Equipment.h"
#include "Inventory.h"

CPlayerState* CPlayerPunchState::Transition(CPlayer& _player)
{
	if (m_bGuardable && m_pGameInstance->Key_Pressing(VK_RBUTTON))
	{
		return new CPlayerGuardState;
	}
	if (_player.GetHoldItem()!=nullptr && m_ePunchState == PUNCH_FORWARD && 
		(_player.GetHoldItem()->Get_Item_Type() == ITEM_TOMMY_GUN ||_player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL))
	{
		if (_player.GetHoldItem()->Get_Item_Type() == ITEM_TOMMY_GUN)
			_player.SetPlayerTex(FIREIDLE);
		else if (_player.GetHoldItem()->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
			_player.SetPlayerTex(PISTOLIDLE);
		m_bHeavyPunch = false;
		m_fHeavyPunchTime = 0.5f;
		return nullptr;
	}

	if ((!(GetAsyncKeyState(VK_LBUTTON) && 0x8000) && m_fHeavyPunchTime < 0.5f))
	{
		m_bHeavyPunch = false;
		m_fHeavyPunchTime = 0.5f;
		return nullptr;
	}
	else if (m_bHeavyPunch && m_fHeavyPunchTime >= 0.5f && _player.GetPlayerDesc()->fStamina > 30.f)
	{
		if (_player.GetHoldItem() == nullptr || _player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE)
			return new CPlayerHeavyPunchState();
	}
		
	if (m_ePunchState==PUNCH_FINISH)
	{
		if(_player.GetHoldItem()==nullptr || _player.GetHoldItem()->Get_Item_Type() <=100)
		{
			_player.SetPlayerTex(IDLE);
			return new CPlayerIdleState(true);
		}
		else if (_player.GetHoldItem()->Get_Item_Type() > 100 && _player.GetHoldItem()->Get_Item_Type() <= 300)
		{
			_player.SetHoldItem(nullptr);
			_player.SetPlayerOnlyTex(IDLE);
			_player.SetAnimPosIdle();
			return new CPlayerIdleState(false);
		}
	}
    return nullptr;
}

void CPlayerPunchState::Entry(CPlayer& _player)
{
	__super::Entry(_player);
	_player.SetSpeed(_player.GetSpeedInfo(SPEED_WALK));
	if (_player.GetDeliaHold())
		_player.SetPunchLeft(false);

	if (_player.GetHoldItem() != nullptr)
	{
		if(_player.GetHoldItem()->Get_Item_Type()<=100)
		{
			switch (_player.GetHoldItem()->Get_Item_Type())
			{
			case ITEM_PIPE:
				_player.SetPlayerTex(HEAVYREADY);
				_player.SetPlayerTex2(PIPEIDLE);
				break;
			case ITEM_BOTTLE:
				_player.SetPlayerOnlyTex(HEAVYREADY);
				_player.SetPlayerOnlyTex2(BOTTLEIDLE);
				break;
			case ITEM_BOTTLE_BROKEN:
				_player.SetPlayerOnlyTex(HEAVYREADY);
				_player.SetPlayerOnlyTex2(BOTTLEBROKENATK);
				_player.AddRightY(200.f);
				break;
			case ITEM_TOMMY_GUN:
				_player.SetPlayerOnlyTex(HEAVYREADY);
				_player.SetPlayerOnlyTex2(FIREIDLE);
				break;
			case ITEM_AUTOMATIC_PISTOL:
				if(!_player.GetDeliaHold())
					_player.SetPlayerOnlyTex(HEAVYREADY);
				_player.SetPlayerOnlyTex2(PISTOLIDLE);
				break;
			}
		}
		else if (_player.GetHoldItem()->Get_Item_Type() > 100 && _player.GetHoldItem()->Get_Item_Type() <= 200)
		{
			_player.SetPlayerOnlyTex(IDLE);
			_player.SetAnimPosIdle();
		}
		else if (_player.GetHoldItem()->Get_Item_Type() > 300 && _player.GetHoldItem()->Get_Item_Type() <= 400)
		{
			if(!_player.GetDeliaHold())
				_player.SetPlayerOnlyTex(HEAVYREADY);
			_player.SetPlayerOnlyTex2(THROW);
			_player.SetRightPos({ g_iWinSizeX + 200.f,g_iWinSizeY - 200.f });
		}
	}
	if(_player.GetHoldItem()==nullptr||(_player.GetHoldItem()!=nullptr&&_player.GetHoldItem()->Get_Item_Type()!=ITEM_AUTOMATIC_PISTOL && _player.GetHoldItem()->Get_Item_Type()!=ITEM_TOMMY_GUN))
	{
		_float fUse = -10.f;
		_player.ChangeStamina(&fUse);
		_player.GetPlayerDesc()->bStaminaChanged = true;
	}
	m_pGameInstance->StopSound(SOUND_PLAYER_SWING);
	m_pGameInstance->StopSound(SOUND_PLAYER_ATTACK);
}

void CPlayerPunchState::Update(CPlayer& _player, float fTimeDelta)
{
	m_fTime += fTimeDelta;
	m_fHeavyPunchTime += fTimeDelta;
	_player.Move(fTimeDelta);
	m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

	_player.ResetCamera(fTimeDelta);

	if (m_fHeavyPunchTime < 0.5f)
		return;

	if (_player.GetHoldItem() == nullptr)
	{
		if (m_bInitPos)
		{
			if (_player.GetPunchLeft())
			{
				_player.SetPlayerTex(PUNCH);
				_player.SetPlayerTex2(IDLE);
			}
			else
			{
				_player.SetPlayerTex(IDLE);
				_player.SetPlayerTex2(PUNCH);
			}
			_player.SetTime(0.f);
			m_bInitPos = false;
		}

		switch (m_ePunchState)
		{
		case PUNCH_FORWARD:
		{
			m_fTiltAngle += fTimeDelta;
			if (_player.GetPunchLeft() && m_fHeavyPunchTime > 0.4f && m_fTiltAngle < 0.1f)
				m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta * 0.2f);
			else if (!_player.GetPunchLeft() && m_fHeavyPunchTime > 0.4f && m_fTiltAngle < 0.1f)
				m_pTransform->Turn({ 0.f, -1.f, 0.f }, fTimeDelta * 0.2f);
		}
		break;
		case PUNCH_BACK:
			m_bAttackEnd = true;
			m_fTiltAngle -= fTimeDelta;
			if (_player.GetPunchLeft() && m_fTiltAngle > 0.f)
				m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta * 0.2f);
			else if (!_player.GetPunchLeft() && m_fTiltAngle > 0.f)
				m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta * 0.2f);
			break;
		}

		m_ePunchState = _player.AnimPunch(m_bAttackEnd, fTimeDelta);
		if (m_fHeavyPunchTime > 0.8f)
		{
			PLAYERATTACKDESC at;
			at.bHeavy = false;
			at.bKick = false;
			at.pPlayerTransform = m_pTransform;
			at.vPickPos = _player.GetPickItemCollidedPos();
			at.iDmg = 20;
			at.eType = ITEM_NONE;
			SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
			if(_player.GetPunchLeft())
				m_pGameInstance->Play_Sound(L"Punch1.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
			else
				m_pGameInstance->Play_Sound(L"Punch2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
			m_pGameInstance->Play_Sound(L"MediumSwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
		}
		if (m_fHeavyPunchTime > 0.9f)
			m_bGuardable = true;
	}
	//소비용 아이템 소비
	else if (_player.GetHoldItem()->Get_Item_Type() > 100 && _player.GetHoldItem()->Get_Item_Type() <= 200)
	{
		m_pGameInstance->OnEvent(EVENT_EAT_HEAL);
		CItem_Consumable* pCon = static_cast<CItem_Consumable*>(_player.GetHoldItem());
		_int iHeal = pCon->GetHeal();
		_player.ChangeHp(&iHeal);
		CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
		m_ePunchState = PUNCH_FINISH;
		switch (pCon->Get_Item_Type())
		{
		case ITEM_MEDIKIT:
			m_pGameInstance->Play_Sound(L"DrinkSoda.wav", SOUND_ITEM, pCon->GetSoundVolume());
			break;
		case ITEM_COLA:
			m_pGameInstance->Play_Sound(L"DrinkSoda.wav", SOUND_ITEM, pCon->GetSoundVolume());
			break;
		case ITEM_APPLE:
			m_pGameInstance->Play_Sound(L"Eat1.wav", SOUND_ITEM, pCon->GetSoundVolume());
			break;
		case ITEM_BANANA:
			m_pGameInstance->Play_Sound(L"Eat2.wav", SOUND_ITEM, pCon->GetSoundVolume());
			break;
		}
	}
	//던짐용 아이템 던짐
	else if (_player.GetHoldItem()->Get_Item_Type() > 300 && _player.GetHoldItem()->Get_Item_Type() <= 400)
	{
		if (m_fHeavyPunchTime > 0.5f && m_fHeavyPunchTime <= 0.8f)
		{
			_player.AddRightX(fTimeDelta * -8000.f);
			_player.AddRightY(fTimeDelta * 4000.f);
			_player.AddRightAngle(fTimeDelta);
			if(!m_bThrow)
			{
				m_bThrow = true;
				_player.GetHoldItem()->Ready_ForThrow(m_pTransform);
			}
		}
		if (m_fHeavyPunchTime > 0.8f)
		{
			_player.SetHoldItem(static_cast<CInteractiveObject*>(_player.GetInventory()->Get_CurSelectedItem()));
			m_ePunchState = PUNCH_FINISH;
		}
	}
	//장비용 아이템
	else
	{
		switch (_player.GetHoldItem()->Get_Item_Type())
		{
		case ITEM_PIPE:
			if (m_fHeavyPunchTime > 0.5f && m_fHeavyPunchTime < 0.75f)
			{
				_player.AddRightAngle(fTimeDelta);
				if(!_player.GetDeliaHold())
				{
					_player.AddLeftX(-fTimeDelta * 1500.f);
					_player.AddLeftY(fTimeDelta * 3000.f);
				}
				_player.AddRightX(fTimeDelta * 7000.f);
				_player.AddRightY(-fTimeDelta * 4000.f);
				if (m_fTiltAngle < 0.1f)
				{
					m_fTiltAngle += fTimeDelta;
					m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.75f && m_fHeavyPunchTime < 0.85f)
			{
				_player.SetPlayerOnlyTex2(PIPEATK2);
				_player.AddRightAngle(fTimeDelta);
				_player.AddRightX(-fTimeDelta * 13000.f);
				_player.AddRightY(fTimeDelta * 8500.f);
				if (m_fTiltAngle > 0.f)
				{
					m_fTiltAngle -= fTimeDelta;
					m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta);
				}
			}

			if (m_fHeavyPunchTime >= 0.85f && m_fHeavyPunchTime < 0.95f)
			{
				if (!m_bTex3)
				{
					_player.SetPlayerOnlyTex2(PIPEATK3);
					m_bTex3 = true;
					_player.AddRightY(100.f);
					PLAYERATTACKDESC at;
					at.bHeavy = false;
					at.bKick = false;
					at.pPlayerTransform = m_pTransform;
					at.vPickPos = _player.GetPickItemCollidedPos();
					at.iDmg = 40;
					at.eType = ITEM_PIPE;
					SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
					m_pGameInstance->Play_Sound(L"Punch1.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
					m_pGameInstance->Play_Sound(L"MediumSwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
				}
				_player.AddRightAngle(-fTimeDelta * 0.5f);
				_player.AddRightX(-fTimeDelta * 500.f);
				_player.AddRightY(-fTimeDelta * 400.f);
			}
			else if (m_fHeavyPunchTime >= 0.95f && m_fHeavyPunchTime < 1.1f)
			{
				_player.AddRightAngle(-fTimeDelta);
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * 1500.f);
					_player.AddLeftY(-fTimeDelta * 1500.f);
				}
				_player.AddRightY(fTimeDelta * 500.f);
				m_bGuardable = true;
			}
			if (m_fHeavyPunchTime > 1.1f)
				m_ePunchState = PUNCH_FINISH;
			break;
		case ITEM_BOTTLE:
			if (m_fHeavyPunchTime >= 0.5f && m_fHeavyPunchTime < 0.6f)
			{
				_player.SetPlayerOnlyTex2(BOTTLEATK1);
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(-fTimeDelta * 1500.f);
					_player.AddLeftY(fTimeDelta * 1000.f);
				}
				_player.AddRightX(fTimeDelta * 2000.f);
				_player.AddRightY(fTimeDelta * -1000.f);
				if (m_fTiltAngle < 0.05f)
				{
					m_fTiltAngle += fTimeDelta;
					m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.6f && m_fHeavyPunchTime < 0.7f)
			{
				_player.SetPlayerOnlyTex2(BOTTLEATK2);
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(-fTimeDelta * 1500.f);
					_player.AddLeftY(fTimeDelta * 1000.f);
				}
				_player.AddRightX(fTimeDelta * 2000.f);
				_player.AddRightY(fTimeDelta * -500.f);
				if (m_fTiltAngle < 0.05f)
				{
					m_fTiltAngle += fTimeDelta;
					m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.7f && m_fHeavyPunchTime < 0.8f)
			{
				_player.SetPlayerOnlyTex2(BOTTLEATK3);
				_player.AddRightX(-fTimeDelta * 7000.f);
				_player.AddRightY(fTimeDelta * 2000.f);
				if (m_fTiltAngle > 0.f)
				{
					m_fTiltAngle -= fTimeDelta;
					m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.8f && m_fHeavyPunchTime < 0.9f)
			{
				if (!m_bBottleAtk4)
				{
					m_bBottleAtk4 = true;
					_player.SetPlayerOnlyTex2(BOTTLEATK4);
					_player.AddRightX(-50.f);
					_player.AddRightY(50.f);
				}
				_player.AddRightX(-fTimeDelta * 1000.f);
				_player.AddRightY(fTimeDelta * 500.f);
				if (m_fTiltAngle > 0.f)
				{
					m_fTiltAngle -= fTimeDelta;
					m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta);
				}
				PLAYERATTACKDESC at;
				at.bHeavy = false;
				at.bKick = false;
				at.pPlayerTransform = m_pTransform;
				at.vPickPos = _player.GetPickItemCollidedPos();
				at.iDmg = 30;
				at.eType = ITEM_BOTTLE;
				SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
				m_pGameInstance->Play_Sound(L"Punch2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
				m_pGameInstance->Play_Sound(L"MediumSwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
			}
			else if (m_fHeavyPunchTime >= 0.9f && m_fHeavyPunchTime < 1.1f)
			{
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * 1500.f);
					_player.AddLeftY(-fTimeDelta * 1000.f);
				}
				_player.AddRightX(fTimeDelta * 1000.f);
				_player.AddRightY(-fTimeDelta * 500.f);
			}
			if (m_fHeavyPunchTime > 1.1f)
				m_ePunchState = PUNCH_FINISH;
			break;
		case ITEM_BOTTLE_BROKEN:
			if (m_fHeavyPunchTime > 0.5f && m_fHeavyPunchTime < 0.6f)
			{
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * -2000.f);
					_player.AddLeftY(fTimeDelta * 2000.f);
				}
				_player.AddRightX(fTimeDelta * -1000.f);
				_player.AddRightY(fTimeDelta * -2000.f);
				if (m_fTiltAngle < 0.05f)
				{
					m_fTiltAngle += fTimeDelta;
					m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.6f && m_fHeavyPunchTime < 0.65f)
			{
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * -2000.f);
					_player.AddLeftY(fTimeDelta * 2000.f);
				}
				_player.AddRightX(-fTimeDelta * 2000.f);
				_player.AddRightY(fTimeDelta * -1000.f);
				if (m_fTiltAngle < 0.05f)
				{
					m_fTiltAngle += fTimeDelta;
					m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta);
				}
				PLAYERATTACKDESC at;
				at.bHeavy = false;
				at.bKick = false;
				at.pPlayerTransform = m_pTransform;
				at.vPickPos = _player.GetPickItemCollidedPos();
				at.iDmg = 30;
				at.eType = ITEM_BOTTLE_BROKEN;
				SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
				m_pGameInstance->Play_Sound(L"MediumSwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
			}
			else if (m_fHeavyPunchTime >= 0.65f && m_fHeavyPunchTime < 0.7f)
			{
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * 3000.f);
					_player.AddLeftY(fTimeDelta * -3000.f);
				}
				_player.AddRightX(fTimeDelta * 4000.f);
				_player.AddRightY(fTimeDelta * 3000.f);
				if (m_fTiltAngle > 0.f)
				{
					m_fTiltAngle -= fTimeDelta;
					m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta);
				}
			}
			else if (m_fHeavyPunchTime >= 0.7f && m_fHeavyPunchTime < 0.75f)
			{
				if (!_player.GetDeliaHold())
				{
					_player.AddLeftX(fTimeDelta * 2000.f);
					_player.AddLeftY(fTimeDelta * -2000.f);
				}
				_player.AddRightX(fTimeDelta * 2000.f);
				_player.AddRightY(fTimeDelta * 4000.f);
				if (m_fTiltAngle > 0.f)
				{
					m_fTiltAngle -= fTimeDelta;
					m_pTransform->Turn({ 0.f,-1.f,0.f }, fTimeDelta);
				}
			}
			if (m_fHeavyPunchTime > 0.8f)
				m_ePunchState = PUNCH_FINISH;
			break;
		case ITEM_TOMMY_GUN:
			m_ePunchState = PUNCH_BACK;
			if (m_fHeavyPunchTime >= 0.5f && m_fHeavyPunchTime < 0.55f)
			{
				_player.SetPlayerOnlyTex2(FIRE1);
				_player.AddRightX(fTimeDelta * 100.f);
				_player.AddRightY(fTimeDelta * 100.f);
			}
			else if (m_fHeavyPunchTime >= 0.55f && m_fHeavyPunchTime < 0.6f)
			{
				_player.SetPlayerOnlyTex2(FIRE2);
				_player.AddRightX(fTimeDelta * 100.f);
				_player.AddRightY(fTimeDelta * 100.f);
				SendEvent(_player,EVENT_PLAYER_GUNFIRE);
				
			}
			else if (m_fHeavyPunchTime >= 0.6f && m_fHeavyPunchTime < 0.65f)
			{
				_player.SetPlayerOnlyTex2(FIRE3);
				_player.AddRightX(fTimeDelta * -200.f);
				_player.AddRightY(fTimeDelta * -200.f);
			}
	
			if (m_fHeavyPunchTime >= 0.65f)
			{
				if (!m_pGameInstance->Key_Pressing(VK_LBUTTON))
				{
					_player.SetPlayerTex(FIREIDLE);
					m_ePunchState = PUNCH_FINISH;
				}
				else
				{
					m_pGameInstance->StopSound(SOUND_PLAYER_ATTACK);
					m_fHeavyPunchTime = 0.5f;
					_player.SetPlayerTex(FIREIDLE);
					m_bEvent = false;
				}
			}
			break;
		case ITEM_AUTOMATIC_PISTOL:
			m_ePunchState = PUNCH_BACK;
			if(!_player.GetPistolMeelee())
			{
				if (m_fHeavyPunchTime >= 0.5f && m_fHeavyPunchTime < 0.55f)
				{
					_player.SetPlayerOnlyTex2(PISTOLFIRE);
					_player.AddRightX(fTimeDelta * 300.f);
					_player.AddRightY(fTimeDelta * 300.f);
					_player.AddRightAngle(-fTimeDelta);
					SendEvent(_player, EVENT_PLAYER_GUNFIRE);
				}
				else if (m_fHeavyPunchTime >= 0.55f && m_fHeavyPunchTime < 0.6f)
				{
					_player.SetPlayerOnlyTex2(PISTOLIDLE);
					_player.AddRightX(fTimeDelta * -300.f);
					_player.AddRightY(fTimeDelta * -300.f);
					_player.AddRightAngle(fTimeDelta);
				}
				else if (m_fHeavyPunchTime >= 0.6f)
				{
					m_ePunchState = PUNCH_FINISH;
				}
			}
			else
			{
				if (m_fHeavyPunchTime >= 0.5f && m_fHeavyPunchTime < 0.6f)
				{
					_player.SetPlayerOnlyTex2(PISTOLMEELEEATK);
					_player.AddRightX(fTimeDelta * 10000.f);
					_player.AddRightY(fTimeDelta * -6000.f);
					_player.SetRightAngle(1.f);
				}
				else if (m_fHeavyPunchTime >= 0.6f && m_fHeavyPunchTime < 0.9f)
				{
					_player.AddRightX(fTimeDelta * -12000.f);
					_player.AddRightY(fTimeDelta * 10000.f);
					PLAYERATTACKDESC at;
					at.bHeavy = false;
					at.bKick = false;
					at.pPlayerTransform = m_pTransform;
					at.iDmg = 30;
					at.eType = ITEM_AUTOMATIC_PISTOL;
					SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
					m_pGameInstance->Play_Sound(L"MediumSwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
				}
				else if (m_fHeavyPunchTime >= 0.9f)
				{
					m_ePunchState = PUNCH_FINISH;
				}
			}
			break;
		}
	}
}

void CPlayerPunchState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.SetPunchLeft(!_player.GetPunchLeft());
	_player.SetSpeed(0.f);
	_player.SetRightAngle(0.f);
}
