#pragma once
#include "Base.h"

BEGIN(Engine)

class CUI_Manager : public CBase
{
	DECLARE_SINGLETON(CUI_Manager);

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	HRESULT Reserve_UI_Manager(_uint iNumLevels);
	HRESULT Add_Prototype_UI(const wstring& strPrototypeUITag, class CUI* pUI);
	HRESULT Add_UI(_uint iLevelIndex, const _uint& iUITag, const wstring& strPrototypeUITag, void* pArg);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);


	_bool SetActive_Tooltip(void* pArg);
	_bool SetActive_Radio(void* pArg);
	_bool SetActive_Objective(void* pArg);
	_bool Set_Render(void* pArg);
	_bool Set_Render_Timer(void* pArg);
	_bool Update_Health(void* pArg);
	_bool Set_EnemyArrow(void* pArg);
	_bool Set_HitDistance(void* pArg);
	_bool Set_InvenSelect(void* pArg);
	_bool Set_RenderMap(void* pArg);
	_bool Set_Charging(void* pArg);
	_bool Set_Attack(void* pArg);
	_bool Cancel_Charging(void* pArg);
	_bool Update_Level(void* pArg);
	_bool Update_Objective(void* pArg);
	_bool Set_Objective_Complete(void* pArg);
	_bool Set_Bullet(void* pArg);

	_bool Set_PartiallyHidden(void* pArg);
	_bool Set_Hidden(void* pArg);
	_bool Set_Reveal(void* pArg);

	_bool Set_WeaponUI(void* pArg);
	_bool Set_Prompter(void* pArg);
	_bool Set_EnemyHP(void* pArg);
	_bool Set_Buff(void* pArg);
	_bool Update_Loading(void* pArg);
	_bool Set_Start_Off(void* pArg);
	_bool Update_Stamina(void* pArg);
	_bool Set_Notification(void* pArg);
	_bool Update_Guard(void* pArg);
	_bool Set_HP_UpEffect(void* pArg);
	_bool Update_Select_Weapon_HP(void* pArg);
	_bool Set_Inven_Item(void* pArg);
	_bool Set_Talk(void* pArg);
	_bool Set_Dead(void* pArg);



	void  Set_LateTick(_bool bInput)
	{
		m_LateTick = bInput;
	}


private:
	//UI��ü ���� ����
	map<wstring, class CUI*> m_Prototypes_UI;
	_uint					m_iNumLevels = { 0 };
	//UI��ü �纻 ����
	map<_uint, class CUI*>* m_UI = { nullptr };
	typedef map<_uint, class CUI*> UIS;

	map<class CTransform*, _uint> m_EnemyArrows;
	
	//For HudBox
	//������ ü��
	_uint m_OldHp;
	//������ ����ũ�� Tag
	_uint m_OldMikeUITag;
	//������ ���뽺���� �̹����� TexIndex
	_int m_OldPain;
	//���� �̹����� ���� ���� �ð� ����
	_float m_fAccTime;
	//���뽺���� �̹��� �����ð�
	_float m_fPainTime;

	//For.Map
	//���� 3bit�� ���� ���� �ǹ�(����Ʈ 2��, ����Ʈ 1��, ����Ʈ ��)
	//�߰� 2bit�� ���� ��ǥ ������Ʈ ��Ȳ(3��°����, 2��°����)
	//���� 3bit�� �� ��ǥ Ŭ������Ȳ(3��°, 2��°, 1��°)
	_byte m_bCurrentState = 0b00100000;

	//For.Attack
	_float m_fAttackTime = 0.0f;


	//For.Charging
	_bool m_bIsCharging = false;
	//��¡�ð�
	_float m_fCharingTime = 2.0f;
	//���� ��¡�ߴ°�
	_float m_fAccTime_Charging = 0.0f;

	_bool m_bIsHeavyReady = false;
	_bool m_bIsChargeAttack = false;

	//��񷹵� �����ð�
	_float m_fHeavyRenderTime = 1.0f;
	//��񷹵� ���ʵ��� �����ߴ°�
	_float m_fHeavyRenderAcc = 0.0f;

	//�������ϴ� �ð�
	_float m_fHeavyAttackTime = 1.0f;

	//�̹�ƽ�� ���� fTimeDelta ����
	_float m_fDeltaTime = 0.0f;
	//���� �������� Level
	_uint m_uiCurrentLevel = 0;


	//�� ü�¿� �Լ�
	_bool m_bIsEnemyDead = false;
	_uint m_OldEnemyHp = 1000;

	//���� �̺�Ʈ�� ����
	_bool m_bIsStaminaBuff = false;
	_bool m_bIsHealBuff = false;
	_bool m_bDoubleStamina = false;
	_bool m_bDoubleHeal = false;


	_float m_fEffectSceneRenderTime = 0.333333f;
 
	_bool m_bTalkTrigger1 = false;
	_bool m_bTalkTrigger2 = false;

	_uint m_uiLoadingSuccess = 0;

	vector<void*> m_InDistEnemy;

	_bool m_LateTick;

	_bool m_bDeadTrigger;
	_float m_fDeadAccTime;
	_float m_fDeadOnceTime;
private:
	class CUI* Find_Prototype_UI(const wstring& iPrototypeUITag);
	class CUI* Find_UI(const _uint& iLevelIndex, const _uint& iUITag);

public:
	virtual void Free()	override;


};

END

