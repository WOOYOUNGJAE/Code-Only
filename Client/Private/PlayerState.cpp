#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerState.h"
#include "Player.h"
#include "Transform.h"
#include "Physics.h"
#include "InteractiveObject.h"
#include "Item_Equipment.h"

CPlayerState::CPlayerState()
{

}

CPlayerState::~CPlayerState()
{
}

CPlayerState* CPlayerState::Transition(CPlayer& _player)
{
	return nullptr;
}

void CPlayerState::Entry(CPlayer& _player)
{
	m_pGameInstance = CGameInstance::Get_Instance();
	//Safe_AddRef(m_pGameInstance);
	m_pTransform = static_cast<CTransform*>(_player.Find_Component(L"Com_Transform"));
	//Safe_AddRef(m_pTransform);
	m_pPhysics = static_cast<CPhysics*>(_player.Find_Component(L"Com_Physics"));
	//Safe_AddRef(m_pPhysics);
	m_vInitLPos = _player.GetLeftPos();
	m_vInitRPos = _player.GeRightPos();
}

void CPlayerState::Update(CPlayer& _player, float fTimeDelta)
{
}

void CPlayerState::Exit(CPlayer& _player)
{
	//Safe_Release(m_pGameInstance);
	//Safe_Release(m_pTransform);
	//Safe_Release(m_pPhysics);
}

void CPlayerState::SendEvent(class CPlayer& _player, _uint iEventIndex, void* pArg)
{
	if (!m_bEvent)
	{
		m_bEvent = true;
		m_pGameInstance->OnEvent(iEventIndex, pArg);
		if (iEventIndex == EVENT_PLAYER_ATTACK)
		{
			PLAYERATTACKDESC* at = static_cast<PLAYERATTACKDESC*>(pArg);
			if (_player.GetPickItem() != nullptr)
			{
				_player.GetPickItem()->ObjectHit(at);
			}
		}
		else if (iEventIndex == EVENT_PLAYER_GUNFIRE)
		{
			m_iSoundChannel++;
			switch (m_iSoundChannel % 4)
			{
			case 0:
				m_pGameInstance->StopSound(SOUND_GUN1);
				m_pGameInstance->Play_Sound(L"TommyGunFireAlt.wav", SOUND_GUN1, _player.GetSoundVolume());
				break;
			case 1:
				m_pGameInstance->StopSound(SOUND_GUN2);
				m_pGameInstance->Play_Sound(L"TommyGunFireAlt.wav", SOUND_GUN2, _player.GetSoundVolume());
				break;
			case 2:
				m_pGameInstance->StopSound(SOUND_GUN3);
				m_pGameInstance->Play_Sound(L"TommyGunFireAlt.wav", SOUND_GUN3, _player.GetSoundVolume());
				break;
			case 3:
				m_pGameInstance->StopSound(SOUND_GUN4);
				m_pGameInstance->Play_Sound(L"TommyGunFireAlt.wav", SOUND_GUN4, _player.GetSoundVolume());
				break;
			}

			CItem_Equipment* eq = static_cast<CItem_Equipment*>(_player.GetHoldItem());
			WEAPONUIDESC we;
			if (eq->Get_Item_Type() == ITEM_AUTOMATIC_PISTOL)
				we.uiUITag = UI_WEAPON_AUTOPISTOL;
			else if (eq->Get_Item_Type() == ITEM_TOMMY_GUN)
				we.uiUITag = UI_WEAPON_TOMMYGUN;
			we.fWeaponHP = eq->GetHP();
			we.uiBulletCount = eq->MinusBullet();
			we.bIsRender = true;
			m_pGameInstance->OnEvent(EVENT_SET_WEAPON_UI, &we);
		}
	}
}

void CPlayerState::SendEvent_Directly(_uint iEventIndex, void* pArg)
{
	m_pGameInstance->OnEvent(iEventIndex, pArg);
}
