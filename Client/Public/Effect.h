#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END


BEGIN(Client)
typedef struct tagWoodDebris
{
	CComponent* pTexture;  
	_float3 vPos; 
	_float3 vDir; 
	float fSpeed;  
	bool bisActive; 
	float flifeTime;  
}WOODDEBRISDESC;

class CEffect : public CBlendObject
{
public:
	CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	EFFECT_TYPE Get_EffectType() { return m_eEffectType; }
public:
	virtual _bool OnEvent(EFFECT_TYPE eEffectType, void* pArg);

public:
	virtual void TurnOff() override;

private:
	CTexture* m_pTextureCom = { nullptr };
//	CTexture* m_pTexEffect_Type[EFFECT_END] = { nullptr };
	CTexture* m_pTexWoodCom[11] = {nullptr};
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = {nullptr};
	CTransform* m_pTargetTransform = { nullptr };
public:
	_bool			Effect_Blood_Impact_Strip(void* pArg);
	_bool			Handle_Effect(void* pArg);
	_bool			Effect_Pow(void* pArg);

	_bool			Effect_Strip(void* pArg);
	_bool			Effect_Smashed(void* pArg);
	_bool			Effect_DazeSwirl(void* pArg);	//±‚¿˝¿Ã∆Â∆Æ
	_bool           Effect_GrossDebris(void* pArg);
	_bool           Effect_SmokeImpactUp(void* pArg);
	_bool           Effect_Smoking(void* pArg);
	_bool           Effect_SparkStrip(void* pArg);
	_bool           Effect_WoodDebrisSprites(void* pArg);
	_bool           Effect_BloodMist(void* pArg);
	_bool           Effect_BulletHoles(void* pArg);
	_bool			Effect_FireStrip(void* pArg);
private:
	_float3					m_vEffectRanPos = { 0.f,0.f,0.f };
	_float					m_fFrame = { 0.0f };
	_bool					m_bEffectPlaying = false;
	_int                    m_iTexCount = 0;
	_int                    m_iWoodIndex = 0;

	vector<CEffect*>		m_vecEffect = { nullptr };
	vector<WOODDEBRISDESC>      m_vecWoodList = { };

	_int					m_WoodIndexList[10];
	vector<CTexture*>		m_WoodTextureList = { nullptr };

private:
	_float3 m_vDirection;


private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	static _uint INDEX_THRESHOLD;
	static _int WOOD_INDEX;
};
END
