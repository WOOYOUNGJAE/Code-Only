#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cylinder;
END

BEGIN(Client)

class CEvent_Sphere final : public CGameObject
{
protected:
	CEvent_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Sphere(const CEvent_Sphere& rhs);
	virtual ~CEvent_Sphere() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cylinder* m_pVIBufferCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	_bool On_Revive(void* pArg);
	_bool On_SmokeEnable(void* pArg);

public:
	static CEvent_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	CTransform* m_pPlayerTransform = { nullptr };
	float m_fThresholdDist = { 0.f };

	OBJECT_TYPE m_eObjectType = OBJECT_NONE;
	TOOLTIP m_eTooltipType = TOOLTIP::TOOLTIP_END;
	int m_iRadioIndex = {0};

private:
	std::wstring m_strEventTag;

private:
	_float m_fAccEffectInterval = { 0.f };
	_float m_fEffectThreshold = { 1.5f };

	_bool m_bSmokeEnable = { false };
};

END