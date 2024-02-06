#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Billboard final : public CVIBuffer
{
protected:
	CVIBuffer_Billboard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Billboard(const CVIBuffer_Billboard& rhs);
	virtual ~CVIBuffer_Billboard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Billboard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END