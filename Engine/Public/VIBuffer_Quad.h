#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Quad final : public CVIBuffer
{
protected:
	CVIBuffer_Quad(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Quad(const CVIBuffer_Quad& rhs);
	virtual ~CVIBuffer_Quad() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Quad* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END