#pragma once
#include "Component.h"

BEGIN(Engine)
	class ENGINE_DLL
CState abstract : public CBase // State Base
{
protected:
	CState(LPDIRECT3DDEVICE9 pGraphic_Device) : m_pGraphic_Device(pGraphic_Device)
	{
		Safe_AddRef(m_pGraphic_Device);
	}
	virtual ~CState() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr); 
	virtual void Free() override;

public:
	//void Reserve(_uint iInnerFSMCnt) { m_vecInnerFSM.reserve(iInnerFSMCnt); }
	void Set_Owner(class CGameObject* pOwner) { m_pOwner = pOwner; }
	virtual HRESULT Add_InnerFSM(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

	virtual void BeginState(_float fTimeDelta) = 0; // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg = nullptr) = 0; // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta) = 0; // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg = nullptr) = 0;
	

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	// InnerState의 첫 시작과 State의 마지막은 기본상태(0)
	class CFSM*			m_pInnerFSM = nullptr;
	class CGameObject* m_pOwner = nullptr;
};

END