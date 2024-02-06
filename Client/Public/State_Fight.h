#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)
/**
 * \brief Battle안의 Fight 스테이트
 * 무빙
 * 일반펀치
 * 달려오면서 일반펀치
 * 강펀치
 */
class CState_Fight : public CState
{
private:
	CState_Fight(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
	CState_Fight(CState_Fight& rhs) : CState(rhs.m_pGraphic_Device) {}
	virtual ~CState_Fight() = default;
private:
	enum FIGHT_STATE
	{
		MOVING,
		READY_TO_ATTACK,
		RUSH,
		ATTACK,
		FIGHT_STATE_END,
	};
public:
	static CState_Fight* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

private:
	FIGHT_STATE m_eFight_State = FIGHT_STATE_END;
};

END