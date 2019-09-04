#include "Menu.h"
#include "Config.h"

void CConfig::Setup()
{
	//////LEGIT AIMBOT//////
	//Legit
	SetupValue(g_Menu.Config.LegitAimbot, false, "legitaimbot");
	SetupValue(g_Menu.Config.LegitAimKey, 0, "legitaimkey");
	SetupValue(g_Menu.Config.LegitBacktrack, false, "legitbacktrack");
	SetupValue(g_Menu.Config.RecoilControlSystem, false, "rcs");
	SetupValue(g_Menu.Config.RCS_X, 0, "rcsx");
	SetupValue(g_Menu.Config.RCS_Y, 0, "rcsy");
	SetupValue(g_Menu.Config.LegitAimFov, 0, "legitaimbotfov");
	SetupValue(g_Menu.Config.AimSpeed, 0, "legitaimbotspeed");
	SetupValue(g_Menu.Config.LegitAimHitbox, 0, "legitaimbothitbox");
	SetupValue(g_Menu.Config.LegitAA, false, "legitaa");

	//////AIMBOT//////
	SetupValue(g_Menu.Config.Aimbot, false, "aimbot");
	SetupValue(g_Menu.Config.SilentAim, true, "slientaim");
	SetupValue(g_Menu.Config.AimShowRecoil, false, "aimshowrecoil");
	SetupValue(g_Menu.Config.AimMode, 0, "aimmode");
	SetupValue(g_Menu.Config.AimbotFov, 0, "aimfov");
	SetupValue(g_Menu.Config.OnKeyAim, false, "onkeyaim");
	SetupValue(g_Menu.Config.AimHotkey, 3, "aimhotkey");
	SetupValue(g_Menu.Config.FriendlyFire, false, "friendlyfire");
	SetupValue(g_Menu.Config.Hitchance, 1, "hitchance");
	SetupValue(g_Menu.Config.HitchanceValue, 30, "hitchanceval");
	SetupValue(g_Menu.Config.Mindmg, 25, "mindmg");
	SetupValue(g_Menu.Config.Resolver, false, "resolver");
	SetupValue(g_Menu.Config.ResolverMode, 0, "resolvermode");
	SetupValue(g_Menu.Config.BodyScale, 100, "bodyscale");
	SetupValue(g_Menu.Config.HeadScale, 100, "headscale");
	SetupValue(g_Menu.Config.MultiPoint, true, "multipoint");
	SetupValue(g_Menu.Config.ForceBaim, false, "forcebaim");
	SetupValue(g_Menu.Config.BaimAfterXShots, 0, "baimafterxshots");
	SetupValue(g_Menu.Config.BaimAfterXMissedShots, 0, "baimafterxmissedshots");
	SetupValue(g_Menu.Config.DelayShot, true, "delayshot");
	SetupValue(g_Menu.Config.IgnoreLimbs, false, "ignorelimbs");
	SetupValue(g_Menu.Config.Autostop, false, "autostop");
	SetupValue(g_Menu.Config.AutoStopMode, 0, "autostopmode");
	SetupValue(g_Menu.Config.customstop, 0, "customautostopvalue");
	SetupValue(g_Menu.Config.AutoScope, false, "autoscope");
	SetupValue(g_Menu.Config.AutoRevolver, false, "autorevolver");
	SetupValue(g_Menu.Config.AutomaticWeapons, false, "automaticweapons");
	SetupValue(g_Menu.Config.PosBacktrack, false, "posbacktrk");
	SetupValue(g_Menu.Config.LbyBacktrack, false, "lbybacktrk");
	SetupValue(g_Menu.Config.BaimLethal, true, "baimlethal");
	SetupValue(g_Menu.Config.BaimPitch, true, "baimpitch");
	SetupValue(g_Menu.Config.BaimInAir, false, "baiminair");
	SetupValue(g_Menu.Config.StopInBetweenShots, false, "shopstop");
	SetupValue(g_Menu.Config.FakeLagPrediction, false, "fakepred");


	//////HITBOXES//////
	SetupValue(g_Menu.Config.HB_Head, true, "hb_head");
	SetupValue(g_Menu.Config.HB_Chest, true, "hb_chest");
	SetupValue(g_Menu.Config.HB_Arms, true, "hb_arms");
	SetupValue(g_Menu.Config.HB_Thorax, true, "hb_thorax");
	SetupValue(g_Menu.Config.HB_Pelvis, true, "hb_pelvis");
	SetupValue(g_Menu.Config.HB_Thighs, true, "hb_thighs");
	SetupValue(g_Menu.Config.HB_Feet, true, "hb_feet");

	//////ANTI AIM//////
	SetupValue(g_Menu.Config.Antiaim, false, "antiaim");
	SetupValue(g_Menu.Config.AntiAimMode, 0, "antiaimmode");
	SetupValue(g_Menu.Config.AntiAimSide, 1, "antiaimside");
	SetupValue(g_Menu.Config.AntiAimHotkey, 3, "antiaimhotkey");
	SetupValue(g_Menu.Config.FakeSide, 1, "fakeside");
	SetupValue(g_Menu.Config.FakeSideHotkey, 11, "fakesidehotkey");
	SetupValue(g_Menu.Config.AntiAimBackHotkey, 4, "antiaimbackhotkey");
	SetupValue(g_Menu.Config.PitchMode, 0, "pitchmode");
	SetupValue(g_Menu.Config.YawMode, 0, "yawmode");
	SetupValue(g_Menu.Config.CustomPitchValue, 0, "custompitchvalue");
	SetupValue(g_Menu.Config.FixedCustomYaw, 0, "fixedcustomyaw");
	SetupValue(g_Menu.Config.AntiAimKeys, false, "antiaimkeys");
	SetupValue(g_Menu.Config.RealAngles, true, "realangles");
	SetupValue(g_Menu.Config.DesyncAngle, true, "desyncang");
	SetupValue(g_Menu.Config.DesyncMode, 0, "desyncmode");
	SetupValue(g_Menu.Config.ManualMode, 0, "ManualMode");
	SetupValue(g_Menu.Config.LbyBreaker, true, "lbybreaker");
	SetupValue(g_Menu.Config.Lbymode, 0, "lbymode");
	SetupValue(g_Menu.Config.LbyMult, 0, "lbymultiplier");
	SetupValue(g_Menu.Config.LbyAdd, 0, "lbyadd");
	SetupValue(g_Menu.Config.SpeedSpin, 0, "spinspeed");
	SetupValue(g_Menu.Config.JitterRange, 0, "jitterrange");
	SetupValue(g_Menu.Config.JitterMode, 0, "jittermode");
	SetupValue(g_Menu.Config.SlideSpeed, 10, "slidespeed");
	SetupValue(g_Menu.Config.ZeroPitchOnLand, false, "zeropitchonland");
	SetupValue(g_Menu.Config.CustomPitchOnLand, false, "custompitchonland");
	SetupValue(g_Menu.Config.LandPitch, 0, "landpitch");
	SetupValue(g_Menu.Config.AntiAimIndicator, false, "antiaimindicator");
	SetupValue(g_Menu.Config.AdvancedAAIndicator, false, "advancedaaindicator");
	SetupValue(g_Menu.Config.SlideWalk, false, "slidewalk");
	SetupValue(g_Menu.Config.OnShotAA, false, "onshotaa");
	SetupValue(g_Menu.Config.BodyLean, 0, "bodylean");
	SetupValue(g_Menu.Config.BodyLeanInverted, 0, "bodylean2");
	SetupValue(g_Menu.Config.StaticLby, false, "staticlby");

	//////SEND PACKET STUFF//////

	SetupValue(g_Menu.Config.FakeLagType, 0, "fakelagtype");
	SetupValue(g_Menu.Config.FakeLagValue, 0, "fakelagia");
	SetupValue(g_Menu.Config.FakeLagValueGround, 0, "fakelagog");
	SetupValue(g_Menu.Config.BreakLagComp, false, "breaklagcomp");
	SetupValue(g_Menu.Config.ChokeShotOnPeek, false, "chokeshotonpeek");
	SetupValue(g_Menu.Config.FakeWalk, false, "fakewalk");
	SetupValue(g_Menu.Config.NasaWalk, false, "nasawalk");
	SetupValue(g_Menu.Config.NasaWalkKey, 11, "nasawalkkey");
	SetupValue(g_Menu.Config.FakeWalkChoke, 5, "fakewalkchoke");
	SetupValue(g_Menu.Config.FakeDuck, false, "fakeduck");
	SetupValue(g_Menu.Config.FakeDuckKey, 11, "fakeduckkey");
	SetupValue(g_Menu.Config.PingSpike, false, "pingspike");
	SetupValue(g_Menu.Config.PingValue, 100, "pingvalue");
	SetupValue(g_Menu.Config.ShowServerHitboxes, false, "showserverhitboxes");

	//////ESP//////
	SetupValue(g_Menu.Config.Esp, 0, "esp");
	SetupValue(g_Menu.Config.TeamEsp, false, "teamesp");
	SetupValue(g_Menu.Config.SkeletonEsp, false, 2, "skeletonesp");
	SetupValue(g_Menu.Config.SkeletonColor, 255.f, 255.f, 255.f, "skeletoncolor");
	SetupValue(g_Menu.Config.HbEsp, false, "hbesp");
	
	SetupValue(g_Menu.Config.Name, 2, "name");
	SetupValue(g_Menu.Config.YawPitch, false, "yawpitch");
	SetupValue(g_Menu.Config.Money, false, "money");
	SetupValue(g_Menu.Config.Health, false, "health");
	SetupValue(g_Menu.Config.HealthBar, 2, "healthbar");
	SetupValue(g_Menu.Config.Weapon, 2, "weapon");
	SetupValue(g_Menu.Config.ammo, 2, "ammbar");
	SetupValue(g_Menu.Config.Box, true, "box");
	SetupValue(g_Menu.Config.FovEnable, true, "fovenable");
	SetupValue(g_Menu.Config.FovAlways, true, "fovalways");
	SetupValue(g_Menu.Config.FovColor, 255.f, 255.f, 255.f, "fovcolor");
	SetupValue(g_Menu.Config.FovDistance, 10, "Fov Distance");
	SetupValue(g_Menu.Config.FovSize, 10, "Fov Size");

	SetupValue(g_Menu.Config.NameColor, 255.f, 255.f, 255.f, "namecolor");
	SetupValue(g_Menu.Config.HealthColor, 255.f, 255.f, 255.f, "healthcolor");
	SetupValue(g_Menu.Config.HealthBarColor, 255.f, 255.f, 255.f, "healthbarcolor");
	SetupValue(g_Menu.Config.WeaponColor, 255.f, 255.f, 255.f, "weaponcolor");
	SetupValue(g_Menu.Config.YawPitchColor, 255.f, 255.f, 255.f, "yawpitchcolor");
	SetupValue(g_Menu.Config.MoneyColor, 108.f, 192.f, 18.f, "moneycolor");
	SetupValue(g_Menu.Config.BoxColor, 255.f, 255.f, 255.f, "boxcolor");

	//////GLOW//////
	SetupValue(g_Menu.Config.EnemyGlow, false, "enemyglow");
	SetupValue(g_Menu.Config.EnemyGlowColor, 255.f, 255.f, 255.f, "enemyglowcolor");
	SetupValue(g_Menu.Config.EnemyGlowOpacity, 255.f, "enemyglowopacity");

	SetupValue(g_Menu.Config.TeamGlow, false, "teamglow");
	SetupValue(g_Menu.Config.TeamGlowColor, 255.f, 255.f, 255.f, "teamglowcolor");
	SetupValue(g_Menu.Config.TeamGlowOpacity, 255.f, "teamglowopacity");

	SetupValue(g_Menu.Config.LocalGlow, false, "localglow");
	SetupValue(g_Menu.Config.LocalGlowMode, 1, "localglowmode");
	SetupValue(g_Menu.Config.LocalGlowColor, 255.f, 255.f, 255.f, "localglowcolor");
	SetupValue(g_Menu.Config.LocalGlowOpacity, 255.f, "localglowopacity");

	//////WORLD & COLORS//////
	SetupValue(g_Menu.Config.Nightmode, false, "nightmode");
	SetupValue(g_Menu.Config.ProperNightmode, false, "propernightmode");
	SetupValue(g_Menu.Config.WorldColor, 50.f, 50.f, 50.f, "worldcolor");
	SetupValue(g_Menu.Config.PropColor, 50.f, 50.f, 50.f, "worldcolor");
	SetupValue(g_Menu.Config.NightmodeBlend, 255.f, "nightmodeblend");
	SetupValue(g_Menu.Config.ShowLagColor, 255.f, 255.f, 255.f, "showlagcolor");
	SetupValue(g_Menu.Config.AmbientLight, false, "ambientlight");
	SetupValue(g_Menu.Config.AmbientLightColor, 255.f, 255.f, 255.f, "ambientlightcolor");

	//////CHAMS//////
	SetupValue(g_Menu.Config.ArmsChams, false, "armschams");
	SetupValue(g_Menu.Config.ArmsChamsColor, 255.f, 255.f, 255.f, "armschamscolor");
	SetupValue(g_Menu.Config.ArmsChamsTransparency, 255.f, "armschamstransparency");

	SetupValue(g_Menu.Config.WeaponChams, false, "weaponchams");
	SetupValue(g_Menu.Config.WeaponChamsColor, 255.f, 255.f, 255.f, "weaponchamscolor");
	SetupValue(g_Menu.Config.WeaponChamsTransparency, 255.f, "weaponchamstransparency");

	SetupValue(g_Menu.Config.ChamsMaterial, 0, "chamsmaterial");

	SetupValue(g_Menu.Config.EnemyChams, false, "enemychams");
	SetupValue(g_Menu.Config.EnemyXQZChams, true, "enemyxqzchams");
	SetupValue(g_Menu.Config.EnemyVisOnly, false, "enemyvisonly");
	SetupValue(g_Menu.Config.EnemyVisChamsColor, 139.f, 216.f, 14.f, "enemyvischamscolor");
	SetupValue(g_Menu.Config.EnemyXQZChamsColor, 34.f, 124.f, 208.f, "enemyxqzchamscolor");

	SetupValue(g_Menu.Config.BacktrackingChams, 0, "backtrackingchams");

	SetupValue(g_Menu.Config.TeamChams, false, "teamchams");
	SetupValue(g_Menu.Config.TeamXQZChams, true, "teamxqzchams");
	SetupValue(g_Menu.Config.TeamVisOnly, false, "teamvisonly");
	SetupValue(g_Menu.Config.TeamVisChamsColor, 109.f, 181.f, 249.f, "teamvischamscolor");
	SetupValue(g_Menu.Config.TeamXQZChamsColor, 52.f, 132.f, 208.f, "teamxqzchamscolor");

	SetupValue(g_Menu.Config.LocalChams, false, "localchams");
	SetupValue(g_Menu.Config.LocalXQZChams, true, "localxqzchams");
	SetupValue(g_Menu.Config.LocalVisChamsColor, 255.f, 255.f, 255.f, "localvischamscolor");
	SetupValue(g_Menu.Config.LocalXQZChamsColor, 191.f, 191.f, 191.f, "localxqzchamscolor");

	SetupValue(g_Menu.Config.DesyncChams, false, "desyncchams");
	SetupValue(g_Menu.Config.DesyncChamsColor, 255.f, 255.f, 255.f, "desyncchamscolor");
	SetupValue(g_Menu.Config.DesyncChamsTransparency, 255.f, "desyncchamstransparency");

	SetupValue(g_Menu.Config.LbyBreakerChams, false, "lbybreakerchams");
	SetupValue(g_Menu.Config.LbyBreakerChamsColor, 255.f, 255.f, 255.f, "lbybreakerchamscolor");
	SetupValue(g_Menu.Config.LbyBreakerChamsTransparency, 255.f, "lbybreakerchamstransparency");

	SetupValue(g_Menu.Config.PlayerDeadChams, true, "playerdeadchams");

	SetupValue(g_Menu.Config.NoSmoke, false, "nosmoke");
	SetupValue(g_Menu.Config.NoFlash, false, "noflash");
	SetupValue(g_Menu.Config.FlashDuration, 255.f, "flashduration");

	//////REMOVALS//////
	SetupValue(g_Menu.Config.NoZoom, false, "nozoom");
	SetupValue(g_Menu.Config.NoScope, false, "noscope");
	SetupValue(g_Menu.Config.NoRecoil, false, "norecoil");
	SetupValue(g_Menu.Config.AimPunchVal, 1, "aimpunchval");
	SetupValue(g_Menu.Config.DisablePostProcess, false, "disablepostprocess");

	SetupValue(g_Menu.Config.OverrideViewmodel, false, "overrideviewmodel");
	SetupValue(g_Menu.Config.ViewmodelOffsetX, 10, "viewmodeloffsetx");
	SetupValue(g_Menu.Config.ViewmodelOffsetY, 10, "viewmodeloffsety");
	SetupValue(g_Menu.Config.ViewmodelOffsetZ, 10, "viewmodeloffsetz");
	SetupValue(g_Menu.Config.roll, 0, "viewmodelroll");

	//////FOV//////
	SetupValue(g_Menu.Config.Fov, 90, "fov");
	SetupValue(g_Menu.Config.ViewmodelFov, 69, "viewmodelfov");
	SetupValue(g_Menu.Config.ThirdpersonFov, 110, "thirdpersonfov");
	SetupValue(g_Menu.Config.ThirdpersonHotkey, 13, "thirdpersonhotkey");
	SetupValue(g_Menu.Config.aspectvalue, 0.f, "ascpectratiovalue");
	SetupValue(g_Menu.Config.aspectratio, false, "aspectratio");
	SetupValue(g_Menu.Config.SlowWalk, false, "slowwalk");
	SetupValue(g_Menu.Config.SlowWalkType, 2, "slowwalktype");
	SetupValue(g_Menu.Config.SlowWalkAmount, 30, "slowwalkamount");
	SetupValue(g_Menu.Config.StopLegAnim, true, "stopleganimation");
	SetupValue(g_Menu.Config.ShowAccurateLby, true, "showaccuratelowerbody");

	////// i dunno, actually //////
	SetupValue(g_Menu.Config.Crosshair, false, "crosshair");
	SetupValue(g_Menu.Config.quickerplant, false, "quickerplant");
	SetupValue(g_Menu.Config.blockbotonhead, false, "blokbot");
	SetupValue(g_Menu.Config.BlockBotKey, 0, "blokbotkey");
	SetupValue(g_Menu.Config.AutoWallCrosshair, false, "autowallcrosshair");
	SetupValue(g_Menu.Config.Watermark, false, "watermark");
	SetupValue(g_Menu.Config.x88menu, false, "x88menu");
	SetupValue(g_Menu.Config.MenuColor, 132.f, 0.f, 195.f, "menucolor");
	SetupValue(g_Menu.Config.PreserveKillFeed, false, "preservekillfeed");
	SetupValue(g_Menu.Config.BulletTracers, false, "bullettrace");
	SetupValue(g_Menu.Config.Impacts, false, "bulletimpact");
	SetupValue(g_Menu.Config.ImpactType, 1, "bulletimpacttype");
	SetupValue(g_Menu.Config.BulletTracerEnemyColor, 255.f, 255.f, 255.f, "bullettracerenemycolor");
	SetupValue(g_Menu.Config.BulletTracerLocalColor, 255.f, 255.f, 255.f, "bullettracerlocalcolor");
	SetupValue(g_Menu.Config.KillCounter, false, "killcounter");
	SetupValue(g_Menu.Config.VizualizeHits, false, "VHits");
	SetupValue(g_Menu.Config.LogDamage, false, "logdmg");
	//////MISC//////
	SetupValue(g_Menu.Config.Hitmarker, 0, "hitsound");
	SetupValue(g_Menu.Config.KnifeLeftSide, false, "knifeleftside");
	SetupValue(g_Menu.Config.KeyVisualizer, false, "keyvisualizer");
	SetupValue(g_Menu.Config.Bhop, true, "bhop");
	SetupValue(g_Menu.Config.AutoStrafe, false, "autostrafe");
	SetupValue(g_Menu.Config.indicators, false, "indicators");
	SetupValue(g_Menu.Config.ShowLag, false, "showlag");
	SetupValue(g_Menu.Config.DisablePanoramaBlur, false, "disablepanblur");
	SetupValue(g_Menu.Config.MMRegionChanger, 0, "mmregionchanger");
	SetupValue(g_Menu.Config.SpecList, false, "speclist");
	SetupValue(g_Menu.Config.TopInfoBar, false, "topinfobar");
	SetupValue(g_Menu.Config.BuyBot.enable, false, "bbe");
	SetupValue(g_Menu.Config.BuyBot.main, 0, "bbm");
	SetupValue(g_Menu.Config.BuyBot.second, 0, "bbs");
	SetupValue(g_Menu.Config.BuyBot.armor, 0, "bba");
	SetupValue(g_Menu.Config.BuyBot.grenade, 0, "bbg");
	SetupValue(g_Menu.Config.knivchanger, false, "kniv");
	SetupValue(g_Menu.Config.KnifeSkin, 0, "knivskin");
	SetupValue(g_Menu.Config.Knife, 0, "knivmodel");
	SetupValue(g_Menu.Config.Skinchanger.GlobalSkin, 0, "globalskin");
	SetupValue(g_Menu.Config.AntiClientCrasher, true, "acc");
	SetupValue(g_Menu.Config.DrawAimFov, false, "drawaimfov");
	SetupValue(g_Menu.Config.AimFovColor, 255.f, 255.f, 255.f, "aimfovcolor");

	// More ESP
	SetupValue(g_Menu.Config.BombEsp, false, "bombesp");
	SetupValue(g_Menu.Config.monkey, false, "monkey2");
	SetupValue(g_Menu.Config.monkey2, false, "monkey");
	//Weapon Configs
	SetupValue(g_Menu.Config.AutoHC, 45, "autohc");
	SetupValue(g_Menu.Config.AutoMD, 10, "automd");
	SetupValue(g_Menu.Config.AutoBS, 56, "autobs");
	SetupValue(g_Menu.Config.AutoHS, 65, "autohs");

	SetupValue(g_Menu.Config.RifleHC, 39, "riflehc");
	SetupValue(g_Menu.Config.RifleMD, 21, "riflemd");
	SetupValue(g_Menu.Config.RifleBS, 70, "riflebs");
	SetupValue(g_Menu.Config.RifleHS, 95, "riflehs");

	SetupValue(g_Menu.Config.ScoutHC, 75, "scouthc");
	SetupValue(g_Menu.Config.ScoutMD, 43, "scoutmd");
	SetupValue(g_Menu.Config.ScoutBS, 55, "scoutbs");
	SetupValue(g_Menu.Config.ScoutHS, 95, "scouths");

	SetupValue(g_Menu.Config.PistolHC, 25, "pistolhc");
	SetupValue(g_Menu.Config.PistolMD, 10, "pistolmd");
	SetupValue(g_Menu.Config.PistolBS, 40, "pistolbs");
	SetupValue(g_Menu.Config.PistolHS, 90, "pistolhs");

	SetupValue(g_Menu.Config.AwpHC, 97, "awphc");
	SetupValue(g_Menu.Config.AwpMD, 65, "awpmd");
	SetupValue(g_Menu.Config.AwpBS, 90, "awpbs");
	SetupValue(g_Menu.Config.AwpHS, 30, "awphs");

	SetupValue(g_Menu.Config.SmgHC, 8, "smghc");
	SetupValue(g_Menu.Config.SmgMD, 14, "smgmd");
	SetupValue(g_Menu.Config.SmgBS, 100, "smgbs");
	SetupValue(g_Menu.Config.SmgHS, 65, "smghs");

	SetupValue(g_Menu.Config.DeagleHC, 65, "deaglehc");
	SetupValue(g_Menu.Config.DeagleMD, 20, "deaglemd");
	SetupValue(g_Menu.Config.DeagleBS, 45, "deaglebs");
	SetupValue(g_Menu.Config.DeagleHS, 95, "deaglehs");
}

void CConfig::SetAutoConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.AutoHC;
	g_Menu.Config.Mindmg = g_Menu.Config.AutoMD;
	g_Menu.Config.HeadScale = g_Menu.Config.AutoHC;
	g_Menu.Config.BodyScale = g_Menu.Config.AutoBS;
}

void CConfig::SetRifleConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.RifleHC;
	g_Menu.Config.Mindmg = g_Menu.Config.RifleMD;
	g_Menu.Config.HeadScale = g_Menu.Config.RifleHC;
	g_Menu.Config.BodyScale = g_Menu.Config.RifleBS;
}

void CConfig::SetScoutConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.ScoutHC;
	g_Menu.Config.Mindmg = g_Menu.Config.ScoutMD;
	g_Menu.Config.HeadScale = g_Menu.Config.ScoutHC;
	g_Menu.Config.BodyScale = g_Menu.Config.ScoutBS;
}

void CConfig::SetPistolConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.PistolHC;
	g_Menu.Config.Mindmg = g_Menu.Config.PistolMD;
	g_Menu.Config.HeadScale = g_Menu.Config.PistolHC;
	g_Menu.Config.BodyScale = g_Menu.Config.PistolBS;
}

void CConfig::SetAwpConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.AwpHC;
	g_Menu.Config.Mindmg = g_Menu.Config.AwpMD;
	g_Menu.Config.HeadScale = g_Menu.Config.AwpHC;
	g_Menu.Config.BodyScale = g_Menu.Config.AwpBS;
}

void CConfig::SetSmgConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.SmgHC;
	g_Menu.Config.Mindmg = g_Menu.Config.SmgMD;
	g_Menu.Config.HeadScale = g_Menu.Config.SmgHC;
	g_Menu.Config.BodyScale = g_Menu.Config.SmgBS;
}

void CConfig::SetDeagleConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.DeagleHC;
	g_Menu.Config.Mindmg = g_Menu.Config.DeagleMD;
	g_Menu.Config.HeadScale = g_Menu.Config.DeagleHC;
	g_Menu.Config.BodyScale = g_Menu.Config.DeagleBS;
}

void CConfig::SetupValue(int& value, int def, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(name, &value));
}

void CConfig::SetupValue(ColorV2& value, float defr, float defg, float defb, std::string name)
{
	value.red = defr;
	value.green = defg;
	value.blue = defb;
	value.alpha = 255.f;

	floats.push_back(new ConfigValue< float >(name + "red", &value.red));
	floats.push_back(new ConfigValue< float >(name + "green", &value.green));
	floats.push_back(new ConfigValue< float >(name + "blue", &value.blue));
	floats.push_back(new ConfigValue< float >(name + "alpha", &value.alpha));
}

void CConfig::SetupValue(bool* value, bool def, int size, std::string name)
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(name, &value[c]));
	}
}

static char name[] = "t4zCheats";

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\t4zCheats\\";
		switch (g_Menu.Config.CurrentConfig)
		{
		case 0:
			//Legit1
			file = std::string(path) + "\\t4zCheats\\legit1.ini";
			break;
		case 1:
			//Legit2
			file = std::string(path) + "\\t4zCheats\\legit2.ini";
			break;
		case 2:
			//Semi Rage
			file = std::string(path) + "\\t4zCheats\\semirage.ini";
			break;
		case 3:
			//HVH 1
			file = std::string(path) + "\\t4zCheats\\hvh1.ini";
			break;
		case 4:
			//HVH 2
			file = std::string(path) + "\\t4zCheats\\hvh2.ini";
			break;
		default:
			file = std::string(path) + "\\t4zCheats\\hvh1.ini";
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\t4zCheats\\";
		switch (g_Menu.Config.CurrentConfig)
		{
		case 0:
			//Legit1
			file = std::string(path) + "\\t4zCheats\\legit1.ini";
			break;
		case 1:
			//Legit2
			file = std::string(path) + "\\t4zCheats\\legit2.ini";
			break;
		case 2:
			//Semi Rage
			file = std::string(path) + "\\t4zCheats\\semirage.ini";
			break;
		case 3:
			//HVH 1
			file = std::string(path) + "\\t4zCheats\\hvh1.ini";
			break;
		case 4:
			//HVH 2
			file = std::string(path) + "\\t4zCheats\\hvh2.ini";
			break;
		default:
			file = std::string(path) + "\\t4zCheats\\hvh1.ini";
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* g_Config = new CConfig();
