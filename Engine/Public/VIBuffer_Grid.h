#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Grid final : public CVIBuffer
{
protected:
	CVIBuffer_Grid(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Grid(const CVIBuffer_Grid& rhs);
	virtual ~CVIBuffer_Grid() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint			m_iNumVerticesX = { 0 };
	_uint			m_iNumVerticesZ = { 0 };

public:
	static CVIBuffer_Grid* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END