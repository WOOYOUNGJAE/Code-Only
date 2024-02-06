#pragma once
#include "State.h"

BEGIN(Engine)
	/**
	 * \brief Tick �����ֱ⸸
	 */
	class ENGINE_DLL CFSM: public CComponent // FSM Base
{
protected:
	CFSM(LPDIRECT3DDEVICE9 pGraphic_Device) : CComponent(pGraphic_Device){}
	CFSM(const CFSM& rhs) : CComponent(rhs.m_pGraphic_Device){}
	virtual ~CFSM() = default;

public: // Life Cycle
	
	/**
	 * \brief ���ڷ� Owner ����
	 */
	static CFSM* Create(LPDIRECT3DDEVICE9 pGraphic_Device) ;
	CComponent* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;// { return CComponent::Initialize_Prototype(); }
	virtual HRESULT Initialize(void* pArg) override;
	virtual _int Tick(_float fTimeDelta, void* pArg = nullptr);
	virtual _int Late_Tick(_float fTimeDelta, void* pArg = nullptr);
	virtual void Free() override;// { CComponent::Free(); }
	
public:
	bool Is_State(_uint iState) { return m_iCurState == iState; }
	_int Get_PrevState() { return m_iPrevState; }
	void Set_NextState(_uint iNextState) { m_iNextState = iNextState; }
	// State->CompleteState(),BeginState() �� ����Ǵ� ���� ������ Tick�� ����Ǳ� ���� ��� �ٲٰ� ���� ���
	void Change_Next_Immediately(_uint iNextState, _float fTimeDelta);
	void Go_PrevState(_float fTimeDelta);
	void Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	HRESULT Add_NewState(list<CState*>* pStatesList, CGameObject* pOwner = nullptr);
	HRESULT Add_NewState(CState* pState, CGameObject* pOwner = nullptr);
protected: // Define
#define  STATE_EMPTY -1
#define  FSM_NO_EVENT -1

protected:
	class CGameObject* m_pOwner = nullptr;
	vector<CState*> m_vecState;
	_int m_iCurState = 0; // ��� �ް� ������ Enum����
	_int m_iPrevState = STATE_EMPTY;
	_int m_iNextState = STATE_EMPTY;
};
END
