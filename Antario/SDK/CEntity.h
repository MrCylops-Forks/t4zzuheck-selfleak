#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "CInput.h"
#include "..\Utils\Utils.h"
#include "..\Utils\NetvarManager.h"
#include "CGlobalVarsBase.h"

// class predefinition
class C_BaseCombatWeapon;
#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = g_pNetvars->GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
	}
#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = g_pNetvars->GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
	}
class C_AnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};

class AnimationLayer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038



class viewmodel_t;
class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:
	NETVAR(HANDLE, m_hWeaponWorldModel_h, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");//+
	NETVAR(bool, hasdefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(CBaseHandle, m_hWeaponWorldModel_c, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");//+
	NETVAR(int, InBombZone, "DT_CSPlayer", "m_bInBombZone");//+
	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");//+
	NETVAR(CHandle<viewmodel_t>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(int*, ItemIDHigh, "m_iItemIDHigh", "DT_BaseAttributableItem");//+
	NETVAR(int, m_hMyWearables, "m_hMyWearables", "DT_CSPlayer");//+
	NETVAR(float, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	PNETVAR(CBaseHandle, GetWearables, "DT_BaseCombatCharacter", "m_hMyWearables");


	void SetModelIndex(int index)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, int);
		return Utils::GetVFunc<OriginalFn>(this, 75)(this, index);
	}
	bool IsKnife();
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + 0x31B4);
	}

	int* GetAccountId()
	{
		return (int*)((uintptr_t)this + 0x2FB8);
	}
	int* FallbackSeed()
	{
		return (int*)((uintptr_t)this + 0x31AC);
	}
	int* FallbackStartTrak()
	{
		return (int*)((uintptr_t)this + 0x31B4);
	}
	int* ModelIndex()
	{
		return (int*)((uintptr_t)this + 0x258);
	}
	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + 0x3220);
	}
	inline int* FallbackPaintKit() {
		return (int*)((DWORD)this + 0x31B8);
	}
	int* GetEntityQuality()
	{
		return (int*)((uintptr_t)this + 0x2FAC);
	}
	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + 0x31B0);
	}

	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + 0x31C0);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x2FC0);
	}


	CBaseHandle* m_hMyWeapons()
	{
		return (CBaseHandle*)((uintptr_t)this + 0x2DF8);
	}

	short* fixskins()
	{
		return (short*)((uintptr_t)this + 0x2FAA);
	}
	C_AnimState *AnimState()
	{
		return *reinterpret_cast<C_AnimState**>(uintptr_t(this) + 0x3900);
	}

	float GetSimTime() {
		static int m_flSimulationtime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationtime);
	}


	float GetOldSimTime() {
		static int m_flSimulationTime = 0x264;
		return *(float*)((DWORD)this + (m_flSimulationTime + 0x4));
	}

	/*std::array<float, 24> pos_par()
	{
		static int pos_par_hh = g_pNetvars->GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + pos_par_hh);
	}*/

	AnimationLayer *AnimOverlays()
	{
		return *reinterpret_cast<AnimationLayer**>(uintptr_t(this) + 0x2980);
	}

	int GetNumAnimOverlays() {
		return *(int*)((DWORD)this + 0x298C);
	}

	bool& GetClientSideAnimation() {
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");;
		return *reinterpret_cast<bool*>((DWORD)this + m_bClientSideAnimation);
	}

	void UpdateClientSideAnimation() {
		Utils::CallVFunc<219, void>(this);
	}

	int NumOverlays()
	{
		return 15;
	}

	void UpdateClientAnimation()
	{
		Utils::GetVFunc<void(__thiscall*)(void*)>(this, 221)(this);
	}

	void ClientAnimations(bool value)
	{
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}

	int GetSequence()
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		return GetValue<int>(m_nSequence);
	}

	void SetSequence(int Sequence)
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nSequence) = Sequence;
	}

	void SimulatedEveryTick(bool value)
	{
		static int m_bSimulatedEveryTick = g_pNetvars->GetOffset("DT_BaseEntity", "m_bSimulatedEveryTick");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bSimulatedEveryTick) = value;
	}

	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(Vector origin);

	int GetActiveWeaponIndex()
	{
		static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_hActiveWeapon) & 0xFFF; //m_hActiveWeapon
	}

	Vector GetAbsOrigin()
	{
		return Utils::GetVFunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}

	void SetAbsVelocity(Vector velocity);

	C_BaseEntity* GetObserverTarget()
	{
		return Utils::GetVFunc<C_BaseEntity*(__thiscall*)(void*)>(this, 291)(this);
	}

    C_BaseCombatWeapon* GetActiveWeapon()
    {
        static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = GetValue<CBaseHandle>(m_hActiveWeapon);
        return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(weaponData));
    }

    int GetTeam()
    {
        static int m_iTeamNum = g_pNetvars->GetOffset("DT_BaseEntity", "m_iTeamNum");
        return GetValue<int>(m_iTeamNum);
    }

    EntityFlags GetFlags()
    {
        static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
        return GetValue<EntityFlags>(m_fFlags);
    }
	AnimationLayer* GetAnimOverlays4()
	{
		// to find offset: use 9/12/17 dll
		// sig: 55 8B EC 51 53 8B 5D 08 33 C0
		return *(AnimationLayer * *)((DWORD)this + 0x2980);
	}
	AnimationLayer* GetAnimOverlay4(int i)
	{
		if (i < 16)
			return &GetAnimOverlays4()[i];
	}

	void SetFlags(int offset)
	{
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		*reinterpret_cast<int*>(uintptr_t(this) + m_fFlags) = offset;
	}

    MoveType_t GetMoveType()
    {
        static int m_Movetype = g_pNetvars->GetOffset("DT_BaseEntity", "m_nRenderMode") + 1;
        return GetValue<MoveType_t>(m_Movetype);
    }

	float GetSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationTime);
	}

	float GetOldSimulationTime()
	{
		static int m_flOldSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 4;
		return GetValue<float>(m_flOldSimulationTime);
	}

	float GetLowerBodyYaw()
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return GetValue<float>(m_flLowerBodyYawTarget);
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}

	bool GetHeavyArmor()
	{
		static int m_bHasHeavyArmor = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
		return GetValue<bool>(m_bHasHeavyArmor);
	}

	int ArmorValue()
	{
		static int m_ArmorValue = g_pNetvars->GetOffset("DT_CSPlayer", "m_ArmorValue");
		return GetValue<int>(m_ArmorValue);
	}

	bool HasHelmet()
	{
		static int m_bHasHelmet = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return GetValue<bool>(m_bHasHelmet);
	}


    bool GetLifeState()
    {
        static int m_lifeState = g_pNetvars->GetOffset("DT_BasePlayer", "m_lifeState");
        return GetValue<bool>(m_lifeState);
    }

	bool IsSpotted()
	{
		static int m_bSpotted = g_pNetvars->GetOffset("DT_BaseEntity", "m_bSpotted");
		return GetValue<bool>(m_bSpotted);
	}

	bool IsScoped()
	{
		static int m_bIsScoped = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsScoped");
		return GetValue<bool>(m_bIsScoped);
	}

    int GetHealth()
    {
        static int m_iHealth = g_pNetvars->GetOffset("DT_BasePlayer", "m_iHealth");
        return GetValue<int>(m_iHealth);
    }

	int GetMoney()
	{
		static int m_ArmorValue = g_pNetvars->GetOffset("DT_CSPlayer", "m_iAccount");
		return GetValue<int>(m_ArmorValue);
	}



	void SetFlashDuration(float fDuration)
	{
		static int pFlashDuration = g_pNetvars->GetOffset("DT_CSPlayer", "m_flFlashDuration");
		*reinterpret_cast<int*>(uintptr_t(this) + pFlashDuration) = fDuration;
	}

	bool is_valid(C_BaseEntity* local_player = nullptr, bool is_player = true, bool dormant = true, bool team_check = true) {
		if (!this)
			return false;

		if (dormant)
			if (this->IsDormant())
				return false;

		if (team_check && local_player)
			if (this->GetTeam() == local_player->GetTeam())
				return false;

		if (is_player)
			if (!this->IsPlayer())
				return false;

		if (this->GetHealth() <= 0)
			return false;

		return true;
	}

	bool IsPlayer() {
		using original_fn = bool(__thiscall*)(C_BaseEntity*);
		return (*(original_fn**)this)[155](this);
	}

	bool IsKnifeorNade();

	bool IsNade();


	int GetSequenceActivity(int sequence);

    bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

	bool IsPistol();

	bool IsShotgun();

	bool IsSniper();

	bool IsEnemy();

    bool IsImmune()
    {
        static int m_bGunGameImmunity = g_pNetvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
        return GetValue<bool>(m_bGunGameImmunity);
    }

    int GetTickBase()
    {
        static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
        return GetValue<int>(m_nTickBase);
    }

	int GetShotsFired()
	{
		static int m_iShotsFired = g_pNetvars->GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");
		return GetValue<int>(m_iShotsFired);
	}

	void SetTickBase(int TickBase)
	{
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}

	Vector GetEyeAngles()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return GetValue<Vector>(m_angEyeAngles);
	}

	void SetEyeAngles(Vector Angle)
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_angEyeAngles) = Angle;
	}

    Vector GetOrigin()
    {
        static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
        return GetValue<Vector>(m_vecOrigin);
    }

	Vector GetOldOrigin()
	{
		static int m_vecOldOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 12;
		return GetValue<Vector>(m_vecOldOrigin);
	}

	Vector GetNetworkOrigin()
	{
		static int m_vecNetworkOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 40;
		return GetValue<Vector>(m_vecNetworkOrigin);
	}

	void SetOrigin(Vector Origin)
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecOrigin) = Origin;
	}

	Vector GetVelocity()
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return GetValue<Vector>(m_vecVelocity);
	}

	void SetVelocity(Vector velocity)
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecVelocity) = velocity;
	}

	Vector GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x302C));
	}

	Vector GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3020));
	}

	Vector GetVecViewOffset()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x108));
	}

	Vector GetViewPunch()
	{
		static int m_viewPunchAngle = g_pNetvars->GetOffset("DT_BaseEntity", "m_viewPunchAngle");
		return GetValue<Vector>(m_viewPunchAngle);
	}

    Vector GetEyePosition() 
	{
		Vector ret;
		typedef void(__thiscall * OrigFn)(void*, Vector&);
		Utils::GetVFunc<OrigFn>(this, 281)(this, ret);
		return ret;
	}

	inline Vector ExtrapolateTick(Vector p0, Vector v0)
	{
		return p0 + (v0 * g_pGlobalVars->intervalPerTick);
	}

	Vector GetPredicted(Vector p0)
	{
		return ExtrapolateTick(p0, this->GetVelocity());
	}

	Vector GetBonePosition(int iBone)
	{
		matrix3x4_t boneMatrixes[128];
		if (this->SetupBones(boneMatrixes, 128, 0x100, 0))
		{
			matrix3x4_t boneMatrix = boneMatrixes[iBone];
			return Vector(boneMatrix.flMatVal[0][3], boneMatrix.flMatVal[1][3], boneMatrix.flMatVal[2][3]);
		}
		else
			return Vector(0, 0, 0);
	}

	ICollideable* GetCollideable()
	{
		return reinterpret_cast<ICollideable*>(reinterpret_cast<uintptr_t>(this) + 0x318);
	}

	void SetCurrentCommand(CUserCmd *cmd)
	{
		static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}

	float FireRate();

	void FixSetupBones(matrix3x4_t *Matrix);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix);
};



class C_BaseCombatWeapon : public C_BaseEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

public:

    ItemDefinitionIndex GetItemDefinitionIndex()
    {
        static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
        return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
    }

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::GetVFunc< oInaccuracy >(this, 476)(this);
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::GetVFunc< oWeaponSpread >(this, 446)(this);
	}

	float GetAccuracyPenalty()
	{
		static int m_fAccuracyPenalty = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		return GetValue<float>(m_fAccuracyPenalty);
	}

	float GetLastShotTime()
	{
		static int m_fLastShotTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return GetValue<float>(m_fLastShotTime);
	}

	void AccuracyPenalty()
	{
		typedef void(__thiscall *OrigFn)(void *);
		return Utils::GetVFunc<OrigFn>(this, 447)(this);
	}

    float GetNextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
        return GetValue<float>(m_flNextPrimaryAttack);
    }

    int GetAmmo()
    {
        static int m_iClip1 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return GetValue<int>(m_iClip1);
    }

    WeaponInfo_t* GetCSWpnData()
    {
        return Utils::CallVFunc<454, WeaponInfo_t*>(this);
    }

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->weapon_name);
    }
};

class C_BaseViewModel
{
public:
	NETVAR(DWORD, GetOwner, "DT_BaseViewModel", "m_hOwner");
	NETVAR(int, GetModelIndex, "DT_BaseViewModel", "m_nModelIndex");
};

class viewmodel_t : public C_BaseEntity {
public:
	NETVAR(int, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BaseEntity>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
};
