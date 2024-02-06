#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CGameObject
{
public:
	enum ANCHOR { LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN, CENTER, ANCHOREND };
	enum RENDER { RENDER_TEST, RENDER_BLEND, RENDER_END};
	typedef struct tagUI_Position_Size
	{
		//현재 LEVEL에서 원형을 찾기 위해 사용
		_uint iLevelIndex;
		//LEVEL_STATIC의 원형을 쓰기 위해서 받는다.
		_uint iLevelStatic;
		//컴포넌트 태그
		wstring strTextureTag;
		wstring strRendererTag;
		wstring strVIBuffer_RectTag;
		wstring strTransformTag;
		//패딩값
		_float fPaddingX, fPaddingY;
		//X, Y 사이즈, zbuffer
		_float fSizeX, fSizeY, fPosZ;
		//초기 렌더 여부 
		_bool bIsRender;
		_bool bIsAnimation;
		_float fAnimationRatio;
		//앤커값
		ANCHOR eAnchor;
		//렌더옵션
		RENDER eRender;
		//고정된 UI인지 파악
		_bool bIsFixed;
		//윈도우 사이즈 기준으로 배치하기 위한 변수
		_uint iWindowSizeX, iWindowSizeY;
	}UIDESC;

protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	virtual void SetActive_Unfixed(_float fActiveTime, _int iTexIndex);
	virtual _float Get_Ratio();
	virtual void Set_Ratio(_float fRatio);
	virtual void Set_Changed(_float fDest);
	virtual void Change_Ratio_while(_float fTimeDelta);
	void Set_Render(_bool bInput, _int iTexIndex);
	void Set_Render_Timer(_bool bIsRender, _bool bIsAnimation, _int iTexIndex, _float fDuration);
	void Set_TexIndex(_uint iInput);
	_bool Get_Timer()
	{
		return m_bIsSetTimer;
	}
	void Set_Timer(_bool bInput)
	{
		m_bIsSetTimer = bInput;
	}
	_uint GetTexIndex();
	_bool GetIsRender()
	{
		return m_bIsRender;
	}
	void Rotate_UI(const _float3& vAxis, float fRadian);
	void Set_Position(_float length, _float fRadian);

protected:
	class CTexture* m_pTextureCom = { nullptr };
	class CRenderer* m_pRendererCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferRectCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };

protected:
	//렌더여부 결정
	_bool m_bIsRender;

	//트리거 여부
	_bool m_bIsSetTimer;
	
	//변환행렬
	//위치가 고정된 UI들의 월드 변환행렬
	_float m_fTransX, m_fTransY, m_fTransZ;
	ANCHOR m_eAnchor;
	_float m_fPaddingX, m_fPaddingY, m_fSizeX, m_fSizeY;
	_float4x4 m_ViewMatrix, m_ProjMatrix;
	_float4x4 m_PreViewMatirx, m_PreProjMatrix;

	
	//애니메이션
	//애니메이션 여부
	_bool m_bIsAnimation;
	//텍스쳐 바꾸기 위한 멤버변수
	_float m_fTexIndex;

	_float m_fAnimationRatio;
	//타이머 누적시간 위한 누적시간
	_float m_fAccTime;
	//최대 텍스쳐 수.
	_uint m_iTexCnt;
	RENDER m_eRender;
	_float m_fDuration;
	
public:
	
	virtual CUI* Clone(void* pArg)=0;
	virtual void Free() override;

};

END