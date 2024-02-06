#pragma once

#include "Engine_Defines.h"

namespace Engine
{
	typedef struct tagGraphicDesc
	{	
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND			hWnd;
		unsigned int	iSizeX, iSizeY;
		WINMODE			eWinMode;		
		
	}GRAPHICDESC;

	// D3DDECLUSAGE


	typedef struct tagVertex_Position_Texcoord
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR2			vTexcoord;		
	} VTXPOSTEX;

	typedef struct tagVertex_Position_Normal_Texcoord
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTexcoord;
	} VTXNORTEX;

	typedef struct OBB_Info
	{
		_float3 vCenterPos;
		_float3 vAxisDir[3]; //normalized
		_float fAxisLen[3];
	}OBBDESC;

	typedef struct AABB_Info
	{
		_float3 vMinPos;
		_float3 vMaxPos;
	}AABBDESC;

	typedef struct tag_Tooltip_Radio
	{
		_float fActiveTime;
		_int iTexIndex;
	}TOOLTIP_RADIO_DESC;

	typedef struct tag_Objective
	{
		_float fActiveTime;
	}OBJECTIVEDESC;

	typedef struct tag_Render
	{
		_uint iUITag;
		_bool bIsRender;
		_int iTexIndex;
	}RENDERDESC;

	typedef struct tag_Render_Timer
	{
		_uint iUITag;
		_bool bIsRender;
		_bool bIsAnimation;
		_float fDuration;
		_int iTexIndex;
	}RENDERTIMERDESC;

	typedef struct tag_Health
	{
		_uint iHealth;
	}HEALTHDESC;

	typedef struct tag_EnemyArrow
	{
		_float fEnemyGauge;
		class CTransform* pPlayerTransform;
		class CTransform* pEnemyTransform;
		_bool bIsRender;
	}ENEMYARROWDESC;

	typedef struct tag_HitDistance
	{
		void* pEnemy;
		_bool bInDistance;
	}HITDISTANCEDESC;

	typedef struct tag_InvenSelect
	{
		_uint iUITag;
		class CGameObject* pInteractiveObj;
	}INVENSELECT;

	typedef struct tag_WeaponUI
	{
		_uint uiUITag;
		_float fWeaponHP;
		_uint uiBulletCount;
		_bool bIsRender;
	}WEAPONUIDESC;

	typedef struct tag_EnemyHP
	{
		_uint uiEnemyTag;
		_uint uiOldHP; 
		_uint uiCurrentHP;
	}ENEMYHPDESC;

	typedef struct tag_Bullet
	{
		_uint uiUITag;
		_uint uiBulletCount;
		_bool bIsRender;
	}BULLETDESC;

	typedef struct tag_Loading
	{
		_uint Level_Loading;
		_uint Loading_Success_Count;
		_uint Loading_All_Count;
	}LOADINGDESC;

	typedef struct tag_ExecutionBack
	{
		 class CGameObject* pEnemy;
		_bool bChop;
	}EXECUTEBACKDESC;

	typedef struct tag_InvenItem
	{
		_bool bIsRender;
		_uint iUITag;
		_uint iTexIndex;
	}INVENITEMDESC;

	typedef struct tag_InvenHP
	{
		_float fItemHP;
		_uint iUITag;
	}INVENHPDESC;

	typedef struct tag_EffectInfo
	{
		_float3 vPos;
		_float3 vScale;
		_bool   bActive = false;
		_uint iIndex;
		//EFFECTARG effectarg;
	}EFFECTDESC;

	typedef struct tag_MoveableEffectInfo
	{
		_float3 vPos;
		_float3 vScale;
		_bool   bActive = false;
		_uint iIndex;
		_float3 vDir;
		//EFFECTARG effectarg;
	}MOVEABLEEFFECTDESC;

	typedef struct tag_EffectInfo2
	{
		_float3 vPos;
		_float3 vScale;
		_bool   bActive = false;
		_uint iIndex;
		class CTransform* pTargetTransform;
		//EFFECTARG effectarg;
	}EFFECTDESC2;

	typedef struct tagEffectArg {
		_uint eEffecttype;
		_int iEffectIdentity[13][10];
	}EFFECTARG;

	

	typedef struct tag_ParticleEvent
	{
		_uint ParticleType;
		_float3 targetPos;
	}PARTICLEEVENTDESC;

	typedef struct tag_ParticleInfo
	{
		std::wstring strParticleTexTag;//텍스쳐 묶음태그
		_uint iIndex; // 몇번째 세트인지
		_uint ParticleType; // Goon, Pipe, Wood
		_float3 fSpread;// 파티클이 퍼지는정도
		_uint iStatic;// LEVEL_STATIC
		_uint iCurrentLevel; // LEVEL_OUTDOOR
		_float fHeight; //높이
		_float fGravityStr;//중력강도
		_float fDirStr;//iSpread를 노말라이즈 하고 곱할 크기
		_float3 vec3ParticleSize; //사이즈
	}PARTICLEDESC;

	typedef struct tag_FadeInfo
	{
		_float fAccTime;
		_uint OldChannel;
		_uint CurrentChannel;
		_float OldVolume;
		_float CurrentVolume;
	}FADEINFO;

	typedef struct tag_BGMInfo
	{
		TCHAR* pSoundKey;
		_uint ChannelId;
		_float fVolume;
	}BGMINFO;

}
