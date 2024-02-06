#pragma once
#include "Client_Defines.h"
#include "EnemyBase.h"

BEGIN(Client)
/**
 * \brief 바보 적. 기본 적
 */
class CGoon final : public CEnemyBase
{
protected:
	CGoon(LPDIRECT3DDEVICE9 pGraphic_Device) : CEnemyBase(pGraphic_Device){}
	CGoon(const CGoon& rhs) : CEnemyBase(rhs.m_pGraphic_Device){}
	~CGoon() = default;

public:
	// idle -> 
public:
	static CGoon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Free() override;

private:
	virtual HRESULT Add_Components() override;
	virtual HRESULT Add_StateOnFSM() override;

private:
	CTexture* m_pTextureComArr[ENEMY_TEXTURE_DIR_END] = {};
};

END