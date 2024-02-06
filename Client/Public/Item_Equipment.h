#pragma once
#include "InteractiveObject.h"
#include "Client_Defines.h"
#include "Client_Struct.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Quad;
END

BEGIN(Client)
class CItem_Equipment :
    public CInteractiveObject
{
	
public:
	CItem_Equipment(LPDIRECT3DDEVICE9 pGrahic_Device);
	CItem_Equipment(const CInteractiveObject& rhs);
	virtual ~CItem_Equipment() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CItem_Equipment* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

public:
	virtual _bool   OnItemPicked(void* pArg);

private:
	_float3* m_pDistance = { nullptr };
	_bool m_bPicked = false;

public:
	_int	GetMaxHP() { return m_iMaxHP; }
	_int	GetHP() { return m_iHP; }
	void	SetHP(_int i) { m_iHP = i; }

	_bool	OnUsedEquipment(void* pArg);
	void	SetBrokenBottle() { 
		m_eItemType = ITEM_BOTTLE_BROKEN;
		m_iAttack = 34;
		m_iMaxHP = 50;
		m_iHP = m_iMaxHP;
	}
	_int GetBulletCount() { return m_iBulletCount; }
	_int MinusBullet() {
		--m_iBulletCount;
		if (m_iBulletCount < 0)
			m_iBulletCount = 0;
		return m_iBulletCount;
	}

private:
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);
	//virtual HRESULT SetUp_RenderState();
	//virtual HRESULT Release_RenderState();
	
private:
	bool		m_bEquiped = false;
	//bool		m_bStorable = true;
	_int		m_iAttack;
	_int		m_iMaxHP = 0;
	_int		m_iHP = 0;
	_int        m_iBulletCount = 0;
	//_int		m_iThrowingDamage;
};

END