#pragma once
#include "Effect.h"
class CEffect_Blood_ImpactStrip : public CEffect
{
public:
	CEffect_Blood_ImpactStrip(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect_Blood_ImpactStrip(const CEffect_Blood_ImpactStrip& rhs);
	virtual ~CEffect_Blood_ImpactStrip();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
};

