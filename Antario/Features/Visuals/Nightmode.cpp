#include "Nightmode.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Hooks.h"

Nightmode g_Nightmode;

void Nightmode::Run()
{
	static std::string OldSkyname = "";
	static bool OldNightmode;
	static int OldSky;
	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame()) {
		OldSkyname = "";
		OldNightmode = false;
		OldSky = 0;
		return;
	}
	static ConVar*r_DrawSpecificStaticProp;
	static ConVar*mat_ambient_light_r;
	static ConVar*mat_ambient_light_g;
	static ConVar*mat_ambient_light_b;

	if (OldNightmode != g_Menu.Config.Nightmode)
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_pCvar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->SetValue(0);
		for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
			if (!pMaterial)
				continue;
			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (g_Menu.Config.Nightmode)
					if (!strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
						pMaterial->ColorModulate(g_Menu.Config.WorldColor.red / 255.f, g_Menu.Config.WorldColor.green / 255.f, g_Menu.Config.WorldColor.blue / 255.f);
					else
						pMaterial->ColorModulate(g_Menu.Config.PropColor.red / 255.f, g_Menu.Config.PropColor.green / 255.f, g_Menu.Config.PropColor.blue / 255.f);
				else
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
			}
		}
		OldNightmode = g_Menu.Config.Nightmode;
	}

		ConVar* mat_force_tonemap_scale = g_pCvar->FindVar("mat_force_tonemap_scale");

		if (g_Menu.Config.ProperNightmode)
		{
			if (mat_force_tonemap_scale->GetFloat() != 0.2f)
				g_pEngine->ExecuteClientCmd("mat_force_tonemap_scale 0.2");

			static float storedCurtime = g_pGlobalVars->curtime;

			double timeSoFar = abs(g_pGlobalVars->curtime - storedCurtime);

			static int choose = 1;

			srand(time(NULL));

			if (timeSoFar > 22.0)
			{
				switch (choose)
				{
				case 1:g_pEngine->ExecuteClientCmd("play ambient\\playonce\\weather\\thunder4.wav"); break;
				case 2:g_pEngine->ExecuteClientCmd("play ambient\\playonce\\weather\\thunder5.wav"); break;
				case 3:g_pEngine->ExecuteClientCmd("play ambient\\playonce\\weather\\thunder6.wav"); break;
				case 4:g_pEngine->ExecuteClientCmd("play ambient\\playonce\weather\\thunder_distant_01.wav"); break;
				case 5:g_pEngine->ExecuteClientCmd("play ambient\\playonce\\weather\\thunder_distant_02.wav"); break;
				case 6:g_pEngine->ExecuteClientCmd("play ambient\\playonce\\weather\\thunder_distant_06.wav"); break;
				}
				storedCurtime = g_pGlobalVars->curtime;
			}
			else
			{
				int randomnum = 1 + (rand() % 6);

				if (randomnum == choose)
					choose = 1 + (rand() % 6); // could still be the same number but less likely
				else
					choose = randomnum;
			}
		}
		else if (!g_Menu.Config.ProperNightmode && mat_force_tonemap_scale->GetFloat() != 1.f)
			g_pEngine->ExecuteClientCmd("mat_force_tonemap_scale 1");

	/*if (g_Menu.Config.AntiUntrusted)
		return;*/

	if (g_Menu.Config.AmbientLight)
	{
		if (!mat_ambient_light_r || !mat_ambient_light_g || !mat_ambient_light_b)
		{
			mat_ambient_light_r = g_pCvar->FindVar("mat_ambient_light_r");
			mat_ambient_light_g = g_pCvar->FindVar("mat_ambient_light_g");
			mat_ambient_light_b = g_pCvar->FindVar("mat_ambient_light_b");
		}

		mat_ambient_light_r->SetValue(g_Menu.Config.AmbientLightColor.red / 255.f);
		mat_ambient_light_g->SetValue(g_Menu.Config.AmbientLightColor.green / 255.f);
		mat_ambient_light_b->SetValue(g_Menu.Config.AmbientLightColor.blue / 255.f);
	}
}