#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cylinder final : public CVIBuffer
{
protected:
	CVIBuffer_Cylinder(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Cylinder(const CVIBuffer_Cylinder& rhs);
	virtual ~CVIBuffer_Cylinder() = default;

public:
	virtual HRESULT Initialize_Prototype(const _uint iSliceCount, const float fHeight, const float fRadius);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Cylinder* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint iSliceCount, const float fHeight, const float fRadius);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END