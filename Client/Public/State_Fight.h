#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)
/**
 * \brief Battle���� Fight ������Ʈ
 * ����
 * �Ϲ���ġ
 * �޷����鼭 �Ϲ���ġ
 * ����ġ
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
	virtual void BeginState(_float fTimeDelta); // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta); // �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	virtual void Free() override;

private:
	FIGHT_STATE m_eFight_State = FIGHT_STATE_END;
};

END