#pragma once

#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CPhysics final :
	public CComponent
{
protected:
	CPhysics(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPhysics(const CPhysics& rhs);
	virtual ~CPhysics() = default;

private:
	_float3 m_vRay, m_vRayPos;
	_float m_fTime = 0.f;
	_float m_fForceTime = 0.f;
	_bool m_bGround = false;
	_bool m_bJumpStart = false;
	_bool m_bAddForceStart = false;

	_float m_fGravityForce = 0.2f;
	_float m_fJumpForce = 0.4f;
	_float m_fPushForce = 0.f;
	_float3 m_vPushForceDir = _float3(0.f, 0.f, 0.f);
	_float m_fHeight = 1.f;

	OBBDESC m_OBBDesc;
	AABBDESC m_AABBDesc;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CPhysics* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	_bool GetGround() { return m_bGround; }
	void SetJumpStart() {m_bJumpStart = true;}
	void SetGravityForce(_float _f) { m_fGravityForce = _f; }
	void SetJumpForce(_float _f) { m_fJumpForce = _f; }
	void SetHeight(_float _f) { m_fHeight = _f; }
	void SetTime(_float _f) { m_fTime = _f; }
	void SetOBBDesc(_float3* _vNextPos, _float4x4* _matrix);
	OBBDESC* GetOBBDesc() { return &m_OBBDesc; }
	void SetAABBDesc(_float4x4* _matrix, class CVIBuffer_Cube* _cubeCom);
	AABBDESC* GetAABBDesc() { return &m_AABBDesc; }
	void SetAddForce(_float3 _vDir,_float _fForce)
	{
		m_bAddForceStart = true;
		m_vPushForceDir = _vDir;
		m_fPushForce = _fForce;
	}

	_bool RayCast(class CTransform* _transform,_uint iLevelIndex, const wstring& _strLayerTag, _float3 _ray,_float3* _pOut);
	_bool MouseRayCast(_uint iLevelIndex, const wstring& _strLayerTag, _float3* _pOut,class CGameObject** _pGameObject);
	void GroundPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float _fDeltaTime);
	void JumpPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float _fDeltaTime);
	void ThrowPhysics(CTransform* _transform,_float3 _dir, _float fDeltaTime);
	_bool Collision_OBB(_uint iLevelIndex, const wstring& _strLayerTag, CGameObject** pOut);
	_bool Collision_AABB(_uint iLevelIndex, const wstring& _strLayerTag);
	void AddForceTick(CTransform* _transform, _float fTimeDelta);
};

END
