#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\Misc\Misc.h"
#include <iostream>
#include <random>
#include <chrono>

// suggestion https://yougame.biz/threads/70120/ we could steal something from here yes?


AntiAim g_AntiAim;

//test aa
bool m_bBreakBalance = false;
bool testbool = false;
bool testside = false;

bool Swtich = false;
bool lastmove = true;

void FreeStanding()
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint, g::pLocalEntity))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2, g::pLocalEntity))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y = view.y + 180;
	else
		g::pCmd->viewangles.y = FinalAngle;
}


void FreeStanding2()
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint, g::pLocalEntity))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 180;
					g_Menu.Config.FakeSide = 1;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal + 180;
					g_Menu.Config.FakeSide = 0;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2, g::pLocalEntity))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 180;
							g_Menu.Config.FakeSide = 1;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal + 180;
							g_Menu.Config.FakeSide = 0;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y = view.y + 180;
	else
		g::pCmd->viewangles.y = FinalAngle;
}

void testaa()
{
	Vector view;
	g_pEngine->GetViewAngles(view);

	if (g_Menu.Config.AntiAimSide == 0)
	{
		testside = true;
		g::pCmd->viewangles.y += 90;
		g::pCmd->viewangles.y += testbool ? -75 : 75;
	}
	else if (g_Menu.Config.AntiAimSide == 2)
	{
		testside = false;
		g::pCmd->viewangles.y -= 90;
		g::pCmd->viewangles.y += testbool ? 75 : -75;
	}
	else
	{
		g::pCmd->viewangles.y = view.y + 180;
	}
}


bool NextLbyUpdate()
{
	static float next_lby_update_time = 0;
	float curtime = g_pGlobalVars->curtime;

	auto animstate = g::pLocalEntity->AnimState();
	if (!animstate)
		return false;

	if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

float AntiAim::MaxDelta(C_BaseEntity* pEnt) {

	auto animstate = uintptr_t(pEnt->AnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001f) - 0.19999999f) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}

void UpDownRand()
{
	bool upDownRnd = Misc::RandomBool();
	if (upDownRnd)
		g::pCmd->viewangles.x = 88.f;
	else
		g::pCmd->viewangles.x = -88.f;
}

void nospread()
{
	bool upDownRnd = Misc::RandomBool();
	if (upDownRnd)
		g::pCmd->viewangles.x = 540;
	else
		g::pCmd->viewangles.x = -540;
}

void corruption()
{
	long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timeStamp = currentTime_ms;

	timeStamp = currentTime_ms;

	switch (timeStamp % 8)
	{
	case 1: g::pCmd->viewangles.y += 360;  break;
	case 2: g::pCmd->viewangles.y -= 180;  break;
	case 3: g::pCmd->viewangles.y += 360;  break;
	case 4: g::pCmd->viewangles.y -= 180;  break;
	case 5: g::pCmd->viewangles.y += 360;  break;
	case 6: g::pCmd->viewangles.y -= 180;  break;
	case 7: g::pCmd->viewangles.y += 360;  break;
	case 8: g::pCmd->viewangles.y -= 180;  break;
	}
}

static bool DesyncSwitch = false;

void Desync()
{
	int finalangle = g::RealAngle.y;
	float MaxDelta = g_AntiAim.MaxDelta(g::pLocalEntity);
	float MaxDeltaLby = 34;
	int val = MaxDelta;
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();

	Vector view;
	g_pEngine->GetViewAngles(view);

	DesyncSwitch = !DesyncSwitch;

	if (g_Menu.Config.DesyncAngle)
	{
		if (g_Menu.Config.YawMode == 9)
		{
			finalangle = view.y;
			g::pCmd->viewangles.y = finalangle;
			g_Menu.Config.MaxDesyncDelta = MaxDelta;
			return;
		}
		if (g_Menu.Config.YawMode == 10)
		{
			if (testside)
				finalangle += testbool ? MaxDelta : -MaxDelta;
			else
				finalangle += testbool ? -MaxDelta : MaxDelta;
			g::pCmd->viewangles.y = finalangle;
			g_Menu.Config.MaxDesyncDelta = MaxDelta;
			return;
		}
		switch (g_Menu.Config.DesyncMode)
		{
		case AntiAim::FAKE_STATIC:
			if (g_Menu.Config.JitterMode == AntiAim::JITTER_DESYNC && g_Menu.Config.JitterRange > 0)
				if (DesyncSwitch) MaxDelta -= g_Menu.Config.JitterRange;
			finalangle += g_Menu.Config.FakeSide ? -MaxDelta : MaxDelta;
			break;
		case AntiAim::FAKE_SWITCH:
			if (DesyncSwitch) MaxDelta -= 302;
			finalangle += g_Menu.Config.FakeSide ? -34 * MaxDelta : 34 * MaxDelta;
			break;
		case AntiAim::FAKE_BALANCE:
			if (DesyncSwitch) MaxDelta -= 302;
			finalangle += g_Menu.Config.FakeSide ? -MaxDelta : MaxDelta;
			break;
		}
		g::pCmd->viewangles.y = finalangle;
		g_Menu.Config.MaxDesyncDelta = MaxDelta;
	}
}

float updateTime;
float lastUpdate;
float wasmoving;
bool performBreak;

void LbyBreaker() {
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();
	float MaxDelta = g_AntiAim.MaxDelta(g::pLocalEntity);

	int Mult = g_Menu.Config.LbyMult;
	int Add = g_Menu.Config.LbyAdd;

	if (!performBreak) {
		if (Velocity > 0.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
			lastUpdate = g_pGlobalVars->curtime;
			wasmoving = true;
			m_bBreakBalance = true;
		}
		else {
			if (wasmoving && g_pGlobalVars->curtime - lastUpdate > 0.22f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				wasmoving = false;
				performBreak = true;
				m_bBreakBalance = false;
			}
			else if (g_pGlobalVars->curtime - lastUpdate > 1.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				performBreak = true;
				m_bBreakBalance = false;
			}
		}
	}
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void AntiAim::OnCreateMove()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
		return;

	if (!g_Menu.Config.Antiaim)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsNade())
		return;

	if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
		return;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	if (weapon && weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
	{
		float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
		bool canShoot = (weapon->GetNextPrimaryAttack() <= flServerTime);
		if (canShoot && (g::pCmd->buttons & IN_ATTACK))
			return;
	}

	Vector view;
	g_pEngine->GetViewAngles(view);

	if (g::bSendPacket)
		Swtich = !Swtich;
	testbool = !testbool;

	if (g::FreezeTime)
		return;

	switch (g_Menu.Config.PitchMode)
	{
	case 0:
		//Down
		g::pCmd->viewangles.x = 90;
		break;
	case 1:
		//Nazi Down
		g::pCmd->viewangles.x = 88;
		break;
	case 2:
		//Half-Down
		g::pCmd->viewangles.x = 45;
		break;
	case 3:
		//Zero
		g::pCmd->viewangles.x = 0;
		break;
	case 4:
		//Up
		g::pCmd->viewangles.x = -90;
		break;
	case 5:
		//Jitter Up/Down
		UpDownRand();
		break;
	case 6:
		//Custom
		g::pCmd->viewangles.x = g_Menu.Config.CustomPitchValue - 90;
		break;
	case 7:
		//nospread
		g::pCmd->viewangles.x = -540;
		break;
	case 8:
		//nospread
		nospread();
		break;
	case 9:
		g::pCmd->viewangles.x = view.x;
		break;
	default:
		break;
	}

	if (g_Menu.Config.LbyBreaker)
	{
		int Mult = g_Menu.Config.LbyMult;
		int Add = g_Menu.Config.LbyAdd;

		LbyBreaker();

		if (g_Menu.Config.StaticLby)
		{
			if (fabsf(g::pCmd->forwardmove) < 1.2f && fabsf(g::pCmd->sidemove) < 1.2f)
			{
				if (!g::pCmd->buttons && IN_DUCK)
					g::pCmd->sidemove = g::pCmd->command_number % 2 ? -1.10f : 1.10f;
				else
					g::pCmd->sidemove = g::pCmd->command_number % 2 ? -3.25f : 3.25f;
			}
		}

		if (performBreak) // using anything else makes 0 sense
		{
			if (g_Menu.Config.SendPacketOnBreak)
				g::bSendPacket = true;
			else
				g::bSendPacket = false;
			g::pCmd->viewangles.y += g_Menu.Config.FakeSide ? 360 - g_Menu.Config.BodyLean - Add : 360 - g_Menu.Config.BodyLeanInverted + Add;
			lastUpdate = g_pGlobalVars->curtime;
			performBreak = false;
			return;
		}
	}

	if (!g::bSendPacket && g::pLocalEntity->AnimState())
	{
		Desync();
	}
	else
	{
		switch (g_Menu.Config.YawMode)
		{
		case 0: //Backwards	
			g::pCmd->viewangles.y += g_Menu.Config.FakeSide ? 180 - g_Menu.Config.BodyLean : 180 - g_Menu.Config.BodyLeanInverted;
			break;
		case 1: //sideways1
			g::pCmd->viewangles.y = view.y + 90;
			break;
		case 2: //sideways2
			g::pCmd->viewangles.y = view.y - 90;
			break;
		case 3: //switch
			corruption();
			break;
		case 4: // spin
			g::pCmd->viewangles.y = (g_pGlobalVars->curtime * g_Menu.Config.SpeedSpin);
			break;
		case 5: // freestanding
			FreeStanding();
			break;
		case 6: // manual
			if (g_Menu.Config.ManualMode == 0)
			{
				if (g_Menu.Config.AntiAimSide == 0)
					g::pCmd->viewangles.y = view.y + 90;
				else if (g_Menu.Config.AntiAimSide == 1)
					g::pCmd->viewangles.y = view.y - 180;
				else if (g_Menu.Config.AntiAimSide == 2)
					g::pCmd->viewangles.y = view.y - 90;
			}
			else if (g_Menu.Config.ManualMode == 1)
			{
				if (g_Menu.Config.Back == true)
					g::pCmd->viewangles.y = view.y + 180;
				if (g_Menu.Config.Right == true)
					g::pCmd->viewangles.y = view.y + 90;
				if (g_Menu.Config.Left == true)
					g::pCmd->viewangles.y = view.y - 90;
			}
			break;
		case 7: // fixed custom
			g::pCmd->viewangles.y = g_Menu.Config.FixedCustomYaw;
			break;
		case 8:
			g::pCmd->viewangles.y = view.y;
			break;
		case 9:
			g::pCmd->viewangles.y = g_Menu.Config.FakeSide ? view.y + AntiAim::MaxDelta(g::pLocalEntity) : view.y - AntiAim::MaxDelta(g::pLocalEntity);
			break;
		case 10:
			testaa();
			break;
		case 11:
			FreeStanding2();
			break;
		case 12:
			if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
				g::pCmd->viewangles.y = view.y + 145;
			else
				FreeStanding();
			break;
		}

		if (g_Menu.Config.JitterRange > 0)
		{
			static bool slide = false;
			static int slideval = 0;
			switch (g_Menu.Config.JitterMode)
			{
			case JITTER_NORMAL:
				g::pCmd->viewangles.y += Swtich ? -g_Menu.Config.JitterRange : g_Menu.Config.JitterRange;
				break;
			case JITTER_RANDOM:
				g::pCmd->viewangles.y += RandomFloat(-g_Menu.Config.JitterRange, g_Menu.Config.JitterRange);
				break;
			case JITTER_SLIDE:
				if (slide)
				{
					if (slideval > (g_Menu.Config.JitterRange / 2))
						slide = false;
					else
						slideval += g_Menu.Config.SlideSpeed;
				}
				else
				{
					if (slideval < -(g_Menu.Config.JitterRange / 2))
						slide = true;
					else
						slideval -= g_Menu.Config.SlideSpeed;
				}
				g::pCmd->viewangles.y += slideval;
				break;
			}
		}

		if (g::pCmd->buttons & IN_USE)
		{
			g::pCmd->viewangles = view;
		}
	}
}