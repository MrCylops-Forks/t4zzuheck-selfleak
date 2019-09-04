#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"

namespace g
{
    extern CUserCmd*      pCmd;
    extern C_BaseEntity*  pLocalEntity;
    extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool           bSendPacket;
	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         EnemyEyeAngs[65];
	extern Vector         AimbotHitbox[65][28];
	extern Vector         RealAngle;
	extern Vector         FakeAngle;
	extern bool           Shot[65];
	extern bool           Hit[65];
	extern int            Shots[65];
	extern int            MissedShots[65];
	extern bool			  PreserveDelete;
	extern int            flHurtTime;
	extern bool           isfakeducking;
	extern bool           fakeducked;
	extern bool           IsFakewalking;
	extern bool           IsSlowwalking;
	extern bool           FreezeTime;
	extern int            kills;
	extern float          killtime;
	extern int            globalAlpha;
	extern float          GoalLby;
	extern int            hits;
	extern int            shots;
	extern bool           knifeapply;
	extern float          viewMatrix[4][4];

	extern DWORD grenade;
	extern DWORD Tahoma;
	extern DWORD TahomaSmall;
	extern DWORD EspTahoma;
	extern DWORD EspSmallText;
	extern DWORD WeaponIcons;
	extern DWORD SansSerif;
	extern DWORD SansSerifBold;
	extern DWORD Indicators;
	extern DWORD Watermark;
	extern DWORD KillCounter;
	extern DWORD x88Font;

	//Remastered Esp
	extern DWORD ReEspText;
}