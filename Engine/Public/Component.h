#pragma once

#include "Base.h"

/* ��ü�� ���(�浹, ��, �ؽ���, ����ó��)���μ� ��������� ������Ʈ���� �θ��. */

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