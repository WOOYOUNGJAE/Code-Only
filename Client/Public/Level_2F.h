#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_2F final : public CLevel
{
private:
	CLevel_2F(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_2F() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Camera_Player(const wstring& strLayerTag);

	HRESULT Ready_Layer_UI_HudBox();
	HRESULT Ready_Layer_UI_Player_Stamina();
	HRESULT Ready_Layer_UI_Tooltip();
	HRESULT Ready_Layer_UI_RadioBase();
	HRESULT Ready_Layer_UI_CrossHair();
	HRESULT Ready_Layer_UI_Buff();
	HRESULT Ready_Layer_UI_Objective_Complete();
	HRESULT Ready_Layer_UI_Text();
	HRESULT Ready_Layer_UI_Map();
	HRESULT Ready_Layer_UI_Weapon();
	HRESULT Ready_Layer_UI_Notification();

	HRESULT Ready_Layer_UI_Progress();

	HRESULT Ready_Layer_Inventory(const wstring& strLayerTag);

private:
	HRESULT Load_Map(const wstring& strFilePath, const wstring& strSectionTag);

public:
	static CLevel_2F* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END