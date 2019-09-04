#include "LegitAntiAim.h"
#include "AntiAim.h"
#include "..\..\Menu\Menu.h"

LegitAntiAim g_LegitAntiAim;

float updateTime2;
float lastUpdate2;
float wasmoving2;
bool performBreak2;
bool m_bBreakBalance2 = false;

void LbyBreaker2() {
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();
	float MaxDelta = g_AntiAim.MaxDelta(g::pLocalEntity);

	if (!performBreak2) {
		if (Velocity > 0.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
			lastUpdate2 = g_pGlobalVars->curtime;
			wasmoving2 = true;
			m_bBreakBalance2 = true;
		}
		else {
			if (wasmoving2 && g_pGlobalVars->curtime - lastUpdate2 > 0.22f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				wasmoving2 = false;
				performBreak2 = true;
				m_bBreakBalance2 = false;
			}
			else if (g_pGlobalVars->curtime - lastUpdate2 > 1.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				performBreak2 = true;
				m_bBreakBalance2 = false;
			}
		}
	}
}

void LegitAntiAim::Run()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
		return;

	if (!g_Menu.Config.LegitAA)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (true)
		return;

	Vector view;
	g_pEngine->GetViewAngles(view);

	LbyBreaker2();

	if (performBreak2)
	{
		g::bSendPacket = false;
		g::pCmd->viewangles += g_Menu.Config.FakeSide ? 120.f : -120.f;
		lastUpdate2 = g_pGlobalVars->curtime;
		performBreak2 = false;
		return;
	}

	if ((g::pCmd->command_number % 2) == 1)
	{
		if (g::bSendPacket)
		{
			g::pCmd->viewangles.y = view.y + g_Menu.Config.FakeSide ? g_AntiAim.MaxDelta(g::pLocalEntity) : -g_AntiAim.MaxDelta(g::pLocalEntity);
			g::bSendPacket = false;
		}
		else
		{
			g::pCmd->viewangles = view;
			g::bSendPacket = true;
		}
	}
}