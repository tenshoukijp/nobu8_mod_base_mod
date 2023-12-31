#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
using PFNONFINALIZE = void (WINAPI*)();
using PFNONMMIOOPENA = void (WINAPI*)(char*, char*);

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;
PFNONMMIOOPENA  pMod_onMmioOpenA = NULL;

// 烈風伝Mod.dllの読み込み
HMODULE hNB8ModDll = NULL;
BOOL loadNB8Mod() {

	hNB8ModDll = LoadLibrary("烈風伝.mod.dll");

	// 存在しない
	if (!hNB8ModDll) {
		MessageBoxA(NULL, "烈風伝Modが無い", "エラー", NULL);
		return FALSE;
	}

	return TRUE;
}

void freeNB8Mod() {
	if (hNB8ModDll) {
		FreeLibrary(hNB8ModDll);
		hNB8ModDll = NULL;
	}
}

HWND hNB8Wnd = NULL;
void Mod_onInitialize() {
	pMod_onInitialize = (PFNONINITIALIZE)GetProcAddress(hNB8ModDll, "onInitialize");
	if (hNB8ModDll && pMod_onInitialize) {
		pMod_onInitialize(&hNB8Wnd);
	}
}

void Mod_onMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	pMod_onMmioOpenA = (PFNONMMIOOPENA)GetProcAddress(hNB8ModDll, "onMmioOpenA");
	if (hNB8ModDll && pMod_onMmioOpenA) {
		pMod_onMmioOpenA(pszFileName, bufOverrideFileName);
	}
}


