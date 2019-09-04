#include "Menu.h"
#include "Config.h"
#include "Hotkeys.h"
#include "..\SDK\Vector.h"
#include "..\SDK\ISurface.h"
#include "..\Utils\Color.h"
#include "..\Utils\GlobalVars.h"
#include "..\Utils\XorStr.h"
#include "..\Features\Misc\Misc.h"
#include "..\Utils\Color.h"
#include "..\Features\Features.h"
#include "KeyVisualizer.h"
#include "SpecList.h"
#include "models.h"
#include "x88/x88Menu.h"

Menu g_Menu;
Hotkeys g_Hotkeys;
KeyVisualizer g_KeyVisualizer;

// NEW MENU
Color menu_background(33, 33, 33);
Color menu_background2(66, 66, 66);
Color menu_text(255, 255, 255);
Color menu_accent(132, 0, 195);

Color button_light(239, 239, 239);
Color button_idle_gray(191, 184, 191);
Color button_dark(23, 23, 23);

Color EnemyColor(255, 255, 255);
Color TeamColor(255, 255, 255);
Color LocalColor(255, 255, 255);
Color World2Color(50, 50, 50);
Color Prop2Color(50, 50, 50);
Color AmbientLight2Color(255, 255, 255);
Color ArmsChams2Color(255, 255, 255);
Color WeaponChams2Color(255, 255, 255);

int OldTickCount = 0;
int GlobalOpacity = 0;

bool showMenu = true;
bool shouldopenmenu = false;
bool Debug = false;

int MenuWidth;

void save() { g_Config->Save(); }
void reset() { g::shots == 0; g::hits == 0; }
void load() { g_Config->Load(); }
void unload() { g_Menu.Config.Unload = true; }
void forceupdate() { g::knifeapply = true; }
void InvisName()
{
	g_Misc.SetName("\n\xAD\xAD\xAD");
	g_Misc.SetName("\n\xAD\xAD\xAD");
	g_Misc.SetName("\n\xAD\xAD\xAD");
}

int AntiAimFlag = 0;
int EspType = 0;
int Chamstype = 0;

void Menu::Render()
{
	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	menu_accent = Color(Config.MenuColor.red, Config.MenuColor.green, Config.MenuColor.blue);
	EnemyColor = Color(Config.EnemyGlowColor.red, Config.EnemyGlowColor.green, Config.EnemyGlowColor.blue);
	TeamColor = Color(Config.TeamGlowColor.red, Config.TeamGlowColor.green, Config.TeamGlowColor.blue);
	LocalColor = Color(Config.LocalGlowColor.red, Config.LocalGlowColor.green, Config.LocalGlowColor.blue);
	World2Color = Color(Config.WorldColor.red, Config.WorldColor.green, Config.WorldColor.blue);
	Prop2Color = Color(Config.PropColor.red, Config.PropColor.green, Config.PropColor.blue);
	AmbientLight2Color = Color(Config.AmbientLightColor.red, Config.AmbientLightColor.green, Config.AmbientLightColor.blue);
	ArmsChams2Color = Color(Config.ArmsChamsColor.red, Config.ArmsChamsColor.green, Config.ArmsChamsColor.blue);
	WeaponChams2Color = Color(Config.WeaponChamsColor.red, Config.WeaponChamsColor.green, Config.WeaponChamsColor.blue);

	static bool Pressed = false;

	if (!Pressed && GetAsyncKeyState(VK_INSERT))
		Pressed = true;
	else if (Pressed && !GetAsyncKeyState(VK_INSERT))
	{
		Pressed = false;
		shouldopenmenu = !shouldopenmenu;
	}

	if (shouldopenmenu)
	{
		// opening
		if (GlobalOpacity < 255)
		{
			GlobalOpacity += 15;
			menuOpened = true;
		}
	}
	else
	{
		// closing
		if (GlobalOpacity > 0)
			GlobalOpacity -= 15;
		else
			menuOpened = false;
	}

	static Vector2D oldPos;
	static Vector2D mousePos;

	static int dragX = 0;
	static int dragY = 0;
	static int Width = MenuWidth = 600;
	static int Height = 500;

	static int iScreenWidth, iScreenHeight;

	static bool Dragging = false;
	bool click = false;

	static bool changed_Debug = false;
	if (!changed_Debug && GetAsyncKeyState(VK_DELETE))
		changed_Debug = true;
	else if (changed_Debug && !GetAsyncKeyState(VK_DELETE))
	{
		changed_Debug = false;
		Debug = !Debug;
	}

	if (Config.OnKeyAim)
	{
		if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.AimHotkey)))
			Config.Aimbot = true;
		else
			Config.Aimbot = false;
	}
	
	g_KeyVisualizer.Draw();

	//Draw info shit for testing
	if (g::pLocalEntity && g_pEngine->IsConnected() && g_pEngine->IsInGame() && Config.TopInfoBar)
	{
		g_pSurface->FilledRect(0, 0, iWidth, 12, Color(33, 33, 33, 125));
		PlayerInfo_t pInfo;
		g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &pInfo);
		std::string debug;
		debug += "Name: " + (std::string)pInfo.szName;
		debug += " | Health: " + std::to_string(g::pLocalEntity->GetHealth());
		debug += " | Armor: " + std::to_string(g::pLocalEntity->ArmorValue());
		if (g::pLocalEntity->GetActiveWeapon())
			debug += " | Weapon: " + g::pLocalEntity->GetActiveWeapon()->GetName();
		debug += " | Minimum Damage: " + std::to_string(Config.Mindmg);
		debug += " | Hitchance: " + std::to_string(Config.HitchanceValue);
		g_pSurface->DrawT(iWidth / 2, 0, Color(255, 255, 255), g::Tahoma, true, debug.c_str());
	}

	if (Config.Watermark)
	{
		int width, height;
		g_pEngine->GetScreenSize(width, height);
	
		static int fps, old_tick_count;

		if ((g_pGlobalVars->tickcount - old_tick_count) > 50) {
			fps = static_cast<int>(1.f / g_pGlobalVars->frametime);
			old_tick_count = g_pGlobalVars->tickcount;
		}
		char FPS;
		int radix = 10;
		static char buffer[256];
		std::stringstream ss;
		static unsigned int last_time;

		/////rainbow sync//////
		last_time = GetTickCount();
		Color rainbow;
		rainbow.FromHSV(fmod(last_time * 0.0002f, 1.f), 1.f, 0.5f);
		//////////////////////

		auto net_channel = g_pEngine->GetNetChannelInfo();
		auto local_player = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));
		std::string incoming = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : "0";
		std::string outgoing = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";

		ss << "t4zCheats" << " | incoming: " << incoming.c_str() << "ms" << " | outgoing: " << outgoing.c_str() << "ms" <<" | fps: " << fps;

		RECT TextSize = g_pSurface->GetTextSizeRect(g::Tahoma, ss.str().c_str());

		g_pSurface->FilledRect(width - (TextSize.right + 20), 25, TextSize.right + 10, 2, rainbow);
		g_pSurface->FilledRect(width - (TextSize.right + 20), 5, TextSize.right + 10, 20, Color(33, 33, 33));
		g_pSurface->DrawT(width - (TextSize.right + 15), 8, Color(200, 200, 200), g::Tahoma, false, ss.str().c_str());
	}

	g_x88Menu.Draw();

	if (g::pLocalEntity && g_pEngine->IsInGame())
	{
		if (Config.Aimbot && g::pLocalEntity->IsAlive())
		{
			C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)g::pLocalEntity->GetActiveWeapon();
			if (pWeapon)
			{
				std::string WeaponName = pWeapon->GetName();
				if (WeaponName == "weapon_glock")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_hkp2000")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_p250")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_tec9")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_elite")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_fiveseven")
					g_Config->SetPistolConf();
				else if (WeaponName == "weapon_deagle")
					g_Config->SetDeagleConf();
				else if (WeaponName == "weapon_nova")
					g_Config->SetScoutConf();
				else if (WeaponName == "weapon_sawedoff")
					g_Config->SetScoutConf();
				else if (WeaponName == "weapon_mag7")
					g_Config->SetScoutConf();
				else if (WeaponName == "weapon_xm1014")
					g_Config->SetScoutConf();
				else if (WeaponName == "weapon_mac10")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_ump45")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_mp9")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_bizon")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_mp7")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_p90")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_mp5sd")
					g_Config->SetSmgConf();
				else if (WeaponName == "weapon_galilar")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_ak47")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_sg556")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_m4a1")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_aug")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_m249")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_negev")
					g_Config->SetRifleConf();
				else if (WeaponName == "weapon_ssg08")
					g_Config->SetScoutConf();
				else if (WeaponName == "weapon_awp")
					g_Config->SetAwpConf();
				else if (WeaponName == "weapon_g3sg1")
					g_Config->SetAutoConf();
				else if (WeaponName == "weapon_scar20")
					g_Config->SetAutoConf();
				else
					g_Config->SetAutoConf();
			}
		}

		if (Config.Crosshair)
		{
			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle();

			int x = iWidth / 2;
			int y = iHeight / 2;
			int dy = iHeight / 90;
			int dx = iWidth / 90;
			x -= (dx * (punchAngle.y));
			y += (dy * (punchAngle.x));

			g_pSurface->DrawT(x + 1, y - 6, Color(255, 255, 255), g::Tahoma, true, "+");
		}
			

		if (Config.Antiaim && Config.DesyncAngle)
		{
			static bool changed_FakeSide = false;
			if (!changed_FakeSide && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.FakeSideHotkey)))
				changed_FakeSide = true;
			else if (changed_FakeSide && !GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.FakeSideHotkey)))
			{
				changed_FakeSide = false;
				if (Config.FakeSide == 0)
					Config.FakeSide = 2;
				else
					Config.FakeSide = 0;
			}
		}

		if (Config.Antiaim && (Config.YawMode == 6 || Config.YawMode == 10) && g::pLocalEntity->IsAlive())
		{
			static bool changed_AASide = false;
			if (!changed_AASide && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.AntiAimHotkey)))
				changed_AASide = true;
			else if (changed_AASide && !GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.AntiAimHotkey)))
			{
				changed_AASide = false;
				if (Config.AntiAimSide == 0)
					Config.AntiAimSide = 2;
				else
					Config.AntiAimSide = 0;
			}

			if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.AntiAimBackHotkey)))
			{
				Config.AntiAimSide = 1;
			}


			if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.BackHotkey)))
			{
				Config.Back = true;
				Config.Right = false;
				Config.Left = false;
			}
			if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.LeftHotkey)))
			{
				Config.Back = false;
				Config.Right = false;
				Config.Left = true;
			}
			if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.RightHotkey)))
			{
				Config.Back = false;
				Config.Right = true;
				Config.Left = false;
			}

			if (g_Menu.Config.Back == true)
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, menu_accent, g::Tahoma, true, "v");
			}
			if (g_Menu.Config.Right == true)
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, menu_accent, g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::Tahoma, true, "v");
			}
	
			if (g_Menu.Config.Left == true)
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, menu_accent, g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::Tahoma, true, "v");
			}


			if (Config.AntiAimSide == 0)
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, menu_accent, g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::Tahoma, true, "v");
			}
			else if (Config.AntiAimSide == 2)
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, menu_accent, g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::Tahoma, true, "v");
			}
			else
			{
				g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, "<");
				g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::Tahoma, true, ">");
				g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, menu_accent, g::Tahoma, true, "v");
			}
		}

		if (Config.indicators)
		{
			if (g::pLocalEntity->IsAlive())
			{
				float desyncAmt = g_AntiAim.MaxDelta(g::pLocalEntity);
				float diffrence = (g::RealAngle.y - g::pLocalEntity->GetLowerBodyYaw());
				float Velocity = g::pLocalEntity->GetVelocity().Length2D();
				int offset = 40;
				Color fake = desyncAmt <= 29 ? Color(255, 0, 0) : (desyncAmt >= 55 ? Color(132, 195, 16) : Color(255 - (desyncAmt * 2.55), desyncAmt * 2.55, 0));
				std::string choke;
				auto NetChannel = g_pEngine->GetNetChannel();

				if (!NetChannel)
					return;

				choke += "choke: " + std::to_string(NetChannel->m_nChokedPackets);
				g_pSurface->DrawT(20, (iHeight - offset - 90), Color(255, 255, 255), g::Indicators, false, choke.c_str());
				
				
				if (diffrence > 35 && Velocity < 0.1f) // we could make multi combo box for this // no -t4zzuu
					g_pSurface->DrawT(20, (iHeight - offset - 60), Color(132, 195, 16), g::Indicators, false, "LBY");
				else
					g_pSurface->DrawT(20, (iHeight - offset - 60), Color(255, 0, 0), g::Indicators, false, "LBY");

				if (!(desyncAmt < 29) && g_Menu.Config.DesyncAngle)
					g_pSurface->DrawT(20, (iHeight - offset - 30), fake, g::Indicators, false, "FAKE");
			}
		}

		if (Config.shotcounter)
		{
			if (g::pLocalEntity->IsAlive())
			{
				std::string shotscounter;
				std::string shotscounter2;
				std::string shotscounter3;
				shotscounter += "shot: " + std::to_string(g::shots);
				shotscounter2 += "hit: " + std::to_string(g::hits);
				shotscounter3 += "miss: " + std::to_string(g::shots - g::hits);
				g_pSurface->DrawT(55, 400, Color(255, 255, 255), g::Indicators, true, shotscounter.c_str());
				g_pSurface->DrawT(55, 430, Color(255, 255, 255), g::Indicators, true, shotscounter2.c_str());
				g_pSurface->DrawT(55, 460, Color(255, 255, 255), g::Indicators, true, shotscounter3.c_str());
			
			}
		}

		if (Config.DrawAimFov) //draw fov
		{
			if (Config.Aimbot && Config.AimbotFov > 0)
			{
				Color FovColor(Config.AimFovColor.red, Config.AimFovColor.green, Config.AimFovColor.blue, 100);
				g_pSurface->OutlineCircle(iWidth / 2, iHeight / 2, Config.AimbotFov * 11, 100, FovColor);
			}
			else if (Config.LegitAimbot && Config.LegitAimFov > 0)
			{
				Color FovColor(Config.AimFovColor.red, Config.AimFovColor.green, Config.AimFovColor.blue, 100);
				g_pSurface->OutlineCircle(iWidth / 2, iHeight / 2, Config.LegitAimFov * 11, 100, FovColor);
			}
		}
	}

	if (menuOpened)
	{
		menu_background.alpha = GlobalOpacity;
		menu_background2.alpha = GlobalOpacity;
		menu_text.alpha = GlobalOpacity;
		menu_accent.alpha = GlobalOpacity;

		if (GetAsyncKeyState(VK_LBUTTON))
			click = true;

		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		Vector2D MousePos = g_pSurface->GetMousePosition();

		if (Dragging && !click)
			Dragging = false;

		if (Dragging && click)
		{
			Pos.x = MousePos.x - dragX;
			Pos.y = MousePos.y - dragY;
		}

		if (g_pSurface->MouseInRegion(Pos.x, Pos.y, Width, 24))
		{
			Dragging = true;
			dragX = MousePos.x - Pos.x;
			dragY = MousePos.y - Pos.y;
		}

		if (Pos.x < 0)
			Pos.x = 0;
		if (Pos.y < 0)
			Pos.y = 0;
		if ((Pos.x + Width) > iScreenWidth)
			Pos.x = iScreenWidth - Width;
		if ((Pos.y + Height) > iScreenHeight)
			Pos.y = iScreenHeight - Height;

		// START MENU
#pragma region Menu

		// Main Background
		g_pSurface->FilledRect(Pos.x, Pos.y, Width, Height, menu_background);
		g_pSurface->OutlinedRect(Pos.x, Pos.y, Width, Height, menu_accent);

		// Header
		g_pSurface->DrawT(Pos.x + (Width / 2), Pos.y + 6, menu_accent, g::SansSerif, true, "t4zCheats");

#pragma endregion



		GroupTabPos[0] = Pos.x + 16;
		GroupTabPos[1] = Pos.y + 55;
		GroupTabPos[2] = Width - 32;
		GroupTabPos[3] = Height - 98;

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);
		GroupTabBottom = GroupTabPos[1] + GroupTabPos[3];

		TabOffset = 0;
		SubTabOffset = 0;
		PreviousControl = -1;
		OldOffsetY = 0;

		Tab("legit");
		{
			CheckBox("Legit Aimbot", &Config.LegitAimbot);
			Slider(100, "Legit Aim Speed", &Config.AimSpeed);
			Slider(15, "Legit Aim Fov", &Config.LegitAimFov);
			KeyBind("Legit Aim Key", &Config.LegitAimKey);
			ComboBox("Legit Aim Hitbox", { "Head", "Chest", "Pelvis" }, &Config.LegitAimHitbox);
			CheckBox("Recoil Control System", &Config.RecoilControlSystem, "Not standalone");
			if (Config.RecoilControlSystem)
			{
				Slider(100, "RCS X", &Config.RCS_X);
				Slider(100, "RCS Y", &Config.RCS_Y);
			}
			CheckBox("Legit Backtracking", &Config.LegitBacktrack, "Uses closest recording tickcount to crosshair as tickcount to backtrack");
		}

		Tab("aimbot");
		{
			SubTab("main");
			{
				CheckBox("Enable", &Config.Aimbot);
				CheckBox("Silent Aim", &Config.SilentAim, "Turns on and off visual aimbot snapping");
				if (!Config.SilentAim)
					CheckBox("Show Recoil", &Config.AimShowRecoil);
				CheckBox("Auto Stop", &Config.Autostop);
				if (Config.Autostop)
				{
					ComboBox("Autostop Mode", { "Fullstop", "Minspeed" , "Custom" }, &Config.AutoStopMode);
					if (Config.AutoStopMode == 2)
						Slider(100, "Custom Autostop Speed", &Config.customstop);
					CheckBox("Stop In Between Shots", &Config.StopInBetweenShots);
				}
				CheckBox("Auto Scope", &Config.AutoScope);
				Slider(180, "Aim Field Of View", &Config.AimbotFov);
				CheckBox("Resolver", &Config.Resolver, "Tries to adjust enemys angles to fix desynced animations");
				CheckBox("Friendly Fire", &Config.FriendlyFire , "Makes aimbot target teammates");
				CheckBox("Position Adjustment", &Config.PosBacktrack);
				CheckBox("Backtrack Lower Body", &Config.LbyBacktrack, "Tries to backtrack the enemys lby, useful when enemy not resolvable");
				CheckBox("Fake Lag Prediction", &Config.FakeLagPrediction , "Tried to predict enemys fakelag");
				CheckBox("On Key Aim (mouse5)", &Config.OnKeyAim);
				ComboBox("Aim Key", { "Mouse 1", "Mouse 2", "Middle Mouse", "Mouse 5", "Mouse 4", "Left Shift", "Left Control", "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow", "C Key", "H Key", "X Key", "V Key" }, &Config.AimHotkey);
				CheckBox("Automatic Revolver", &Config.AutoRevolver , "Automaticly Cogs Resolver");
				CheckBox("Automatic Weapons", &Config.AutomaticWeapons);

				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				ComboBox("Weapon", { "auto", "rifle" , "scout" ,"pistol", "awp" , "heavy" }, &Config.weaponmode);
				if (Config.weaponmode == 0)
				{
					Slider(100, "Auto Minimum Damage", &Config.AutoMD);
					Slider(100, "Auto Hitchance Damage", &Config.AutoHC);
					Slider(100, "Auto Head Scale", &Config.AutoHS);
					Slider(100, "Auto Body Scale", &Config.AutoBS);
				}
				if (Config.weaponmode == 1)
				{
					Slider(100, "Rifle Minimum Damage", &Config.RifleMD);
					Slider(100, "Rifle Hitchance Damage", &Config.RifleHC);
					Slider(100, "Rifle Head Scale", &Config.RifleHS);
					Slider(100, "Rifle Body Scale", &Config.RifleBS);
				}
				if (Config.weaponmode == 2)
				{
					Slider(100, "Scout Minimum Damage", &Config.ScoutMD);
					Slider(100, "Scout Hitchance Damage", &Config.ScoutHC);
					Slider(100, "Scout Head Scale", &Config.ScoutHS);
					Slider(100, "Scout Body Scale", &Config.ScoutBS);
				}
				if (Config.weaponmode == 3)
				{
					Slider(100, "Pistol Minimum Damage", &Config.PistolMD);
					Slider(100, "Pistol Hitchance Damage", &Config.PistolHC);
					Slider(100, "Pistol Head Scale", &Config.PistolHS);
					Slider(100, "Pistol Body Scale", &Config.PistolBS);
				}
				if (Config.weaponmode == 4)
				{
					Slider(150, "Awp Minimum Damage", &Config.AwpMD);
					Slider(100, "Awp Hitchance Damage", &Config.AwpHC);
					Slider(100, "Awp Head Scale", &Config.AwpHS);
					Slider(100, "Awp Body Scale", &Config.AwpBS);
				}
				if (Config.weaponmode == 5)
				{
					Slider(150, "Heavy Minimum Damage", &Config.DeagleMD);
					Slider(100, "Heavy Hitchance Damage", &Config.DeagleHC);
					Slider(100, "Heavy Head Scale", &Config.DeagleHS);
					Slider(100, "Heavy Body Scale", &Config.DeagleBS);
				}

				CheckBox("Head Hitbox", &Config.HB_Head);
				CheckBox("Arms Hitbox", &Config.HB_Arms);
				CheckBox("Chest Hitbox", &Config.HB_Chest);
				CheckBox("Pelvis Hitbox", &Config.HB_Pelvis);
				CheckBox("Thighs Hitbox", &Config.HB_Thighs);
				CheckBox("Feet Hitbox", &Config.HB_Feet);
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab("advanced");
			{
				CheckBox("Baim Lethal", &Config.BaimLethal , "Makes aimbot target body parts when its lethal");
				CheckBox("Baim In Air", &Config.BaimInAir , "Makes aimbot target body parts when enemy is in air");
				Slider(20, "Baim After X Shots", &Config.BaimAfterXShots);
				Slider(20, "Baim After X Missed Shots", &Config.BaimAfterXMissedShots);
			}
		}

		Tab("antiaim");
		{
			CheckBox("Enable", &Config.Antiaim);
			ComboBox("Anti Aim Flags", { "On Ground", "In Air" }, &AntiAimFlag);
			if (AntiAimFlag == 0)
			{
				ComboBox("Yaw", { "Backwards", "Sideways1", "Sideways2", "Switch" , "Spin" , "Freestanding", "Manual", "Fixed", "View", "Legit AA", "Test", "AutoDesync", "dangerzonetest" }, & Config.YawMode);
				if (Config.YawMode == 4)
					Slider(1000, "Spin Speed", &Config.SpeedSpin);
				if (Config.YawMode == 10)
				{

					KeyBind("Anti Aim Switch Key", &Config.AntiAimHotkey);
					KeyBind("Anti Aim Back Key", &Config.AntiAimBackHotkey);
				}
				if (Config.YawMode == 6)
				{
					ComboBox("Manual Mode", { "Mode1", "Mode2" }, &Config.ManualMode);
					if (Config.ManualMode == 1)
					{
						KeyBind("Anti Aim Right Key", &Config.RightHotkey);
						KeyBind("Anti Aim Left Key", &Config.LeftHotkey);
						KeyBind("Anti Aim Back Key", &Config.BackHotkey);
					}
					else
					{
						KeyBind("Anti Aim Switch Key", &Config.AntiAimHotkey);
						KeyBind("Anti Aim Back Key", &Config.AntiAimBackHotkey);
					}
				}

				if (Config.YawMode == 7)
					Slider(360, "Fixed Yaw Value", &Config.FixedCustomYaw);
				Slider(360, "Jitter Range", &Config.JitterRange);
				ComboBox("Jitter Mode", { "Offset", "Random", "Jitter Desync", "Slide" }, &Config.JitterMode);
				if (Config.JitterMode == 3)
					Slider(100, "Slide Speed", &Config.SlideSpeed);
				ComboBox("Pitch", { "Down", "Nazi Down", "Half Down", "Zero", "Up", "Jitter Up/Down", "Custom", "Nospread", "Nospread 2", "View" }, & Config.PitchMode);
				if (Config.PitchMode == 6)
					Slider(180, "Custom Pitch", &Config.CustomPitchValue);
				if (Config.YawMode == 0)
				{
					Slider(360, "Body Lean", &Config.BodyLean);
					Slider(360, "Body Lean Inverted", &Config.BodyLeanInverted);
				}
				CheckBox("Desync Angle", &Config.DesyncAngle);
				if (Config.DesyncAngle)
				{
					if (Config.YawMode != 9)
						ComboBox("Desync Mode", { "Normal", "Experimental", "Maximum" }, &Config.DesyncMode);
					ComboBox("Desync Fake Side Key", { "Mouse 1", "Mouse 2", "Middle Mouse", "Mouse 5", "Mouse 4", "Left Shift", "Left Control", "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow", "C Key", "H Key", "X Key", "V Key" }, &Config.FakeSideHotkey);
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0] + (Width / 2);
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			CheckBox("Lby Breaker", &Config.LbyBreaker, "Breaks Lower Body Yaw");
			if (Config.LbyBreaker)
			{
				Slider(360, "Lby Offset Add", &Config.LbyAdd);
				CheckBox("Lby Static", &Config.StaticLby);
				CheckBox("SendPacket On Break", &Config.SendPacketOnBreak , "Allows to send packets on lby flicks");
			}
			CheckBox("Zero Pitch On Land", &Config.ZeroPitchOnLand);
			CheckBox(Config.Antiaim ? "Slide Fix" : "Slide Walk", &Config.SlideWalk);
			CheckBox("Anti Aim Indicators", &Config.AntiAimIndicator, "Shows angle lines while in thirdperson");
			if (Config.AntiAimIndicator)
				CheckBox("Advanced AA Indicators", &Config.AdvancedAAIndicator);
			ComboBox("On Shot AA", { "Normal", "Fast" }, & Config.OnShotAA);
			CheckBox("Show Server Hitboxes", &Config.ShowServerHitboxes, "Shows your 'real' hitboxes networked by the server");
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);

		Tab("visuals");
		{
			SubTab("esp");
			{
				ComboBox("Entitys", { "Player", "Bomb" }, &EspType); // For switching settings!
				switch (EspType)
				{
				case 0: // Player
					ComboBox("Enable", { "Off","On","Dead" }, &Config.Esp);
					CheckBox("2D Box", &Config.Box);
					ColorPicker("Esp Box Color", Config.BoxColor);
					CheckBox("Health Based Esp", &Config.HbEsp);
					CheckBox("Team Esp", &Config.TeamEsp);
					ComboBox("Show Name", { "Off", "Left Align", "Center" }, & Config.Name);
					ColorPicker("Name Color", Config.NameColor);
					CheckBox("Show Health", &Config.Health);
					ColorPicker("Health Color", Config.HealthColor);
					ComboBox("Show Health Bar", { "Off", "Static Color", "Health Based" }, & Config.HealthBar);
					ColorPicker("Health Bar Color", Config.HealthBarColor);
					CheckBox("Show Money", &Config.Money);
					CheckBox("Ammo Bar", &Config.ammo);
					ColorPicker("Money Color", Config.MoneyColor);
					ComboBox("Show Weapon", { "Off", "Right", "Bottom" }, & Config.Weapon);
					ColorPicker("Weapon Color", Config.WeaponColor);
					CheckBox("Show Yaw/Pitch", &Config.YawPitch);
					ColorPicker("Yaw/Pitch Color", Config.YawPitchColor);

					OffsetY = GroupTabPos[1] + 28;
					ControlsX = GroupTabPos[0] + (Width / 2);
					ControlsX2 = ControlsX + ((Width / 2) - 30);

					CheckBox("Spectator List", &Config.SpecList);
					CheckBox("No Smoke", &Config.NoSmoke);
					CheckBox("No Flash", &Config.NoFlash);
					if (Config.NoFlash)
						Slider(255, "Flash Duration", &Config.FlashDuration);
					CheckBox("Preserve Kill Feed", &Config.PreserveKillFeed);
					CheckBox("Offscreen Esp", &Config.FovEnable);
					ColorPicker("Offscreen Color", Config.FovColor);
					if (Config.FovEnable)
					{
						Slider(255, "OffScreen Distance", &Config.FovSize);
						Slider(30, "OffScreen Size", &Config.FovDistance);
					}
					CheckBox("Visualize Player Hits", &Config.VizualizeHits);
					ColorPicker("Vizualize Hits Color", Config.CapsuleColor);
					CheckBox("Aspect Ratio Changer", &Config.aspectratio);
					if(Config.aspectratio)
						SliderFloat(10.f, "Aspect Ratio Value", &Config.aspectvalue);
					break;
				case 1: // Bomb
					CheckBox("Bomb Esp", &Config.BombEsp);
					break;
				default:
					break;
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab("glow");
			{
				CheckBox("Enemy Glow", &Config.EnemyGlow);
				if (Config.EnemyGlow)
				{
					Slider(255, "Enemy Glow Red", &EnemyColor.red);
					Slider(255, "Enemy Glow Green", &EnemyColor.green);
					Slider(255, "Enemy Glow Blue", &EnemyColor.blue);
					Config.EnemyGlowColor = { (float)EnemyColor.red, (float)EnemyColor.green, (float)EnemyColor.blue };
					Slider(255, "Enemy Glow Opacity", &Config.EnemyGlowOpacity);
				}
				CheckBox("Team Glow", &Config.TeamGlow);
				if (Config.TeamGlow)
				{
					Slider(255, "Team Glow Red", &TeamColor.red);
					Slider(255, "Team Glow Green", &TeamColor.green);
					Slider(255, "Team Glow Blue", &TeamColor.blue);
					Config.TeamGlowColor = { (float)TeamColor.red, (float)TeamColor.green, (float)TeamColor.blue };
					Slider(255, "Team Glow Opacity", &Config.TeamGlowOpacity);
				}

				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				CheckBox("Local Glow", &Config.LocalGlow);
				if (Config.LocalGlow)
				{
					ComboBox("Local Glow Mode", { "Basic", "Pulse", "Outline" }, &Config.LocalGlowMode);
					Slider(255, "Local Glow Red", &LocalColor.red);
					Slider(255, "Local Glow Green", &LocalColor.green);
					Slider(255, "Local Glow Blue", &LocalColor.blue);
					Config.LocalGlowColor = { (float)LocalColor.red, (float)LocalColor.green, (float)LocalColor.blue };
					Slider(255, "Local Glow Opacity", &Config.LocalGlowOpacity);
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab("rendering");
			{
				CheckBox("No Zoom", &Config.NoZoom);
				CheckBox("No Scope", &Config.NoScope);
				CheckBox("No Recoil", &Config.NoRecoil);
				Slider(150, "Fov", &Config.Fov);
				Slider(150, "Thirdperson Fov", &Config.ThirdpersonFov);
				Slider(250, "Viewmodel Fov", &Config.ViewmodelFov);
				CheckBox("Draw Aim Fov Circle", &Config.DrawAimFov);
				ColorPicker("Aim Fov Circle Color", Config.AimFovColor);
				CheckBox("Recoil Crosshair", &Config.Crosshair);
				if (Config.NoRecoil)
					Slider(2.f, "Aimpunch Amount", &Config.AimPunchVal);
				CheckBox("Disable Post Processing", &Config.DisablePostProcess);

				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				CheckBox("Night Mode", &Config.Nightmode);
				if (Config.Nightmode)
				{
					Slider(255, "World Red", &World2Color.red);
					Slider(255, "World Green", &World2Color.green);
					Slider(255, "World Blue", &World2Color.blue);
					Config.WorldColor = { (float)World2Color.red, (float)World2Color.green, (float)World2Color.blue };
					Slider(255, "Prop Red", &Prop2Color.red);
					Slider(255, "Prop Green", &Prop2Color.green);
					Slider(255, "Prop Blue", &Prop2Color.blue);
					Config.PropColor = { (float)Prop2Color.red, (float)Prop2Color.green, (float)Prop2Color.blue };
				}
				CheckBox("Proper Night Mode", &Config.ProperNightmode);
				if (Config.ProperNightmode || Config.Nightmode)
					Slider(255, "Object Blend", &Config.NightmodeBlend);
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab("rendering 2");
			{
				CheckBox("Ambient Light Controller", &Config.AmbientLight);
				if (Config.AmbientLight)
				{
					Slider(255, "Ambient Light Red", &AmbientLight2Color.red);
					Slider(255, "Ambient Light Green", &AmbientLight2Color.green);
					Slider(255, "Ambient Light Blue", &AmbientLight2Color.blue);
					Config.AmbientLightColor = { (float)AmbientLight2Color.red, (float)AmbientLight2Color.green, (float)AmbientLight2Color.blue };
				}

				CheckBox("Trace Bullets", &Config.BulletTracers);
				if (Config.BulletTracers)
				{
					CheckBox("Trace Bullets Ipacts", &Config.Impacts);
					ComboBox("Impact Type", { "Square", "Sphere"}, & Config.ImpactType);
					CheckBox("Trace Enemy Bullets" ,&Config.monkey2);
					ColorPicker("Bullet Tracer Enemy Color", Config.BulletTracerEnemyColor);
					CheckBox("Trace Local Player Bullets", &Config.monkey);
					ColorPicker("Bullet Tracer Local Color", Config.BulletTracerLocalColor);
				}
				CheckBox("Kill Counter", &Config.KillCounter);
			}

			SubTab("chams");
			{
				ComboBox("Chams Model", { "Players", "Viewmodel" }, &Chamstype);
				switch (Chamstype)
				{
				case 0:
					ComboBox("Chams Material", { "Metallic VertexLitGeneric", "Plastic VertexLitGeneric", "Unlit Generic", "Animated" }, &Config.ChamsMaterial);

					CheckBox("Enemy Chams", &Config.EnemyChams);
					if (Config.EnemyChams)
					{
						ColorPicker("Enemy Chams Visible", Config.EnemyVisChamsColor);
						CheckBox("Enemy XQZ Chams", &Config.EnemyXQZChams);
						ColorPicker("Enemy Chams XQZ", Config.EnemyXQZChamsColor);
						ComboBox("BT Chams", { "Off", "Purple->Cyan", "White->Black", "Chams->Transparent", "Last White" }, & Config.BacktrackingChams);
					}

					CheckBox("Team Chams", &Config.TeamChams);
					if (Config.TeamChams)
					{
						ColorPicker("Team Chams Visible", Config.TeamVisChamsColor);
						CheckBox("Team XQZ Chams", &Config.TeamXQZChams);
						ColorPicker("Team Chams XQZ", Config.TeamXQZChamsColor);
					}

					CheckBox("Local Chams", &Config.LocalChams);
					if (Config.LocalChams)
					{
						ColorPicker("Local Chams Visible", Config.LocalVisChamsColor);
						CheckBox("Local XQZ Chams", &Config.LocalXQZChams);
						ColorPicker("Local Chams XQZ", Config.LocalXQZChamsColor);
					}

					if (Config.Antiaim && Config.DesyncAngle)
					{
						CheckBox("Desync Chams/Ghost Chams", &Config.DesyncChams);
						if (Config.DesyncChams)
						{
							ColorPicker("Desync Chams Color", Config.DesyncChamsColor);
							Slider(255, "Desync Chams Transparency", &Config.DesyncChamsTransparency);
						}
					}
					break;
				case 1:
					CheckBox("Viewmodel stuff", &Config.viewmodelchams);
					if (Config.viewmodelchams)
					{
						CheckBox("Hand Chams", &Config.ArmsChams);
						if (Config.ArmsChams)
						{
							ComboBox("Arms Chams Mode", { "Normal","Light" }, &Config.ArmsChamsMode);
							Slider(255, "Arms Chams Red", &ArmsChams2Color.red);
							Slider(255, "Arms Chams Green", &ArmsChams2Color.green);
							Slider(255, "Arms Chams Blue", &ArmsChams2Color.blue);
							Config.ArmsChamsColor = { (float)ArmsChams2Color.red, (float)ArmsChams2Color.green, (float)ArmsChams2Color.blue };
							Slider(255, "Arms Chams Transparency", &Config.ArmsChamsTransparency);
							CheckBox("Arms Chams WireFrame", &Config.ArmsWireFrame);
						}
						
						CheckBox("Weapon Chams", &Config.WeaponChams);
						if (Config.WeaponChams)
						{
							Slider(255, "Weapon Chams Red", &WeaponChams2Color.red);
							Slider(255, "Weapon Chams Green", &WeaponChams2Color.green);
							Slider(255, "Weapon Chams Blue", &WeaponChams2Color.blue);
							Config.WeaponChamsColor = { (float)WeaponChams2Color.red, (float)WeaponChams2Color.green, (float)WeaponChams2Color.blue };
						}
					}
					break;
				default:
					break;
				}
			}
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);

		Tab("misc");
		{
			SubTab("first");
			{
				ComboBox("Fake Lag Type", { "Off","Factor", "Adaptive", "Switch" }, & Config.FakeLagType);
				Slider(20, "Fake Lag Value On Ground", &Config.FakeLagValue);
				Slider(20, "Fake Lag Value In Air", &Config.FakeLagValueGround);
				CheckBox("Show FakeLag", &Config.ShowLag);
				ColorPicker("Show FakeLag Color", Config.ShowLagColor);
				CheckBox("Slow Walk", &Config.SlowWalk);

				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				if (Config.SlowWalk)
				{
					ComboBox("Slow Walk Type", { "Hitchance Based", "Weapon Based", "Custom" }, &Config.SlowWalkType);
					if (Config.SlowWalkType == 2)
						Slider(100, "Custom Speed", &Config.SlowWalkAmount);
					CheckBox("Stop Leg Animations", &Config.StopLegAnim);
				}
				CheckBox("Hitmarker", &Config.VisHitmarker);
				if (Config.VisHitmarker)
					CheckBox("Show Dmg", &Config.ShowDmg);
				CheckBox("Log Damage", &Config.LogDamage);
				CheckBox("Crouch Block", &Config.blockbotonhead);
				if(Config.blockbotonhead)
					KeyBind("^ Key", &Config.BlockBotKey);
				CheckBox("Buy Bot", &Config.BuyBot.enable);
				if (Config.BuyBot.enable)
				{
					ComboBox("Primary Weapon", { "none","auto","awp", "scout"}, &Config.BuyBot.main);
					ComboBox("Secondary Weapon", { "none","heavy", "dualies" }, & Config.BuyBot.second);
					ComboBox("Armor", { "none", "full" }, & Config.BuyBot.armor);
					ComboBox("Grenades", { "none", "all" }, & Config.BuyBot.grenade);
				}
				Button("Hide Name", &InvisName);
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab("second");
			{
				ComboBox("Hitsound", { "None","Metalic", "Metalic2", "Beep1", "Beep2", "Bell", "Headshot", "Headshot2", "Fatality1", "Fatality2", "Cod", "Laser" }, & Config.Hitmarker);
				KeyBind("Thirdperson Hotkey", &Config.ThirdpersonHotkey);
				ComboBox("Clantag Spammer", { "Off", "Static", "Rotate", "gamesense", "Nasa Technology", "Software.gov", "x88Cheats" }, & Config.ClanTagMode);
				CheckBox("Override Viewmodel", &Config.OverrideViewmodel);
				if (Config.OverrideViewmodel)
				{
					Slider(60, "Viewmodel X", &Config.ViewmodelOffsetX);
					Slider(60, "Viewmodel Y", &Config.ViewmodelOffsetY);
					Slider(60, "Viewmodel Z", &Config.ViewmodelOffsetZ);
					Slider(360, "Roll", &Config.roll);
				}
				CheckBox("Knife Left Side", &Config.KnifeLeftSide);
				CheckBox("Key Visualization", &Config.KeyVisualizer);
				CheckBox("Bunnyhop", &Config.Bhop);
				CheckBox("Auto Strafe", &Config.AutoStrafe);
				CheckBox("Quick Bomb Plant", &Config.quickerplant);
				CheckBox("namestealer", &Config.namestealer);

				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				CheckBox("Edge Jump", &Config.EdgeJump);
				KeyBind("Edge Jump Key", &Config.EdgeJumpKey);
				CheckBox("Indicators", &Config.indicators);
				CheckBox("Shot Counter", &Config.shotcounter);
				CheckBox("Disable Panorama Blur", &Config.DisablePanoramaBlur);
				CheckBox("Watermark", &Config.Watermark);
				CheckBox("Anti Client Crasher", &Config.AntiClientCrasher, "Prevents users from eploiting the invite system to crash your CS:GO");
			}
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);

		Tab("config");
		{
			static bool overridemenucolor;
			CheckBox("Override Accent", &overridemenucolor);
			if (overridemenucolor)
			{
				Slider(255, "Accent Red", &menu_accent.red);
				Slider(255, "Accent Green", &menu_accent.green);
				Slider(255, "Accent Blue", &menu_accent.blue);
				Config.MenuColor = { (float)menu_accent.red, (float)menu_accent.green, (float)menu_accent.blue };
			}
			CheckBox("Draw Top Info Bar", &Config.TopInfoBar);
			CheckBox("x88 Menu Test", &Config.x88menu);

			Space(10);

			ComboBox("Config", { "Legit 1", "Legit 2", "Semi Rage", "HvH 1", "HvH 2" }, &Config.CurrentConfig);

			Space(5);

			Button("Save", &save);
			Button("Load", &load);

			Button("Reset Shot Counter", &reset);
			OffsetY = GroupTabPos[1] + 28;
			ControlsX = GroupTabPos[0] + (Width / 2);
			ControlsX2 = ControlsX + ((Width / 2) - 30);


			CheckBox("Knife Changer", &Config.knivchanger);
			ComboBox("Knife Model", { "Bayonet","Flip Knife","Gut Knife","Karambit","M9 Bayonet","Huntsman Knife","Butterfly Knife","Falchion Knife","Shadow Daggers","Bowie Knife","Navaja Knife","Stiletto Knife","Ursus Knife","Talon Knife" } , &Config.Knife);
			ComboBox("Knife Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, &Config.KnifeSkin);
			ComboBox("Scar Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, & Config.Skinchanger.SCAR20Skin);
			ComboBox("G3SG1 Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, & Config.Skinchanger.G3sg1Skin);
			ComboBox("SSG08 Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, & Config.Skinchanger.SSG08Skin);
			ComboBox("Dual Berretas Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, & Config.Skinchanger.DualSkin);
			ComboBox("R8 Skin", { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler Phase 1","Doppler Phase 2","Doppler Phase 3","Doppler Phase 4","Gamma Doppler Phase 1","Gamma Doppler Phase 2","Gamma Doppler Phase 3","Gamma Doppler Phase 4","Gamma Doppler Emerald","Lore","Black Laminate","Autotronic","Freehand","Doppler_1new","Doppler_2new","Doppler_3new","Doppler_4new","Marble Fade_new","Damascus Steel_1new","Damascus Steel_2new" }, & Config.Skinchanger.RevolverSkin);
			Button("Update", &forceupdate);
		}

		TabSize = TabOffset;
		SubTabSize = SubTabOffset;
	}
}

Color Shade(Color clr, float factor)
{
	Color newcolor(255, 255, 255);
	newcolor.red = clr.red * (1 - factor);
	newcolor.green = clr.green * (1 - factor);
	newcolor.blue = clr.blue * (1 - factor);

	return newcolor;
}

void Menu::Tab(std::string name)
{
	int height = 25;
	int width = (MenuWidth - 2) / 6;
	int TabArea[4] = {Pos.x + (TabOffset * width), Pos.y + 24, width, height};
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(TabArea[0], TabArea[1], TabArea[2], TabArea[3]))
		TabNum = TabOffset;

	if (TabOffset == TabNum)
	{
		g_pSurface->RoundedFilledRect(TabArea[0] + (TabArea[2] / 2) - 25, TabArea[1] + 24, 50, 10, 5, menu_accent);
		g_pSurface->FilledRect(TabArea[0] + (TabArea[2] / 2) - 25, TabArea[1] + 29, 50, 5, menu_background);
		g_pSurface->DrawT(TabArea[0] + (TabArea[2] / 2), TabArea[1] + 6, menu_text, g::SansSerifBold, true, name.c_str());
	}
	else
	{
		g_pSurface->DrawT(TabArea[0] + (TabArea[2] / 2), TabArea[1] + 6, menu_text, g::SansSerif, true, name.c_str());
	}

	TabOffset += 1;
	PreviousControl = -1;
}

void Menu::SubTab(std::string name)
{
	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	static int TabSkip = 0;

	if (SubTabSize != 0 && TabSkip == TabNum)
	{

		int TabLength = (GroupTabPos[2] / SubTabSize);

		int GroupTabArea[4] = { (GroupTabPos[0]) + (TabLength * SubTabOffset), GroupTabPos[1], TabLength, 20 };

		if ((GroupTabArea[0] + GroupTabArea[3]) <= (GroupTabPos[0] + GroupTabPos[2]))
		{
			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(GroupTabArea[0], GroupTabArea[1], GroupTabArea[2], GroupTabArea[3]))
				SubTabNum = SubTabOffset;

			int Offset = ((SubTabSize - 1) == SubTabOffset) ? 0 : 1;

			if (((SubTabSize - 1) == SubTabOffset) && (((TabLength * SubTabSize) > GroupTabPos[2]) || ((TabLength * SubTabSize) < GroupTabPos[2])))
				Offset = (GroupTabPos[2] - (TabLength * SubTabSize));

			if (SubTabNum == SubTabOffset)
			{
				g_pSurface->FilledRect(GroupTabArea[0], GroupTabArea[1], GroupTabArea[2], GroupTabArea[3], menu_background2);
			}

			g_pSurface->DrawT(GroupTabArea[0] + (GroupTabArea[2] / 2), GroupTabArea[1] + 4, menu_text, g::SansSerif, true, name.c_str());
		}
	}

	if (TabSkip != TabNum) // frame skip for drawing
		TabSkip = TabNum;

	if (SubTabOffset == SubTabNum)
		OffsetY += 20;

	SubTabOffset += 1;
	PreviousControl = -1;
}

void Menu::CheckBox(std::string name, bool* item, std::string tooltip)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	static bool pressed = false;

	if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX2 - 24, OffsetY + 6, 24, 12))
	{
		if (pressed)
			*item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX2 - 24, OffsetY + 6, 24, 12) && !pressed)
		pressed = true;

	g_pSurface->FilledRect(ControlsX2 - 24, OffsetY + 6, 24, 12, menu_background2);

	if (*item == true)
		g_pSurface->FilledRect(ControlsX2 - 12, OffsetY + 6, 12, 12, menu_accent);
	else
		g_pSurface->FilledRect(ControlsX2 - 24, OffsetY + 6, 12, 12, Shade(menu_accent, 0.6));

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());

	if (g_pSurface->MouseInRegion(ControlsX2 - 24, OffsetY + 6, 24, 12))
	{
		g_pSurface->OutlinedRect(ControlsX2 - 26, OffsetY + 4, 28, 16, Color(255, 255, 255));

		if (!tooltip.empty())
		{
			RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, tooltip.c_str());
			Vector2D MousePos = g_pSurface->GetMousePosition();

			g_pSurface->FilledRect(MousePos.x + 10, MousePos.y + 10, 32 + TextSize.right, 24, menu_background);
			g_pSurface->OutlinedRect(MousePos.x + 10, MousePos.y + 10, 32 + TextSize.right, 24, menu_accent);

			g_pSurface->DrawT(MousePos.x + 26, MousePos.y + 16, menu_text, g::SansSerif, false, tooltip.c_str());
		}
	}

	OldOffsetY = OffsetY;
	OffsetY += 24;
	PreviousControl = check_box;
}

void Menu::Slider(int max, std::string name, int* item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, std::to_string(*item).c_str());

	int width = ControlsX2 - ControlsX;

	float pixelValue = max / 265.f;

	g_pSurface->DrawT(ControlsX - 8, OffsetY + 22, menu_text, g::Tahoma, false, "-");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 8, OffsetY + 22, 4, 8))
		*item -= 1;

	g_pSurface->DrawT(ControlsX + width + 4, OffsetY + 22, menu_text, g::Tahoma, false, "+");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + width + 4, OffsetY + 22, 4, 8))
		* item += 1;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 2, OffsetY + 24, width, 8))
		*item = (g_pSurface->GetMousePosition().x - (ControlsX - 2)) * pixelValue;

	if (*item > max)
		*item = max;
	if (*item < 0)
		*item = 0;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());
	g_pSurface->DrawT(ControlsX2 - TextSize.right, OffsetY + 6, menu_text, g::SansSerif, true, std::to_string(*item).c_str());

	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, width, 8, 4, menu_background2);
	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, *item / pixelValue, 8, 4, menu_accent);
	g_pSurface->FilledRect((ControlsX + (*item / pixelValue)) - 3, OffsetY + 22, 6, 12, Color(255, 255, 255));

	OldOffsetY = OffsetY;
	OffsetY += 36;
	PreviousControl = slider;
}

void Menu::SliderFloat(float max, std::string name, float* item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, std::to_string(*item).c_str());

	int width = ControlsX2 - ControlsX;

	float pixelValue = max / 265.f;

	g_pSurface->DrawT(ControlsX - 8, OffsetY + 22, menu_text, g::Tahoma, false, "-");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 8, OffsetY + 22, 4, 8))
		* item -= 1;

	g_pSurface->DrawT(ControlsX + width + 4, OffsetY + 22, menu_text, g::Tahoma, false, "+");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + width + 4, OffsetY + 22, 4, 8))
		* item += 1;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 2, OffsetY + 24, width, 8))
		* item = (g_pSurface->GetMousePosition().x - (ControlsX - 2)) * pixelValue;

	if (*item > max)
		* item = max;
	if (*item < 0.f)
		* item = 0.f;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());
	g_pSurface->DrawT(ControlsX2 - TextSize.right, OffsetY + 6, menu_text, g::SansSerif, true, std::to_string(*item).c_str());

	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, width, 8, 4, menu_background2);
	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, *item / pixelValue, 8, 4, menu_accent);
	g_pSurface->FilledRect((ControlsX + (*item / pixelValue)) - 3, OffsetY + 22, 6, 12, Color(255, 255, 255));

	OldOffsetY = OffsetY;
	OffsetY += 36;
	PreviousControl = sliderfloat;
}

void Menu::ComboBox(std::string name, std::vector< std::string > itemname, int* item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 153, OffsetY, 100, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 153, OffsetY, 100, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	int arrowpos = 240;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());

	if (g_pSurface->MouseInRegion(ControlsX + 153, OffsetY + 2, 100, 16))
	{
		g_pSurface->OutlinedRect(ControlsX + 151, OffsetY, 104, 20, Color(255, 255, 255));
	}
	g_pSurface->FilledRect(ControlsX + 153, OffsetY + 2, 100, 16, menu_background2);
	g_pSurface->FilledRect(ControlsX + arrowpos + 4, OffsetY + 2 + 7, 5, 1, menu_background2);
	g_pSurface->FilledRect(ControlsX + arrowpos + 5, OffsetY + 2 + 8, 3, 1, menu_background2);
	g_pSurface->FilledRect(ControlsX + arrowpos + 6, OffsetY + 2 + 9, 1, 1, menu_background2);

	if (open)
	{
		g_pSurface->FilledRect(ControlsX + 153, OffsetY + 2, 100, 17 + (itemname.size() * 16), menu_background2);
		g_pSurface->FilledRect(ControlsX + 153, OffsetY + 2, 100, 16, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 4, OffsetY + 2 + 7, 5, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 5, OffsetY + 2 + 8, 3, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 6, OffsetY + 2 + 9, 1, 1, menu_background2);

		for (int i = 0; i < itemname.size(); i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 153, OffsetY + 2 + 16 + i * 16, 100, 16))
				*item = i;

			if (*item == i)
				g_pSurface->FilledRect(ControlsX + 153, OffsetY + 2 + 16 + (i * 16), 100, 20, menu_background2);

			g_pSurface->DrawT(ControlsX + 159, OffsetY + 2 + 16 + (i * 16), menu_text, g::SansSerif, false, itemname.at(i).c_str());
		}
	}

	g_pSurface->DrawT(ControlsX + 159, OffsetY + 2, menu_text, g::SansSerif, false, itemname.at(*item).c_str());

	OldOffsetY = OffsetY;

	if (open)
		OffsetY += 24 + (itemname.size() * 16);
	else
		OffsetY += 24;

	PreviousControl = combo_box;
}

void Menu::ColorPicker(std::string name, ColorV2& item) // best coder in the universe
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	if (PreviousControl == slider || PreviousControl == -1)
		return;

	int CtrXoffset = 0;

	if (PreviousControl != check_box)
		CtrXoffset = 132;
	else
		CtrXoffset = 165;

	int yoffset = OldOffsetY + 6;
	int xoffset = ControlsX + 330;

	Color rainbow;

	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + CtrXoffset, OldOffsetY, 16, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + CtrXoffset, OldOffsetY, 16, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	if (open)
	{
		g_pSurface->RoundedFilledRect(xoffset, OldOffsetY, 100, 20, 5, Color(0, 0, 0, 255));
		g_pSurface->RoundedFilledRect(xoffset, OldOffsetY + 100, 100, 20, 5, Color(255, 255, 255, 255));

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(xoffset, OldOffsetY, 100, 10))
		{
			item.red = 0;
			item.green = 0;
			item.blue = 0;
			item.alpha = 255;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(xoffset, OldOffsetY + 110, 100, 10))
		{
			item.red = 255;
			item.green = 255;
			item.blue = 255;
			item.alpha = 255;
		}

		for (int i = 0; i < 100; i++)
		{
			if (xoffset >= ControlsX + 430)
			{
				xoffset -= 100;
				yoffset += 10;
			}

			float hue = (i * .01f);

			rainbow.FromHSV(hue, 1.f, 1.f);

			g_pSurface->FilledRect(xoffset, yoffset, 10, 10, rainbow);

			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(xoffset, yoffset, 10, 10))
			{
				item.red = rainbow.red;
				item.green = rainbow.green;
				item.blue = rainbow.blue;
				item.alpha = 255.f;
			}

			xoffset += 10;
		}
	}

	rainbow.red = item.red;
	rainbow.green = item.green;
	rainbow.blue = item.blue;
	rainbow.alpha = 255;

	g_pSurface->FilledRect(ControlsX + CtrXoffset, OldOffsetY, 16, 12, rainbow);
	g_pSurface->OutlinedRect(ControlsX + CtrXoffset, OldOffsetY, 16, 12, menu_background2);
}

void Menu::KeyBind(std::string name, int* item) // needs to be working with mouse keys pls fix
{
	if (ControlsReturn)
		return;

	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	static bool gotKey = false;
	bool pressed = false;
	if (gotKey)
		pressed = true;
	gotKey = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 153, OffsetY, 140, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 153, OffsetY, 140, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	g_pSurface->DrawT(ControlsX + 6, OffsetY, Color(255, 255, 255, 255), g::SansSerif, false, name.c_str());
	g_pSurface->AutoGradientRect(ControlsX + 153, OffsetY, 140, 16, Color(119, 119, 119, 255));

	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if ((GetAsyncKeyState(i) & 0x8000) && i != 1 && i != 45)
			{
				*item = i;
				gotKey = true;
			}
		}
		g_pSurface->AutoGradientRect(ControlsX + 153, OffsetY, 140, 16, Color(53, 53, 53, 255));
	}

	g_pSurface->DrawT(ControlsX + 159, OffsetY, Color(255, 255, 255, 255), g::SansSerif, false, std::string(1, char(*item)).c_str()); // K LOL

	OldOffsetY = OffsetY;
	OffsetY += 18;

	PreviousControl = key_bind;
}


void Menu::Button(std::string text, Menu::ButtonCallback_t callback)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	bool pressed = false;
	static bool clickRest;
	static bool rest;

	int height = 20;
	RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, text.c_str());
	int width = 16 + TextSize.right;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX, OffsetY + 6, width, height) && !clickRest)
	{
		pressed = true;
		clickRest = true;

		g_pSurface->FilledRect(ControlsX, OffsetY + 6, width, height, menu_accent);
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX, OffsetY + 5, width, height))
		clickRest = false;

	if (g_pSurface->MouseInRegion(ControlsX, OffsetY + 6, width, height))
	{
		g_pSurface->OutlinedRect(ControlsX - 2, OffsetY + 4, width + 4, height + 4, Color(255, 255, 255));
	}

	if (pressed)
	{
		if (!rest)
			callback();

		rest = true;
	}
	else
	{
		rest = false;
		g_pSurface->FilledRect(ControlsX, OffsetY + 6, width, height, menu_accent);
	}

	g_pSurface->DrawT(ControlsX + (width / 2) - 2, OffsetY + 8, menu_text, g::SansSerif, true, text.c_str());

	OldOffsetY = OffsetY;

	OffsetY += 32;

	PreviousControl = button;
}

void Menu::Space(int spaceval)
{
	OffsetY += spaceval;
	PreviousControl = spacer;
}

bool keys[256];
bool oldKeys[256];

bool GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	return false;
}

void Menu::TextBox(std::string name, std::string* text)
{
	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	if (GroupTabBottom <= OffsetY + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}


	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 225, OffsetY + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 225, OffsetY + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}

	const char* strg = text->c_str();
	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					open = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					*text = text->substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 6 && (i == 0x30 || i == 0x31 || i == 0x32 || i == 0x33 || i == 0x34 || i == 0x35 || i
					== 0x36 || i == 0x37 || i == 0x38 || i == 0x39 || i == VK_NUMPAD0 || i == VK_NUMPAD1 || i ==
					VK_NUMPAD2 || i == VK_NUMPAD3 || i == VK_NUMPAD4 || i == VK_NUMPAD5 || i == VK_NUMPAD6 || i ==
					VK_NUMPAD7 || i == VK_NUMPAD8 || i == VK_NUMPAD9))
				{
					*text = *text + g_Hotkeys.KeyDigits[i];
					return;
				}
			}
		}
	}

	const char* cstr = text->c_str();
	g_pSurface->FilledRect(ControlsX + 20, OffsetY, ComboSize, 20, button_dark);
	g_pSurface->OutlinedRect(ControlsX + 20, OffsetY, ComboSize, 20, menu_text);
	g_pSurface->DrawT(ControlsX + 20 + 5, OffsetY + 3, menu_text, g::SansSerif, false, cstr);

	OffsetY += 29;
}

void Menu::Text(std::string text)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;
	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, text.c_str());
	OffsetY += 24;
	PreviousControl = label;
}