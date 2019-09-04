#include "FakeLag.h"

int CFakeLag::Fakelag_AdaptiveFactor()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return 0;
	int choked_ticks;
	auto velocity = local_player->GetVelocity();;
	auto speed = velocity.Length();
	auto distance_per_tick = speed *
		g_pGlobalVars->intervalPerTick;
	choked_ticks = std::ceilf(TELEPORT_DISTANCE / distance_per_tick);
	return std::min<int>(choked_ticks, MAX_CHOKE);
}
int iChoke = 1;

void CFakeLag::do_fakelag(CUserCmd* cmd, C_BaseEntity* local)
{
	if (g_pEngine->IsVoiceRecording())
		return;


	if (cmd->buttons & IN_ATTACK)
		return;

	auto NetChannel = g_pEngine->GetNetChannel();

	if (!NetChannel)
		return;


	bool flag1 = true;
	bool flag2 = true;
	bool flag3 = true;
	bool flag4 = true;

	int fakelagVal;

	if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
		fakelagVal = g_Menu.Config.FakeLagValue;
	else
		fakelagVal = g_Menu.Config.FakeLagValueGround;


     if (flag1 || flag2 || flag3 || flag4)
		iChoke = fakelagVal;
	else
		return;

	if (g_Menu.Config.FakeLagType == 1)
	{
		if (NetChannel->m_nChokedPackets < iChoke)
		{
		g::bSendPacket = false;
		}
		else
		{
			g::bSendPacket = true;
		}
	}
	else if (g_Menu.Config.FakeLagType == 2)
	{
		auto velocity = g::pLocalEntity->GetVelocity();;

		auto speed = velocity.Length();

		auto distance_per_tick = speed * g_pGlobalVars->intervalPerTick;

        if (velocity.Length2D() > 0)
			iChoke = std::min<int>(std::ceilf(TELEPORT_DISTANCE / distance_per_tick), fakelagVal);
		else
			iChoke = fakelagVal;

		if (NetChannel->m_nChokedPackets < iChoke)
		{
			g::bSendPacket = false;
		}
		else
		{
			g::bSendPacket = true;
		}
	}
	else if (g_Menu.Config.FakeLagType == 3)
	{
		static float next_update_time = 0;
		bool yo = false;
		if (next_update_time < g_pGlobalVars->curtime)
		{
			next_update_time = g_pGlobalVars->curtime + 1.1f;
			yo = true;
		}

		if (NetChannel->m_nChokedPackets < iChoke && yo)
		{
			g::bSendPacket = false;
		}
		else
		{
			g::bSendPacket = true;
		}
	}
}

CFakeLag* fakelag = new CFakeLag();