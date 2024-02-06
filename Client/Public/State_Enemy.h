#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Enemy abstract: public CState
{
protected:
	CState_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
	CState_Enemy(CState_Enemy& rhs) : CState(rhs.m_pGraphic_Device) {}
	virtual ~CState_Enemy() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override { CState::Free(); /*Safe_Release(m_pOwnerDetailed)*/; }

public:
	virtual void BeginState(_float fTimeDelta) {} // �� ���¿��� ������ �� �ѹ��� ȣ��
	virtual _int Tick_State(_float fTimeDelta, void* pArg) { return 0; } // �� ���¿��� �� ������ ȣ��
	virtual void CompleteState(_float fTimeDelta) {}// �ٸ� ���·� �ٲٱ� ���� �ѹ��� ȣ��
	virtual _int LateTick_State(_float fTimeDelta, void* pArg) { return 0; }

public: // getter setter
	virtual HRESULT Set_OwnerDetailed(CGameObject* pOwner_Original);
protected:
	class CEnemyBase* m_pOwnerDetailed = nullptr;
};

END