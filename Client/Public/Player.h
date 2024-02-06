#pragma once

#include "Client_Defines.h"
#include "Client_Struct.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CPhysics;
END

BEGIN(Client)

typedef struct TextureInfo
{
	_float2 vInitLPos;
	_float2 vInitRPos;
	_float2 vLSize;
	_float2 vRSize;
}TEXINFO;

class CPlayer final : public CGameObject
{
protected:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override; 
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = { nullptr };
	CTexture*				m_pDebugTextureCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };	
	CTransform*				m_pTransformCom = { nullptr };
	CVIBuffer_Rect*			m_pLeftVIBufferCom = { nullptr };
	CVIBuffer_Rect*			m_pRightVIBufferCom = { nullptr };
	CVIBuffer_Cube*			m_pCubeVIBufferCom = { nullptr };
	CPhysics*				m_pPhysicsCom = { nullptr };

	class CCamera_Player* m_pCamera = { nullptr };
	class CPlayerState* m_curState = { nullptr };
	class CGameObject* m_pPickEnemy = { nullptr };
	class CInteractiveObject* m_pPickItem = { nullptr };
	class CInteractiveObject* m_pOldPickItem = { nullptr };
	class CInteractiveObject* m_pHoldItem = { nullptr };
	class CInventory* m_pInventory = { nullptr };

	POINT			m_ptOldMousePos = { 0, 0 };
	POINT			m_ptPauseMousePos = { 0,0 };
	_uint		m_ePlayerTex = { IDLE };
	_uint		m_ePlayerTex2 = { IDLE };
	TEXINFO m_TexInfoArray[TEX_END];
	_float m_fSpeedInfoArray[SPEED_END];
	_float2					m_vLPos, m_vRPos, m_vLSize, m_vRSize;
	_float4x4				m_LworldMatrix, m_RworldMatrix, m_ViewMatrix, m_ProjMatrix;
	_float4x4				m_matNotRot;
	_float m_fTime = 0.f;
	_float2 m_fMouseMovePos = { 0,0 };
	POINT m_fMouseDiff = { 0,0 };
	_float m_fTiltAngle = 0.f;
	_float m_fRightAngle = 0.f;
	_float m_fStaminaFillTime = 0.f;
	_float m_fGuardGaugeFillTime = 0.f;
	_float m_fDeadCamTime = 0.f;
	_bool m_bAlive = true;
	_bool m_bCrouch = false;
	_bool m_bJump = false;
	_bool m_bGuard = false;
	_bool m_bPunchLeft = false;
	_bool m_bEvade = false;
	_bool m_bPistolMeelee = false;
	_bool m_bDeliaHold = false;

	EVADE_DIR m_eEvade = EVADE_S;
	_float m_fEvadeTime = 0.f;
	_float m_fEvadeY = 0.f;

	_bool m_bPause = false;
	_bool m_bDebug = false;
	_bool m_bDebugCollider = false;
	LEVEL m_eLevel = LEVEL_OUTDOOR;
	_float m_fSpeed = 0.f;
	wstring m_strCollisionTag = TEXT("Layer_Floor");
	_int m_iExecuteIndex = 1;
	PLAYERDESC m_PlayerDesc = {};
	OBBDESC m_OBBDesc = {};
	ENEMY_TYPE2 m_eExecuteType = ENEMY_TYPE_END;

	_float3 m_vPickEnemyCollidedPos = { 0,0,0 };
	_float3 m_vPickItemCollidedPos = { 0,0,0 };

	_float m_fSoundVolume = 0.5f;
private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	void CalculateWorldMatrix();
	void InitializeTexInfo();
	void KeyInput();
	void FillStats(_float fTimeDelta);
	void Respawn();

public:	
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

	_uint GetPlayerTex() { return m_ePlayerTex; }
	void SetPlayerOnlyTex(PLAYER_TEX _tex);
	void SetPlayerOnlyTex2(PLAYER_TEX _tex2) {
		m_ePlayerTex2 = _tex2;
		m_vRSize = m_TexInfoArray[_tex2].vRSize;
	}
	void SetPlayerTex(_uint _tex);
	void SetPlayerTex2(PLAYER_TEX _tex)
	{
		m_ePlayerTex2 = _tex;
		m_vRPos = m_TexInfoArray[_tex].vInitRPos;
		m_vRSize = m_TexInfoArray[_tex].vRSize;
	}
	_bool GetJump() { return m_bJump; }
	void SetJump(_bool b) { m_bJump = b; }
	_bool GetPunchLeft() { return m_bPunchLeft; }
	void SetPunchLeft(_bool b) { m_bPunchLeft = b; }
	void AddLeftX(_float f) { m_vLPos.x += f; }
	void AddLeftY(_float f) { m_vLPos.y += f; }
	void AddRightX(_float f) { m_vRPos.x += f; }
	void AddRightY(_float f) { m_vRPos.y += f; }
	void SetLeftX(_float f) { m_vLPos.x = f; }
	void SetLeftY(_float f) { m_vLPos.y = f; }
	_float2 GetLeftPos() { return m_vLPos; }
	_float2 GeRightPos() { return m_vRPos; }
	void SetRightX(_float f) { m_vRPos.x = f; }
	void SetRightY(_float f) { m_vRPos.y = f; }
	void SetLeftPos(_float2 v) { m_vLPos = v; }
	void SetRightPos(_float2 v) { m_vRPos = v; }
	_float2 GetRightPos() { return m_vRPos; }
	_int GetExecuteIndex() { return m_iExecuteIndex; }
	void AddExecuteIndex() {
		m_iExecuteIndex += 1;
		if (m_iExecuteIndex == 4)
			m_iExecuteIndex = 1;
	}
	_float GetRightAngle() { return m_fRightAngle; }
	void AddRightAngle(_float f) { m_fRightAngle += f; }
	void SetRightAngle(_float f) { m_fRightAngle = f; }
	class CCamera_Player* GetCamera() { return m_pCamera; }
	void SetCamera(class CCamera_Player* _camera) { m_pCamera = _camera; }
	void SetTime(float _f) { m_fTime = _f; }
	_float GetTime() { return m_fTime; }
	void SetSpeed(_float _f) { m_fSpeed = _f; }
	_float GetSpeed() { return m_fSpeed; }
	_float GetSpeedInfo(_int i) { return m_fSpeedInfoArray[i]; }
	LEVEL GetLevel() { return m_eLevel; }
	wstring GetCollisionTag() { return m_strCollisionTag; }
	TEXINFO GetTexInfo(_int i) { return m_TexInfoArray[i]; }
	OBBDESC GetOBBDesc() { return m_OBBDesc; }
	void SetCrouch(_bool b) { m_bCrouch = b; }
	_bool GetCrouch() { return m_bCrouch; }
	class CInteractiveObject* GetPickItem() { return m_pPickItem; }
	class CInteractiveObject* GetHoldItem() { return m_pHoldItem; }
	void SetHoldItem(class CInteractiveObject* _pGameObject) { m_pHoldItem = _pGameObject; }
	_float3 GetPickItemCollidedPos() { return m_vPickItemCollidedPos; }
	_float3 GetPickEnemyCollidedPos() { return m_vPickEnemyCollidedPos; }
	PLAYERDESC* GetPlayerDesc() { return &m_PlayerDesc; }
	void SetGuard(_bool b) { m_bGuard = b; }
	_bool GetGuard() { return m_bGuard; }
	void SetInventory(class CInventory* pInven) { m_pInventory = pInven; }
	class CInventory* GetInventory() { return m_pInventory; }
	void SetSoundVolume(_float f) { m_fSoundVolume = f; }
	_float GetSoundVolume() { return m_fSoundVolume; }
	ENEMY_TYPE2 GetExecuteType() { return m_eExecuteType; }
	void SetExecuteType(ENEMY_TYPE2 u) { m_eExecuteType = u; }
	void SetPistolMeelee(_bool b) { m_bPistolMeelee = b; }
	_bool GetPistolMeelee() { return m_bPistolMeelee; }
	void SetDeliaHold(_bool b) { m_bDeliaHold = b; }
	_bool GetDeliaHold(){return m_bDeliaHold;}
	void SetAlive(_bool b) { m_bAlive = b; }
	_bool GetAlive() { return m_bAlive; }

	_bool InvenSelect(void* pArg);

	_bool ChangeHp(void* pArg);
	_bool ChangeStamina(void* pArg);
	_bool ChangeGuardGauge(void* pArg);
	_bool PlayerHit(void* pArg);
	_bool ReduceCurItemHP(void* pArg);
	_bool HoldDelia(void* pArg);

	void ResetCamera(_float fTimeDelta);
	_bool Move(_float fDeltaTime);
	void Evade(_float fTimeDelta);
	void MouseMove(_float fTimeDelta);
	void AnimMouseMove(float fTimeDelta);
	void SetAnimPosIdle();
	_bool AnimToIdle(float fTimeDelta);
	void AnimIdle(float fTimeDelta);
	void AnimWalk(float fTimeDelta);
	void AnimRun(float fTimeDelta);
	void AnimHeavyPunch(_bool bCharge, _float fTime, _float fDeltaTime);
	PLAYER_PUNCH AnimPunch(_bool bAttackEnd,_float fTimeDelta);
	void AnimGuard(_bool bGuardEnd, _float fTime, _float fTimeDelta);
	void AnimKick(_float fTime, _float fTimeDelta);

	void PickObjects();


private:

	CGameObject* m_pExecutionTarget = nullptr; 
	_bool bBack = false;
public:
	typedef  struct tExecutionDesc 
	{
		_int iExecutionIndex = 0; 
		CGameObject* pExecutionTarget = nullptr; 
	}EXECUTIONDESC;

	void 
		Set_ExecutionTarget(CGameObject* pTarget) { m_pExecutionTarget = pTarget; }
	CGameObject* Get_ExecutionTarget() { return m_pExecutionTarget; }
	_bool GetChop() { return bBack; }
	_bool On_ExecutionSuccess(void* pArg);
#pragma endregion

};

END