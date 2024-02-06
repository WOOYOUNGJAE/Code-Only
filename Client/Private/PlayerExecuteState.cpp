#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerExecuteState.h"
#include "Player.h"
#include "Transform.h"
#include "Camera_Player.h"

CPlayerState* CPlayerExecuteState::Transition(CPlayer& _player)
{
    if (m_fTime > 1.5f) // 처형 실패했으면
    {
        _player.SetPlayerTex(IDLE);
        _player.SetAnimPosIdle();
        return new CPlayerIdleState(false);
    }
    return nullptr;
}

void CPlayerExecuteState::Entry(CPlayer& _player)
{
    __super::Entry(_player);

    if (_player.GetExecuteType() == ENEMY_GOON)
        eEnemyTex = GRAB1;
    else  if (_player.GetExecuteType() == ENEMY_PIPEMAN)
        eEnemyTex = GRAB11;

    if (_player.GetDeliaHold())
    {
        while (_player.GetExecuteIndex() == 1)
            _player.AddExecuteIndex();
    }

    if (_player.GetChop())
    {
        _player.SetPlayerTex2(CHOP1);
    }
    else
    {
        switch (_player.GetExecuteIndex())
        {
        case 1:
            _player.SetPlayerTex(eEnemyTex);
            break;
        case 2:
            _player.SetPlayerTex(HEAVYREADY);
            _player.SetPlayerTex2(SPINKICK1);
            break;
        case 3:
            _player.SetPlayerTex(HEAVYREADY);
            _player.SetPlayerTex2(UPPERCUT);
        }
    }
   
    _float fUse = -30.f;
    _player.ChangeStamina(&fUse);
    _player.GetPlayerDesc()->bStaminaChanged = true;

}

void CPlayerExecuteState::Update(CPlayer& _player, float fTimeDelta)
{
    m_fTime += fTimeDelta;
    CPlayer::EXECUTIONDESC tArg{_player.GetExecuteIndex(), _player.Get_ExecutionTarget() };

    if (_player.GetChop())
    {
        if (m_fTime > 0.f && m_fTime < 0.3f)
        {
            _player.AddLeftX(fTimeDelta * -1000.f);
            _player.AddLeftY(fTimeDelta * 1000.f);
            _player.AddRightX(fTimeDelta * -2000.f);
            _player.AddRightY(fTimeDelta * -500.f);
        }
        else  if (m_fTime >= 0.3f && m_fTime < 0.5f)
        {
            _player.SetPlayerOnlyTex2(CHOP2);
            _player.AddRightX(fTimeDelta * -1000.f);
        }
        else if (m_fTime >= 0.5f && m_fTime < 0.7f)
        {
            _player.SetPlayerOnlyTex2(CHOP3);
            _player.AddRightX(fTimeDelta * 6500.f);
        }
        else if (m_fTime >= 0.7f && m_fTime < 1.f)
        {
            _player.SetPlayerOnlyTex2(CHOP4);
            tArg.iExecutionIndex = 4;
            SendEvent_Directly(EVENT_PLAY_EXECUTION, &tArg);
            m_pGameInstance->Play_Sound(L"FinisherStinger2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
        }
        else if (m_fTime >= 1.2f && m_fTime < 1.5f)
        {
            _player.AddRightX(fTimeDelta * 1000.f);
            _player.AddRightY(fTimeDelta * 2500.f);
        }
    }
    else
    {
        switch (_player.GetExecuteIndex())
        {
        case 1:
            if (m_fTime > 0.1f && m_iFrame < 9)
            {
                m_fTime = 0.f;
                m_iFrame++;
                _uint iTex = eEnemyTex + m_iFrame;
                _player.SetPlayerTex(iTex);
                if (m_iFrame == 9)
                {
                    _player.SetPlayerOnlyTex2(HEAVYPUNCH);
                    _player.SetRightPos({ g_iWinSizeX ,  g_iWinSizeY - 220.f });
                }
            }

            if (m_iFrame == 9 && m_fTime < 0.075f && m_fTime > 0.f)
            {
                _player.AddRightX(-fTimeDelta * 5700.f);
            }
            else if (m_iFrame == 9 && m_fTime > 0.1f && m_fTime < 0.2f)
            {
                _player.SetPlayerTex(HEAVYREADY);
                _player.SetPlayerTex2(HEAVYPUNCH);
                _player.SetRightPos({ g_iWinSizeX / 2.f + 600.f ,  g_iWinSizeY - 220.f });
                SendEvent_Directly(EVENT_PLAY_EXECUTION, &tArg);
                m_pGameInstance->Play_Sound(L"FinisherStinger2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
            }
            else if (m_iFrame == 9 && m_fTime > 0.5f)
            {
                _player.AddLeftY(fTimeDelta * 1000.f);
            }
            break;

        case 2:
            if (m_fTime > 0.f + m_fSpinReadyTime && m_fTime < 0.075f + m_fSpinReadyTime)
            {
                _player.AddRightX(fTimeDelta * 3300.f);
                _player.AddRightY(-fTimeDelta * 1500.f);
            }
            else if (m_fTime >= 0.075f + m_fSpinReadyTime && m_fTime < 0.15f + m_fSpinReadyTime)
            {
                _player.SetPlayerOnlyTex2(SPINKICK2);
                _player.AddRightX(fTimeDelta * 3300.f);
                _player.AddRightY(-fTimeDelta * 1200.f);
            }
            else if (m_fTime >= 0.15f + m_fSpinReadyTime && m_fTime < 0.225f + m_fSpinReadyTime)
            {
                _player.SetPlayerOnlyTex2(SPINKICK3);
                _player.AddRightX(fTimeDelta * 3200.f);
                _player.AddRightY(-fTimeDelta * 1200.f);
            }
            else if (m_fTime >= 0.225f + m_fSpinReadyTime && m_fTime < 0.3f + m_fSpinReadyTime)
            {
                _player.SetPlayerOnlyTex2(SPINKICK4);
                _player.AddRightX(fTimeDelta * 3200.f);
                _player.AddRightY(-fTimeDelta * 1200.f);
            }
            if (m_fTime < 0.3f)
            {
                _player.AddLeftX(-fTimeDelta * 1000.f);
                _player.AddLeftY(fTimeDelta * 500.f);
            }
            if (m_fTime < 0.3f + m_fSpinReadyTime)
            {
                m_pTransform->Turn({ 0.f,1.f,0.f }, fTimeDelta * 9.f);
            }
            else if (m_fTime >= 0.3f + m_fSpinReadyTime && m_fTime < 1.f)
            {
                _player.AddRightX(-fTimeDelta * 20.f);
                SendEvent_Directly(EVENT_PLAY_EXECUTION, &tArg);
                m_pGameInstance->Play_Sound(L"FinisherStinger2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
            }
            else if (m_fTime >= 1.f && m_fTime < 1.35f)
            {
                _player.AddRightX(fTimeDelta * 20.f);
            }
            else if (m_fTime >= 1.35f && m_fTime < 1.4f)
            {
                _player.SetPlayerOnlyTex2(SPINKICK3);
                _player.AddRightX(-fTimeDelta * 1500.f);
                _player.AddRightY(fTimeDelta * 1000.f);
            }
            else if (m_fTime >= 1.4f && m_fTime < 1.45f)
            {
                _player.SetPlayerOnlyTex2(SPINKICK2);
                _player.AddRightX(-fTimeDelta * 1500.f);
                _player.AddRightY(fTimeDelta * 1000.f);
            }
            else if (m_fTime >= 1.45f && m_fTime < 1.5f)
            {
                _player.SetPlayerOnlyTex2(SPINKICK1);
                _player.AddRightX(-fTimeDelta * 1500.f);
                _player.AddRightY(fTimeDelta * 1000.f);
            }
            break;

        case 3:
        {
            _float3 pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
            if (m_fTime > 0.0f && m_fTime < 0.3f)
            {
                _player.AddLeftX(-fTimeDelta * 500.f);
                _player.AddLeftY(fTimeDelta * 500.f);
                _player.AddRightY(fTimeDelta * 1000.f);
                _float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
                m_pTransform->Turn(-vRight, fTimeDelta * 0.5f);
                m_pTransform->Set_State(CTransform::STATE_POSITION, pos + _float3(0.f, -fTimeDelta * 3.f, 0.f));
            }
            else if (m_fTime > 0.3f && m_fTime < 0.4f)
            {
                _player.AddRightY(-fTimeDelta * 10000.f);
                m_pTransform->Set_State(CTransform::STATE_POSITION, pos + _float3(0.f, fTimeDelta * 9.f, 0.f));
            }
            else if (m_fTime > 0.4f && m_fTime < 0.45f)
            {
                _player.AddRightY(fTimeDelta * 1000.f);
                SendEvent_Directly(EVENT_PLAY_EXECUTION, &tArg);
                m_pGameInstance->Play_Sound(L"FinisherStinger2.wav", SOUND_PLAYER_ATTACK, _player.GetSoundVolume());
            }
            else if (m_fTime > 0.45f && m_fTime < 0.5f)
            {
                _player.AddRightY(-fTimeDelta * 1000.f);
            }
            break;
        }
        }
    }

}

void CPlayerExecuteState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
    _player.AddExecuteIndex();
    _player.Set_ExecutionTarget(nullptr);
}
