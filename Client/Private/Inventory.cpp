#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Item_Interactive.h"
#include "InteractiveObject.h"
#include "Camera_Player.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Player.h"
//CInventory* CInventory::m_pInstance = nullptr;


CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CInventory::CInventory(const CInventory& rhs)
	: CGameObject(rhs)
{

}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	m_mapInvenList.emplace(INVEN_SLOT1, nullptr);
	m_mapInvenList.emplace(INVEN_SLOT2, nullptr);
	m_mapInvenList.emplace(INVEN_SLOT3, nullptr);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//�κ��丮�� ������ �߰� ���
	pGameInstance->Subscribe(EVENT_ADD_ITEM_IN_INVENTORY,
		std::bind(&CInventory::On_Add_Inven, this, std::placeholders::_1));

	pGameInstance->Subscribe(EVENT_REMOVE_ITEM_IN_INVENTORY,
		std::bind(&CInventory::Remove_Inven, this, std::placeholders::_1));

	/*pGameInstance->Subscribe(EVENT_INVEN_SELECT,f
		std::bind(&CInventory::Remove_Inven, this, std::placeholders::_1));*/

	static_cast<CPlayer*>(pGameInstance->GetPlayer())->SetInventory(this);
	Safe_Release(pGameInstance);


	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	INVENSELECT tInvenSelect;
	//�������
	//tInvenSelect.iLevelIndex = LEVEL_OUTDOOR;



	if (pGameInstance->Key_Down('1'))
	{
		m_iCurSelectedSlot = INVEN_SLOT1;
		tInvenSelect.iUITag = UI_HUDBOX_INVEN_SELECT0;
		tInvenSelect.pInteractiveObj = &*m_mapInvenList.find(INVEN_SLOT1)->second;
		pGameInstance->OnEvent(EVENT_INVEN_SELECT, &tInvenSelect);
	}

	else if (pGameInstance->Key_Down('2'))
	{
		m_iCurSelectedSlot = INVEN_SLOT2;
		tInvenSelect.iUITag = UI_HUDBOX_INVEN_SELECT1;
		tInvenSelect.pInteractiveObj = &*m_mapInvenList.find(INVEN_SLOT2)->second;
		pGameInstance->OnEvent(EVENT_INVEN_SELECT, &tInvenSelect);
	}

	else if (pGameInstance->Key_Down('3'))
	{
		m_iCurSelectedSlot = INVEN_SLOT3;
		tInvenSelect.iUITag = UI_HUDBOX_INVEN_SELECT2;
		tInvenSelect.pInteractiveObj = &*m_mapInvenList.find(INVEN_SLOT3)->second;
		pGameInstance->OnEvent(EVENT_INVEN_SELECT, &tInvenSelect);
	}

	Safe_Release(pGameInstance);

	if (CGameInstance::Get_Instance()->Key_Down('N'))
	{
		EFFECTDESC effect;
		effect.vScale = { 0.5f,0.5f,0.5f };
		effect.vPos = { 2.f,3.f,5.f };

		effect.bActive = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_BLOOD_MIST, &effect);
		testIndex = effect.iIndex;
	}


	if (CGameInstance::Get_Instance()->Key_Down('M'))
	{
		EFFECTDESC effect;
		effect.vScale = { 0.5f,0.5f,0.5f };
		//effect.vPos = {  2, 2, 0.f };
		effect.vPos = { g_iWinSizeX / 2,g_iWinSizeY / 2, 0.f };

		effect.bActive = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_SMOKING, &effect);

		/*EFFECTDESC effect;
		effect.vScale = { 0.5f,0.5f,0.5f };
		effect.vPos = { 2.f,3.f,5.f };

		effect.bActive = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_SMOKING, &effect);*/
	}

	//if (CGameInstance::Get_Instance()->Key_Down('M'))
	//{
	//	EFFECTDESC effect;
	//	effect.vScale = { 0.5f,0.5f,0.5f };
	//	

	//	//effect.bActive = false;
	//	for (_int i = 0; i < 10; ++i)
	//	{
	//		// 랜덤 위치 생성
	//		_float randomX = static_cast<_float>(rand() % 5) + 1.0f; // 1.0f ~ 5.0f
	//		_float randomY = static_cast<_float>(rand() % 5) + 1.0f; // 1.0f ~ 5.0f
	//		_float randomZ = static_cast<_float>(rand() % 5) + 1.0f; // 1.0f ~ 5.0f
	//		effect.vPos = { randomX, randomY, randomZ };

	//		_float fTime = static_cast<_float>(rand() % 1000) / 1000.0f;
	//		// 조건을 만족하는 임의의 시간 조건을 설정
	//		if (fTime > 0.5f)
	//		{
	//			CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_SMOKEIMPACTUP, &effect);
	//			// 이벤트가 발생한 후에는 일정 시간 대기하는 코드가 필요합니다.
	//			// 위에서 언급한 것처럼 실제 게임에서는 게임의 프레임 흐름을 중단시키지 않는 방식으로 구현해야 합니다.
	//		}
	//			
	//	}
	//}
}

void CInventory::Late_Tick(_float fTimeDelta)
{
	
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CInventory");
	}

	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CInventory");
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	/*for (auto it = m_mapInvenList.begin(); it != m_mapInvenList.end(); ++it)
	{
		Safe_Release(it->second);
	}*/

	m_mapInvenList.clear();
}


_bool CInventory::On_Add_Inven(void* pArg)
{
	CGameObject* pGameObject = static_cast<CGameObject*>(pArg);




	if (nullptr != pGameObject)
	{

		m_bStorable = false;
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		ITEM_TYPE itemtype = dynamic_cast<CInteractiveObject*>(pGameObject)->Get_Item_Type();
		_bool storable = true;
		if (false == dynamic_cast<CInteractiveObject*>(pGameObject)->Get_Enable())
			storable = false;

		//_bool storable = dynamic_cast<CInteractiveObject*>(pGameObject)->Is_Storable();
		INVENTORY eInvenItem = MapItemType_For_UI(itemtype);
		
		if (201 <= itemtype)
		{
			
			storable = false;
			static_cast<CInteractiveObject*>(pGameObject)->Set_Enable_Item(false);

		}


		if (true == storable)
		{
			if (m_iInvenSize >= 3)
			{
				m_bInvenFull = true;
			
			}
			else
			{
				Safe_AddRef(pGameObject);
				dynamic_cast<CInteractiveObject*>(pGameObject)->Set_Enable_Item(false);

				m_bInvenFull = false;
				if (m_mapInvenList.find(INVEN_SLOT1)->second == nullptr)
				{
					m_mapInvenList.find(INVEN_SLOT1)->second = pGameObject;
					INVENITEMDESC itemDesc;
					itemDesc.bIsRender = true;
					itemDesc.iUITag= UI_HUDBOX_INVEN_SELECT0;
					itemDesc.iTexIndex = eInvenItem;
					pGameInstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);

					//pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_0, true, eInvenItem);
					m_iCurSelectedSlot = INVEN_SLOT1;
				}
				else if (m_mapInvenList.find(INVEN_SLOT2)->second == nullptr)
				{
					m_mapInvenList.find(INVEN_SLOT2)->second = pGameObject;
					INVENITEMDESC itemDesc;
					itemDesc.bIsRender = true;
					itemDesc.iUITag = UI_HUDBOX_INVEN_SELECT1;
					itemDesc.iTexIndex = eInvenItem;
					pGameInstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);
					//pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_1, true, eInvenItem);
					m_iCurSelectedSlot = INVEN_SLOT2;
				}
				else if (m_mapInvenList.find(INVEN_SLOT3)->second == nullptr)
				{
					m_mapInvenList.find(INVEN_SLOT3)->second = pGameObject;
					INVENITEMDESC itemDesc;
					itemDesc.bIsRender = true;
					itemDesc.iUITag = UI_HUDBOX_INVEN_SELECT2;
					itemDesc.iTexIndex = eInvenItem;
					pGameInstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);
					//pGameInstance->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_2, true, eInvenItem);
					m_iCurSelectedSlot = INVEN_SLOT3;
				}

				m_iInvenSize++;
				Safe_Release(pGameObject);
			}
		}


		Safe_Release(pGameInstance);

	}

	return true;


}

_bool CInventory::Remove_Inven(void* pArg)
{
	CGameInstance* pGameinstance = CGameInstance::Get_Instance();
	switch (m_iCurSelectedSlot)
	{
	case INVEN_SLOT1:
	{
		if (m_mapInvenList.find(INVEN_SLOT1)->second != nullptr)
		{
			INVENITEMDESC itemDesc;
			itemDesc.bIsRender = false;
			itemDesc.iUITag = UI_HUDBOX_INVEN_SELECT0;
			itemDesc.iTexIndex = 0;
			pGameinstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);
			//CGameInstance::Get_Instance()->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_0, false, INVEN_BOLT);
			m_mapInvenList.find(INVEN_SLOT1)->second = nullptr;
			m_iInvenSize--;
		}
	}
	break;
	case INVEN_SLOT2:
	{
		if (m_mapInvenList.find(INVEN_SLOT2)->second != nullptr)
		{
			INVENITEMDESC itemDesc;
			itemDesc.bIsRender = false;
			itemDesc.iUITag = UI_HUDBOX_INVEN_SELECT1;
			itemDesc.iTexIndex = 0;
			pGameinstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);
			//CGameInstance::Get_Instance()->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_1, false, INVEN_BOLT);
			m_mapInvenList.find(INVEN_SLOT2)->second = nullptr;
			m_iInvenSize--;
		}
	}
	break;
	case INVEN_SLOT3:
	{
		if (m_mapInvenList.find(INVEN_SLOT3)->second != nullptr)
		{
			INVENITEMDESC itemDesc;
			itemDesc.bIsRender = false;
			itemDesc.iUITag = UI_HUDBOX_INVEN_SELECT2;
			itemDesc.iTexIndex = 0;
			pGameinstance->OnEvent(EVENT_SET_INVEN_ITEM, &itemDesc);
			//CGameInstance::Get_Instance()->Set_Render(LEVEL_OUTDOOR, UI_HUDBOX_INVEN_2, false, INVEN_BOLT);
			m_mapInvenList.find(INVEN_SLOT3)->second = nullptr;
			m_iInvenSize--;
		}
	}
	break;
	}
	return true;
}


INVENTORY CInventory::MapItemType_For_UI(ITEM_TYPE eItemtype)
{
	switch (eItemtype)
	{
	case ITEM_BOTTLE:
		return INVEN_BOTTLE;
	case ITEM_AUTOMATIC_PISTOL:
		return INVEN_PISTOL;
	case ITEM_TOMMY_GUN:
		return INVEN_TOMMY;
	case ITEM_PIPE:
		return INVEN_PIPE;
	case ITEM_APPLE:
		return INVEN_APPLE;
	case ITEM_BANANA:
		return INVEN_BANANA;
	case ITEM_COLA:
		return INVEN_COLA;
	case ITEM_MEDIKIT:
		return INVEN_MEDKIT;
	case ITEM_BOTTLE_BROKEN:
		return INVEN_BROKEN_BOTTLE;
	default:
		return INVEN_BOLT;  // �⺻������ �����ϰų� ���� ó��
	}

	return INVENTORY();
}


CGameObject* CInventory::Get_CurSelectedItem()
{
	return m_mapInvenList.find(m_iCurSelectedSlot)->second;
}



