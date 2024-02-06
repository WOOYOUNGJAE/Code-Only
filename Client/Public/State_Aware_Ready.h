#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Aware_Ready : public CState_Enemy
{
private:
	CState_Aware_Ready(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Aware_Ready(CState_Aware_Ready& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Aware_Ready() = default;

public: //Defines
public:
	static CState_Aware_Ready* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);

	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	_int m_iAnimMaxCycle = 2;
};

END