#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CEnemy_HealthSystem : public CBase
{
	
private:
	CEnemy_HealthSystem() = default;
	virtual ~CEnemy_HealthSystem() = default;

public: // Getter Setter
	_uint Get_CurHP() { return m_iCurHP; }
	_uint Get_PrevHP() { return m_iPrevHP; }
	_bool Is_Dazed() { return 0 < m_iCurHP && m_iCurHP < m_iDazeHP; }
	void Set_HealthPlus(_uint iPlus) { m_iCurHP += iPlus; }
	void Make_Dead() { m_iPrevHP = m_iCurHP; m_iCurHP = 0; }
public: // LifeCycle
	static CEnemy_HealthSystem* Create(class CEnemyBase* pOwner = nullptr);
	void Initialize();
	_bool Tick(_float fTimeDelta, _bool& refMakeDazed); //죽으면 False
	void LateTick(_float fTimeDelta);
	virtual void Free() override;

public:
	void Tick_CalculateHP(_float fTimeDelta);
	void Tick_Recover(_float fTimeDelta);
	void Damaged(_int iDamage);
	

private:
	// Result
	_ubyte m_byResult = 1 << 0;
	_ubyte m_byDead = 0 << 0;
	_ubyte m_byNormal = 1 << 0;
	_ubyte m_byDazed = 1 << 1;
	_bool m_bMakeDazed = false;
	//HP
	_uint m_iMaxHP = 100;
	_int m_iCurHP = 100;
	_int m_iPrevHP = 100;
	_uint m_iDazeHP = 40; // 맞아서 0초과 이 이하 되면 Dazed
	//Damage
	_uint m_iDamage = 0;
	//Timer
	_float m_fRecoverAccTime;
	
	CEnemyBase* m_pOwner = nullptr;
};

END