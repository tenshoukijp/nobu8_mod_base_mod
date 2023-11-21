#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
extern PFNONINITIALIZE pMod_onInitialize;

using PFNONFINALIZE = void (WINAPI*)();
extern PFNONFINALIZE pMod_onFinalize;

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

// óÛïóì`Mod.dllÇÃì«Ç›çûÇ›
HMODULE hNB8ModDll = NULL;
BOOL loadNB8Mod() {

	hNB8ModDll = LoadLibrary("óÛïóì`.mod.dll");

	// ë∂ç›ÇµÇ»Ç¢
	if (!hNB8ModDll) {
		MessageBoxA(NULL, "óÛïóì`ModÇ™ñ≥Ç¢", "ÉGÉâÅ[", NULL);
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

