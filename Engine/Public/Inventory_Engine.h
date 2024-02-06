#pragma once
#include "Base.h"

BEGIN(Engine)

class CInventory_Engine : public CBase
{
	DECLARE_SINGLETON(CInventory_Engine);
protected:
	CInventory_Engine();
	CInventory_Engine(const CInventory_Engine& rhs);
	virtual ~CInventory_Engine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

public:
	virtual CInventory_Engine* Clone(void* pArg);
	virtual void Free() override;
};
END
