#pragma once

#include "Base.h"
#include <functional>

/*
	편한 이벤트 관리를 위한 클래스입니다.

	Subscribe 함수를 통해 특정 Event 발생 시 콜백 받을 함수를 구독합니다.
	콜백 함수는 반드시 반환형은 _bool이며 매개변수로 void*를 받는 시그니처를 가져야합니다.

	OnEvent 함수를 통해 특정 Event를 직접 발생시킵니다. 이때 구독된 모든 함수에 콜백 이벤트가 발생합니다.
	void* 형 데이터를 콜백 함수의 매개변수로 전달할 수 있습니다.
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