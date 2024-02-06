#pragma once

#include "Client_Defines.h"
#include "Camera.h"

/* 니네들이 클라에서 쓰기위한 카메라를 맘껏 니 하고싶은대로 만들어라. */

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	typedef struct tagCameraFreeDesc
	{
		_int					iData; /* CameraFree에게만 필요한 데이터.*/
		CCamera::CAMERADESC		CameraDesc;		/* 모든 Camera들에게 필요한 데이터.*/
	}CAMERAFREEDESC;

protected:
	CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

	// TODO: 충돌 방지용 코드.
	// 지워야함~~
public:
	POINT GetMouseDiff() { return POINT(); }

private:
	_int			m_iData = { 0 };
	POINT			m_ptOldMousePos = { 0, 0 };

public:
	static CCamera_Free* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END