#pragma once
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_BlinkableRect final : public CVIBuffer_Rect
{
protected:
	CVIBuffer_BlinkableRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_BlinkableRect(const CVIBuffer_BlinkableRect& rhs);
	virtual ~CVIBuffer_BlinkableRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

public:
	void SetWorldMatrix(const _float4x4& mWorld) { m_WorldMatrix = mWorld; }
	void SetViewMatrix(const _float4x4& mView) { m_ViewMatrix = mView; }
	void SetProjectionMatrix(const _float4x4& mProjection) { m_ProjectionMatrix = mProjection; }

	void Set_Texture(IDirect3DBaseTexture9* pTexture) { m_pTexture = pTexture; }


	_float Get_Ratio() { return m_fRatio; }
	void Set_Ratio(_float fRatio) { m_fRatio = fRatio; }

private:
	_float4x4 m_WorldMatrix;
	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjectionMatrix;

	IDirect3DBaseTexture9* m_pTexture = { nullptr };

	_float m_fRatio = { 0.f };

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

	D3DXHANDLE m_hRatio;

public:
	static CVIBuffer_BlinkableRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END