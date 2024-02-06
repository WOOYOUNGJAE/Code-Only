#include "stdafx.h"
#include "EnemyPhysics.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Layer.h"

CEnemyPhysics* CEnemyPhysics::Create(void* pArg)
{
	CEnemyPhysics* pInstance = new CEnemyPhysics(static_cast<CEnemyBase*>(pArg));

	if (pInstance == nullptr)
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CEnemyPhysics");
	}
	return pInstance;
}

void CEnemyPhysics::Initialize(void* pArg)
{

}

_float3 CEnemyPhysics::Tick(_float fTimeDelta)
{
	m_vCurPos = m_pOwner->Get_Pos();

	if (m_fMagnitude < 0.1f)
	{
		m_fMagnitude = 0.f;
		m_vForce = _float3(0, 0, 0);
	}

	m_vCurPos += m_vForce * m_fMagnitude * fTimeDelta;
	m_fMagnitude *= m_fDecelerationFactor * m_fDecelerationFactor;

	return m_vForce * m_fMagnitude * fTimeDelta;
	return m_vCurPos;
}

void CEnemyPhysics::LateTick(_float fTimeDelta)
{
}

_bool CEnemyPhysics::RayCast(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float3 _ray, _float3* pOut)
{
    CGameInstance* pGameInstance = CGameInstance::Get_Instance();

    CLayer* pLayer = pGameInstance->Find_Layer(iLevelIndex, _strLayerTag);
    if (pLayer == nullptr)
    {
        return false;
    }

    _float minDist = 100001.f;
    _float minPoint;

    _bool bCollided = false;
    for (int i = 0; i < pLayer->Size(); i++)
    {
        // RayCast 대상이 활성화 상태인지 검사
        if (false == pGameInstance->Get_GameObject(iLevelIndex, _strLayerTag, i)->Enable())
        {
            continue;
        }

        CVIBuffer* buffer = static_cast<CVIBuffer*>(pGameInstance->Get_Component(iLevelIndex, _strLayerTag, TEXT("Com_VIBuffer"), i));
        CTransform* buffer_transform = static_cast<CTransform*>(pGameInstance->Get_Component(iLevelIndex, _strLayerTag, TEXT("Com_Transform"), i));

        if (!buffer)
            return E_FAIL;
        if (!buffer_transform)
            return E_FAIL;

        _float3 rayPos = _transform->Get_State(CTransform::STATE_POSITION);
        _float3 ray = _ray;

        _float3 fPoint;
        _bool bColl = buffer->Intersect_Polygons_ForRay(buffer_transform->Get_WorldMatrix(), ray, rayPos, &fPoint);
        if (bColl)
            bCollided = true;

        _float fDist = D3DXVec3Length(&(fPoint - rayPos));
        if (fDist < minDist)
        {
            minDist = fDist;
            *pOut = fPoint;
        }
    }

    return bCollided;
}


_bool CEnemyPhysics::GroundPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag,
                                   _float _fDeltaTime)
{
    m_fGravityAccTime += _fDeltaTime;

    _float3 fOut;
    _bool bCollided = RayCast(_transform, iLevelIndex, _strLayerTag, { 0.f,-1.f,0.f }, &fOut);

    _float3 length = fOut - _transform->Get_State(CTransform::STATE_POSITION);
    _float fDist = D3DXVec3Length(&length);

    _float3 pos = _transform->Get_State(CTransform::STATE_POSITION);

    _float fFallDist = m_fGravityForce * m_fGravityAccTime * m_fGravityAccTime;
    fFallDist = min(fFallDist, m_fHeight - 0.1f);

    if (bCollided)
    {
        if (fDist < m_fHeight)
        {
            _transform->SetNextPos(_transform->GetNextPos() + _float3(0.f, m_fHeight - 0.1f - fDist, 0.f));
            m_bGround = true;
            m_fGravityAccTime = 0.f;
        }
        else
        {
            _transform->SetNextPos(_transform->GetNextPos() + _float3(0.f, -fFallDist, 0.f));
            m_bGround = false;
        }
    }
    else
    {
        _transform->SetNextPos(_transform->GetNextPos() + _float3(0.f, -fFallDist, 0.f));
        m_bGround = false;
    }

    return m_bGround;
}

