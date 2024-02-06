#include "UI_Manager.h"
#include "UI.h"
#include "GameInstance.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CUI_Manager);

CUI_Manager::CUI_Manager()
{

}


HRESULT CUI_Manager::Reserve_UI_Manager(_uint iNumLevels)
{
	if (1 > iNumLevels)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_UI = new UIS[m_iNumLevels];
	m_OldHp = 100;
	m_OldPain = -1;
	//건강한 얼굴의 텍스쳐
	m_OldMikeUITag = UI_HUDBOX_MIKE_HEALTHY;
	m_fAccTime = 0.0f;
	m_fPainTime = 2.0f;
	m_LateTick = true;
	m_fAttackTime = 0.4f;
	m_bDeadTrigger = false;
	m_fDeadAccTime = 0.0f;
	m_fDeltaTime = 0.0f;
	m_fDeadOnceTime = 4.f;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->Subscribe(EVENT_UPDATE_LEVEL, std::bind(&CUI_Manager::Update_Level, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_TOOLTIP_ACTIVE, std::bind(&CUI_Manager::SetActive_Tooltip, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_RADIO_ACTIVE, std::bind(&CUI_Manager::SetActive_Radio, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_OBJECTIVE_COMPLETE_ACTIVE, std::bind(&CUI_Manager::SetActive_Objective, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_HEALTH, std::bind(&CUI_Manager::Update_Health, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_ENEMY_ARROW, std::bind(&CUI_Manager::Set_EnemyArrow, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_RENDER, std::bind(&CUI_Manager::Set_Render, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_RENDER_TIMER, std::bind(&CUI_Manager::Set_Render_Timer, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_PARTIALLY_HIDDEN, std::bind(&CUI_Manager::Set_PartiallyHidden, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_HIDDEN, std::bind(&CUI_Manager::Set_Hidden, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_REVEAL, std::bind(&CUI_Manager::Set_Reveal, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_ENEMY_IN_DISTANCE, std::bind(&CUI_Manager::Set_HitDistance, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_INVEN_SELECT, std::bind(&CUI_Manager::Set_InvenSelect, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_MAP, std::bind(&CUI_Manager::Set_RenderMap, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_LBUTTON_PRESSING, std::bind(&CUI_Manager::Set_Charging, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_LBUTTON_CLICK, std::bind(&CUI_Manager::Set_Attack, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_LBUTTON_UP, std::bind(&CUI_Manager::Cancel_Charging, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_OBJECTIVE, std::bind(&CUI_Manager::Update_Objective, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_OBJECTIVE_COMPLETE, std::bind(&CUI_Manager::Set_Objective_Complete, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_WEAPON_UI, std::bind(&CUI_Manager::Set_WeaponUI, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_ENEMY_HP, std::bind(&CUI_Manager::Set_EnemyHP, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_BULLET_UI, std::bind(&CUI_Manager::Set_Bullet, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_PROMPTER, std::bind(&CUI_Manager::Set_Prompter, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_BUFF, std::bind(&CUI_Manager::Set_Buff, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_LOADING, std::bind(&CUI_Manager::Update_Loading, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_START_OFF, std::bind(&CUI_Manager::Set_Start_Off, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_STAMINA, std::bind(&CUI_Manager::Update_Stamina, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_NOTIFICATION, std::bind(&CUI_Manager::Set_Notification, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_GUARD, std::bind(&CUI_Manager::Update_Guard, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_EAT_HEAL, std::bind(&CUI_Manager::Set_HP_UpEffect, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_UPDATE_INVEN_WEAPON_HP, std::bind(&CUI_Manager::Update_Select_Weapon_HP, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_INVEN_ITEM, std::bind(&CUI_Manager::Set_Inven_Item, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_SET_TALK, std::bind(&CUI_Manager::Set_Talk, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_DEAD, std::bind(&CUI_Manager::Set_Dead, this, std::placeholders::_1));
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CUI_Manager::Add_Prototype_UI(const wstring& strPrototypeUITag, CUI* pUI)
{
	if (nullptr != Find_Prototype_UI(strPrototypeUITag))
	{
		return E_FAIL;
	}

	m_Prototypes_UI.emplace(strPrototypeUITag, pUI);

	return S_OK;
}

HRESULT CUI_Manager::Add_UI(_uint m_uiCurrentLevel, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg)
{
	//이전에 프로토타입에 없으면
	CUI* pPrototypeUI = Find_Prototype_UI(strPrototypeUITag);
	if (nullptr == pPrototypeUI)
		return E_FAIL;
	
	CUI* pUI = pPrototypeUI->Clone(pArg);
	if (nullptr == pUI)
		return E_FAIL;

	if (nullptr != Find_UI(m_uiCurrentLevel, iUITag))
		return E_FAIL;
	m_UI[m_uiCurrentLevel].emplace(iUITag, pUI);

	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
	//맵, 목표달성, 업데이트에 따른 처리가 필요

	m_fDeltaTime = fTimeDelta;

	//강 공격 준비상태인 경우
	if (m_bIsHeavyReady)
	{
		m_fHeavyRenderAcc += fTimeDelta;
		if (m_fHeavyRenderTime < m_fHeavyRenderAcc)
		{
			CUI* pUI_Crosshair = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR);
			CUI* pUI_Crosshair_Attack = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_ATTACK);
			pUI_Crosshair->Set_Render_Timer(false, false, -1, m_fHeavyAttackTime);
			pUI_Crosshair_Attack->Set_Render_Timer(true, true, -1, m_fHeavyAttackTime);
			m_bIsHeavyReady = false;
			m_fHeavyRenderAcc = 0.0f;
		}
	}


	if (true == m_bIsStaminaBuff)
	{
		CUI* pUI_Stamina1 = Find_UI(m_uiCurrentLevel, UI_BUFF_1L_1F_STAMINA);

		if (true == m_bDoubleStamina)
		{
			CUI* pUI_Stamina2 = Find_UI(m_uiCurrentLevel, UI_BUFF_1L_2F_STAMINA);
			if (false == pUI_Stamina1->Get_Timer())
			{
				m_bDoubleStamina = false;
				pUI_Stamina2->Set_Render_Timer(false, false, -1, 0.0f);
				pUI_Stamina1->Set_Render_Timer(true, true, -1, 20.f);
			}
				
		}
		else {
			if (false == pUI_Stamina1->Get_Timer())
				m_bIsStaminaBuff = false;
		}
	}
	if (true == m_bIsHealBuff)
	{
		CUI* pUI_Heal1 = Find_UI(m_uiCurrentLevel, UI_BUFF_2L_1F_HEAL);

		if (true == m_bDoubleHeal)
		{
			CUI* pUI_Heal2 = Find_UI(m_uiCurrentLevel, UI_BUFF_2L_2F_HEAL);
			m_bDoubleHeal = false;
			pUI_Heal2->Set_Render_Timer(false, false, -1, 0.0f);
			pUI_Heal1->Set_Render_Timer(true, true, -1, 5.f);

		}
		else {
			if (false == pUI_Heal1->Get_Timer())
				m_bIsHealBuff = false;
		}
	}
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_UI[i])
		{
			Pair.second->Tick(fTimeDelta);
		}
	}

	if (true == m_bTalkTrigger1)
	{
		CUI* pUI = Find_UI(m_uiCurrentLevel, UI_TEXT);
		if (false == pUI->Get_Timer())
		{
			m_bTalkTrigger1 = false;
			pUI->Set_Render_Timer(true, false, 7, 3.83f);
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			pGameInstance->StopSound(SOUND_PLAYER_TALK);
			pGameInstance->Play_Sound(L"MIKE_MothNFlame_FineFolks_v3.wav", SOUND_PLAYER_TALK, 1.0f);
			Safe_Release(pGameInstance);
		}
	}

	if (false == m_bTalkTrigger1 && true == m_bTalkTrigger2)
	{
		CUI* pUI = Find_UI(m_uiCurrentLevel, UI_TEXT);
		if (false == pUI->Get_Timer())
		{
			m_bTalkTrigger2 = false;
			pUI->Set_Render_Timer(true, false, 8, 7.94f);
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			pGameInstance->StopSound(SOUND_RADIO);
			pGameInstance->Play_Sound(L"NW_MothNFlame_Spite.wav", SOUND_RADIO, 0.7f);
			Safe_Release(pGameInstance);
		}
	}

	if (true == m_bDeadTrigger)
	{
		m_fDeadAccTime += fTimeDelta;
		if (m_fDeadOnceTime < m_fDeadAccTime)
		{
			CUI* pUI = Find_UI(m_uiCurrentLevel, UI_DEADONCE);
			CUI* pUI_LOOP = Find_UI(m_uiCurrentLevel, UI_DEADLOOP);
			pUI_LOOP->Set_Render(true, 0);
			pUI->Set_Render(false, -1);
			m_bDeadTrigger = false;
			m_fDeadAccTime = 0.0f;
		}
		
	}
}

void CUI_Manager::Late_Tick(_float fTimeDelta)
{
	if (true == m_LateTick)
	{
		for (_uint i = 0; i < m_iNumLevels; i++)
		{
			for (auto& Pair : m_UI[i])
			{
				Pair.second->Late_Tick(fTimeDelta);
			}
		}
	}

	
	
}

void CUI_Manager::Clear(_uint m_uiCurrentLevel)
{
	if (m_uiCurrentLevel >= m_iNumLevels)
		return;

	for (auto& Pair : m_UI[m_uiCurrentLevel])
		Safe_Release(Pair.second);

	m_UI[m_uiCurrentLevel].clear();

}

_bool CUI_Manager::SetActive_Tooltip(void* pArg)
{
	TOOLTIP_RADIO_DESC* pTooltipDesc = (TOOLTIP_RADIO_DESC*)pArg;
	//_uint m_uiCurrentLevel, const _uint& iUITag, _float fActiveTime, _int iTexIndex
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_TOOLTIP);
	if (nullptr == pUI)
		MSG_BOX("SetActive_Tooltip_Find_UI_FAILED");
	pUI->SetActive_Unfixed(pTooltipDesc->fActiveTime, pTooltipDesc->iTexIndex);
	return true;
}

_bool CUI_Manager::SetActive_Radio(void* pArg)
{
	//라디오 애니메이션
	TOOLTIP_RADIO_DESC* pRadioDesc = (TOOLTIP_RADIO_DESC*)pArg;
	//_uint m_uiCurrentLevel, const _uint& iUITag, _float fActiveTime, _int iTexIndex
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_RADIOBASE);
	if (nullptr == pUI)
		MSG_BOX("SetActive_Radio_Find_UI_FAILED");
	if(pRadioDesc->iTexIndex!=0 && pRadioDesc->iTexIndex != 5 && pRadioDesc->iTexIndex != 7 && pRadioDesc->iTexIndex != 11 && pRadioDesc->iTexIndex !=14)
		pUI->SetActive_Unfixed(pRadioDesc->fActiveTime,-1);
	
	//라디오에 따른 텍스트 텍스쳐 렌더링
	CUI* pTextUI = Find_UI(m_uiCurrentLevel, UI_TEXT);
	if (nullptr == pUI)
		MSG_BOX("SetActive_Radio_Text_Find_UI_FAILED");
	TOOLTIP_RADIO_DESC talkdesc;
	talkdesc.iTexIndex = pRadioDesc->iTexIndex;
	talkdesc.fActiveTime = pRadioDesc->fActiveTime;
	if (6 == pRadioDesc->iTexIndex)
	{
		m_bTalkTrigger1 = true;
		m_bTalkTrigger2 = true;
	}
	Set_Talk(&talkdesc);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (pRadioDesc->iTexIndex == 1)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_Welcome.wav", SOUND_RADIO, 0.7f);
	}
		
	else if (pRadioDesc->iTexIndex == 2)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_ClocksTicking.wav", SOUND_RADIO, 0.9f);
	}
		
	else if (pRadioDesc->iTexIndex == 3)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_Stealth.wav", SOUND_RADIO, 0.9f);
	}
		
	else if (pRadioDesc->iTexIndex == 4)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_LocalRabble.wav", SOUND_RADIO, 0.7f);
	}
		
	else if (pRadioDesc->iTexIndex == 5)
	{
		pGameInstance->StopSound(SOUND_PLAYER_TALK);
		pGameInstance->Play_Sound(L"MIKE_MothNFlame_BurningDown.wav", SOUND_PLAYER_TALK, 0.7f);
	}

	else if (pRadioDesc->iTexIndex == 6)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_MeanRightHook.wav", SOUND_RADIO, 0.7f);
	}
	
	else if (pRadioDesc->iTexIndex == 9)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_SecurityTight.wav", SOUND_RADIO, 0.7f);
	}
		
	else if (pRadioDesc->iTexIndex == 10)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_EnterClub.wav", SOUND_RADIO, 0.7f);
	}
	
	else if (pRadioDesc->iTexIndex == 11)
	{
		pGameInstance->StopSound(SOUND_PLAYER_TALK);
		pGameInstance->Play_Sound(L"MIKE_MothNFlame_GoingUp.wav", SOUND_PLAYER_TALK, 0.7f);
	}
	else if (pRadioDesc->iTexIndex == 12)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_Floor2.wav", SOUND_RADIO, 0.7f);
	}
		
	else if (pRadioDesc->iTexIndex == 13)
	{
		pGameInstance->StopSound(SOUND_RADIO);
		pGameInstance->Play_Sound(L"NW_MothNFlame_MaloneKilled.wav", SOUND_RADIO, 0.7f);
	}
	else if (pRadioDesc->iTexIndex == 14)
	{
		pGameInstance->StopSound(SOUND_PLAYER_TALK);
		pGameInstance->Play_Sound(L"MIKE_MothNFlame_Pricks.wav", SOUND_PLAYER_TALK, 0.7f);
	}
		
	Safe_Release(pGameInstance);
	return true;
}

_bool CUI_Manager::SetActive_Objective(void* pArg)
{
	OBJECTIVEDESC* pObjectiveDesc = (OBJECTIVEDESC*)pArg;
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_OBJECTIVE_COMPLETE);
	if(nullptr == pUI)
		MSG_BOX("SetActive_Objective_Find_UI_FAILED");
	pUI->SetActive_Unfixed(pObjectiveDesc->fActiveTime, -1);
	return true;
}

_bool CUI_Manager::Set_Render(void* pArg)
{
	RENDERDESC* pRenderDesc = (RENDERDESC*)pArg;
	CUI* pUI = Find_UI(m_uiCurrentLevel, pRenderDesc->iUITag);
	if (nullptr == pUI)
		MSG_BOX("Set_Render_Find_UI_FAILED");
	pUI->Set_Render(pRenderDesc->bIsRender, pRenderDesc->iTexIndex);
	
	return true;
}

_bool CUI_Manager::Set_Render_Timer(void* pArg)
{
	RENDERTIMERDESC* pRenderTimeDesc = (RENDERTIMERDESC*)pArg;

	CUI* pUI = Find_UI(m_uiCurrentLevel, pRenderTimeDesc->iUITag);
	if (nullptr == pUI)
		MSG_BOX("Set_Render_Timer_Find_UI_FAILED");
	pUI->Set_Render_Timer(pRenderTimeDesc->bIsRender, pRenderTimeDesc->bIsAnimation, pRenderTimeDesc->iTexIndex, pRenderTimeDesc->fDuration);

	return true;
}

_bool CUI_Manager::Update_Health(void* pArg)
{
	HEALTHDESC* pHealthDesc = (HEALTHDESC*)pArg;
	_uint iHealth = pHealthDesc->iHealth;
	//체력이 이전과 변하지 않은 경우
	if (iHealth == 100)
	{
		CUI* pUI_Dead = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_DEAD);
		if (true == pUI_Dead->GetIsRender())
		{
			pUI_Dead->Set_Render(false, -1);
		}
		//만약 과거의 얼굴이 건강한 얼굴이 아니라면
		if (m_OldMikeUITag != UI_HUDBOX_MIKE_HEALTHY)
		{
			//과거의 얼굴끄기
			CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Render(FALSE, -1);

			//건강한 얼굴 키기
			pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HEALTHY);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Render(TRUE, 0);

			//과거 얼굴 업데이트
			m_OldMikeUITag = UI_HUDBOX_MIKE_HEALTHY;
		}
	}
	if (iHealth == m_OldHp)
	{
		
		//만약 고통스러운 표정이었다면
		if (-1 != m_OldPain)
		{
			CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			////시간을 추가하고
			//m_fAccTime += m_fDeltaTime;
			////만약 렌더링해야할 시간이 넘었다면.
			//if (m_fPainTime < m_fAccTime)
			//{
			//	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
			//	if (nullptr == pUI)
			//		MSG_BOX("Update_Health_Find_UI_FAILED");
			//	//고통스러워하는 UI를 끈다
			//	pUI->Set_Render(FALSE, -1);
			//	//누적시간도 초기화
			//	m_fAccTime = 0.0f;
			//	m_OldPain = -1;
			//}
		}
		return true;
	}

	//체력 감소
	else if (m_OldHp > iHealth)
	{
		CUI* pUI_Red = Find_UI(m_uiCurrentLevel, UI_HP_DOWN);
		pUI_Red->Set_Render_Timer(true, true, -1, m_fEffectSceneRenderTime);
		//체력 백의자리
		CUI* pUIHPNum0 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM0);
		if (nullptr == pUIHPNum0)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		//체력 십의자리
		CUI* pUIHPNum1 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM1);
		if (nullptr == pUIHPNum1)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		//체력 일의자리
		CUI* pUIHPNum2 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM2);
		if (nullptr == pUIHPNum2)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		//현재 체력이 81~100
		if (100 >= iHealth && 80 < iHealth)
		{
			if (100 == iHealth)
			{
				pUIHPNum0->Set_Render(TRUE, 1);
				pUIHPNum1->Set_Render(TRUE, 0);
				pUIHPNum2->Set_Render(TRUE, 0);
			}

			else {
				pUIHPNum0->Set_Render(TRUE, 0);
				pUIHPNum1->Set_Render(TRUE, iHealth / 10);
				pUIHPNum2->Set_Render(TRUE, iHealth % 10);
			}

			//만약 과거의 얼굴이 건강한 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_HEALTHY)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//건강한 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HEALTHY);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_HEALTHY;
			}
			
			
			//만약 과거의 고통스러운 표정이 지금의 표정과 맞다면 시간만 초기화
			
			if (m_OldPain == 0)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			}

			//만약 고통스러운 표정이 아니었다면
			else if (m_OldPain == -1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 0, m_fPainTime);
				m_OldPain = 0;
			}
			//다른 고통스러운 표정이었다면
			else 
			{
				////이전의 고통스러운 표정 비활성화
				//CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
				//if (nullptr == pUI)
				//	return E_FAIL;
				//pUI->Set_Render(FALSE, -1);

				//현재 체력에 맞는 고통스러운 얼굴 텍스쳐로 변경.
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 0, m_fPainTime);
				m_OldPain = 0;
			}




		}
		else if (80 >= iHealth && 60 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 살짝 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != 5)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//살짝 아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_SLIGHTLYHURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_SLIGHTLYHURT;
			}

			//만약 과거의 고통스러운 표정이 지금의 표정과 맞다면 시간만 초기화
			if (m_OldPain == 1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			}

			//만약 고통스러운 표정이 아니었다면
			else if (m_OldPain == -1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 1, m_fPainTime);
				m_OldPain = 1;
			}
			//다른 고통스러운 표정이었다면
			else
			{
				////이전의 고통스러운 표정 비활성화
				//CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
				//if (nullptr == pUI)
				//	return E_FAIL;
				//pUI->Set_Render(FALSE, -1);

				//현재 체력에 맞는 고통스러운 얼굴 텍스쳐로 변경.
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 1, m_fPainTime);//Set_Render(TRUE, 1);
				m_OldPain = 1;
			}
		}

		else if (60 >= iHealth && 40 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_HURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_HURT;
			}

			//만약 과거의 고통스러운 표정이 지금의 표정과 맞다면 시간만 초기화
			if (m_OldPain == 2)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			}

			//만약 고통스러운 표정이 아니었다면
			else if (m_OldPain == -1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 2, m_fPainTime);
				m_OldPain = 2;
			}
			//다른 고통스러운 표정이었다면
			else
			{
				////이전의 고통스러운 표정 비활성화
				//CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
				//if (nullptr == pUI)
				//	return E_FAIL;
				//pUI->Set_Render(FALSE, -1);

				//현재 체력에 맞는 고통스러운 얼굴 텍스쳐로 변경.
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 2, m_fPainTime);//Set_Render(TRUE, 1);
				m_OldPain = 2;
			}
		}

		else if (40 >= iHealth && 20 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 꽤 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_MODERATELYHURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//꽤 아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_MODERATELYHURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_MODERATELYHURT;
			}

			//만약 과거의 고통스러운 표정이 지금의 표정과 맞다면 시간만 초기화
			if (m_OldPain == 3)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			}

			//만약 고통스러운 표정이 아니었다면
			else if (m_OldPain == -1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(TRUE, false, 3, m_fPainTime);
				m_OldPain = 3;
			}
			//다른 고통스러운 표정이었다면
			else
			{
				////이전의 고통스러운 표정 비활성화
				//CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
				//if (nullptr == pUI)
				//	return E_FAIL;
				//pUI->Set_Render(FALSE, -1);

				//현재 체력에 맞는 고통스러운 얼굴 텍스쳐로 변경.
				CUI* pUI = Find_UI(m_uiCurrentLevel, 9);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 3, m_fPainTime);//Set_Render(TRUE, -1);
				m_OldPain = 3;
			}
		}
		else if (20 >= iHealth && 0 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth/10);
			pUIHPNum2->Set_Render(TRUE, iHealth%10);

			//만약 과거의 얼굴이 매우 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_BADLYHURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//매우 아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_BADLYHURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_BADLYHURT;
			}

			//만약 과거의 고통스러운 표정이 지금의 표정과 맞다면 시간만 초기화
			if (m_OldPain == 4)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				pUI->Set_Render_Timer(true, false, m_OldPain, m_fPainTime);
			}

			//만약 고통스러운 표정이 아니었다면
			else if (m_OldPain == -1)
			{
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(TRUE, false, 4, m_fPainTime);
				m_OldPain = 4;
			}
			//다른 고통스러운 표정이었다면
			else
			{
				////이전의 고통스러운 표정 비활성화
				//CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
				//if (nullptr == pUI)
				//	return E_FAIL;
				//pUI->Set_Render(FALSE, -1);

				//현재 체력에 맞는 고통스러운 얼굴 텍스쳐로 변경.
				CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render_Timer(true, false, 4, m_fPainTime);//Set_Render(TRUE, -1);
				m_OldPain = 4;
			}
		}
		//만약 체력이 0이라면
		else
		{
			
			//m_fAccTime = 0.0f;
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, 0);
			pUIHPNum2->Set_Render(TRUE, 0);
			CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldPain);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Timer(false);
			pUI->Set_Render(FALSE, -1);
			m_OldPain = -1;

			pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Render(FALSE, -1);
			m_OldMikeUITag = 4;

			//죽은 얼굴 텍스쳐
			pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_DEAD);
			if (nullptr == pUI)
				MSG_BOX("Update_Health_Find_UI_FAILED");
			pUI->Set_Render(TRUE, -1);

			//추가 작업 필요
			//화면이 피로 물들면서
			//죽었다는 이미지가 떠야 한다.
			
		}
		m_OldHp = iHealth;
		return true;
	}

	//체력 회복
	else 
	{
		//체력회복 나중에 지울 부분
		CUI* pUI_Green = Find_UI(m_uiCurrentLevel, UI_HP_UP);
		pUI_Green->Set_Render_Timer(true, true, -1, m_fEffectSceneRenderTime);
		//체력 백의자리
		CUI* pUIHPNum0 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM0);
		if (nullptr == pUIHPNum0)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		//체력 십의자리
		CUI* pUIHPNum1 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM1);
		if (nullptr == pUIHPNum1)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		//체력 일의자리
		CUI* pUIHPNum2 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_HP_NUM2);
		if (nullptr == pUIHPNum2)
			MSG_BOX("Update_Health_Find_UI_FAILED");

		////만약 고통스러운 표정이었다면
		//if (-1 != m_OldPain)
		//{
		//	//시간을 추가하고
		//	m_fAccTime += m_fDeltaTime;
		//	//만약 렌더링해야할 시간이 넘었다면.
		//	if (m_fPainTime < m_fAccTime)
		//	{
		//		CUI* pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PAIN);
		//		if (nullptr == pUI)
		//			MSG_BOX("Update_Health_Find_UI_FAILED");
		//		//고통스러워하는 UI를 끈다
		//		pUI->Set_Render(FALSE, -1);
		//		//누적시간도 초기화
		//		m_fAccTime = 0.0f;
		//		m_OldPain = -1;
		//	}
		//}

		//현재 체력이 81~100
		if (100 >= iHealth && 80< iHealth)
		{
			if (100 == iHealth)
			{
				pUIHPNum0->Set_Render(TRUE, 1);
				pUIHPNum1->Set_Render(TRUE, 0);
				pUIHPNum2->Set_Render(TRUE, 0);
			}

			else {
				pUIHPNum0->Set_Render(TRUE, 0);
				pUIHPNum1->Set_Render(TRUE, iHealth / 10);
				pUIHPNum2->Set_Render(TRUE, iHealth % 10);
			}
				
			//만약 과거의 얼굴이 건강한 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_HEALTHY|| 0==m_OldHp )
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//건강한 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HEALTHY);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_HEALTHY;
			}


		}
		else if (80 >= iHealth && 60 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 살짝 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_SLIGHTLYHURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//살짝 아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_SLIGHTLYHURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_SLIGHTLYHURT;
			}
		}

		else if (60 >= iHealth && 40 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_HURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_HURT;
			}
		}

		else if (40 >= iHealth && 20 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, iHealth / 10);
			pUIHPNum2->Set_Render(TRUE, iHealth % 10);

			//만약 과거의 얼굴이 꽤 아픈 얼굴이 아니라면
			if (m_OldMikeUITag != UI_HUDBOX_MIKE_MODERATELYHURT)
			{
				//과거의 얼굴끄기
				CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(FALSE, -1);

				//꽤 아픈 얼굴 키기
				pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_MODERATELYHURT);
				if (nullptr == pUI)
					MSG_BOX("Update_Health_Find_UI_FAILED");
				pUI->Set_Render(TRUE, 0);

				//과거 얼굴 업데이트
				m_OldMikeUITag = UI_HUDBOX_MIKE_MODERATELYHURT;
			}
		}
		else if (20 >= iHealth && 0 < iHealth)
		{
			pUIHPNum0->Set_Render(TRUE, 0);
			pUIHPNum1->Set_Render(TRUE, 0);
			pUIHPNum2->Set_Render(TRUE, iHealth);

			////만약 과거의 얼굴이 매우 아픈 얼굴이 아니라면
			//if (m_OldMikeUITag != ch)
			//{
			//	//과거의 얼굴끄기
			//	CUI* pUI = Find_UI(m_uiCurrentLevel, m_OldMikeUITag);
			//	if (nullptr == pUI)
			//		return E_FAIL;
			//	pUI->Set_Render(FALSE, -1);

			//	//매우 아픈 얼굴 키기
			//	pUI = Find_UI(m_uiCurrentLevel, ch);
			//	if (nullptr == pUI)
			//		return E_FAIL;
			//	pUI->Set_Render(TRUE, 0);

			//	//과거 얼굴 업데이트
			//	m_OldMikeUITag = ch;
			//}
		}
		m_OldHp = iHealth;
		return true;
	}
	return true;
}

_bool CUI_Manager::Set_EnemyArrow(void* pArg)
{
	ENEMYARROWDESC* pEnemyArrowDesc = (ENEMYARROWDESC*)pArg;
	//적의 Transform컴포넌트로 구분
	//아직 등록된 적이 없다면 
	if (m_EnemyArrows.end() == m_EnemyArrows.find(pEnemyArrowDesc->pEnemyTransform))
	{
		//등록된 게 5개보다 적고, 렌더가 true이면
		if (m_EnemyArrows.size() < 5&& true == pEnemyArrowDesc->bIsRender)
		{
			//새로 등록하기
			m_EnemyArrows.emplace(pEnemyArrowDesc->pEnemyTransform, UI_ENEMY_ARROW0 + m_EnemyArrows.size());
			
		}
	}
	//이미 등록된 적인 경우
	else
	{
		if (false == pEnemyArrowDesc->bIsRender)
		{
			CUI* pUI = Find_UI(m_uiCurrentLevel, m_EnemyArrows.find(pEnemyArrowDesc->pEnemyTransform)->second);
			//더이상 그리지 않을 것이기 때문에 바로 리턴
			if (nullptr == pUI)
			{
				return true;
			}
			pUI->Set_Render(false, -1);
			m_EnemyArrows.erase(pEnemyArrowDesc->pEnemyTransform);
			return true;
		}
			
	}
	//만약 5개가 넘어서 추가가 안됐으면 return하기.
	if (m_EnemyArrows.end() == m_EnemyArrows.find(pEnemyArrowDesc->pEnemyTransform))
		return true;
	CUI* pUI = Find_UI(m_uiCurrentLevel, m_EnemyArrows.find(pEnemyArrowDesc->pEnemyTransform)->second);
	if(nullptr == pUI)
		MSG_BOX("EnemyArrow_Find_UI_FAILED");
	pUI->Set_TexIndex((_uint)(pEnemyArrowDesc->fEnemyGauge / 12500.f));
	_float3 vec3PlayerLookAt = pEnemyArrowDesc->pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	vec3PlayerLookAt.y = 0.0f;
	_float3 vec3PlayerPos = pEnemyArrowDesc->pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vec3EnemyPos = pEnemyArrowDesc->pEnemyTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vec3PlayerToEnemy = vec3EnemyPos - vec3PlayerPos;
	vec3PlayerToEnemy.y = 0.0f;
	_float fDotProduct= D3DXVec3Dot(D3DXVec3Normalize(&vec3PlayerLookAt, &vec3PlayerLookAt), D3DXVec3Normalize(&vec3PlayerToEnemy, &vec3PlayerToEnemy));
	if (fDotProduct > 1.f)
		fDotProduct = 1.f;//fDotProduct = 1.f;
	else if (fDotProduct < -1.f)
		fDotProduct = -1.f;//fDotProduct = -1.f;
	_float4x4 MatfPlayerWorldInverse = pEnemyArrowDesc->pPlayerTransform->Get_WorldMatrix_Inverse();
	_float3* EnemyPos = &pEnemyArrowDesc->pEnemyTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vecEnemyPlayerLocal = *D3DXVec3TransformCoord(&vecEnemyPlayerLocal, EnemyPos, &MatfPlayerWorldInverse);

	_float fTheta = acosf(fDotProduct);
	
	_float f360Degree = D3DXToRadian(360);
	if (0<=vecEnemyPlayerLocal.x )
		fTheta = f360Degree-fTheta;		

	if (true == pEnemyArrowDesc->bIsRender)
		pUI->Set_Render(true, -1);
	else if (false == pEnemyArrowDesc->bIsRender)
		pUI->Set_Render(false, -1);
	pUI->Rotate_UI(_float3(0.0f, 0.0f, 1.0f), fTheta);
	pUI->Set_Position(80.0f, fTheta);
	return true;
}

_bool CUI_Manager::Set_HitDistance(void* pArg)
{
	HITDISTANCEDESC* pHitDistanceDesc = (HITDISTANCEDESC*)pArg;
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_INDISTANCE);
	_bool bIsInclude = false;
	if (nullptr == pUI)
		MSG_BOX("HitDistance_Find_UI_FAILED");
	if (true == pHitDistanceDesc->bInDistance)
	{
		for (auto& member : m_InDistEnemy)
		{
			if (member == pHitDistanceDesc->pEnemy)
			{
				bIsInclude = true;
				break;
			}
		}
		if (false == bIsInclude)
		{
			m_InDistEnemy.push_back(pHitDistanceDesc->pEnemy);
		}
	}
	else
	{
		for (auto& iter = m_InDistEnemy.begin(); iter!=m_InDistEnemy.end(); iter++)
		{
			if (*iter == pHitDistanceDesc->pEnemy)
			{
				m_InDistEnemy.erase(iter);
				if (0 == m_InDistEnemy.size())
				{
					pUI->Set_Render(false, 0);
				}
				return true;
			}
		}
	}

	if (0 == m_InDistEnemy.size())
	{
		pUI->Set_Render(false, 0);
	}
	else 
	{
		pUI->Set_Render(true, 0);
	}
		
	return true;
}

_bool CUI_Manager::Set_InvenSelect(void* pArg)
{
	INVENSELECT* pInvenSelect = (INVENSELECT*)pArg;
	CUI* pUI0 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_SELECT0);
	CUI* pUI1 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_SELECT1);
	CUI* pUI2 = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_SELECT2);
	if (UI_HUDBOX_INVEN_SELECT0 == pInvenSelect->iUITag)
	{
		pUI0->Set_Render(true, 0);
		pUI1->Set_Render(false, 0);
		pUI2->Set_Render(false, 0);
	}
	else if (UI_HUDBOX_INVEN_SELECT1 == pInvenSelect->iUITag)
	{
		pUI0->Set_Render(false, 0);
		pUI1->Set_Render(true, 0);
		pUI2->Set_Render(false, 0);
	}
	else if (UI_HUDBOX_INVEN_SELECT2 == pInvenSelect->iUITag)
	{
		pUI0->Set_Render(false, 0);
		pUI1->Set_Render(false, 0);
		pUI2->Set_Render(true, 0);
	}
	return true;
}

_bool CUI_Manager::Set_RenderMap(void* pArg)
{	
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_MAP);
	if (pUI->GetIsRender())
	{
		pUI->Set_Render(false, -1);
		return true;
	}	
	_uint uiTexIndex = 0;
	if (0b00100000 == m_bCurrentState)
		uiTexIndex = 0;
	else if (0b00101001 == m_bCurrentState)
		uiTexIndex = 1;
	else if (0b00111001 == m_bCurrentState)
		uiTexIndex = 2;
	else if (0b00111011 == m_bCurrentState)
		uiTexIndex = 3;
	else if (0b01001001 == m_bCurrentState)
		uiTexIndex = 4;
	else if (0b01011001 == m_bCurrentState)
		uiTexIndex = 5;
	else if (0b01011011 == m_bCurrentState)
		uiTexIndex = 6;
	else if (0b10011001 == m_bCurrentState)
		uiTexIndex = 7;
	else if (0b10011011 == m_bCurrentState)
		uiTexIndex = 8;
	else if (0b10011111 == m_bCurrentState)
		uiTexIndex = 9;

	pUI->Set_Render(true, uiTexIndex);
	return true;
}

_bool CUI_Manager::Set_Charging(void* pArg)
{
	//차징중이면 차징 시작
	//Player에서 차징중이면 시간을 넘겨서 몇초 이상 차징하면 렌더링 변경
	if (m_bIsCharging)
	{
		//플레이어의 시간을 누적
		m_fAccTime_Charging += m_fDeltaTime;
		if (2.0f < m_fAccTime_Charging)
		{
			m_bIsCharging = false;
			m_bIsHeavyReady = true;
			m_uiCurrentLevel = m_uiCurrentLevel;
			CUI* pUI_HeavyReady = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_HEAVY_READY);
			pUI_HeavyReady->Set_Render_Timer(true, false, -1, m_fHeavyRenderTime);
			m_fAccTime_Charging = 0.0f;
		}
	}
	else {
		//차징한 상태가 아닌경우 Charging으로 바꾼다.
		m_bIsCharging = true;
		CUI* pUI_Charging = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_CHARGING);
		CUI* pUI_Attack = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_ATTACK);
		CUI* pUI_Crosshair = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR);
		if (nullptr == pUI_Charging)
			MSG_BOX("Set_Charging_Find_UI_FAILED");
		if (nullptr == pUI_Attack)
			MSG_BOX("Check_Attack_Find_UI_FAILED");
		//차징은 몇초간 띄우고
		pUI_Charging->Set_Render_Timer(true, true, -1, m_fCharingTime);
		//일반공격은 띄우지는 않으나 애니메이션은 실행중
		pUI_Attack->Set_Render_Timer(false, true, -1, m_fCharingTime);
		//크로스헤어는 몇초간 띄우지 않는다.
		pUI_Crosshair->Set_Render_Timer(false, false, -1, m_fCharingTime);
	}

	return true;
}

_bool CUI_Manager::Set_Attack(void* pArg)
{
	
	CUI* pUI_Crosshair = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR);
	CUI * pUI_Crosshair_Attack = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_ATTACK);
	pUI_Crosshair->Set_Render_Timer(false, false, -1, m_fAttackTime);
	pUI_Crosshair_Attack->Set_Render_Timer(true, true, -1, m_fAccTime);

	return true;
}

_bool CUI_Manager::Cancel_Charging(void* pArg)
{
	if (m_bIsCharging)
	{
		CUI* pUI_Crosshair_Attack = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_ATTACK);
		CUI* pUI_Crosshair_Charging = Find_UI(m_uiCurrentLevel, UI_CROSSHAIR_CHARGING);
		pUI_Crosshair_Charging->Set_Timer(false);
		pUI_Crosshair_Charging->Set_Render(false, -1);
		pUI_Crosshair_Attack->Set_Render(true, -1);
		m_bIsCharging = false;
		m_fAccTime_Charging = 0.0f;
	}
	return true;
}

_bool CUI_Manager::Update_Level(void* pArg)
{
	m_uiCurrentLevel = *(_uint*)pArg;
	
	m_bCurrentState = m_bCurrentState & 0b00011111;

	//차후에 Level enum 최신화 작업 필요
	switch (m_uiCurrentLevel)
	{
	//아웃도어
	case 5:
		m_bCurrentState = m_bCurrentState | 0b00100000;
		break;
	//나이트 클럽 1층
	case 6:
		m_bCurrentState = m_bCurrentState | 0b01000000;
		break;
	//나이트 클럽 2층
	case 7:
		m_bCurrentState = m_bCurrentState | 0b10000000;
		break;
	}
	
	return true;
}

_bool CUI_Manager::Update_Objective(void* pArg)
{
	_uint ObjectiveIndex = *(_uint*)pArg;
	if (OBJ_BLACKBOOK == ObjectiveIndex)
		m_bCurrentState = m_bCurrentState | 0b00001000;
	else if (OBJ_DELIA == ObjectiveIndex)
		m_bCurrentState = m_bCurrentState | 0b00010000;
	return true;
}

_bool CUI_Manager::Set_Objective_Complete(void* pArg)
{
	_uint ObjectiveIndex = *(_uint*)pArg;
	if (OBJ_NIGHTCLUB == ObjectiveIndex)
		m_bCurrentState = m_bCurrentState | 0b00000001;
	else if (OBJ_BLACKBOOK == ObjectiveIndex)
		m_bCurrentState = m_bCurrentState | 0b00000010;
	else if (OBJ_DELIA == ObjectiveIndex)
		m_bCurrentState = m_bCurrentState | 0b00000100;
	return true;
}

_bool CUI_Manager::Set_Bullet(void* pArg)
{
	BULLETDESC* pBulletDesc = (BULLETDESC*)pArg;
	_uint uiStartIndex = 0;
	//수정요망 나중에 들어올 값에 따라서 변경
	//권총기준
	if (UI_WEAPON_AUTOPISTOL == pBulletDesc->uiUITag)
	{
		uiStartIndex = UI_PISTOL_BULLET0;
	}
	else if(UI_WEAPON_TOMMYGUN == pBulletDesc->uiUITag)
	{
		uiStartIndex = UI_TOMMYGUN_BULLET0;
	}
	for (_uint i = UI_PISTOL_BULLET0; i <= UI_TOMMYGUN_BULLET49; i++)
	{
		CUI* pUI = Find_UI(m_uiCurrentLevel, i);
		pUI->Set_Render(false, -1);
	}
	if (true == pBulletDesc->bIsRender)
	{
		for (_uint j = 0; j < pBulletDesc->uiBulletCount; j++)
		{
			CUI* pUI = Find_UI(m_uiCurrentLevel, uiStartIndex + j);
			pUI->Set_Render(true, -1);
		}
	}
	return true;
}

_bool CUI_Manager::Set_PartiallyHidden(void* pArg)
{
	//반쯤 숨은 UI
	CUI* pUIPH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PARTIALLYHIDDEN);
	if (nullptr == pUIPH)
		MSG_BOX("PartiallyHidden_Find_UI_FAILED");
	pUIPH->Set_Render(TRUE, -1);

	//완전히 숨은 UI
	CUI* pUIH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HIDDEN);
	if (nullptr == pUIH)
		MSG_BOX("Hidden_Find_UI_FAILED");
	pUIH->Set_Render(FALSE, -1);
	return true;
}

_bool CUI_Manager::Set_Hidden(void* pArg)
{
	//반쯤 숨은 UI
	CUI* pUIPH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PARTIALLYHIDDEN);
	if (nullptr == pUIPH)
		MSG_BOX("PartiallyHidden_Find_UI_FAILED");
	pUIPH->Set_Render(FALSE, -1);

	//완전히 숨은 UI
	CUI* pUIH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HIDDEN);
	if (nullptr == pUIH)
		MSG_BOX("Hidden_Find_UI_FAILED");
	pUIH->Set_Render(TRUE, -1);
	return true;
}

_bool CUI_Manager::Set_Reveal(void* pArg)
{
	//반쯤 숨은 UI
	CUI* pUIPH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_PARTIALLYHIDDEN);
	if (nullptr == pUIPH)
		MSG_BOX("PartiallyHidden_Find_UI_FAILED");
	pUIPH->Set_Render(FALSE, -1);

	//완전히 숨은 UI
	CUI* pUIH = Find_UI(m_uiCurrentLevel, UI_HUDBOX_MIKE_HIDDEN);
	if (nullptr == pUIH)
		MSG_BOX("Hidden_Find_UI_FAILED");
	pUIH->Set_Render(FALSE, -1);
	return true;
}

_bool CUI_Manager::Set_WeaponUI(void* pArg)
{
	WEAPONUIDESC* pWeaponDesc = (WEAPONUIDESC*)pArg;
	CUI* pUI_Weapon = Find_UI(m_uiCurrentLevel, pWeaponDesc->uiUITag);
	CUI* pUI_Weapon_Background = nullptr;
	if (UI_WEAPON_BOTTLE == pWeaponDesc->uiUITag || UI_WEAPON_BOTTLE_BROKEN == pWeaponDesc->uiUITag || UI_WEAPON_LEADPIPE == pWeaponDesc->uiUITag)
	{
		pUI_Weapon_Background = Find_UI(m_uiCurrentLevel, pWeaponDesc->uiUITag + 1);
		pUI_Weapon->Set_Ratio(pWeaponDesc->fWeaponHP);
	}
	if (UI_WEAPON_AUTOPISTOL == pWeaponDesc->uiUITag || UI_WEAPON_TOMMYGUN == pWeaponDesc->uiUITag)
	{
		pUI_Weapon->Set_Ratio(pWeaponDesc->fWeaponHP);
		BULLETDESC BulletDesc;
		BulletDesc.bIsRender = pWeaponDesc->bIsRender;
		BulletDesc.uiBulletCount = pWeaponDesc->uiBulletCount;
		BulletDesc.uiUITag = pWeaponDesc->uiUITag;
		Set_Bullet(&BulletDesc);
	}
	pUI_Weapon->Set_Render(pWeaponDesc->bIsRender, -1);
	if (nullptr != pUI_Weapon_Background)
		pUI_Weapon_Background->Set_Render(pWeaponDesc->bIsRender, -1);
	return true;
}

_bool CUI_Manager::Set_Prompter(void* pArg)
{
	static _uint CurIndex = 0;
	_uint PrompterIndex = *(_uint*)pArg;

	if (CurIndex == PROMPTER_FINISH || CurIndex == PROMPTER_CHOP)
	{
		if (PrompterIndex == PROMPTER_END)
		{
			return true;			
		}
		else if (PrompterIndex == PROMPTER_FINISH_END)
		{
			PrompterIndex = PROMPTER_END;
			CurIndex = PROMPTER_END;
		}
	}
	else
	{
		if (PrompterIndex == PROMPTER_FINISH_END)
			PrompterIndex = PROMPTER_END;

		CurIndex = PrompterIndex;
	}

	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_PROMPTER);
	if (PROMPTER_END == PrompterIndex)
		pUI->Set_Render(false, -1);
	else
		pUI->Set_Render(true, PrompterIndex);
	return true;
}

_bool CUI_Manager::Set_EnemyHP(void* pArg)
{
	ENEMYHPDESC* pEnemyHPDesc = (ENEMYHPDESC*)pArg;
	//체력이 같다면 바로 리턴
	/*if (0==m_OldEnemyHp == pEnemyHPDesc->uiCurrentHP)
		return true;*/
	CUI* pEnemyHPBar = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP);
	CUI* pEnemyHPBarBack = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACK);
	//에너미 enum에 따라서 다른 UI 찾아줘야 한다. 수정요망
	CUI* pEnemyHPBar_Background = nullptr;
	CUI* pEnemyHPBar_Except1 = nullptr;
	CUI* pEnemyHPBar_Except2 = nullptr;
	if (0 == pEnemyHPDesc->uiEnemyTag)
	{
		pEnemyHPBar_Background = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_GOON);
		pEnemyHPBar_Except1 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MOOK);
		pEnemyHPBar_Except2 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MALONE);
	}
	else if (1 == pEnemyHPDesc->uiEnemyTag)
	{
		pEnemyHPBar_Background = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MOOK);
		pEnemyHPBar_Except1 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_GOON);
		pEnemyHPBar_Except2 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MALONE);
	}
	else if (2 == pEnemyHPDesc->uiEnemyTag)
	{
		pEnemyHPBar_Background = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MALONE);
		pEnemyHPBar_Except1 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_GOON);
		pEnemyHPBar_Except2 = Find_UI(m_uiCurrentLevel, UI_ENEMY_HP_BACKGROUND_MOOK);
	}


 	if(0==pEnemyHPDesc->uiCurrentHP)
	{
		/*pEnemyHPBar->Set_Timer(false);
		pEnemyHPBar->Set_Render(false, -1);
		pEnemyHPBarBack->Set_Timer(false);
		pEnemyHPBarBack->Set_Render(false, -1);
		pEnemyHPBar_Background->Set_Timer(false); 
		pEnemyHPBar_Background->Set_Render(false, -1);*/
		m_bIsEnemyDead = true;
		//return true;
	}
	
	else if (true == m_bIsEnemyDead && 0 != pEnemyHPDesc->uiCurrentHP)
	{
		m_bIsEnemyDead = false;
		pEnemyHPBar->Set_Ratio(1.0f);
		pEnemyHPBarBack->Set_Ratio(1.0f);
		
	}
	if (pEnemyHPDesc->uiOldHP != m_OldEnemyHp )
	{
		pEnemyHPBar->Set_Ratio(_float(pEnemyHPDesc->uiOldHP / 100.f));
		pEnemyHPBarBack->Set_Ratio(_float(pEnemyHPDesc->uiOldHP / 100.f));
	}
	if (true == pEnemyHPBar_Except1->GetIsRender())
	{
		pEnemyHPBar_Except1->Set_Timer(false);
		pEnemyHPBar_Except1->Set_Render(false,-1);
	}
	if (true == pEnemyHPBar_Except2->GetIsRender())
	{
		pEnemyHPBar_Except2->Set_Timer(false);
		pEnemyHPBar_Except2->Set_Render(false,-1);
	}
	pEnemyHPBar->Set_Ratio(_float(pEnemyHPDesc->uiCurrentHP / 100.f));
	pEnemyHPBar->Set_Render_Timer(true, false, -1, 2.0f);
	pEnemyHPBarBack->Set_Changed(_float(pEnemyHPDesc->uiCurrentHP / 100.f));
	pEnemyHPBarBack->Set_Render_Timer(true, false, -1, 2.0f);
	pEnemyHPBar_Background->Set_Render_Timer(true, false, -1, 2.0f);

	m_OldEnemyHp = pEnemyHPDesc->uiCurrentHP;


	
	return true;
}

_bool CUI_Manager::Set_Buff(void* pArg)
{
	CUI* pUI_Stamina1 = Find_UI(m_uiCurrentLevel, UI_BUFF_1L_1F_STAMINA);
	CUI* pUI_Stamina2 = Find_UI(m_uiCurrentLevel, UI_BUFF_1L_2F_STAMINA);
	CUI* pUI_Heal1 = Find_UI(m_uiCurrentLevel, UI_BUFF_2L_1F_HEAL);
	CUI* pUI_Heal2 = Find_UI(m_uiCurrentLevel, UI_BUFF_2L_2F_HEAL);

	//스태미나인 경우
	if (false == m_bIsStaminaBuff)
	{
		pUI_Stamina1->Set_Render_Timer(true, true, -1, 20.f);
		m_bIsStaminaBuff = true;
	}
	else
	{
		pUI_Stamina2->Set_Render_Timer(true, false, 0, 20.f);
		m_bDoubleStamina = true;
	}
	if (false == m_bIsHealBuff)
	{
		pUI_Heal1->Set_Render_Timer(true, true, -1, 5.f);
		m_bIsHealBuff = true;
	}
	else
	{
		pUI_Heal2->Set_Render_Timer(true, false, 0, 5.f);
		m_bDoubleHeal = true;
	}
	return true;
}

_bool CUI_Manager::Update_Loading(void* pArg)
{
	LOADINGDESC* pLoadingDesc = (LOADINGDESC*)pArg;
	CUI* pUI_Background = Find_UI(pLoadingDesc->Level_Loading, UI_LOADING_BACKGROUND);
	CUI* pUI_ProgressBar = Find_UI(pLoadingDesc->Level_Loading, UI_LOADING_PROGRESSBAR);

	

	if (0 == pLoadingDesc->Loading_Success_Count)
		pUI_ProgressBar->Set_Ratio(0.0f);
	else
	{
		m_uiLoadingSuccess += pLoadingDesc->Loading_Success_Count;
		pUI_ProgressBar->Set_Changed(_float(m_uiLoadingSuccess) / _float(pLoadingDesc->Loading_All_Count));
	}
	
	pUI_Background->Set_Render(true, -1);
	pUI_ProgressBar->Set_Render(true, -1);
	if (pLoadingDesc->Loading_All_Count == m_uiLoadingSuccess)
		m_uiLoadingSuccess = 0;
	return true;
}

_bool CUI_Manager::Set_Start_Off(void* pArg)
{
	CUI* pUI_Background = Find_UI(*(_uint*)pArg, UI_START_BACKGROUND);
	CUI* pUI_Logo = Find_UI(*(_uint*)pArg, UI_START_LOGO);
	pUI_Background->Set_Render(false, -1);
	pUI_Logo->Set_Render(false, -1);
	return true;
}

_bool CUI_Manager::Update_Stamina(void* pArg)
{
	CUI* pUI_StaminaBar = Find_UI(m_uiCurrentLevel, UI_PLAYER_STAMINA);
	CUI* pUI_BackGround = Find_UI(m_uiCurrentLevel, UI_PLAYER_STAMINA_BACKGROUND);
	pUI_StaminaBar->Set_Ratio(*(_float*)pArg / 100.f);
	pUI_StaminaBar->Set_Render_Timer(true, false, -1, 2.0f);
	pUI_BackGround->Set_Render_Timer(true, false, -1, 2.0f);
	return true;
}

_bool CUI_Manager::Set_Notification(void* pArg)
{
	CUI* pUI_Noti = Find_UI(m_uiCurrentLevel, UI_NOTIFICATION);
	if (nullptr == pUI_Noti)
		return true;
	pUI_Noti->Set_Render_Timer(true, false, *(_uint*)pArg, 3.0f);
	return true;
}

_bool CUI_Manager::Update_Guard(void* pArg)
{
	CUI* pUI_GuardBar = Find_UI(m_uiCurrentLevel, UI_GUARD_BAR);
	CUI* pUI_BackGround = Find_UI(m_uiCurrentLevel, UI_GUARD_BACKGROUND);
	pUI_GuardBar->Set_Ratio((_float)(*(_int*)pArg) / 100.f);
	pUI_GuardBar->Set_Render_Timer(true, false, -1, 2.0f);
	pUI_BackGround->Set_Render_Timer(true, false, -1, 2.0f);
	return true;
}

_bool CUI_Manager::Set_HP_UpEffect(void* pArg)
{
	CUI* pUI_Green = Find_UI(m_uiCurrentLevel, UI_HP_UP);
	pUI_Green->Set_Render_Timer(true, true, -1, m_fEffectSceneRenderTime);
	return true;
}

_bool CUI_Manager::Update_Select_Weapon_HP(void* pArg)
{
	INVENHPDESC* pInvenHPDesc = (INVENHPDESC*)pArg;
	CUI* pUI_Broken = nullptr;
	CUI* pUI_HPBar= nullptr;
	if (UI_HUDBOX_INVEN_SELECT0 == pInvenHPDesc->iUITag)
	{
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN0);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR0);
	}
	else if (UI_HUDBOX_INVEN_SELECT1 == pInvenHPDesc->iUITag)
	{
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN1);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR1);
	}
	else if (UI_HUDBOX_INVEN_SELECT2 == pInvenHPDesc->iUITag)
	{
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN2);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR2);
	}

	pUI_HPBar->Set_Ratio(pInvenHPDesc->fItemHP);
	/*if (0.001f >= pInvenHPDesc->fItemHP)
		pUI_Broken->Set_Render(true, -1);*/
	return true;
}

_bool CUI_Manager::Set_Inven_Item(void* pArg)
{
	INVENITEMDESC* pInvenItemDesc = (INVENITEMDESC*)pArg;
	CUI* pUI = nullptr;
	CUI* pUI_Background = nullptr;
	CUI* pUI_HPBar=nullptr;
	CUI* pUI_Broken = nullptr;
	if (UI_HUDBOX_INVEN_SELECT0 == pInvenItemDesc->iUITag)
	{
		pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_0);
		pUI_Background = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR_BACKGROUND0);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR0);
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN0);

	}
		
	else if (UI_HUDBOX_INVEN_SELECT1 == pInvenItemDesc->iUITag)
	{
		pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_1);
		pUI_Background = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR_BACKGROUND1);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR1);
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN1);
	}
		
	else if (UI_HUDBOX_INVEN_SELECT2 == pInvenItemDesc->iUITag)
	{
		pUI = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_2);
		pUI_Background = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR_BACKGROUND2);
		pUI_HPBar = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_HPBAR2);
		pUI_Broken = Find_UI(m_uiCurrentLevel, UI_HUDBOX_INVEN_BROKEN2);

	}
		
	pUI->Set_Render(pInvenItemDesc->bIsRender, pInvenItemDesc->iTexIndex);
	pUI_Background->Set_Render(pInvenItemDesc->bIsRender, -1);//pInvenItemDesc->iTexIndex);
	pUI_HPBar->Set_Ratio(1.0f);
	pUI_HPBar->Set_Render(pInvenItemDesc->bIsRender, -1);// pInvenItemDesc->iTexIndex);
	if (false == pInvenItemDesc->bIsRender)
	{
		pUI_Broken->Set_Render(pInvenItemDesc->bIsRender, -1);//pInvenItemDesc->iTexIndex);
	}
	return true;
}

_bool CUI_Manager::Set_Talk(void* pArg)
{
	TOOLTIP_RADIO_DESC* pTalkDesc = (TOOLTIP_RADIO_DESC*)pArg;
	CUI* pUI = Find_UI(m_uiCurrentLevel, UI_TEXT);
	pUI->Set_Render_Timer(true, false, pTalkDesc->iTexIndex, pTalkDesc->fActiveTime);
	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (5 == pTalkDesc->iTexIndex)
		pGameInstance->Play_Sound(L"MIKE_MothNFlame_BurningDown.wav", SOUND_PLAYER, 0.3f);
	else if (5 == pTalkDesc->iTexIndex)
		pGameInstance->Play_Sound(L"MIKE_MothNFlame_GoingUp.wav", SOUND_PLAYER, 0.3f);

	Safe_Release(pGameInstance);*/
	return true;
}

_bool CUI_Manager::Set_Dead(void* pArg)
{
	CUI* pUI_Once = Find_UI(m_uiCurrentLevel, UI_DEADONCE);
	if (nullptr == pUI_Once)
		return true;
	CUI* pUI_Loop = Find_UI(m_uiCurrentLevel, UI_DEADLOOP);
	if (nullptr == pUI_Loop)
		return true;
	_bool bRender = *(_bool*)pArg;
	if (true == bRender)
	{
		m_bDeadTrigger = true;
		pUI_Once->Set_Render(true, 0);
		pUI_Loop->Set_Render(false, -1);
	}
	else
	{
		if (true == pUI_Once->Get_Timer())
		{
			pUI_Once->Set_Timer(false);
		}
		pUI_Loop->Set_Render(false, -1);
		pUI_Once->Set_Render(false, -1);
	}
	
	return true;
}

CUI* CUI_Manager::Find_Prototype_UI(const wstring& strPrototypeUITag)
{
	auto iter = m_Prototypes_UI.find(strPrototypeUITag);
	if (iter == m_Prototypes_UI.end())
		return nullptr;
	return iter->second;
}

CUI* CUI_Manager::Find_UI(const _uint& m_uiCurrentLevel, const _uint& iUITag)
{
	auto iter = m_UI[m_uiCurrentLevel].find(iUITag);
	if (iter == m_UI[m_uiCurrentLevel].end())
		return nullptr;
	return iter->second;
}

void CUI_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_UI[i])
		{
			Safe_Release(Pair.second);
		}
		m_UI[i].clear();
	}
	Safe_Delete_Array(m_UI);

	for (auto& Pair : m_Prototypes_UI)
		Safe_Release(Pair.second);

	m_InDistEnemy.clear();
	m_InDistEnemy.shrink_to_fit();
	m_Prototypes_UI.clear();
}
