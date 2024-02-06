#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CVIBuffer_BlinkableRect;
class CVIBuffer_Cube;
class CTransform;
class CFSM;
class CPhysics;
class CLayer;
class CGameInstance;
END

BEGIN(Client)
	/**
	 * \brief
	 * 매 프레임 수행될 것:\n
	 * 타겟까지의 거리와 방향계산: 텍스처 방향, 상시 피격 때문에\n
	 */
	class CEnemyBase : public CGameObject
{
public: // 자주 테스트 해야 하는 값들
	//void Set_SpawnPos(_float _x = 3, _float _y = 1.5f, _float _z = -2.5f);
	void Set_SpawnPos(_float _x = 2.5, _float _y = 1.5f, _float _z = 0.0f);
	void Set_InitMatrix(_float4x4* pMatrix);
protected:
	CEnemyBase(LPDIRECT3DDEVICE9 pGraphic_Device): CGameObject(pGraphic_Device){}
	CEnemyBase(const CEnemyBase& rhs) : CGameObject(rhs){}
	virtual ~CEnemyBase() = default;

public: // Defines
#pragma region Defines
	enum ENEMY_BASE_STATE
	{
		IDLE,
		CAUTIOUS,
		AWARE,
		ATTACKED,
		BEING_FINISHED, // 최후 공격
		DEAD,
		ENEMY_BASE_STATE_END,
	};
	enum ENEMY_TEXTURE_DIR
	{
		// 바라보는 방향. 뒷모습이 North
		NORTH,
		NORTH_EAST,
		NORTH_WEST,
		EAST,
		WEST,
		SOUTH,
		SOUTH_EAST,
		SOUTH_WEST,
		ENEMY_TEXTURE_DIR_END,
	};
	enum EIGHT_DIR_ACTION // 8방향이 모두 있는 동작들
	{
		STAND,
		WALK,
		RUN,
		SUSPICIOUS,
		EIGHT_DIR_ACTION_END,
	};
	enum ONLY_SOUTH_ACTION // 정면만 있는 동작들
	{
		FACE_PUNCHED, // 0
		BODY_PUNCHED,
		LOW_PUNCHED,
		BATTLE_DEFAULT, // todo : 삭제
		INCH_FORWARD,
		TAUNT,
		BASIC_ATTACK,
		HEAVY_ATTACK_WINDUP,
		HEAVY_ATTACK,
		ATTACK_DEFAULT,
		BATTLE_IDLE,
		STRAFING,
		FALLING_START,
		FALLING,
		FALLING_END,
		DEAD_LYING,
		DAZED,
		GET_UP,
		JUMP,
		KARATE_CHOPPED,
		HEADLESS,
		HEADLESS_END,
		ONLY_SOUTH_ACTION_END
	};
	enum ALL_ACTION_STATE
	{
		ACTION_STAND,
		ACTION_WALK,
		ACTION_RUN,
		ACTION_SUSPICIOUS,
		ACTION_FACE_PUNCHED,
		ACTION_BODY_PUNCHED,
		ACTION_LOW_PUNCHED,
		ACTION_BATTLE_DEFAULT, // todo : 삭제
		ACTION_INCH_FORWARD,
		ACTION_TAUNT,
		ACTION_BASIC_ATTACK,
		ACTION_HEAVY_ATTACK_WINDUP,
		ACTION_HEAVY_ATTACK,
		ACTION_ATTACK_DEFAULT,
		ACTION_BATTLE_IDLE,
		ACTION_STRAFING,
		ACTION_FALLING_START,
		ACTION_FALLING,
		ACTION_FALLING_END,
		ACTION_DEAD_LYING,
		ACTION_DAZED,
		ACTION_GET_UP,
		ACTION_JUMP,
		ACTION_KARATE_CHOPPED,
		ACTION_HEADLESS,
		ACTION_HEADLESS_END,
		ALL_ACTION_STATE_END,
	};
	
	typedef struct tagCautious
	{
		tagCautious() = default;
		tagCautious(_float _fNoFindMaxTime) : fNoFindMaxTime(_fNoFindMaxTime){}
		~tagCautious() = default;
		vector<_float3> vecPatrolPos;
		_float fNoFindMaxTime; // 못 찾는 상태 최대 시간, 초과하면 Idle위치로
		_float fNoFindAccTime = 0; // 못 찾는 상태 누적 시간
		
	}CAUTIOUSDESC;
	typedef struct tagAware
	{
		_float fDist_BattleStart; // 배틀 스테이트 스타트 거리
		_float fDist_Attackable; // 이 거리 이내에서 펀치 or 공격
		_float fDist_AlwaysFollow; // 이 이하면 무조건 따라가기
		_float fDist_CheckHidden; // Hidden이면 Cautious로
		_float fDist_CheckPrtHidden; // (Partially)Hidden이면 Cautious
	}AWAREDESC;

	typedef  struct tExecutionDesc
	{
		_int iExecutionIndex = 0; // 공격 유형
		CGameObject* pExecutionTarget = nullptr; // 적들 내부에서 이게 this랑 같으면 처형 실행
	}EXECUTIONDESC;

#pragma endregion Defines
public: // Life Cycle
	CGameObject* Clone(void* pArg) override = 0;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Free() override;

public: // FSM Func - 자신이 쓰지 않고 FSM 전용
	virtual void Reset_IdleState() {} // To be Overwritten
	virtual void Reset_CautiousState(){} // To be Overwritten
	virtual void Reset_AwareState(){} // To be Overwritten
	virtual void Tick_Moving(_float fTimeDelta, _uint* eStateOut, _uint eStateReadyToAttack, _uint eStateRush);
	virtual void Tick_Attack(_float fTimeDelta);
	virtual _int LateTick_NotAware(_float fTimeDelta); // Aware 상태가 아닐 경우에 공통적으로 호출
	virtual _bool Chase_Target(_float fMinDistance, _float fTimeDelta, _float vRandomAddingX, _float fMoreWalkDistance = 0.f);// 계속 따라갈거면 true, Transform 컴포넌트의 Chase로 변경 여지 있음
	virtual _bool ApproachtoTarget(_float fMinDistance, _float fTimeDelta);
	virtual _bool Chase_Target_Lerp(_bool& bStepable, _float3* pStartDir, _float& fCurStep, _float fOneStep, _float fAttackable, _float fLerpSpeed, _float fTimeDelta);
	virtual _bool Strafing_Lerp(_bool& bStepable, _float3* pStartDir, _float& fCurStep, _float fOneStep, _float fLerpSpeed, _float fTimeDelta);
	virtual _bool Simple_Move(_float& refAccDistance, _float fMaxDistance, _float fKnockBackSpeed);
	virtual void Set_KnockBackDir();
	virtual void Make_Look_Target();
	virtual _bool Check_AttackSuccess();
	virtual void StayTurn(_float fDegree);
	virtual _bool Is_Dazed();
	virtual void Add_Force(_float3 vForceDirNormalized, _float fMagnitude, _float fDecelerationFactor = 0.75f);
	virtual _bool Has_Magnitude();
	virtual void Reset_Gauge();
	virtual void Send_HPInfo(_uint iPrevHP, _uint iCurHP);
	virtual void Play_MySound(_tchar* strKey);
	virtual void Stop_MySound();
	virtual void Go_Straight(_float fTimeDelta);
	virtual void Create_DazedEffect();
	virtual void Erase_DazedEffect();
	virtual void Stop_Talking() { m_bIsTalking = false; }
public: // Getter Setter
	_bool Is_Dynamic() { return m_bIsDynamic; }
	_bool Is_Sensitive() { return m_bIsSensitive; }
	_bool Is_Alive() { return m_bIsAlive; }
	_float Get_Distance_ToTarget() { return m_fDist_BtwTarget; }
	CFSM* Get_FSM() { return m_pFSMCom; }
	_uint Get_AnimPlayedCount() { return m_iAnimPlayedCount; }
	_uint Get_FrameLeft() { return m_iAnimFrameMaxArr[m_iCurAction] - static_cast<_uint>(m_fCurFrame); }
	_float Get_Anim_FrameRate() { return m_fCurFrame / static_cast<_float>(m_iAnimFrameMaxArr[m_iCurAction]); } // 전체중에 얼마나 진행되었는지
	_float3 Get_ToTargetXZ() { return m_vToTargetXZ; }
	_float3 Get_Pos();
	_float3 Get_KnockBackDir() { return m_vKnockBackDir; }
	HRESULT Set_Target(class CPlayer* pTarget);
	_float Get_CurFrame() { return m_fCurFrame; }
	_uint Get_CurAction() { return m_iCurAction; }
	_float3 Get_TargetRight();
	_bool Is_Executed() { return m_bExecuted; }
	ENEMY_TYPE2 Get_EnemyType() { return m_eEnemy_Type; }
	_bool Get_PromptTrue() { return m_bExecutePrompt; }
	_float3 Get_NearRandomPos(_float fMulti = 1.f);
	_float3 Get_Look();
	_float3 Get_Up();
	_float4x4 Get_TextureMatrix() { return m_TextureMat; }
	_bool Is_Stepable() { return m_bStepable; }
	_bool Get_Rest() { return m_bRest; }
	_bool Get_Hit_byTrain() { return m_bHit_byTrain; }
	_bool Is_State(_uint iState);
	_bool Is_TargetAlive();
	void Set_CautiousDesc(_float _fNoFindMaxTime) { m_CautiousDesc.fNoFindMaxTime = _fNoFindMaxTime; }
	
	//void Set_AwareDesc(_float _fBattleDist, _float _fAlwaysFollowDist, _float _fCheckHiddenDist, _float _fCheckPrtHiddenDist) { m_AwareDesc.fDist_BattleStart = _fBattleDist; m_AwareDesc.fDist_AlwaysFollow = _fAlwaysFollowDist; m_AwareDesc.fDist_CheckHidden = _fCheckHiddenDist; m_AwareDesc.fDist_CheckPrtHidden = _fCheckPrtHiddenDist; }
	/**
	 * \brief Awareness 게이지 연산이 필요 없을 경우 호출, 그 외에는 LateTick_NotAware()\ㅜ
	 * 내부에서 타겟과 각도 계산까지 자동으로
	 */
	void Set_TextureDir() { m_eTextureDir = Get_Texture_Dir(Calculate_Angle()); }
	void Set_NextAction(_uint iNextAction);
	void Set_CurAnimSpeedMultiplied(_float fNum) { m_fAnimSpeedArr[m_iCurAction] *= fNum; }
	virtual void Set_AnimSpeedDefault();
	void Set_GaugeActive(_bool _b);
	void Set_NextFSMState(_uint iNextState);
	void Make_Dead();
	void Set_TextureSize_Regular(_bool b) { m_bRegularSize = b; }
	void Set_Size(_float fX, _float fY) { m_fSizeX = fX; m_fSizeY = fY; }
	void Set_Chopped(_bool b) { m_bIsChopped = b; }
	void Set_PromptTrue(_bool b) { m_bExecutePrompt = b; }
	void Set_Dead();
	void Set_InAir(_bool b) { m_bInAir = b; }
	void Set_Stepable(_bool b) { m_bStepable = b; }
	void Set_Rest(_bool b) { m_bRest = b; }
	void Set_Hit_byTrain();
	void Set_Look(const _float3& vLookDir);
protected:
#pragma region StaticConst
	// for Angle
	static const _float PI;
	static const _float HALF_QRT_PI;
	static const _float NORTH_MIN_RAD;
	static const _float NORTH_MAX_RAD;
	static const _float NORTH_EAST_MAX_RAD;
	static const _float NORTH_WEST_MAX_RAD;
	static const _float SOUTH_MIN_RAD;
	static const _float SOUTH_MAX_RAD;
	static const _float SOUTH_EAST_MAX_RAD;
	static const _float SOUTH_WEST_MAX_RAD;
	static const _float EAST_MAX_RAD;
	static const _float WEST_MAX_RAD;
#pragma endregion StaticConst
	
protected:
	// Components
	virtual HRESULT Add_Components();
	virtual HRESULT Add_StateOnFSM();

	// Func - States Use Not Directly
	virtual HRESULT Update_ToTarget(); // 현재 위치 타겟 위치 방향 업데이트
	virtual _float Calculate_Angle(); // 적과의 각도 계산
	virtual void Func_AwarenessGauge(_float fTimeDelta); // Awareness 게이지 계산, State 변경
	virtual ENEMY_TEXTURE_DIR Get_Texture_Dir(_float fRad); // 주어진 각도에 따른 텍스처Dir 반환
	virtual void Make_LookCam(); // BillBoard
	virtual _bool Can_See_Target(); //  TODO : 타겟까지 장애물 없으면 true
	/**
	 * \brief 
	 * \param eTextureDir 매 프레임 결정되는 텍스처 방향
	 * \param curAction FSM 내부 동작으로 결정된느 현재 동작
	 * \return 텍스처
	 */
	virtual CTexture* Get_CurTexture(ENEMY_TEXTURE_DIR eTextureDir, _uint curAction);
	// in,  맞았을 때 out : 어디 맞았는지, 자신 포인터, 현재 hp,
	// EVENT
	virtual _bool On_CheckHit(void* pArg);
	virtual _bool On_CheckArrow(void* pArg);
	virtual _bool On_CheckTobeExecuted(void* pArg);
	virtual _bool On_GetExecutionHit(void* pArg); // 처형 확정 된 상태
	virtual _bool On_CheckThrownHit(void* pArg);
	virtual _bool On_CheckGunFire(void* pArg);
	virtual void Set_AnimProperty();
	virtual void Play_DyingSound();
protected:
	// Components and Transform
#pragma region Components and Transform Info
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDebugTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CVIBuffer_BlinkableRect* m_pVIBufferCom = { nullptr };
	CVIBuffer_Rect* m_pVIDeliaBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CFSM* m_pFSMCom = nullptr;
	CPhysics* m_pPhysicsCom = nullptr;

	CVIBuffer_Cube* m_pCubeVIBufferCom = { nullptr };

	CVIBuffer_Rect* m_pVIBufferCom_Awareness = { nullptr };

	CTexture* m_EightDirTexturesArr[ENEMY_TEXTURE_DIR_END][EIGHT_DIR_ACTION_END]{};
	CTexture* m_OnlySouthTexturesArr[ONLY_SOUTH_ACTION_END]{};
	CTexture* m_AwarenessIcon_Alert = nullptr;
	CTexture* m_AwarenessIcon_Spooked = nullptr;
	CTexture* m_AwarenessIcon_Cautious = nullptr;
	CTexture* m_SpeechBubble = nullptr;

	_bool m_bRegularSize = false; // true 면 텍스처 1대1 사이즈
	_float m_fX = 0.f;
	_float m_fSizeY = 1.5f;
	_float m_fY = 0.f;
	_float m_fSizeX = 0.5f;
	_float4x4				m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
	_float4x4 m_TextureMat; // 항상 카메라를 보는 텍스처용 Matrix
	_float4x4 m_matNotRot; // obb 박스 용

	_float4x4 m_IconMat{}; // 아이콘 전용
	_float m_fIconSizeX = 0.25f;
	_float m_fIconSizeY = 0.5f;
	_float m_fBubbleIconSizeX = 1.f;
	_float m_fBubbleIconSizeY = 1.f;
	_float m_fCollectiveRange = 15.f;
#pragma endregion Components and Transform Info

	// Pointer
	class CPlayer* m_pTarget = nullptr;
	CTransform* m_pTargetTransform = nullptr;
	class CAwarenessGauge* m_pGauge = nullptr;
	class CEnemy_HealthSystem* m_pHealthSystem = nullptr;
	class CEnemyPhysics* m_pPhysics = nullptr;
	CLayer* m_pThisLayer = nullptr;
	CGameInstance* m_pGameInstance = nullptr;
	// Render And Texture
	ENEMY_TEXTURE_DIR m_eTextureDir = SOUTH;
	EIGHT_DIR_ACTION m_eEightDirAction = EIGHT_DIR_ACTION_END;
	_uint m_iAnimFrameMaxArr[ALL_ACTION_STATE_END] {}; // 동작당 프레임 개수
	_float m_fAnimSpeedArr[ALL_ACTION_STATE_END]{};
	_float m_fAnimDefaultSpeedArr[ALL_ACTION_STATE_END]{};
	//_float m_fAnimSpeed
	_uint m_iCurAction = ACTION_STAND;
	_uint m_iNextAction = ALL_ACTION_STATE_END;
	_float m_fCurFrame = 0.f;
	_uint m_iAnimPlayedCount = 0; // 현 애니메이션 몇 번 재생되었는지
	

	CAUTIOUSDESC m_CautiousDesc; // Cautious 상태 Desc
	//AWAREDESC m_AwareDesc; // Aware 상태 Desc

	// Enemy Type
	ENEMY_TYPE2 m_eEnemy_Type = ENEMY_TYPE_END;
	_bool m_bIsDynamic = false; // Idle일 때 돌아다니냐
	_bool m_bIsSensitive = true; // false면 뒤에서 뭔 짓을 해도 모름
	// Health and Attacked Desc
	ENEMYHPDESC m_tHP_Desc{};
	_uint m_iMaxHP = 100;
	_uint m_iCurHP = m_iMaxHP;
	_float m_iLowBodyHeight = m_fSizeY * 0.5f; // 이 이하면 사타구니 밑
	_float m_iFaceHeight = m_fSizeY * 0.2f; ; // 이 이상이면 얼굴

	_float3 m_vToTarget; // 적 to 타겟
	_float3 m_vToTargetXZ; // 방향의 y는 0인 수평 방향
	_float m_fDist_BtwTarget = FLT_MAX; // 타겟까지의 거리
	//_float m_fRadian_BtwTarget = 0.f; // 절대값처리된 각도

	// for 공격 판정
	//_float m_fAngleXZ_Attacked = D3DXToRadian(15.f); // 특정 거리 이내에서 이 각도 이내에 있으면 피격
	//_float m_fAngleYZ_Attacked = D3DXToRadian(33.f); // 특정 거리 이내에서 이 각도 이내에 있으면 피격
	_float m_fDist_Attacked = 1.f;
	_uint m_iAttackedPart = 0; // 맞은 부위 0머리 1몸통 2사타구니

	_float3 m_vKnockBackDir;

	// for Battle::Fight State
	_float m_fDist_Attack; // 이 거리 이내가 되도록 무빙하고 들어왔을 때 공격

	// for Sensitive
	_float m_fDist_Hearable; // 이 거리 이내의 서서 걷는 발소리 들리면 Cautious
	_float m_fDamageRange = 3.f;

	// 사운드 볼륨
	_float m_fSoundVolume = 0.5f;

	_bool m_bIsAlive = true;
	_bool m_bIsNotTick = false;
	_float m_fIsDeadTime = 0.f;
	_bool m_bHit_byTrain = false;

	_bool m_bExecuting = false; // true 면 Render 막기
	_bool m_bExecuted = false; //
	//Gravity, Physics
	_bool m_bInAir = true;
	// HP Bar
	_bool m_bRenderHpBar = true;

	_float3 m_vCamPos;
	_float3 m_vCamLook;
	// For Goon
	_bool m_bIsChopped = false;
	// Prompt
	_bool m_bExecutePrompt = false;
	// detour : 우회하다
	_bool m_bDetour = false;
	_float m_fDetourMaxDistance = 1.5f; // 우회 거리
	_float m_fDetourAccDistance = 0.f;
	_float3 m_vDetourDir;
	_bool If_Close_EachOther(); // 동료랑 너무 가까운지
	//list<class CGameObject*> m_Enemies;
	_int m_iSound_Channel = 9;
	// Moving - Step
	_bool m_bStepable = false;
	_bool m_bRest = false;
	_bool m_bSendArrow = true;
	_uint m_iEffectIndex = 0;
	EFFECTDESC m_tEffectDesc{};
	EFFECTDESC2 m_tEffectDesc2{};
	_bool m_bEffectOn = false;
	wstring m_strCautiousSoundKey;
	_bool m_bTalkingEnemy = false; // true 일 때 사운드 키 재생
	_bool m_bIsTalking = false;
	_float m_fCurBubbleFrame = 0.f;
protected: // 자식이 건들 필요 없는 것들
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void SetIsNotTick(_bool b) { m_bIsNotTick = b; }
	_bool GetIsNotTick() { return m_bIsNotTick; }
	// 각도와 거리에 따른 스피드, 노출되고 난 후 지나간 시간. int로 버림

public:
	_float Get_Ratio() { return m_pVIBufferCom->Get_Ratio(); }
	void Set_Ratio(_float f) { m_pVIBufferCom->Set_Ratio(f); }
protected:
	_bool m_bRedStart = false;
	_float m_fRedTime = 0.f;
};
END