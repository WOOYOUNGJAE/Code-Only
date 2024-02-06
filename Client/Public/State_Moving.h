#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)

class CState_Moving_Enemy : public CState_Enemy
{
private:
	CState_Moving_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Moving_Enemy(CState_Moving_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Moving_Enemy() = default;

public:
	static CState_Moving_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;
	
private:
	_uint iArg = 0x0000; //isKnockBacking attack2 attack1 attack0

	_float m_fDist_AttackReady = 1.0f;
	_float m_fOneStep = 0.4f;
	_float m_fCurrentStep = 0.f;
	_bool m_bStepable = false;
	_float m_fRestAccTime = 0.f;
	_float m_fRestMaxTime = 0.5f;
	_float3 m_vStepStartDir{};

	_float m_fKnockBackMaxDistance = 0.9f;
	_float m_fKnockBackAccDistance = 0.f;
	_float m_fKnockBackSpeed = 3.f;
	//_float3 m_vKnockBackPos;

	_bool m_bIsStrafing = false;
	_bool m_bIsJumpingBack = false;
	_float3 m_vStrafingDir;

	_float m_fStrafingMagnitude = 5.5f;
	_float m_fForwardingMagnitude = 5.f;

	_float m_fMinApproachDistance = 1.4f;
};
END