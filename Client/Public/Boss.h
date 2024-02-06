#pragma once
#include "Client_Defines.h"
#include "EnemyBase.h"

BEGIN(Client)
class CBoss final : public CEnemyBase
{
protected:
	CBoss(LPDIRECT3DDEVICE9 pGraphic_Device) : CEnemyBase(pGraphic_Device) {}
	CBoss(const CBoss& rhs) : CEnemyBase(rhs.m_pGraphic_Device) {}
	~CBoss() = default;

public: // define
	enum BOSS_EIGHT_DIR_ACTION // 8방향이 모두 있는 동작들
	{
		STAND,
		WALK,
		BATTLE_IDLE_NEAR,
		BATTLE_IDLE_FAR,
		BASIC_ATTACK, // 총으로 때리기
		BOSS_FIRE,
		BOSS_EIGHT_DIR_ACTION_END,
	};
	enum BOSS_ONLY_SOUTH_ACTION // 정면만 있는 동작들
	{
		FACE_PUNCHED, // 0
		BODY_PUNCHED,
		LOW_PUNCHED,
		BATTLE_DEFAULT, // 
		TAUNT,
		FALLING_START,
		FALLING,
		FALLING_END,
		DEAD_LYING,
		DAZED,
		GET_UP,
		RELOADING,
		ROLL_EAST,
		ROLL_WEST,
		ROLL_EAST_REVERSE,
		ROLL_WEST_REVERSE,
		RUN,
		BOSS_ONLY_SOUTH_ACTION_END
	};
	enum BOSS_ALL_ACTION_STATE
	{
		ACTION_BOSS_STAND,
		ACTION_BOSS_WALK,
		ACTION_BOSS_BATTLE_IDLE_NEAR,
		ACTION_BOSS_BATTLE_IDLE_FAR,
		ACTION_BOSS_BASIC_ATTACK, // 총으로 때리기
		ACTION_BOSS_BOSS_FIRE,
		ACTION_BOSS_FACE_PUNCHED, // 0
		ACTION_BOSS_BODY_PUNCHED,
		ACTION_BOSS_LOW_PUNCHED,
		ACTION_BOSS_BATTLE_DEFAULT, // 
		ACTION_BOSS_TAUNT,
		ACTION_BOSS_FALLING_START,
		ACTION_BOSS_FALLING,
		ACTION_BOSS_FALLING_END,
		ACTION_BOSS_DEAD_LYING,
		ACTION_BOSS_DAZED,
		ACTION_BOSS_GET_UP,
		ACTION_BOSS_RELOADING,
		ACTION_BOSS_ROLL_EAST,
		ACTION_BOSS_ROLL_WEST,
		ACTION_BOSS_ROLL_EAST_REVERSE,
		ACTION_BOSS_ROLL_WEST_REVERSE,
		ACTION_BOSS_RUN,
		BOSS_ALL_ACTION_STATE_END,
	};

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Free() override;
public: // Getter Setter
	_uint Get_DistanceState() { return m_iDistanceState; }
	void Set_DistanceState(_uint iDistanceState) { m_iDistanceState = iDistanceState; }
public: // override
	virtual HRESULT Add_Components() override;
	virtual HRESULT Add_StateOnFSM() override;
	virtual _int LateTick_NotAware(_float fTimeDelta) override;
	virtual void Set_AnimProperty() override;
	virtual void Set_AnimSpeedDefault() override{}
	virtual _bool Check_AttackSuccess() override;
	virtual _bool Chase_Target(_float fMinDistance,  _float fWalkMaxDistance, _float fTimeDelta);
	virtual CTexture* Get_CurTexture(ENEMY_TEXTURE_DIR eTextureDir, _uint curAction) override;
	virtual _bool On_CheckHit(void* pArg) override;
	virtual _bool On_GetExecutionHit(void* pArg) override;
	virtual _bool On_CheckThrownHit(void* pArg) override;
	virtual _bool On_CheckGunFire(void* pArg) override;
public:
	void Make_Aware();
	void Restart_AwareState();
private:
	CTexture* m_pTextureComArr[ENEMY_TEXTURE_DIR_END] = {};
	//Components
	CTexture* m_BossEightDirTexturesArr[ENEMY_TEXTURE_DIR_END][BOSS_EIGHT_DIR_ACTION_END]{};
	CTexture* m_BossOnlySouthTexturesArr[BOSS_ONLY_SOUTH_ACTION_END]{};

	//Animation
	_uint m_iBossAnimFrameMaxArr[BOSS_ALL_ACTION_STATE_END]{};
	_float m_fBossAnimSpeedArr[BOSS_ALL_ACTION_STATE_END]{};
	// Pointer
	class CState_Enemy* m_pState_Aware = nullptr;
	// FSM
	_uint m_iDistanceState = 3; // end
	_bool m_bTriggered = false;

};

END