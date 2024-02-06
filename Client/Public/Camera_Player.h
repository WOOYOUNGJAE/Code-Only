#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CCamera_Player :
    public CCamera 
{
public:
	typedef struct tagCameraFreeDesc
	{
		_int					iData; /* CameraFree���Ը� �ʿ��� ������.*/
		CCamera::CAMERADESC		CameraDesc;		/* ��� Camera�鿡�� �ʿ��� ������.*/
	}CAMERAFREEDESC;

protected:
	CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

private:
	_int			m_iData = { 0 };
	_bool			m_bFollowPlayer = true;
	class CTransform* m_pPlayerTransform = nullptr;
	_float3			m_vCameraPos = { 0,0,0 };

public:
	static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

	class CPlayer* m_pPlayer = { nullptr };

	void SetFollowPlayer(_bool b) { m_bFollowPlayer = b; }
	class CTransform* GetTransform() { return m_pTransform; }
	void AddCameraPos(_float3 v) { m_vCameraPos += v; }
	void SetCameraPos(_float3 v) { m_vCameraPos = v; }

	
};
END

