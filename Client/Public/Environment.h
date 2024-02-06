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

class CEnvironment final : public CGameObject
{
protected:
	CEnvironment(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnvironment(const CEnvironment& rhs);
	virtual ~CEnvironment() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool OnCheckGunfireEffect(void* pArg);

private:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDebugTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Quad* m_pVIBufferCom = { nullptr };
	CVIBuffer_Cube* m_pCubeVIBufferCom = { nullptr };
	CPhysics* m_pPhysicsCom = { nullptr };

	MESH_TYPE m_eMeshType = MESH_QUAD;
	_float4x4 m_WorldMatrix = {};
private:
	HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CEnvironment* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
private:
	_bool m_bAlphaTesting = { false };
	_int m_iAlphaRef = { 0 };

private:
	_bool m_bBillboard = { false };
private:
	//_bool m_bProjectionCulling = { false };
	//_float m_fCullingThreshold = { 3.f };
};

END