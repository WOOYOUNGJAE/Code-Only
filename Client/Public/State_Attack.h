#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)

class CState_Attack_Enemy : public CState_Enemy
{
protected:
	CState_Attack_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Attack_Enemy(CState_Attack_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Attack_Enemy() = default;

public:
	static CState_Attack_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

public: //Defines

protected:
	_bool m_bHeavyAttackWindUpFinished = false;
	_float m_fWindUpAccTime = 0.f;
	_float m_fWindUpMaxTime = 0.35f;
};

class CState_Attack_PipeGuy : public CState_Attack_Enemy
{
protected:
	CState_Attack_PipeGuy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Attack_Enemy(pGraphic_Device) {}
	virtual ~CState_Attack_PipeGuy() = default;

public:
	static CState_Attack_PipeGuy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;
};
END