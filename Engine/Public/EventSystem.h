#pragma once

#include "Base.h"
#include <functional>

/*
	���� �̺�Ʈ ������ ���� Ŭ�����Դϴ�.

	Subscribe �Լ��� ���� Ư�� Event �߻� �� �ݹ� ���� �Լ��� �����մϴ�.
	�ݹ� �Լ��� �ݵ�� ��ȯ���� _bool�̸� �Ű������� void*�� �޴� �ñ״�ó�� �������մϴ�.

	OnEvent �Լ��� ���� Ư�� Event�� ���� �߻���ŵ�ϴ�. �̶� ������ ��� �Լ��� �ݹ� �̺�Ʈ�� �߻��մϴ�.
	void* �� �����͸� �ݹ� �Լ��� �Ű������� ������ �� �ֽ��ϴ�.
*/

BEGIN(Engine)

class CEventSystem final : public CBase
{
	DECLARE_SINGLETON(CEventSystem)
private:
	CEventSystem();
	virtual ~CEventSystem() = default;

public:
	HRESULT Reserve_Manager(_uint iNumEvents);
	HRESULT Subscribe(_uint iEventIndex, std::function<_bool(void*)> fpCallback);

	void OnEvent(_uint iEventIndex, void* pArg = nullptr);

	void Clear();

private:
	_uint									m_iNumEvents = { 0 };
	std::list<std::function<_bool(void*)>>* m_pEvents = { nullptr };
	typedef std::list<std::function<_bool(void*)>> EVENT;

public:
	virtual void Free() override;
};

END