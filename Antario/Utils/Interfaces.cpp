#include "Interfaces.h"
#include "Utils.h"
#include "..\SDK\IClientMode.h"
#include "..\SDK\IBaseClientDll.h"
#include "..\SDK\IClientEntityList.h"
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CPrediction.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\SDK\IEngineTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\ICvar.h"
#include "..\SDK\IVModelInfo.h"
#include "..\SDK\CModelRender.h"
#include "..\SDK\IMaterial.h"
#include "..\SDK\IVRenderView.h"
#include "..\SDK\CGlowObjectManager.h"
#include "..\SDK\IClientState.h"
#include <fstream>
#include "..\SDK\CHudChat.h"

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

IBaseClientDLL*     g_pClientDll    = nullptr;
IClientMode*        g_pClientMode   = nullptr;
IClientEntityList*  g_pEntityList   = nullptr;
IVEngineClient*     g_pEngine       = nullptr;
CPrediction*        g_pPrediction   = nullptr;
IGameMovement*      g_pMovement     = nullptr;
IMoveHelper*        g_pMoveHelper   = nullptr;
CGlobalVarsBase*    g_pGlobalVars   = nullptr;
IGameEventManager*  g_pEventManager = nullptr;
ISurface*           g_pSurface      = nullptr;
IEngineTrace*       g_pTrace        = nullptr;
IPhysicsSurfaceProps* g_pSurfaceData = nullptr;
ICVar*              g_pCvar			= nullptr;
IPanel*				g_pPanel		= nullptr;
IVModelInfo*		g_pModelInfo	= nullptr;
CModelRender*       g_pModelRender  = nullptr;
IMaterialSystem*    g_pMaterialSys  = nullptr;
IVRenderView*       g_pRenderView   = nullptr;
IVDebugOverlay* g_pDebugOverlay = nullptr;
IClientState* g_pClientState = nullptr;
CGlowObjectManager* g_GlowManager = nullptr;
CHudChat* g_ChatElement = nullptr;

namespace interfaces
{
	template< class T >
	T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
	{
		if (szModuleName.empty() || szInterfaceName.empty())
			return nullptr;
		typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
		CreateInterfaceFn hInterface = nullptr;
		while (!hInterface)
		{
			hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
			Sleep(5);
		}

		char pszBuffer[256];
		for (int i = 0; i < 100; i++)
		{
			sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
			PVOID pInterface = hInterface(pszBuffer, nullptr);

			if (pInterface && pInterface != NULL)
			{
				if (bSkip)
					sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

				Sleep(5);
				break;
			}
		}

		return (T*)hInterface(pszBuffer, nullptr);
	}

	template<typename T>
	T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
	{
		HMODULE moduleHandle = GetModuleHandleA(szModuleName);
		if (moduleHandle)
		{
			CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, "CreateInterface"));
			return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
		}
		Utils::Log("Error getting interface %", szInterfaceVersion);
		return nullptr;
	}

    void Init()
    {
        g_pClientDll    = FindClass<IBaseClientDLL>("client_panorama.dll", "VClient");
		g_pClientMode   = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5u);
		g_pGlobalVars   = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1Bu);
        g_pEntityList   = FindClass<IClientEntityList>("client_panorama.dll", "VClientEntityList");
        g_pEngine       = FindClass<IVEngineClient>("engine.dll", "VEngineClient");
        g_pPrediction   = FindClass<CPrediction>("client_panorama.dll", "VClientPrediction");
        g_pMovement     = FindClass<IGameMovement>("client_panorama.dll", "GameMovement");
        g_pMoveHelper   = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client_panorama.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));  
        g_pEventManager = CaptureInterface<IGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
        g_pSurface      = FindClass<ISurface>("vguimatsurface.dll", "VGUI_Surface");
		g_pTrace        = FindClass<IEngineTrace>("engine.dll", "EngineTraceClient");
		g_pSurfaceData  = FindClass<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
		g_pCvar         = FindClass<ICVar>("vstdlib.dll", "VEngineCvar");
		g_pPanel		= FindClass<IPanel>("vgui2.dll", "VGUI_Panel");
		g_pModelInfo    = FindClass<IVModelInfo>("engine.dll", "VModelInfoClient");
		g_pModelRender  = FindClass<CModelRender>("engine.dll", "VEngineModel");
		g_pMaterialSys  = FindClass<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		g_pRenderView   = FindClass<IVRenderView>("engine.dll", "VEngineRenderView");
		g_GlowManager   = *reinterpret_cast<CGlowObjectManager**>(Utils::FindSignature("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3);
		g_pDebugOverlay = CaptureInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
		g_pClientState = **reinterpret_cast<IClientState * **>((*reinterpret_cast<uintptr_t * *>(g_pEngine))[12] + 0x10u);	
		g_ChatElement = *reinterpret_cast<CHudChat * *>(reinterpret_cast<uintptr_t>(g_pClientMode) + *reinterpret_cast<uint8_t*>(Utils::FindSignature("client_panorama.dll", "E8 ? ? ? ? 8B 4F ? 85 C9 74 06 51") + 7));
		std::ofstream{ "csgo/materials/chamsAnimated.vmt" } <<
			"VertexLitGeneric { $envmap editor/cube_vertigo $envmapcontrast 1 $envmaptint \"[.7 .7 .7]\" $basetexture dev/zone_warning proxies { texturescroll { texturescrollvar $basetexturetransform texturescrollrate 0.6 texturescrollangle 90 } } }";

    }
}