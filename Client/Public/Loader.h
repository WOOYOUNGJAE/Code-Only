#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public: /* Getter */
	const _tchar* Get_LoadingText() const {
		return m_szLoading;
	}

	


public:
	HRESULT Initialize(LEVEL eNextLevel);
	HRESULT LoadingForNextLevel();

public:
	_bool is_Finished() const {
		return m_isFinished;
	}

	_uint Get_Loading()
	{
		return m_uiLoadingIndex;
	}

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_CriticalSection;
	LEVEL					m_eNextLevel = { LEVEL_END };

	_bool					m_isFinished = { false };
	_uint					m_uiLoadingIndex = 0;
	_tchar					m_szLoading[MAX_PATH] = TEXT("");

private:
	HRESULT Loading_ForLogo();
	HRESULT Loading_ForGamePlay();
	HRESULT Loading_ForWYJ();
	
	HRESULT Loading_ForOutdoor();
	HRESULT Loading_For2F();

	HRESULT Loading_Gore_Textures();

	HRESULT Loading_Textures(const std::wstring& strPath, LEVEL eLevel);
public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};

END