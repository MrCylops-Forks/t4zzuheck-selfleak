#include "skinchanger.h"
#include <Windows.h>
#include "../../SDK/CEntity.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/Definitions.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/IVModelInfo.h"
#include "../../Menu/Menu.h"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
C_BaseCombatWeapon* worldmodel;
C_BaseCombatWeapon* pWeapon;
int custommodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))





enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_iwoff = 0;

static void* o_uint_fn;
/*
static unsigned int __fastcall hooked_uint_fn(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned fl)
{
	C_BaseCombatWeapon* item = reinterpret_cast<C_BaseCombatWeapon*>(uintptr_t(thisptr) - s_iwoff);

	if (attribute == EStickerAttributeType::Index)
	{
		switch (slot)
		{
		case 0:
			return g_Menu.Config.allin1skins[*item->fixskins()].Stikers1;
			break;
		case 1:
			return g_Menu.Config.allin1skins[*item->fixskins()].Stikers2;
			break;
		case 2:
			return g_Menu.Config.allin1skins[*item->fixskins()].Stikers3;
			break;
		case 3:
			return g_Menu.Config.allin1skins[*item->fixskins()].Stikers4;
			break;
		default:
			break;
		}
	}

	return reinterpret_cast<decltype(hooked_uint_fn)*>(o_uint_fn)(thisptr, nullptr, slot, attribute, fl);
}


void ApplyStickerHooks(C_BaseCombatWeapon* item)
{
	if (!s_iwoff)
		s_iwoff = 0x2DC0 + 0xC; //m_Item

	void**& iw_vt = *reinterpret_cast<void***>(uintptr_t(item) + s_iwoff);

	static void** iw_hook_vt = nullptr;

	if (!iw_hook_vt)
	{
		size_t len = 0;
		for (; iw_vt[len]; ++len);
		iw_hook_vt = new void*[len];

		memcpy(iw_hook_vt, iw_vt, len * sizeof(void*));

		o_uint_fn = iw_hook_vt[5];
		iw_hook_vt[5] = &hooked_uint_fn;
	}

	iw_vt = iw_hook_vt;
}

*/


void skinchanger()
{
	if (!g::pLocalEntity) return;
	auto weapons = g::pLocalEntity->m_hMyWeapons();
	if (weapons)
		for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
		{
			{
				C_BaseEntity* pEntity = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(weapons[i]);
				if (pEntity)
				{
					pWeapon = (C_BaseCombatWeapon*)pEntity;
					if (g_Menu.Config.knivchanger)
					{
						int Model = g_Menu.Config.Knife;
						int weapon = *pWeapon->fixskins();
						if (weapon == (int)ItemDefinitionIndex::WEAPON_AK47)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_DEAGLE)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_ELITE)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_FIVESEVEN)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_AWP)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_USP_SILENCER)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_GLOCK)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_AUG)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_FAMAS)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_G3SG1)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_GALILAR)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_M249)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_M4A1_SILENCER)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_M4A1)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_MAC10)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_P90)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_BIZON)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_HKP2000)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_MP7)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_MP9)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_P250)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_SCAR20)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_SG556)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_SSG08)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;
						if (weapon == (int)ItemDefinitionIndex::WEAPON_REVOLVER)
							* pWeapon->FallbackPaintKit() = g_Menu.Config.Skinchanger.GlobalSkin;


						if (pEntity->GetClientClass()->ClassID == (int)107)
						{
							g_Menu.Config.fix = true;
							auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);

							HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel_h();
							if (worldmodel_handle2) worldmodel = (C_BaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel_c());
							if (Model == 0) // Bayonet
							{
								int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
								*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iBayonet;
								if (worldmodel)* worldmodel->ModelIndex() = iBayonet + 1;
								*pWeapon->fixskins() = 500;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "bayonet";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 563; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 573; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 9) // Bowie Knife
							{
								int iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
								*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iBowie;
								if (worldmodel)* worldmodel->ModelIndex() = iBowie + 1;
								*pWeapon->fixskins() = 514;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_survival_bowie";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 6) // Butterfly Knife
							{
								int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
								*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iButterfly;
								if (worldmodel)* worldmodel->ModelIndex() = iButterfly + 1;
								*pWeapon->fixskins() = 515;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_butterfly";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 7) // Falchion Knife
							{
								int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
								*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iFalchion;
								if (worldmodel)* worldmodel->ModelIndex() = iFalchion + 1;
								*pWeapon->fixskins() = 512;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_falchion";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 1) // Flip Knife
							{
								int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
								*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iFlip;
								if (worldmodel)* worldmodel->ModelIndex() = iFlip + 1;
								*pWeapon->fixskins() = 505;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_flip";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 559; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 564; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}

							}
							else if (Model == 2) // Gut Knife
							{
								int iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
								*pWeapon->ModelIndex() = iGut; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iGut;
								if (worldmodel)* worldmodel->ModelIndex() = iGut + 1;
								*pWeapon->fixskins() = 506;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_gut";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 560; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 565; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 575; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 5) // Huntsman Knife
							{
								int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
								*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iHuntsman;
								if (worldmodel)* worldmodel->ModelIndex() = iHuntsman + 1;
								*pWeapon->fixskins() = 509;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_tactical";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 559; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 3) // Karambit
							{
								int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
								*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iKarambit;
								if (worldmodel)* worldmodel->ModelIndex() = iKarambit + 1;
								*pWeapon->fixskins() = 507;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_karambit";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 566; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 576; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 582; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}

							}
							else if (Model == 4) // M9 Bayonet
							{
								auto iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
								*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iM9Bayonet;
								if (worldmodel)* worldmodel->ModelIndex() = iM9Bayonet + 1;
								*pWeapon->fixskins() = 508;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_m9_bayonet";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 562; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 577; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 581; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 8)
							{
								int iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
								*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iDagger;
								if (worldmodel)* worldmodel->ModelIndex() = iDagger + 1;
								*pWeapon->fixskins() = 516;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_push";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 10)
							{
								int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
								*pWeapon->ModelIndex() = Navaja; // m_nModelIndex
								*pWeapon->ViewModelIndex() = Navaja;
								if (worldmodel)* worldmodel->ModelIndex() = Navaja + 1;
								*pWeapon->fixskins() = 520;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_gypsy_jackknife";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 11) // Shadow Daggers
							{
								int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
								*pWeapon->ModelIndex() = Stiletto; // m_nModelIndex
								*pWeapon->ViewModelIndex() = Stiletto;
								if (worldmodel)* worldmodel->ModelIndex() = Stiletto + 1;
								*pWeapon->fixskins() = 522;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_stiletto";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 12)
							{
								int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
								*pWeapon->ModelIndex() = Ursus; // m_nModelIndex
								*pWeapon->ViewModelIndex() = Ursus;
								if (worldmodel)* worldmodel->ModelIndex() = Ursus + 1;
								*pWeapon->fixskins() = 519;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_ursus";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
							else if (Model == 13)
							{
								int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
								*pWeapon->ModelIndex() = Talon; // m_nModelIndex
								*pWeapon->ViewModelIndex() = Talon;
								if (worldmodel)* worldmodel->ModelIndex() = Talon + 1;
								*pWeapon->fixskins() = 523;
								*pWeapon->GetEntityQuality() = 3;
								g_Menu.Config.killicon_config = "knife_widowmaker";
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
								else if (Skin == 33)
								{
									*pWeapon->FallbackPaintKit() = 567; // Black Laminate
								}
								else if (Skin == 34)
								{
									*pWeapon->FallbackPaintKit() = 574; // Autotronic
								}
								else if (Skin == 35)
								{
									*pWeapon->FallbackPaintKit() = 580; // Freehand
								}
								else if (Skin == 36)
								{
									*pWeapon->FallbackPaintKit() = 852; // 
								}
								else if (Skin == 37)
								{
									*pWeapon->FallbackPaintKit() = 853; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 854; // 
								}
								else if (Skin == 38)
								{
									*pWeapon->FallbackPaintKit() = 855; // 
								}
								else if (Skin == 39)
								{
									*pWeapon->FallbackPaintKit() = 856; // 
								}
								else if (Skin == 40)
								{
									*pWeapon->FallbackPaintKit() = 857; // 
								}
								else if (Skin == 41)
								{
									*pWeapon->FallbackPaintKit() = 858; // 
								}
							}
						}
						else
							g_Menu.Config.fix = false;
						*pWeapon->OwnerXuidLow() = 0;
						*pWeapon->OwnerXuidHigh() = 0;
						*pWeapon->FallbackWear() = 0.001;
						*pWeapon->ItemIDHigh() = 1;
					}
				}
			}

		}
}
