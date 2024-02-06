#include "stdafx.h"
#include "GameInstance.h"
#include "Item_Interactive.h"
#include "Player.h"

#include "ItemDatabase.h"
#include <Inventory.h>

CItem_Interactive::CItem_Interactive(LPDIRECT3DDEVICE9 pGrahic_Device)
	:CInteractiveObject(pGrahic_Device)
	
{
}

CItem_Interactive::CItem_Interactive(const CInteractiveObject& rhs)
	: CInteractiveObject(rhs)
{
}

HRESULT CItem_Interactive::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Interactive::Initialize(void* pArg)
{
	OBJECTDESC* itemdesc = (OBJECTDESC*)pArg;

	HRESULT hr = __super::Initialize(pArg);
	if (FAILED(hr))
		return hr;
	
	//Å¸ÀÔÀ» ¹Þ¾Æ¿È
	m_eItemType = itemdesc->eItemType;

	Interactive_Info& temp = CItemDatabase::Get_Interactive_Info(itemdesc->eItemType);
	if (m_eItemType == ITEM_CARDBOARD_BOX)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_CardboardBoxes_0_Outline"),
			TEXT("Com_Texture_CardboardBoxes_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
			return E_FAIL;
	}

	if (m_eItemType == ITEM_FOUNTAIN)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Fountain"),
			TEXT("Com_Texture_Fountain"), (CComponent**)&m_pTextureCom_Frame)))
			return E_FAIL;
	}

	if (m_eItemType == ITEM_BANANA_PEEL)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BananaPeel_Outline"),
			TEXT("Com_Texture_BananaPeel_Outline"), (CComponent**)&m_pTextureCom_Frame)))
			return E_FAIL;
	}

	if (m_eItemType == ITEM_BOARD)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_WoodDebris"),
			TEXT("Com_Texture_WoodDebris"), (CComponent**)&m_pTextureCom_Dead)))
			return E_FAIL;
	}

	if (m_eItemType == ITEM_COMBOLOCK)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_PadLockChain_Outline"),
			TEXT("Com_Texture_PadLockChain_Outline"), (CComponent**)&m_pTextureCom_Outline)))
			return E_FAIL;
	}
	
	if (m_eItemType == ITEM_BLOODY_HAND_PRINT)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_BloodyHandPrint_Outline"),
			TEXT("Com_Texture_BloodyHandPrint_Outline"), (CComponent**)&m_pTextureCom_Outline)))
			return E_FAIL;
	}

	if (m_eItemType == ITEM_BUTTON)
	{
		if (FAILED(Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Red_Button_0_Outline"),
			TEXT("Com_Texture_Red_Button_0_Outline"), (CComponent**)&m_pTextureCom_Outline)))
			return E_FAIL;
	}

	//m_bAlive = temp.bAlive;
	m_bBlock = temp.bBlock;

	
	return S_OK;
}

void CItem_Interactive::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (true == m_bEnable )
	{
		__super::Tick(fTimeDelta);

		if (m_eItemType == ITEM_FOUNTAIN)
		{
			m_fFrame += 10.0f * fTimeDelta;

			if (m_fFrame >= 10.0f)
			{
				m_fFrame = 0.f;	
			}
		}
		////º¸µå°¡ µé¾î¿Ô´Ù
		//if (ITEM_BOARD == m_eItemType)
		//{
		//	// Ä«¸Þ¶óÀÇ À§Ä¡¿Í crosshairÀÇ ¹æÇâÀ» ÀÌ¿ëÇØ ·¹ÀÌ¸¦ ±¸¼º
		//	_float3 rayPos = { 0.f, 0.f, 0.f };
		//	_float3 rayDir = { 0.f, 0.f, 1.f };
		//	_float3 outPoint;

		//	_float4x4		CameraWorldMatrix;

		//	_float3	vTarget = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		//	//Ä«¸Þ¶óÀÇ ¿ùµåÇà·Ä °¡Á®¿È
		//	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
		//	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		//	//Ä«¸Þ¶óÀÇ ¿øÁ¡
		//	rayPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		//	//Ä«¸Þ¶óÀÇ ·èº¤ÅÍ
		//	rayDir = { CameraWorldMatrix._31, CameraWorldMatrix._32, CameraWorldMatrix._33 };

		//	if (IsPicked(m_pTransformCom->Get_WorldMatrix(), rayDir, rayPos, &outPoint))
		//	{
		//		_float3 vDistVector = outPoint - rayPos;
		//		_float fDist = D3DXVec3Length(&vDistVector);
		//		//±³Â÷Á¡°ú Ä«¸Þ¶ó¿øÁ¡°úÀÇ °Å¸®°¡ 2ÀÌÇÏÀÏ °æ¿ì
		//		if (fDist < 2.f)
		//		{
		//			//ÇÃ·¹ÀÌ¾î°¡ ¶§¸®¸é ºÎ¼­Á®¼­ ¾ø¾îÁü
		//			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		//			{
		//				m_bEnable = false;
		//				pGameInstance->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		//				pGameInstance->OnEvent(EVENT_EFFECT_POW, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		//			}

		//				//ÇÃ·¹ÀÌ¾î¿Í ¸ó½ºÅÍ¸¦ ¸·´Â´Ù
		//				//else if ()
		//			
		//		}
		//	}
		//	if (GetAsyncKeyState('E') & 0x8000)
		//	{
		//		
		//	}
		//}	
	}

	if (false == m_bEnable)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		

		if (true == m_bBroken)
		{
			if (ITEM_BOARD == m_eItemType)
			{
				pGameInstance->StopSound(SOUND_ITEM);
				pGameInstance->Play_Sound(L"WoodHitGeneric1.wav", SOUND_ITEM, 0.3f);

				EFFECTDESC effect;
				_float3 vBoardPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				effect.vPos = vBoardPos;
				effect.vScale = { 0.2f,0.2f,0.2f };
				pGameInstance->OnEvent(EVENT_EFFECT_GROSSDEBRIS, &effect);

				//임시 주석

				PARTICLEEVENTDESC particle;
				particle.ParticleType = PARTICLE_BOARD;
				particle.targetPos = vBoardPos;
				pGameInstance->OnEvent(EVENT_PARTICLE, &particle);
				//pGameInstance->OnEvent(EVENT_EFFECT_SMOKEIMPACTUP, &effect);

			
				effect.vPos = { vBoardPos.x - 0.5f, vBoardPos.y, vBoardPos.z };
				effect.vScale = { 0.2f,0.2f,0.2f };
				pGameInstance->OnEvent(EVENT_EFFECT_GROSSDEBRIS, &effect);
				/*particle.targetPos = { vBoardPos.x - 0.5f, vBoardPos.y, vBoardPos.z };
				pGameInstance->OnEvent(EVENT_PARTICLE, &particle);*/
				//pGameInstance->OnEvent(EVENT_EFFECT_SMOKEIMPACTUP, &effect);

				effect.vPos = { vBoardPos.x + 0.5f, vBoardPos.y, vBoardPos.z };
				effect.vScale = { 0.2f,0.2f,0.2f };
				pGameInstance->OnEvent(EVENT_EFFECT_GROSSDEBRIS, &effect);
				/*particle.targetPos = { vBoardPos.x + 0.5f, vBoardPos.y, vBoardPos.z };
				pGameInstance->OnEvent(EVENT_PARTICLE, &particle);*/
				//pGameInstance->OnEvent(EVENT_EFFECT_SMOKEIMPACTUP, &effect);


				
				m_bBroken = false;
				
			}
			else if (ITEM_CARDBOARD_BOX == m_eItemType)
			{
				pGameInstance->StopSound(SOUND_ITEM);
				pGameInstance->Play_Sound(L"CardboardBreak.wav", SOUND_ITEM, 0.3f);

				

				m_bBroken = false;

			}
			else if (ITEM_BANANA_PEEL == m_eItemType)
			{
				pGameInstance->StopSound(SOUND_ITEM);
				pGameInstance->Play_Sound(L"GrossPropBreak1.wav", SOUND_ITEM, 0.3f);



				m_bBroken = false;

			}
			else if(ITEM_COMBOLOCK)
			{
				pGameInstance->StopSound(SOUND_ITEM);
				pGameInstance->Play_Sound(L"DoorUnlock.wav", SOUND_ITEM, 1.f);



				m_bBroken = false;
			}

			else 
			{
				pGameInstance->StopSound(SOUND_ITEM);
				pGameInstance->Play_Sound(L"GrossPropBreak1.wav", SOUND_ITEM, 0.3f);



				m_bBroken = false;
			}
		}
	
		Safe_Release(pGameInstance);
		
	}
	Safe_Release(pGameInstance);
}

void CItem_Interactive::Late_Tick(_float fTimeDelta)
{
	if (m_bEnable)
	{
		__super::Late_Tick(fTimeDelta);
	}
	else
		__super::Late_Tick(fTimeDelta);
}

HRESULT CItem_Interactive::Render()
{
	if (m_bEnable)
	{
		if (true == m_bPrompt)
		{
			if (nullptr == m_pVIBufferCom)
				return E_FAIL;

			// m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

			if (m_eItemType == ITEM_CARDBOARD_BOX)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				/* ÀåÄ¡¿¡ ÅØ½ºÃÄ¸¦ ¹ÙÀÎµùÇÑ´Ù. */
				if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)))
					return E_FAIL;

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			
			else if (m_eItemType == ITEM_FOUNTAIN)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				if (FAILED(m_pTextureCom_Frame->Bind_OnGraphicDevice((_uint)m_fFrame)));

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			else if (m_eItemType == ITEM_BANANA_PEEL)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				if (FAILED(m_pTextureCom_Frame->Bind_OnGraphicDevice((_uint)m_fFrame)));

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			else if (m_eItemType == ITEM_COMBOLOCK)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)));

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			else if (m_eItemType == ITEM_BLOODY_HAND_PRINT)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)));

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			else if (m_eItemType == ITEM_BUTTON)
			{
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

				if (FAILED(m_pTextureCom_Outline->Bind_OnGraphicDevice(0)));

				if (FAILED(__super::SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(__super::Release_RenderState()))
					return E_FAIL;
			}

			else
				__super::Render();	//¾Æ¿ô¶óÀÎ ¾ø´Â¾ÖµéÀº ±×³É ºÎ¸ð²¨ ½á¶ó
		}
		else if (m_eItemType == ITEM_FOUNTAIN)
		{
			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

			if (FAILED(m_pTextureCom_Frame->Bind_OnGraphicDevice((_uint)m_fFrame)));

			if (FAILED(__super::SetUp_RenderState()))
				return E_FAIL;

			m_pVIBufferCom->Render();

			if (FAILED(__super::Release_RenderState()))
				return E_FAIL;
		}
		
		else
		{
			__super::Render();	//ÇÁ·ÒÆ÷Æ®¿¡ ¾ÈÀâÇûÀ»¶© ±×³É ºÎ¸ð²¨ ½á¶ó
		}
		
	}

	else
	{
		if(ITEM_BOARD == m_eItemType)
		{
			if (nullptr == m_pVIBufferCom)
				return E_FAIL;

			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());

			/* ÀåÄ¡¿¡ ÅØ½ºÃÄ¸¦ ¹ÙÀÎµùÇÑ´Ù. */
			if (FAILED(m_pTextureCom_Dead->Bind_OnGraphicDevice(0)))
				return E_FAIL;

			if (FAILED(__super::SetUp_RenderState()))
				return E_FAIL;

			m_pVIBufferCom->Render();

			if (FAILED(__super::Release_RenderState()))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CItem_Interactive::Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType)
{
	__super::Add_Components(strTextureTag, eMeshType);

	
	return S_OK;
}

_bool CItem_Interactive::IsBlocking(void* pArg)
{

	m_bBlock = !m_bBlock;
	/*if (false == m_bBlock)
		return false;*/


	return true; //µ¿ÁØ Å×½ºÆ®
}

_bool CItem_Interactive::OnItemPicked(void* pArg)
{
	return _bool();
}

CItem_Interactive* CItem_Interactive::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Interactive* pInstance = new CItem_Interactive(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CItem_Interactive");
	}

	return pInstance;
}

CGameObject* CItem_Interactive::Clone(void* pArg)
{
	CItem_Interactive* pInstance = new CItem_Interactive(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CIntem_Interactive");
	}

	return pInstance;
}

void CItem_Interactive::Free()
{

	__super::Free();

	Safe_Release(m_pTextureCom_Frame);
	//Safe_Release(m_pTransformCom);
	//Safe_Release(m_pTextureCom);
	//Safe_Release(m_pRendererCom);
	//Safe_Release(m_pVIBufferCom);
}
