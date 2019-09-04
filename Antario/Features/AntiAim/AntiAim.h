#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"

class AntiAim
{
public:

	enum LbyMode
	{
		LBY_REAL,
		LBY_NORMAL,
		LBY_MAXDELTA,
		LBY_MAXDELTAMULT,
		LBY_MAXDELTAADD,
		LBY_ADD,
		LBY_SPIN
	};

	enum DesyncMode
	{
		FAKE_STATIC,
		FAKE_SWITCH,
		FAKE_BALANCE
	};

	enum JitterMode
	{
		JITTER_NORMAL,
		JITTER_RANDOM,
		JITTER_DESYNC,
		JITTER_SLIDE
	};

	void OnCreateMove();
	float MaxDelta(C_BaseEntity* pEnt);
};
extern AntiAim g_AntiAim;