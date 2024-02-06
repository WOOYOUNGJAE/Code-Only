#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CBlendObject abstract : public CGameObject
{
protected:
	CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlendObject(const CBlendObject& rhs);
	virtual ~CBlendObject() = default;

public:
	_float Get_CamDistance() const {
		return m_fCamDistance;
	}

public:
	_bool Get_Active() { return m_bActive; }
	void Set_Active(_bool activeon) { m_bActive = activeon; }
	
	
	/*void Set_EffectIndex(_int index) {
		m_bBloodIndex = index;
	}*/
			
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_uint Get_EffectIndex() { return m_iEffectIndex; }
	virtual void TurnOff() = 0;

public:
	virtual _bool OnEvent(EFFECT_TYPE eEffectType, void* pArg) = 0;

protected:
	_float			m_fCamDistance = { 0.0f };
	_bool			m_bActive = false;
	
	EFFECT_TYPE           m_eEffectType;
	_uint                    m_iEffectIndex;
	
protected:
	void Compute_CamDistance(class CTransform* pTransform);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

