#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
extern PFNONINITIALIZE pMod_onInitialize;

using PFNONGAMEINITIALIZE = void (WINAPI*)();
extern PFNONGAMEINITIALIZE pMod_onGameInitialize;

using PFNONFINALIZE = void (WINAPI*)();
extern PFNONFINALIZE pMod_onFinalize;

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONGAMEINITIALIZE pMod_onGameInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

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

