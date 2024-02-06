#pragma once

#include "GameObject.h"
#include "Transform.h"

/* 클라이언트에서 제작하고 자하는 카메라들의 부모가 된다. */
/* 모든 카메라에게 공통적인 기능이 필요하기때문에 .*/

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_uint		iLevelIndex;
		wstring		strTranformTag;
		_float3		vEye, vAt;
		_float		fFovy, fAspect, fNear, fFar;
		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;

protected:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:	
	_float				m_fFovy, m_fAspect, m_fNear, m_fFar;
	class CTransform*	m_pTransform = { nullptr };

protected:
	HRESULT Set_Transform();


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

	class CTransform* GetTransform() { return m_pTransform; }


};



END