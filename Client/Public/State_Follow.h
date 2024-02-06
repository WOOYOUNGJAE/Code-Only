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
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

private:
	_float m_fDist_BattleStart = 1.85f;
	//_float m_fDist_Attackable = 0.2f;
	//_float m_fDist_ActionInchForward = 1.85f;

	_float m_fRandomAddingX;
	_float m_fRandom_MoreWalkDistance = 0.f; // 랜덤으로 더 많이 걸어야 하는 거리 0~1.5f;
};

END