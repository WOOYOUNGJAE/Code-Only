#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Mask;
END

BEGIN(Client)

class CCircleMask final : public CGameObject
{
protected:
	CCircleMask(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCircleMask(const CCircleMask& rhs);
	virtual ~CCircleMask() {};

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool Floor_Change_Trigger(void* pArg);

private:
	CRenderer* m_pRendererCom = { nullptr };
	CVIBuffer_Mask* m_pVIBufferCom = { nullptr };

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	_bool m_bFadeOut = { false };
	_float m_fCircleThreshold = { 1000.f };
	_float m_fCircleRadius = { 0.f };

private:
	_float m_fInoutSpeed = { 1000.f };

private:
	_float m_fDelayThreshold = { 1.f };
	_float m_fAccDelayTime = { 0.f };

	_bool m_bChangeSectionTrigger = { false };

private:
	std::wstring m_strCurrentSection;
	std::wstring m_strNextSection;
	_float3 m_vNewPosition;

private:
	HRESULT Add_Components();

public:
	static CCircleMask* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END