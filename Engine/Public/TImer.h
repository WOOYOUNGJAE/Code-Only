#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	float			Get_Time(void) { 
		return m_fTime; }
	void			Set_Time(_float _f)
	{
		m_fTime = _f;
	}

public:
	HRESULT				Initialize();
	void				Update(void);

private:
	LARGE_INTEGER		m_CurrentTime;
	LARGE_INTEGER		m_OldTime;
	LARGE_INTEGER		m_OriginTime;
	LARGE_INTEGER		m_CpuTick;

	float				m_fTimeDelta;
	_float				m_fTime;

public:
	static CTimer* Create();
	virtual void Free() override;
};

END