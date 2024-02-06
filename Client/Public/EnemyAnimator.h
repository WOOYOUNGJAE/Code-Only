#pragma once
#include "Client_Defines.h"
#include "EnemyBase.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CEnemyAnimator : public CBase
{
private:
	CEnemyAnimator(CEnemyBase* pOwner) : m_pOwner(pOwner){ };
	virtual ~CEnemyAnimator() = default;

	CEnemyAnimator* Create(CEnemyBase* pOwner);
	void Free() override;
	void Initialize();
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	CTexture* Render();

	void OnEvent(void*);
private:
	CEnemyBase* m_pOwner = nullptr;

	// Animation(Texture)Arr
	CTexture* m_EightDirTexturesArr[CEnemyBase::ENEMY_TEXTURE_DIR_END][CEnemyBase::EIGHT_DIR_ACTION_END]{};
	CTexture* m_OnlySouthTexturesArr[CEnemyBase::ONLY_SOUTH_ACTION_END]{};

	_uint m_iAnimFrameMaxArr[CEnemyBase::ALL_ACTION_STATE_END]{}; // 동작당 프레임 개수
	_float m_fAnimSpeedArr[CEnemyBase::ALL_ACTION_STATE_END]{};
	//_float m_fAnimSpeed, 1 => 1초당 1프레임
	_uint m_iCurAction = CEnemyBase::ACTION_STAND;
	_uint m_iNextAction = CEnemyBase::ALL_ACTION_STATE_END;
	_float m_fCurFrame = 0.f;
	_uint m_iPrevFrame = 0.f;
	_uint m_iAnimCycleCount = 0; // 현 애니메이션 몇 번 재생되었는지

	_bool m_bIsReverse = false;
};

END