#include "CCollider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider& rhs) :
	CComponent(rhs)
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return S_OK;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider* coll = new CCollider(pGraphic_Device);
	return nullptr;
}

CComponent* CCollider::Clone(void* pArg)
{
	return nullptr;
}

void CCollider::Free()
{
}
