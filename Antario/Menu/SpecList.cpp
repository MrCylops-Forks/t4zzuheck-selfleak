#include "SpecList.h"

SpecList g_SpecList;

Color list_background(33, 33, 33);
Color list_accent(222, 118, 0);

void SpecList::Draw()
{
	if (!g_Menu.Config.SpecList)
		return;

	if (g_pEngine->IsInGame() && g::pLocalEntity) {
		if (!g::pLocalEntity->IsAlive())
			return;

		int width, height;
		g_pEngine->GetScreenSize(width, height);

		int textPositionY = 10;

		for (int i = 1; i < g_pEngine->GetMaxClients(); i++) {
			C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);
			if (!entity || entity->IsAlive() || entity->IsDormant())
				continue;

			PlayerInfo_t pInfo;
			g_pEngine->GetPlayerInfo(i, &pInfo);

			if (entity->GetObserverTarget() == g::pLocalEntity) {
				RECT TextSize = g_pSurface->GetTextSizeRect(g::Tahoma, pInfo.szName);
				g_pSurface->DrawT(width - TextSize.right, textPositionY, entity->GetTeam() == g::pLocalEntity->GetTeam() ? Color(255, 255, 255) : Color(255, 190, 190), g::Tahoma, false, pInfo.szName);
				textPositionY += TextSize.bottom;
			}
		}
	}

	//if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
	//	return;

	//int ObserverCount = 0;

	//static Vector2D oldPos;
	//static Vector2D mousePos;
	//static bool Dragging = false;
	//bool click = false;
	//static int dragX = 0;
	//static int dragY = 0;

	//int Width = 400;

	//static int iWidth, iHeight;
	//g_pEngine->GetScreenSize(iWidth, iHeight);

	//if (GetAsyncKeyState(VK_LBUTTON))
	//	click = true;

	//Vector2D MousePos = g_pSurface->GetMousePosition();

	//if (Dragging && !click)
	//	Dragging = false;

	//if (Dragging && click)
	//{
	//	Pos.x = MousePos.x - dragX;
	//	Pos.y = MousePos.y - dragY;
	//}

	//if (g_pSurface->MouseInRegion(Pos.x, Pos.y, Width, 14))
	//{
	//	Dragging = true;
	//	dragX = MousePos.x - Pos.x;
	//	dragY = MousePos.y - Pos.y;
	//}

	//if (Pos.x < 0)
	//	Pos.x = 0;
	//if (Pos.y < 0)
	//	Pos.y = 0;
	//if ((Pos.x + Width) > iWidth)
	//	Pos.x = iWidth - Width;
	//if ((Pos.y + 14) > iHeight)
	//	Pos.y = iHeight - 14;

	//g_pSurface->FilledRect(Pos.x, Pos.y, Width, 14, list_background);
	//g_pSurface->OutlinedRect(Pos.x, Pos.y, Width, 14, list_accent);
	//g_pSurface->DrawT(Pos.x, Pos.y + 1, Color(255, 255, 255), g::Tahoma, false, "Spectators");

	//if (g::pLocalEntity && g_pEngine->IsInGame() && g_pEngine->IsConnected())
	//{
	//	if (!g::pLocalEntity->IsAlive())
	//		return;

	//	for (int i = 1; i < g_pEngine->GetMaxClients(); i++)
	//	{
	//		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
	//		if (!pEnt)
	//			continue;
	//		if (!pEnt->IsAlive())
	//			continue;
	//		if (pEnt == g::pLocalEntity)
	//			continue;
	//		if (pEnt->IsDormant())
	//			continue;
	//		/*if (pEnt->GetObserverTarget() != g::pLocalEntity)
	//			continue;*/
	//		PlayerInfo_t pInfo;
	//		g_pEngine->GetPlayerInfo(i, &pInfo);
	//		if (pInfo.ishltv)
	//			continue;
	//		g_pSurface->FilledRect(Pos.x, Pos.y + 14 + (ObserverCount * 12), Width, 12, Color(33, 33, 33, 125));
	//		g_pSurface->DrawT(Pos.x, Pos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, (char*)i);
	//		g_pSurface->DrawT(Pos.x + 25, Pos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, pInfo.szName);
	//		if (pEnt->GetTeam() == g::pLocalEntity->GetTeam())
	//			g_pSurface->DrawT(Pos.x + 125, Pos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, "TEAM");
	//		else
	//			g_pSurface->DrawT(Pos.x + 125, Pos.y + 14 + (ObserverCount * 12), Color(255, 255, 255), g::Tahoma, false, "ENEMY");
	//		ObserverCount++;
	//	}
	//}
}
/*
bool SpecList::IsObserving(C_BaseEntity* pEnt)
{
	return (pEnt->GetObserverTarget() == g::pLocalEntity);
}
*/