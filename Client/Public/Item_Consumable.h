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
class CItem_Consumable :
    public CInteractiveObject
{
public:
	CItem_Consumable(LPDIRECT3DDEVICE9 pGrahic_Device);
	CItem_Consumable(const CInteractiveObject& rhs);
	virtual ~CItem_Consumable() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CItem_Consumable* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);
	
public:
	virtual _bool   OnItemPicked(void* pArg);
	_int GetHeal() { return m_iHeal; }

private:
	_int    m_iHeal;
};
END

