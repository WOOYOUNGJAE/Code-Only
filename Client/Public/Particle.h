#pragma once

#include "Client_Defines.h"
#include "Client_Struct.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CPhysics;
END

BEGIN(Client)
class CParticle final : public CGameObject
{
protected:
    CParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
    CParticle(const CParticle& rhs);
    virtual ~CParticle() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void Late_Tick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void Set_Position(_float3 TargetPos);
    void Set_Texture(_uint iTexIndex);

private:
    CTexture* m_pTextureCom = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CPhysics* m_pPhysicsCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT SetUp_RenderState();
    HRESULT Release_RenderState();

private:
    _bool m_bRender = { false };
    wstring m_TextureTag;
    _uint m_iIndex;
    PARTICLE m_ParticleType;
    _float3 m_Dir;
    _uint m_iSpread;

public:
    static CParticle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END