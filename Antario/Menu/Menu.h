#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>

namespace fs = std::experimental::filesystem;

struct ColorV2
{
	float red, green, blue, alpha;
};

enum
{
	check_box,
	slider,
	combo_box,
	multi_box

};

class Menu
{
public:
	bool menuOpened;
	void Render();

	struct
	{
		//Legit
		bool LegitAimbot;
		int LegitAimKey;
		bool LegitBacktrack;
		bool RecoilControlSystem;
		int RCS_X;
		int RCS_Y;
		int LegitAimFov;
		int AimSpeed;
		int LegitAimHitbox;
		bool LegitAA;

		//Rage
		bool Aimbot;
		bool SilentAim;
		bool AimShowRecoil;
		int  AimMode;
		int  AimbotFov;
		bool OnKeyAim;
		int  AimHotkey;
		bool FriendlyFire;
		int  Hitchance;
		int  HitchanceValue;
		int  Mindmg;
		bool Resolver;
		int  BodyScale;
		int  HeadScale;
		bool MultiPoint;
		bool ForceBaim;
		int BaimAfterXShots;
		int BaimAfterXMissedShots;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		int customstop;
		int AutoStopMode;
		bool AutoScope;
		bool AutoRevolver;
		bool AutomaticWeapons;
		bool PosBacktrack;
		bool LbyBacktrack;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;

		//Hitboxes
		bool HB_Head;
		bool HB_Neck;
		bool HB_Chest;
		bool HB_Belly;
		bool HB_Pelvis;
		bool HB_Thorax;
		bool HB_Arms;
		bool HB_Thighs;
		bool HB_Calf;
		bool HB_Feet;

		bool Antiaim;
		int AntiAimMode;
		int AntiAimSide;
		int AntiAimHotkey;
		int FakeSide;
		int FakeSideHotkey;
		int AntiAimBackHotkey;
		int PitchMode;
		int YawMode;
		int CustomPitchValue;
		int FixedCustomYaw;
		bool LbyBreaker;
		int Lbymode;
		int LbyMult;
		int LbyAdd;
		int SpeedSpin;
		bool AntiAimKeys;
		bool RealAngles;
		bool DesyncAngle;
		int DesyncMode;
		float DesyncY;
		float MaxDesyncDelta;
		int	JitterRange;
		int	JitterMode;
		int SlideSpeed;
		bool ZeroPitchOnLand;
		bool CustomPitchOnLand;
		int LandPitch;
		int OnShotAA;
		bool AntiAimIndicator;
		bool AdvancedAAIndicator;
		bool SlideWalk;
		int	FakeLagInAir;
		int FakeLagOnGround;
		bool FakeLagOnPeek;
		bool BreakLagComp;
		bool ChokeShotOnPeek;
		bool FakeWalk;
		bool NasaWalk;
		int NasaWalkKey;
		int FakeWalkChoke;
		bool FakeDuck;
		int FakeDuckKey;
		bool PingSpike;
		int PingValue;
		bool ShowServerHitboxes;
		bool SpecList;
		bool aspectratio;

		int Esp;
		bool TeamEsp;
		bool SkeletonEsp[2] = { false, false };
		ColorV2 SkeletonColor = { 255.f,255.f,255.f,255.f };

		bool HbEsp;

		int Name;
		bool YawPitch;
		bool Money;
		bool Health;
		int HealthBar;
		int Weapon;
		bool Box;
		bool skeldead;

		bool OffscreenEsp;
		ColorV2 OffscreenEspColor = { 255.f,255.f,255.f,255.f };
		ColorV2 FovColor = { 255.f,255.f,255.f,255.f };
		ColorV2 NameColor = { 255.f,255.f,255.f,255.f };
		ColorV2 HealthColor = { 255.f,255.f,255.f,255.f };
		ColorV2 HealthBarColor = { 255.f,255.f,255.f,255.f };
		ColorV2 WeaponColor = { 255.f,255.f,255.f,255.f };
		ColorV2 YawPitchColor = { 255.f,255.f,255.f,255.f };
		ColorV2 MoneyColor = { 255.f,255.f,255.f,255.f };
		ColorV2 BoxColor = { 255.f,255.f,255.f,255.f };

		bool EnemyGlow;
		ColorV2 EnemyGlowColor = { 255.f,255.f,255.f,255.f };
		int EnemyGlowOpacity;

		bool TeamGlow;
		ColorV2 TeamGlowColor = { 255.f,255.f,255.f,255.f };
		int TeamGlowOpacity;

		bool LocalGlow;
		ColorV2 LocalGlowColor = { 255.f,255.f,255.f,255.f };
		int LocalGlowOpacity;

		bool Nightmode;
		bool ProperNightmode;
		ColorV2 WorldColor = { 50.f, 50.f, 50.f, 255.f };
		ColorV2 PropColor = { 50.f, 50.f, 50.f, 255.f };
		int NightmodeBlend;

		bool AmbientLight;
		ColorV2 AmbientLightColor = { 255.f, 255.f, 255.f, 255.f };

		// Chams

		bool viewmodelchams;
		bool ArmsChams;
		ColorV2 ArmsChamsColor = { 255.f,255.f,255.f,255.f };
		int ArmsChamsTransparency;

		bool WeaponChams;
		ColorV2 WeaponChamsColor = { 255.f,255.f,255.f,255.f };
		int WeaponChamsTransparency;

		int ChamsMaterial;

		bool EnemyChams;
		bool EnemyXQZChams;
		bool EnemyVisOnly;
		ColorV2 EnemyVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 EnemyXQZChamsColor = { 255.f,255.f,255.f,255.f };

		int BacktrackingChams;

		bool TeamChams;
		bool TeamXQZChams;
		bool TeamVisOnly;
		ColorV2 TeamVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 TeamXQZChamsColor = { 255.f,255.f,255.f,255.f };

		bool LocalChams;
		bool LocalXQZChams;
		ColorV2 LocalVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 LocalXQZChamsColor = { 255.f,255.f,255.f,255.f };

		bool DesyncChams;
		ColorV2 DesyncChamsColor = { 255.f,255.f,255.f,255.f };
		int DesyncChamsTransparency;

		bool LbyBreakerChams;
		ColorV2 LbyBreakerChamsColor = { 255.f,255.f,255.f,255.f };
		int LbyBreakerChamsTransparency;

		bool PlayerDeadChams;

		// End Chams

		bool NoSmoke;
		bool NoFlash;
		int FlashDuration;

		bool NoZoom;
		bool NoScope;
		bool NoRecoil;
		bool DisablePostProcess;
		int AimPunchVal;

		bool OverrideViewmodel;
		int ViewmodelOffsetX;
		int ViewmodelOffsetY;
		int ViewmodelOffsetZ;
		int roll;

		int Fov;
		int CurrentFov;
		int ViewmodelFov;
		int ThirdpersonFov;

		bool PreserveKillFeed;

		bool Crosshair;
		bool AutoWallCrosshair;
		bool IsThirdperson = false;
		int ThirdpersonHotkey;
		bool SlowWalk;
		int SlowWalkType;
		int SlowWalkAmount;
		bool StopLegAnim;
		bool ShowAccurateLby;
		bool StopInBetweenShots;
		int ResolverMode;

		bool KeyVisualizer;
		bool KnifeLeftSide;
		int Hitmarker;
		bool Bhop;
		bool AutoStrafe;
		int ClanTagMode = 0;
		std::string ClantagText = "t4zCheats";
		bool DisablePanoramaBlur;
		bool PlayerList;
		int CurrentConfig = 0;
		bool Unload;
		bool indicators;
		bool AntiUntrusted = true;
		int weaponmode;
		bool BulletTracers;
		bool Impacts;
		ColorV2 BulletTracerEnemyColor = { 255.f,255.f,255.f,255.f };
		ColorV2 BulletTracerLocalColor = { 255.f,255.f,255.f,255.f };
		ColorV2 CapsuleColor = { 255.f,255.f,255.f,255.f };
		ColorV2 ShowLagColor = { 255.f,255.f,255.f,255.f };
		bool KillCounter;
		bool VizualizeHits;
		int moonmode;
		bool ShowLag;
		int fakelagonpeek;
		bool FakeLagPrediction;
		bool ammo;
		int FovDistance;
		int FovSize;
		bool FovAlways;
		bool FovEnable;
		int LocalGlowMode;
		int MMRegionChanger;
		bool TopInfoBar;
		bool monkey;
		bool monkey2;
		bool AntiClientCrasher;
		bool DrawAimFov;
		bool namestealer;
		bool quickerplant;
		float aspectvalue;
		bool defuseyes;
		int BlockBotKey;
		bool blockbotonhead;
		ColorV2 AimFovColor = { 255.f,255.f,255.f,255.f };

		// More ESP
		bool BombEsp;

		bool Watermark;
		bool x88menu;
		ColorV2 MenuColor = { 255.f,255.f,255.f,255.f };

		//Weapon Config
		int AutoHC;
		int AutoMD;
		int AutoBS;
		int AutoHS;

		int RifleHC;
		int RifleMD;
		int RifleBS;
		int RifleHS;

		int ScoutHC;
		int ScoutMD;
		int ScoutBS;
		int ScoutHS;

		int PistolHC;
		int PistolMD;
		int PistolBS;
		int PistolHS;

		int AwpHC;
		int AwpMD;
		int AwpBS;
		int AwpHS;

		int SmgHC;
		int SmgMD;
		int SmgBS;
		int SmgHS;

		int DeagleHC;
		int DeagleMD;
		int DeagleBS;
		int DeagleHS;
		bool EdgeJump;
		int EdgeJumpKey;
		bool shotcounter;
		float ExpireTime = 1.3f;
		float MarkerSize = 12.f;
		bool VisHitmarker;
		bool LogDamage;
		class BuyBot
		{
		public:
			bool enable;
			int armor;
			int main;
			int second;
			int grenade;
		}BuyBot;
		class Skinchanger
		{
		public:
			bool skinenabled;
			int Knife;
			int gloves;
			int skingloves;
			float glovewear;
			bool glovesenabled;
			bool rankchanger;
			int KnifeSkin;
			int AK47Skin;
			int GalilSkin;
			int M4A1SSkin;
			int M4A4Skin;
			int AUGSkin;
			int FAMASSkin;
			int AWPSkin;
			int SSG08Skin;
			bool fixshit;
			bool knifes;
			int SCAR20Skin;
			int P90Skin;
			int Mp7Skin;
			int NovaSkin;
			int UMP45Skin;
			int GlockSkin;
			int SawedSkin;
			int USPSkin;
			int MagSkin;
			int XmSkin;
			int DeagleSkin;
			int DualSkin;
			int FiveSkin;
			int RevolverSkin;
			int Mac10Skin;
			int tec9Skin;
			int Cz75Skin;
			int NegevSkin;
			int M249Skin;
			int Mp9Skin;
			int P2000Skin;
			int BizonSkin;
			int Sg553Skin;
			int P250Skin;
			int G3sg1Skin;
			struct stickers
			{
				int Stikers1;
				int Stikers2;
				int Stikers3;
				int Stikers4;
			}allin1skins[530];



			bool stickers;
			bool customchams;
			int awpcustom;
			int ak47custom;
			int scarcustom;
			int ssgcustom;
			int m4a1scustom;
			int knifecustom;
			int glovecustom;
			int GlobalSkin;
			std::string killicon_config;
		}Skinchanger;
		bool knivchanger;
		int Knife;
		int KnifeSkin;
		bool fix;
		bool Back;
		bool Left;
		bool Right;
		int BackHotkey;
		int LeftHotkey;
		int RightHotkey;
		int ManualMode;
		std::string killicon_config;
		bool GrenadePrediction;
		int ArmsChamsMode;
		bool ArmsWireFrame;
		int ImpactType;
		int BodyLean;
		int BodyLeanInverted;
		bool AnimLayers;
		bool LbyConfuser;
		bool StaticLby;
		bool SendPacketOnBreak;
		bool invis_name;
		bool ShowDmg;
		bool ChokeShot;
		int FakeLagType;
		int FakeLagValue;
		int FakeLagValueGround;
	}Config;

private:
	struct
	{
		float x = 50.f, y = 50.f;
	}Pos; // lol

	enum
	{
		check_box,
		slider,
		sliderfloat,
		combo_box,
		multi_box,
		button,
		spacer,
		label,
		key_bind
	};

	int ControlsX;
	int ControlsX2;
	int GroupTabBottom;
	int OffsetY;
	int OldOffsetY;
	int TabOffset;
	int SubTabOffset;
	float SubTabSize; // cpp fuckin sux had to make this a float or the whole thing crashes
	float TabSize;
	int GroupTabPos[4];
	bool ControlsReturn;

	int TabNum = 0;
	int SubTabNum = 0;
	int PreviousControl = -1;

	typedef void(*ButtonCallback_t)(void);


	void KeyBind(std::string name, int* item);
	void Tab(std::string name);
	void SubTab(std::string name);
	void CheckBox(std::string name, bool* item, std::string tooltip = "");
	void Slider(int max, std::string name, int* item);
	void SliderFloat(float max, std::string name, float* item);
	void ComboBox(std::string name, std::vector< std::string > itemname, int* item);
	void ColorPicker(std::string name, ColorV2& item);
	void Button(std::string text, ButtonCallback_t callback);
	void Space(int spaceval);
	void TextBox(std::string name, std::string* text);
	void Text(std::string text);

};

extern Menu g_Menu;