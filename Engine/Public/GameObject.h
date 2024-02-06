#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() {};

public:
	class CComponent* Find_Component(const wstring& strComTag);

	_bool Enable() { return m_bEnable; }
	void Enable(_bool bEnable) { m_bEnable = bEnable; }
	_bool Is_Pickable() { return m_bPickable; }
	void Set_Pickable(_bool bPickable) { m_bPickable = bPickable; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

protected:
	map<wstring, class CComponent*>			m_Components;
	_bool									m_bEnable = { true };
	_bool									m_bDebugCollider = { false };
	_bool									m_bPickable = true;
protected:
	HRESULT Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& strComTag, CComponent** ppOut, void* pArg = nullptr);


public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
	virtual void OnPicked(void* pArg);

	void SetDebugCollider(_bool b) { m_bDebugCollider = b; }
	_bool GetDebugCollider() { return m_bDebugCollider; }
};

END