#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Particle.h"

BEGIN(Engine)
class CParticle_Manager :
    public CBase
{

    DECLARE_SINGLETON(CParticle_Manager)
private:
    CParticle_Manager();
    ~CParticle_Manager() = default;
public:
    virtual void Free() override;
public:
    
    void Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
    void Tick(_float fTimeDelta);
    void Late_Tick(_float fTimeDelta);
    void Add_Particle(void* pArg);
    void Add_BloodPool(void* pArg);

    void Create_Prototype(LPDIRECT3DDEVICE9 pGraphic_Device);

private:
    _bool Set_Particle(void* pArg);
    _bool Set_Bloodpool(void* pArg);
    _bool Section_Change(void* pArg);
    //   CEffect*    Find_Effect();

private:
    vector<vector<CParticle*>>* m_vecParticles = nullptr;
    vector<CParticle*> m_vecBloodPool;
    CParticle* m_Prototype_Particle = nullptr;
    LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
    
private:
    _uint m_iActiveCount[PARTICLE_END];


};

END