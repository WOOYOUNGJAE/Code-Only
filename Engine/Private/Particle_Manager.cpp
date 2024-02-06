#include "Particle_Manager.h"
#include "Particle.h"

IMPLEMENT_SINGLETON(CParticle_Manager)

CParticle_Manager::CParticle_Manager()
{
}

void CParticle_Manager::Free()
{
    //시체, 나무 판자용 벡터
    for (int i = 0; i < PARTICLE_END-1; i++)
    {
        //vector<vector<>>
        for (auto& ParticleSet : m_vecParticles[i])
        {
            for (auto& Particle : ParticleSet)
            {
                Safe_Release(Particle);
            }
            ParticleSet.clear();
            ParticleSet.shrink_to_fit();
        }
        m_vecParticles[i].clear();
        m_vecParticles[i].shrink_to_fit();
    }
    //피웅덩이를 보관하기 위한 벡터
    for (auto& blood : m_vecBloodPool)
    {
        Safe_Release(blood);
    }
    m_vecBloodPool.clear();
    m_vecBloodPool.shrink_to_fit();

    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_Prototype_Particle);
    Safe_Delete_Array(m_vecParticles);
}

void CParticle_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    m_vecParticles = new vector<vector<CParticle*>>[PARTICLE_END-1];
    for (int i = 0; i < PARTICLE_END; i++)
    {
        m_iActiveCount[i] = 0;
    }
    m_pGraphic_Device = pGraphic_Device;
    Safe_AddRef(m_pGraphic_Device);
    Create_Prototype(pGraphic_Device);
    CGameInstance* pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(pGameInstance);
    pGameInstance->Subscribe(EVENT_PARTICLE, std::bind(&CParticle_Manager::Set_Particle, this, std::placeholders::_1));
    pGameInstance->Subscribe(EVENT_BLOODPOOL, std::bind(&CParticle_Manager::Set_Bloodpool, this, std::placeholders::_1));
    pGameInstance->Subscribe(EVENT_CLEAR_PARTICLE, std::bind(&CParticle_Manager::Section_Change, this, std::placeholders::_1));
    srand((unsigned)time(NULL));
    Safe_Release(pGameInstance);
}

void CParticle_Manager::Tick(_float fTimeDelta)
{
    for (int i = 0; i < PARTICLE_END - 1; i++)
    {
        //vector<vector<>>
        for (auto& ParticleSet : m_vecParticles[i])
        {
            for (auto& Particle : ParticleSet)
            {
                Particle->Tick(fTimeDelta);
            }
        }
    }
    //피웅덩이를 보관하기 위한 벡터
    for (auto& blood : m_vecBloodPool)
    {
        blood->Tick(fTimeDelta);
    }
}

void CParticle_Manager::Late_Tick(_float fTimeDelta)
{
    for (int i = 0; i < PARTICLE_END - 1; i++)
    {
        //vector<vector<>>
        for (auto& ParticleSet : m_vecParticles[i])
        {
            for (auto& Particle : ParticleSet)
            {
                Particle->Late_Tick(fTimeDelta);
            }
        }
    }
    //피웅덩이를 보관하기 위한 벡터
    for (auto& blood : m_vecBloodPool)
    {
        blood->Late_Tick(fTimeDelta);
    }
}



void CParticle_Manager::Add_Particle(void* pArg)
{
    PARTICLEDESC* pParticleDesc = (PARTICLEDESC*)pArg;
    CParticle* pParticle = nullptr;
    if(nullptr != m_Prototype_Particle)
        pParticle = m_Prototype_Particle->Clone(pArg);
    if (m_vecParticles[pParticleDesc->ParticleType].size() <= pParticleDesc->iIndex)
    {
        vector<CParticle*> Particles;
        m_vecParticles[pParticleDesc->ParticleType].push_back(Particles);
    }
    m_vecParticles[pParticleDesc->ParticleType][pParticleDesc->iIndex].push_back(pParticle);
    if(PARTICLE_GOON==pParticleDesc->ParticleType || PARTICLE_PIPE == pParticleDesc->ParticleType)
        pParticle->Set_TextureIndex(rand()%21);
    else
        pParticle->Set_TextureIndex(m_vecParticles[pParticleDesc->ParticleType][pParticleDesc->iIndex].size() - 1);
}

void CParticle_Manager::Add_BloodPool(void* pArg)
{
    PARTICLEDESC* pParticleDesc = (PARTICLEDESC*)pArg;
    CParticle* pParticle = nullptr;
    if (nullptr != m_Prototype_Particle)
        pParticle = m_Prototype_Particle->Clone(pArg);
    m_vecBloodPool.push_back(pParticle);
    pParticle->Set_TextureIndex(0);
}

void CParticle_Manager::Create_Prototype(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    m_Prototype_Particle = CParticle::Create(pGraphic_Device);
}

_bool CParticle_Manager::Set_Particle(void* pArg)
{
   
    PARTICLEEVENTDESC* pParticleEvent = (PARTICLEEVENTDESC*)pArg;
    if (m_iActiveCount[pParticleEvent->ParticleType] >= m_vecParticles[pParticleEvent->ParticleType].size())
        return true;

    _uint iParticleSetIndex = m_iActiveCount[pParticleEvent->ParticleType];

    for (auto& particle : m_vecParticles[pParticleEvent->ParticleType][iParticleSetIndex])
        particle->Set_Position(pParticleEvent->targetPos/*, pParticleEvent->fHeight*/);

    m_iActiveCount[pParticleEvent->ParticleType] += 1;
    return true;
}

_bool CParticle_Manager::Set_Bloodpool(void* pArg)
{
    _float3* pPos = (_float3*)pArg;
    if (m_iActiveCount[PARTICLE_BLOOD] >= m_vecParticles[PARTICLE_BLOOD].size())
        return true;

    _uint iParticleSetIndex = m_iActiveCount[PARTICLE_BLOOD];

    m_vecBloodPool[iParticleSetIndex]->Set_Position(*pPos/*, 0.0f*/);

    m_iActiveCount[PARTICLE_BLOOD] += 1;

    return true;
}

_bool CParticle_Manager::Section_Change(void* pArg)
{
    //시체, 나무 판자용 벡터
    for (int i = 0; i < PARTICLE_END - 1; i++)
    {
        //vector<vector<>>
        for (auto& ParticleSet : m_vecParticles[i])
        {
            for (auto& Particle : ParticleSet)
            {
                Particle->Set_Render(false);
            }
        }
    }
    //피웅덩이를 보관하기 위한 벡터
    for (auto& blood : m_vecBloodPool)
    {
        blood->Set_Render(false);
    }
    return true;
}
