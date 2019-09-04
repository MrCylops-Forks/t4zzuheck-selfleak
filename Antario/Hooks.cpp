#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Features\Features.h"
#include "Menu\Menu.h"
#include "SDK\Hitboxes.h"
#include "libloaderapi.h"
#include "SDK/IMaterial.h"
#include "SDK/ICvar.h"
#include "SDK/CGlowObjectManager.h"
#include "Utils/Math.h"
#include "Utils\hitsounds.h"
#include "Features/Knife/skinchanger.h"
#include "Features/Knife/glovechanger.h"
#include "SDK/CHudChat.h"
#include "Utils\xor.h"
#include "Utils\Interfaces.h"
#include "Features/Knife/Knife.h"
#include "Features/Visuals/grenadetracer.h"
#include "Features/Visuals/grenadeparth.h"
#include "Menu/SpecList.h"
#include "Features/AntiAim/LegitAntiAim.h"
#include "Features/AntiAim/FakeLag.h"

Misc     g_Misc;
Hooks    g_Hooks;
Event    g_Event;

struct BulletImpact_t
{
	float			flImpactTime;
	Vector			vecImpactPos;
	Color			color;
	C_BaseEntity* pPlayer;

	__forceinline BulletImpact_t()
	{
		vecImpactPos = { 0.0f, 0.0f, 0.0f };
		flImpactTime = 0.0f;
		color = Color(255, 255, 255, 255);
		pPlayer = nullptr;
	}

	__forceinline BulletImpact_t(C_BaseEntity* player, Vector pos, float time, Color col = Color(255, 255, 255, 255))
	{
		pPlayer = player;
		flImpactTime = time;
		vecImpactPos = pos;
		color = col;
	}
};

std::deque<BulletImpact_t> Impacts;
std::vector<trace_info> trace_logs;

void bulletTrace()
{
	if (!g_pEngine->IsInGame() || !g::pLocalEntity)
		return;

	if (g_Menu.Config.BulletTracers)
	{
		for (unsigned int i = 0; i < trace_logs.size(); i++)
		{
			auto current = trace_logs.at(i);

			auto* shooter = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(trace_logs[i].userid));


			bool is_local_player = shooter == g::pLocalEntity;
			bool is_teammate = g::pLocalEntity->GetTeam() == shooter->GetTeam() && !is_local_player;


			Color bullet_tracer_color;

			if (is_local_player && g_Menu.Config.monkey) // if the shooter is local player
			{

				Color localclr(g_Menu.Config.BulletTracerLocalColor.red, g_Menu.Config.BulletTracerLocalColor.green, g_Menu.Config.BulletTracerLocalColor.blue);

				if (g_Menu.Config.Impacts)
				{
					if (g_Menu.Config.ImpactType == 0)
					{
		
						g_pDebugOverlay->AddBoxOverlay(trace_logs[i].position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), localclr.red, localclr.green, localclr.blue, 127, 2);
					}
					if (g_Menu.Config.ImpactType == 1)
					{
						g_pDebugOverlay->AddSphereOverlay(trace_logs[i].position, 5, 5, 5, localclr.red, localclr.green, localclr.blue, 127, 2);
					}
				}

				g_pDebugOverlay->AddLineOverlay(trace_logs[i].start, trace_logs[i].position, localclr.red, localclr.green, localclr.blue, true, 2);

			}
			else if (!is_teammate && g_Menu.Config.monkey2)
			{
				Color enemyclr(g_Menu.Config.BulletTracerEnemyColor.red, g_Menu.Config.BulletTracerEnemyColor.green, g_Menu.Config.BulletTracerEnemyColor.blue);

				if (g_Menu.Config.Impacts)
				{
					if (g_Menu.Config.ImpactType == 0)
					{
						g_pDebugOverlay->AddBoxOverlay(trace_logs[i].position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), enemyclr.red, enemyclr.green, enemyclr.blue, 127, 2);
					}
					if (g_Menu.Config.ImpactType == 1)
					{
						g_pDebugOverlay->AddSphereOverlay(trace_logs[i].position, 5, 5, 5, enemyclr.red, enemyclr.green, enemyclr.blue, 127, 2);
					}
				}

				g_pDebugOverlay->AddLineOverlay(trace_logs[i].start, trace_logs[i].position, enemyclr.red, enemyclr.green, enemyclr.blue, true, 2);
			}

			trace_logs.erase(trace_logs.begin() + i);
		}
	}
}

struct HitMarker_t
{
	float expireTime;
	int damage;
};

std::vector<HitMarker_t> HitMarkers;

void Hooks::Init()
{
    // Get window handle
    while (!(g_Hooks.hCSGOWindow = FindWindowA("Valve001", nullptr)))
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(50ms);
    }

    interfaces::Init();                         // Get interfaces
    g_pNetvars = std::make_unique<NetvarTree>();// Get netvars after getting interfaces as we use them

    if (g_Hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
        g_Hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC,
                                                                              reinterpret_cast<LONG_PTR>(g_Hooks.WndProc)));

    // VMTHooks
	g_Hooks.pClientHook     = std::make_unique<VMTHook>(g_pClientDll);
    g_Hooks.pClientModeHook = std::make_unique<VMTHook>(g_pClientMode);
    g_Hooks.pSurfaceHook	= std::make_unique<VMTHook>(g_pSurface);
	g_Hooks.pPanelHook		= std::make_unique<VMTHook>(g_pPanel);
	g_Hooks.pModelHook      = std::make_unique<VMTHook>(g_pModelRender);
	g_Hooks.pRenderViewHook = std::make_unique<VMTHook>(g_pRenderView);

    // Hook the table functions
	g_Hooks.pClientHook    ->Hook(vtable_indexes::frameStage, Hooks::FrameStageNotify);
    g_Hooks.pClientModeHook->Hook(vtable_indexes::createMove, Hooks::CreateMove);
	g_Hooks.pClientModeHook->Hook(vtable_indexes::view, Hooks::OverrideView);
	g_Hooks.pClientModeHook->Hook(vtable_indexes::viewmodelFov, Hooks::GetViewmodelFov);
    g_Hooks.pSurfaceHook   ->Hook(vtable_indexes::lockCursor, Hooks::LockCursor);
	g_Hooks.pPanelHook	   ->Hook(vtable_indexes::paint, Hooks::PaintTraverse);
	g_Hooks.pModelHook	   ->Hook(vtable_indexes::dme, Hooks::DrawModelExecute);
	g_Hooks.pRenderViewHook->Hook(vtable_indexes::sceneEnd, Hooks::SceneEnd);

	g_Event.Init();

	g::Tahoma = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Tahoma, "Tahoma", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);

	g::TahomaSmall = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::TahomaSmall, "Tahoma", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);

	g::EspTahoma = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::EspTahoma, "Tahoma", 12, 520, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::EspSmallText = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::EspSmallText, "Small Fonts", 10, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::WeaponIcons = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::WeaponIcons, "Counter-Strike", 24, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::SansSerif = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::SansSerif, "MS Sans Serif", 12, 500, 0, 0, FONTFLAG_NONE | FONTFLAG_DROPSHADOW);

	g::SansSerifBold = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::SansSerifBold, "MS Sans Serif", 12, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::Indicators = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Indicators, "Verdana", 30, 10000, 0 , 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::Watermark = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Watermark, "Verdana", 50, 10000, 0, 0, FONTFLAG_ANTIALIAS);

	g::KillCounter = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::KillCounter, "Tahoma", 24, 700, 0, 0, FONTFLAG_DROPSHADOW);

	g::x88Font = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::x88Font, "Tahoma", 16, 1100, 0, 0, FONTFLAG_OUTLINE);

	g::grenade = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::grenade, "undefeated", 16, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	g::ReEspText = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::ReEspText, "Tahoma", 12, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

    Utils::Log("Hooked");
}

void Hooks::Restore()
{
	Utils::Log("Unhooking in progress...");
    {   // Unhook every function we hooked and restore original one
		g_Hooks.pClientHook->Unhook(vtable_indexes::frameStage);
        g_Hooks.pClientModeHook->Unhook(vtable_indexes::createMove);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::view);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::viewmodelFov);
        g_Hooks.pSurfaceHook->Unhook(vtable_indexes::lockCursor);-
		g_Hooks.pPanelHook->Unhook(vtable_indexes::paint);
		g_Hooks.pModelHook->Unhook(vtable_indexes::dme);
		g_Hooks.pRenderViewHook->Unhook(vtable_indexes::sceneEnd);

        SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_Hooks.pOriginalWNDProc));
        g_pNetvars.reset();   /* Need to reset by-hand, global pointer so doesnt go out-of-scope */
    }
    Utils::Log("Unhooking succeded!");
}

void Hooks::HookPlayers()
{
	static bool Init[65];
	static bool Hooked[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			if (Hooked[i])
				g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

			Hooked[i] = false;
			continue;
		}

		if (!Init[i])
		{
			g_Hooks.pPlayerHook[i] = std::make_unique<ShadowVTManager>();
			Init[i] = true;
		}
		
		if (Hooked[i])
			g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

		if (!Hooked[i])
		{
			g_Hooks.pPlayerHook[i]->Setup(pPlayerEntity);
			g_Hooks.pPlayerHook[i]->Hook(vtable_indexes::extraBonePro, Hooks::DoExtraBonesProcessing);

			Hooked[i] = true;
		}
	}
}

void Hooks::SetClanTag(const char* tag, const char* name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utils::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}

bool __fastcall Hooks::CreateMove(IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd)
{
	static auto oCreateMove = g_Hooks.pClientModeHook->GetOriginal<CreateMove_t>(vtable_indexes::createMove);
	oCreateMove(thisptr, edx, sample_frametime, pCmd);

    if (!pCmd || !pCmd->command_number)
		return oCreateMove;

    g::pCmd         = pCmd;
    g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	g::bSendPacket  = true;

	if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return oCreateMove;

	if (g_Menu.Config.quickerplant)
		g_Misc.FixedBombPlant(pCmd);

	if (g_Menu.Config.defuseyes)
	{
		for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
			if (pEnt && pEnt != g::pLocalEntity && pEnt->GetClientClass()->ClassID == EClassIds::CPlantedC4)
			{
				if (g::pLocalEntity->InBombZone())
				{
					float flBlow = pEnt->m_flC4Blow();
					float lifetime = flBlow - (g_pGlobalVars->intervalPerTick * g::pLocalEntity->GetTickBase());
					if (flBlow < (g::pLocalEntity->hasdefuser() ? 5.05f : 10.05f))
						g::pCmd->buttons |= IN_USE;
				}
			}
		}
	}

	uintptr_t *framePtr;
	__asm mov framePtr, ebp;

	g::OriginalView = g::pCmd->viewangles;

	g::pCmd->buttons |= IN_BULLRUSH;

	if (g_Menu.Config.blockbotonhead && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.BlockBotKey)))
		g_Misc.onheadblockbot(pCmd);

    g_Misc.OnCreateMove();

	g_Resolver.OnCreateMove();

	if (g_Menu.Config.GrenadePrediction)
		GrenadePrediction::get().Tick(g::pCmd->buttons);

	g_EnginePrediction.Start();
	if (g::pLocalEntity->IsAlive())
	{
		fakelag->do_fakelag(pCmd, g::pLocalEntity);
	}

	g_AntiAim.OnCreateMove();

	g_LegitAntiAim.Run();

	g_Aimbot.OnCreateMove();

	if (g_Menu.Config.EdgeJump && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.EdgeJumpKey)))
		g_Misc.EdgeJump();

	g_EnginePrediction.End();

	g_Misc.MovementFix(g::OriginalView);

	Backtracking->LegitBackTrack(pCmd, g::pLocalEntity);

	g_LegitAimbot.Aim_CreateMove(pCmd);

	g_Nightmode.Run();

	if (g_Menu.Config.namestealer)
	{
		for (int i = 0; i < g_pEngine->GetMaxClients(); i++)
		{
			C_BaseEntity* player = g_pEntityList->GetClientEntity(i);
			if (!player)
				continue;
			if (player == g::pLocalEntity)
				continue;
			PlayerInfo_t pInfo;
			g_pEngine->GetPlayerInfo(i, &pInfo);
			if (pInfo.ishltv)
				continue;
			std::string name = "  ";
			name += pInfo.szName;
			name += " ";
			char* tekst = &name[0];
			g_Misc.SetName(tekst);

		}
	}

	if (!g_Menu.Config.Antiaim && !g_Menu.Config.LegitAA)
	{
		Vector view;
		g_pEngine->GetViewAngles(view);
		g::RealAngle = view;
	}
	else
	{
		if (g::bSendPacket)
			g::RealAngle = g::pCmd->viewangles;
		else
			g::FakeAngle = g::pCmd->viewangles;
	}

	*reinterpret_cast<bool*>(*framePtr - 0x1C) = g::bSendPacket;

	g_Math.Clamp(g::pCmd->viewangles);

    return false;
}

void __fastcall Hooks::SceneEnd(void *ecx, void *edx)
{
	static auto oSceneEnd = g_Hooks.pRenderViewHook->GetOriginal<SceneEnd_t>(vtable_indexes::sceneEnd);
	oSceneEnd(ecx, edx);

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity)
		return;
	
	static auto linegoesthrusmoke = Utils::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
	
	if (g_Menu.Config.NoSmoke) {
		std::vector<const char*> vistasmoke_wireframe = { "particle/vistasmokev1/vistasmokev1_smokegrenade", };

		for (auto mat_s : vistasmoke_wireframe)
		{
			IMaterial* mat = g_pMaterialSys->FindMaterial(mat_s, "Other textures");
			mat->IncrementReferenceCount();
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		}

		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		*(int*)(smokecout) = 0;
    }


	IMaterial* ChamsMaterial = nullptr;
	switch (g_Menu.Config.ChamsMaterial)
	{
	case 0:
		ChamsMaterial = g_pMaterialSys->FindMaterial("MetallicLitChams", "Model textures");
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 1:
		ChamsMaterial = g_pMaterialSys->FindMaterial("PlasticLitChams", "Model textures");
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 2:
		ChamsMaterial = g_pMaterialSys->FindMaterial("UnlitChams", "Model textures");
		ChamsMaterial->IncrementReferenceCount();
		break;
	case 3:
		ChamsMaterial = g_pMaterialSys->FindMaterial("chamsAnimated", "Model textures");
		ChamsMaterial->IncrementReferenceCount();
		break;
	default:
		break;
	}

	if (ChamsMaterial)
	{
		for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
		{
			C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant()
				|| !g::pLocalEntity)
				continue;

			if (pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam()) // Team
			{
				if (pPlayerEntity == g::pLocalEntity) //Local
				{
					if (g_Menu.Config.DesyncChams && g_Menu.Config.Antiaim && g_Menu.Config.DesyncAngle && !g::FreezeTime && g_Menu.Config.IsThirdperson)
					{
						float VisColor[3] = { 1.f, 1.f, 1.f };
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(g_Menu.Config.DesyncChamsTransparency / 255.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(VisColor);
						g_pPrediction->SetViewAngles(Vector(0, g::FakeAngle.y, 0));
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pPrediction->SetViewAngles(Vector(0, g::RealAngle.y, 0));
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}

					float VisColor[3] = { g_Menu.Config.LocalVisChamsColor.red / 255.f, g_Menu.Config.LocalVisChamsColor.green / 255.f, g_Menu.Config.LocalVisChamsColor.blue / 255.f };
					float XQZColor[3] = { g_Menu.Config.LocalXQZChamsColor.red / 255.f, g_Menu.Config.LocalXQZChamsColor.green / 255.f, g_Menu.Config.LocalXQZChamsColor.blue / 255.f };

					if (g_Menu.Config.LocalChams)
					{
						if (g_Menu.Config.LocalXQZChams)
						{
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							ChamsMaterial->AlphaModulate(1.f);
							g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
							g_pRenderView->SetColorModulation(XQZColor);
							pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
							g_pModelRender->ForcedMaterialOverride(nullptr);
						}

						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(VisColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}
				}
				else
				{
					float VisColor[3] = { g_Menu.Config.TeamVisChamsColor.red / 255.f, g_Menu.Config.TeamVisChamsColor.green / 255.f, g_Menu.Config.TeamVisChamsColor.blue / 255.f };
					float XQZColor[3] = { g_Menu.Config.TeamXQZChamsColor.red / 255.f, g_Menu.Config.TeamXQZChamsColor.green / 255.f, g_Menu.Config.TeamXQZChamsColor.blue / 255.f };

					if (g_Menu.Config.TeamChams)
					{
						if (g_Menu.Config.TeamXQZChams)
						{
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							ChamsMaterial->AlphaModulate(1.f);
							g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
							g_pRenderView->SetColorModulation(XQZColor);
							pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
							g_pModelRender->ForcedMaterialOverride(nullptr);
						}

						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(VisColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}
				}
			}
			else // Enemy
			{
				float VisColor[3] = { g_Menu.Config.EnemyVisChamsColor.red / 255.f, g_Menu.Config.EnemyVisChamsColor.green / 255.f, g_Menu.Config.EnemyVisChamsColor.blue / 255.f };
				float XQZColor[3] = { g_Menu.Config.EnemyXQZChamsColor.red / 255.f, g_Menu.Config.EnemyXQZChamsColor.green / 255.f, g_Menu.Config.EnemyXQZChamsColor.blue / 255.f };

				if (g_Menu.Config.EnemyChams)
				{
					if (g_Menu.Config.EnemyXQZChams)
					{
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						ChamsMaterial->AlphaModulate(1.f);
						g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
						g_pRenderView->SetColorModulation(XQZColor);
						pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}

					ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					ChamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
					ChamsMaterial->AlphaModulate(1.f);
					g_pModelRender->ForcedMaterialOverride(ChamsMaterial);
					g_pRenderView->SetColorModulation(VisColor);
					pPlayerEntity->DrawModel(0x1, pPlayerEntity->GetModelInstance());
					g_pModelRender->ForcedMaterialOverride(nullptr);
				}
			}

			float LagColor[3] = { g_Menu.Config.ShowLagColor.red / 255.f, g_Menu.Config.ShowLagColor.green / 255.f, g_Menu.Config.ShowLagColor.blue / 255.f };

			if (pPlayerEntity == g::pLocalEntity && g_Menu.Config.ShowLag && g::pLocalEntity->GetVelocity().Length2D() > 5)
			{
				static IMaterial* material = g_pMaterialSys->FindMaterial("PlasticLitChams", "Model textures");

				if (!material)
					return;

				if (g::pLocalEntity->IsKnifeorNade())
					return;

				static auto last_networked_origin = Vector(0, 0, 0);

				if (g::bSendPacket)
				{
					last_networked_origin = g::pLocalEntity->GetAbsOrigin();
				}

				g::pLocalEntity->SetAbsOrigin(last_networked_origin);
				material->AlphaModulate(100.f / 255.f);
				g_pModelRender->ForcedMaterialOverride(material);
				g_pRenderView->SetColorModulation(LagColor);
				g::pLocalEntity->DrawModel(0x1, g::pLocalEntity->GetModelInstance());
				g_pModelRender->ForcedMaterialOverride(nullptr);
				g::pLocalEntity->SetAbsOrigin(g::pLocalEntity->GetAbsOrigin());
			}
		}
	}
}

void __fastcall Hooks::DoExtraBonesProcessing(void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6)
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity)
		return;

	auto oDoExtraBonesProcessing = g_Hooks.pPlayerHook[pPlayerEntity->EntIndex()]->GetOriginal<ExtraBoneProcess_t>(vtable_indexes::extraBonePro);

	//float Backup = pPlayerEntity->AnimState()->m_flUnknownFraction;
	//pPlayerEntity->AnimState()->m_flUnknownFraction = 0;

	oDoExtraBonesProcessing(ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6);

	//pPlayerEntity->AnimState()->m_flUnknownFraction = Backup;
}

bool resetMaterials = true;

void __fastcall Hooks::DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
	static auto oDrawModelExecute = g_Hooks.pModelHook->GetOriginal<DrawModelExecute_t>(vtable_indexes::dme);

	const char* ModelName = g_pModelInfo->GetModelName((model_t*)info.pModel);
	const std::string strModelName = g_pModelInfo->GetModelName((model_t*)info.pModel);

	g_GlowEsp.RunGlow();

	if (g_Menu.Config.Nightmode || g_Menu.Config.ProperNightmode)
	{
		if (strstr(ModelName, "models/props"))
		{
			g_pRenderView->SetBlend(g_Menu.Config.NightmodeBlend / 255.f);
		}
	}

	if (strstr(ModelName, "models/player/") && g_Menu.Config.Aimbot && g::pLocalEntity && g::pLocalEntity->IsScoped())
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity && pEnt && pEnt == g::pLocalEntity)
			g_pRenderView->SetBlend(0.25f);
	}

	if (strstr(ModelName, "models/player/") && (g_Menu.Config.LegitBacktrack || g_Menu.Config.PosBacktrack) && g_Menu.Config.BacktrackingChams != 0)
	{
		C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(info.index);
		if (g::pLocalEntity &&
			pEnt &&
			pEnt != g::pLocalEntity &&
			pEnt->IsAlive() &&
			pEnt->GetTeam() != g::pLocalEntity->GetTeam())
		{
			if (!g_LagComp.PlayerRecord[info.index].empty() && pEnt->GetVelocity().Length2D() > 5)
			{
				float OLDCOLORMODULATION[3];
				g_pRenderView->GetColorModulation(OLDCOLORMODULATION);
				if (g_Menu.Config.BacktrackingChams != 4)
				{
					for (int i = 0; i < g_LagComp.PlayerRecord[info.index].size(); i++)
					{
						if (g_Menu.Config.BacktrackingChams == 1)
						{
							float BTCOLOR[3] = { (i * 21.25f) / 255.f, (255 - (i * 21.25f)) / 255.f, 1.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend(0.05f);
						}
						else if (g_Menu.Config.BacktrackingChams == 2)
						{
							float BTCOLOR[3] = { (255 - (i * 21.25f)) / 255.f, (255 - (i * 21.25f)) / 255.f, (255 - (i * 21.25f)) / 255.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend(0.05f);
						}
						else if (g_Menu.Config.BacktrackingChams == 3)
						{
							float BTCOLOR[3] = { g_Menu.Config.EnemyVisChamsColor.red / 255.f, g_Menu.Config.EnemyVisChamsColor.green / 255.f, g_Menu.Config.EnemyVisChamsColor.blue / 255.f };
							g_pRenderView->SetColorModulation(BTCOLOR);
							g_pRenderView->SetBlend((i * 21.25f) / 255.f);
						}
						oDrawModelExecute(ecx, context, state, info, g_LagComp.PlayerRecord[info.index].at(i).Matrix);
					}
				}
				else
				{
					float BTCOLOR[3] = { 1.f, 1.f, 1.f };
					g_pRenderView->SetColorModulation(BTCOLOR);
					g_pRenderView->SetBlend(0.5f);
					oDrawModelExecute(ecx, context, state, info, g_LagComp.PlayerRecord[info.index].at(0).Matrix);
				}

				g_pRenderView->SetColorModulation(OLDCOLORMODULATION);
				g_pRenderView->SetBlend(1.f);
				oDrawModelExecute(ecx, context, state, info, matrix);
			}
		}
	}



	if (strstr(ModelName, "weapons/v") && !strstr(ModelName, "arms")/*|| strstr(ModelName, "models/weapons")*/ && g_Menu.Config.WeaponChams)
	{

		static IMaterial* material = g_pMaterialSys->FindMaterial("PlasticLitChams", "Model textures");

		if (g::pLocalEntity->IsScoped())
			return;

		if (material && !(strstr(ModelName, "arms")))
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			float WEAPONCOLOR[3] = { g_Menu.Config.WeaponChamsColor.red / 255.f, g_Menu.Config.WeaponChamsColor.green / 255.f, g_Menu.Config.WeaponChamsColor.blue / 255.f };
			g_pRenderView->SetColorModulation(WEAPONCOLOR);
			g_pRenderView->SetBlend(g_Menu.Config.WeaponChamsTransparency / 255.f);
			g_pModelRender->ForcedMaterialOverride(material);
		}
		if (strstr(ModelName, "arms") && g_Menu.Config.ArmsChams)
		{
			static IMaterial* material;
			if (g_Menu.Config.ArmsChamsMode == 0)
			{
				material = g_pMaterialSys->FindMaterial("PlasticLitChams", "Model textures");
				material->IncrementReferenceCount();
			}
			else if (g_Menu.Config.ArmsChamsMode == 1)
			{
				material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_lightray", "Model textures");
				material->IncrementReferenceCount();
			}
			if (material)
			{
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Menu.Config.ArmsWireFrame);
				float ARMSCOLOR[3] = { g_Menu.Config.ArmsChamsColor.red / 255.f, g_Menu.Config.ArmsChamsColor.green / 255.f, g_Menu.Config.ArmsChamsColor.blue / 255.f };
				g_pRenderView->SetColorModulation(ARMSCOLOR);
				g_pRenderView->SetBlend(g_Menu.Config.ArmsChamsTransparency / 255.f);
				g_pModelRender->ForcedMaterialOverride(material);
			}
		}
	}

	if (strstr(ModelName, "arms") && g_Menu.Config.ArmsChams)
	{
		static IMaterial* material;
		if (g_Menu.Config.ArmsChamsMode == 0)
		{
			material = g_pMaterialSys->FindMaterial("PlasticLitChams", "Model textures");
			material->IncrementReferenceCount();
		}
		else if (g_Menu.Config.ArmsChamsMode == 1)
		{
			material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_lightray", "Model textures");
			material->IncrementReferenceCount();
		}
		if (material)
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Menu.Config.ArmsWireFrame);
			float ARMSCOLOR[3] = { g_Menu.Config.ArmsChamsColor.red / 255.f, g_Menu.Config.ArmsChamsColor.green / 255.f, g_Menu.Config.ArmsChamsColor.blue / 255.f };
			g_pRenderView->SetColorModulation(ARMSCOLOR);
			g_pRenderView->SetBlend(g_Menu.Config.ArmsChamsTransparency / 255.f);
			g_pModelRender->ForcedMaterialOverride(material);
		}
	}

	oDrawModelExecute(ecx, context, state, info, matrix);
}

void __fastcall Hooks::FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	static auto oFrameStage = g_Hooks.pClientHook->GetOriginal<FrameStageNotify_t>(vtable_indexes::frameStage);

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && g::pLocalEntity)
	{
		g_Misc.ThirdPerson(curStage);

		g_Resolver.FrameStage(curStage);

		if (g::pLocalEntity->IsAlive())
		{
			if (g::knifeapply)
			{
				KnifeApplyCallbk();
				g::knifeapply = false;
			}

			if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			{
				skinchanger();
				//glovechanger();//ez
			}
		}
	}

	oFrameStage(ecx, curStage);
}

C_BaseEntity* UTIL_PlayerByIndex(int index)
{
	typedef C_BaseEntity*(__fastcall* PlayerByIndex)(int);
	static PlayerByIndex UTIL_PlayerByIndex = (PlayerByIndex)Utils::FindSignature("server.dll", "85 C9 7E 2A A1");

	if (!UTIL_PlayerByIndex)
		return false;

	return UTIL_PlayerByIndex(index);
}

void hit()
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	static int width = 0;
	static int height = 0;
	if (height == 0 || width == 0)
		g_pEngine->GetScreenSize(width, height);

	float alpha = 0.f;

	for (size_t i = 0; i < HitMarkers.size(); i++)
	{
		float timeDiff = HitMarkers[i].expireTime - g_pGlobalVars->curtime;

		if (timeDiff < 0.f)
		{
			HitMarkers.erase(HitMarkers.begin() + i);
			continue;
		}

		std::string dmg = "-";
		dmg += std::to_string(HitMarkers[i].damage).c_str();

		int moveDist = 24;

		float ratio = 1.f - (timeDiff / g_Menu.Config.ExpireTime);
		alpha = (timeDiff / g_Menu.Config.ExpireTime) * 2.f;
		if (g_Menu.Config.ShowDmg)
			g_pSurface->DrawT(width / 2 + 6 + ratio * moveDist / 2, height / 2 + 6 + ratio * moveDist, Color(255, 255, 255, 255), g::Tahoma, false, dmg.c_str());
	}

	if (HitMarkers.size() > 0)
	{
		int lineSize = (int)g_Menu.Config.MarkerSize;
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_pEngine->GetScreenSize(screenSizeX, screenSizeY);
		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;
		g_pSurface->Line(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, 255));
		g_pSurface->Line(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, 255));

	}
}

void __fastcall Hooks::PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static auto oPaintTraverse = g_Hooks.pPanelHook->GetOriginal<PaintTraverse_t>(vtable_indexes::paint);
	static unsigned int panelID, panelHudID;

	if (!panelHudID)
	{
		if (!strcmp("HudZoom", g_pPanel->GetName(vguiPanel)))
		{
			panelHudID = vguiPanel;
		}
	}

	if (panelHudID == vguiPanel && g::pLocalEntity && g::pLocalEntity->IsAlive() && g_Menu.Config.NoScope)
	{
		if (g::pLocalEntity->IsScoped())
			return;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	if (!panelID)
	{
		if (!strcmp("MatSystemTopPanel", g_pPanel->GetName(vguiPanel)))
		{
			panelID = vguiPanel;
			g_Hooks.bInitializedDrawManager = true;
		}
	}
			
	if (panelID == vguiPanel) 
	{
		g_ESP.Render();

		// Remastered Esp
		g_RemasteredEsp.Render();

		g_Misc.Crosshair();
		bulletTrace();
		g_ESP.QuakeKillCounter();
		if (g_Menu.Config.VisHitmarker)
		hit();
		if (g_Menu.Config.GrenadePrediction)
		{
			GrenadePrediction::get().Paint();
			paintgrenadeparth();
		}
		g_SpecList.Draw();
		g_Menu.Render();
	}

	// Disable Panorama Blur
	ConVar* PanoramaDisableBlur = g_pCvar->FindVar("@panorama_disable_blur");
	PanoramaDisableBlur->SetValue(g_Menu.Config.DisablePanoramaBlur);

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g::pLocalEntity)
		return;

	static const auto real_color = Color(255, 0, 0, 200);
	static const auto fake_color = Color(50, 50, 255, 200);
	static const auto lby_color = Color(0, 255, 0, 200);
	static const auto advanced_shit = Color(255, 255, 255, 80);

	if (g_Menu.Config.AntiAimIndicator)
	{
		if (!g_Menu.Config.Antiaim && !g_Menu.Config.LegitAA)
			return;

		if (!g_Menu.Config.IsThirdperson)
			return;
		
		if (!g::pLocalEntity->IsAlive())
			return;

		float distance = 40.f;

		Vector local_origin = g::pLocalEntity->GetAbsOrigin();

		Vector2D screen1, screen2;

		Utils::WorldToScreen(local_origin, screen1);

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::FakeAngle.y, distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, fake_color);
		g_pSurface->DrawT(screen2.x, screen2.y, fake_color, g::TahomaSmall, true, "FAKE");

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::RealAngle.y, distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, real_color);
		g_pSurface->DrawT(screen2.x, screen2.y, real_color, g::TahomaSmall, true, "REAL");

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->GetLowerBodyYaw(), distance), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, lby_color);
		g_pSurface->DrawT(screen2.x, screen2.y, lby_color, g::TahomaSmall, true, "LBY");

		if (!g_Menu.Config.AdvancedAAIndicator)
			return;

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flCurrentFeetYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "CUR_FEET");

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flGoalFeetYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "GOAL_FEET");

		Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flCurrentTorsoYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "CUR_TORSO");

		/*Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flEyeYaw, 30.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "EYES");*/

		/*Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::GoalLby, 35.f), screen2);
		g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
		g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "GOAL_LBY");*/
	}

	if (g_Menu.Config.SkeletonEsp[0])
	{
		for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
		{
			C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

			if (g_Menu.Config.Esp == 2 && g::pLocalEntity->IsAlive())
				continue;

			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant()
				|| pPlayerEntity == g::pLocalEntity
				|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
				continue;

			Vector Hitboxes[19];
			Vector2D HitboxW2S[19];

			for (int Hitbox = 0; Hitbox < 19; Hitbox++)
			{
				Hitboxes[Hitbox] = pPlayerEntity->GetHitboxPosition(Hitbox, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]);
				Hitbox == 15 ? Utils::WorldToScreen(Hitboxes[Hitbox] + Vector(0, 0, 3), HitboxW2S[Hitbox]) : Utils::WorldToScreen(Hitboxes[Hitbox], HitboxW2S[Hitbox]);
			}

			// Spine
			g_pSurface->Line(HitboxW2S[HITBOX_HEAD].x, HitboxW2S[HITBOX_HEAD].y, HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_LOWER_CHEST].x, HitboxW2S[HITBOX_LOWER_CHEST].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_LOWER_CHEST].x, HitboxW2S[HITBOX_LOWER_CHEST].y, HitboxW2S[HITBOX_THORAX].x, HitboxW2S[HITBOX_THORAX].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_THORAX].x, HitboxW2S[HITBOX_THORAX].y, HitboxW2S[HITBOX_BELLY].x, HitboxW2S[HITBOX_BELLY].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_BELLY].x, HitboxW2S[HITBOX_BELLY].y, HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, Color(255, 255, 255));

			// Right Leg
			g_pSurface->Line(HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, HitboxW2S[HITBOX_RIGHT_THIGH].x, HitboxW2S[HITBOX_RIGHT_THIGH].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_THIGH].x, HitboxW2S[HITBOX_RIGHT_THIGH].y, HitboxW2S[HITBOX_RIGHT_CALF].x, HitboxW2S[HITBOX_RIGHT_CALF].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_CALF].x, HitboxW2S[HITBOX_RIGHT_CALF].y, HitboxW2S[HITBOX_RIGHT_FOOT].x, HitboxW2S[HITBOX_RIGHT_FOOT].y, Color(255, 255, 255));

			// Right Arm
			g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].x, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_UPPER_ARM].x, HitboxW2S[HITBOX_RIGHT_UPPER_ARM].y, HitboxW2S[HITBOX_RIGHT_FOREARM].x, HitboxW2S[HITBOX_RIGHT_FOREARM].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_RIGHT_FOREARM].x, HitboxW2S[HITBOX_RIGHT_FOREARM].y, HitboxW2S[HITBOX_RIGHT_HAND].x, HitboxW2S[HITBOX_RIGHT_HAND].y, Color(255, 255, 255));

			// Left Leg
			g_pSurface->Line(HitboxW2S[HITBOX_PELVIS].x, HitboxW2S[HITBOX_PELVIS].y, HitboxW2S[HITBOX_LEFT_THIGH].x, HitboxW2S[HITBOX_LEFT_THIGH].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_LEFT_THIGH].x, HitboxW2S[HITBOX_LEFT_THIGH].y, HitboxW2S[HITBOX_LEFT_CALF].x, HitboxW2S[HITBOX_LEFT_CALF].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_LEFT_CALF].x, HitboxW2S[HITBOX_LEFT_CALF].y, HitboxW2S[HITBOX_LEFT_FOOT].x, HitboxW2S[HITBOX_LEFT_FOOT].y, Color(255, 255, 255));

			// Left Arm
			g_pSurface->Line(HitboxW2S[HITBOX_UPPER_CHEST].x, HitboxW2S[HITBOX_UPPER_CHEST].y, HitboxW2S[HITBOX_LEFT_UPPER_ARM].x, HitboxW2S[HITBOX_LEFT_UPPER_ARM].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_LEFT_UPPER_ARM].x, HitboxW2S[HITBOX_LEFT_UPPER_ARM].y, HitboxW2S[HITBOX_LEFT_FOREARM].x, HitboxW2S[HITBOX_LEFT_FOREARM].y, Color(255, 255, 255));
			g_pSurface->Line(HitboxW2S[HITBOX_LEFT_FOREARM].x, HitboxW2S[HITBOX_LEFT_FOREARM].y, HitboxW2S[HITBOX_LEFT_HAND].x, HitboxW2S[HITBOX_LEFT_HAND].y, Color(255, 255, 255));
		}
	}

	if (g_Menu.Config.OverrideViewmodel) {
		ConVar* viewmodel_offset_x = g_pCvar->FindVar("viewmodel_offset_x");
		ConVar* viewmodel_offset_y = g_pCvar->FindVar("viewmodel_offset_y");
		ConVar* viewmodel_offset_z = g_pCvar->FindVar("viewmodel_offset_z");

		*(int*)((DWORD)&viewmodel_offset_x->fnChangeCallback + 0xC) = 0;
		*(int*)((DWORD)&viewmodel_offset_y->fnChangeCallback + 0xC) = 0;
		*(int*)((DWORD)&viewmodel_offset_z->fnChangeCallback + 0xC) = 0;

		static float old_value_x = viewmodel_offset_x->GetFloat();
		static float old_value_y = viewmodel_offset_y->GetFloat();
		static float old_value_z = viewmodel_offset_z->GetFloat();

		viewmodel_offset_x->SetValue(old_value_x + (g_Menu.Config.ViewmodelOffsetX - 20));
		viewmodel_offset_y->SetValue(old_value_y + (g_Menu.Config.ViewmodelOffsetY - 20));
		viewmodel_offset_z->SetValue(old_value_z + (g_Menu.Config.ViewmodelOffsetZ - 20));
	}

	if (g_Menu.Config.NoFlash)
	{
		g::pLocalEntity->SetFlashDuration(g_Menu.Config.FlashDuration / 255.f);
	}

	if (g_Menu.Config.aspectratio)
		g_Misc.acpectratio(g_Menu.Config.aspectvalue);
	else
		g_Misc.acpectratio(0);

	if (g_Menu.Config.ShowServerHitboxes && g::pLocalEntity && g::pLocalEntity->IsAlive())
	{
		static uintptr_t pCall = (uintptr_t)Utils::FindSignature("server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE");
			
		static float fDuration = 0.1f;

		PVOID pEntity = nullptr;
		pEntity = UTIL_PlayerByIndex(g::pLocalEntity->EntIndex());

		if (pEntity)
		{
			__asm
			{
				pushad
				movss xmm1, fDuration
				push 0 //bool monoColor
				mov ecx, pEntity
				call pCall
				popad
			}
		}
	}

	if (g_Menu.Config.BombEsp)
	{
		for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
			if (pEnt && pEnt != g::pLocalEntity)
			{
				Vector2D PlantedPos;
				Vector2D DroppedPos;

				if (pEnt->GetClientClass()->ClassID == EClassIds::CPlantedC4)
				{
					int bombTimer = std::ceil(pEnt->m_flC4Blow() - g_pGlobalVars->curtime);
					std::string timer = std::to_string(bombTimer);
					char* timere = &timer[0];

					if (pEnt->GetOrigin() != Vector(0, 0, 0) && Utils::WorldToScreen(pEnt->GetOrigin(), PlantedPos))
					{
						g_pSurface->DrawT(PlantedPos.x, PlantedPos.y - 24, Color(255, 255, 255), g::WeaponIcons, true, "d");
						g_pSurface->DrawT(PlantedPos.x, PlantedPos.y, Color(255, 255, 255), g::Tahoma, true, "Planted Bomb");
						if(bombTimer >= 0)
							g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 10, Color(255, 255, 255), g::Tahoma, true, timere);
						else
							g_pSurface->DrawT(PlantedPos.x, PlantedPos.y + 10, Color(255, 255, 255), g::Tahoma, true, "bum");
					}
				}
				else if (pEnt->GetClientClass()->ClassID == EClassIds::CC4)
				{
					if (pEnt->GetOrigin() != Vector(0, 0, 0) && Utils::WorldToScreen(pEnt->GetOrigin(), DroppedPos))
					{
						g_pSurface->DrawT(DroppedPos.x, DroppedPos.y - 24, Color(255, 255, 255), g::WeaponIcons, true, "d");
						g_pSurface->DrawT(DroppedPos.x, DroppedPos.y, Color(255, 255, 255), g::Tahoma, true, "Dropped Bomb");
					}
				}
			}
		}
	}
}

float dot_product(const Vector& a, const Vector& b) {
	return (a.x * b.x
		+ a.y * b.y
		+ a.z * b.z);
}
static void CapsuleOverlay(C_BaseEntity* pPlayer, Color col, float duration)
{
	if (!pPlayer)
		return;

	studiohdr_t* pStudioModel = g_pModelInfo->GetStudiomodel((model_t*)pPlayer->GetModel());
	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];
	if (!pPlayer->SetupBones(pBoneToWorldOut, 128, 256, 0))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	auto VectorTransform2 = [](const Vector in1, matrix3x4_t in2, Vector& out)
	{

		out[0] = dot_product(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = dot_product(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = dot_product(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	};

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		VectorTransform2(pHitbox->min, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform2(pHitbox->max, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->radius > -1)
		{
			g_pDebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->radius, col.red, col.green, col.blue, col.alpha, duration);
		}
	}
}
template<class T2>
static T2* FindHudElement2(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD * *>(Utils::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T2*)find_hud_element(pThis, name);
}
PlayerInfo_t GetInfo(int Index) {
	PlayerInfo_t Info;
	g_pEngine->GetPlayerInfo(Index, &Info);
	return Info;
}
static bool buybotfix;
void Event::FireGameEvent(IGameEvent* event)
{
	if (!event)
		return;

	if (!g::pLocalEntity)
		return;

	if (buybotfix)
	{
		if (g_Menu.Config.BuyBot.enable)
		{
			//armor
			if (g_Menu.Config.BuyBot.armor == 0)
				g_pEngine->ExecuteClientCmd("");
			if (g_Menu.Config.BuyBot.armor == 1)
				g_pEngine->ExecuteClientCmd("buy vesthelm");
			//main
			if (g_Menu.Config.BuyBot.main == 0)
				g_pEngine->ExecuteClientCmd("");
			if (g_Menu.Config.BuyBot.main == 1)
				g_pEngine->ExecuteClientCmd("buy g3sg1; buy scar20");
			if (g_Menu.Config.BuyBot.main == 2)
				g_pEngine->ExecuteClientCmd("buy awp");
			if (g_Menu.Config.BuyBot.main == 3)
				g_pEngine->ExecuteClientCmd("buy ssg08");
			//second
			if (g_Menu.Config.BuyBot.main == 0)
				g_pEngine->ExecuteClientCmd("");
			if (g_Menu.Config.BuyBot.second == 1)
				g_pEngine->ExecuteClientCmd("buy deagle; buy revolver;  buy taser;");
			if (g_Menu.Config.BuyBot.second == 2)
				g_pEngine->ExecuteClientCmd("buy elite;  buy taser;");
			//grenade
			if (g_Menu.Config.BuyBot.armor == 0)
				g_pEngine->ExecuteClientCmd("");
			if (g_Menu.Config.BuyBot.grenade == 1)
				g_pEngine->ExecuteClientCmd("buy hegrenade; buy molotov; buy smokegrenade; buy incgrenade;");

		}
		buybotfix = false;
	}

	if (strstr(event->GetName(), "round_start") || strstr(event->GetName(), "round_start") || strstr(event->GetName(), "round_prestart") || strstr(event->GetName(), "client_disconnect") || strstr(event->GetName(), "cs_game_disconnected")) // trying everything
	{
		buybotfix = true;
		return;
	}


	auto filter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
	auto name = event->GetName();
	if (strstr(event->GetName(), "player_hurt")) {

		auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));
		if (!attacker)
			return;

		if (attacker != g::pLocalEntity)
			return;

		int index = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
		int hitgroup = event->GetInt("hitgroup");

		if (g_pEngine->GetPlayerForUserID(event->GetInt("attacker")) == g_pEngine->GetLocalPlayer())
		{
			HitMarker_t temp = { g_pGlobalVars->curtime + g_Menu.Config.ExpireTime, event->GetInt("dmg_health") };

			if (temp.damage > 0 && temp.damage < 100)
				HitMarkers.push_back(temp);
		}

		PlayerInfo_t pInfo;
		g_pEngine->GetPlayerInfo(index, &pInfo);

		g::Hit[index] = true;
		g::hits++;
		switch (g_Menu.Config.Hitmarker)
		{
		case 1:
			g_pSurface->PlaySoundA("buttons\\arena_switch_press_02.wav"); // metalic
			break;
		case 2:
			g_pSurface->PlaySoundA("physics\\metal\\paintcan_impact_hard3.wav"); // metalic2
			break;
		case 3:
			g_pSurface->PlaySoundA("buttons\\combine_button7.wav");  // beep1
			break;
		case 4:
			g_pSurface->PlaySoundA("buttons\\button14.wav"); //beep 2
			break;
		case 5:
			g_pSurface->PlaySoundA("buttons\\bell1.wav"); //bell
			break;
		case 6:
			g_pSurface->PlaySoundA("player\\headshot1.wav"); // hs 1
			break;
		case 7:
			g_pSurface->PlaySoundA("player\\headshot2.wav"); // hs 2
			break;
		case 8:
			PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 9:
			PlaySoundA(fatality, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 10:
			PlaySoundA(cod, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		case 11:
			PlaySoundA(laser, NULL, SND_ASYNC | SND_MEMORY); break;
			break;
		default:
			break;
		}

		if (g_Menu.Config.VizualizeHits) 
		{
			C_BaseEntity* hittedplayer = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));

			if (hittedplayer && hittedplayer->EntIndex() > 0 && hittedplayer->EntIndex() < 64)
			{
				if (g::pLocalEntity && hittedplayer != g::pLocalEntity)
					CapsuleOverlay(hittedplayer, Color(g_Menu.Config.CapsuleColor.red, g_Menu.Config.CapsuleColor.green, g_Menu.Config.CapsuleColor.blue, 255), 3);
			}
		}

		static auto get_hitgroup_name = [](int hitgroup) -> std::string {
			switch (hitgroup) {
			case HITGROUP_HEAD:
				return "head";
			case HITGROUP_LEFTLEG:
				return "left leg";
			case HITGROUP_RIGHTLEG:
				return "right leg";
			case HITGROUP_STOMACH:
				return "stomach";
			case HITGROUP_LEFTARM:
				return "left arm";
			case HITGROUP_RIGHTARM:
				return "right arm";
			default:
				return "body";
			}
		};

		int attackerid = event->GetInt("attacker");
		int entityid = g_pEngine->GetPlayerForUserID(attackerid);

		int userid = event->GetInt("userid");
		int userentityid = g_pEngine->GetPlayerForUserID(userid);
		if (g_Menu.Config.LogDamage)
		{
			if (entityid == g_pEngine->GetLocalPlayer() && userentityid != g_pEngine->GetLocalPlayer())
			{
				int nUserID = event->GetInt("attacker");
				int nDead = event->GetInt("userid");
				if (nUserID || nDead)
				{
					PlayerInfo_t killer_info = GetInfo(g_pEngine->GetPlayerForUserID(nUserID));
					PlayerInfo_t killed_info = GetInfo(g_pEngine->GetPlayerForUserID(nDead));

					std::string hit = xorstr(" \x02[t4zcheats] \x01hit"" ").crypt_get();
					std::string player = killed_info.szName;
					std::string inthe = xorstr(" in the ").crypt_get();
					std::string hitbox = get_hitgroup_name(event->GetInt("hitgroup"));
					std::string forr = xorstr(" for ").crypt_get();
					std::string damage = event->GetString("dmg_health");
					std::string damage2 = xorstr(" damage").crypt_get();
					std::string fourb = xorstr(" (").crypt_get();
					std::string fiveb = event->GetString("health");
					std::string six = xorstr(" health remaining)").crypt_get();

					g_ChatElement->ChatPrintf(0, filter, (hit + player + inthe + hitbox + forr + damage + damage2 + fourb + fiveb + six).c_str());
				}
			}
		}

	//	g_Aimbot.player_hurt_event(event);

		auto victim = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));

	}
	else if (strstr(event->GetName(), "player_death")) {

		if (!g_pEngine->IsConnected() || !g_pEngine->IsConnected())
			return;

		if (!g::pLocalEntity)
			return;

		auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));
		if (!attacker)
			return;

		if (attacker != g::pLocalEntity)
			return;

		int index = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));

		if (!index)
			return;

		static DWORD* deathNotice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");

		static DWORD* _death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
		static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");


		if (_death_notice)
			* (float*)((DWORD)_death_notice + 0x50) = (g_Menu.Config.PreserveKillFeed ? 500 : 1.5);

		if (g_Menu.Config.KillCounter)
		{
			g::killtime = g_pGlobalVars->realtime;
			g::kills++;
		}

		g::globalAlpha = g_pGlobalVars->curtime;
	}
	else if (strstr(event->GetName(), "round_start"))
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsConnected())
			return;

		if (!g::pLocalEntity)
			return;

		g::kills = 0;

		static DWORD* deathNotice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");

		static DWORD* _death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
		static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");

		_death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
		_clear_notices(((DWORD)_death_notice - 20));
	}
	else if (strstr(event->GetName(), "item_purchase"))
	{

	}
	else if (strstr(event->GetName(), "round_freeze_end"))
	{
		g::FreezeTime = false;
	}
	else if (strstr(event->GetName(), "round_prestart"))
	{
		g::FreezeTime = true;
	}
	else if (strstr(event->GetName(), "bullet_impact"))
	{
		if (g_Menu.Config.BulletTracers)
		{
			auto* index = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));


			Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

			if (g::pLocalEntity)
			{
				if (index)
					trace_logs.push_back(trace_info(index->GetEyePosition(), position, g_pGlobalVars->curtime, event->GetInt("userid")));
			}
		}
	}
	else if (strstr(event->GetName(), "weapon_fire") && !g::pLocalEntity->IsKnifeorNade())
	{
		if (g_pEngine->GetPlayerForUserID(event->GetInt("userid")) == g_pEngine->GetLocalPlayer())
			g::shots++;
	}
};

void __fastcall Hooks::OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	static auto oOverrideView = g_Hooks.pClientModeHook->GetOriginal<OverrideView_t>(vtable_indexes::view);

	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (!g::pLocalEntity)
			return;

		if (g_Menu.Config.NoRecoil)
		{
			Vector viewPunch = g::pLocalEntity->GetViewPunchAngle();
			Vector aimPunch = g::pLocalEntity->GetAimPunchAngle();

			pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * g_Menu.Config.AimPunchVal * 0.4499999f));
			pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * g_Menu.Config.AimPunchVal * 0.4499999f));
			pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * g_Menu.Config.AimPunchVal * 0.4499999f));
		}

		if (g_Menu.Config.IsThirdperson)
			g_Menu.Config.CurrentFov = g_Menu.Config.ThirdpersonFov;
		else
			g_Menu.Config.CurrentFov = g_Menu.Config.Fov;

		if (!g::pLocalEntity->IsScoped())
			pSetup->fov = g_Menu.Config.CurrentFov;

		if (g::pLocalEntity->GetActiveWeapon()->IsNade())
			pSetup->fov = 90;

		if (g_Menu.Config.GrenadePrediction)
			GrenadePrediction::get().View(pSetup);

		if (g_Menu.Config.OverrideViewmodel)
			g_Misc.roll_fov(pSetup);

		if (g_Menu.Config.NoZoom)
		{
			if (g::pLocalEntity->IsScoped())
				pSetup->fov = g_Menu.Config.CurrentFov;
		}

		if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Menu.Config.FakeDuckKey)) {
			pSetup->origin.z = g::pLocalEntity->GetAbsOrigin().z + 64.f; // lol
		}
	}

	oOverrideView(ecx, edx, pSetup);
}

void __fastcall Hooks::LockCursor(ISurface* thisptr, void* edx)
{
    static auto oLockCursor = g_Hooks.pSurfaceHook->GetOriginal<LockCursor_t>(vtable_indexes::lockCursor);

	if (!g_Menu.menuOpened)
        return oLockCursor(thisptr, edx);

    g_pSurface->UnLockCursor();
}

float __stdcall Hooks::GetViewmodelFov()
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame() && g::pLocalEntity)
		return g_Menu.Config.ViewmodelFov;
}

LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
    {
		if (wParam != vKey) return;

        if (uMsg == WM_KEYDOWN)
            bButton = true;
        else if (uMsg == WM_KEYUP)
            bButton = false;
    };

	const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYUP)
			bButton = !bButton;
	};

    if (g_Hooks.bInitializedDrawManager)
    {
        if (g_Menu.menuOpened)
        {
            return true;
        }
    }


    return CallWindowProcA(g_Hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
}
