#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_WYJ final : public CLevel
{
private:
	CLevel_WYJ(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_WYJ() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_Layer_Enemy(const wstring& strLayerTag);
public:
	static CLevel_WYJ* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END