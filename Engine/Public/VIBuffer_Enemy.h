#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Enemy : public CVIBuffer
{
protected:
	CVIBuffer_Enemy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Enemy(const CVIBuffer_Enemy& rhs);
	virtual ~CVIBuffer_Enemy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Enemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END