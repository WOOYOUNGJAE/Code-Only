#pragma once
#include "Client_Defines.h"
#include "Client_Struct.h"
#include "ItemDatabase.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Quad;
class CPicking;
class CLayer;
END


BEGIN(Client)
typedef struct tagInvenDesc
{
    Equipment_Info      tEquipInfo;
    Consumable_Info     tConsumInfo;
    // Interactive_Info    tInterInfo;
     //ThrowOnly_Info      tThrowOnlyInfo;
}INVENDESC;


class CItem_Equipment;
class CInventory : public CGameObject
  
{
   
private:
    CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
    CInventory(const CInventory& rhs);
    virtual ~CInventory() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void Late_Tick(_float fTimeDelta) override;

public:
    static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;


public:
  
    _bool       On_Add_Inven(void* pArg);
    _bool       Remove_Inven(void* pArg);
    INVENTORY   MapItemType_For_UI(ITEM_TYPE eItemtype);
    _int        GetInvenSize() { return m_iInvenSize; }
    _int        GetCurSlot() { return m_iCurSelectedSlot; }

public:
     CGameObject* Get_CurSelectedItem();

private:
    map<_int, CGameObject*>      m_mapInvenList;
    _bool                       m_bInvenFull = false;
    _bool                       m_bStorable = true;
    _int                        m_iCurSelectedSlot = 1;
    _int                        m_iInvenSize = 0;
    CGameObject* m_CurSelectedItem = { nullptr };

    //TODO: DEMO
private:
    _uint testIndex;
};
END
