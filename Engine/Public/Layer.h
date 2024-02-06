#pragma once

#include "Base.h"

/* 내가 나누고 싶은 기준으로 객체들을 모아두기위해. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() {};

public:
	class CComponent* Get_Component(const wstring& strComTag, _uint iIndex);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	class CGameObject* Get_GameObject(_int iObjectIndex);
	list<class CGameObject*>& Get_GameObjectList(){
		return m_GameObjects;
	}
public:
	void Enable_GameObject(_bool bEnable);
	void Clear();

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	size_t Size() { return m_GameObjects.size(); }
	virtual void Free() override;
};

END