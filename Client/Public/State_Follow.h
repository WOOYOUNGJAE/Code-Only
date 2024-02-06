#pragma once
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Follow_Enemy : public CState_Enemy
{
	CState_Follow_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	virtual ~CState_Follow_Enemy() = default;


public:
	static CState_Follow_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, _float fDist_BattleStart, void* pArg = nullptr);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	_float m_fDist_BattleStart = 1.85f;
	//_float m_fDist_Attackable = 0.2f;
	//_float m_fDist_ActionInchForward = 1.85f;

	_float m_fRandomAddingX;
	_float m_fRandom_MoreWalkDistance = 0.f; // �������� �� ���� �ɾ�� �ϴ� �Ÿ� 0~1.5f;
};

END