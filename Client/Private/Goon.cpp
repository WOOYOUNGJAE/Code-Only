#include "stdafx.h"
#include "Goon.h"
#include <GameInstance.h>
#include "AwarenessGauge.h"
#include "EnemyPhysics.h"
#include "Enemy_HealthSystem.h"
#include "State_Idle.h"

CGoon* CGoon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGoon* pInstance = new CGoon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CGoon");
	}

	return pInstance;
}

CGameObject* CGoon::Clone(void* pArg)
{
	CGameObject* pInstance = new CGoon(*this);

	if (FAILED(dynamic_cast<CEnemyBase*>(pInstance)->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CGoon");
	}

	return pInstance;
}

HRESULT CGoon::Initialize_Prototype()
{
	return CEnemyBase::Initialize_Prototype();
}

HRESULT CGoon::Initialize(void* pArg)
{
	m_eEnemy_Type = ENEMY_TYPE_GOON;
	// 임시 캐릭 속성 수치 입력
	// 
	//  서 FSM에 자신만의 State 푸쉬
	if (FAILED(CEnemyBase::Initialize(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(Add_Components()))
	{
		return E_FAIL;
	}
	// 자식만의 State 푸쉬
	if (FAILED(Add_StateOnFSM()))
	{
		return E_FAIL;
	}

	// 애니메이션 별로 최대 프레임 설정
	Set_AnimProperty();
	//m_iAnimFrameMaxArr = { 1, 1, 1, 8, 6, 1 };
	return S_OK;
}

void CGoon::Tick(_float fTimeDelta)
{
	CEnemyBase::Tick(fTimeDelta);

	if (m_bTalkingEnemy == true && m_fDist_BtwTarget < 7.f)
	{
		m_bTalkingEnemy = false;
		m_bIsTalking = true;
		// 중얼거림
		m_pGameInstance->StopSound(static_cast<CHANNELID>(m_iSound_Channel));
		_tchar szSoundKey[MAX_PATH] = TEXT("");
		lstrcpy(szSoundKey, m_strCautiousSoundKey.c_str());
		m_pGameInstance->Play_Sound(szSoundKey, static_cast<CHANNELID>(m_iSound_Channel), 0.7f);
	}

	// Temp
	_int pNextState = m_pFSMCom->Tick(fTimeDelta);

	m_pTransformCom->SetNextPos(m_pTransformCom->GetNextPos() + m_pPhysics->Tick(fTimeDelta));

	if (pNextState != FSM_NO_EVENT) // 다음 스테이트 정보가 비지 않았다면
	{
		//m_pFSMCom->Set_NextState(*static_cast<_uint*>(pNextState));
		//m_pFSMCom->Set_NextState(ENEMY_BASE_STATE::IDLE);
	}

	// Animation Frame
	if (m_bIsTalking)
	{
		m_fCurBubbleFrame += 3 * fTimeDelta;
		if (m_fCurBubbleFrame >= 4)
		{
			m_fCurBubbleFrame = 0.f;
		}		
	}

}

void CGoon::Late_Tick(_float fTimeDelta)
{
	CEnemyBase::Late_Tick(fTimeDelta);
}

HRESULT CGoon::Render()
{
	if (m_bHit_byTrain == true)
		return S_OK;

	if (m_bExecuting) // 처형 씬
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	Make_LookCam(); // BillBoard, TextureMat 전용


#pragma region Render Character

	_float4x4 finalMat;
	memcpy(&m_TextureMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION), sizeof(_float3));
	finalMat = m_TextureMat;

	// 텍스쳐 스케일 조정
	_float4x4 matScaling;
	_float fSizeX = m_bRegularSize ? 1.f : m_fSizeX;
	_float fSizeY = m_bRegularSize ? 1.f : m_fSizeY;
	D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
	finalMat = matScaling * finalMat; // S R T 순서라 matScaling먼저

	// Texture Matrix
	if (m_bRegularSize == false)
	{
		memcpy(&finalMat.m[CTransform::STATE_POSITION][0], m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			sizeof(_float3));
	}
	else // 정사각형
	{
		if (m_bIsChopped == true)
		{
			D3DXMatrixIdentity(&matScaling);
			fSizeX = 1.6f;
			fSizeY = 1.6f;
			D3DXMatrixScaling(&matScaling, fSizeX, fSizeY, fSizeX);
			finalMat = matScaling * finalMat; // S R T 순서라 matScaling먼저
			finalMat.m[3][0] += 0.2f;
			finalMat.m[3][1] += 0.2f;
			finalMat.m[3][2] += 0.2f;
		}
		_float3 vLandPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0, -0.4f, 0};
		memcpy(&finalMat.m[CTransform::STATE_POSITION][0], &vLandPos, sizeof(_float3));
	}

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &finalMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(Get_CurTexture(m_eTextureDir, m_iCurAction)->Bind_OnGraphicDevice(static_cast<_uint>(m_fCurFrame))))
		return E_FAIL;

	m_pVIBufferCom->SetWorldMatrix(finalMat);
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pVIBufferCom->SetViewMatrix(m_ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	m_pVIBufferCom->SetProjectionMatrix(m_ProjMatrix);
	m_pVIBufferCom->Set_Texture(Get_CurTexture(m_eTextureDir, m_iCurAction)->Get_Texture(static_cast<_uint>(m_fCurFrame)));

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
#pragma endregion Render Character

#pragma region Render AwarenessIcon
	if (m_bIsAlive == false || m_fDist_BtwTarget > 6.f || m_pGauge->Is_Acting() == false)
		return S_OK;

	if (m_pVIBufferCom_Awareness == nullptr)
		return E_FAIL;
	
	_float4x4 matScaling0{};
	if (m_bIsTalking == true)
	{
		D3DXMatrixScaling(&matScaling0, m_fBubbleIconSizeX, m_fBubbleIconSizeY, m_fBubbleIconSizeX);
	}
	else
	{
		D3DXMatrixScaling(&matScaling0, m_fIconSizeX, m_fIconSizeY, m_fIconSizeX);
	}
	m_IconMat = matScaling0 * m_IconMat; // S R T 순서라 matScaling먼저

	
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_IconMat);

	/* 장치에 텍스쳐를 바인딩한다. */
	if (m_bIsTalking && m_pFSMCom->Is_State(ENEMY_BASE_STATE::IDLE))
	{
		if (FAILED(m_SpeechBubble->Bind_OnGraphicDevice(static_cast<_int>(m_fCurBubbleFrame))))
		{
			return E_FAIL;
		}
	}
	else if (m_pFSMCom->Is_State(AWARE))
	{
		if (FAILED(m_AwarenessIcon_Alert->Bind_OnGraphicDevice(0)))
		{
			return E_FAIL;
		}
	}
	else if (m_pFSMCom->Is_State(CAUTIOUS))
	{
		if (FAILED(m_AwarenessIcon_Cautious->Bind_OnGraphicDevice(m_pGauge->Get_TextureFrame())))
		{
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(m_AwarenessIcon_Spooked->Bind_OnGraphicDevice(m_pGauge->Get_TextureFrame())))
		{
			return E_FAIL;
		}
	}

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom_Awareness->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
#pragma endregion Render AwarenessIcon
	return S_OK;
}

void CGoon::Free()
{
	CEnemyBase::Free();

	for (int i = 0; i < ENEMY_TEXTURE_DIR_END; ++i)
	{
		Safe_Release(m_pTextureComArr[i]);
	}
	for (int i = 0; i < ONLY_SOUTH_ACTION_END; ++i)
	{
		Safe_Release(m_OnlySouthTexturesArr[i]);
	}

	for (int i = 0; i < ENEMY_TEXTURE_DIR_END; ++i)
	{
		for (int j = 0; j < EIGHT_DIR_ACTION_END; ++j)
		{
			Safe_Release(m_EightDirTexturesArr[i][j]);			
		}
	}

	Safe_Release(m_SpeechBubble);
}

HRESULT CGoon::Add_Components()
{
	// 자식만의 Com만
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_North"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// Texture Arr Test
#pragma region Legacy
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_North"),
		TEXT("Com_Texture_North"), (CComponent**)&m_pTextureComArr[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthEast"),
		TEXT("Com_Texture_NorthEast"), (CComponent**)&m_pTextureComArr[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthWest"),
		TEXT("Com_Texture_NorthWest"), (CComponent**)&m_pTextureComArr[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_East"),
		TEXT("Com_Texture_East"), (CComponent**)&m_pTextureComArr[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_West"),
		TEXT("Com_Texture_West"), (CComponent**)&m_pTextureComArr[4])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_South"),
		TEXT("Com_Texture_South"), (CComponent**)&m_pTextureComArr[5])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthEast"),
		TEXT("Com_Texture_SouthEast"), (CComponent**)&m_pTextureComArr[6])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthWest"),
		TEXT("Com_Texture_SouthWest"), (CComponent**)&m_pTextureComArr[7])))
		return E_FAIL; 
#pragma endregion Legacy

#pragma region Renewed
	// South
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_South"),
		TEXT("Com_Texture_Stand_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_South"),
		TEXT("Com_Texture_Strip_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][WALK])))
		return E_FAIL;	
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][RUN])))
		return E_FAIL;	
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FacePunched"),
		TEXT("Com_Texture_FacePunched"), (CComponent**)&m_OnlySouthTexturesArr[FACE_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BodyPunched"),
		TEXT("Com_Texture_BodyPunched"), (CComponent**)&m_OnlySouthTexturesArr[BODY_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_LowPunched"),
		TEXT("Com_Texture_LowPunched"), (CComponent**)&m_OnlySouthTexturesArr[LOW_PUNCHED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BattleDefault"),
		TEXT("Com_Texture_BattleDefault"), (CComponent**)&m_OnlySouthTexturesArr[BATTLE_DEFAULT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_InchForward"),
		TEXT("Com_Texture_InchForward"), (CComponent**)&m_OnlySouthTexturesArr[INCH_FORWARD])))
		return E_FAIL;;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Taunt"),
		TEXT("Com_Texture_Taunt"), (CComponent**)&m_OnlySouthTexturesArr[TAUNT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BasicAttack"),
		TEXT("Com_Texture_BasicAttack"), (CComponent**)&m_OnlySouthTexturesArr[BASIC_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeavyAttackWindUp"),
		TEXT("Com_Texture_HeavyAttackWindUp"), (CComponent**)&m_OnlySouthTexturesArr[HEAVY_ATTACK_WINDUP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeavyAttack"),
		TEXT("Com_Texture_HeavyAttack"), (CComponent**)&m_OnlySouthTexturesArr[HEAVY_ATTACK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_AttackDefault"),
		TEXT("Com_Texture_AttackDefault"), (CComponent**)&m_OnlySouthTexturesArr[ATTACK_DEFAULT])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_BattleIdle"),
		TEXT("Com_Texture_BattleIdle"), (CComponent**)&m_OnlySouthTexturesArr[BATTLE_IDLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strafing"),
		TEXT("Com_Texture_Strafing"), (CComponent**)&m_OnlySouthTexturesArr[STRAFING])))
		return E_FAIL;

	// Other
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_North"),
		TEXT("Com_Texture_Stand_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthEast"),
		TEXT("Com_Texture_Stand_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_NorthWest"),
		TEXT("Com_Texture_Stand_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_East"),
		TEXT("Com_Texture_Stand_East"), (CComponent**)&m_EightDirTexturesArr[EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_West"),
		TEXT("Com_Texture_Stand_West"), (CComponent**)&m_EightDirTexturesArr[WEST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthEast"),
		TEXT("Com_Texture_Stand_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][STAND])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_SouthWest"),
		TEXT("Com_Texture_Stand_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][STAND])))
		return E_FAIL;

#pragma region Walk
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_West"), (CComponent**)&m_EightDirTexturesArr[WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_East"), (CComponent**)&m_EightDirTexturesArr[EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthWest"),
		TEXT("Com_Texture_Strip_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_SouthEast"),
		TEXT("Com_Texture_Strip_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][WALK])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Strip_South"),
		TEXT("Com_Texture_Strip_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][WALK])))
		return E_FAIL;
#pragma endregion Walk

#pragma region Run

	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_West"), (CComponent**)&m_EightDirTexturesArr[WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_East"), (CComponent**)&m_EightDirTexturesArr[EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][RUN])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Run_South"),
		TEXT("Com_Texture_Run_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][RUN])))
		return E_FAIL;

#pragma endregion Run

#pragma region suspicious
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_South"),
		TEXT("Com_Texture_Suspicious_South"), (CComponent**)&m_EightDirTexturesArr[SOUTH][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_SouthWest"),
		TEXT("Com_Texture_Suspicious_SouthWest"), (CComponent**)&m_EightDirTexturesArr[SOUTH_WEST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_SouthEast"),
		TEXT("Com_Texture_Suspicious_SouthEast"), (CComponent**)&m_EightDirTexturesArr[SOUTH_EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_East"),
		TEXT("Com_Texture_Suspicious_East"), (CComponent**)&m_EightDirTexturesArr[EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_West"),
		TEXT("Com_Texture_Suspicious_West"), (CComponent**)&m_EightDirTexturesArr[WEST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_North"),
		TEXT("Com_Texture_Suspicious_North"), (CComponent**)&m_EightDirTexturesArr[NORTH][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_NorthEast"),
		TEXT("Com_Texture_Suspicious_NorthEast"), (CComponent**)&m_EightDirTexturesArr[NORTH_EAST][SUSPICIOUS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Suspicious_NorthWest"),
		TEXT("Com_Texture_Suspicious_NorthWest"), (CComponent**)&m_EightDirTexturesArr[NORTH_WEST][SUSPICIOUS])))
		return E_FAIL;
#pragma endregion suspicious

#pragma region FallingStart - End
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FallingStart"),
		TEXT("Com_Texture_FallingStart"), (CComponent**)&m_OnlySouthTexturesArr[FALLING_START])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Falling"),
		TEXT("Com_Texture_Falling"), (CComponent**)&m_OnlySouthTexturesArr[FALLING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_FallingEnd"),
		TEXT("Com_Texture_FallingEnd"), (CComponent**)&m_OnlySouthTexturesArr[FALLING_END])))
		return E_FAIL;
#pragma endregion

	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Dead"),
		TEXT("Com_Texture_Dead"), (CComponent**)&m_OnlySouthTexturesArr[DEAD_LYING])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Dazed"),
		TEXT("Com_Texture_Dazed"), (CComponent**)&m_OnlySouthTexturesArr[DAZED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_GetUp"),
		TEXT("Com_Texture_GetUp"), (CComponent**)&m_OnlySouthTexturesArr[GET_UP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Jump"),
		TEXT("Com_Texture_Jump"), (CComponent**)&m_OnlySouthTexturesArr[JUMP])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_KarateChopped"),
		TEXT("Com_Texture_KarateChopped"), (CComponent**)&m_OnlySouthTexturesArr[KARATE_CHOPPED])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_Headless_South"),
		TEXT("Com_Texture_Headless"), (CComponent**)&m_OnlySouthTexturesArr[HEADLESS])))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Goon_HeadlessEnd_South"),
		TEXT("Com_Texture_HeadlessEnd"), (CComponent**)&m_OnlySouthTexturesArr[HEADLESS_END])))
		return E_FAIL;
#pragma endregion Renewed

	// Bubble
	if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Bubble"),
		TEXT("Com_Texture_Bubble"), (CComponent**)&m_SpeechBubble)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGoon::Add_StateOnFSM()
{
	// 부모 함수는 부모Init에서 호출하기 때문에 별도로 호출 안함
	list<CState*> tmpStateList;
	//tmpStateList.emplace_back(CState_Idle_Enemy::Create(m_pGraphic_Device, this));
	//tmpStateList.emplace_back(CEnemy0State_TestDetailed::Create(m_pGraphic_Device));

	return m_pFSMCom->Add_NewState(&tmpStateList, this);
}
