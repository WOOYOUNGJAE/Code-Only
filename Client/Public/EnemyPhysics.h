#pragma once
#include "EnemyBase.h"

BEGIN(Client)
class CEnemyPhysics : public CBase
{
private:
	CEnemyPhysics(CEnemyBase* pOwner) : m_pOwner(pOwner){}
	virtual ~CEnemyPhysics() = default;

public: // LifeCycle
	static CEnemyPhysics* Create(void* pArg);
	void Initialize(void* pArg);
	_float3 Tick(_float fTimeDelta); // Return new Pos
	void LateTick(_float fTimeDelta);
	_bool RayCast(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float3 _ray, _float3* pOut);
	void Free() override{};

public: //getter setter
	_bool Has_Magnitude() { return m_fMagnitude > 0.1f; }
	void Set_Height(_float _f) { m_fHeight = _f; }
public:
	void Add_Force(_float3 vForceDirNormalized, _float fMagnitude, _float fDecelerationFactor = 0.75f) { m_vForce = vForceDirNormalized; m_fMagnitude = fMagnitude; m_fDecelerationFactor = fDecelerationFactor; };
	// ¶¥¿¡ ´êÀ» ‹š true
	_bool GroundPhysics(CTransform* _transform, _uint iLevelIndex, const wstring& _strLayerTag, _float _fDeltaTime);
private:
	CEnemyBase* m_pOwner = nullptr;
	_float3 m_vCurPos;
	_float3 m_vForce;
	_float m_fMagnitude = 0.f;
	_float m_fDecelerationFactor = 0.75f;

	_float m_fGravityAccTime = 0.f;
	_float m_fGravityForce = 0.2f;
	_float m_fHeight = 1.f;
	_bool m_bGround = false;
};
END