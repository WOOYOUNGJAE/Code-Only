#include "..\Public\KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Init()
{
	// key 정보 초기화
	for (auto& iter : m_key)
		iter = { false, KEY_NONE };
}

void CKeyMgr::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000) // 키가 눌리면
		{
			if (m_key[i].first) // 이전에 눌렸다면
			{
				m_key[i].second = KEY_PRESSING; // 누르고 있는 상태
			}
			else
			{
				m_key[i].first = true;
				m_key[i].second = KEY_DOWN;		// 막 누른 상태
			}
		}
		else // 눌리지 않았다면
		{
			if (m_key[i].first) // 이전에 눌렸다면
			{
				m_key[i].first = false;
				m_key[i].second = KEY_UP;
			}
			else
			{
				m_key[i].second = KEY_NONE;
			}
		}
	}
}

void CKeyMgr::Free()
{
	
}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	if (m_key[_iKey].second == KEY_PRESSING) return true;
	return false;
}

bool CKeyMgr::Key_Down(int _iKey)
{
	if (m_key[_iKey].second == KEY_DOWN) return true;
	return false;
}

bool CKeyMgr::Key_Up(int _iKey)
{
	if (m_key[_iKey].second == KEY_UP) return true;
	return false;
}
