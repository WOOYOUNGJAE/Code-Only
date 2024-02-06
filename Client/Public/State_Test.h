#pragma once
# include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CEnemy0State_Test : public CState
{
private:
	CEnemy0State_Test(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
	CEnemy0State_Test(CEnemy0State_Test& rhs) : CState(rhs.m_pGraphic_Device) {}
	virtual ~CEnemy0State_Test() = default;

public:
	static CEnemy0State_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
	virtual void Free() override { CState::Free(); }

public:
	virtual void* BeginState(_float fTimeDelta, void* pArg); // 현 상태에서 시작할 때 한번만 호출
	virtual void* Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void* CompleteState(_float fTimeDelta, void* pArg); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual void* LateTick_State(_float fTimeDelta, void* pArg);
};


class CEnemy0State_TestDetailed : public CState
{
private:
	CEnemy0State_TestDetailed(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device, 1) {}
	CEnemy0State_TestDetailed(CEnemy0State_TestDetailed& rhs) : CState(rhs.m_pGraphic_Device, rhs.m_vecInnerFSM.size()) {}
	virtual ~CEnemy0State_TestDetailed() = default;

public:
	enum ENEMY0BATTLE_STATE
	{
		STATE_REALIZED,
		STATE_FOLLOW,
	};
// Inner Class
	class CEnemy0State_TestDetailed_0 : public CState
	{
	private:
		CEnemy0State_TestDetailed_0(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
		CEnemy0State_TestDetailed_0(CEnemy0State_TestDetailed_0& rhs) : CState(rhs.m_pGraphic_Device) {}
		virtual ~CEnemy0State_TestDetailed_0() = default;

	public:
		static CEnemy0State_TestDetailed_0* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
		virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
		virtual void Free() override { CState::Free(); }

	public:
		virtual void* BeginState(_float fTimeDelta, void* pArg); // 현 상태에서 시작할 때 한번만 호출
		virtual void* Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
		virtual void* CompleteState(_float fTimeDelta, void* pArg); // 다른 상태로 바꾸기 직전 한번만 호출
		virtual void* LateTick_State(_float fTimeDelta, void* pArg);
	};
	class CEnemy0State_TestDetailed_1 : public CState
	{
	private:
		CEnemy0State_TestDetailed_1(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
		CEnemy0State_TestDetailed_1(CEnemy0State_TestDetailed_1& rhs) : CState(rhs.m_pGraphic_Device) {}
		virtual ~CEnemy0State_TestDetailed_1() = default;

	public:
		static CEnemy0State_TestDetailed_1* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
		virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
		virtual void Free() override { CState::Free(); }

	public:
		virtual void* BeginState(_float fTimeDelta, void* pArg); // 현 상태에서 시작할 때 한번만 호출
		virtual void* Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
		virtual void* CompleteState(_float fTimeDelta, void* pArg); // 다른 상태로 바꾸기 직전 한번만 호출
		virtual void* LateTick_State(_float fTimeDelta, void* pArg);
	};

public:
	static CEnemy0State_TestDetailed* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;

public:
	virtual void* BeginState(_float fTimeDelta, void* pArg); // 현 상태에서 시작할 때 한번만 호출
	virtual void* Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void* CompleteState(_float fTimeDelta, void* pArg); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual void* LateTick_State(_float fTimeDelta, void* pArg);
};


END