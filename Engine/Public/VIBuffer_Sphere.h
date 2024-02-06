#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
protected:
	CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs);
	virtual ~CVIBuffer_Sphere() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END