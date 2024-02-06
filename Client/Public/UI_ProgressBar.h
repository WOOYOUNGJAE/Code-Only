#pragma once
#include "UI.h"
#include "Client_Defines.h"

BEGIN(Client)
class CUI_ProgressBar final : public CUI
{

public:
	enum BAR { BAR_PLAYER, BAR_ENEMY, BAR_LOADING, BAR_END};
	typedef struct tagUI_ProgressBarDesc
	{
		_float fDecrease;
		_float fIncrease;
		_float fTotal;
		BAR eBar;
		UIDESC UIDesc;
	}UIPROGRESSBARDESC;

protected:
	CUI_ProgressBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_ProgressBar(const CUI_ProgressBar& rhs);
	virtual ~CUI_ProgressBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CUI_ProgressBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fDecrease;
	_float m_fIncrease;
	_float m_fTotal;
	_float m_fCurrent;
	BAR m_eBar;
};

END
