#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )

class Aimbot
{
public:
	matrix3x4_t Matrix[65][128];
	int Sequence;
	void OnCreateMove();
	int bestEntDmg;

private:
	bool Backtrack[65];
	bool ShotBacktrack[65];
	Vector Hitscan(C_BaseEntity* pEnt);
	bool HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance);
	void Autostop();
	//bool CanWallbang(float &dmg);
};
extern Aimbot g_Aimbot;