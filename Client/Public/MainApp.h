#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };
	CRenderer*					m_pRenderer = { nullptr };
	class CGimmickManager*		m_pGimmickManager = { nullptr };

private:
	HRESULT Ready_Default_Setting();
	HRESULT Ready_Loading_Scene();
	HRESULT Open_Level(LEVEL eLevel);
	HRESULT Ready_Prototype_Component_ForStatic();
	/*HRESULT Ready_Prototype_GameObject_ForStatic();*/

public:
	static CMainApp* Create();
	virtual void Free() override;	
};

END

