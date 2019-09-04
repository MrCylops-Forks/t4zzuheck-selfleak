#include "GlobalVars.h"

namespace g
{
    CUserCmd*      pCmd         = nullptr;
    C_BaseEntity*  pLocalEntity = nullptr;
    std::uintptr_t uRandomSeed  = NULL;
	Vector         OriginalView;
	bool           bSendPacket  = true;
	bool		   LagPeek      = false;
	int            TargetIndex  = -1;
	Vector         EnemyEyeAngs[65];
	Vector         AimbotHitbox[65][28];
	Vector         RealAngle;
	Vector         FakeAngle;
	bool           Shot[65];
	bool           Hit[65];
	int            Shots[65];
	int            MissedShots[65];
	bool		   PreserveDelete;
	int            flHurtTime;
	bool           fakeducked = false;
	bool           isfakeducking = false;
	bool           IsFakewalking = false;
	bool           IsSlowwalking = false;
	bool           FreezeTime = false;
	int            kills = 0;  
	float          killtime = 0.0f;
	int            globalAlpha = 0;
	float          GoalLby = 0;
    int            hits = 0;
	int            shots = 0;
	bool           knifeapply;
	float          viewMatrix[4][4];

	DWORD grenade;
	DWORD Tahoma;
	DWORD TahomaSmall;
	DWORD EspTahoma;
	DWORD EspSmallText;
	DWORD WeaponIcons;
	DWORD SansSerif;
	DWORD SansSerifBold;
	DWORD Indicators;
	DWORD Watermark;
	DWORD KillCounter;
	DWORD x88Font;

	//Remastered Esp
	DWORD ReEspText;
}
