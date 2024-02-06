#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
	/**
	 * \brief ���ݹ޴� ����� 1�������̱� ������ ���Ƿ� ���ӽð��� �����ص־���
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
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
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
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	class CBoss* m_pOwnerBoss = nullptr;
	_float m_fAnimAccTime = 0.f;
	_float m_fAnimMaxTime = 1.f;
	_float m_fRedTime = 0.f;
};

END