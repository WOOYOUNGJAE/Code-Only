#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Quad;
class CVIBuffer_Cube;
class CPicking;
class CLayer;
class CPhysics;
END

BEGIN(Client)
class CInteractiveObject :
	public CGameObject
{
public:

protected:
	CInteractiveObject(LPDIRECT3DDEVICE9 pGrahic_Device);
	CInteractiveObject(const CInteractiveObject& rhs);
	virtual ~CInteractiveObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _bool	IsPicked(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOut);
	virtual _bool   OnItemPicked(void* pArg);
	void  Set_Render_Item(_bool brender) { m_bRender = brender; }
	void  Set_Pickable_Item(_bool bpickable) { m_bPickable = bpickable; }
	void  Set_Enable_Item(_bool bEnable) { m_bEnable = bEnable; }
	void  Set_Effect_Item(_bool EffectOn) { m_bEffectOn = EffectOn; }
	void  Set_Prompter(bool bPrompt) { m_bPrompt = bPrompt; }
	_bool Get_Prompter() { return m_bPrompt; }
	_bool Get_Enable() { return m_bEnable; }



	void  Ready_ForThrow(class CTransform* _transform);
	virtual _bool  ObjectHit(void* pArg);

	void  Item_Hited(const _float3& vPickedPosition, CInteractiveObject* pTarget);
	void Item_Picked(CInteractiveObject* pTarget);
	
public:
	ITEM_TYPE Get_Item_Type() { return m_eItemType; }
//	_bool Is_Storable() { return m_bStorable; }

	///*todod*/  CTransform * Get_TransformCom() { return m_pTransformCom; }
protected:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Outline = { nullptr };
	CTexture* m_pTextureCom_Frame = { nullptr };
	CTexture* m_pTextureCom_Dead = { nullptr };
	CTexture* m_pDebugTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Quad* m_pVIBufferCom = { nullptr };
	CVIBuffer_Cube* m_pCubeVIBufferCom = { nullptr };
	CPhysics* m_pPhysicsCom = { nullptr };
	//CPicking* m_pPickingCom = { nullptr };

	CTransform* m_pPlayerTransform = { nullptr };
	_float4x4 m_matNotRot;

	_float					m_fFrame = { 0.0f };

	_float m_fSoundVolume = 0.5f;

protected:
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CInteractiveObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
	//CLayer* Get_Layer() { CLayer::Find_Layer() }

	void SetSoundVolume(_float f) { m_fSoundVolume = f; }
	_float GetSoundVolume() { return m_fSoundVolume; }

public:
	class CItem_Equipment* pEquip = { nullptr };
	class CItem_Interactive* pInter = { nullptr };
	class CPlayer* pPlayer = { nullptr };

protected:
	_bool m_bAlphaTesting = { false };
	_int m_iAlphaRef = { 0 };
	_bool m_bThrow = { false };
	//_float3* m_pDistance = { nullptr };
	

protected:
	ITEM_TYPE m_eItemType = ITEM_PIPE;
	_bool m_bPicked = false;
	_float m_fDistance = 0;
	_bool m_bBillboard = { false };
	_bool m_bAlive = true;
	_int m_iThrowingDamage;
	//_bool m_bStorable = true;
	_bool m_bPickable = true;
	_bool m_bRender = true;


	_bool m_bEffectOn = false;


	_bool m_bPrompt = false;

	_bool       m_bBroken = false; //사운드체크용 불변수
	_bool		m_bItemCollideSound = false; //던져서 맞았을때 사운드
};

END

