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

class CGore_Body final : public CGameObject
{
protected:
	CGore_Body(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGore_Body(const CGore_Body& rhs);
	virtual ~CGore_Body() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	ENEMY_TYPE2 m_eEnemyType;

	_uint m_iTextureCount = { 0 };
	_float3* m_pBodyPositionPivots = { nullptr };

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
	static CGore_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END