#pragma once

#include "GameObject.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
#include "Engine_Struct.h"


BEGIN(Engine)
class CParticle final : public CGameObject
{
protected:
    CParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
    CParticle(const CParticle& rhs);
    virtual ~CParticle() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void Late_Tick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void Set_Position(_float3 TargetPos);//, _float fHeight);
   /* void Set_Position_Direction(_float3 TargetPos);*/
    void Set_TextureIndex(_uint iTexIndex)
    {
        m_iTexIndex = iTexIndex;
    }
    void Set_Render(_bool bInput)
    {
        m_bRender = bInput;
    }

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
    _uint m_ParticleType;
    _float3 m_Dir;
    _float3 m_fSpread;
    _uint m_iStatic;
    _uint m_iCurrentLevel;
    _uint m_iTexIndex;
    _float m_height;
    _float m_fGravityStr;
    _float m_fDirStr;
    _float3 m_vec3ParticleSize;

public:
    static CParticle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CParticle* Clone(void* pArg);
    virtual void Free() override;
};

END
