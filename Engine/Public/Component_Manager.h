#pragma once

#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Quad.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Grid.h"
#include "VIBuffer_Cylinder.h"
#include "VIBuffer_Sphere.h"
#include "VIBuffer_Circle.h"
#include "VIBuffer_ProgressableRect.h"
#include "VIBuffer_FlowableRect.h"
#include "VIBuffer_Billboard.h"
#include "VIBuffer_Mask.h"
#include "VIBuffer_BlinkableRect.h"
#include "Physics.h"
#include "FSM.h"
/*������Ʈ���� �������� ������ �����Ѵ�. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);

private:
	_uint									m_iNumLevels = { 0 };
	map<wstring, class CComponent*>*		m_pPrototypes = { nullptr };
	typedef map<wstring, class CComponent*>	PROTOTYPE;

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	virtual void Free() override;
};

END