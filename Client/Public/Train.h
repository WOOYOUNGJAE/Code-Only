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

class CTrain final : public CGameObject
{
protected:
	CTrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain(const CTrain& rhs);
	virtual ~CTrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool OnTrainActivate(void* pArg);

private:
	_float3 m_vDirection;
	_bool m_bActive = { false };

	_float m_fSpeed = { 10.f };

private:
	_float m_fAccRunTime = { 0.f };
	_float m_fRespawnThreshold = { 20.f };
	_float m_fGoneThreshold = { 8.f };

	_float3 m_vPivotPosition;
	_float m_fAccidentThreshold = { 3.5f };

private:
	CTexture* m_pFrontTextureCom = { nullptr };
	CTexture* m_pSideTextureCom = { nullptr };

	CVIBuffer_Quad* m_pVIBufferCom = { nullptr };
	_float4x4 m_PivotTransform[4];

	CTransform* m_pTransformCom = { nullptr };

	CRenderer* m_pRendererCom = { nullptr };

	CTexture* m_pDebugTextureCom = { nullptr };
	
	CVIBuffer_Cube* m_pCubeVIBufferCom = { nullptr };
	CPhysics* m_pPhysicsCom = { nullptr };

	_float4x4 m_WorldMatrix = {};
private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CTrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END