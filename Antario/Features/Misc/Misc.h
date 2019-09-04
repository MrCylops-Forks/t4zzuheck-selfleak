#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\Aimbot\EnginePrediction.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Hooks.h"
#include "..\..\Menu\Hotkeys.h"
#include "..\..\Utils\Interfaces.h"
#include "..\..\Utils\Math.h"
#include "..\Aimbot\EnginePrediction.h"
#include <iostream>
#include <algorithm>

#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)
#define Square(x) ((x)*(x))

class Misc
{
public:
	int counter = 0;
    void OnCreateMove()
    {
        this->pCmd   = g::pCmd;
        this->pLocal = g::pLocalEntity;

		this->DoAutostrafe();
		this->DoBhop();
		this->DoSlideWalk();
		this->DoSlowWalk();
		this->AnimClantag();
		this->DoKnifeLeftSide();
		this->LinearExtrapolations();
		this->DoDisablePostProcess();
		fakeduck2();
    };

	void fakeduck2()
	{
		if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.FakeDuckKey)))
		{
			if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
				return;

			auto NetChannel = g_pEngine->GetNetChannel();

			if (!NetChannel)
				return;
			int fakelag_limit = 14;
			g::bSendPacket = (NetChannel->m_nChokedPackets >= fakelag_limit);
			int choked_goal = fakelag_limit / 2;
			bool should_crouch = NetChannel->m_nChokedPackets >= choked_goal;

			if (should_crouch)
			{
				g::pCmd->buttons |= IN_DUCK;
				g::isfakeducking = true;
			}
			else
			{
				g::pCmd->buttons &= ~IN_DUCK;
				g::isfakeducking = false;
			}

			g::fakeducked = true;
		}
		else
			g::fakeducked = false;
	}


	void EdgeJump()
	{
		int old_flags = g::pLocalEntity->GetFlags();

		g_EnginePrediction.Start();
		{
			if (g_Menu.Config.EdgeJump && (old_flags & FL_ONGROUND) && !(g::pLocalEntity->GetFlags() & FL_ONGROUND))
				g::pCmd->buttons |= IN_JUMP;
		}
		g_EnginePrediction.End();
	}

	void MovementFix(Vector& oldang)
	{
		Vector vMovements(g::pCmd->forwardmove, g::pCmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = g::pCmd->viewangles;
		aRealDir.Clamp();

		g_Math.AngleVectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		VectorNormalize(vRealF);
		VectorNormalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		g_Math.AngleVectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		VectorNormalize(vWishF);
		VectorNormalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * g::pCmd->forwardmove + vWishR[0] * g::pCmd->sidemove;
		vWishVel[1] = vWishF[1] * g::pCmd->forwardmove + vWishR[1] * g::pCmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		g::pCmd->forwardmove = x;
		g::pCmd->sidemove = y;
	}

	void ThirdPerson(ClientFrameStage_t curStage)
	{
		static bool init = false;

		if (GetKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.ThirdpersonHotkey)))
		{
			if (init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("thirdperson");
				g_Menu.Config.IsThirdperson = true;
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("firstperson");
				g_Menu.Config.IsThirdperson = false;
			}
			init = true;
		}

		if (curStage == FRAME_RENDER_START && g_Menu.Config.IsThirdperson)
		{
			g_pPrediction->SetLocalViewAngles(Vector(g::RealAngle.x, g::RealAngle.y, 0));
			if (g_Menu.Config.ZeroPitchOnLand)
			{
				if (g::pLocalEntity->AnimState()->m_bInHitGroundAnimation && !GetAsyncKeyState(VK_SPACE))
				{
					if (g::pLocalEntity->AnimState()->m_flHeadHeightOrOffsetFromHittingGroundAnimation)
					{
						g_pPrediction->SetLocalViewAngles(Vector(0, g::RealAngle.y, 0));
					}
				}
			}
			g::pLocalEntity->UpdateClientAnimation();
			g_pPrediction->SetLocalViewAngles(Vector(0, g::FakeAngle.y, 0));
		}


		if (!g::pLocalEntity->IsAlive())
		{
			ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
			*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
			sv_cheats->SetValue(1);
			g_pEngine->ExecuteClientCmd("firstperson");
			g_Menu.Config.IsThirdperson = false;
		}
	}

	void Crosshair()
	{
		if (!g::pLocalEntity)
			return;
		
		if (g::pLocalEntity->IsScoped() && g_Menu.Config.NoScope && g::pLocalEntity->IsAlive())
		{
			int Height, Width;
			g_pEngine->GetScreenSize(Width, Height);

			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle();

			float x = Width / 2;
			float y = Height / 2;
			int dy = Height / 90;
			int dx = Width / 90;
			x -= (dx*(punchAngle.y));
			y += (dy*(punchAngle.x));

			Vector2D screenPunch = { x, y };

			g_pSurface->Line(0, Height / 2, Width, Height / 2, Color(0, 0, 0, 255));
			g_pSurface->Line(Width / 2, 0, Width / 2, Height, Color(0, 0, 0, 255));
		}

		static bool init = false;
		static bool init2 = false;

		if (g_Menu.Config.Crosshair && g_Menu.Config.Aimbot)
		{
			if (g::pLocalEntity->IsScoped())
			{
				if (init2)
				{
					ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
					*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
					sv_cheats->SetValue(1);

					g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 0");
					g_pEngine->ExecuteClientCmd("crosshair 0");
				}
				init2 = false;
			}
			else
			{
				if (!init2)
				{
					ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
					*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
					sv_cheats->SetValue(1);

					g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 3");
					g_pEngine->ExecuteClientCmd("crosshair 1");
				}
				init2 = true;
			}

			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);

				g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 0");
				g_pEngine->ExecuteClientCmd("crosshair 1");
			}
			init = true;
		}
	}

	
	void FixedBombPlant(CUserCmd* cmd)
	{
		if (!(cmd->buttons & IN_USE) && !(cmd->buttons & IN_ATTACK))
			return;
		if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
			return;

		C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)g::pLocalEntity->GetActiveWeapon();
		auto isbomb = pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_C4;

		if (!pWeapon || !isbomb || g::pLocalEntity->InBombZone() && g::pLocalEntity->GetFlags() & FL_ONGROUND)
			return;//m_bInBombZone

		//keep antiaiming/dont run 10m into site
		cmd->buttons &= ~IN_USE;
		cmd->buttons &= ~IN_ATTACK;
		//jk
	}

	void roll_fov(CViewSetup* pSetup)
	{
		if (g_pEngine->IsInGame() && g::pLocalEntity && g::pLocalEntity->GetActiveWeapon())
		{
			auto ViewModel = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntityFromHandle(g::pLocalEntity->m_hViewModel()));
			if (ViewModel)
			{
				auto eyeAng = pSetup->angles;
				eyeAng.z -= g_Menu.Config.roll;
				ViewModel->SetAbsAngles(eyeAng);
			}
		}
	}

	#define PI_F	((float)(M_PI))

	void onheadblockbot(CUserCmd* cmd) {
		if (cmd->sidemove != 0 || cmd->forwardmove != 0)
			return;

		C_BaseEntity* local = g::pLocalEntity;
		C_BaseEntity* closest_teammate = nullptr;

		if (!local)
			return;


		float dist = 245.f;
		for (int i = 1; i < g_pGlobalVars->maxClients; i++)
		{
			auto current_ent = g_pEntityList->GetClientEntity(i);
			if (!current_ent || current_ent == local)
				continue;

			if (!current_ent->IsAlive() || current_ent->IsDormant() || current_ent->GetTeam() != local->GetTeam())
				continue;

			float current_dist = local->GetAbsOrigin().DistTo(current_ent->GetAbsOrigin());
			if (current_dist < dist) {
				dist = current_dist;
				closest_teammate = current_ent;
			}
		}

		if (!closest_teammate)
			return;

		Vector entPos = closest_teammate->GetAbsOrigin();
		Vector ownPos = local->GetAbsOrigin();

		/*if (entPos.z < ownPos.z)
		{
			Vector angles = g_Math.CalcAngle(ownPos, entPos);

			float dog = closest_teammate->GetVelocity().Length2D();
			float me = local->GetVelocity().Length2D();
			angles.y -= g::pLocalEntity->GetEyeAngles().y;
			g_Math.NormalizeAngles(angles);//(angles);
			if (angles.y < 0.0f)
				cmd->sidemove = dog * 1.8;
			else if (angles.y > 0.0f)
				cmd->sidemove = -dog * 1.8;
		}
		else if(entPos.z > ownPos.z)
		{*/
			float yaw = cmd->viewangles.y;
			Vector VecForward = ownPos - entPos;

			Vector translatedVelocity = Vector(
				(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
				(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
				VecForward.z
			);
	
			cmd->forwardmove = -translatedVelocity.x * 20.f;
			cmd->sidemove = translatedVelocity.y * 20.f;
	}


	void acpectratio(float numbero)
	{
		ConVar* yessoftware = g_pCvar->FindVar("r_aspectratio");
		*(int*)((DWORD)& yessoftware->fnChangeCallback + 0xC) = 0;
		yessoftware->SetValue(numbero);
	}

	void SetName(const char* name)
	{
		ConVar* nameConvar = g_pCvar->FindVar("name");
		*(int*)((DWORD)& nameConvar->fnChangeCallback + 0xC) = 0;
		nameConvar->SetValueChar(name);
	}

	Color getHealthColor(C_BaseEntity* Entity)
	{
		int red, green, blue, alpha;
		int health = Entity->GetHealth();
		red = 255 - (health * 2.55);
		green = health * 2.55;
		blue = 0;
		return Color(red, green, blue, 255);
	}

	static void SetClanTag(std::string text, std::string text2)
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
			return;
		Hooks::SetClanTag(text.c_str(), text.c_str());
	}

	static void Marquee(std::string& clantag)
	{
		std::string temp = clantag;
		clantag.erase(0, 1);
		clantag += temp[0];
	}

	static void AnimClantag()
	{
		/*if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity)
		{
			g_Menu.Config.ClanTagMode = 0;
			SetClanTag(" ", " ");
		}*/

		if (g_Menu.Config.ClanTagMode == 1)
		{
			static float oldTime;
			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.25f)
				{
					SetClanTag(g_Menu.Config.ClantagText, g_Menu.Config.ClantagText);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 2)
		{
			// \u0020
			static std::string cur_clantag;
			auto net_channel = g_pEngine->GetNetChannelInfo();
			float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
			static float oldTime;
			if (g_pGlobalVars->curtime - oldTime >= 0.25f)
			{
				switch (int((flServerTime + net_channel->GetAvgLatency(MAX_FLOWS)) * 3.f ) % 18)
				{
				case 0:  cur_clantag = "        t"; break;
				case 1:  cur_clantag = "       t4"; break;
				case 2:  cur_clantag = "      t4z"; break;
				case 3:  cur_clantag = "     t4zC"; break;
				case 4:  cur_clantag = "    t4zCh"; break;
				case 5:  cur_clantag = "   t4zChe"; break;
				case 6:  cur_clantag = "  t4zChea"; break;
				case 7:  cur_clantag = " t4zCheat"; break;
				case 8:  cur_clantag = "t4zCheats"; break;
				case 9:  cur_clantag = "4zCheats "; break;
				case 10: cur_clantag = "zCheats  "; break;
				case 11: cur_clantag = "Cheats   "; break;
				case 12: cur_clantag = "heats    "; break;
				case 13: cur_clantag = "eats     "; break;
				case 14: cur_clantag = "ats      "; break;
				case 15: cur_clantag = "ts       "; break;
				case 16: cur_clantag = "s        "; break;
				case 17: cur_clantag = "         "; break;
				}
				SetClanTag(cur_clantag, cur_clantag);
				oldTime = g_pGlobalVars->curtime;
			}
		}
		else if (g_Menu.Config.ClanTagMode == 3)
		{
			// \u0020
			static std::string cur_clantag = "gamesense \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020";
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.5f)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 4)
		{
			// \u0020
			static std::string cur_clantag = "Nasa Technology \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020 \u0020";
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.3f)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 5)
		{
			// \u0020
			static std::string cur_clantag = " Software.gov ";
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.25f)
				{
					Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
		else if (g_Menu.Config.ClanTagMode == 6)
		{
			// \u0020
			static std::string cur_clantag = "x88Cheats";
			static float oldTime;

			if (g_pEngine->IsConnected())
			{
				if (g_pGlobalVars->curtime - oldTime >= 0.25f)
				{
					//Marquee(cur_clantag);
					SetClanTag(cur_clantag, cur_clantag);
					oldTime = g_pGlobalVars->curtime;
				}
			}
		}
	}

	static bool Misc::RandomBool()
	{
		return 0 + (rand() % (1 - 0 + 1)) == 1;
	}

private:
    CUserCmd*     pCmd;
    C_BaseEntity* pLocal;

    void DoBhop() const
    {
        if (!g_Menu.Config.Bhop)
            return;
		if (!g::pLocalEntity->IsAlive())
			return;
        static bool bLastJumped = false;
        static bool bShouldFake = false;
        if (!bLastJumped && bShouldFake)
        {
            bShouldFake = false;
            pCmd->buttons |= IN_JUMP;
        }
        else if (pCmd->buttons & IN_JUMP)
        {
            if (pLocal->GetFlags() & FL_ONGROUND)
                bShouldFake = bLastJumped = true;
            else 
            {
                pCmd->buttons &= ~IN_JUMP;
                bLastJumped = false;
            }
        }
        else
            bShouldFake = bLastJumped = false;
    }

	void DoSlideWalk()
	{
		if (!g_Menu.Config.SlideWalk)
			return;

		if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
			return;

		if (pCmd->forwardmove > 0)
		{
			pCmd->buttons |= IN_BACK;
			pCmd->buttons &= ~IN_FORWARD;
		}

		if (pCmd->forwardmove < 0)
		{
			pCmd->buttons |= IN_FORWARD;
			pCmd->buttons &= ~IN_BACK;
		}

		if (pCmd->sidemove < 0)
		{
			pCmd->buttons |= IN_MOVERIGHT;
			pCmd->buttons &= ~IN_MOVELEFT;
		}

		if (pCmd->sidemove > 0)
		{
			pCmd->buttons |= IN_MOVELEFT;
			pCmd->buttons &= ~IN_MOVERIGHT;
		}
	}

	void DoKnifeLeftSide()
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
			return;

		if (!g_Menu.Config.KnifeLeftSide)
			return;

		ConVar* cl_righthand = g_pCvar->FindVar("cl_righthand");

		if (g::pLocalEntity->IsKnifeorNade() && !g::pLocalEntity->IsNade())
			cl_righthand->SetValue(0);
		else
			cl_righthand->SetValue(1);
	}

	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	void DoAutostrafe() const
	{
		if (!g_Menu.Config.AutoStrafe)
			return;

		if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
			return;

		if (g::pLocalEntity->IsAlive() && !(g::pLocalEntity->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(VK_SPACE))
		{
			pCmd->forwardmove = (10000.f / g::pLocalEntity->GetVelocity().Length2D() > 450.f) ? 450.f : 10000.f / g::pLocalEntity->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->mousedx != 0) ? (pCmd->mousedx < 0.0f) ? -450.f : 450.f : (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
		}
	}

	int lag_comp_break(bool mode) const
	{
		Vector velocity = g::pLocalEntity->GetVelocity(); velocity.z = 0;
		float speed = velocity.Length();

		if (speed > 0.f) {
			auto distance_per_tick = speed * g_pGlobalVars->intervalPerTick;
			int choked_ticks = std::ceilf(65.f / distance_per_tick);
			return mode ? std::min<int>(choked_ticks, g_Menu.Config.FakeLagOnGround) : std::min<int>(choked_ticks, g_Menu.Config.FakeLagInAir);

		}
		return 1;
	}

	void DoFakeLag() const
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
			return;
		if (!g::pLocalEntity->IsAlive())
			return;
		if (g::pLocalEntity->IsNade())
			return;
		if (g::IsFakewalking || g::isfakeducking)
			return;
		if (g_pEngine->IsVoiceRecording())
			return;

		int fakelagVal;

		if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
			fakelagVal = g_Menu.Config.BreakLagComp ? lag_comp_break(true) : g_Menu.Config.FakeLagOnGround;
		else
			fakelagVal = g_Menu.Config.BreakLagComp ? lag_comp_break(false) : g_Menu.Config.FakeLagInAir;

		auto NetChannel = g_pEngine->GetNetChannel();

		if (!NetChannel)
			return;

		static float maxSpeed = 320.f;
		static float Acceleration = 5.5f;
		float maxAccelspeed = Acceleration * maxSpeed * g_pGlobalVars->intervalPerTick;

		float TicksToStop = g::pLocalEntity->GetVelocity().Length() / maxAccelspeed;

		bool canHit = false;
		static bool init = false;
		static bool stop = true;
		static bool stop2 = true;
		bool skip = false;

		if (g_Menu.Config.FakeLagOnPeek)
		{
			for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
			{
				C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

				if (!pPlayerEntity
					|| !pPlayerEntity->IsAlive()
					|| pPlayerEntity->IsDormant()
					|| pPlayerEntity == g::pLocalEntity
					|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
					continue;

				Vector EnemyHead = { pPlayerEntity->GetOrigin().x, pPlayerEntity->GetOrigin().y, (pPlayerEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };

				Vector Head = { g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y, (g::pLocalEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };
				Vector HeadExtr = (Head + (g::pLocalEntity->GetVelocity() * 0.203125f));
				Vector OriginExtr = ((g::pLocalEntity->GetOrigin() + (g::pLocalEntity->GetVelocity() * 0.21875f)) + Vector(0, 0, 8));

				float dmg;

				if (abs(g::pLocalEntity->GetVelocity().Length2D()) > 50.f && (g_Autowall.CanHitFloatingPoint(HeadExtr, EnemyHead, g::pLocalEntity) || g_Autowall.CanHitFloatingPoint(OriginExtr, EnemyHead, g::pLocalEntity)))
				{
					canHit = true;
				}
			}

			if (canHit)
			{
				if (stop2)
				{
					init = true;
					stop2 = false;
				}
			}
			else
				stop2 = true;

			if (init)
			{
				if (!stop)
				{
					g::bSendPacket = true;
					g::LagPeek = true;
					stop = true;
					skip = true;
				}
			}

			if (!skip)
			{
				if (g::LagPeek)
				{
					if (NetChannel->m_nChokedPackets < g_Menu.Config.fakelagonpeek)
						g::bSendPacket = false;
					else
						g::LagPeek = false;
				}
				else
				{
					g::bSendPacket = (NetChannel->m_nChokedPackets >= fakelagVal);
					stop = false;
					init = false;
				}
			}
		}
		else
		{
			init = false;
			stop = true;
			stop2 = true;
			g::LagPeek = false;

			g::bSendPacket = (NetChannel->m_nChokedPackets >= fakelagVal);
		}
	}

	void LinearExtrapolations()
	{
		if (g_Menu.Config.Aimbot && g_Menu.Config.FakeLagPrediction)
		{
			auto m_local = g::pLocalEntity;
			if (m_local && m_local->IsAlive()) {
				for (int i = 1; i < g_pEngine->GetMaxClients(); i++)
				{
					auto m_entity = g_pEntityList->GetClientEntity(i);
					if (m_entity) {

						float simtime_delta = m_entity->GetSimulationTime() - m_entity->GetOldSimulationTime();
						int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
						Vector lastOrig;

						if (lastOrig.Length() != m_entity->GetOrigin().Length())
							lastOrig = m_entity->GetOrigin();

						float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
						if (delta_distance > 4096.f)
						{
							Vector velocity_per_tick = m_entity->GetVelocity() * g_pGlobalVars->intervalPerTick;
							auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
							m_entity->SetAbsOrigin(new_origin);
						}
					}

				}
			}
		}
	}

	void DoSlowWalk()
	{
		g::IsSlowwalking = false;
		auto velocity = g::pLocalEntity->GetVelocity().Length2D();
		auto weapon = g::pLocalEntity->GetActiveWeapon(); 	if (!weapon) return;
		float min_speed = (float)(FastSqrt(Square(g::pCmd->forwardmove) + Square(g::pCmd->sidemove) + Square(g::pCmd->upmove)));
		float kys = g_Menu.Config.SlowWalkAmount / min_speed;
		if (g_Menu.Config.SlowWalk) {
			switch (g_Menu.Config.SlowWalkType)
			{
			case 0:
				if (velocity >= g_Menu.Config.HitchanceValue)  // we need someting that checks what weapon we are using and get the hitchance from the menu
				{
					if (GetAsyncKeyState(VK_LSHIFT))
					{
						g::pCmd->buttons |= IN_WALK;
						g::pCmd->forwardmove = -g::pCmd->forwardmove;
						g::pCmd->sidemove = -g::pCmd->sidemove;
						g::pCmd->upmove = 0;
						g::IsSlowwalking = true;
						if (g_Menu.Config.StopLegAnim)
						{
							g::pLocalEntity->AnimState()->m_flUnknownFraction = 0.f;
							//g::pLocalEntity->SetAbsVelocity(Vector(0, 0, 0));
						}
					}
				}
				break;
			case 1:
				if (velocity >= (weapon->GetCSWpnData()->max_speed * .34))
				{
					if (GetAsyncKeyState(VK_LSHIFT))
					{
						g::pCmd->buttons |= IN_WALK;
						g::pCmd->forwardmove = -g::pCmd->forwardmove;
						g::pCmd->sidemove = -g::pCmd->sidemove;
						g::pCmd->upmove = 0;
						g::IsSlowwalking = true;
						if (g_Menu.Config.StopLegAnim)
						{
							g::pLocalEntity->AnimState()->m_flUnknownFraction = 0.f;
							//g::pLocalEntity->SetAbsVelocity(Vector(0, 0, 0));
						}
					}
				}
				break;
			case 2:
				if (GetAsyncKeyState(VK_LSHIFT))
				{

					g::pCmd->forwardmove *= kys;
					g::pCmd->sidemove *= kys;
					g::pCmd->upmove *= kys;
					g::IsSlowwalking = true;
					if (g_Menu.Config.StopLegAnim)
					{
						g::pLocalEntity->AnimState()->m_flUnknownFraction = 0.f;
						//g::pLocalEntity->SetAbsVelocity(Vector(0, 0, 0));
					}
				}
				break;
			}
		}
	}

	void DoDisablePostProcess()
	{
		ConVar * post_process = g_pCvar->FindVar("mat_postprocess_enable");
        *(int*)((DWORD)& post_process->fnChangeCallback) = 1;
        if (!g_Menu.Config.DisablePostProcess)
			post_process->SetValue(1);
        else
			post_process->SetValue(0);
	}

	void AntiClientCrasher()
	{
		if (!g_Menu.Config.AntiClientCrasher)
			return;

		ConVar* cl_invites_only_friends = g_pCvar->FindVar("cl_invites_only_friends");
		ConVar* cl_invites_only_mainmenu = g_pCvar->FindVar("cl_invites_only_mainmenu");

		if (cl_invites_only_friends->GetInt() != g_Menu.Config.AntiClientCrasher || cl_invites_only_mainmenu->GetInt() != g_Menu.Config.AntiClientCrasher)
		{
			cl_invites_only_friends->SetValue(g_Menu.Config.AntiClientCrasher);
			cl_invites_only_mainmenu->SetValue(g_Menu.Config.AntiClientCrasher);
		}
	}
};
extern Misc g_Misc;