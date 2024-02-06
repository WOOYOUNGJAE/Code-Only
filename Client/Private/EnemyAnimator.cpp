#include "stdafx.h"
#include "EnemyAnimator.h"

void CEnemyAnimator::Free()
{
}

void CEnemyAnimator::Tick(_float fTimeDelta)
{
	// Animation Frame
	_float fCurAnimSpeed = fCurAnimSpeed = m_fAnimSpeedArr[m_iCurAction];

	if (m_bIsReverse) // 역방향
	{
		fCurAnimSpeed *= -1.f;
		m_fCurFrame += static_cast<_float>(m_iAnimFrameMaxArr[m_iCurAction]) *
			fCurAnimSpeed * fTimeDelta;


		if (m_fCurFrame <= 0.f) // 다시 처음으로
		{
			m_fCurFrame = m_iAnimFrameMaxArr[m_iCurAction];
			++m_iAnimCycleCount;
		}
	}
	else // 정방향
	{
		m_fCurFrame += static_cast<_float>(m_iAnimFrameMaxArr[m_iCurAction]) *
			fCurAnimSpeed * fTimeDelta;

		if (static_cast<_uint>(m_fCurFrame) != m_iPrevFrame)
		{
			// Tick
			m_iPrevFrame = static_cast<_uint>(m_fCurFrame);
			// Event
			//OnEvent(&m_pOwner->Set_NextAction(2));
		}

		if (m_fCurFrame >= m_iAnimFrameMaxArr[m_iCurAction]) // 다시 처음으로
		{
			m_fCurFrame = 0.f;
			++m_iAnimCycleCount;
		}
	}


}
