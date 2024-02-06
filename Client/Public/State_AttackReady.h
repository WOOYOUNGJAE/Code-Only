#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)

class CState_AttackReady_Enemy : public CState_Enemy
{
protected:
	CState_AttackReady_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_AttackReady_Enemy(CState_AttackReady_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_AttackReady_Enemy() = default;

public:
	static CState_AttackReady_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

protected:
	_float m_fChoosingTimeMax = 0.5f;
	_float m_fChoosingAccTime = 0.f;
};

class CState_AttackReady_PipeGuy : public CState_AttackReady_Enemy
{
protected:
	CState_AttackReady_PipeGuy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_AttackReady_Enemy(pGraphic_Device) {}
	virtual ~CState_AttackReady_PipeGuy() = default;

public:
	static CState_AttackReady_PipeGuy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

};
END