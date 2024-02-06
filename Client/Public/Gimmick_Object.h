#pragma once
#include "InteractiveObject.h"

/*
Gimmick TextureTag
{
	Gimmick_Fan,
	Gimmick_Key
}
*/
BEGIN(Client)
class CGimmick_Object final: public CInteractiveObject
{
private:
	CGimmick_Object(LPDIRECT3DDEVICE9 pGrahic_Device) : CInteractiveObject(pGrahic_Device){}
	CGimmick_Object(const CGimmick_Object& rhs) : CInteractiveObject(rhs){}
	virtual ~CGimmick_Object() = default;


public:
	static CGimmick_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_bool Get_DoorLocked() { return m_bDoorLocked; }
	virtual HRESULT Add_Components(const std::wstring& strTextureTag, MESH_TYPE eMeshType) override;
	virtual _bool ObjectHit(void* pArg) override;
	virtual _bool IsPicked(const _float4x4& WorldMatrix, const _float3& _ray, const _float3& _rayPos, _float3* pOut) override;
	void Set_RenderAfterUsed(_bool b) { m_bRenderAfterUsed = b; }
	void Set_Position(_float3 vPos);

private:
	_bool m_bPlayAnimation = true;
	_uint m_iAnimFrameMax = 0;
	_float m_fCurFrame = 0.f;
	_float m_fAnimSpeed = 1.f;

	_float3 m_vRotationPivot;
	_float3 m_vRightPivot;
	_float3 m_vScale;
	_float m_fRotateAmount = { 0.f };
	_float m_fRotateThreshold = { 0.f };

	_bool m_bRotateClockWise = { false };
	_bool m_bRotationPivotRight = { false };

	// after used
	CVIBuffer_Quad* m_pVIBufferCom_Used = nullptr;
	_bool m_bRenderAfterUsed = true;
	_bool m_bDoorLocked = true;
	//_bool m_bUsed = false;
	_bool m_bDoorOpening = false;
	class CGimmickManager* m_pGimmickMgrInstance = nullptr;

private:
	_float m_fDoorSpeed = { 2.f };
};

END