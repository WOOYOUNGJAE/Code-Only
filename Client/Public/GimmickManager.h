#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CGimmickManager final: public CBase
{
	DECLARE_SINGLETON(CGimmickManager)
private:
	CGimmickManager() = default;
	~CGimmickManager() = default;
public:
	void Free() override;

public:
	_bool Get_EntranceKeyUsed() { return m_bEntranceKeyUsed; }
	_uint Get_PipeGuy_KillCount() { return m_iPipeGuy_KillCount; }
	void Set_EntranceKeyUsed(_bool b) { m_bEntranceKeyUsed = b; }
	void Set_KeyInstance(class CGimmick_Object* pKeyInstance);

	_uint Goon_KillPlus() { return ++m_iGoon_KillCount; }
	_uint PipeGuy_KillPlus() { return ++m_iPipeGuy_KillCount; }
public:
	void DropKey(_float3 vDropPos);
private:
	_bool m_bEntranceKeyUsed = false;
	CGimmick_Object* m_pKeyInstance = nullptr;
	// Kill Count
	_uint m_iGoon_KillCount = 0;
	_uint m_iPipeGuy_KillCount = 0;
};

END