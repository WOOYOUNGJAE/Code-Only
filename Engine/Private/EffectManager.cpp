#include "EffectManager.h"
#include <GameInstance.h>
#include <Object_Manager.h>

IMPLEMENT_SINGLETON(CEffectManager)


void CEffectManager::Free()
{
   /* for (_uint i = 0; i < EFFECT_END; ++i)
    {
        for (CBlendObject* effect : m_vecAvailableEffects[i])
        {
            Safe_Release(effect);
        }

        m_vecAvailableEffects[i].clear();
    }*/
   
    Safe_Delete_Array(m_vecAvailableEffects);
}

HRESULT CEffectManager::Reserve_Manager(_uint iLevelIndex)
{
    m_iNumLevel = iLevelIndex;
    m_vecAvailableEffects = new EFFECT[EFFECT_END];

   

    for (_uint effectIndex = 0; effectIndex < EFFECT_END; ++effectIndex)
    {
        for (int i = 0; i < 10; ++i)
        {
            EFFECTARG tEffectArg;
            tEffectArg.eEffecttype = effectIndex;
            tEffectArg.iEffectIdentity[effectIndex][i] = (effectIndex * 10) + i;
            //_int index = i;
           // CObject_Manager::Get_Instance()->Add_Effect(iLevelIndex, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect"), &effectIndex);
            CObject_Manager::Get_Instance()->Add_Effect(iLevelIndex, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect"), &tEffectArg);
        }
    }

    Initialize();

    return S_OK;
}

void CEffectManager::Initialize()
{
    //이벤트 구독
    CGameInstance* pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(pGameInstance);

    //구독함수
    pGameInstance->Subscribe(EVENT_EFFECT_BLOOD_IMPACTSTRIP,
        std::bind(&CEffectManager::Effect_Blood_Impact_Strip, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_POW,
        std::bind(&CEffectManager::Effect_Pow, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_STRIP,
        std::bind(&CEffectManager::Effect_Strip, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_SMASHED,
        std::bind(&CEffectManager::Effect_Smashed, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_DAZESWIRL,
        std::bind(&CEffectManager::Effect_DazeSwirl, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_GROSSDEBRIS,
        std::bind(&CEffectManager::Effect_GrossDebris, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_SMOKEIMPACTUP,
        std::bind(&CEffectManager::Effect_SmokeImpactUp, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_SMOKING,
        std::bind(&CEffectManager::Effect_Smoking, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_SPARKSTRIP,
        std::bind(&CEffectManager::Effect_SparkStrip, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_WOODDEBRISSPRITES,
        std::bind(&CEffectManager::Effect_WoodDebrisSprites, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_BLOOD_MIST,
        std::bind(&CEffectManager::Effect_BloodMist, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_BULLET_HOLES,
        std::bind(&CEffectManager::Effect_BulletHoles, this, std::placeholders::_1));

    pGameInstance->Subscribe(EVENT_EFFECT_FIRE_STRIP,
        std::bind(&CEffectManager::Effect_Fire_Strip, this, std::placeholders::_1));
    

    Safe_Release(pGameInstance);
}

void CEffectManager::Add_Effect(CBlendObject* pEffect, _uint iNumEffects)
{
    m_vecAvailableEffects[iNumEffects].push_back(pEffect);
}
void CEffectManager::Add_Effect(CBlendObject* pEffect, EFFECTARG* iNumEffects)
{
  
   // pEffect->Set_EffectIndex(iNumEffects->iEffectIdentity);
    
    
    //pEffect->Set_EffectIndex(iNumEffects->iEffectIdentity[iNumEffects->eEffecttype]);
    m_vecAvailableEffects[iNumEffects->eEffecttype].push_back(pEffect);
}

_bool CEffectManager::Effect_Blood_Impact_Strip(void* pArg)
{
    //for (int i = 0; i < 10; ++i)
   // {
        Execute_Event(EFFECT_BLOOD_IMPACTSTRIP, pArg);
   // }
    return true;
}

_bool CEffectManager::Effect_Pow(void* pArg)
{
    return Execute_Event(EFFECT_POW, pArg);
}

_bool CEffectManager::Effect_Strip(void* pArg)
{
    return Execute_Event(EFFECT_STRIP, pArg);
}

_bool CEffectManager::Effect_Smashed(void* pArg)
{
    return Execute_Event(EFFECT_SMASHED, pArg);
}

_bool CEffectManager::Effect_DazeSwirl(void* pArg)
{
    return Execute_Event(EFFECT_DAZESWIRL, pArg);
}

_bool CEffectManager::Effect_GrossDebris(void* pArg)
{
    return Execute_Event(EFFECT_GROSSDEBRIS, pArg);
}

_bool CEffectManager::Effect_SmokeImpactUp(void* pArg)
{
    return Execute_Event(EFFECT_SMOKEIMPACTUP, pArg);
}

_bool CEffectManager::Effect_Smoking(void* pArg)
{
    return Execute_Event(EFFECT_SMOKING, pArg);
}

_bool CEffectManager::Effect_SparkStrip(void* pArg)
{
    return Execute_Event(EFFECT_SPARKSTRIP, pArg);
}

_bool CEffectManager::Effect_WoodDebrisSprites(void* pArg)
{
    return Execute_Event(EFFECT_WOODDEBRISSPRITES, pArg);
}

_bool CEffectManager::Effect_BloodMist(void* pArg)
{
    return Execute_Event(EFFECT_BLOOD_MIST, pArg);
}

_bool CEffectManager::Effect_BulletHoles(void* pArg)
{
    return Execute_Event(EFFECT_BULLET_HOLES, pArg);
}

_bool CEffectManager::Effect_Fire_Strip(void* pArg)
{
    return Execute_Event(EFFECT_FIRE_STRIP, pArg);
}



HRESULT CEffectManager::TurnOff_Effect(_uint iEffectType, _uint iEffectIndex)
{
    for (auto& effect : m_vecAvailableEffects[iEffectType])
    {
        if (effect->Get_EffectIndex() == iEffectIndex)
        {
            effect->TurnOff();

            return S_OK;
        }
    }

    return E_FAIL;
}

_bool CEffectManager::Execute_Event(EFFECT_TYPE eEffectType, void* pArg)
{
   
   
    _bool bActiveExist = false;
    for (auto& effect : m_vecAvailableEffects[eEffectType])
    {   
      
        if (effect->Get_Active() == false)
        {
           
            bActiveExist = true;
           

            return effect->OnEvent(eEffectType, pArg);
        }
    }

    if (bActiveExist == false)
    {
        EFFECT_TYPE eEffectTypeBuffer = eEffectType;
       
        CObject_Manager::Get_Instance()->Add_Effect(m_iNumLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect"), &eEffectTypeBuffer);

        return Execute_Event(eEffectType, pArg);
    }

    return true;
}
