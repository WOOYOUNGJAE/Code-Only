#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
	/**
	 * \brief 공격받는 모션은 1프레임이기 때문에 임의로 지속시간을 지정해둬야함
	 */
	class CState_Attacked_Enemy : public CState_Enemy
{
private:
	CState_Attacked_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Attacked_Enemy(CState_Attacked_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Attacked_Enemy() = default;

public: //Defines
public:
	static CState_Attacked_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);

	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	_float m_fAnimAccTime = 0.f;
	_float m_fAnimMaxTime = 1.f;
	_float m_fRedTime = 0.f;
};

class CState_Attacked_Boss : public CState_Enemy
{
private:
	CState_Attacked_Boss(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Attacked_Boss(CState_Attacked_Boss& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Attacked_Boss() = default;

public: //Defines
public:
	static CState_Attacked_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);

	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	class CBoss* m_pOwnerBoss = nullptr;
	_float m_fAnimAccTime = 0.f;
	_float m_fAnimMaxTime = 1.f;
	_float m_fRedTime = 0.f;
};

END