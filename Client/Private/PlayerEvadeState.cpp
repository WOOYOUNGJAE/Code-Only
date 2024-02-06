#include "stdafx.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerEvadeState.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Physics.h"
#include "Transform.h"

CPlayerState* CPlayerEvadeState::Transition(CPlayer& _player)
{
	if (m_fTime > 0.4f)
		return new CPlayerIdleState(true);
	return nullptr;
}

void CPlayerEvadeState::Entry(CPlayer& _player)
{
    __super::Entry(_player);
	if (m_pGameInstance->Key_Pressing('S') && m_pGameInstance->Key_Pressing('A'))
	{
		m_eDir = EVADE_SA;
	}
	else if (m_pGameInstance->Key_Pressing('S') && m_pGameInstance->Key_Pressing('D'))
	{
		m_eDir = EVADE_SD;
	}
	else if (m_pGameInstance->Key_Pressing('S'))
	{
		m_eDir = EVADE_S;
	}
	else if (m_pGameInstance->Key_Pressing('A'))
	{
		m_eDir = EVADE_A;
	}
	else if (m_pGameInstance->Key_Pressing('D'))
	{
		m_eDir = EVADE_D;
	}

	_player.SetSpeed(1.f);
	_float fUse = -30.f;
	_player.ChangeStamina(&fUse);
	_player.GetPlayerDesc()->bStaminaChanged = true;
}

void CPlayerEvadeState::Update(CPlayer& _player, float fTimeDelta)
{
    m_fTime += fTimeDelta;
    _float fSpeed = max(_player.GetSpeed() - m_fTime*0.1f,0);
    _player.SetSpeed(fSpeed);
    
	switch (m_eDir)
	{
	case EVADE_A:
		m_pTransform->Go_Left_FixedY(fTimeDelta * _player.GetSpeed());
		break;
	case EVADE_SA:
		m_pTransform->Go_LeftDown_FixedY(fTimeDelta * _player.GetSpeed());
		break;
	case EVADE_S:
		m_pTransform->Go_Backward_FixedY(fTimeDelta * _player.GetSpeed());
		break;
	case EVADE_SD:
		m_pTransform->Go_RightDown_FixedY(fTimeDelta * _player.GetSpeed());
		break;
	case EVADE_D:
		m_pTransform->Go_Right_FixedY(fTimeDelta * _player.GetSpeed());
		break;
	}

	if(m_fTime < 0.3f)
	{
		fMoveY -= fTimeDelta * 3.f;
	}
	else
	{
		fMoveY += fTimeDelta * 6.f;
	}
	_player.GetCamera()->SetCameraPos({ 0.f,fMoveY,0.f });
	m_pPhysics->GroundPhysics(m_pTransform, _player.GetLevel(), _player.GetCollisionTag(), fTimeDelta);
}

void CPlayerEvadeState::Exit(CPlayer& _player)
{
    __super::Exit(_player);
	_player.GetCamera()->SetCameraPos({0.f,0.f,0.f});
}
