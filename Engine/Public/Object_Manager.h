#pragma once

/* 1. 원형객체들을 보관한다. */
/* 2. 실제사용할 객체를 복제하는 작업을 한다. 복제하여 생성한 객체를 내가 나누고싶은 기준에따라 보관하낟 */
/* 3. 복제된 객체들의 틱과 레이트 틱을 호출한다. */
/* 4. 객체들의 렌더를 호출한다.(x) */

#include "Base.h"

#include <thread>
#include <condition_variable>

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComTag, _uint iIndex);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);
	HRESULT Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const wstring& strSectionTag, void* pArg);
	HRESULT Add_Effect(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);
	HRESULT Add_Particle(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg);

	class CGameObject* Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _int iObjectIndex);
	HRESULT Delete_GameObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	class CLayer* Find_Section(_uint iLevelIndex, const wstring& strSectionTag);

	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

	void Clear(_uint iLevelIndex);

public:
	HRESULT Change_Section(_uint iLevelIndex, const wstring& strLastSectionTag, const wstring& strNextSectionTag);
	
	HRESULT Enable_Section(_uint iLevelIndex, const wstring& strSectionTag);
	HRESULT Disable_Section(_uint iLevelIndex, const wstring& strSectionTag);

private:
	map<wstring, class CGameObject*>		m_Prototypes;

private:
	_uint									m_iNumLevels = { 0 };
	map<wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<wstring, class CLayer*>		LAYERS;

	map<wstring, class CLayer*>*			m_pSections = { nullptr };
	typedef map<wstring, class CLayer*>		SECTIONS;

	CGameObject*							m_pPlayer = { nullptr };

private:
	_bool m_bLevelChanged = { false };
	std::vector<class CLayer*> m_vecLayerBuffer;

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);

public:
	virtual void Free() override;

	CGameObject* GetPlayer() { return m_pPlayer; }
	void SetPlayer(CGameObject* _player) { m_pPlayer = _player; }

private:
	std::thread* m_sectionControlThread;

private:
	std::condition_variable m_sectionChangeEventCV;
	std::mutex m_sectionMutex;
	bool m_bSectionChagneEventOccurred = { false };
	_bool m_bThreadEnable = { true };

	_uint m_iCurrentLevelIndex;

	std::wstring m_strLastSectionTag;
	std::wstring m_strCurrentSectionTag;

	std::wstring m_strStaticSectionTag;

private:
	void SectionControlThread();
};

END