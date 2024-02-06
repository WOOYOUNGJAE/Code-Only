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
		_int	  iAttack;			//���ݷ�
		_float   	  fDurability;		//������
		_int	  iThrowingDamage;	//�������� ������ ������
		_int      iBulletCount;		//�Ѿ˼�
		//_int	  iAttackSpeed;		//���ݼӵ�
		//_float	  fRange;			//���ݹ���
	};

	struct Consumable_Info
	{
		_bool	 bAllive;
		_int	 iHeal;				//���� ���
		_int	 iThrowingDamage;	//�������� ������ ������
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