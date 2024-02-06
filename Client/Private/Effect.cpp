#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "EffectManager.h"

_uint CEffect::INDEX_THRESHOLD = 0;
_int CEffect::WOOD_INDEX = 0;

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject(pGraphic_Device)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CBlendObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{

	//���̾�� ��� ����Ʈ������Ʈ �޾ƿ�
	/*EFFECT_TYPE* pEffect_Type = (EFFECT_TYPE*)pArg;
	m_eEffectType = *pEffect_Type;*/
	
	EFFECTARG* pEffectArg = (EFFECTARG*)pArg;
	m_eEffectType = (EFFECT_TYPE)pEffectArg->eEffecttype;


	m_iEffectIndex = INDEX_THRESHOLD;
	++INDEX_THRESHOLD;

	if (m_eEffectType == EFFECT_WOODDEBRISSPRITES)
	{
		WOODDEBRISDESC tWoodDesc;
		m_WoodIndexList[0] = pEffectArg->iEffectIdentity[9][0];
		m_WoodIndexList[1] = pEffectArg->iEffectIdentity[9][1];
		m_WoodIndexList[2] = pEffectArg->iEffectIdentity[9][2];
		m_WoodIndexList[3] = pEffectArg->iEffectIdentity[9][3];
		m_WoodIndexList[4] = pEffectArg->iEffectIdentity[9][4];
		m_WoodIndexList[5] = pEffectArg->iEffectIdentity[9][5];
		m_WoodIndexList[6] = pEffectArg->iEffectIdentity[9][6];
		m_WoodIndexList[7] = pEffectArg->iEffectIdentity[9][7];
		m_WoodIndexList[8] = pEffectArg->iEffectIdentity[9][8];
		m_WoodIndexList[9] = pEffectArg->iEffectIdentity[9][9];
	}

	/* ��׶��尡 ����ϱ����� ������Ʈ���� �߰��ϳ�. */
	if (FAILED(Add_Components()))
		return E_FAIL;
	if (m_eEffectType)
	{
		//m_pTransformCom->Scaling({ 2.f, 2.f, 1.f });
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0, 0, 0));
		
	}
	
	
	
	
	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	

	m_pTransformCom->SetNextPos(_float3(0.f, 0.f, 0.f));

	if (true == m_bActive)
	{
		__super::Tick(fTimeDelta);

		if (EFFECT_BLOOD_IMPACTSTRIP == m_eEffectType)
		{
			m_fFrame += 14.0f * fTimeDelta;

			if (m_fFrame >= 14.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}
			m_pTransformCom->SetNextPos(m_vEffectRanPos * fTimeDelta * 20.f);
			m_pTransformCom->SetNextPos(m_pTransformCom->GetNextPos() + _float3(0.f, -fTimeDelta*m_fFrame, 0.f)); 
			
		}
		if (EFFECT_POW == m_eEffectType)
		{
			
			m_fFrame += 18.0f * fTimeDelta;

			if (m_fFrame >= 4.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}
			
		}
		if (EFFECT_STRIP == m_eEffectType)
		{
			m_fFrame += 18.0f * fTimeDelta;

			if (m_fFrame >= 16.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

		}

		if (EFFECT_DAZESWIRL == m_eEffectType)
		{
			if (m_pTargetTransform == nullptr)
			{
				MSG_BOX("nullptr");
			}
			// Position
			_float3 vMove = { 0.f, 0.75f, 0.f };
			_float3 vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
			_float3 vNextPos = m_pTargetTransform->Get_State(CTransform::STATE_POSITION) + vMove
				- m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->SetNextPos(vNextPos);

			m_fFrame += 12.f * fTimeDelta;

			if (m_fFrame >= 4.0f)
			{
				m_fFrame = 0.f;
				//m_bActive = false;
			}

		}

		if (EFFECT_GROSSDEBRIS == m_eEffectType)
		{
			m_fFrame += 15.0f * fTimeDelta;

			if (m_fFrame >= 14.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

		}

		if (EFFECT_SMOKEIMPACTUP == m_eEffectType)
		{
			m_fFrame += 18.0f * fTimeDelta;

			if (m_fFrame >= 18.0f)
			{
				m_fFrame = 0.f;
				//m_bActive = false;
			}

		}

		if (EFFECT_SMOKING == m_eEffectType)
		{
			m_fFrame += 16.0f * fTimeDelta;

			if (m_fFrame >= 16.0f)
			{
				m_fFrame = 0.f;
				//m_bActive = false;
			}

		}

		if (EFFECT_SPARKSTRIP == m_eEffectType)
		{
			m_fFrame += 14.0f * fTimeDelta;

			if (m_fFrame >= 14.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

		}

		if (EFFECT_WOODDEBRISSPRITES == m_eEffectType)
		{
			m_fFrame += 5.0f * fTimeDelta;

			if (m_fFrame >= 11.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

		}

		if (EFFECT_BLOOD_MIST == m_eEffectType)
		{
			m_fFrame += 17.0f * fTimeDelta;

			if (m_fFrame >= 17.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

		}
		if (EFFECT_BULLET_HOLES == m_eEffectType)
		{
			m_fFrame += 1.0f * fTimeDelta;

			if (m_fFrame >= 1.0f)
			{
				m_fFrame = 0.f;
				//m_bActive = false;
			}

		}

		if (EFFECT_FIRE_STRIP == m_eEffectType)
		{
			m_fFrame += fTimeDelta * 0.5f;

			if (m_fFrame >= 16.0f)
			{
				m_fFrame = 0.f;
				m_bActive = false;
			}

			m_vDirection += _float3(0.f, 1.f, 0.f) * m_fFrame * 0.000005f;
			m_pTransformCom->SetNextPos(m_vDirection);
		}


	}
	

	
}

void CEffect::Late_Tick(_float fTimeDelta)
{

	if (false == m_bActive)
		return;

	__super::Late_Tick(fTimeDelta);

	//to billboard
	_float4x4		CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);


	D3DXVECTOR3 vScale = m_pTransformCom->Get_Scaled();
	_float3 vRight, vUp, vLook;
	vRight = *(_float3*)&CameraWorldMatrix.m[0][0] * vScale.x;
	vLook = *(_float3*)&CameraWorldMatrix.m[2][0] * vScale.y;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&CameraWorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	if (m_eEffectType == EFFECT_DAZESWIRL)
	{
		m_pTransformCom->Turn({ 1.f,0.f,0.f }, D3DXToRadian(20));
	}
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,vPos +  m_pTransformCom->GetNextPos());
	if (m_eEffectType == EFFECT_SMOKING)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
	
	
}

HRESULT CEffect::Render()
 {

	if (true == m_bActive)
	{

		if (nullptr == m_pVIBufferCom)
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());
		//_float4x4 ViewMatrix;
		//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		/*m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ViewMatrix);*/
		//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &ViewMatrix);

		_float4x4 WorldMatrix;
		_float4x4 ViewMatrix;
		_float4x4 ProjMatrix;
		_float4x4		Scaling, Translation;
		switch (m_eEffectType)
		{
		case(EFFECT_BLOOD_IMPACTSTRIP):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_POW):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			
			break;
		case(EFFECT_STRIP):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			
			break;
		case(EFFECT_SMASHED):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_DAZESWIRL):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;

			m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case(EFFECT_GROSSDEBRIS):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_SMOKEIMPACTUP):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_SMOKING):
				
			WorldMatrix = *D3DXMatrixScaling(&Scaling, 128.f, 162.f, 300.f) * *D3DXMatrixTranslation(&Translation, 433.f, 49.f, 0.1f);
			
				ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
				D3DXMatrixOrthoLH(&ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);
				m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMatrix);
				m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
				m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); //�����׽���
			m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0); // �����Ӱ��� ����
			m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			break;
		case(EFFECT_SPARKSTRIP):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_WOODDEBRISSPRITES):
			if (FAILED(m_pTexWoodCom[0]->Bind_OnGraphicDevice(0)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[1]->Bind_OnGraphicDevice(1)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[2]->Bind_OnGraphicDevice(2)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[3]->Bind_OnGraphicDevice(3)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[4]->Bind_OnGraphicDevice(4)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[5]->Bind_OnGraphicDevice(5)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[6]->Bind_OnGraphicDevice(6)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[7]->Bind_OnGraphicDevice(7)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[8]->Bind_OnGraphicDevice(8)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[9]->Bind_OnGraphicDevice(9)))
				return E_FAIL;
			if (FAILED(m_pTexWoodCom[10]->Bind_OnGraphicDevice(10)))
				return E_FAIL;
			break;
		case(EFFECT_BLOOD_MIST):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case(EFFECT_BULLET_HOLES):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;
			break;
		case(EFFECT_FIRE_STRIP):
			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
				return E_FAIL;

			break;
		}



		///* ��ġ�� �ؽ��ĸ� ���ε��Ѵ�. */
		//�̷��� �ϸ� ���ÿ� ����Ʈ �ȳ��´ٴ� ����, ������ ������...
		//if (EFFECT_BLOOD_IMPACTSTRIP == m_eEffectType)
		//{
		//	if (FAILED(m_pTexEffect_Type[EFFECT_BLOOD_IMPACTSTRIP]->Bind_OnGraphicDevice((_uint)m_fFrame)))
		//		return E_FAIL;
		//}

		//if (EFFECT_POW == m_eEffectType )
		//{
		//	if (FAILED(m_pTexEffect_Type[EFFECT_POW]->Bind_OnGraphicDevice((_uint)m_fFrame)))
		//		return E_FAIL;
		//}

		//if (EFFECT_STRIP == m_eEffectType)
		//{
		//	if (FAILED(m_pTexEffect_Type[EFFECT_STRIP]->Bind_OnGraphicDevice((_uint)m_fFrame)))
		//		return E_FAIL;
		//}
		//
		//if (EFFECT_SMASHED == m_eEffectType)
		//{
		//	if (FAILED(m_pTexEffect_Type[EFFECT_SMASHED]->Bind_OnGraphicDevice((_uint)m_fFrame)))
		//		return E_FAIL;
		//}
	

		
		
		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		if (FAILED(Release_RenderState()))
			return E_FAIL;

	}
	
	return S_OK;
	
}

_bool CEffect::OnEvent(EFFECT_TYPE eEffectType, void* pArg)
{
	switch (eEffectType)
	{
	case EFFECT_BLOOD_IMPACTSTRIP:
		return Effect_Blood_Impact_Strip(pArg);
	case EFFECT_POW:
		return Effect_Pow(pArg);
	case EFFECT_STRIP:
		return Effect_Strip(pArg);
	case EFFECT_SMASHED:
		return Effect_Smashed(pArg);
	case EFFECT_DAZESWIRL:
		return Effect_DazeSwirl(pArg);
	case EFFECT_GROSSDEBRIS:
		return Effect_GrossDebris(pArg);
	case EFFECT_SMOKEIMPACTUP:
		return Effect_SmokeImpactUp(pArg);
	case EFFECT_SMOKING:
		return Effect_Smoking(pArg);
	case EFFECT_SPARKSTRIP:
		return Effect_SparkStrip(pArg);
	case EFFECT_WOODDEBRISSPRITES:
		return Effect_WoodDebrisSprites(pArg);
	case EFFECT_BLOOD_MIST:
		return Effect_BloodMist(pArg);
	case EFFECT_BULLET_HOLES:
		return Effect_BulletHoles(pArg);
	case EFFECT_FIRE_STRIP:
		return Effect_FireStrip(pArg);
	}
	return _bool();
}

void CEffect::TurnOff()
{
	m_bActive = false;
	m_fFrame = 0.f;
}



_bool CEffect::Effect_Blood_Impact_Strip(void* pArg)
{
	if (m_eEffectType == EFFECT_BLOOD_IMPACTSTRIP)
	{
		
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_BLOOD_IMPACTSTRIP;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.3f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos;
	
		// ������ �������� �߰��Ͽ� ��ŷ�� �� ��ó���� ����Ʈ�� �����ǰ� ��
		 
		m_vEffectRanPos.x = (rand() % 6 - 3) * 0.05f;  // x�� �������� ���� ������
		m_vEffectRanPos.y = (rand() % 6 - 3) * 0.05f;  // y�� �������� ���� ������
		m_vEffectRanPos.z = (rand() % 6 - 3) * 0.05f;  // z�� �������� ���� ������ �̰� �ּ�ó��������

		effectPosition += m_vEffectRanPos;

		//distanceToCamera = (rand() % 3) * distanceToCamera;//
		effectPosition += vDir * distanceToCamera; /*distanceToCamera*/;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		//m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
	
}

_bool CEffect::Handle_Effect(void* pArg)
{
	

	return true;
}

_bool CEffect::Effect_Pow(void* pArg)
{
	if (m_eEffectType == EFFECT_POW)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_POW;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.1f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_Strip(void* pArg)
{
	if (m_eEffectType == EFFECT_STRIP)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		m_eEffectType = EFFECT_STRIP;
		m_bActive = true;
		//m_bEffectActive = true;
		//m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_Smashed(void* pArg)
{
	if (m_eEffectType == EFFECT_SMASHED)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//�÷��̾ ���͸� ������
		m_eEffectType = EFFECT_SMASHED;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effect->vPos);
		m_pTransformCom->Scaling({ effect->vScale });
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.3f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}



_bool CEffect::Effect_DazeSwirl(void* pArg)
{
	if (m_eEffectType == EFFECT_DAZESWIRL)
	{
		//EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		EFFECTDESC2* effect = static_cast<EFFECTDESC2*>(pArg);
		
		//2,3,5
		m_eEffectType = EFFECT_DAZESWIRL;
		m_bActive = true;
		m_bActive = effect->bActive;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		m_pTargetTransform = effect->pTargetTransform;
		//rotation
	
		_float4x4		CameraWorldMatrix;





		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos; //�����ִ°ſ��� ����� ����
		D3DXVec3Normalize(&vDir, &vDir);



		



		float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
		
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });
		
		effect->iIndex = m_iEffectIndex;

		return true;
	}
}

_bool CEffect::Effect_GrossDebris(void* pArg)
{
	if (m_eEffectType == EFFECT_GROSSDEBRIS)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_POW;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.1f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_SmokeImpactUp(void* pArg)
{
	if (m_eEffectType == EFFECT_SMOKEIMPACTUP)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_BLOOD_IMPACTSTRIP;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.3f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos;

		// ������ �������� �߰��Ͽ� ��ŷ�� �� ��ó���� ����Ʈ�� �����ǰ� ��

		m_vEffectRanPos.x = (rand() % 6 - 3) * 0.3f;  // x�� �������� ���� ������
		m_vEffectRanPos.y = (rand() % 6 - 3) * 0.3f;  // y�� �������� ���� ������
		//m_vEffectRanPos.z = (rand() % 6 - 3) * 0.05f;  // z�� �������� ���� ������

		effectPosition += m_vEffectRanPos;

		//distanceToCamera = (rand() % 3) * distanceToCamera;//������ �̰� �������ƺ�
		effectPosition += vDir * distanceToCamera; /*distanceToCamera*/;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		//m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;






		//EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		////2,3,5
		////m_eEffectType = EFFECT_STRIP;
		//m_bActive = true;
		////m_bEffectActive = true;
		////m_bEffectPlaying = true;
		//m_pTransformCom->Scaling(effect->vScale);
		//_float4x4		CameraWorldMatrix;

		//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		//D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		//_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		//_float3 vDir = vCamaraPos - effect->vPos;
		//D3DXVec3Normalize(&vDir, &vDir);

		//float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		//_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		//m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		//return true;
	}
}

_bool CEffect::Effect_Smoking(void* pArg)
{
	if (m_eEffectType == EFFECT_SMOKING)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_STRIP;
		m_bActive = true;
		//m_bEffectActive = true;
		//m_bEffectPlaying = true;
		/*if (EFFECT_SMOKING == m_eEffectType)
		{
			_float4x4 ViewMatrix;
			_float4x4 ProjMatrix;

			ViewMatrix = *D3DXMatrixIdentity(&ViewMatrix);
			D3DXMatrixOrthoLH(&ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.0f, 1.f);
			m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
		}*/
		//m_pTransformCom->Scaling(effect->vScale);
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effect->vPos);

	

		
		
		//m_pGraphic_Device->GetTransform(d3dts_view)
		//_float4x4		CameraWorldMatrix;

		//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		//D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		//_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		//_float3 vDir = vCamaraPos - effect->vPos;
		//D3DXVec3Normalize(&vDir, &vDir);

		//float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		//_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		//m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_SparkStrip(void* pArg)
{
	if (m_eEffectType == EFFECT_SPARKSTRIP)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_STRIP;
		m_bActive = true;
		//m_bEffectActive = true;
		//m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_WoodDebrisSprites(void* pArg)
{
	if (m_eEffectType == EFFECT_WOODDEBRISSPRITES)
	{
		
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_BLOOD_IMPACTSTRIP;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.3f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos;

		// ������ �������� �߰��Ͽ� ��ŷ�� �� ��ó���� ����Ʈ�� �����ǰ� ��

		m_vEffectRanPos.x = (rand() % 6 - 3) * 0.03f;  // x�� �������� ���� ������
		m_vEffectRanPos.y = (rand() % 6 - 3) * 0.03f;  // y�� �������� ���� ������
		m_vEffectRanPos.z = (rand() % 6 - 3) * 0.03f;  // z�� �������� ���� ������ �̰� �ּ�ó��������

		effectPosition += m_vEffectRanPos;

		//distanceToCamera = (rand() % 3) * distanceToCamera;//
		effectPosition += vDir * distanceToCamera; /*distanceToCamera*/;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);

		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		//m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_BloodMist(void* pArg)
{
	if (m_eEffectType == EFFECT_BLOOD_MIST)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_POW;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.1f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_BulletHoles(void* pArg)
{
	if (m_eEffectType == EFFECT_BULLET_HOLES)
	{
		EFFECTDESC* effect = static_cast<EFFECTDESC*>(pArg);
		//2,3,5
		//m_eEffectType = EFFECT_POW;
		m_bActive = true;
		m_bEffectPlaying = true;
		m_pTransformCom->Scaling(effect->vScale);
		_float4x4		CameraWorldMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

		D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

		_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

		_float3 vDir = vCamaraPos - effect->vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float distanceToCamera = 0.1f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
		_float3 effectPosition = effect->vPos + vDir * distanceToCamera;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

		return true;
	}
}

_bool CEffect::Effect_FireStrip(void* pArg)
{
	MOVEABLEEFFECTDESC* tEffectDesc = static_cast<MOVEABLEEFFECTDESC*>(pArg);

	m_eEffectType = EFFECT_FIRE_STRIP;
	m_bActive = true;
	m_pTransformCom->Scaling(tEffectDesc->vScale);
	_float4x4		CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	_float3 vCamaraPos = { CameraWorldMatrix._41, CameraWorldMatrix._42, CameraWorldMatrix._43 };

	_float3 vDir = vCamaraPos - tEffectDesc->vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	float distanceToCamera = 0.2f;  // �̰ɷ� ����Ʈ ��ġ �����ؾߵ�
	_float3 effectPosition = tEffectDesc->vPos + vDir * distanceToCamera;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, effectPosition);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, { 0.f, 0.f, 1.f });

	m_vDirection = tEffectDesc->vDir;

	return true;
}


HRESULT CEffect::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */

	if (EFFECT_BLOOD_IMPACTSTRIP == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BloodImpactStrip"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_POW == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Pow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

	}

	else if (EFFECT_STRIP == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Strip"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (EFFECT_FIRE_STRIP == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smoke_Strip"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_SMASHED == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smashed"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_DAZESWIRL == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_DazeSwirl"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_GROSSDEBRIS == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_GrossDebris"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_SMOKEIMPACTUP == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_SmokeImpactUp"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_SMOKING == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_Smoking"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_SPARKSTRIP == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_SparkStrip"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_WOODDEBRISSPRITES == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture0"), (CComponent**)&m_pTexWoodCom[0])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture1"), (CComponent**)&m_pTexWoodCom[1])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture2"), (CComponent**)&m_pTexWoodCom[2])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture3"), (CComponent**)&m_pTexWoodCom[3])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture4"), (CComponent**)&m_pTexWoodCom[4])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture5"), (CComponent**)&m_pTexWoodCom[5])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture6"), (CComponent**)&m_pTexWoodCom[6])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture7"), (CComponent**)&m_pTexWoodCom[7])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture8"), (CComponent**)&m_pTexWoodCom[8])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture9"), (CComponent**)&m_pTexWoodCom[9])))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_WoodDebrisSprites"),
			TEXT("Com_Texture10"), (CComponent**)&m_pTexWoodCom[10])))
			return E_FAIL;
	}

	else if (EFFECT_BLOOD_MIST == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BloodMist"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else if (EFFECT_BULLET_HOLES == m_eEffectType)
	{
		if (FAILED(__super::Add_Component(LEVEL_OUTDOOR, TEXT("Prototype_Component_Texture_Effect_BulletHoles"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	/* For.Com_Transform */
	/* �� ��ü���� Ʈ�������� �����Ͽ� ��ü�ȿ� ������ ��! ��ü�� ����, ȸ���ؾ��Ѥ� �ӵ��� ������ ���̴�. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/*if (EFFECT_WOODDEBRISSPRITES == m_eEffectType)
	{
		for (_int i = 0; i < m_iWoodIndex; ++i)
		{
			if (m_iWoodIndex == i)
			{
				CTransform::TRANSFORMDESC		TransformDesc;
				TransformDesc.fSpeedPerSec = 20.f;
				TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
					TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
					return E_FAIL;
			}
		}
		*/
		/*else if (m_iWoodIndex == 1)
		{
			CTransform::TRANSFORMDESC		TransformDesc;
			TransformDesc.fSpeedPerSec = 30.f;
			TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
				TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
				return E_FAIL;
		}*/
	//}


	return S_OK;
}

HRESULT CEffect::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
*/
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); //�����׽���
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000008); // �����Ӱ��� ����
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CEffect::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect* pInstance = new CEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CEffect");
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CEffect");
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	for (int i = 0; i < 11; ++i)
	{
		Safe_Release(m_pTexWoodCom[i]);
	}
	//Safe_Delete_Array(m_pTexWoodCom[10]);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
