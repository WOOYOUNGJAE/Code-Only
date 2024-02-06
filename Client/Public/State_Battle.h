#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)

class CState_Battle_Enemy : public CState_Enemy
{
protected:
	CState_Battle_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Battle_Enemy(CState_Battle_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Battle_Enemy() = default;

public: //Defines
	enum BATTLE_STATE
	{
		ATTACK_READY, // �Ÿ� ����ϰ� Rush���� Attack���� �������� backMoving ����
		MOVING, // ATTACK_READY���� �� ������
		RUSH, // �÷��̾� ���ؼ� AttackDistance �ɶ����� �޸���
		ATTACK, // �ָ���
		BATTLE_STATE_END,
	};
	enum ATTACK_TYPE
	{
		BASIC_ATTACK,//0
		RUSH_ATTACK, //1
		HEAVY_ATTACK, //2
		ATTACK_TYPE_END,
	};
public:
	static CState_Battle_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

protected:
	_float m_fDist_AttackReady = 1.4f;
	_bool m_bRushAttack = false; // remove
	_uint m_iArg = 0;
};

class CState_Battle_PipeGuy : public CState_Battle_Enemy
{
private:
	CState_Battle_PipeGuy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Battle_Enemy(pGraphic_Device) {}
	CState_Battle_PipeGuy(CState_Battle_PipeGuy& rhs) : CState_Battle_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Battle_PipeGuy() = default;

public: //define
	enum BATTLE_STATE_PIPEGUY
	{
		ATTACK_READY, // �Ÿ� ����ϰ� Rush���� Attack���� �������� backMoving ����
		ATTACK, // �ָ���
		BATTLE_STATE_PIPEGUY_END,
	};
public:
	static CState_Battle_PipeGuy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

	_float m_fRestAfterAttackAccTime = 0.f;
	_float m_fRestAfterAttackMaxTime = 0.5f;
};
END