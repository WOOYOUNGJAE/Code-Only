#pragma once
#include "EnemyBase.h"

BEGIN(Client)

class CDelia final : public CEnemyBase
{
protected:
	CDelia(LPDIRECT3DDEVICE9 pGraphic_Device) : CEnemyBase(pGraphic_Device) {}
	CDelia(const CDelia& rhs) : CEnemyBase(rhs.m_pGraphic_Device) {}
	~CDelia() = default;

public:
	static CDelia* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Free() override;

private:
	_bool Interactive(void* pArg);

private:
	virtual HRESULT Add_Components() override;

private:
	CTexture* m_pTextureComArr[ENEMY_TEXTURE_DIR_END] = {};
};

END