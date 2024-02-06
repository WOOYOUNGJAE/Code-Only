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
	// key ���� �ʱ�ȭ
	for (auto& iter : m_key)
		iter = { false, KEY_NONE };
}

void CKeyMgr::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000) // Ű�� ������
		{
			if (m_key[i].first) // ������ ���ȴٸ�
			{
				m_key[i].second = KEY_PRESSING; // ������ �ִ� ����
			}
			else
			{
				m_key[i].first = true;
				m_key[i].second = KEY_DOWN;		// �� ���� ����
			}
		}
		else // ������ �ʾҴٸ�
		{
			if (m_key[i].first) // ������ ���ȴٸ�
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
