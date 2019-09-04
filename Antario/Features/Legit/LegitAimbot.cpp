#include "LegitAimbot.h"
#include "../Aimbot/Aimbot.h"
#include "../../Menu/Menu.h"
#include "../../Menu/config.h"
#include "../Visuals/ESP.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Hotkeys.h"
#include "..\Aimbot\LagComp.h"

float m_fbestfov = 20.0f;
int m_ibesttargetlegit = -1;

float deltaTime;
float curAimTime;

LegitAimbot g_LegitAimbot;

void BackTrack::Update(int tick_count)
{
	if (!g_Menu.Config.FakeLagOnGround && !g_Menu.Config.FakeLagInAir)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{
	int delta = latest_tick - tick;
	float deltaTime = delta * g_pGlobalVars->intervalPerTick;
	return (fabs(deltaTime) <= 0.2f);
}

mstudiobbox_t* get_hitboxv3(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}

Vector GetHitboxPositionv3(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitboxv3(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}


void BackTrack::UpdateRecord(int i)
{
	C_BaseEntity* pEntity = g_pEntityList->GetClientEntity(i);
	if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
	{
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = GetHitboxPositionv3(pEntity, 0);
			records[i].Angles = pEntity->GetEyeAngles();
			records[i].Origin = pEntity->GetOrigin();
		}
	}
	else
	{
		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CUserCmd * cmd, Vector & aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f * static_cast<float>(M_PI));

	return Vector(cp * cy, cp * sy, -sp);
}

float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}


void BackTrack::LegitBackTrack(CUserCmd * cmd, C_BaseEntity * pLocal)
{
	if (g_Menu.Config.LegitBacktrack)
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		PlayerInfo_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 1; i < g_pEngine->GetMaxClients(); i++)
		{
			auto entity = (C_BaseEntity*)g_pEntityList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!g_pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == pLocal->GetTeam())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPositionv3(entity, 0);

				HeadPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; t++)
			{
				float tempFOVDistance = distance_point_to_line(HeadPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && HeadPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = HeadPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime + g_LagComp.LerpTime());
			}
		}
	}
}

BackTrack* Backtracking = new BackTrack();
backtrackData HeadPositions[64][12];

void LegitAimbot::Aim_CreateMove(CUserCmd * cmd)
{
	DropTarget();

	static float oldServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	float serverTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	deltaTime = serverTime - oldServerTime;
	oldServerTime = serverTime;

	if (!g::pLocalEntity->GetActiveWeapon())
		return;

	if (g::pLocalEntity->GetActiveWeapon()->IsKnifeorNade() || g::pLocalEntity->GetActiveWeapon()->GetAmmo() == 0)
		return;

	if (g_Menu.Config.RecoilControlSystem)
		RCS();

	if (!g_Menu.Config.LegitAimbot)
		return;

	if ((!(g::pCmd->buttons & IN_ATTACK) || m_ibesttargetlegit == -1))
		FindTarget();

	if (m_ibesttargetlegit != -1 && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.LegitAimKey)))
		GoToTarget();
}

void LegitAimbot::FindTarget()
{
	m_fbestfov = g_Menu.Config.LegitAimFov;

	for (int i = 1; i < g_pEngine->GetMaxClients(); i++)
	{
		auto Entity = g_pEntityList->GetClientEntity(i);

		if (!Entity->is_valid(g::pLocalEntity))
			continue;

		int hitbox;

		if (g_Menu.Config.LegitAimHitbox == 0)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			hitbox = 8;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(hitbox)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
		else if (g_Menu.Config.LegitAimHitbox == 1)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			hitbox = 6;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(hitbox)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
		else if (g_Menu.Config.LegitAimHitbox == 2)
		{
			Vector viewangles = g::pCmd->viewangles + g::pLocalEntity->GetAimPunchAngle() * 2.f;
			hitbox = 0;
			float fov = g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(hitbox)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_ibesttargetlegit = i;
			}
		}
	}
}

void LegitAimbot::GoToTarget()
{
	auto Entity = g_pEntityList->GetClientEntity(m_ibesttargetlegit);

	if (!Entity)
		return;

	int hitbox;

	switch (g_Menu.Config.LegitAimHitbox)
	{
	case 0: //head
		hitbox = 8;
		break;
	case 1: //chest
		hitbox = HITBOX_UPPER_CHEST;
		break;
	case 2: //pelvis
		hitbox = 0;
		break;
	default:
		break;
	}

	Vector predicted = Entity->GetPredicted(Entity->GetBonePosition(hitbox));

	Vector dst = g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), predicted);
	Vector src = g::pCmd->viewangles;

	dst.x -= g::pLocalEntity->GetAimPunchAngle().x * (g_Menu.Config.RCS_X / 50.f);
	dst.y -= g::pLocalEntity->GetAimPunchAngle().y * (g_Menu.Config.RCS_Y / 50.f);

	Vector delta = dst - src;

	delta.Clamp();

	if (!delta.IsZero())
	{
		float finalTime = delta.Length() / (g_Menu.Config.AimSpeed / 5);

		curAimTime += deltaTime;

		if (curAimTime > finalTime)
			curAimTime = finalTime;

		float percent = curAimTime / finalTime;

		delta *= percent;
		dst = src + delta;
	}

	g_pEngine->SetViewAngles(dst.Clamp());
}

void LegitAimbot::DropTarget()
{
	if (!EntityIsValid(m_ibesttargetlegit))
	{
		m_ibesttargetlegit = -1;
		curAimTime = 0.f;
	}
}

bool LegitAimbot::EntityIsValid(int i)
{
	auto Entity = g_pEntityList->GetClientEntity(i);

	if (!Entity->is_valid())
		return false;

	int hitbox;

	switch (g_Menu.Config.LegitAimHitbox)
	{
	case 0: //head
		hitbox = 8;
		break;
	case 1: //chest
		hitbox = HITBOX_UPPER_CHEST;
		break;
	case 2: //pelvis
		hitbox = 0;
		break;
	default:
		break;
	}

	if (g_Math.GetFov(g::pCmd->viewangles, g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Entity->GetBonePosition(hitbox))) > g_Menu.Config.LegitAimFov * 0.1)
		return false;

	return true;
}

Vector m_oldangle;

void LegitAimbot::RCS()
{
	auto weap = g::pLocalEntity->GetActiveWeapon();
	if (weap->IsPistol() || weap->IsSniper() || weap->IsShotgun())
		return;

	if (g::pCmd->buttons & IN_ATTACK)
	{
		Vector aimpunch = g::pLocalEntity->GetAimPunchAngle() * 2.f;

		g::pCmd->viewangles += (m_oldangle - aimpunch);

		m_oldangle = aimpunch;
	}
	else
	{
		m_oldangle.x = m_oldangle.y = m_oldangle.z = 0;
	}
}