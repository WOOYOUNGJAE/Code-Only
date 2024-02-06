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
		//���� LEVEL���� ������ ã�� ���� ���
		_uint iLevelIndex;
		//LEVEL_STATIC�� ������ ���� ���ؼ� �޴´�.
		_uint iLevelStatic;
		//������Ʈ �±�
		wstring strTextureTag;
		wstring strRendererTag;
		wstring strVIBuffer_RectTag;
		wstring strTransformTag;
		//�е���
		_float fPaddingX, fPaddingY;
		//X, Y ������, zbuffer
		_float fSizeX, fSizeY, fPosZ;
		//�ʱ� ���� ���� 
		_bool bIsRender;
		_bool bIsAnimation;
		_float fAnimationRatio;
		//��Ŀ��
		ANCHOR eAnchor;
		//�����ɼ�
		RENDER eRender;
		//������ UI���� �ľ�
		_bool bIsFixed;
		//������ ������ �������� ��ġ�ϱ� ���� ����
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
	//�������� ����
	_bool m_bIsRender;

	//Ʈ���� ����
	_bool m_bIsSetTimer;
	
	//��ȯ���
	//��ġ�� ������ UI���� ���� ��ȯ���
	_float m_fTransX, m_fTransY, m_fTransZ;
	ANCHOR m_eAnchor;
	_float m_fPaddingX, m_fPaddingY, m_fSizeX, m_fSizeY;
	_float4x4 m_ViewMatrix, m_ProjMatrix;
	_float4x4 m_PreViewMatirx, m_PreProjMatrix;

	
	//�ִϸ��̼�
	//�ִϸ��̼� ����
	_bool m_bIsAnimation;
	//�ؽ��� �ٲٱ� ���� �������
	_float m_fTexIndex;

	_float m_fAnimationRatio;
	//Ÿ�̸� �����ð� ���� �����ð�
	_float m_fAccTime;
	//�ִ� �ؽ��� ��.
	_uint m_iTexCnt;
	RENDER m_eRender;
	_float m_fDuration;
	
public:
	
	virtual CUI* Clone(void* pArg)=0;
	virtual void Free() override;

};

END