#include "x88Menu.h"
#include <sstream>

x88Menu g_x88Menu;

void x88Menu::Draw()
{
	if (!g_Menu.Config.x88menu)
		return;

	ConVar* HUDSCALE = g_pCvar->FindVar("hud_scaling");
	float hudmult = HUDSCALE->GetFloat();

	Color Text(255, 255, 255);
	Color ON(0, 130, 200);
	Color WARN(230, 30, 30);
	Color OFF(255, 255, 255);
	Color SECON(30, 200, 60);
	Color Watermark(230, 150, 160);

	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	int OffsetX = (round(iWidth / 100) * 19.27) * hudmult;
	int OffsetX2 = OffsetX + 140;
	int OffsetY = 40;
	int NewLineOffset = 16;
	int NewLineCount = 1;

	std::string textbuffer;
	Color buffercolor(255, 255, 255);

	if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
	{
		textbuffer = "Hello User :)";
	}
	else
	{
		PlayerInfo_t pInfo;
		g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &pInfo);
		std::string name = pInfo.szName;
		textbuffer = "Hello " + name + " :)";
	}
	g_pSurface->DrawT(OffsetX, 10, Color(255, 255, 0), g::x88Font, false, textbuffer.c_str());

	g_pSurface->DrawT(10, OffsetY, Watermark, g::x88Font, false, "t4zCheats");

	if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;

	const C_BaseEntity* address = static_cast<const C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));
	std::stringstream ss;
	ss << address;
	std::string localptr = ss.str();
	textbuffer = "LocalPlayer " + localptr;
	g_pSurface->DrawT(OffsetX, 26, Color(255, 255, 0), g::x88Font, false, textbuffer.c_str());

	if (g_Menu.Config.Antiaim)
	{
		std::string shit1 = std::to_string(roundf(g::FakeAngle.y * 100) / 100);
		textbuffer = "Fake: " + shit1.erase(shit1.length() - 4);
		buffercolor = Color(255, 255, 0);
		g_pSurface->DrawT(OffsetX + 110, 42, buffercolor, g::x88Font, false, textbuffer.c_str());

		std::string shit2 = std::to_string(roundf(g::RealAngle.y * 100) / 100);
		textbuffer = "Real: " + shit2.erase(shit2.length() - 4);
		buffercolor = Color(255, 255, 0);
		g_pSurface->DrawT(OffsetX + 210, 42, buffercolor, g::x88Font, false, textbuffer.c_str());

		int diff = fabs(g::FakeAngle.y - g::RealAngle.y);
		textbuffer = "Diff: " + std::to_string(diff);
		buffercolor = diff > 32.f ? SECON : WARN;
		g_pSurface->DrawT(OffsetX + 310, 42, buffercolor, g::x88Font, false, textbuffer.c_str());
	}

	textbuffer = "Aimbot:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	textbuffer = g_Menu.Config.OnKeyAim ? "ON KEY" : g_Menu.Config.Aimbot ? "ON" : g_Menu.Config.LegitAimbot ? "Legit" : "OFF";
	buffercolor = g_Menu.Config.OnKeyAim ? ON : g_Menu.Config.Aimbot ? ON : g_Menu.Config.LegitAimbot ? SECON : OFF;
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	textbuffer = "Backtrack:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	textbuffer = g_Menu.Config.LegitBacktrack ? "Legit" : g_Menu.Config.PosBacktrack ? "ON" : "OFF";
	buffercolor = g_Menu.Config.LegitBacktrack ? SECON : g_Menu.Config.PosBacktrack ? ON : OFF;
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	textbuffer = "Chams:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	textbuffer = g_Menu.Config.EnemyChams ? (g_Menu.Config.EnemyXQZChams ? "FULL" : "ON") : "OFF";
	buffercolor = g_Menu.Config.EnemyChams ? (g_Menu.Config.EnemyXQZChams ? WARN : ON) : OFF;
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	textbuffer = "ESP:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	if (g_Menu.Config.Esp == 1) {
		textbuffer = "ON";
		buffercolor = ON;
	}
	else if (g_Menu.Config.Esp == 2) {
		textbuffer = "Dead";
		buffercolor = SECON;
	}
	else {
		textbuffer = "OFF";
		buffercolor = OFF;
	}
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	textbuffer = "AA:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	textbuffer = g_Menu.Config.Antiaim ? "WARNING: ON" : "OFF";
	buffercolor = g_Menu.Config.Antiaim ? WARN : OFF;
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	textbuffer = "Desync:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	textbuffer = g_Menu.Config.DesyncAngle ? "ON" : "OFF";
	buffercolor = g_Menu.Config.DesyncAngle ? ON : OFF;
	g_pSurface->DrawT(OffsetX + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;

	////// NEW LINE  //////

	NewLineCount = 1;

	textbuffer = "BT Chams:";
	buffercolor = Text;
	g_pSurface->DrawT(OffsetX2, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	if (g_Menu.Config.BacktrackingChams == 0)
	{
		textbuffer = "OFF";
		buffercolor = OFF;
	}
	else if (g_Menu.Config.BacktrackingChams == 1)
	{
		textbuffer = "P->C";
		buffercolor = ON;
	}
	else if (g_Menu.Config.BacktrackingChams == 2)
	{
		textbuffer = "W->B";
		buffercolor = ON;
	}
	else if (g_Menu.Config.BacktrackingChams == 3)
	{
		textbuffer = "C->T";
		buffercolor = ON;
	}
	g_pSurface->DrawT(OffsetX2 + 110, OffsetY + (NewLineOffset * NewLineCount), buffercolor, g::x88Font, false, textbuffer.c_str());
	NewLineCount++;
}