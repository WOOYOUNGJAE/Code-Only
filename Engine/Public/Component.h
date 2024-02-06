#pragma once

#include "Base.h"

/* 객체의 기능(충돌, 모델, 텍스쳐, 상태처리)으로서 만들어지는 컴포넌트들의 부모다. */

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	_bool						m_isCloned = { false };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END