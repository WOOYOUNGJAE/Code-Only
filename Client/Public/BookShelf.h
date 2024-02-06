#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Quad;
class CVIBuffer_Cube;
class CPhysics;
END

BEGIN(Client)

class CBookShelf final : public CGameObject
{
protected:
	CBookShelf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBookShelf(const CBookShelf& rhs);
	virtual ~CBookShelf() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool OnButtonClickedEvent(void* pArg);

private:
	_bool m_bEventTriggered = { false };

private:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDebugTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Quad* m_pVIBufferCom = { nullptr };
	CVIBuffer_Cube* m_pCubeVIBufferCom = { nullptr };
	CPhysics* m_pPhysicsCom = { nullptr };

	_float4x4 m_WorldMatrix = {};
private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CBookShelf* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END