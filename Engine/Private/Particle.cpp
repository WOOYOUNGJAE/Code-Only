#include "Particle.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Physics.h"


CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CParticle::CParticle(const CParticle& rhs)
	:CGameObject(rhs)
{
}

HRESULT CParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	PARTICLEDESC* pParticleDesc = (PARTICLEDESC*)pArg;

	m_TextureTag = pParticleDesc->strParticleTexTag;
	m_iIndex = pParticleDesc->iIndex;
	m_ParticleType = pParticleDesc->ParticleType;
	m_fSpread = pParticleDesc->fSpread;
	m_iStatic = pParticleDesc->iStatic;
	m_iCurrentLevel = pParticleDesc->iCurrentLevel;
	m_iTexIndex = 0;
	m_height = 0.2f;
	m_bRender = false;
	m_fGravityStr = pParticleDesc->fGravityStr;
	m_fDirStr = pParticleDesc->fDirStr;
	m_vec3ParticleSize = pParticleDesc->vec3ParticleSize;
	
	if (FAILED(Add_Components()))
		return E_FAIL;
	if (PARTICLE_BLOOD != m_ParticleType)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * m_vec3ParticleSize.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 1.f, 0.f) * m_vec3ParticleSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0.f, 0.f, 1.f) * m_vec3ParticleSize.z);
		
	}
	
	//m_pPhysicsCom->SetAddForce(*D3DXVec3Normalize(&m_Dir, &m_Dir), 10.f);
	if (PARTICLE_BLOOD == m_ParticleType)
	{

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f) * m_vec3ParticleSize.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 0.f, 1.f) * m_vec3ParticleSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0.f, 1.f, 0.f) * m_vec3ParticleSize.z);

	}
	m_pPhysicsCom->SetGravityForce(m_fGravityStr);
	m_pPhysicsCom->SetHeight(m_height);
	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	if (true == m_pPhysicsCom->GetGround() || !m_bRender)
		return;

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->SetNextPos(_float3(0.f, 0.f, 0.f));

	if (PARTICLE_BLOOD != m_ParticleType)
	{

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,vPos + *D3DXVec3Normalize(&m_Dir, &m_Dir)*fTimeDelta* m_fDirStr);

	}

	m_pPhysicsCom->GroundPhysics(m_pTransformCom, m_iCurrentLevel, TEXT("Layer_Floor"), fTimeDelta);


}

void CParticle::Late_Tick(_float fTimeDelta)
{
	
	


	if (PARTICLE_BLOOD != m_ParticleType)
	{
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
	}
	if(true == m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + m_pTransformCom->GetNextPos());

}



HRESULT CParticle::Render()
{
	if (true == m_bRender)
	{
		SetUp_RenderState();
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Bind_OnGraphicDevice(m_iTexIndex);

		m_pVIBufferCom->Render();
		Release_RenderState();
	}
	

	return S_OK;
}

void CParticle::Set_Position(_float3 TargetPos)//, _float fheight)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TargetPos);
	if (PARTICLE_GOON == m_ParticleType || PARTICLE_PIPE)
	{
		m_Dir.x = (_float(rand() % _int(m_fSpread.x) - ((m_fSpread.x - 1.0f) / 2.f )));
		m_Dir.y = m_fSpread.y;
		m_Dir.z = (_float(rand() % _int(m_fSpread.z) - ((m_fSpread.z - 1.0f) / 2.f)));
	}
	else
	{
		m_Dir.x = (_float(rand() % _int(m_fSpread.x) - ((m_fSpread.x - 1.0f) / 2.f)));
		m_Dir.y = (_float(rand() % _int(m_fSpread.y) - ((m_fSpread.y - 1.0f) / 2.f)));
		m_Dir.z = (_float(rand() % _int(m_fSpread.z) - ((m_fSpread.z - 1.0f) / 2.f)));
	}
	
	/*m_Dir.x = (_float(rand() %9) -5.f);
	m_Dir.y = 10.f;
	m_Dir.z = (_float(rand() % 9) - 5.f);*/
	m_pPhysicsCom->SetGravityForce(m_fGravityStr);
	m_bRender = true;

}

//void CParticle::Set_Position_Direction(_float3 TargetPos)
//{
//	_float3 Up = TargetPos;
//	Up.y += 300.f;
//	m_pTransformCom->LookAt(_float3{0.0f, 1.0f, 0.0f});
//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TargetPos);
//	m_bRender = true;
//}


HRESULT CParticle::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(m_iStatic, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(m_iStatic, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_LeftVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iCurrentLevel, m_TextureTag,
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(m_iStatic, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Physics */
	if (FAILED(__super::Add_Component(m_iStatic, TEXT("Prototype_Component_Physics"),
		TEXT("Com_Physics"), (CComponent**)&m_pPhysicsCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CParticle::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CParticle::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	return S_OK;
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle* pInstance = new CParticle(pGraphic_Device);
	if (nullptr == pInstance)
		return nullptr;
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created: Particle");
	}

	return pInstance;
}

CParticle* CParticle::Clone(void* pArg)
{
	CParticle* pInstance = new CParticle(*this);
	if (nullptr == pInstance)
		return nullptr;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned: Particle");
	}
	return pInstance;
}

void CParticle::Free()
{
	__super::Free();
	Safe_Release(m_pPhysicsCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}