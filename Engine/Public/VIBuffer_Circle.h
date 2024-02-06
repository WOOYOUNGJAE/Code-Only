#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Circle final : public CVIBuffer
{
protected:
	CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Circle(const CVIBuffer_Circle& rhs);
	virtual ~CVIBuffer_Circle() = default;

public:
	virtual HRESULT Initialize_Prototype(const _uint iSliceCount);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Circle* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint iSliceCount);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END