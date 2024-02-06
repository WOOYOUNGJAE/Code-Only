#pragma once

#include "Base.h"
#include "BlendObject.h"

BEGIN(Engine)

class CEffectManager : public CBase
{
	DECLARE_SINGLETON(CEffectManager)
private:
    CEffectManager() = default;
    ~CEffectManager() = default;
public:
    void Free() override;
public:
    HRESULT Reserve_Manager(_uint iLevelIndex);
    void Initialize();

    void Add_Effect(CBlendObject* pEffect, _uint iNumEffects);
    void Add_Effect(CBlendObject* pEffect, EFFECTARG* iNumEffects);

public:
    HRESULT         TurnOff_Effect(_uint iEffectType, _uint iEffectIndex);

private:
    _bool			Effect_Blood_Impact_Strip(void* pArg);
    _bool			Effect_Pow(void* pArg);

    _bool			Effect_Strip(void* pArg);
    _bool			Effect_Smashed(void* pArg);
    _bool			Effect_DazeSwirl(void* pArg);	//±‚¿˝¿Ã∆Â∆Æ
    _bool			Effect_GrossDebris(void* pArg);
    _bool			Effect_SmokeImpactUp(void* pArg);
    _bool			Effect_Smoking(void* pArg);
    _bool			Effect_SparkStrip(void* pArg);
    _bool			Effect_WoodDebrisSprites(void* pArg);
    _bool			Effect_BloodMist(void* pArg);
    _bool			Effect_BulletHoles(void* pArg);

    _bool           Effect_Fire_Strip(void* pArg);

private:
    _bool Execute_Event(EFFECT_TYPE eEffectType, void* pArg);

    //   CEffect*    Find_Effect();

private:
    vector<CBlendObject*>* m_vecAvailableEffects = nullptr;
    typedef vector<CBlendObject*> EFFECT;

private:
    _uint m_iNumLevel = { 0 };
    

};

END