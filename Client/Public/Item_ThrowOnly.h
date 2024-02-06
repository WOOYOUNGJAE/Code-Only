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

class CItem_ThrowOnly :
    public CInteractiveObject
{
public:
	CItem_ThrowOnly(LPDIRECT3DDEVICE9 pGrahic_Device);
	CItem_ThrowOnly(const CInteractiveObject& rhs);
	virtual ~CItem_ThrowOnly() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);

public:
	static CItem_ThrowOnly* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	virtual _bool   OnItemPicked(void* pArg);
};

