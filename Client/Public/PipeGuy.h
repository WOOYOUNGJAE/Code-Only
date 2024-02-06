#pragma once
#include "Client_Defines.h"
#include "EnemyBase.h"

BEGIN(Client)
class CPipeGuy final : public CEnemyBase
{
protected:
	CPipeGuy(LPDIRECT3DDEVICE9 pGraphic_Device) : CEnemyBase(pGraphic_Device){}
	CPipeGuy(const CPipeGuy& rhs) : CEnemyBase(rhs.m_pGraphic_Device){}
	~CPipeGuy() = default;

public:
	// idle -> 
public:
	static CPipeGuy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Free() override;

private: // override
	virtual HRESULT Add_Components() override;
	virtual HRESULT Add_StateOnFSM() override;

	virtual void Set_AnimProperty() override;
	//virtual _bool On_CheckHit(void* pArg) override;
private:
	CTexture* m_pTextureComArr[ENEMY_TEXTURE_DIR_END] = {};
};

END