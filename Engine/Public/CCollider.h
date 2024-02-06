#pragma once

#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CCollider final :
	public CComponent
{
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
private:
	class CTransform* m_vTargetTransform = nullptr;
	OBBDESC m_ObbDesc = {};
	class CVIBuffer_Cube* m_pBufferCubeCom = { nullptr };
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END