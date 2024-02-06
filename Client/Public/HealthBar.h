#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CHealthBar final : public CUI
{
public:
	typedef struct tagHealthBarDesc
	{
		_float			fRecoverSpeed;
		_float			fConsumptionSpeed;
		CUI::UIDESC		UIDesc;
	}HEALTHBARDESC;
protected:
	CHealthBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHealthBar(const CHealthBar& rhs);
	virtual ~CHealthBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fRecoverSpeed;
	_float m_fConsumptionSpeed;

public:
	static CHealthBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END