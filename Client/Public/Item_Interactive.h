#pragma once
#include "InteractiveObject.h"

BEGIN(Client)
class CItem_Interactive :
    public CInteractiveObject
{
	
public:
	CItem_Interactive(LPDIRECT3DDEVICE9 pGrahic_Device);
	CItem_Interactive(const CInteractiveObject& rhs);
	virtual ~CItem_Interactive() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);
	/*HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();*/
public:
	_bool IsBlocking(void* pArg); //동준테스트
	
public:
	virtual _bool   OnItemPicked(void* pArg);
public:
	static CItem_Interactive* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	//_bool		m_bStorable = false;
	_bool       m_bBlock = true;
	
};
END

