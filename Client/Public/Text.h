#pragma once

#include "Client_Defines.h"
#include "Font.h"

BEGIN(Client)
class CText final : public CFont
{
protected:
	CText(LPDIRECT3DDEVICE9 pGraphic_Device);
	CText(const CText& rhs);
	virtual ~CText() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CText* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI* Clone(void* pArg);
	virtual void Free() override;
};

END

