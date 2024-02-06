#pragma once

#include "Component.h"

/* 화면에 그려지는 객체들을 그리는 순서대로 모아놓는다. */
/* 보관하고 있는 순서대로 객체들의 렌더함수를 호출해준다. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_POST_PROCESSING, RENDER_END };
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;	
public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Draw_RenderGroup();

	void Clear();

private:
	list<class CGameObject*>			m_RenderGroup[RENDER_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();
	HRESULT Render_PostProcess();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END