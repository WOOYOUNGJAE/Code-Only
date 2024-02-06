#pragma once

#include "Component_Manager.h"
#include "EventSystem.h"
#include "Engine_Defines.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine(_float fTimeDelta);
	void Key_Input(_uint iLevelIndex);
	void Clear(_uint iLevelIndex);

public: /* For. Unility System */
	HRESULT Initialize_Event_System(_uint iNumEvents);

public: /* For.Graphic_Device */
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(void);

public: /* For.Keys_Manager */
	_bool Key_Pressing(_int _iKey);
	_bool Key_Down(_int _iKey);
	_bool Key_Up(_int _iKey);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strComTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Add_Effect(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT Add_Section_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const wstring& strSectionTag, void* pArg = nullptr);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iObjectIndex);
	HRESULT Delete_GameObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject);

	HRESULT Change_Section(_uint iLevelIndex, const wstring& strLastSectionTag, const wstring& strNextSectionTag);
	HRESULT Enable_Section(_uint iLevelIndex, const wstring& strSectionTag);
	HRESULT Disable_Section(_uint iLevelIndex, const wstring& strSectionTag);

	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	CGameObject* GetPlayer();
	void SetPlayer(CGameObject* _player);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const wstring& strTimerTag);
	_float Compute_TimeDelta(const wstring& strTimerTag);
	void Set_Time(const wstring& strTimerTag, _float fTime);

public: /* For. Event_Manager */
	HRESULT Subscribe(_uint iEventIndex, std::function<_bool(void*)> fpCallback);
	void OnEvent(_uint iEventIndex, void* pArg = nullptr);

	void Clear_Events();

public:	/* For.UI_Manager */
	HRESULT Add_Prototype_UI(const wstring& strPrototypeUITag, class CUI* pUI);
	HRESULT Add_UI(_uint iLevelIndex, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg);

	HRESULT Set_Render(_uint iLevelIndex, const _uint& iUITag, _bool bIsRender, _int iTexIndex = -1);
	HRESULT Set_Render_Timer(_uint iLevelIndex, const _uint& iUITag, _bool bIsRender, _bool bIsAnimation, _float fDuration, _int iTexIndex=-1);
	HRESULT Update_Health(_uint iLevelIndex, const _uint& iHealth, _float fTimeDelta);

	HRESULT Set_PartiallyHidden(_uint iLevelIndex);
	HRESULT Set_Hidden(_uint iLevelIndex);
	HRESULT Set_Reveal(_uint iLevelIndex);
	void Set_LateTick(_bool bInput);

public: /* For.Sound_Manager */
	HRESULT Play_Sound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	HRESULT PlayBGM(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	HRESULT StopSound(CHANNELID eID);
	HRESULT StopBGM(CHANNELID eID);
	HRESULT StopAll();
	HRESULT SetChannelVolume(CHANNELID eID, float fVolume);
	_bool	IsPlaying(CHANNELID eID);

public: /* For.Effect_Manager */
	HRESULT Reserve_Manager(_uint iLevelIndex);

	HRESULT TurnOff_Effect(_uint iEffectType, _uint iEffectIndex);


public: /* For.Particle_Manager */
	void Add_Particle(void* pArg);
	void Add_BloodPool(void* pArg);

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CKeyMgr*					m_pKey_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CPicking*					m_pPicking = { nullptr };
	class CUI_Manager*				m_pUI_Manager = { nullptr };
	class CSound_Manager*			m_pSound_Manager = { nullptr };
	class CEventSystem*				m_pEvent_System = { nullptr };
	class CEffectManager*			m_pEffect_Manager = { nullptr };
	class CParticle_Manager*		m_pParticle_Manager = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
};

END