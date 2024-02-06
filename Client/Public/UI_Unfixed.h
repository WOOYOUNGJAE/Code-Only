#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CUI_Unfixed final : public CUI
{
public:
	enum DIRECTION { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_END };
	typedef struct tagUI_UnfixedDesc
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;
		//_float fRenderDuration;
		DIRECTION eDir;
		UIDESC UIDesc;
	}UIUNFIXEDDESC;

protected:
	CUI_Unfixed(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Unfixed(const CUI_Unfixed& rhs);
	virtual ~CUI_Unfixed() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void SetActive_Unfixed(_float fActiveTime, _int iTexIndex);



public:
	static CUI_Unfixed* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI* Clone(void* pArg);
	virtual void Free() override;


private:
	_float m_fTransHiddenX, m_fTransHiddenY, m_fTransHiddenZ;
	//_float m_fRenderDuration;
	DIRECTION m_eDir;
};

END
