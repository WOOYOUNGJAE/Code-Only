#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "KeyMgr.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "EventSystem.h"
#include "GameObject.h"
#include "UI_Manager.h"
#include "Picking.h"
#include "Layer.h"
#include "GameObject.h"
#include "EventSystem.h"
#include "Sound_Manager.h"
#include "EffectManager.h"
#include "Particle_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device { CGraphic_Device::Get_Instance() }
	, m_pKey_Manager{ CKeyMgr::Get_Instance() }
	, m_pLevel_Manager { CLevel_Manager::Get_Instance() }
	, m_pObject_Manager { CObject_Manager::Get_Instance() }
	, m_pComponent_Manager { CComponent_Manager::Get_Instance() }
	, m_pTimer_Manager { CTimer_Manager::Get_Instance()}
	, m_pPicking { CPicking::Get_Instance()}
	, m_pEvent_System { CEventSystem::Get_Instance() }
	, m_pUI_Manager{ CUI_Manager::Get_Instance()}
	, m_pSound_Manager{ CSound_Manager::Get_Instance() }
	, m_pEffect_Manager{ CEffectManager::Get_Instance() }
	, m_pParticle_Manager{ CParticle_Manager::Get_Instance()}
{
	Safe_AddRef(m_pEvent_System);
	Safe_AddRef(m_pPicking);	
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pEffect_Manager);
	Safe_AddRef(m_pParticle_Manager);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_GraphicDev(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iSizeX, GraphicDesc.iSizeY, ppOut)))
		return E_FAIL;	

	m_pKey_Manager->Init();

	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;	

	if (FAILED(m_pUI_Manager->Reserve_UI_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pPicking->Initialize(*ppOut, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pSound_Manager->Initialize()))
		return E_FAIL;

	m_pParticle_Manager->Initialize(*ppOut);

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager ||
		nullptr == m_pUI_Manager)
		return;

	m_pKey_Manager->Update();

	m_pObject_Manager->Tick(fTimeDelta);
	m_pUI_Manager->Tick(fTimeDelta);
	m_pLevel_Manager->Tick(fTimeDelta);
	m_pParticle_Manager->Tick(fTimeDelta);
	m_pSound_Manager->Tick(fTimeDelta);

	m_pPicking->Tick();

	m_pObject_Manager->Late_Tick(fTimeDelta);
	m_pUI_Manager->Late_Tick(fTimeDelta);
	m_pLevel_Manager->Late_Tick(fTimeDelta);
	m_pParticle_Manager->Late_Tick(fTimeDelta);
}

void CGameInstance::Key_Input(_uint iLevelIndex)
{
	if (m_pKey_Manager->Key_Down('Z'))
	{
		CLayer* pLayer = m_pObject_Manager->Find_Layer(iLevelIndex, TEXT("Layer_Environment"));
		for (int i = 0; i < pLayer->Size(); i++)
		{
			CGameObject* pGameObject = Get_GameObject(iLevelIndex, TEXT("Layer_Environment"), i);
			pGameObject->SetDebugCollider(!pGameObject->GetDebugCollider());
		}
	}
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
	m_pUI_Manager->Clear(iLevelIndex);

	CRenderer* pRendererCom = (CRenderer*)m_pComponent_Manager->Clone_Component(0, TEXT("Prototype_Component_Renderer"), nullptr);
	pRendererCom->Clear();
	Safe_Release(pRendererCom);
}

HRESULT CGameInstance::Initialize_Event_System(_uint iNumEvents)
{
	if (FAILED(m_pEvent_System->Reserve_Manager(iNumEvents)))
		return E_FAIL;
	

	return S_OK;
}

void CGameInstance::Render_Begin(D3DXCOLOR Color)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin(Color);
}

void CGameInstance::Render_End(void)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End();
}

_bool CGameInstance::Key_Pressing(_int _iKey)
{
	return m_pKey_Manager->Key_Pressing(_iKey);
}

_bool CGameInstance::Key_Down(_int _iKey)
{
	return m_pKey_Manager->Key_Down(_iKey);
}

_bool CGameInstance::Key_Up(_int _iKey)
{
	return m_pKey_Manager->Key_Up(_iKey);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	return m_pLevel_Manager->Open_Level(iLevelIndex, pLevel);	
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComTag, iIndex);	
}

HRESULT CGameInstance::Add_Prototype(const wstring& strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Effect(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Effect(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Section_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const wstring& strSectionTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, strLayerTag, strPrototypeTag, strSectionTag, pArg);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, _uint iObjectIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iObjectIndex);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	return m_pObject_Manager->Delete_GameObject(iLevelIndex, strLayerTag, pGameObject);
}

HRESULT CGameInstance::Change_Section(_uint iLevelIndex, const wstring& strLastSectionTag, const wstring& strNextSectionTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Change_Section(iLevelIndex, strLastSectionTag, strNextSectionTag);
}

HRESULT CGameInstance::Enable_Section(_uint iLevelIndex, const wstring& strSectionTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Enable_Section(iLevelIndex, strSectionTag);
}

HRESULT CGameInstance::Disable_Section(_uint iLevelIndex, const wstring& strSectionTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Disable_Section(iLevelIndex, strSectionTag);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::GetPlayer()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->GetPlayer();
}

void CGameInstance::SetPlayer(CGameObject* _player)
{
	if (nullptr == m_pObject_Manager)
		return;

	return m_pObject_Manager->SetPlayer(_player);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{

	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);	
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

void CGameInstance::Set_Time(const wstring& strTimerTag, _float fTime)
{
	if (nullptr == m_pTimer_Manager)
		return;

	 m_pTimer_Manager->Set_Time(strTimerTag,fTime);
}

void CGameInstance::OnEvent(_uint iEventIndex, void* pArg)
{
	if (nullptr == m_pEvent_System)
	{
		return;
	}

	m_pEvent_System->OnEvent(iEventIndex, pArg);
}

void CGameInstance::Clear_Events()
{
	m_pEvent_System->Clear();
}

HRESULT CGameInstance::Subscribe(_uint iEventIndex, std::function<_bool(void*)> fpCallback)
{
	if (nullptr == m_pEvent_System)
	{
		return E_FAIL;
	}

	return m_pEvent_System->Subscribe(iEventIndex, fpCallback);
}

HRESULT CGameInstance::Add_Prototype_UI(const wstring& strPrototypeUITag, CUI* pUI)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	return m_pUI_Manager->Add_Prototype_UI(strPrototypeUITag, pUI);
}

HRESULT CGameInstance::Add_UI(_uint iLevelIndex, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	return m_pUI_Manager->Add_UI(iLevelIndex, iUITag, strPrototypeUITag, pArg);
}



HRESULT CGameInstance::Set_Render(_uint iLevelIndex, const _uint& iUITag, _bool bIsRender, _int iTexIndex)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;
	RENDERDESC RenderDesc;
	RenderDesc.iUITag = iUITag;
	RenderDesc.bIsRender = bIsRender;
	RenderDesc.iTexIndex = iTexIndex;

	m_pUI_Manager->Set_Render(&RenderDesc);
	return S_OK;
}

HRESULT CGameInstance::Set_Render_Timer(_uint iLevelIndex, const _uint& iUITag, _bool bIsRender, _bool bIsAnimation, _float fDuration, _int iTexIndex)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	RENDERTIMERDESC RenderTimerDesc;

	RenderTimerDesc.iUITag = iUITag;
	RenderTimerDesc.bIsRender = bIsRender;
	RenderTimerDesc.bIsAnimation = bIsAnimation;
	RenderTimerDesc.fDuration = fDuration;
	RenderTimerDesc.iTexIndex = iTexIndex;

	m_pUI_Manager->Set_Render_Timer(&RenderTimerDesc);

	return S_OK;
}

HRESULT CGameInstance::Update_Health(_uint iLevelIndex, const _uint& iHealth, _float fTimeDelta)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	HEALTHDESC HealthDesc;

	HealthDesc.iHealth = iHealth;

	m_pUI_Manager->Update_Health(&HealthDesc);
	return S_OK;
}

HRESULT CGameInstance::Set_PartiallyHidden(_uint iLevelIndex)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pUI_Manager->Set_PartiallyHidden(&iLevelIndex);
	return S_OK;
}

HRESULT CGameInstance::Set_Hidden(_uint iLevelIndex)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pUI_Manager->Set_Hidden(&iLevelIndex);
	return S_OK;
}

HRESULT CGameInstance::Set_Reveal(_uint iLevelIndex)
{
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pUI_Manager->Set_Reveal(&iLevelIndex);
	return S_OK;
}

void CGameInstance::Set_LateTick(_bool bInput)
{
	if (nullptr == m_pUI_Manager)
		return;

	m_pUI_Manager->Set_LateTick(bInput);
	return;
}

HRESULT CGameInstance::Play_Sound(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->PlaySound(pSoundKey, eID, fVolume);
	return S_OK;
}

HRESULT CGameInstance::PlayBGM(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->PlayBGM(pSoundKey, eID, fVolume);
	return S_OK;
}

HRESULT CGameInstance::StopSound(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->StopSound(eID);
	return S_OK;
}

HRESULT CGameInstance::StopBGM(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->StopBGM(eID);
	return S_OK;
}

HRESULT CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->StopAll();
	return S_OK;
}

HRESULT CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->SetChannelVolume(eID, fVolume);
	return S_OK;
}

_bool CGameInstance::IsPlaying(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	return m_pSound_Manager->IsPlaying(eID);
}

HRESULT CGameInstance::Reserve_Manager(_uint iLevelIndex)
{
	if (nullptr == m_pEffect_Manager)
	{
		return E_FAIL;
	}

	return m_pEffect_Manager->Reserve_Manager(iLevelIndex);
}


HRESULT CGameInstance::TurnOff_Effect(_uint iEffectType, _uint iEffectIndex)
{
	if (nullptr == m_pEffect_Manager)
	{
		return E_FAIL;
	}

	return m_pEffect_Manager->TurnOff_Effect(iEffectType, iEffectIndex);
}

void CGameInstance::Add_Particle(void* pArg)
{
	if (nullptr == m_pParticle_Manager)
	{
		return;
	}
	m_pParticle_Manager->Add_Particle(pArg);
	return;
}

void CGameInstance::Add_BloodPool(void* pArg)
{
	if (nullptr == m_pParticle_Manager)
	{
		return;
	}
	m_pParticle_Manager->Add_BloodPool(pArg);
	return;
}

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CUI_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CEffectManager::Get_Instance()->Destroy_Instance();

	CEventSystem::Get_Instance()->Destroy_Instance();

	CParticle_Manager::Get_Instance()->Destroy_Instance();

	CSound_Manager::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pParticle_Manager);
	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pEvent_System);
	Safe_Release(m_pPicking);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pGraphic_Device);
}
