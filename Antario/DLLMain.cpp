#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Utils\GlobalVars.h"
#include "Menu/Menu.h"
#include "AntiLeak.h"
#include "Features/Knife/Knife.h"

HINSTANCE HThisModule;
UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

#define moongangsta 710933552
#define moongangsta2 710933552
// Idk why yall do 2/3 lines for this shit

#define niggerrias 1047255890
#define niggerrias2 1047255890

#define niggert4zzu -1575605383
#define niggert4zzu2 -1575605383

#define niggermallow 2022785656
#define niggermallow2 2022785656
#define niggermallow3 2022785656

#define niggerhl -202545503
#define niggerhl2 202545503
#define niggerhl3 -202545504

#define niggerlife -1239187743
#define niggerlife2 -1239187743
#define niggerlife3 -1239187743

#define niggermoki -458713095
#define niggermoki2 -458713095
#define niggermoki3 -458713095


#define niggermagic -145121179
#define niggermagic2 145121179
#define niggermagic3 -145121179

#define niggeragera -600794882
#define niggeragera2 +600794882
#define niggeragera3 -600794882

#define niggermoe -1096402643
#define niggermoe2 +1096402643
#define niggermoe3 -1096402643

#define evolved -1908538499
#define evolved3 -1908538499
#define evolved2 -1908538499

#define vektus -629771589
#define vektus2 -629771589
#define vektus3 -629771589

#define bando 1923034899
#define bando2 -1923034899
#define bando3 1923034844

#define kitten -1232652941
#define kitten2 -1232652941
#define kitten3 -1232652941

#define hoppa 352210298
#define hoppa2 352210298
#define hoppa3 352210298

#define mistrz -870711942
#define mistrz2 -870711942
#define mistrz3 -870711942

#define josh 1616455467
#define josh2 -1616555467
#define josh3 1616555467

#define jordan -1654552397
#define jordan2 1654552397
#define jordan3 1654352397

#define jackie -1021115483
#define jackie2 1021115483
#define jackie3 1031115483

#define pumpi -232459507
#define pumpi2 233459507
#define pumpi3 -233459507

#define moon moongangsta
#define rias niggerrias
#define t4zzuu niggert4zzu2
#define mallowlover niggermallow3
#define hL niggerhl2
#define life niggerlife
#define moki niggermoki
#define magic niggermagic2
#define agera niggeragera3
#define moe niggermoe3
#define evo evolved3
#define vek vektus3
#define band bando
#define dog kitten3
#define hoppaa hoppa3
#define polak mistrz
#define jossi josh3
#define michealjordan jordan2
#define jack jackie2
#define penispumpi pumpi3
#define alast -220404074
#define cadan 1357184389
#define faruk -165625801
#define neut 1722084745
#define hayday -1329992899

int OnDllAttach()
{
    Hooks::Init();
	knifeload();

	while (!g_Menu.Config.Unload)
		Sleep(1000);

	Hooks::Restore();

	FreeLibraryAndExitThread(HThisModule, 0);
}

bool ErasePEHeader(HMODULE hModule)
{
	hModule = GetModuleHandle(NULL);
	if ((DWORD)hModule == 0) return 0;
	DWORD IMAGE_NT_HEADER = *(int*)((DWORD)hModule + 60);
	for (int i = 0; i < 0x108; i++)
		* (BYTE*)(IMAGE_NT_HEADER + i) = 0;
	for (int i = 0; i < 120; i++)
		* (BYTE*)((DWORD)hModule + i) = 0;
	return 1;
}

bool DoUnload;

void CAntiLeak::ErasePE()
{
	char* pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH && GetModuleHandleA("csgo.exe"))
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		if (dwSerial == moon ||
			dwSerial == rias ||
			dwSerial == t4zzuu ||
			dwSerial == mallowlover ||
			dwSerial == hL ||
			dwSerial == life ||
			dwSerial == moki ||
			dwSerial == magic ||
			dwSerial == agera ||
			dwSerial == evo ||
			dwSerial == vek ||
			dwSerial == moe ||
			dwSerial == dog ||
			dwSerial == hoppaa ||
			dwSerial == polak ||
			dwSerial == band ||
			dwSerial == jossi ||
			dwSerial == michealjordan ||
			dwSerial == jack ||
			dwSerial == penispumpi ||
			dwSerial == alast ||
			dwSerial == cadan ||
			dwSerial == faruk ||
			dwSerial == neut ||
			dwSerial == hayday)
		{
			Sleep(100);
		}
		else
		{
			// when HWID rejected
			exit(0);
			return TRUE;
		}

		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}

