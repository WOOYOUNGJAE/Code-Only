#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerSwapState.h"
#include "PlayerHoldItemState.h"
#include "InteractiveObject.h"
#include "Player.h"

CPlayerState* CPlayerSwapState::Transition(CPlayer& _player)
{
    if (m_fTime > 0.3f)
    {
		return new CPlayerIdleState(false);
    }
    return nullptr;
}

void CPlayerSwapState::Entry(CPlayer& _player)
{
     __super::Entry(_player);
}

void CPlayerSwapState::Update(CPlayer& _player, float fTimeDelta)
{
	m_fTime += fTimeDelta;
    if (!_player.GetDeliaHold())
    {
        _player.AddLeftX(-fTimeDelta * 2000.f);
        _player.AddLeftY(fTimeDelta * 2000.f);
    }
        _player.AddRightX(fTimeDelta * 2000.f);
        _player.AddRightY(fTimeDelta * 2000.f);

}

void CPlayerSwapState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
    _player.SetPlayerOnlyTex(IDLE);
    _player.SetAnimPosIdle();
}
