#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	/* 백그라운드가 사용하기위한 컴포넌트들을 추가하낟. */
	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(rand() % 20, 3.f, rand() % 20));


	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float4x4		CameraWorldMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);

	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&CameraWorldMatrix.m[0][0]);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&CameraWorldMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&CameraWorldMatrix.m[2][0]);
		
	SetUp_OnTerrain();

}

void CMonster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);	
}


HRESULT CMonster::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());



	/* 장치에 텍스쳐를 바인딩한다. */
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;	

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;
	
	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
		
	return S_OK;
}

HRESULT CMonster::Add_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	/* For.Com_Transform */
	/* 각 객체들이 트랜스폼을 복제하여 객체안에 보관할 때! 객체가 움직, 회전해야한ㄴ 속도를 저장할 것이다. */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonster::SetUp_RenderState()
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
	

	return S_OK;
}

HRESULT CMonster::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMonster::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_Terrain* pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CTransform* pTerrainTS = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTS)
		return E_FAIL;

	_float3		vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vWorldPos.y = pTerrainBuffer->Compute_Height(vWorldPos, pTerrainTS->Get_WorldMatrix()) + 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWorldPos);

	Safe_Release(pGameInstance);


	return S_OK;
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster*	pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Created : CMonster");
	}

	return pInstance;
}

CGameObject * CMonster::Clone(void* pArg)
{
	CMonster*	pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Cloned : CMonster");
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	/*Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);	*/
}
