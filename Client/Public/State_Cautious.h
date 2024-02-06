#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Cautious_Enemy : public CState_Enemy
{
private:
	CState_Cautious_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Cautious_Enemy(CState_Cautious_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Cautious_Enemy() = default;

public:
	static CState_Cautious_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);

	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	_float m_fCautiousMaxTime = 10.f; // 이 이상으로 아무 일 없으면 Idle로 돌아감
	_float m_fCautiousAccTime = 0.f;
	_float m_fStayTurnMaxTime = 5.f; // 5초

	_float m_fRemainCurDirMaxTime = 1.f; // 이 시간 넘으면 방향 바꾸기
	_float m_fRemainCurDirAccTime = 0.f;

	_float m_fTurnAngle = 0.f;
};

class CState_Cautious_Boss : public CState_Enemy
{
private:
	CState_Cautious_Boss(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Cautious_Boss(CState_Cautious_Boss& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Cautious_Boss() = default;

public:
	static CState_Cautious_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);

	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
};

END