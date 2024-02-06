#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"

namespace Client
{
	typedef struct tagObjectDesc
	{
		MESH_TYPE eMeshType;
		OBJECT_TYPE eObjectType;
		std::wstring strTexturePath;

		D3DXVECTOR3 vPosition;
		D3DXQUATERNION vQuaternion;
		D3DXVECTOR3 vScale;

		bool bAlphaTesting;
		int iAlphaRef;

		std::wstring strTextureTag;
		ITEM_TYPE eItemType;
		TOOLTIP eTooltipType;
		int iRadioIndex;

		std::wstring strSoundKey;
	}OBJECTDESC;

	typedef struct tagEnemyObjectDesc
	{
		_float4x4 EnemyMatrix;
		std::wstring strSoundKey;
	}ENEMYOBJECTDESC;

	struct ENEMYRESPAWNDESC
	{
		_float4x4 EnemyMatrix;
		std::wstring strEnemyPrototype;
	};

	struct GoreDesc
	{
		_float3 vPosition;
		ENEMY_TYPE2 eEnemyType;
	};

	struct Equipment_Info
	{
		_bool	  bAllive;
		_int	  iAttack;			//공격력
		_float   	  fDurability;		//내구도
		_int	  iThrowingDamage;	//던졌을때 입히는 데미지
		_int      iBulletCount;		//총알수
		//_int	  iAttackSpeed;		//공격속도
		//_float	  fRange;			//공격범위
	};

	struct Consumable_Info
	{
		_bool	 bAllive;
		_int	 iHeal;				//힐링 밸류
		_int	 iThrowingDamage;	//던졌을때 입히는 데미지
	};

	struct Interactive_Info
	{
		_bool  bAlive;
		_bool  bBlock;
	};

	struct ThrowOnly_Info
	{
		_bool bAlive;
		_int iThrowingDamage;
	};

	typedef struct Player_Info
	{
		_int iHp;
		_float fStamina;
		_bool bStaminaChanged;
		_int iGuardGauge;
		_bool bGuardGaugeChanged;
	}PLAYERDESC;

	typedef struct Player_Attack_Info
	{
		_bool bHeavy;
		_bool bKick;
		CTransform* pPlayerTransform;
		_float3 vPickPos;
		_int iDmg;
		ITEM_TYPE eType;
	}PLAYERATTACKDESC;

	typedef struct Enemy_Attack_Info
	{
		CTransform* pEnemyTransform;
		_int iDmg;
	}ENEMYATTACKDESC;

	typedef struct tExcuteCheckDesc
	{
		CGameObject* pExecutionTarget = nullptr;
		_bool bToBeChopped = false;
		ENEMY_TYPE2 eType;
	}EXECUTECHECKDESC;

	struct FLOORCHANGEDESC
	{
		TCHAR* pFrom;
		TCHAR* pTo;

		_float3 vNewPosition;
	};
}