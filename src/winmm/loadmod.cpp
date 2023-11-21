#include <windows.h>
#include "loadmod.h"

using PFNONINITIALIZE = void (WINAPI*)(void*);
extern PFNONINITIALIZE pMod_onInitialize;

using PFNONFINALIZE = void (WINAPI*)();
extern PFNONFINALIZE pMod_onFinalize;

PFNONINITIALIZE pMod_onInitialize = NULL;
PFNONFINALIZE   pMod_onFinalize = NULL;

// �󕗓`Mod.dll�̓ǂݍ���
HMODULE hNB8ModDll = NULL;
BOOL loadNB8Mod() {

	hNB8ModDll = LoadLibrary("�󕗓`.mod.dll");

	// ���݂��Ȃ�
	if (!hNB8ModDll) {
		MessageBoxA(NULL, "�󕗓`Mod������", "�G���[", NULL);
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

