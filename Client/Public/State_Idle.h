#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Idle : public CState
{
private:
	CState_Idle(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
	CState_Idle(CState_Idle& rhs) : CState(rhs.m_pGraphic_Device) {}
	virtual ~CState_Idle() = default;

public:
	static CState_Idle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
	virtual void Free() override { CState::Free(); }

public:
	virtual void* BeginState(_float fTimeDelta, void* pArg) { return S_OK; } // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg) { return S_OK; } // 현 상태에서 매 프레임 호출
	virtual void* CompleteState(_float fTimeDelta, void* pArg) { return S_OK; }// 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg) { return S_OK; }
};

class CState_Idle_Enemy : public CState_Enemy
{
private:
	CState_Idle_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Idle_Enemy(CState_Idle_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Idle_Enemy() = default;

public: //Defines
	enum WALK_STATE
	{
		
	};
	class CState_Idle_Enemy_Walk : public CState
	{
	private:
		CState_Idle_Enemy_Walk(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
		CState_Idle_Enemy_Walk(CState_Idle_Enemy_Walk& rhs) : CState(rhs.m_pGraphic_Device) {}
		virtual ~CState_Idle_Enemy_Walk() = default;

	public:
		static CState_Idle_Enemy_Walk* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
		virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
		virtual void Free() override { CState::Free(); }

	public:
		virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
		virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
		virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
		virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	};
public:
	static CState_Idle_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	/**
	 * \brief Dynamic 일 경우 InnerFSM으로 돌아다니기 추가, GameObj인 Owner을 EnemyBase로 구체화
	 */
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

protected:
	_bool m_bIsDynamic = false;
};

class CState_Idle_Boss : public CState_Enemy
{
private:
	CState_Idle_Boss(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Idle_Boss(CState_Idle_Boss& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Idle_Boss() = default;

public: //Defines
	enum WALK_STATE
	{
		
	};
public:
	static CState_Idle_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	/**
	 * \brief Dynamic 일 경우 InnerFSM으로 돌아다니기 추가, GameObj인 Owner을 EnemyBase로 구체화
	 */
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
};
END
