#pragma once

#include "Client_Defines.h"
#include "UI.h"


BEGIN(Client)

class CUI_Fixed final : public CUI
{
public:
	enum TRANSFORM { TF_ROTATION, TF_SCALING, TF_ROTATION_SCALING, TF_END};
protected:
	CUI_Fixed(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Fixed(const CUI_Fixed& rhs);
	virtual ~CUI_Fixed() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	static CUI_Fixed* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI* Clone(void* pArg);
	virtual void Free() override;

};

END

