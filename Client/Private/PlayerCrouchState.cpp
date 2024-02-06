#include "stdafx.h"
#include "PlayerCrouchState.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Physics.h"

CPlayerState* CPlayerCrouchState::Transition(CPlayer& _player)
{
    if (m_fTime > 0.3f)
        return new CPlayerIdleState(true);
    return nullptr;
}

void CPlayerCrouchState::Entry(CPlayer& _player)
{
    __super::Entry(_player);
    _player.SetCrouch(!_player.GetCrouch());
    _player.SetSpeed(_player.GetSpeedInfo(SPEED_WALK));
}

void CPlayerCrouchState::Update(CPlayer& _player, float fTimeDelta)
{
    m_fTime += fTimeDelta;
    fMoveY += fTimeDelta * 0.2f;
    _float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
    if(_player.GetCrouch())
    {
        _player.GetCamera()->AddCameraPos({ 0.f,-fMoveY,0.f });
    }
    else
    {
        _player.GetCamera()->AddCameraPos({ 0.f,fMoveY,0.f });
    }

    _player.Move(fTimeDelta);
     m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);
}

void CPlayerCrouchState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
    _player.SetSpeed(0.f);
}
