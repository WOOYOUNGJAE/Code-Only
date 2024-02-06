#include "..\Public\Physics.h"
#include "Picking.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"

CPhysics::CPhysics(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent(pGraphic_Device)
{
}

CPhysics::CPhysics(const CPhysics& rhs)
    : CComponent(rhs)
{
}

HRESULT CPhysics::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPhysics::Initialize(void* pArg)
{
    return S_OK;
}

CPhysics* CPhysics::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPhysics* pInstance = new CPhysics(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Created : CPhysics");
    }
    return pInstance;
}

CComponent* CPhysics::Clone(void* pArg)
{
    CPhysics* pInstance = new CPhysics(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Failed to Cloned : CPhysics");
    }
    return pInstance;
}

void CPhysics::Free()
{
    __super::Free();
}

void CPhysics::SetOBBDesc(_float3* _vNextPos, _float4x4* _matrix)
{
    m_OBBDesc.vCenterPos =  _float3(_matrix->_41,_matrix->_42,_matrix->_43) + *_vNextPos;
	D3DXVec3Normalize(&m_OBBDesc.vAxisDir[0], &_float3(_matrix->_11, _matrix->_12, _matrix->_13));
	D3DXVec3Normalize(&m_OBBDesc.vAxisDir[1], &_float3(_matrix->_21, _matrix->_22, _matrix->_23));
	D3DXVec3Normalize(&m_OBBDesc.vAxisDir[2], &_float3(_matrix->_31, _matrix->_32, _matrix->_33));
    m_OBBDesc.fAxisLen[0] = D3DXVec3Length(&_float3(_matrix->_11, _matrix->_12, _matrix->_13));
    m_OBBDesc.fAxisLen[1] = D3DXVec3Length(&_float3(_matrix->_21, _matrix->_22, _matrix->_23));
    m_OBBDesc.fAxisLen[2] = D3DXVec3Length(&_float3(_matrix->_31, _matrix->_32, _matrix->_33));
    //_float3 vVertex0;
    //D3DXVec3TransformCoord(&vVertex0, &_cubeCom->GetVerticesPos()[0], _matrix);
    //_float3 vVertex1;
    //D3DXVec3TransformCoord(&vVertex1, &_cubeCom->GetVerticesPos()[1], _matrix);
    //_float3 vVertex3;
    //D3DXVec3TransformCoord(&vVertex3, &_cubeCom->GetVerticesPos()[3], _matrix);
    //_float3 vVertex4;
    //D3DXVec3TransformCoord(&vVertex4, &_cubeCom->GetVerticesPos()[4], _matrix);

    //_float3 vNormal1 = vVertex0 - vVertex1;
    //_float3 vNormal2 = vVertex0 - vVertex3;
    //_float3 vNormal3 = vVertex0 - vVertex4;

    //D3DXVec3Normalize(&m_OBBDesc.vAxisDir[0], &vNormal1);
    //D3DXVec3Normalize(&m_OBBDesc.vAxisDir[1], &vNormal2);
    //D3DXVec3Normalize(&m_OBBDesc.vAxisDir[2], &vNormal3);
    //m_OBBDesc.fAxisLen[0] = D3DXVec3Length(&(vVertex0 - vVertex1));
    //m_OBBDesc.fAxisLen[1] = D3DXVec3Length(&(vVertex0 - vVertex3));
    //m_OBBDesc.fAxisLen[2] = D3DXVec3Length(&(vVertex0 - vVertex4));
    //사람이 아니었던 흔적
}

void CPhysics::SetAABBDesc(_float4x4* _matrix, CVIBuffer_Cube* _cubeCom)
{
    D3DXVec3TransformCoord(&m_AABBDesc.vMinPos, &_cubeCom->GetVerticesPos()[0], _matrix);
    D3DXVec3TransformCoord(&m_AABBDesc.vMaxPos, &_cubeCom->GetVerticesPos()[7], _matrix);
    for (int i = 0; i < 7; i++)
    {
        _float3 VertexPos;
        D3DXVec3TransformCoord(&VertexPos, &_cubeCom->GetVerticesPos()[i], _matrix);
        if (m_AABBDesc.vMinPos.x >= VertexPos.x &&
            m_AABBDesc.vMinPos.y >= VertexPos.y &&
            m_AABBDesc.vMinPos.z >= VertexPos.z)
        {
            m_AABBDesc.vMinPos = VertexPos;
        }
        if (m_AABBDesc.vMaxPos.x <= VertexPos.x &&
            m_AABBDesc.vMaxPos.y <= VertexPos.y &&
            m_AABBDesc.vMaxPos.z <= VertexPos.z)
        {
            m_AABBDesc.vMaxPos = VertexPos;
        }
    }
}

_bool CPhysics::RayCast(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float3 _ray, _float3* pOut)
{
    CPicking* pPicking = CPicking::Get_Instance();

    CGameInstance* pGameInstance = CGameInstance::Get_Instance();

    CLayer* pLayer = pGameInstance->Find_Layer(iLevelIndex, _strLayerTag);
    if (pLayer == nullptr)
    {
        return false;
    }

    _float minDist = 100001.f;

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
            return FALSE;
        if (!buffer_transform)
            return FALSE;
         
        _float3 vBufferPos = buffer_transform->Get_State(CTransform::STATE_POSITION);
        _float3 rayPos = _transform->Get_State(CTransform::STATE_POSITION);

        //_float3 vDist = vBufferPos - rayPos;
        //_float fvDist = D3DXVec3Length(&vDist);

        //if (fvDist > 15.f)
        //    continue;

        _float3 ray = _ray;

        _float3 fPoint = _float3(0.f,0.f,0.f);
        _bool bColl = buffer->Intersect_Polygons_ForRay(buffer_transform->Get_WorldMatrix(), ray, rayPos, &fPoint);
        if (bColl)
            bCollided = true;

        _float fDist = D3DXVec3Length(&(fPoint - rayPos));
        if (fDist < minDist && bColl)
        {
            minDist = fDist;
            *pOut = fPoint;
        }
    }

    return bCollided;
}

_bool CPhysics::MouseRayCast( _uint iLevelIndex, const wstring& _strLayerTag, _float3* pOut, CGameObject** pGameObject)
{
    CPicking* pPicking = CPicking::Get_Instance();

    CGameInstance* pGameInstance = CGameInstance::Get_Instance();

    CLayer* pLayer = pGameInstance->Find_Layer(iLevelIndex, _strLayerTag);
    if (pLayer == nullptr)
    {
        return false;
    }

    _float minDist = 100001.f;

    _bool bCollided = false;
    for (int i = 0; i < pLayer->Size(); i++)
    {
        CGameObject* pObject = pGameInstance->Get_GameObject(iLevelIndex, _strLayerTag, i);
        if (pObject->Is_Pickable() == false)
        {
            int a = 1;
        }
        // RayCast 대상이 활성화 상태인지 검사
        if (false == pObject->Enable() ||
            false == pObject->Is_Pickable())
        {
            continue;
        }

        CVIBuffer* buffer = static_cast<CVIBuffer*>(pGameInstance->Get_Component(iLevelIndex, _strLayerTag, TEXT("Com_VIBuffer"), i));
        CTransform* buffer_transform = static_cast<CTransform*>(pGameInstance->Get_Component(iLevelIndex, _strLayerTag, TEXT("Com_Transform"), i));

        if (!buffer)
            return FALSE;
        if (!buffer_transform)
            return FALSE;

        _float3 fPoint = _float3(0.f,0.f,0.f);
        _bool bColl = buffer->Intersect_Polygons(buffer_transform->Get_WorldMatrix(), &fPoint);
        if (bColl)
            bCollided = true;

        _float fDist = D3DXVec3Length(&(fPoint - pPicking->GetMouseRayPos()));
        if (fDist < minDist && bColl)
        {
            minDist = fDist;
            *pOut = fPoint;
            *pGameObject = static_cast<CGameObject*>(pGameInstance->Get_GameObject(iLevelIndex, _strLayerTag, i));
        }
    }
    if (!bCollided)
    {
        *pOut = _float3(0.f, 0.f, 0.f);
        *pGameObject = nullptr;
    }

    return bCollided;
}

void CPhysics::GroundPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float _fDeltaTime)
{
    m_fTime += _fDeltaTime;

    _float3 fOut;
    _bool bCollided = RayCast(_transform, iLevelIndex, _strLayerTag, { 0.f,-1.f,0.f }, &fOut);

    _float3 length = fOut - _transform->Get_State(CTransform::STATE_POSITION);
    _float fDist = D3DXVec3Length(&length);

    _float3 pos = _transform->Get_State(CTransform::STATE_POSITION);

    _float fFallDist = m_fGravityForce * m_fTime * m_fTime;
    fFallDist = min(fFallDist,m_fHeight - 0.1f);
    if (bCollided)
    {
        if (fDist < m_fHeight && fOut.y <= pos.y)
        {
            //_transform->SetNextPos(_transform->GetNextPos() + _float3(0.f, m_fHeight - 0.1f - fDist, 0.f));
            _transform->Set_State(CTransform::STATE_POSITION, (pos + _float3(0.f, m_fHeight - 0.1f - fDist, 0.f)));
            m_bGround = true;
            m_fTime = 0.f;
        }
        else
        {
            //_transform->SetNextPos(_transform->GetNextPos() +  _float3(0.f, -fFallDist, 0.f));
            _transform->Set_State(CTransform::STATE_POSITION, (pos + _float3(0.f, -fFallDist, 0.f)));
            m_bGround = false;
        }
    }
    else
    {
        //_transform->SetNextPos(_transform->GetNextPos() +  _float3(0.f, -fFallDist, 0.f));
        _transform->Set_State(CTransform::STATE_POSITION, (pos + _float3(0.f, -fFallDist, 0.f)));
        m_bGround = false;
    }

}

void CPhysics::JumpPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float _fDeltaTime)
{
    m_fTime += _fDeltaTime;

    _float3 fOut;
    _bool bCollided = RayCast(_transform, iLevelIndex, _strLayerTag, { 0.f,-1.f,0.f }, &fOut);

    _float3 length = fOut - _transform->Get_State(CTransform::STATE_POSITION);
    _float fDist = D3DXVec3Length(&length);

    _float3 pos = _transform->Get_State(CTransform::STATE_POSITION);

    _float fFallDist = m_fGravityForce * m_fTime * m_fTime - m_fJumpForce *m_fTime;
    fFallDist = min(fFallDist, m_fHeight - 0.1f);

    if (bCollided)
    {
        //공중에서 지면에 착지할 때
        if (fDist < m_fHeight - 0.1f && !m_bJumpStart)
        {
            m_bGround = true;
        }
        //공중
        else if (fDist >= m_fHeight - 0.1f)
        {
            m_bJumpStart = false;
            _transform->Set_State(CTransform::STATE_POSITION, { pos.x,pos.y - fFallDist ,pos.z });
            m_bGround = false;
        }
        //지면에서 점프를 시작할 때
        else
        {
            _transform->Set_State(CTransform::STATE_POSITION, { pos.x,pos.y - fFallDist ,pos.z });
            m_bGround = false;
        }
    }
    else
    {
        m_bJumpStart = false;
        _transform->Set_State(CTransform::STATE_POSITION, { pos.x,pos.y - fFallDist ,pos.z });
        m_bGround = false;
    }
}

void CPhysics::ThrowPhysics(CTransform* _transform, _float3 _dir, _float fDeltaTime)
{
    m_fTime += fDeltaTime;
    _float _fY = -m_fTime * m_fTime;
    _transform->SetNextPos(_float3(_dir.x * fDeltaTime* 30.f,_dir.y  +  _fY * 3.f, _dir.z * fDeltaTime * 30.f));
}

_bool CPhysics::Collision_OBB(_uint iLevelIndex, const wstring& _strLayerTag, CGameObject** pOut)
{
    CLayer* pLayer = CGameInstance::Get_Instance()->Find_Layer(iLevelIndex, _strLayerTag);
    if (pLayer == nullptr)
    {
        MSG_BOX("Failed to Find Layer in Collision_OBB");
        return false;
    }

    for (int j = 0; j < pLayer->Size(); j++)
    {
        auto pGameObject = CGameInstance::Get_Instance()->Get_GameObject(iLevelIndex, _strLayerTag, j);
         //RayCast 대상이 활성화 상태인지 검사
        if (false == pGameObject->Enable())
        {
            continue;
        }

        double c[3][3];
        double absC[3][3];
        double d[3];

        double r0, r1, r;
        int i;

        const double cutoff = 0.999999;
        bool existsParallelPair = false;

        OBBDESC* Box1 = &m_OBBDesc;
        OBBDESC* Box2 = static_cast<CPhysics*>(pGameObject->Find_Component(L"Com_Physics"))->GetOBBDesc();
        D3DXVECTOR3 diff = Box1->vCenterPos - Box2->vCenterPos;
        //_float vLength = D3DXVec3Length(&diff);
        //if (vLength > 10.f)
        //    continue;
        for (i = 0; i < 3; ++i)
        {
            c[0][i] = D3DXVec3Dot(&Box1->vAxisDir[0], &Box2->vAxisDir[i]);
            absC[0][i] = abs(c[0][i]);
            if (absC[0][i] > cutoff)
                existsParallelPair = true;
        }
        d[0] = D3DXVec3Dot(&diff, &Box1->vAxisDir[0]);
        r = abs(d[0]);
        r0 = Box1->fAxisLen[0];
        r1 = Box2->fAxisLen[0] * absC[0][0] + Box2->fAxisLen[1] * absC[0][1] + Box2->fAxisLen[2] * absC[0][2];

        if (r > r0 + r1)
            continue;


        for (i = 0; i < 3; ++i)
        {
            c[1][i] = D3DXVec3Dot(&Box1->vAxisDir[1], &Box2->vAxisDir[i]);
            absC[1][i] = abs(c[1][i]);
            if (absC[1][i] > cutoff)
                existsParallelPair = true;
        }
        d[1] = D3DXVec3Dot(&diff, &Box1->vAxisDir[1]);
        r = abs(d[1]);
        r0 = Box1->fAxisLen[1];
        r1 = Box2->fAxisLen[0] * absC[1][0] + Box2->fAxisLen[1] * absC[1][1] + Box2->fAxisLen[2] * absC[1][2];

        if (r > r0 + r1)
            continue;



        for (i = 0; i < 3; ++i)
        {
            c[2][i] = D3DXVec3Dot(&Box1->vAxisDir[2], &Box2->vAxisDir[i]);
            absC[2][i] = abs(c[2][i]);
            if (absC[2][i] > cutoff)
                existsParallelPair = true;
        }
        d[2] = D3DXVec3Dot(&diff, &Box1->vAxisDir[2]);
        r = abs(d[2]);
        r0 = Box1->fAxisLen[2];
        r1 = Box2->fAxisLen[0] * absC[2][0] + Box2->fAxisLen[1] * absC[2][1] + Box2->fAxisLen[2] * absC[2][2];

        if (r > r0 + r1)
            continue;



        r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[0]));
        r0 = Box1->fAxisLen[0] * absC[0][0] + Box1->fAxisLen[1] * absC[1][0] + Box1->fAxisLen[2] * absC[2][0];
        r1 = Box2->fAxisLen[0];

        if (r > r0 + r1)
            continue;



        r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[1]));
        r0 = Box1->fAxisLen[0] * absC[0][1] + Box1->fAxisLen[1] * absC[1][1] + Box1->fAxisLen[2] * absC[2][1];
        r1 = Box2->fAxisLen[1];

        if (r > r0 + r1)
            continue;



        r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[2]));
        r0 = Box1->fAxisLen[0] * absC[0][2] + Box1->fAxisLen[1] * absC[1][2] + Box1->fAxisLen[2] * absC[2][2];
        r1 = Box2->fAxisLen[2];

        if (r > r0 + r1)
            continue;



        if (existsParallelPair == true)
        {
            *pOut = pGameObject;
            return TRUE;
        }



        r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
        r0 = Box1->fAxisLen[1] * absC[2][0] + Box1->fAxisLen[2] * absC[1][0];
        r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
        if (r > r0 + r1)
            continue;



        r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
        r0 = Box1->fAxisLen[1] * absC[2][1] + Box1->fAxisLen[2] * absC[1][1];
        r1 = Box2->fAxisLen[0] * absC[0][2] + Box2->fAxisLen[2] * absC[0][0];
        if (r > r0 + r1)
            continue;



        r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
        r0 = Box1->fAxisLen[1] * absC[2][2] + Box1->fAxisLen[2] * absC[1][2];
        r1 = Box2->fAxisLen[0] * absC[0][1] + Box2->fAxisLen[1] * absC[0][0];
        if (r > r0 + r1)
            continue;



        r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
        r0 = Box1->fAxisLen[0] * absC[2][0] + Box1->fAxisLen[2] * absC[0][0];
        r1 = Box2->fAxisLen[1] * absC[1][2] + Box2->fAxisLen[2] * absC[1][1];
        if (r > r0 + r1)
            continue;



        r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
        r0 = Box1->fAxisLen[0] * absC[2][1] + Box1->fAxisLen[2] * absC[0][1];
        r1 = Box2->fAxisLen[0] * absC[1][2] + Box2->fAxisLen[2] * absC[1][0];
        if (r > r0 + r1)
            continue;



        r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
        r0 = Box1->fAxisLen[0] * absC[2][2] + Box1->fAxisLen[2] * absC[0][2];
        r1 = Box2->fAxisLen[0] * absC[1][1] + Box2->fAxisLen[1] * absC[1][0];
        if (r > r0 + r1)
            continue;



        r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
        r0 = Box1->fAxisLen[0] * absC[1][0] + Box1->fAxisLen[1] * absC[0][0];
        r1 = Box2->fAxisLen[1] * absC[2][2] + Box2->fAxisLen[2] * absC[2][1];
        if (r > r0 + r1)
            continue;



        r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
        r0 = Box1->fAxisLen[0] * absC[1][1] + Box1->fAxisLen[1] * absC[0][1];
        r1 = Box2->fAxisLen[0] * absC[2][2] + Box2->fAxisLen[2] * absC[2][0];
        if (r > r0 + r1)
            continue;



        r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
        r0 = Box1->fAxisLen[0] * absC[1][2] + Box1->fAxisLen[1] * absC[0][2];
        r1 = Box2->fAxisLen[0] * absC[2][1] + Box2->fAxisLen[1] * absC[2][0];
        if (r > r0 + r1)
            continue;

        *pOut = pGameObject;
        return TRUE;
    }
    return FALSE;
}

_bool CPhysics::Collision_AABB(_uint iLevelIndex, const wstring& _strLayerTag)
{
    CLayer* pLayer = CGameInstance::Get_Instance()->Find_Layer(iLevelIndex, _strLayerTag);
    if (pLayer == nullptr)
    {
        MSG_BOX("Failed to Find Layer in Collision_OBB");
        return false;
    }

    for (int j = 0; j < pLayer->Size(); j++)
    {
        auto pGameObject = CGameInstance::Get_Instance()->Get_GameObject(iLevelIndex, _strLayerTag, j);
        // RayCast 대상이 활성화 상태인지 검사
        if (false == pGameObject->Enable())
        {
            continue;
        }
        AABBDESC* targetBox = static_cast<CPhysics*>(pGameObject->Find_Component(L"Com_Physics"))->GetAABBDesc();

        if (m_AABBDesc.vMinPos.x <= targetBox->vMaxPos.x && m_AABBDesc.vMaxPos.x >= targetBox->vMinPos.x &&
            m_AABBDesc.vMinPos.y <= targetBox->vMaxPos.y && m_AABBDesc.vMaxPos.y >= targetBox->vMinPos.y &&
            m_AABBDesc.vMinPos.z <= targetBox->vMaxPos.z && m_AABBDesc.vMaxPos.z >= targetBox->vMinPos.z)
        {
            return true;
        }
    }
	return false;
}

void CPhysics::AddForceTick(CTransform* _transform, _float fTimeDelta)
{
    if (m_bAddForceStart)
    {
        _float3 vPos = _transform->Get_State(CTransform::STATE_POSITION);
        m_fForceTime += fTimeDelta;
        m_vPushForceDir.x -= fTimeDelta;
        m_vPushForceDir.x = max(m_vPushForceDir.x, 0.f);
        m_vPushForceDir.y -= fTimeDelta;
        m_vPushForceDir.y = max(m_vPushForceDir.y, 0.f);
        m_vPushForceDir.z -= fTimeDelta;
        m_vPushForceDir.z = max(m_vPushForceDir.z, 0.f);
        _transform->SetNextPos(_transform->GetNextPos() + m_vPushForceDir * m_fPushForce * fTimeDelta);
        if (m_fForceTime > 1.f)
        {
            m_bAddForceStart = false;
            m_fForceTime = 0.f;
        }
    }
}
