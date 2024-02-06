#pragma once

#include "Component.h"

/* 객체의 월드 변환 행렬을 가진다. (m_WorldMatrix) */
/* 월드 공간에서의 상태변환하는 긴으을 가진다. (Go, Left, Right, Turn, Scaling )*/


BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		/* 초당 이동해야하는 속도 */
		_float		fSpeedPerSec = { 0.0f };
		/* 초당 회전해야하는 속도. */
		_float		fRotationPerSec = { 0.0f };
	}TRANSFORMDESC;
protected:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) const {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scaled() const;


	_float4x4 Get_WorldMatrix() {
		return m_WorldMatrix;
	}
	_float4x4 Get_WorldMatrix_Inverse() {
		_float4x4	InverseMatrix;
		return *D3DXMatrixInverse(&InverseMatrix, nullptr, &m_WorldMatrix);
	}

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	// m_vPos += normalize(m_vDir) * Speed * TimeDelta
	void Go_Straight(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);

	//이동하는 UI를 위한 함수(Up, Down)
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void Go_Dir(_float fTimeDelta);
	void Go_DirBack(_float fTimeDelta);
	void Go_Straight_FixedY(_float fTimeDelta);
	void Go_Left_FixedY(_float fTimeDelta);
	void Go_Right_FixedY(_float fTimeDelta);
	void Go_Backward_FixedY(_float fTimeDelta);
	void Go_LeftUp_FixedY(_float fTimeDelta);
	void Go_RightUp_FixedY(_float fTimeDelta);
	void Go_LeftDown_FixedY(_float fTimeDelta);
	void Go_RightDown_FixedY(_float fTimeDelta);

	void Turn(const _float3& vAxis, _float fTimeDelta); /* 초당 정해진 속도로 회전한다. */
	void Rotation(const _float3& vAxis, float fRadian ); /* 내가 지정한 각도로 회전한상태를 만들어준다.  */
	void Scaling(const _float3& vScale); /* 지정한 크기로 변환한다. */
	void Scaling_Ratio(const _float3& vScale); //현재 크기에서 입력한 비율로 작아짐.
	void LookAt(const _float3& vAt); /* 지정한 위치를 바라본다. */

	void TurnOneSide(const _float3& vAxis, const _float3& vMove, _float fTimeDelta);
private:
	/* 크 * 자 * 이 * 공 * 부 */
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;
	_float3				m_vDir = { 0,0,0 };
	_float3				m_vNextPos = { 0,0,0 };
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	void SetDir(_float3 _v) { m_vDir = _v; }
	void SetNextPos(_float3 _v) { m_vNextPos = _v; }
	_float3 GetNextPos() { return m_vNextPos; }
};

END