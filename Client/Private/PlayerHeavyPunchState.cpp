#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerHeavyPunchState.h"
#include "Player.h"
#include "Transform.h"
#include "Physics.h"
#include "InteractiveObject.h"

CPlayerState* CPlayerHeavyPunchState::Transition(CPlayer& _player)
{
    if (!m_bCharge && m_fTime > 0.5f)
    {
        _player.SetPlayerTex(IDLE);
        return new CPlayerIdleState(true);
    }
    return nullptr;
}

void CPlayerHeavyPunchState::Entry(CPlayer& _player)
{
    __super::Entry(_player);
    _player.SetPlayerTex(HEAVYREADY);
    if (_player.GetHoldItem() == nullptr)
    {
        _player.SetPlayerTex2(IDLE);
    }
    else
    {
        if (_player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE)
        {
            _player.SetPlayerTex2(PIPEIDLE);
        }
    }
  
    _player.SetSpeed(_player.GetSpeedInfo(SPEED_WALK));
    m_pGameInstance->StopSound(SOUND_PLAYER_SWING);
    m_pGameInstance->Play_Sound(L"HeavyAttackWindup.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
    m_pGameInstance->StopSound(SOUND_PLAYER_ATTACK);

}

void CPlayerHeavyPunchState::Update(CPlayer& _player, float fTimeDelta)
{
    m_fTime += fTimeDelta;
    //차지했다 풀 떄 
    if (!GetAsyncKeyState(VK_LBUTTON) && 0x8000 && m_fTime > 0.3f && m_bCharge)
    {
        m_bCharge = false;
        m_fTime = 0.f;
        if (_player.GetHoldItem() == nullptr)
        {
            _player.SetPlayerTex2(HEAVYPUNCH);
        }
        else
        {
            if (_player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE)
            {
                _player.SetPlayerOnlyTex2(PIPEHEAVYATK5);
            }
        }
        _float fUse = -30.f;
        _player.ChangeStamina(&fUse);
        _player.GetPlayerDesc()->bStaminaChanged = true;
    }

    if (!m_bCharge && m_fForce > 0.f)
    {
        if (m_fForce >= 0.8f)
        {
            m_fForce -= fTimeDelta * 2.f;
        }
        else if (m_fForce < 0.8f)
        {
            m_fForce -= fTimeDelta * 5.f;
            PLAYERATTACKDESC at;
            at.bHeavy = true;
            at.bKick = false;
            at.pPlayerTransform = m_pTransform;
            at.vPickPos = _player.GetPickItemCollidedPos();
            at.iDmg = 50;
            at.eType = ITEM_NONE;
            SendEvent(_player,EVENT_PLAYER_ATTACK,&at);
            m_pGameInstance->Play_Sound(L"HeavySwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
        }
        else if (m_fForce < 0.2f)
        {
            m_fForce -= fTimeDelta;
        }
		m_pTransform->Go_Straight_FixedY(fTimeDelta * m_fForce);
    }
    _player.Move(fTimeDelta);
    m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);

    _player.ResetCamera(fTimeDelta);

    if(_player.GetHoldItem()==nullptr)
    {
        _player.AnimHeavyPunch(m_bCharge, m_fTime, fTimeDelta);
    }
    else if (_player.GetHoldItem()->Get_Item_Type() == ITEM_PIPE)
    {
        if (m_bCharge)
        {
            if (m_fTime > 0.f && m_fTime < 0.2f)
            {
                _player.AddLeftX(-fTimeDelta * 1000.f);
                _player.AddLeftY(fTimeDelta * 1500.f);
                _player.AddRightX(-fTimeDelta * 5000.f);
                _player.AddRightY(-fTimeDelta * 1000.f);
            }
            else if (m_fTime >= 0.2f && m_fTime < 1.f)
            {
                if(!m_bPipeHeavyAtk1)
                {
                    m_bPipeHeavyAtk1 = true;
                    _player.SetPlayerOnlyTex2(PIPEHEAVYATK3);
                    _player.AddRightY(400.f);
                }
                _player.AddRightAngle(fTimeDelta * 0.2f);
                _player.AddRightX(-fTimeDelta * 200.f);
            }
        }
        else
        {
            if (m_fTime > 0.f && m_fTime < 0.1f)
            {
                _player.AddRightX(fTimeDelta * 10000.f);
                _player.AddRightY(fTimeDelta * 1000.f);
            }
            else if (m_fTime >= 0.1f && m_fTime < 0.15f)
            {
                PLAYERATTACKDESC at;
                at.bHeavy = true;
                at.bKick = false;
                at.pPlayerTransform = m_pTransform;
                at.vPickPos = _player.GetPickItemCollidedPos();
                at.iDmg = 75;
                at.eType = ITEM_PIPE;
                SendEvent(_player, EVENT_PLAYER_ATTACK,&at);
                m_pGameInstance->Play_Sound(L"HeavySwing1.wav", SOUND_PLAYER_SWING, _player.GetSoundVolume());
                _player.AddRightX(fTimeDelta * -500.f);
            }
            else if (m_fTime >= 0.15f && m_fTime < 0.2f)
            {
                _player.AddRightX(fTimeDelta * 500.f);
            }
        }


    }

}

void CPlayerHeavyPunchState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
    _player.SetSpeed(0.f);
}
