#pragma once
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Mask final : public CVIBuffer_Rect
{
protected:
	CVIBuffer_Mask(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Mask(const CVIBuffer_Mask& rhs);
	virtual ~CVIBuffer_Mask() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

public:
	void SetWorldMatrix(const _float4x4& mWorld) { m_WorldMatrix = mWorld; }
	void SetViewMatrix(const _float4x4& mView) { m_ViewMatrix = mView; }
	void SetProjectionMatrix(const _float4x4& mProjection) { m_ProjectionMatrix = mProjection; }

	void Set_Texture(IDirect3DBaseTexture9* pTexture) { m_pTexture = pTexture; }

	void Set_Radius(_float fRadius) { m_fRadius = fRadius; }

	void Set_ViewportSize(_float fX, _float fY) { m_fViewportSizeX = fX; m_fViewportSizeY = fY; }

private:
	_float4x4 m_WorldMatrix;
	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjectionMatrix;

	IDirect3DBaseTexture9* m_pTexture = { nullptr };

	_float m_fRadius = { 0.f };

	_float m_fViewportSizeX = { 0.f };
	_float m_fViewportSizeY = { 0.f };

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

	D3DXHANDLE m_hRadius;
	D3DXHANDLE m_hViewportSizeX;
	D3DXHANDLE m_hViewportSizeY;

public:
	static CVIBuffer_Mask* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END