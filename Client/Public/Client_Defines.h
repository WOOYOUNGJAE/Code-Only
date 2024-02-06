﻿#pragma once

#include <process.h>

namespace Client
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_WYJ, LEVEL_OUTDOOR, LEVEL_2F, LEVEL_END };
	enum TOOLTIP {
		TOOLTIP_ATTACK, TOOLTIP_BLOCK1, TOOLTIP_BLOCK2, TOOLTIP_CHOP, TOOLTIP_DODGE, TOOLTIP_FINISH, TOOLTIP_HEAL,
		TOOLTIP_KEY, TOOLTIP_KICK, TOOLTIP_SMALLWEAPON, TOOLTIP_STAMINA, TOOLTIP_STEALTH, TOOLTIP_SUSPICION, TOOLTIP_TABULATOR, TOOLTIP_THROW,
		TOOLTIP_ZIPPO, TOOLTIP_END
	};

	enum HEALTH {
		HEALTH_HEALTHY, HEALTH_SLIGHTLYHURT,
		HEALTH_HURT, HEALTH_MODERATELYHURT, HEALTH_BADLYHURT, HEALTH_END
	};

	enum INVENTORY {
		INVEN_BOLT, INVEN_BOTTLE, INVEN_DOCTORBAG, INVEN_KNIFE, INVEN_NAILBOARD,
		INVEN_PIPE, INVEN_REVOLVER, INVEN_SAWNOFF, INVEN_PISTOL, INVEN_TOMMY,
		INVEN_APPLE, INVEN_BANANA, INVEN_COLA, INVEN_MEDKIT, INVEN_BROKEN_BOTTLE
	};
	enum ITEM_TYPE
	{
		// 0 ~ 100 : Equipment
		ITEM_BOTTLE = 0,
		ITEM_AUTOMATIC_PISTOL,
		ITEM_TOMMY_GUN,
		ITEM_PIPE,
		ITEM_FRYING_PAN,
		ITEM_BOTTLE_BROKEN,

		// 101 ~ 200 : Consumable
		ITEM_MEDIKIT = 101,	//피 35 즉시 회복
		ITEM_BANANA,
		ITEM_APPLE,
		ITEM_COLA,

		// 201 ~ 300 : Interactive
		ITEM_DOOR = 201, // 자물쇠 철문
		ITEM_BOARD,
		ITEM_COMBOLOCK,
		ITEM_FAN, 		//이름이 안나오는데 환풍구고 파괴가능
		ITEM_BANANA_PEEL,
		ITEM_EATEN_APPLE,
		ITEM_COMIC_BOOK,
		ITEM_NEWS_PAPER,
		ITEM_ENTRANCE_KEY,
		ITEM_VENDING_MACHINE, 	//이름이 안나오는데 콜라 나오는 자판기	
		ITEM_CARDBOARD_BOX,   	//던져서 부셨을때 열쇠 한번 나온적 있음
		ITEM_RECEIPT,		//영수증
		ITEM_BASEMENT_KEY,
		ITEM_DUMMY_WAITER_BREAKER, //무슨 레버같은거
		ITEM_MALONES_OFFIC_KEY,
		ITEM_UPSTAIRS_KEY,
		ITEM_MALONES_NOTES,
		ITEM_BUTTON,
		ITEM_ELECTRIC_BOX,	//닿으면 감전되는 오브젝트
		ITEM_TRAIN,		//기차 지나갈때 치이면 피 깎임
		ITEM_FOUNTAIN,	//분수
		ITEM_CLUB_DOOR,// 클럽 문 왼쪽
		ITEM_WOODEN_DOOR, // 2층 내부 열리는 나무 문 왼쪽
		ITEM_BLOODY_HAND_PRINT,

		ITEM_CLUB_DOOR_R, // 클럽 문 오른쪽
		ITEM_WOODEN_DOOR_R, // 2층 내부 열리는 나무 문 오른쪽

		ITEM_METAL_DOOR, // 보스방 문 왼쪽
		ITEM_METAL_DOOR_R, // 보스방 문 오른쪽

		ITEM_FANCY_DOOR, // 1층 방 입구 문
		ITEM_FANCY_DOOR_R, // 1층 방 입구 문 오른쪽

		ITEM_METAL_CLUB_DOOR, // 1층 내려가는 문

		// 301 ~     :던지기 전용
		ITEM_SCRUNCHED_PAPER = 301,
		ITEM_WOODEN_BARREL,
		ITEM_OIL_BARREL,
		ITEM_PAINT_CAN,
		ITEM_TRASH_CAN,
		ITEM_TRASH_BIN,
		ITEM_TRAFFIC_CONE,
		ITEM_TOOL_BOX,
		ITEM_UMBRELLA_STAND,
		ITEM_TABLE_LAMP,
		ITEM_POT_PLANT,
		ITEM_WOODEN_STOOL,
		ITEM_TABLE_PLANT,
		ITEM_MARTINI_GLASS,
		ITEM_TUMBLER_GLASS,
		ITEM_POOL_BALL,
		ITEM_ASHTRAY_BIN,
		ITEM_GAS_CANISTER,	//던지면 가스통 폭발함
		ITEM_METAL_BARREL,
		ITEM_HAIR_BRUSH,
		ITEM_BOTTLE_ELSE,	//맥주병빼고 나머지 병들
		ITEM_BAR_STOOL,
		ITEM_JAR,
		ITEM_CAN,
		ITEM_ESPRESSO_MACHINE,
		ITEM_CANTAINER,
		ITEM_CHAMPAGNE_BUCKET,
		ITEM_COAT_HANGER,
		ITEM_HAND_BAG,
		ITEM_TRUTH_SERUM,
		ITEM_BUCKET,

		ITEM_POOL_BALL_0,
		ITEM_POOL_BALL_1,
		ITEM_POOL_BALL_2,
		ITEM_POOL_BALL_3,
		ITEM_POOL_BALL_4,
		ITEM_POOL_BALL_5,
		ITEM_POOL_BALL_6,
		ITEM_POOL_BALL_7,
		ITEM_POOL_BALL_8,
		ITEM_POOL_BALL_9,
		ITEM_POOL_BALL_10,
		ITEM_POOL_BALL_11,
		ITEM_POOL_BALL_12,
		ITEM_POOL_BALL_13,
		ITEM_POOL_BALL_14,
		ITEM_POOL_BALL_15,


		
		ITEM_NONE = 999,
	};

	const unsigned int	g_iWinSizeX = 1920;
	const unsigned int	g_iWinSizeY = 1080;

	enum MESH_TYPE 
	{ 
		MESH_QUAD,
		MESH_GRID,
		MESH_SPHERE,
		MESH_CUBE,
		MESH_CYLINDER,
		MESH_CIRCLE,
		MESH_BILLBOARD,
		MESH_NONE,
	};

	enum OBJECT_TYPE
	{
		OBJECT_STATIC,
		OBJECT_FLOOR,
		OBJECT_INTERATIVE,
		OBJECT_BILLBOARD,
		OBJECT_EVENT_SECTION,
		OBJECT_ENEMY,
		OBJECT_TOOLTIP_EVENT,
		OBJECT_RADIO_EVENT,
		OBJECT_BGM_EVENT,
		OBJECT_ETC_EVENT,
		OBJECT_NONE,
	};

	enum PLAYER_TEX {
		IDLE, RUN, PUNCH, HEAVYREADY, HEAVYPUNCH, GUARD,
		KICK1, KICK2, KICK3, KICK4,
		GRAB1, GRAB2, GRAB3, GRAB4, GRAB5, GRAB6, GRAB7, GRAB8, GRAB9, GRAB10,
		SPINKICK1, SPINKICK2, SPINKICK3, SPINKICK4,
		UPPERCUT,
		GRAB11, GRAB12, GRAB13, GRAB14, GRAB15, GRAB16, GRAB17, GRAB18, GRAB19, GRAB20,
		PIPEATK1, PIPEATK2, PIPEATK3, PIPEHEAVYATK3, PIPEHEAVYATK5, PIPEGUARD, PIPEIDLE,
		BOTTLEATK1, BOTTLEATK2, BOTTLEATK3, BOTTLEATK4, BOTTLEIDLE, BOTTLEBROKENATK, BOTTLEBROKENIDLE,
		MEDIKIT,APPLE,THROW,WOODENBARREL,
		GUARDL,GUARDR,CHOP1,CHOP2,CHOP3,CHOP4,
		BANANA,COLA,CARD,TOOLBOX, TRASHBIN1,TRASHBIN2,TRASH,
		FIREIDLE,FIRE1,FIRE2,FIRE3,
		BARREL,PAINT,POTPLANT,TABLEPLANT,TUMBLERGLASS,UMBRELLASTAND,COATHANGER,
		PISTOLIDLE,PISTOLFIRE,PISTOLMEELEE,PISTOLSPIN1, PISTOLSPIN2, PISTOLSPIN3, PISTOLSPIN4, PISTOLMEELEEATK,
		DELIAHOLD,
		BALL1,BALL2,BALL3, BALL4, BALL5, BALL6, BALL7, BALL8, BALL9, BALL10, BALL11, BALL12, BALL13, BALL14, BALL15, WOODENSTOOL,BARSTOOL,
		TEX_END
	};
	enum PLAYER_PUNCH { PUNCH_FORWARD, PUNCH_BACK, PUNCH_FINISH };
	enum PLAYER_SPEED { SPEED_WALK, SPEED_RUN, SPEED_END };
	enum PLAYER_WEAPON { WEAPON_NONE, WEAPON_PIPE, WEAPON_BOTTLE, WEAPON_BOTTLEBROKEN };
	enum EVADE_DIR { EVADE_A, EVADE_SA, EVADE_D, EVADE_SD, EVADE_S };

	enum ENEMY_TYPE {ENEMY_GOON, ENEMY_PIPEMAN, ENEMY_BOSS};
	enum ENEMY_TYPE2
	{
		ENEMY_TYPE_GOON,
		ENEMY_TYPE_PIPE_MAN,
		ENEMY_TYPE_MALONE,
		ENEMY_TYPE_DELIA,
		ENEMY_TYPE_END,
	};

#define pNOTHING nullptr

	

	
}

#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

extern HWND		g_hWnd;
using namespace Client;