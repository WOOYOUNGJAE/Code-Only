#include "stdafx.h"
#include "Effect_Blood_ImpactStrip.h"

CEffect_Blood_ImpactStrip::CEffect_Blood_ImpactStrip(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CEffect(pGraphic_Device)
{
}

CEffect_Blood_ImpactStrip::CEffect_Blood_ImpactStrip(const CEffect_Blood_ImpactStrip& rhs)
{
}

CEffect_Blood_ImpactStrip::~CEffect_Blood_ImpactStrip()
{
}

HRESULT CEffect_Blood_ImpactStrip::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Blood_ImpactStrip::Initialize(void* pArg)
{
    return S_OK;
}

void CEffect_Blood_ImpactStrip::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CEffect_Blood_ImpactStrip::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);
}

HRESULT CEffect_Blood_ImpactStrip::Render()
{
    return S_OK;
}
