#include "Aimbot.h"
#include "Autowall.h"
#include "LagComp.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\Utils\Interfaces.h"
#include "..\..\SDK\CPrediction.h"

Aimbot g_Aimbot;

static C_BaseEntity* OldTarget = nullptr;

void Aimbot::Autostop()
{
	if (!g_Menu.Config.Autostop)
		return;

	Vector Velocity = g::pLocalEntity->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;

	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;
}



bool Aimbot::HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0)
		return true;

	float Seeds = 256.f;

	Angle -= (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(g::pLocalEntity->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;


	switch (g_Menu.Config.weaponmode)
	{
	case 0: //auto
		break;
	case 1: //rifle
		break;
	case 2: //scout
		break;
	case 3: //pistol
		break;
	case 4: // awp
		break;
	case 5: // smg
		break;
	case 6: // deagl
		break;
	}
}

bool ShouldBaim(C_BaseEntity* pEnt)
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}

	if (g_Menu.Config.BaimInAir && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (g_Menu.Config.BaimAfterXMissedShots > 0 && g::MissedShots[pEnt->EntIndex()] >= g_Menu.Config.BaimAfterXMissedShots)
		return true;

	if (g_Menu.Config.BaimAfterXShots > 0 && g::Shots[pEnt->EntIndex()] >= g_Menu.Config.BaimAfterXShots)
		return true;

	return false;
}

Vector Aimbot::Hitscan(C_BaseEntity* pEnt)
{
	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	float DamageArray[28];
	float tempDmg = 0.f;
	Vector tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };

	float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pEnt->GetOrigin()).y;

	Vector2D MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
	Vector2D MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
	Vector2D Mutipoint[] = { Vector2D(MutipointXY.x, MutipointXY.y), Vector2D(MutipointXY180.x, MutipointXY180.y) };

	float Velocity = abs(pEnt->GetVelocity().Length2D());

	std::vector<int> Scan;

	int HeadHeight = g_Menu.Config.HeadScale;

	bool Baim = ShouldBaim(pEnt);

	if (g_Menu.Config.ForceBaim)
		Baim = true;

	if (!Baim && g_Menu.Config.HB_Head)
	{
		Scan.push_back(HITBOX_HEAD);
	}

	if (Velocity <= 215.f || Baim)
	{

		if (g_Menu.Config.MultiPoint)
		{
			if (g_Menu.Config.HB_Pelvis)
			{
				Scan.push_back(HITBOX_PELVIS);
			}

			if (g_Menu.Config.HB_Thorax)
			{
				Scan.push_back(HITBOX_THORAX);
			}

			if (g_Menu.Config.HB_Chest)
			{
				Scan.push_back(HITBOX_LOWER_CHEST);
				Scan.push_back(HITBOX_UPPER_CHEST);
			}

			if (g_Menu.Config.HB_Feet)
			{
				Scan.push_back(HITBOX_LEFT_FOOT);
				Scan.push_back(HITBOX_RIGHT_FOOT);
			}

			if (g_Menu.Config.HB_Arms)
			{
				Scan.push_back(HITBOX_LEFT_UPPER_ARM);
				Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
			}

			if (g_Menu.Config.HB_Thighs)
			{
				Scan.push_back(HITBOX_LEFT_THIGH);
				Scan.push_back(HITBOX_RIGHT_THIGH);
			}

			if (g_Menu.Config.HB_Calf)
			{
				Scan.push_back(HITBOX_RIGHT_CALF);
				Scan.push_back(HITBOX_LEFT_CALF);
			}

			if (g_Menu.Config.HB_Belly)
			{
				Scan.push_back(HITBOX_BELLY);
			}

			if (g_Menu.Config.HB_Neck)
			{
				Scan.push_back(HITBOX_NECK);
			}
		}
	}

	Vector Hitbox;
	int bestHitboxint = 0;

	for (int hitbox : Scan)
	{
		if (hitbox < 19)
			Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);
		else if (hitbox > 18 && hitbox < 25)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (g_Menu.Config.BodyScale / 100.f);
			Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 19) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 19) % 2)].y), Hitbox.z);
		}
		else if (hitbox > 24 && hitbox < 28)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(0, Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (HeadHeight / 100.f);
			if (hitbox != 27)
				Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 25) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 25) % 2)].y), Hitbox.z);
			else
				Hitbox += Vector(0, 0, Radius);
		}

		float Damage = g_Autowall.Damage(Hitbox);

		if (Damage > 0.f)
			DamageArray[hitbox] = Damage;
		else
			DamageArray[hitbox] = 0;

		if (g_Menu.Config.BaimLethal && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->GetHealth() + 10))
		{
			DamageArray[hitbox] = 400;
			Baim = true;
		}

		if (DamageArray[hitbox] > tempDmg)
		{
			tempHitbox = Hitbox;
			bestHitboxint = hitbox;
			tempDmg = DamageArray[hitbox];
		}

		g::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
	}

	PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

	Backtrack[pEnt->EntIndex()] = false;
	ShotBacktrack[pEnt->EntIndex()] = false;

	if (tempDmg >= g_Menu.Config.Mindmg)
	{
		bestEntDmg = tempDmg;

		if ((bestHitboxint == 25 || bestHitboxint == 26 || bestHitboxint == 27) && abs(DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint]) <= 10.f)
			return pEnt->GetHitboxPosition(HITBOX_HEAD, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 19 || bestHitboxint == 20) && DamageArray[HITBOX_PELVIS] > 30)
			return pEnt->GetHitboxPosition(HITBOX_PELVIS, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 21 || bestHitboxint == 22) && DamageArray[HITBOX_THORAX] > 30)
			return pEnt->GetHitboxPosition(HITBOX_THORAX, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 23 || bestHitboxint == 24) && DamageArray[HITBOX_UPPER_CHEST] > 30)
			return pEnt->GetHitboxPosition(HITBOX_UPPER_CHEST, Matrix[pEnt->EntIndex()]);

		return tempHitbox;
	} // && pPlayerEntityRecord.Velocity >= 29.f
	else if (g_Menu.Config.PosBacktrack)
	{
		if (g_Menu.Config.LbyBacktrack)
		{
			int bestHeadEntDmg = 0;
			int bestLbyEntDmg = 0;

			if (g_Autowall.CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix), g::pLocalEntity->GetEyePosition(), g::pLocalEntity))
				bestHeadEntDmg = (100000.f - fabs(g_Math.Distance(Vector2D(g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y), Vector2D(pEnt->GetOrigin().x, pEnt->GetOrigin().y))));
			
			if (g_Autowall.CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_PELVIS, pPlayerEntityRecord.Matrix), g::pLocalEntity->GetEyePosition(), g::pLocalEntity))
				bestLbyEntDmg = (100000.f - fabs(g_Math.Distance(Vector2D(g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y), Vector2D(pEnt->GetOrigin().x, pEnt->GetOrigin().y))));

			Backtrack[pEnt->EntIndex()] = true;

			if (bestHeadEntDmg == 0 && bestLbyEntDmg == 0)
				return Vector(0,0,0);

			if (bestHeadEntDmg == 0 && bestLbyEntDmg != 0)
				return pEnt->GetHitboxPosition(HITBOX_PELVIS, pPlayerEntityRecord.Matrix);

			if (bestLbyEntDmg == 0 && bestHeadEntDmg != 0)
				return pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix);

			if (bestHeadEntDmg > bestLbyEntDmg)
				return pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix);
			else
				return pEnt->GetHitboxPosition(HITBOX_PELVIS, pPlayerEntityRecord.Matrix);
		}
		else
		{
			if (g_Autowall.CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix), g::pLocalEntity->GetEyePosition(), g::pLocalEntity))
			{
				bestEntDmg = (100000.f - fabs(g_Math.Distance(Vector2D(g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y), Vector2D(pEnt->GetOrigin().x, pEnt->GetOrigin().y))));
				Backtrack[pEnt->EntIndex()] = true;
				return pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix);
			}
		}
	}

	return Vector(0, 0, 0);
}

void AutoRevolver()
{
	if (!g_Menu.Config.AutoRevolver)
		return;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon || weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
		return;

	static float delay = 0.f;
	if (delay < 0.15f)
	{
		delay += g_pGlobalVars->intervalPerTick;
		g::pCmd->buttons |= IN_ATTACK;
	}
	else
		delay = 0.f;
}

void AutomaticWeapons()
{
	C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (pWeapon->GetAmmo() == 0)
		return;

	static bool WasFiring = false;
	WeaponInfo_t* WeaponInfo = pWeapon->GetCSWpnData();
	if (!WeaponInfo->full_auto)
	{
		if (g::pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				g::pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = g::pCmd->buttons & IN_ATTACK ? true : false;
	}
}

void Aimbot::OnCreateMove()
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;

	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;

	int targetID = 0;
	int tempDmg = 0;
	static bool shot = false;
	if (g_Menu.Config.AutoRevolver)
		AutoRevolver();

	if (g_Menu.Config.AutomaticWeapons && !g::pLocalEntity->IsKnifeorNade())
		AutomaticWeapons();

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			g_LagComp.ClearRecords(i);
			g::Shots[i] = 0;
			g::MissedShots[i] = 0;
			continue;
		}

		if (pPlayerEntity == g::pLocalEntity)
			continue;

		g_LagComp.StoreRecord(pPlayerEntity);

		if (!g_Menu.Config.FriendlyFire && pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		if (pPlayerEntity->IsImmune())
			continue;

		g::EnemyEyeAngs[i] = pPlayerEntity->GetEyeAngles();

		if (g_LagComp.PlayerRecord[i].size() == 0 || !g::pLocalEntity->IsAlive() || !g_Menu.Config.Aimbot)
			continue;

		bestEntDmg = 0;

		Vector Hitbox = Hitscan(pPlayerEntity);

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}
	}

	if (!g::pLocalEntity->IsAlive())
	{
		shot = false;
		return;
	}

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade() || g::pLocalEntity->GetActiveWeapon()->GetAmmo() == 0)
	{
		shot = false;
		return;
	}

	if (g::FreezeTime)
	{
		shot = false;
		return;
	}

	if (shot)
	{
		if (g_Menu.Config.OnShotAA == 0)
		{
			g::bSendPacket = true;
			g_AntiAim.OnCreateMove();
			g::pLocalEntity->SetTickBase(g::pLocalEntity->GetTickBase() - 6);
		}
		else
		{
			g::bSendPacket = true;
		}

		/*if (g_Menu.Config.OnShotAA && g_Menu.Config.Antiaim) // gotta improve this a bit more, tickbase shifting doesnt work here 4 some reason // https://www.unknowncheats.me/forum/counterstrike-global-offensive/252632-tickbase-manipulation-instant-defuse-etc.html 
		{                                                                                                                                          //wonder fucking why
			g::bSendPacket = true;
			g_AntiAim.OnCreateMove();
			g::pLocalEntity->SetTickBase(g::pLocalEntity->GetTickBase() - 6);
		}*/
		shot = false;
	}

	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (g::pLocalEntity->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime);

	if (Target)
	{
		g::TargetIndex = targetID;

		float SimulationTime = 0.f;

		if (Backtrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(0).SimTime;
		else
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

		Vector Angle = g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Aimpoint);

		Vector view;
		g_pEngine->GetViewAngles(view);

		static int MinimumVelocity = 0;

		if (g_Menu.Config.AimbotFov > 0 && !(g_Math.GetFov(view, Angle) < g_Menu.Config.AimbotFov))
			return;

		if (g_Menu.Config.AutoScope) // so it doesnt not scope anymore
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)g::pLocalEntity->GetActiveWeapon();
			if (pWeapon)
			{
				std::string WeaponName = pWeapon->GetName();
				if (pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08 ||
					pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP ||
					pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1 ||
					pWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20)
				{
					if (!g::pLocalEntity->IsScoped())
					{
						g::pCmd->buttons |= IN_ATTACK2;
					}
				}
			}
		}

		C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
		if (weapon && weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_TASER)
		{
			if (g::pLocalEntity->IsKnifeorNade())
				return;

			if (!g_Menu.Config.AutoStopMode == 0)
			{
				MinimumVelocity = g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f;
			}
			else if (!g_Menu.Config.AutoStopMode == 1)
			{
				MinimumVelocity = 0;
			}
			else if (!g_Menu.Config.AutoStopMode == 2)
			{
				MinimumVelocity = g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt / g_Menu.Config.customstop * 0.1;
			}

			bool shouldstop = !g_Menu.Config.StopInBetweenShots ? true : canShoot;

			if (g::pLocalEntity->GetVelocity().Length() >= MinimumVelocity && shouldstop && !GetAsyncKeyState(VK_SPACE))
				Autostop();
		}

		if (!(g::pCmd->buttons & IN_ATTACK) && canShoot && HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, g_Menu.Config.HitchanceValue))
		{
			if (!Backtrack[targetID])
				g::Shot[targetID] = true;

			shot = true;
			if (g_Menu.Config.OnShotAA == 1)
			{
				g_AntiAim.OnCreateMove();
				g::bSendPacket = false;
			}
			else
			{
				g::bSendPacket = true;
			}
			g::Shots[Target->EntIndex()] += 1;

			g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());
			
			if (!g_Menu.Config.SilentAim)
			{
				if (g_Menu.Config.AimShowRecoil)
					g_pEngine->SetViewAngles(Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat()));
				else
					g_pEngine->SetViewAngles(Angle);
			}
			g::pCmd->buttons |= IN_ATTACK;
			g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.LerpTime());
		}
	}
}