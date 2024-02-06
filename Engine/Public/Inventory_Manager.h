#pragma once
#include "Base.h"

BEGIN(Engine)
class CInventory_Manager : public CBase
{
	DECLARE_SINGLETON(CInventory_Manager);

private:
	CInventory_Manager();
	virtual ~CInventory_Manager() = default;

public:
	HRESULT Reserve_Inventory_Manager(_uint iNumLevels);
	HRESULT Add_Prototype_Inventory(const wstring& strPrototypeUITag, class CUI* pUI);
	HRESULT Add_Inventory(_uint iLevelIndex, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

private:
	map<wstring, class CUI*> m_Prototypes_UI;
};
END
