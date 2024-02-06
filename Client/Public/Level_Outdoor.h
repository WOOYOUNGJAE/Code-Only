#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Client_Struct.h"

BEGIN(Client)

class CLevel_Outdoor final : public CLevel
{
private:
	CLevel_Outdoor(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Outdoor() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_Enemy(const wstring& strLayerTag);
	HRESULT Ready_Layer_Camera_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT Ready_Layer_Item_Interactive(const wstring& strLayerTag, enum ITEM_TYPE eItemType);

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

	HRESULT Ready_Layer_UI_Dead();

	HRESULT Ready_Particle();


	// TODO: Progress Bar Test!!
	HRESULT Ready_Layer_UI_Progress();

	HRESULT Load_Map(const wstring& strFilePath, const wstring& strSectionTag, _bool bRespawnEnemy = false);

	HRESULT Ready_Layer_Inventory(const wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);

	void Ready_Lights_Section_1();
	void Ready_Lights_Section_2();
	void Ready_Lights_Section_3();
	void Ready_Lights_Section_4();
	void Ready_Lights_Section_5();
	void Ready_Lights_Section_6();
	void Ready_Lights_Section_2F();
	void Ready_Lights_Section_1F();

	void Reset_Light_Settings();

private:
	/* For. Section Event Callback */
	_bool On_Section_Trigger(void* pArg);

	/* For. Respawn Event Callback */
	_bool On_Respawn_Event(void* pArg);

private:
	std::vector<ENEMYRESPAWNDESC> m_vecEnemyRespawnPoints;

public:
	static CLevel_Outdoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END