#pragma once
#include "Client_Defines.h"
#include "State_Enemy.h"

BEGIN(Engine)
class CFSM;
END

BEGIN(Client)
class CState_Aware : public CState // Prototype
{
private:
	CState_Aware(LPDIRECT3DDEVICE9 pGraphic_Device) : CState(pGraphic_Device) {}
	CState_Aware(CState_Aware& rhs) : CState(rhs.m_pGraphic_Device) {}
	virtual ~CState_Aware() = default;

public:
	static CState_Aware* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual HRESULT Initialize(void* pArg = nullptr) override { return S_OK; }
	virtual void Free() override { CState::Free(); }

public:
	virtual void* BeginState(_float fTimeDelta, void* pArg) { return S_OK; } // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg) { return S_OK; } // 현 상태에서 매 프레임 호출
	virtual void* CompleteState(_float fTimeDelta, void* pArg) { return S_OK; }// 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg) { return S_OK; }
};

class CState_Aware_Enemy : public CState_Enemy
{
protected:
	CState_Aware_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Enemy(pGraphic_Device) {}
	CState_Aware_Enemy(CState_Aware_Enemy& rhs) : CState_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Aware_Enemy() = default;

protected: //Defines
	enum AWARE_STATE
	{
		READY,
		FOLLOW,
		BATTLE,
		AWARE_STATE_END,
	};
	
	typedef struct tagAware
	{
		tagAware() = default;
		tagAware(_float _fBattleDist, _float _fAlwaysFollowDist, _float _fCheckHiddenDist, _float _fCheckPrtHiddenDist)
			:fDist_BattleStart(_fBattleDist), fDist_AlwaysFollow(_fAlwaysFollowDist), fDist_CheckHidden(_fCheckHiddenDist),
			fDist_CheckPrtHidden(_fCheckPrtHiddenDist) {}
		~tagAware() = default;
		_float fDist_BattleStart; // 배틀 스테이트 스타트 거리
		_float fDist_Attackable; // 이 거리 이내에서 펀치 or 공격
		_float fDist_AlwaysFollow; // 이 이하면 무조건 따라가기
		_float fDist_CheckHidden; // Hidden이면 Cautious로
		_float fDist_CheckPrtHidden; // (Partially)Hidden이면 Cautious
	}AWAREDESC;
	typedef struct tagArgu
	{
		CEnemyBase* pOwnerDetailed = nullptr;
		AWAREDESC* pDesc = nullptr;
	}ARGUDESC;
public:
	static CState_Aware_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, void* pArg);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

protected:
	AWAREDESC m_tDesc {};
	//_float m_fDist_ActionWalk;
};


class CState_Aware_PipeGuy : public CState_Aware_Enemy
{
protected:
	CState_Aware_PipeGuy(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Aware_Enemy(pGraphic_Device) {}
	CState_Aware_PipeGuy(CState_Aware_PipeGuy& rhs) : CState_Aware_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Aware_PipeGuy() = default;
	
public:
	static CState_Aware_PipeGuy* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, void* pArg);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);
	
};



class CState_Aware_Boss : public CState_Aware_Enemy
{
protected:
	CState_Aware_Boss(LPDIRECT3DDEVICE9 pGraphic_Device) : CState_Aware_Enemy(pGraphic_Device) {}
	CState_Aware_Boss(CState_Aware_Boss& rhs) : CState_Aware_Enemy(rhs.m_pGraphic_Device) {}
	virtual ~CState_Aware_Boss() = default;
public://Define
	enum BOSS_AWARE_STATE
	{
		BOSS_AWARE_READY, // TAUNT. RELOAD
		BOSS_AWARE_FOLLOW,
		BOSS_AWARE_FIRE,
		BOSS_AWARE_SHORT_WALK,
		BOSS_AWARE_NEAR_ATTACK,
		BOSS_AWARE_ROLL,
		BOSS_AWARE_RELOAD,
		BOSS_AWARE_STATE_END,
	};

	enum BOSS_DISTANCE_STATE
	{
		DISTANCE_NEAR,
		DISTANCE_NORMAL,
		DISTANCE_FAR,
		BOSS_DISTANCE_STATE_END,
	};
public:
	static CState_Aware_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CEnemyBase* pOwnerDetailed, void* pArg);
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void BeginState(_float fTimeDelta); // 현 상태에서 시작할 때 한번만 호출
	virtual _int Tick_State(_float fTimeDelta, void* pArg); // 현 상태에서 매 프레임 호출
	virtual void CompleteState(_float fTimeDelta); // 다른 상태로 바꾸기 직전 한번만 호출
	virtual _int LateTick_State(_float fTimeDelta, void* pArg);

public: // FSM Func
	void Tick_Ready(_float fTimeDelta);
	void Tick_Follow(_float fTimeDelta);
	void Tick_Fire(_float fTimeDelta);
	void Tick_ShortWalk(_float fTimeDelta);
	void Tick_NearAttack(_float fTimeDelta);
	void Tick_Roll(_float fTimeDelta);
	void Tick_Reload(_float fTimeDelta);
public:
	void Restart();
private:
	void RefreshDir();
	void RefreshDir(const _float3& vNewDir);
	void Set_RollDir(_int iRandom);
	_bool Check_ShootSuccess(_float3 vRealToTargetDirXZ); //
private:
	_float m_fNearMax = 2.4f;
	_float m_fNormalMax = 5.f;
	BOSS_DISTANCE_STATE m_eDistanceState = BOSS_DISTANCE_STATE_END;
	BOSS_AWARE_STATE m_eCurState = BOSS_AWARE_READY;
	_float m_fDistance = 0.f;

	_float3 m_vDirXZ;
	// Follow
	_float m_fChaseMinDistance = 5.f; // m_fNormalMax와 같게
	_float m_fWalkMaxDistance = 5.f + 2.f; // m_fChaseMinDistance에서 몇 걸음 더
	// NearAttack
	_float m_fNearAttackAccTime = 0.f;
	_bool m_bNearAttackRestComplete = false;
	_int m_iNearAttackDamage = -6;
	// Roll
	_float3 m_vRollDir;
	_uint m_vRollActionIndex = 0;
	_float m_fRollAccTime = 0.f;
	_bool m_bRollRestComplete = false;
	// Fire
	_uint m_iFireCount = 0;
	_float m_fFireAccTime = 0.f;
	_float m_fFireTerm = 0.5f;
	_bool m_bFireRestComplete = false;
	_bool m_bRealFired = false;
	_int m_iFireDamage = -6;
};


END
