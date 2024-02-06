#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CVIBuffer_ProgressableRect;
END

BEGIN(Client)

class CProgressableUI : public CUI
{
protected:
	CProgressableUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CProgressableUI(const CProgressableUI& rhs);
	virtual ~CProgressableUI() = default;



public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _float Get_Ratio() { return m_fRatio; }
	virtual void Set_Ratio(_float fRatio) 
	{ 
		if (true == m_bSetChanged)
			m_bSetChanged = false;
		m_fRatio = fRatio; 
	}
	virtual void Set_Changed(_float fDest);
	virtual void Change_Ratio_while(_float fTimeDelta);

private:
	CVIBuffer_ProgressableRect* m_pVIBufferProgressableRectCom = { nullptr };

	_float m_fRatio = { 1.0f };
	_float m_fProgressAccTime = { 0.0f };
	_float m_fProgressDuration = { 2.0f };
	_bool m_bSetChanged = false;
	_float m_fDiff = 0.0f;

public:
	static CProgressableUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI* Clone(void* pArg);
	virtual void Free() override;
};

END