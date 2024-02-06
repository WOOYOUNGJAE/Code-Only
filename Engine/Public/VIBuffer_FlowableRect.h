#pragma once
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_FlowableRect final : public CVIBuffer_Rect
{
protected:
	CVIBuffer_FlowableRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_FlowableRect(const CVIBuffer_FlowableRect& rhs);
	virtual ~CVIBuffer_FlowableRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

public:
	void SetWorldMatrix(const _float4x4& mWorld) { m_WorldMatrix = mWorld; }
	void SetViewMatrix(const _float4x4& mView) { m_ViewMatrix = mView; }
	void SetProjectionMatrix(const _float4x4& mProjection) { m_ProjectionMatrix = mProjection; }

	void Set_Texture(IDirect3DBaseTexture9* pTexture) { m_pTexture = pTexture; }

	void Set_Amount(_float fAmount) { m_fAmount = fAmount; }

private:
	_float4x4 m_WorldMatrix;
	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjectionMatrix;

	IDirect3DBaseTexture9* m_pTexture = { nullptr };

	_float m_fAmount = { 0.f };

private:
	HRESULT Create_VertexShader();
	HRESULT Create_PixelShader();

	void Initialize_ConstantTable();

private:
	// Vertex Shader
	IDirect3DVertexShader9* m_pVertexShader = { nullptr };
	ID3DXConstantTable* m_pVertexConstantTable = { nullptr };

	// Vertex Shader Constant
	D3DXHANDLE m_hTransformWorldHandle = { nullptr };
	D3DXHANDLE m_hTransformViewHandle = { nullptr };
	D3DXHANDLE m_hTransformProjectionHandle = { nullptr };

	// Pixel Shader
	IDirect3DPixelShader9* m_pPixelShader = { nullptr };
	ID3DXConstantTable* m_pPixelConstantTable = { nullptr };

	// Pixel Shader Resource
	D3DXHANDLE m_hBaseTexHandle;
	D3DXCONSTANT_DESC m_hBaseTexDesc;

	D3DXHANDLE m_hAmount;

public:
	static CVIBuffer_FlowableRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END