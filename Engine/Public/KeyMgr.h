#pragma once
#include "Base.h"

BEGIN(Engine)

class CKeyMgr final :
    public CBase
{
	DECLARE_SINGLETON(CKeyMgr)
public:
	CKeyMgr();
	~CKeyMgr();

public:
	bool Key_Pressing(int _iKey);
	bool Key_Down(int _iKey);
	bool Key_Up(int _iKey);

	void Init();
	void Update();

private:
	pair<bool, KEY_STATE> m_key[VK_MAX];

public:
	virtual void Free() override;
};

END

