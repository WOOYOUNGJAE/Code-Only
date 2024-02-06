#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerSwapState.h"
#include "Camera_Player.h"
#include "Physics.h"
#include "Item_Interactive.h"
#include "EnemyBase.h"
#include "Transform.h"
#include "VIBuffer_Cube.h"
#include "Inventory.h"
#include "Item_Equipment.h"
#include "Gimmick_Object.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (m_bDebug)
	{
		m_eLevel = LEVEL_GAMEPLAY;
		m_strCollisionTag = TEXT("Layer_BackGround");
	}
	else
	{
		m_eLevel = LEVEL_OUTDOOR;
		m_strCollisionTag = TEXT("Layer_Floor");
	}

	if (FAILED(Add_Components()))
		return E_FAIL;


	m_pPhysicsCom->SetGravityForce(1.f);
	m_pPhysicsCom->SetJumpForce(0.5f);
	m_pPhysicsCom->SetHeight(1.5f);

	InitializeTexInfo();
	SetPlayerTex(IDLE);

	m_fSpeedInfoArray[SPEED_WALK] = 0.5f;
	m_fSpeedInfoArray[SPEED_RUN] = 0.8f;

	if (m_bDebug)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 5.f, 2.f, 5.f });
	else
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.2f, 2.f, 0.f });
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });


	m_curState = new CPlayerIdleState(true);
	m_curState->Entry(*this);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->SetPlayer(this);
	pGameInstance->Subscribe(EVENT_ENEMY_ATTACK, std::bind(&CPlayer::PlayerHit, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_EXECUTION_SUCCESS, std::bind(&CPlayer::On_ExecutionSuccess, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_INVEN_SELECT, std::bind(&CPlayer::InvenSelect, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_ENEMY_ATTACKED, std::bind(&CPlayer::ReduceCurItemHP, this, std::placeholders::_1));
	pGameInstance->Subscribe(EVENT_DELIA, std::bind(&CPlayer::HoldDelia, this, std::placeholders::_1));
	Safe_Release(pGameInstance);

	m_PlayerDesc.iHp = 100;
	m_PlayerDesc.fStamina = 100.f;
	m_PlayerDesc.bStaminaChanged = false;
	m_PlayerDesc.iGuardGauge = 100;
	m_PlayerDesc.bGuardGaugeChanged = false;

	m_pTransformCom->Scaling(_float3(0.2f, 0.05f, 0.2f));

	m_bAlive = true;
	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down('R') && !m_bAlive)
	{
		Respawn();
		_bool bRender = false;
		pGameInstance->OnEvent(EVENT_DEAD, &bRender);
		pGameInstance->OnEvent(EVENT_SET_BGM_MUTE, &bRender);
	}
	if (!m_bAlive)
	{
		m_fDeadCamTime += fTimeDelta;
		if (m_fDeadCamTime < 3.f)
		{
			m_pCamera->AddCameraPos(_float3(0.f, -fTimeDelta * 0.3f, 0.f));
		}
		return;
	}
	if (pGameInstance->Key_Down(VK_ESCAPE))
	{
		m_bPause = !m_bPause;
		if (m_bPause)
		{
			POINT pt;
			GetCursorPos(&pt);
			m_ptPauseMousePos = pt;
		}
		else
		{
			ShowCursor(false);
			SetCursorPos(m_ptPauseMousePos.x, m_ptPauseMousePos.y);
		}
	}

	if (m_bPause)
	{
		if (ShowCursor(true) > 0)
			ShowCursor(false);
		return;
	}

	__super::Tick(fTimeDelta);
	if (pGameInstance->Key_Down('Z'))
	{
		m_bDebugCollider = !m_bDebugCollider;
	}

	MouseMove(fTimeDelta);
	AnimMouseMove(fTimeDelta);

	PickObjects();
	KeyInput();

	FillStats(fTimeDelta);

	Evade(fTimeDelta);

	m_pTransformCom->SetNextPos(_float3(0.f, 0.f, 0.f));

	m_pPhysicsCom->AddForceTick(m_pTransformCom, fTimeDelta);

	if (m_pPhysicsCom->GetGround())
		m_bJump = false;

	CPlayerState* pState = m_curState->Transition(*this);
	if (nullptr != pState)
	{
		m_curState->Exit(*this);
		delete m_curState;
		m_curState = pState;
		m_curState->Entry(*this);
	}
	m_curState->Update(*this, fTimeDelta);

}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	if (!m_bAlive)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	if (m_bPause)
		return;

	__super::Late_Tick(fTimeDelta);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//cout << vPos << endl;
	if (!m_bDebug)
	{
		_float4x4 matScale, matTrans;
		D3DXMatrixIdentity(&m_matNotRot);
		D3DXMatrixScaling(&matScale, m_pTransformCom->Get_Scaled().x, m_pTransformCom->Get_Scaled().y, m_pTransformCom->Get_Scaled().z);
		if (!m_bCrouch)
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		else
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y - 2.f, vPos.z);
		m_matNotRot *= matScale * matTrans;

		m_pPhysicsCom->SetOBBDesc(&m_pTransformCom->GetNextPos(), &m_matNotRot);

		CGameObject* pGameObject;
		_bool bCollided = m_pPhysicsCom->Collision_OBB(LEVEL_OUTDOOR, TEXT("Layer_Environment"), &pGameObject);
		if (!bCollided)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
		}
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());
	}

	
}


HRESULT CPlayer::Render()
{
	if (!m_bAlive)
		return S_OK;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	_float4x4		preViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &preViewMatrix);

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

	_float4x4		preProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &preProjMatrix);

	_float4x4		ProjMatrix;
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixOrthoLH(&ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	CalculateWorldMatrix();

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_RworldMatrix);
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_ePlayerTex2)))
		return E_FAIL;
	m_pRightVIBufferCom->Render();

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_LworldMatrix);
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_ePlayerTex)))
		return E_FAIL;
	m_pLeftVIBufferCom->Render();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &preViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &preProjMatrix);

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_LeftVIBuffer"), (CComponent**)&m_pLeftVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_RightVIBuffer"), (CComponent**)&m_pRightVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_CubeVIBuffer"), (CComponent**)&m_pCubeVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevel, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Debug"),
		TEXT("Com_Texture_Debug"), (CComponent**)&m_pDebugTextureCom)))
		return E_FAIL;

	/* For.Com_Transform */
	/* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Physics */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (FAILED(m_pDebugTextureCom->Bind_OnGraphicDevice(1)))
		return E_FAIL;

	if (m_bDebugCollider)
	{
		/*	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matNotRot);
			m_pCubeVIBufferCom->Render();*/
	}

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CPlayer::CalculateWorldMatrix()
{
	_float4x4		Scaling, Rotation, Translation;

	m_LworldMatrix = *D3DXMatrixScaling(&Scaling, m_vLSize.x, m_vLSize.y, 1.f) *
		*D3DXMatrixTranslation(&Translation, m_vLPos.x + m_fMouseMovePos.x - g_iWinSizeX * 0.5f, -m_vLPos.y - m_fMouseMovePos.y + g_iWinSizeY * 0.5f, 0.7f);

	m_RworldMatrix = *D3DXMatrixScaling(&Scaling, m_vRSize.x, m_vRSize.y, 1.f) * *D3DXMatrixRotationZ(&Rotation, m_fRightAngle)
		* *D3DXMatrixTranslation(&Translation, m_vRPos.x + m_fMouseMovePos.x - g_iWinSizeX * 0.5f, -m_vRPos.y - m_fMouseMovePos.y + g_iWinSizeY * 0.5f, 0.7f);
}

void CPlayer::InitializeTexInfo()
{
	m_TexInfoArray[IDLE].vInitLPos = { g_iWinSizeX * 0.5f - 400.f , g_iWinSizeY - 50.f };
	m_TexInfoArray[IDLE].vLSize = { -482.f,361.f };
	m_TexInfoArray[IDLE].vInitRPos = { g_iWinSizeX * 0.5f + 400.f , g_iWinSizeY - 50.f };
	m_TexInfoArray[IDLE].vRSize = { 482.f ,361.f };

	m_TexInfoArray[RUN].vInitLPos = { g_iWinSizeX * 0.5f - 500.f , g_iWinSizeY + 100.f - 70.f };
	m_TexInfoArray[RUN].vLSize = { -520.f ,640.f };
	m_TexInfoArray[RUN].vInitRPos = { g_iWinSizeX * 0.5f + 500.f , g_iWinSizeY + 100.f + 640.f * 1 / 3.f };
	m_TexInfoArray[RUN].vRSize = { 520.f ,640.f };

	m_TexInfoArray[PUNCH].vInitLPos = { g_iWinSizeX * 0.5f - 600.f , g_iWinSizeY + 1050.f };
	m_TexInfoArray[PUNCH].vLSize = { -658.f,606.f };
	m_TexInfoArray[PUNCH].vInitRPos = { g_iWinSizeX * 0.5f + 600.f , g_iWinSizeY + 1050.f };
	m_TexInfoArray[PUNCH].vRSize = { 658.f,606.f };

	m_TexInfoArray[HEAVYREADY].vInitLPos = { g_iWinSizeX * 0.5f - 550.f , g_iWinSizeY + 100.f };
	m_TexInfoArray[HEAVYREADY].vLSize = { 723.f,713.f };

	m_TexInfoArray[HEAVYPUNCH].vInitRPos = { g_iWinSizeX ,  g_iWinSizeY - 300.f };
	m_TexInfoArray[HEAVYPUNCH].vRSize = { 1204.f,604.f };

	m_TexInfoArray[GUARD].vInitLPos = { g_iWinSizeX * 0.5f - 450.f , g_iWinSizeY + 30.f };
	m_TexInfoArray[GUARD].vLSize = { -598.f * 3 / 4.f,793.f * 3 / 4.f };
	m_TexInfoArray[GUARD].vInitRPos = { g_iWinSizeX * 0.5f + 450.f , g_iWinSizeY + 50.f };
	m_TexInfoArray[GUARD].vRSize = { 598.f * 3 / 4.f,793.f * 3 / 4.f };

	m_TexInfoArray[KICK1].vInitLPos = { 0.f, 0.f };
	m_TexInfoArray[KICK1].vLSize = { 0.f, 0.f };
	m_TexInfoArray[KICK1].vInitRPos = { g_iWinSizeX * 0.5f + 500.f , g_iWinSizeY };
	m_TexInfoArray[KICK1].vRSize = { 201.f ,51.f };

	m_TexInfoArray[KICK2].vInitLPos = { 0.f, 0.f };
	m_TexInfoArray[KICK2].vLSize = { 0.f, 0.f };
	m_TexInfoArray[KICK2].vInitRPos = { g_iWinSizeX * 0.5f + 400.f , g_iWinSizeY - (254.f - 51.f) / 2.f };
	m_TexInfoArray[KICK2].vRSize = { 362.f ,254.f };

	m_TexInfoArray[KICK3].vInitLPos = { 0.f, 0.f };
	m_TexInfoArray[KICK3].vLSize = { 0.f, 0.f };
	m_TexInfoArray[KICK3].vInitRPos = { g_iWinSizeX * 0.5f + 300.f , g_iWinSizeY - (281.f - 51.f) / 2.f };
	m_TexInfoArray[KICK3].vRSize = { 490.f ,281.f };

	m_TexInfoArray[KICK4].vInitLPos = { 0.f, 0.f };
	m_TexInfoArray[KICK4].vLSize = { 0.f, 0.f };
	m_TexInfoArray[KICK4].vInitRPos = { g_iWinSizeX * 0.5f + 200.f , g_iWinSizeY - (699.f - 51.f) / 2.f + 30.f };
	m_TexInfoArray[KICK4].vRSize = { 324.f ,699.f };

	for (int i = GRAB1; i <= GRAB10; i++)
	{
		m_TexInfoArray[i].vInitLPos = { g_iWinSizeX * 1 / 2.f  , g_iWinSizeY * 2 / 3.f };
		m_TexInfoArray[i].vLSize = { 1839.f * 2 / 3.f, 1503.f * 2 / 3.f };
	}

	m_TexInfoArray[GRAB10].vInitRPos = { g_iWinSizeX ,  g_iWinSizeY - 300.f };
	m_TexInfoArray[GRAB10].vRSize = { 1204.f,604.f };

	m_TexInfoArray[SPINKICK1].vInitRPos = { g_iWinSizeX / 2.f - 800.f ,  g_iWinSizeY + 200.f };
	m_TexInfoArray[SPINKICK1].vRSize = { 696.f,385.f };

	m_TexInfoArray[SPINKICK2].vInitRPos = { g_iWinSizeX / 2.f ,  g_iWinSizeY };
	m_TexInfoArray[SPINKICK2].vRSize = { 1013.f,398.f };

	m_TexInfoArray[SPINKICK3].vInitRPos = { g_iWinSizeX / 2.f ,  g_iWinSizeY };
	m_TexInfoArray[SPINKICK3].vRSize = { 836.f,522.f };

	m_TexInfoArray[SPINKICK4].vInitRPos = { g_iWinSizeX / 2.f ,  g_iWinSizeY };
	m_TexInfoArray[SPINKICK4].vRSize = { 697.f,485.f };

	m_TexInfoArray[UPPERCUT].vInitRPos = { g_iWinSizeX / 2.f + 300.f,  g_iWinSizeY + 300.f };
	m_TexInfoArray[UPPERCUT].vRSize = { 841.f * 2 / 3.f,1260.f * 2 / 3.f };

	for (int i = GRAB11; i <= GRAB20; i++)
	{
		m_TexInfoArray[i].vInitLPos = { g_iWinSizeX * 1 / 2.f  , g_iWinSizeY * 2 / 3.f };
		m_TexInfoArray[i].vLSize = { 1040.f, 920.f };
	}

	m_TexInfoArray[GRAB20].vInitRPos = { g_iWinSizeX ,  g_iWinSizeY - 300.f };
	m_TexInfoArray[GRAB20].vRSize = { 1204.f,604.f };

	m_TexInfoArray[PIPEATK1].vInitRPos = { g_iWinSizeX / 2.f + 300.f ,  g_iWinSizeY };
	m_TexInfoArray[PIPEATK1].vRSize = { 783.f, 1213.f };

	m_TexInfoArray[PIPEATK2].vInitRPos = { g_iWinSizeX / 2.f + 300.f ,  g_iWinSizeY };
	m_TexInfoArray[PIPEATK2].vRSize = { 1168.f, 825.f };

	m_TexInfoArray[PIPEATK3].vInitRPos = { g_iWinSizeX / 2.f + 300.f ,  g_iWinSizeY };
	m_TexInfoArray[PIPEATK3].vRSize = { 1168.f, 689.f };

	m_TexInfoArray[PIPEIDLE].vInitRPos = { g_iWinSizeX / 2.f + 400.f ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[PIPEIDLE].vRSize = { 711.f, 1286.f };

	m_TexInfoArray[PIPEHEAVYATK3].vRSize = { 1591.f, 924.f };
	m_TexInfoArray[PIPEHEAVYATK5].vRSize = { 1043.f, 1159.f };

	m_TexInfoArray[PIPEGUARD].vInitRPos = { g_iWinSizeX / 2.f + 200.f ,  g_iWinSizeY + 400.f };
	m_TexInfoArray[PIPEGUARD].vRSize = { 1111.f, 1131.f };

	m_TexInfoArray[BOTTLEATK1].vRSize = { 581.f, 941.f };
	m_TexInfoArray[BOTTLEATK2].vRSize = { 555.f, 1008.f };
	m_TexInfoArray[BOTTLEATK3].vRSize = { 482.f, 721.f };
	m_TexInfoArray[BOTTLEATK4].vRSize = { 270.f, 346.f };
	m_TexInfoArray[BOTTLEIDLE].vInitRPos = { g_iWinSizeX / 2.f + 550.f ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[BOTTLEIDLE].vRSize = { 704.f, 947.f };

	m_TexInfoArray[BOTTLEBROKENATK].vRSize = { 687.f, 618.f };
	m_TexInfoArray[BOTTLEBROKENIDLE].vInitRPos = { g_iWinSizeX / 2.f + 450.f ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[BOTTLEBROKENIDLE].vRSize = { 426.f, 502.f };

	m_TexInfoArray[MEDIKIT].vInitRPos = { g_iWinSizeX / 2.f + 450.f ,  g_iWinSizeY - 50.f };
	m_TexInfoArray[MEDIKIT].vLSize = { 0.f,0.f };
	m_TexInfoArray[MEDIKIT].vRSize = { 317.f * 1.5f , 212.f * 1.5f };

	m_TexInfoArray[APPLE].vInitRPos = { g_iWinSizeX / 2.f + 450.f ,  g_iWinSizeY - 50.f };
	m_TexInfoArray[APPLE].vLSize = { 0.f,0.f };
	m_TexInfoArray[APPLE].vRSize = { 118.f * 1.5f, 125.f * 1.5f };

	m_TexInfoArray[BANANA].vInitRPos = { g_iWinSizeX / 2.f + 450.f ,  g_iWinSizeY - 50.f };
	m_TexInfoArray[BANANA].vLSize = { 0.f,0.f };
	m_TexInfoArray[BANANA].vRSize = { 279.f * 1.5f, 205.f * 1.5f };

	m_TexInfoArray[COLA].vInitRPos = { g_iWinSizeX / 2.f + 450.f ,  g_iWinSizeY - 50.f };
	m_TexInfoArray[COLA].vLSize = { 0.f,0.f };
	m_TexInfoArray[COLA].vRSize = { 85.f * 1.5f, 308.f * 1.5f };

	m_TexInfoArray[THROW].vRSize = { 1134.f, 564.f };
	m_TexInfoArray[WOODENBARREL].vRSize = { 416.f * 1.5f, 577.f * 1.5f };
	m_TexInfoArray[CARD].vRSize = { 196.f * 1.5f, 144.f * 1.5f };
	m_TexInfoArray[TOOLBOX].vRSize = { 601.f * 1.5f, 475.f * 1.5f };
	m_TexInfoArray[TRASHBIN1].vRSize = { 229.f * 1.5f, 496.f * 1.5f };
	m_TexInfoArray[TRASHBIN2].vRSize = { 239.f * 1.5f, 340.f * 1.5f };
	m_TexInfoArray[TRASH].vRSize = { 95.f * 1.5f, 86.f * 1.5f };
	m_TexInfoArray[BARREL].vRSize = { 792.f * 0.7f,1274.f * 0.7f };
	m_TexInfoArray[PAINT].vRSize = { 206.f * 1.5f, 236.f * 1.5f };
	m_TexInfoArray[POTPLANT].vRSize = { 411.f * 1.5f, 672.f * 1.5f };
	m_TexInfoArray[TABLEPLANT].vRSize = { 218.f * 1.5f, 492.f * 1.5f };
	m_TexInfoArray[TUMBLERGLASS].vRSize = { 157.f * 1.5f, 202.f * 1.5f };
	m_TexInfoArray[UMBRELLASTAND].vRSize = { 133.f * 1.5f, 309.f * 1.5f };
	m_TexInfoArray[COATHANGER].vRSize = { 431.f * 0.7f, 1301.f * 0.7f };

	m_TexInfoArray[BALL1].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL2].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL3].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL4].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL5].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL6].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL7].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL8].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL9].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL10].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL11].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL12].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL13].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL14].vRSize = { 350.f, 350.f };
	m_TexInfoArray[BALL15].vRSize = { 350.f, 350.f };
	m_TexInfoArray[WOODENSTOOL].vRSize = { 391.f, 434.f };
	m_TexInfoArray[BARSTOOL].vRSize = { 341.f, 645.f };

	m_TexInfoArray[CHOP1].vLSize = { 0.f, 0.f };
	m_TexInfoArray[CHOP1].vInitRPos = { g_iWinSizeX / 2.f  ,  g_iWinSizeY - 50.f };
	m_TexInfoArray[CHOP1].vRSize = { 759.f, 455.f };
	m_TexInfoArray[CHOP2].vRSize = { 849.f, 658.f };
	m_TexInfoArray[CHOP3].vRSize = { 1644.f, 989.f };
	m_TexInfoArray[CHOP4].vRSize = { 1184.f, 881.f };

	m_TexInfoArray[FIREIDLE].vInitRPos = { g_iWinSizeX / 2.f + 250.f  ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[FIREIDLE].vRSize = { 1414.f * 0.6f, 750.f * 0.6f };
	m_TexInfoArray[FIRE1].vRSize = { 1423.f * 0.6f, 938.f * 0.6f };
	m_TexInfoArray[FIRE2].vRSize = { 1379.f * 0.6f, 819.f * 0.6f };
	m_TexInfoArray[FIRE3].vRSize = { 1413.f * 0.6f, 751.f * 0.6f };

	m_TexInfoArray[GUARDL].vInitLPos = { g_iWinSizeX / 2.f - 450.f  ,  g_iWinSizeY + 30.f };
	m_TexInfoArray[GUARDL].vLSize = { 468.f , 668.f };
	m_TexInfoArray[GUARDR].vInitRPos = { g_iWinSizeX / 2.f + 400.f  ,  g_iWinSizeY + 50.f };
	m_TexInfoArray[GUARDR].vRSize = { 454.f , 706.f };

	m_TexInfoArray[PISTOLIDLE].vInitRPos = { g_iWinSizeX / 2.f + 400.f  ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[PISTOLIDLE].vRSize = { 466.f , 472.f };
	m_TexInfoArray[PISTOLFIRE].vRSize = { 565.f , 517.f };
	m_TexInfoArray[PISTOLMEELEE].vInitRPos = { g_iWinSizeX / 2.f + 500.f  ,  g_iWinSizeY - 200.f };
	m_TexInfoArray[PISTOLMEELEE].vRSize = { 653.f , 648.f };
	m_TexInfoArray[PISTOLSPIN1].vRSize = { 544.f , 571.f };
	m_TexInfoArray[PISTOLSPIN2].vRSize = { 518.f , 573.f };
	m_TexInfoArray[PISTOLSPIN3].vRSize = { 527.f , 566.f };
	m_TexInfoArray[PISTOLSPIN4].vRSize = { 514.f , 600.f };
	m_TexInfoArray[PISTOLMEELEEATK].vRSize = { 798 , 692 };

	m_TexInfoArray[DELIAHOLD].vInitLPos = { g_iWinSizeX / 2.f - 800.f  ,  g_iWinSizeY - 250.f };
	m_TexInfoArray[DELIAHOLD].vLSize = { 680.f * 0.8f, 850.f * 0.8f };
}

void CPlayer::PickObjects()
{
	_float3 pOut = _float3(0.f, 0.f, 0.f);
	CGameObject* pPickItem = nullptr;
	m_pPhysicsCom->MouseRayCast(LEVEL_OUTDOOR, TEXT("Layer_Interactive"), &pOut, &pPickItem);
	CGameInstance* pGameIns = CGameInstance::Get_Instance();
	PROMPTER Prompter;
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fDist = D3DXVec3Length(&(pOut - vPos));

	if (pPickItem != nullptr && fDist < 3.f)
	{	//동준수정
		if (nullptr != m_pPickItem)
		{
			m_pOldPickItem->Set_Prompter(false);  //옛날에 픽된거 false로
			Prompter = PROMPTER_END;
			pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
		}
		m_pPickItem = dynamic_cast<CInteractiveObject*>(pPickItem);
		if (nullptr != m_pOldPickItem && dynamic_cast<CInteractiveObject*>(pPickItem)->Get_Item_Type() != m_pOldPickItem->Get_Item_Type())
		{
			m_pOldPickItem->Set_Prompter(false);	//다른놈이면 false로
		}
		m_pPickItem->Set_Prompter(true);	//피킹(진)
		ITEM_TYPE ItemType = dynamic_cast<CInteractiveObject*>(pPickItem)->Get_Item_Type();

		switch (ItemType)
		{
		case(ITEM_BOTTLE):
			Prompter = PROMPTER_BOTTLE;
			break;
		case(ITEM_AUTOMATIC_PISTOL):
			Prompter = PROMPTER_AUTOMATIC_PISTOL;
			break;
		case(ITEM_TOMMY_GUN):
			Prompter = PROMPTER_TOMMY_GUN;
			break;
		case(ITEM_PIPE):
			Prompter = PROMPTER_LEADPIPE;
			break;
		case(ITEM_FRYING_PAN):
			Prompter = PROMPTER_FRYINGPAN;
			break;
		case(ITEM_BOTTLE_BROKEN):
			Prompter = PROMPTER_BOTTLE;
			break;
		case(ITEM_MEDIKIT):
			Prompter = PROMPTER_MEDKIT;
			break;
		case(ITEM_BANANA):
			Prompter = PROMPTER_BANANA;
			break;
		case(ITEM_APPLE):
			Prompter = PROMPTER_APPLE;
			break;
		case(ITEM_COLA):
			Prompter = PROMPTER_COLA;
			break;
		case(ITEM_BANANA_PEEL):
			Prompter = PROMPTER_BANANA_PEEL;
			break;
		case(ITEM_DOOR):
			Prompter = PROMPTER_DOOR_LOCKED;	//열쇠로 문 땄을때 수정필요
			{
				if (static_cast<CGimmick_Object*>(pPickItem)->Get_DoorLocked() == false)
				{
					Prompter = PROMPTER_DOOR;
				}
			}
			break;
		case(ITEM_ENTRANCE_KEY):
			Prompter = PROMPTER_KEY;
			break;
		case(ITEM_VENDING_MACHINE):
			Prompter = PROMPTER_INTERACT;
			break;
		case(ITEM_CARDBOARD_BOX):
			Prompter = PROMPTER_CARDBOARD_BOX;
			break;
		case(ITEM_RECEIPT):
			Prompter = PROMPTER_RECEIPT;
			break;
		case(ITEM_BUTTON):
			Prompter = PROMPTER_BUTTON;
			break;
		case(ITEM_SCRUNCHED_PAPER):
			Prompter = PROMPTER_SCRUNCHED_PAPER;
			break;
		case(ITEM_WOODEN_BARREL):
			Prompter = PROMPTER_WOODEN_BARREL;
			break;
		case(ITEM_OIL_BARREL):
			Prompter = PROMPTER_OIL_BARREL;
			break;
		case(ITEM_PAINT_CAN):
			Prompter = PROMPTER_PAINT_CAN;
			break;
		case(ITEM_TRASH_CAN):
			Prompter = PROMPTER_TRASH_CAN;
			break;
		case(ITEM_TRASH_BIN):
			Prompter = PROMPTER_TRASH_BIN;
			break;
		case(ITEM_TOOL_BOX):
			Prompter = PROMPTER_TOOLBOX;
			break;
		case(ITEM_UMBRELLA_STAND):
			Prompter = PROMPTER_UMBRELLA_STAND;
			break;
		case(ITEM_TABLE_LAMP):
			Prompter = PROMPTER_TABLE_LAMP;
			break;
		case(ITEM_POT_PLANT):
			Prompter = PROMPTER_POT_PLANT;
			break;
		case(ITEM_WOODEN_STOOL):
			Prompter = PROMPTER_WOODEN_STOOL;
			break;
		case(ITEM_TABLE_PLANT):
			Prompter = PROMPTER_TABLE_PLANT;
			break;
		case(ITEM_MARTINI_GLASS):
			Prompter = PROMPTER_MARTINI_GLASS;
			break;
		case(ITEM_TUMBLER_GLASS):
			Prompter = PROMPTER_TUMBLER_GLASS;
			break;
		case(ITEM_POOL_BALL_0):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_1):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_2):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_3):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_4):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_5):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_6):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_7):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_8):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_9):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_10):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_11):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_12):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_13):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_14):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_POOL_BALL_15):
			Prompter = PROMPTER_POOL_BALL;
			break;
		case(ITEM_ASHTRAY_BIN):
			Prompter = PROMPTER_ASHTRAY_BIN;
			break;
		case(ITEM_GAS_CANISTER):
			Prompter = PROMPTER_GAS_CANISTER;
			break;
		case(ITEM_METAL_BARREL):
			Prompter = PROMPTER_METAL_BARREL;
			break;
		case(ITEM_HAIR_BRUSH):
			Prompter = PROMPTER_HAIRBRUSH;
			break;
		case(ITEM_BAR_STOOL):
			Prompter = PROMPTER_BAR_STOOL;
			break;
		case(ITEM_JAR):
			Prompter = PROMPTER_JAR;
			break;
		case(ITEM_CAN):
			Prompter = PROMPTER_CAN;
			break;
		case(ITEM_ESPRESSO_MACHINE):
			Prompter = PROMPTER_ESPRESSO_MACHINE;
			break;
		case(ITEM_COAT_HANGER):
			Prompter = PROMPTER_COAT_HANGER;
			break;
		case(ITEM_HAND_BAG):
			Prompter = PROMPTER_HANDBAG;
			break;
		case(ITEM_TRUTH_SERUM):
			Prompter = PROMPTER_TRUTH_SERUM;
			break;
		case(ITEM_COMBOLOCK):
			Prompter = PROMPTER_PADLOCK;
			break;
		default:
			Prompter = PROMPTER_END;
			break;
		}
		pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
		m_vPickItemCollidedPos = pOut;
	}
	//마우스 피킹에 아이템이 안들어왔을때 
	else
	{
		m_pPickItem = nullptr;
		Prompter = PROMPTER_END;
		pGameIns->OnEvent(EVENT_SET_PROMPTER, &Prompter);
		if (m_pPickItem != nullptr)
		{
			m_pPickItem->Set_Prompter(false);
		}
		if (m_pOldPickItem != nullptr)
		{
			m_pOldPickItem->Set_Prompter(false);
		}
	}
	m_pOldPickItem = m_pPickItem;
}

void CPlayer::KeyInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Key_Input(LEVEL_OUTDOOR);

	if (pGameInstance->Key_Down('X'))
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.5f, 2.f, 0.f));
		m_pPhysicsCom->SetTime(0.f);
	}

	if (pGameInstance->Key_Pressing('S') && pGameInstance->Key_Down(VK_SHIFT))
	{
		m_eEvade = EVADE_S;
	}
	else if (pGameInstance->Key_Pressing('A') && pGameInstance->Key_Down(VK_SHIFT))
	{
		m_eEvade = EVADE_A;
	}
	else if (pGameInstance->Key_Pressing('D') && pGameInstance->Key_Down(VK_SHIFT))
	{
		m_eEvade = EVADE_D;
	}
	else if (pGameInstance->Key_Pressing('S') && pGameInstance->Key_Pressing('A') && pGameInstance->Key_Down(VK_SHIFT))
	{
		m_eEvade = EVADE_SA;
	}
	else if (pGameInstance->Key_Pressing('S') && pGameInstance->Key_Pressing('D') && pGameInstance->Key_Down(VK_SHIFT))
	{
		m_eEvade = EVADE_SD;
	}

	if ((pGameInstance->Key_Pressing('S') && pGameInstance->Key_Down(VK_SHIFT)) ||
		(pGameInstance->Key_Pressing('S') && pGameInstance->Key_Pressing('A') && pGameInstance->Key_Down(VK_SHIFT)) ||
		(pGameInstance->Key_Pressing('S') && pGameInstance->Key_Pressing('D') && pGameInstance->Key_Down(VK_SHIFT)) ||
		(pGameInstance->Key_Pressing('A') && pGameInstance->Key_Down(VK_SHIFT)) ||
		(pGameInstance->Key_Pressing('D') && pGameInstance->Key_Down(VK_SHIFT)))
	{
		if (m_PlayerDesc.fStamina > 15.f)
		{
			m_bEvade = true;
			m_fSpeed = 1.f;
			_float fUse = -15.f;
			ChangeStamina(&fUse);
			m_PlayerDesc.bStaminaChanged = true;
		}
		else
		{
			_uint idx = NOTI_NOT_ENOUGH_STAMINA;
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_NOTIFICATION, &idx);
			if (!CGameInstance::Get_Instance()->IsPlaying(SOUND_NOTI_ERROR))
				CGameInstance::Get_Instance()->Play_Sound(L"Error.wav", SOUND_NOTI_ERROR, m_fSoundVolume);
		}
	}
	Safe_Release(pGameInstance);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CPlayer");
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CPlayer");
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pDebugTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pLeftVIBufferCom);
	Safe_Release(m_pRightVIBufferCom);
	Safe_Release(m_pCubeVIBufferCom);
	Safe_Release(m_pPhysicsCom);

	if (m_curState != nullptr)
		m_curState->Exit(*this);
	Safe_Delete(m_curState);

}

void CPlayer::FillStats(_float fTimeDelta)
{
	if (m_PlayerDesc.bStaminaChanged)
	{
		m_fStaminaFillTime += fTimeDelta;
		if (m_fStaminaFillTime > 2.f)
		{
			m_fStaminaFillTime = 0.f;
			m_PlayerDesc.bStaminaChanged = false;
		}
	}
	else
	{
		_float fUse = 2.f;
		ChangeStamina(&fUse);
	}

	if (m_PlayerDesc.bGuardGaugeChanged)
	{
		m_fGuardGaugeFillTime += fTimeDelta;
		if (m_fGuardGaugeFillTime > 1.f)
		{
			m_fGuardGaugeFillTime = 0.f;
			m_PlayerDesc.bGuardGaugeChanged = false;
		}
	}
	else
	{
		if (m_PlayerDesc.iGuardGauge < 100)
		{
			_int fUse = 3;
			ChangeGuardGauge(&fUse);
		}
	}
}

void CPlayer::Respawn()
{
	m_bAlive = true;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.2f, 2.f, 0.f));
	m_PlayerDesc.iHp = 100;
	m_PlayerDesc.fStamina = 100.f;
	m_PlayerDesc.bStaminaChanged = false;
	m_PlayerDesc.iGuardGauge = 100;
	m_PlayerDesc.bGuardGaugeChanged = false;
	m_pCamera->SetCameraPos(_float3(0.f, 0.f, 0.f));
	HEALTHDESC health;
	health.iHealth = m_PlayerDesc.iHp;
	CGameInstance::Get_Instance()->OnEvent(EVENT_UPDATE_HEALTH, &health);
	m_curState->Exit(*this);
	delete m_curState;
	SetPlayerTex(IDLE);
	m_curState = new CPlayerIdleState(true);
	m_curState->Entry(*this);
}

void CPlayer::SetPlayerOnlyTex(PLAYER_TEX _tex)
{
	_uint init = _tex;
	if (_tex == IDLE)
	{
		if (m_pHoldItem != nullptr)
			_tex = HEAVYREADY;
		else
			m_fRightAngle = 0.f;
	}

	if (m_bDeliaHold)
		_tex = DELIAHOLD;

	m_ePlayerTex = _tex;
	m_vLSize = m_TexInfoArray[_tex].vLSize;

	if (m_pHoldItem != nullptr && (_tex == HEAVYREADY || _tex == DELIAHOLD))
	{
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_PIPE:
			_tex = PIPEIDLE;
			break;
		case ITEM_BOTTLE:
			_tex = BOTTLEIDLE;
			break;
		case ITEM_BOTTLE_BROKEN:
			_tex = BOTTLEBROKENIDLE;
			break;
		case ITEM_TOMMY_GUN:
			_tex = FIREIDLE;
			if (!m_bDeliaHold)
				m_vLSize = { 0.f,0.f };
			break;
		case ITEM_AUTOMATIC_PISTOL:
			if (!m_bPistolMeelee)
				_tex = PISTOLIDLE;
			else
				_tex = PISTOLMEELEE;
			if (!m_bDeliaHold)
				m_vLSize = { 0.f,0.f };
			break;
		case ITEM_MEDIKIT:
			_tex = MEDIKIT;
			break;
		case ITEM_APPLE:
			_tex = APPLE;
			break;
		case ITEM_BANANA:
			_tex = BANANA;
			break;
		case ITEM_COLA:
			_tex = COLA;
			break;
		case ITEM_WOODEN_BARREL:
			_tex = WOODENBARREL;
			break;
		case ITEM_CARDBOARD_BOX:
			_tex = CARD;
			break;
		case ITEM_TRASH_BIN:
			_tex = TRASHBIN1;
			break;
		case ITEM_TRASH_CAN:
			_tex = TRASHBIN2;
			break;
		case ITEM_SCRUNCHED_PAPER:
			_tex = TRASH;
			break;
		case ITEM_TOOL_BOX:
			_tex = TOOLBOX;
			break;
		case ITEM_OIL_BARREL:
			_tex = BARREL;
			break;
		case ITEM_PAINT_CAN:
			_tex = PAINT;
			break;
		case ITEM_POT_PLANT:
			_tex = POTPLANT;
			break;
		case ITEM_TABLE_PLANT:
			_tex = TABLEPLANT;
			break;
		case ITEM_TUMBLER_GLASS:
			_tex = TUMBLERGLASS;
			break;
		case ITEM_UMBRELLA_STAND:
			_tex = UMBRELLASTAND;
			break;
		case ITEM_COAT_HANGER:
			_tex = COATHANGER;
			break;
		case ITEM_POOL_BALL_1:
			_tex = BALL1;
			break;
		case ITEM_POOL_BALL_2:
			_tex = BALL2;
			break;
		case ITEM_POOL_BALL_3:
			_tex = BALL3;
			break;
		case ITEM_POOL_BALL_4:
			_tex = BALL4;
			break;
		case ITEM_POOL_BALL_5:
			_tex = BALL5;
			break;
		case ITEM_POOL_BALL_6:
			_tex = BALL6;
			break;
		case ITEM_POOL_BALL_7:
			_tex = BALL7;
			break;
		case ITEM_POOL_BALL_8:
			_tex = BALL8;
			break;
		case ITEM_POOL_BALL_9:
			_tex = BALL9;
			break;
		case ITEM_POOL_BALL_10:
			_tex = BALL10;
			break;
		case ITEM_POOL_BALL_11:
			_tex = BALL11;
			break;
		case ITEM_POOL_BALL_12:
			_tex = BALL12;
			break;
		case ITEM_POOL_BALL_13:
			_tex = BALL13;
			break;
		case ITEM_POOL_BALL_14:
			_tex = BALL14;
			break;
		case ITEM_POOL_BALL_15:
			_tex = BALL15;
			break;
		case ITEM_WOODEN_STOOL:
			_tex = WOODENSTOOL;
			break;
		case ITEM_BAR_STOOL:
			_tex = BARSTOOL;
			break;
		}

		if (m_pHoldItem->Get_Item_Type() > 100)
		{
			m_fRightAngle = -3.14f / 4;
			if (!m_bDeliaHold)
				m_vLSize = { 0.f,0.f };
		}
		else
			m_fRightAngle = 0.f;
	}

	if (init == IDLE && m_pHoldItem == nullptr)
		_tex = IDLE;
	m_ePlayerTex2 = _tex;
	m_vRSize = m_TexInfoArray[_tex].vRSize;
}

void CPlayer::SetPlayerTex(_uint _tex)
{
	_uint init = _tex;
	if (_tex == IDLE)
	{
		if (m_pHoldItem != nullptr)
			_tex = HEAVYREADY;
	}
	if (m_bDeliaHold)
		_tex = DELIAHOLD;

	m_ePlayerTex = _tex;
	m_vLPos = m_TexInfoArray[_tex].vInitLPos;
	m_vLSize = m_TexInfoArray[_tex].vLSize;

	if (m_pHoldItem != nullptr && (_tex == HEAVYREADY || _tex == DELIAHOLD))
	{
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_PIPE:
			_tex = PIPEIDLE;
			break;
		case ITEM_BOTTLE:
			_tex = BOTTLEIDLE;
			break;
		case ITEM_BOTTLE_BROKEN:
			_tex = BOTTLEBROKENIDLE;
			break;
		case ITEM_TOMMY_GUN:
			_tex = FIREIDLE;
			if (!m_bDeliaHold)
				m_vLSize = { 0.f,0.f };
			break;
		case ITEM_AUTOMATIC_PISTOL:
			if (!m_bPistolMeelee)
				_tex = PISTOLIDLE;
			else
				_tex = PISTOLMEELEE;
			if (!m_bDeliaHold)
				m_vLSize = { 0.f,0.f };
			break;
		case ITEM_MEDIKIT:
			_tex = MEDIKIT;
			break;
		case ITEM_APPLE:
			_tex = APPLE;
			break;
		case ITEM_BANANA:
			_tex = BANANA;
			break;
		case ITEM_COLA:
			_tex = COLA;
			break;
		case ITEM_WOODEN_BARREL:
			_tex = WOODENBARREL;
			break;
		case ITEM_CARDBOARD_BOX:
			_tex = CARD;
			break;
		case ITEM_TRASH_BIN:
			_tex = TRASHBIN1;
			break;
		case ITEM_TRASH_CAN:
			_tex = TRASHBIN2;
			break;
		case ITEM_SCRUNCHED_PAPER:
			_tex = TRASH;
			break;
		case ITEM_TOOL_BOX:
			_tex = TOOLBOX;
			break;
		case ITEM_OIL_BARREL:
			_tex = BARREL;
			break;
		case ITEM_PAINT_CAN:
			_tex = PAINT;
			break;
		case ITEM_POT_PLANT:
			_tex = POTPLANT;
			break;
		case ITEM_TABLE_PLANT:
			_tex = TABLEPLANT;
			break;
		case ITEM_TUMBLER_GLASS:
			_tex = TUMBLERGLASS;
			break;
		case ITEM_UMBRELLA_STAND:
			_tex = UMBRELLASTAND;
			break;
		case ITEM_COAT_HANGER:
			_tex = COATHANGER;
		case ITEM_POOL_BALL_1:
			_tex = BALL1;
			break;
		case ITEM_POOL_BALL_2:
			_tex = BALL2;
			break;
		case ITEM_POOL_BALL_3:
			_tex = BALL3;
			break;
		case ITEM_POOL_BALL_4:
			_tex = BALL4;
			break;
		case ITEM_POOL_BALL_5:
			_tex = BALL5;
			break;
		case ITEM_POOL_BALL_6:
			_tex = BALL6;
			break;
		case ITEM_POOL_BALL_7:
			_tex = BALL7;
			break;
		case ITEM_POOL_BALL_8:
			_tex = BALL8;
			break;
		case ITEM_POOL_BALL_9:
			_tex = BALL9;
			break;
		case ITEM_POOL_BALL_10:
			_tex = BALL10;
			break;
		case ITEM_POOL_BALL_11:
			_tex = BALL11;
			break;
		case ITEM_POOL_BALL_12:
			_tex = BALL12;
			break;
		case ITEM_POOL_BALL_13:
			_tex = BALL13;
			break;
		case ITEM_POOL_BALL_14:
			_tex = BALL14;
			break;
		case ITEM_POOL_BALL_15:
			_tex = BALL15;
			break;
		case ITEM_WOODEN_STOOL:
			_tex = WOODENSTOOL;
			break;
		case ITEM_BAR_STOOL:
			_tex = BARSTOOL;
			break;
		}
	}

	if (init == IDLE && m_pHoldItem == nullptr)
		_tex = IDLE;
	m_ePlayerTex2 = _tex;
	m_vRPos = m_TexInfoArray[_tex].vInitRPos;
	m_vRSize = m_TexInfoArray[_tex].vRSize;
}



_bool CPlayer::Move(_float fTimeDelta)
{
	if ((GetAsyncKeyState('W') & 0x8000) &&
		(GetAsyncKeyState('A') & 0x8000))
	{
		m_pTransformCom->Go_LeftUp_FixedY(fTimeDelta * m_fSpeed);
		return true;
	}
	else if ((GetAsyncKeyState('W') & 0x8000) &&
		(GetAsyncKeyState('D') & 0x8000))
	{
		m_pTransformCom->Go_RightUp_FixedY(fTimeDelta * m_fSpeed);
		return true;
	}
	else if ((GetAsyncKeyState('S') & 0x8000) &&
		(GetAsyncKeyState('A') & 0x8000))
	{
		m_pTransformCom->Go_LeftDown_FixedY(fTimeDelta * m_fSpeed);
		return true;
	}
	else if ((GetAsyncKeyState('S') & 0x8000) &&
		(GetAsyncKeyState('D') & 0x8000))
	{
		m_pTransformCom->Go_RightDown_FixedY(fTimeDelta * m_fSpeed);
		return true;
	}
	else if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight_FixedY(fTimeDelta * m_fSpeed);
		ResetCamera(fTimeDelta);
		return true;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward_FixedY(fTimeDelta * m_fSpeed);
		ResetCamera(fTimeDelta);
		return true;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		m_pTransformCom->Go_Left_FixedY(fTimeDelta * m_fSpeed);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		if (m_fTiltAngle < 0.2f)
		{
			m_fTiltAngle += fTimeDelta;
			m_pTransformCom->Turn(vLook, fTimeDelta * 0.1f);
		}
		else
		{
			m_pTransformCom->Turn(-vLook, (m_fTiltAngle - 0.2f) * fTimeDelta * 0.1f);
			m_fTiltAngle = 0.2f;
		}
		return true;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		m_pTransformCom->Go_Right_FixedY(fTimeDelta * m_fSpeed);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		if (m_fTiltAngle > -0.2f)
		{
			m_fTiltAngle -= fTimeDelta;
			m_pTransformCom->Turn(-vLook, fTimeDelta * 0.1f);
		}
		else
		{
			m_pTransformCom->Turn(vLook, -(m_fTiltAngle + 0.2f) * fTimeDelta * 0.1f);
			m_fTiltAngle = -0.2f;
		}
		return true;
	}

	return false;
}


_bool CPlayer::ChangeHp(void* pArg)
{
	if (m_bAlive)
	{
		m_PlayerDesc.iHp += *static_cast<int*>(pArg);
		if (m_PlayerDesc.iHp <= 0)
		{
			_bool bRender = true;
			CGameInstance::Get_Instance()->OnEvent(EVENT_DEAD, &bRender);
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_BGM_MUTE, &bRender);
			CGameInstance::Get_Instance()->OnEvent(EVENT_PLAYER_DIE);
			m_bAlive = false;
			m_PlayerDesc.iHp = 0;
		}
		else if (m_PlayerDesc.iHp > 100)
			m_PlayerDesc.iHp = 100;

		HEALTHDESC health;
		health.iHealth = m_PlayerDesc.iHp;
		CGameInstance::Get_Instance()->OnEvent(EVENT_UPDATE_HEALTH, &health);
	}
	return true;
}

_bool CPlayer::ChangeStamina(void* pArg)
{
	m_PlayerDesc.fStamina += *static_cast<_float*>(pArg);
	if (m_PlayerDesc.fStamina < 0)
	{
		m_PlayerDesc.fStamina = 0.f;
	}
	else if (m_PlayerDesc.fStamina > 100.f)
		m_PlayerDesc.fStamina = 100.f;
#pragma region jungsoo
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->OnEvent(EVENT_UPDATE_STAMINA, &m_PlayerDesc.fStamina);
	Safe_Release(pGameInstance);
#pragma endregion
	return true;
}

_bool CPlayer::ChangeGuardGauge(void* pArg)
{
	m_PlayerDesc.iGuardGauge += *static_cast<_int*>(pArg);
	if (m_PlayerDesc.iGuardGauge < 0)
		m_PlayerDesc.iGuardGauge = 0;
	else if (m_PlayerDesc.iGuardGauge > 100)
		m_PlayerDesc.iGuardGauge = 100;
#pragma region jungsoo
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	pGameInstance->OnEvent(EVENT_UPDATE_GUARD, &m_PlayerDesc.iGuardGauge);
	Safe_Release(pGameInstance);
#pragma endregion

	return true;
}

_bool CPlayer::InvenSelect(void* pArg)
{
	WEAPONUIDESC we;
	if (m_pHoldItem != nullptr)
	{
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_BOTTLE:
			we.uiUITag = UI_WEAPON_BOTTLE;
			break;
		case ITEM_PIPE:
			we.uiUITag = UI_WEAPON_LEADPIPE;
			break;
		case ITEM_BOTTLE_BROKEN:
			we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
			break;
		case ITEM_TOMMY_GUN:
			we.uiUITag = UI_WEAPON_TOMMYGUN;
			break;
		case ITEM_AUTOMATIC_PISTOL:
			we.uiUITag = UI_WEAPON_AUTOPISTOL;
			break;
		}
		if (m_pHoldItem->Get_Item_Type() <= 100)
		{
			we.fWeaponHP = 1.f;
			we.uiBulletCount = 40;
			we.bIsRender = false;
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
		}
	}

	INVENSELECT* inven = static_cast<INVENSELECT*>(pArg);
	if (inven->pInteractiveObj != nullptr)
	{
		m_pHoldItem = static_cast<CInteractiveObject*>(inven->pInteractiveObj);
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_BOTTLE:
			we.uiUITag = UI_WEAPON_BOTTLE;
			we.uiBulletCount = 0;
			break;
		case ITEM_PIPE:
			we.uiUITag = UI_WEAPON_LEADPIPE;
			we.uiBulletCount = 0;
			break;
		case ITEM_BOTTLE_BROKEN:
			we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
			we.uiBulletCount = 0;
			break;
		case ITEM_TOMMY_GUN:
			we.uiUITag = UI_WEAPON_TOMMYGUN;
			break;
		case ITEM_AUTOMATIC_PISTOL:
			we.uiUITag = UI_WEAPON_AUTOPISTOL;
			break;
		}
		if (m_pHoldItem->Get_Item_Type() <= 100)
		{
			we.fWeaponHP = static_cast<CItem_Equipment*>(m_pHoldItem)->GetHP() / (float)static_cast<CItem_Equipment*>(m_pHoldItem)->GetMaxHP();
			we.uiBulletCount = static_cast<CItem_Equipment*>(m_pHoldItem)->GetBulletCount();
			we.bIsRender = true;
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
		}
	}
	else if (m_pHoldItem != nullptr)
	{
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_BOTTLE:
			we.uiUITag = UI_WEAPON_BOTTLE;
			we.uiBulletCount = 0;
			break;
		case ITEM_PIPE:
			we.uiUITag = UI_WEAPON_LEADPIPE;
			we.uiBulletCount = 0;
			break;
		case ITEM_BOTTLE_BROKEN:
			we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
			we.uiBulletCount = 0;
			break;
		case ITEM_TOMMY_GUN:
			we.uiUITag = UI_WEAPON_TOMMYGUN;
			we.uiBulletCount = 40;
			break;
		case ITEM_AUTOMATIC_PISTOL:
			we.uiUITag = UI_WEAPON_AUTOPISTOL;
			we.uiBulletCount = 12;
			break;
		}
		if (m_pHoldItem->Get_Item_Type() <= 100)
		{
			we.fWeaponHP = 0.f;
			we.bIsRender = false;
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
		}
		m_pHoldItem = nullptr;
	}
	m_curState->Exit(*this);
	delete m_curState;
	m_curState = new CPlayerSwapState();
	m_curState->Entry(*this);

	return true;
}

_bool CPlayer::PlayerHit(void* pArg)
{
	if (!m_bAlive)
		return true;
	ENEMYATTACKDESC* at = static_cast<ENEMYATTACKDESC*>(pArg);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vEnemyPos = at->pEnemyTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vDir = vEnemyPos - vPos;
	vDir.y = 0.f;
	EFFECTDESC ef;
	D3DXVec3Normalize(&vDir, &vDir);
	CGameInstance::Get_Instance()->StopSound(SOUND_PLAYER_HIT);
	_tchar str[MAX_PATH] = TEXT("");
	wsprintf(str, TEXT("FleshHitGeneric%d.wav"), rand() % 4 + 1);
	CGameInstance::Get_Instance()->Play_Sound(str, SOUND_PLAYER_HIT, m_fSoundVolume);
	CGameInstance::Get_Instance()->StopSound(SOUND_PLAYER_HIT_VOICE);
	if (m_bGuard)
	{
		_float3 vEnemyLook = at->pEnemyTransform->Get_State(CTransform::STATE_LOOK);
		vEnemyLook.y = 0.f;
		D3DXVec3Normalize(&vEnemyLook, &vEnemyLook);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);
		_float fDot = D3DXVec3Dot(&vLook, &vEnemyLook);
		if (fDot < 0)
		{
			_float3 vEffectPos = vPos + vDir * 0.5f;
			ef.vPos = vEffectPos;
			ef.vScale = _float3(0.3f, 0.3f, 0.1f);
			CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_STRIP, &ef);
			_int iVal = -34;
			ChangeGuardGauge(&iVal);
			m_PlayerDesc.bGuardGaugeChanged = true;
			m_pPhysicsCom->SetAddForce(-vDir, 1.f);

			if (m_pHoldItem != nullptr && m_pHoldItem->Get_Item_Type() == ITEM_PIPE)
			{
				CGameInstance::Get_Instance()->Play_Sound(L"PipeBlock1.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
			}
			else
			{
				CGameInstance::Get_Instance()->Play_Sound(L"Punch6.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
			}
		}
		else
		{
			_float3 vEffectPos = vPos + vDir * 0.5f;
			ef.vPos = vEffectPos;
			ef.vScale = _float3(0.3f, 0.3f, 0.1f);
			for (int i = 0; i < 10; i++)
				CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &ef);
			_int iVal = at->iDmg;
			ChangeHp(&iVal);
			m_pPhysicsCom->SetAddForce(-vDir, 2.f);
			if (m_PlayerDesc.iHp > 70)
				CGameInstance::Get_Instance()->Play_Sound(L"Mike_LightlyHurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
			else if (m_PlayerDesc.iHp > 30 && m_PlayerDesc.iHp <= 70)
				CGameInstance::Get_Instance()->Play_Sound(L"Mike_Hurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
			else
				CGameInstance::Get_Instance()->Play_Sound(L"Mike_BadlyHurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
		}
	}
	else
	{
		_float3 vEffectPos = vPos + vDir * 0.5f;
		ef.vPos = vEffectPos;
		ef.vScale = _float3(0.3f, 0.3f, 0.1f);
		for (int i = 0; i < 10; i++)
			CGameInstance::Get_Instance()->OnEvent(EVENT_EFFECT_BLOOD_IMPACTSTRIP, &ef);
		_int iVal = at->iDmg;
		ChangeHp(&iVal);
		m_pPhysicsCom->SetAddForce(-vDir, 2.f);
		if (m_PlayerDesc.iHp > 70)
			CGameInstance::Get_Instance()->Play_Sound(L"Mike_LightlyHurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
		else if (m_PlayerDesc.iHp > 30 && m_PlayerDesc.iHp <= 70)
			CGameInstance::Get_Instance()->Play_Sound(L"Mike_Hurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
		else
			CGameInstance::Get_Instance()->Play_Sound(L"Mike_BadlyHurt.wav", SOUND_PLAYER_HIT_VOICE, m_fSoundVolume);
	}
	return true;
}


void CPlayer::ResetCamera(_float fTimeDelta)
{
	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vLook.y = 0.f;
	if (m_fTiltAngle > 0.f)
	{
		m_fTiltAngle -= fTimeDelta;
		m_pTransformCom->Turn(-vLook, fTimeDelta * 0.1f);
		if (m_fTiltAngle < 0.f)
		{
			m_pTransformCom->Turn(vLook, -m_fTiltAngle * 0.1f);
			m_fTiltAngle = 0.f;
		}

	}
	else if (m_fTiltAngle < 0.f)
	{
		m_fTiltAngle += fTimeDelta;
		m_pTransformCom->Turn(vLook, fTimeDelta * 0.1f);
		if (m_fTiltAngle > 0.f)
		{
			m_pTransformCom->Turn(-vLook, m_fTiltAngle * 0.1f);
			m_fTiltAngle = 0.f;
		}
	}

}

_bool CPlayer::ReduceCurItemHP(void* pArg)
{
	if (m_pHoldItem == nullptr)
		return true;
	CItem_Equipment* eq = static_cast<CItem_Equipment*>(m_pHoldItem);
	WEAPONUIDESC we;
	if (m_pHoldItem != nullptr)
	{
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_BOTTLE:
			we.uiUITag = UI_WEAPON_BOTTLE;
			we.uiBulletCount = 0;
			break;
		case ITEM_PIPE:
			we.uiUITag = UI_WEAPON_LEADPIPE;
			we.uiBulletCount = 0;
			break;
		case ITEM_BOTTLE_BROKEN:
			we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
			we.uiBulletCount = 0;
			break;
		case ITEM_TOMMY_GUN:
			we.uiUITag = UI_WEAPON_TOMMYGUN;
			we.uiBulletCount = eq->GetBulletCount();
			break;
		case ITEM_AUTOMATIC_PISTOL:
			we.uiUITag = UI_WEAPON_AUTOPISTOL;
			we.uiBulletCount = eq->GetBulletCount();
			break;
		}
	}
	if (eq->GetHP() > 10)
	{
		eq->SetHP(eq->GetHP() - 10);
		we.fWeaponHP = eq->GetHP() / (float)eq->GetMaxHP();
		we.bIsRender = true;
		CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
	}
	else
	{
		eq->SetHP(0);
		if (m_pHoldItem != nullptr)
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_ITEM);
			switch (m_pHoldItem->Get_Item_Type())
			{
			case ITEM_BOTTLE:
			{
				CItem_Equipment* brokenBottle = new CItem_Equipment(m_pGraphic_Device);
				brokenBottle->SetBrokenBottle();
				m_pHoldItem = brokenBottle;
				SetPlayerTex(HEAVYREADY);
				SetPlayerTex2(BOTTLEBROKENIDLE);
				CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
				CGameInstance::Get_Instance()->OnEvent(EVENT_ADD_ITEM_IN_INVENTORY, brokenBottle);
				CGameInstance::Get_Instance()->Play_Sound(L"BottleBreak.wav", SOUND_ITEM, m_fSoundVolume);
				we.uiUITag = UI_WEAPON_BOTTLE;
				we.bIsRender = false;
				CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
				we.uiUITag = UI_WEAPON_BOTTLE_BROKEN;
				we.fWeaponHP = 1.f;
				we.bIsRender = true;
			}
			break;
			case ITEM_PIPE:
				SetPlayerTex(IDLE);
				m_pHoldItem = nullptr;
				CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
				CGameInstance::Get_Instance()->Play_Sound(L"PipeBreak.wav", SOUND_ITEM, m_fSoundVolume);
				we.bIsRender = false;
				break;
			case ITEM_BOTTLE_BROKEN:
				SetPlayerTex(IDLE);
				m_pHoldItem = nullptr;
				CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
				CGameInstance::Get_Instance()->Play_Sound(L"BottleBreak.wav", SOUND_ITEM, m_fSoundVolume);
				we.bIsRender = false;
				break;
			case ITEM_AUTOMATIC_PISTOL:
				SetPlayerTex(IDLE);
				m_pHoldItem = nullptr;
				CGameInstance::Get_Instance()->OnEvent(EVENT_REMOVE_ITEM_IN_INVENTORY);
				CGameInstance::Get_Instance()->Play_Sound(L"PistolBreak.wav", SOUND_ITEM, m_fSoundVolume);
				we.uiBulletCount = 0;
				we.bIsRender = false;
				break;
			}
			CGameInstance::Get_Instance()->OnEvent(EVENT_SET_WEAPON_UI, &we);
			SetPlayerTex(IDLE);
			m_curState->Exit(*this);
			delete m_curState;
			m_curState = new CPlayerIdleState(true);
			m_curState->Entry(*this);
		}
	}
	INVENHPDESC inven;
	inven.fItemHP = eq->GetHP() / (float)eq->GetMaxHP();
	inven.iUITag = m_pInventory->GetCurSlot() + 18;
	CGameInstance::Get_Instance()->OnEvent(EVENT_UPDATE_INVEN_WEAPON_HP, &inven);
	return true;
}

_bool CPlayer::HoldDelia(void* pArg)
{
	CGameInstance::Get_Instance()->StopSound(SOUND_PLAYER_HIT);
	CGameInstance::Get_Instance()->Play_Sound(TEXT("SwapBody1.wav"), SOUND_PLAYER_HIT, 1.f);
	m_bDeliaHold = true;
	m_curState->Exit(*this);
	delete m_curState;
	m_curState = new CPlayerSwapState();
	m_curState->Entry(*this);
	return true;
}

void CPlayer::Evade(_float fTimeDelta)
{
	if (!m_bEvade)
		return;

	m_fEvadeTime += fTimeDelta;

	if (m_fEvadeTime > 0.6f && m_bEvade)
	{
		m_fEvadeTime = 0.f;
		m_bEvade = false;
		m_pCamera->SetCameraPos({ 0.f,0.f,0.f });
		m_fSpeed = m_fSpeedInfoArray[SPEED_WALK];
		return;
	}

	_float fSpeed = max(m_fSpeed - m_fEvadeTime * 0.1f, 0);
	m_fSpeed = fSpeed;

	switch (m_eEvade)
	{
	case EVADE_A:
		m_pTransformCom->Go_Left_FixedY(fTimeDelta * m_fSpeed);
		break;
	case EVADE_SA:
		m_pTransformCom->Go_LeftDown_FixedY(fTimeDelta * m_fSpeed);
		break;
	case EVADE_S:
		m_pTransformCom->Go_Backward_FixedY(fTimeDelta * m_fSpeed);
		break;
	case EVADE_SD:
		m_pTransformCom->Go_RightDown_FixedY(fTimeDelta * m_fSpeed);
		break;
	case EVADE_D:
		m_pTransformCom->Go_Right_FixedY(fTimeDelta * m_fSpeed);
		break;
	}

	if (m_fEvadeTime < 0.2f)
	{
		m_pCamera->AddCameraPos({ 0.f,-fTimeDelta * 4.f ,0.f });
	}
	else
	{
		m_pCamera->AddCameraPos({ 0.f,fTimeDelta * 2.f ,0.f });
	}

}

void CPlayer::MouseMove(_float fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	RECT rc;
	GetWindowRect(g_hWnd, &rc);

	POINT		pCenter = { rc.left + (rc.right - rc.left) / 2,rc.top + (rc.bottom - rc.top) / 2 + 20 };
	SetCursorPos(pCenter.x, pCenter.y);
	_int		iX = { 0 };
	_int		iY = { 0 };

	if (ShowCursor(false) < -1)
		ShowCursor(true);

	/* 가로움직임이 있었다. */
	if (iX = ptMouse.x - m_ptOldMousePos.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iX * fTimeDelta * 0.05f);
	}

	if (iY = ptMouse.y - m_ptOldMousePos.y)
	{
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		if ((vLook.y > 0.7f && iY < 0) || (vLook.y < -0.7f && iY > 0))
		{
			iY = 0;
		}
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), iY * fTimeDelta * 0.05f);
	}

	m_fMouseDiff = { iX,iY };
	m_ptOldMousePos = pCenter;
}

void CPlayer::AnimMouseMove(float fTimeDelta)
{

	if (m_fMouseDiff.x == 0 && m_fMouseDiff.y == 0)
	{
		if (m_fMouseMovePos.x < 0)
		{
			m_fMouseMovePos.x += fTimeDelta * 1000.f;
			if (m_fMouseMovePos.x > 0.f)
				m_fMouseMovePos.x = 0.f;
		}
		else if (m_fMouseMovePos.x > 0)
		{
			m_fMouseMovePos.x -= fTimeDelta * 1000.f;
			if (m_fMouseMovePos.x < 0.f)
				m_fMouseMovePos.x = 0.f;
		}

		if (m_fMouseMovePos.y < 0)
		{
			m_fMouseMovePos.y += fTimeDelta * 1000.f;
			if (m_fMouseMovePos.y > 0.f)
				m_fMouseMovePos.y = 0.f;
		}
		else if (m_fMouseMovePos.y > 0)
		{
			m_fMouseMovePos.y -= fTimeDelta * 1000.f;
			if (m_fMouseMovePos.y < 0.f)
				m_fMouseMovePos.y = 0.f;
		}
	}
	else
	{
		m_fMouseMovePos.x -= m_fMouseDiff.x * 0.02f;
		m_fMouseMovePos.y -= m_fMouseDiff.y * 0.02f;
	}
}

void CPlayer::SetAnimPosIdle()
{
	if (!m_bDeliaHold)
	{
		m_vLPos.x = m_TexInfoArray[IDLE].vInitLPos.x - 100.f;
		m_vLPos.y = m_TexInfoArray[IDLE].vInitLPos.y + 100.f;
	}
	m_vRPos.x = m_TexInfoArray[IDLE].vInitRPos.x + 100.f;
	m_vRPos.y = m_TexInfoArray[IDLE].vInitRPos.y + 100.f;

	if (m_pHoldItem == nullptr)
		return;
	if (m_pHoldItem->Get_Item_Type() <= 100)
	{
		if (!m_bDeliaHold)
		{
			m_vLPos.x = m_TexInfoArray[HEAVYREADY].vInitLPos.x - 100.f;
			m_vLPos.y = m_TexInfoArray[HEAVYREADY].vInitLPos.y + 100.f;
		}
		switch (m_pHoldItem->Get_Item_Type())
		{
		case ITEM_PIPE:
			m_vRPos.x = m_TexInfoArray[PIPEIDLE].vInitRPos.x + 100.f;
			m_vRPos.y = m_TexInfoArray[PIPEIDLE].vInitRPos.y + 100.f;
			break;
		case ITEM_BOTTLE:
			m_vRPos.x = m_TexInfoArray[BOTTLEIDLE].vInitRPos.x + 100.f;
			m_vRPos.y = m_TexInfoArray[BOTTLEIDLE].vInitRPos.y + 100.f;
		case ITEM_BOTTLE_BROKEN:
			m_vRPos.x = m_TexInfoArray[BOTTLEBROKENIDLE].vInitRPos.x + 100.f;
			m_vRPos.y = m_TexInfoArray[BOTTLEBROKENIDLE].vInitRPos.y + 100.f;
			break;
		case ITEM_TOMMY_GUN:
			m_vRPos.x = m_TexInfoArray[FIREIDLE].vInitRPos.x + 100.f;
			m_vRPos.y = m_TexInfoArray[FIREIDLE].vInitRPos.y + 100.f;
			break;
		case ITEM_AUTOMATIC_PISTOL:
			m_vRPos.x = m_TexInfoArray[PISTOLIDLE].vInitRPos.x + 100.f;
			m_vRPos.y = m_TexInfoArray[PISTOLIDLE].vInitRPos.y + 100.f;
			break;
		}
	}
	else
	{
		if (!m_bDeliaHold)
		{
			m_vLPos.x = m_TexInfoArray[MEDIKIT].vInitLPos.x - 100.f;
			m_vLPos.y = m_TexInfoArray[MEDIKIT].vInitLPos.y + 100.f;
		}
		m_vRPos.x = m_TexInfoArray[MEDIKIT].vInitRPos.x + 100.f;
		m_vRPos.y = m_TexInfoArray[MEDIKIT].vInitRPos.y + 100.f;
	}
}

_bool CPlayer::AnimToIdle(float fTimeDelta)
{
	PLAYER_TEX LeftTex = IDLE, RightTex = IDLE;
	if (m_pHoldItem == nullptr)
	{
		if (!m_bDeliaHold)
			LeftTex = IDLE;
		else
			LeftTex = DELIAHOLD;
		RightTex = IDLE;
	}
	else
	{
		if (m_pHoldItem->Get_Item_Type() <= 100)
		{
			switch (m_pHoldItem->Get_Item_Type())
			{
			case ITEM_PIPE:
				LeftTex = HEAVYREADY;
				RightTex = PIPEIDLE;
				break;
			case ITEM_BOTTLE:
				LeftTex = HEAVYREADY;
				RightTex = BOTTLEIDLE;
			case ITEM_BOTTLE_BROKEN:
				LeftTex = HEAVYREADY;
				RightTex = BOTTLEBROKENIDLE;
				break;
			case ITEM_TOMMY_GUN:
				LeftTex = HEAVYREADY;
				RightTex = FIREIDLE;
				break;
			case ITEM_AUTOMATIC_PISTOL:
				LeftTex = HEAVYREADY;
				if (!m_bPistolMeelee)
					RightTex = PISTOLIDLE;
				else
					RightTex = PISTOLMEELEE;
				break;
			}
		}
		else
		{
			LeftTex = HEAVYREADY;
			RightTex = MEDIKIT;
		}
	}
	if (!m_bDeliaHold)
	{
		if (m_vLPos.x < m_TexInfoArray[LeftTex].vInitLPos.x)
			m_vLPos.x += fTimeDelta * 500.f;
		if (m_vLPos.y > m_TexInfoArray[LeftTex].vInitLPos.y)
			m_vLPos.y -= fTimeDelta * 500.f;
	}
	else
	{
		m_vLPos.x = m_TexInfoArray[DELIAHOLD].vInitLPos.x;
		m_vLPos.y = m_TexInfoArray[DELIAHOLD].vInitLPos.y;
	}
	if (m_vRPos.x > m_TexInfoArray[RightTex].vInitRPos.x)
		m_vRPos.x -= fTimeDelta * 500.f;
	if (m_vRPos.y > m_TexInfoArray[RightTex].vInitRPos.y)
		m_vRPos.y -= fTimeDelta * 500.f;

	if (m_vLPos.x >= m_TexInfoArray[LeftTex].vInitLPos.x &&
		m_vLPos.y <= m_TexInfoArray[LeftTex].vInitLPos.y &&
		m_vRPos.x <= m_TexInfoArray[RightTex].vInitRPos.x &&
		m_vRPos.y <= m_TexInfoArray[RightTex].vInitRPos.y)
	{
		return true;
	}
	return false;
}

void CPlayer::AnimIdle(float fTimeDelta)
{
	m_fTime += fTimeDelta * 2.f;
	m_vLPos.x -= sinf(m_fTime) * 0.2f;
	m_vLPos.y += sinf(m_fTime) * 0.2f;
	m_vRPos.x += sinf(m_fTime + 3.14f / 2.f) * 0.2f;
	m_vRPos.y += sinf(m_fTime + 3.14f / 2.f) * 0.2f;

}

void CPlayer::AnimWalk(float fTimeDelta)
{
	m_fTime += fTimeDelta * 8.f;
	m_vLPos.x += sinf(m_fTime) * 4.f;
	m_vLPos.y += sinf(m_fTime * 2.f) * 2.f;
	m_vRPos.x += sinf(m_fTime) * 4.f;
	m_vRPos.y += sinf(m_fTime * 2.f) * 2.f;
}

void CPlayer::AnimRun(float fTimeDelta)
{
	m_fTime += fTimeDelta * 12.f;
	//m_vLPos.x += sinf(m_fTime) * 4.f;
	m_vLPos.y += sinf(m_fTime) * 40.f;
	//m_vRPos.x += sinf(m_fTime) * 4.f;
	m_vRPos.y += sinf(m_fTime + 3.14f) * 40.f;
}

PLAYER_PUNCH CPlayer::AnimPunch(_bool AttackEnd, _float fTimeDelta)
{
	//펀치 내지르기 후
	if (AttackEnd)
	{
		m_fTime += fTimeDelta;
		if (m_bPunchLeft)
		{
			if (!m_bDeliaHold)
			{
				m_vLPos.x -= fTimeDelta * 2000.f;
				m_vLPos.y -= logf(m_fTime) * 15.f;
			}

			if (m_vRPos.x > m_TexInfoArray[IDLE].vInitRPos.x)
				m_vRPos.x -= fTimeDelta * 500.f;
			if (m_vRPos.y > m_TexInfoArray[IDLE].vInitRPos.y)
				m_vRPos.y -= fTimeDelta * 500.f;

			if (m_vLPos.x < m_TexInfoArray[PUNCH].vInitLPos.x)
				return PUNCH_FINISH;
			else
				return PUNCH_BACK;
		}
		else
		{
			m_vRPos.x += fTimeDelta * 2000.f;
			m_vRPos.y -= logf(m_fTime) * 15.f;

			if (!m_bDeliaHold)
			{
				if (m_vLPos.x < m_TexInfoArray[IDLE].vInitLPos.x)
					m_vLPos.x += fTimeDelta * 500.f;
				if (m_vLPos.y > m_TexInfoArray[IDLE].vInitLPos.y)
					m_vLPos.y -= fTimeDelta * 500.f;
			}

			if (m_vRPos.x > m_TexInfoArray[PUNCH].vInitRPos.x)
				return PUNCH_FINISH;
			else
				return PUNCH_BACK;
		}
	}
	//펀치 내지르기 
	else
	{
		if (m_bPunchLeft)
		{
			if (m_vLPos.x > 650.f)
			{
				m_vLPos.x += fTimeDelta * 500.f;
			}
			else
			{
				m_vLPos.x += fTimeDelta * 2000.f;
				m_vLPos.y -= fTimeDelta * 8000.f;
			}

			m_vRPos.x += fTimeDelta * 500.f;
			m_vRPos.y += fTimeDelta * 500.f;

			if (m_vLPos.x > 700.f)
				return PUNCH_BACK;
			else
				return PUNCH_FORWARD;
		}
		else
		{
			if (m_vRPos.x < g_iWinSizeX - 650.f)
			{
				m_vRPos.x -= fTimeDelta * 500.f;
			}
			else
			{
				m_vRPos.x -= fTimeDelta * 2000.f;
				m_vRPos.y -= fTimeDelta * 8000.f;
			}

			m_vLPos.x -= fTimeDelta * 500.f;
			m_vLPos.y += fTimeDelta * 500.f;

			if (m_vRPos.x < g_iWinSizeX - 700.f)
				return PUNCH_BACK;
			else
				return PUNCH_FORWARD;
		}
	}
}

void CPlayer::AnimHeavyPunch(_bool bCharge, _float fTime, _float fDeltaTime)
{
	if (bCharge)
	{
		if (fTime > 0.f && fTime < 0.3f)
		{
			if (!m_bDeliaHold)
			{
				m_vLPos.x -= fDeltaTime * 500.f;
				m_vLPos.y += fDeltaTime * 500.f;
			}
		}
		if (fTime > 0.f && fTime < 0.15f)
		{
			m_vRPos.x += fDeltaTime * 2500.f;
			m_vRPos.y += fDeltaTime * 800.f;
		}
		else if (fTime >= 0.15f && fTime < 0.3f)
		{
			m_vRPos.x += fDeltaTime * 1800.f;
			m_vRPos.y -= fDeltaTime * 2400.f;
		}
	}
	else
	{
		if (fTime >= 0.f && fTime < 0.05f)
		{
			if (!m_bDeliaHold)
				m_vLPos.y += fDeltaTime * 2000.f;
			m_vRPos.x -= fDeltaTime * 7000.f;
			m_vRPos.y += fDeltaTime * 3000.f;
		}
		else if (fTime >= 0.05f && fTime < 0.07f)
		{
			if (!m_bDeliaHold)
				m_vRPos.x += fDeltaTime * 1000.f;
		}
		else if (fTime >= 0.07f && fTime < 0.1f)
		{
			m_vRPos.x -= fDeltaTime * 1000.f;
		}
		else if (fTime >= 0.1f && fTime < 0.2f)
		{
			m_vRPos.x -= fDeltaTime * 300.f;
			m_vRPos.y += fDeltaTime * 200.f;
		}
		else if (fTime >= 0.2f && fTime < 0.3f)
		{
			m_vRPos.y += fDeltaTime * 50.f;
		}
		else if (fTime >= 0.2f && fTime < 0.5f)
		{
			if (!m_bDeliaHold)
			{
				m_vLPos.x += fDeltaTime * 500.f;
				m_vLPos.y -= fDeltaTime * 1000.f;
			}

			m_vRPos.x += fDeltaTime * 1200.f;
			m_vRPos.y += fDeltaTime * 1000.f;
		}
	}
}

void CPlayer::AnimGuard(_bool bGuardEnd, _float fTime, _float fTimeDelta)
{
	if (!bGuardEnd)
	{
		if (fTime > 0.f && fTime < 0.15f)
		{
			if (!m_bDeliaHold)
			{
				m_vLPos.x += fTimeDelta * 600.f;
				m_vLPos.y -= fTimeDelta * 2400.f;
			}
			m_vRPos.x -= fTimeDelta * 600.f;
			m_vRPos.y -= fTimeDelta * 2400.f;
		}
	}
	else
	{
		if (fTime > 0.f && fTime < 0.15f)
		{
			if (!m_bDeliaHold)
			{
				m_vLPos.x -= fTimeDelta * 1200.f;
				m_vLPos.y += fTimeDelta * 4000.f;
			}
			m_vRPos.x += fTimeDelta * 1200.f;
			m_vRPos.y += fTimeDelta * 4000.f;
		}
	}
}

void CPlayer::AnimKick(_float fTime, _float fTimeDelta)
{
	if (fTime > 0.f && fTime < 0.05f)
	{
		SetPlayerTex(KICK2);
	}
	else if (fTime >= 0.05f && fTime < 0.1f)
	{
		SetPlayerTex(KICK3);
	}
	else if (fTime >= 0.1 && fTime < 0.15f)
	{
		SetPlayerTex(KICK4);
	}
	else if (fTime >= 0.15 && fTime < 0.2f)
	{
		m_vRPos.x -= fTimeDelta * 50.f;
		m_vRPos.y -= fTimeDelta * 700.f;
	}
	else if (fTime >= 0.2 && fTime < 0.3f)
	{
		m_vRPos.x += fTimeDelta * 20.f;
	}
	else if (fTime >= 0.3 && fTime < 0.4f)
	{
		m_vRPos.x -= fTimeDelta * 20.f;
	}
	else if (fTime >= 0.4 && fTime < 0.45f)
	{
		m_vRPos.x += fTimeDelta * 50.f;
		m_vRPos.y += fTimeDelta * 700.f;
	}
	else if (fTime >= 0.45 && fTime < 0.5f)
	{
		SetPlayerTex(KICK3);
	}
	else if (fTime >= 0.5 && fTime < 0.55f)
	{
		SetPlayerTex(KICK2);
	}
	else if (fTime >= 0.55 && fTime < 0.6f)
	{
		SetPlayerTex(KICK1);
	}
}

_bool CPlayer::On_ExecutionSuccess(void* pArg)
{
	EXECUTECHECKDESC* pTarget = static_cast<EXECUTECHECKDESC*>(pArg);
	if (pTarget != nullptr) // 성공
	{
		m_pExecutionTarget = pTarget->pExecutionTarget;
		bBack = pTarget->bToBeChopped;
		m_eExecuteType = pTarget->eType;
	}
	return true;
}
