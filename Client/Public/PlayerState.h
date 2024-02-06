#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
BEGIN(Engine)
class CTransform;
class CPhysics;
class CGameInstance;
END

BEGIN(Client)

class CPlayerState
{
public:
	CPlayerState();
	~CPlayerState();

public:
	virtual CPlayerState* Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player,float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

protected:
	CGameInstance* m_pGameInstance = nullptr;
	CTransform* m_pTransform = nullptr;
	CPhysics* m_pPhysics = nullptr;
	_bool m_bAnimToIdleEnd = true;
	_bool m_bEvent = false;
	_float m_fTime = 0.f;
	_int m_iFrame = 0;
	_float2 m_vInitLPos = { 0,0 };
	_float2 m_vInitRPos = { 0,0 };
	_bool m_iSoundChannel = 0;

public:
	void SendEvent(class CPlayer& ,_uint iEventIndex, void* pArg = nullptr);
	void SendEvent_Directly(_uint iEventIndex, void* pArg = nullptr);
};

END

