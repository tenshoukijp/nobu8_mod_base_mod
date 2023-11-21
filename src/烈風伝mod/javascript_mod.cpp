#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"

using PFNONCREATEWINDOW = void (WINAPI*)(HWND);
using PFNONDESTROYWINDOW = void (WINAPI*)();

PFNONCREATEWINDOW pMod_onCreateWindow = NULL;
PFNONDESTROYWINDOW pMod_onDestroyWindow = NULL;


// �󕗓`Mod.dll�̓ǂݍ���
HMODULE hJavaScriptModDll = NULL;

void callJSModCreateWindow(HWND hWnd) {
	hJavaScriptModDll = LoadLibrary("JavaScript.mod.dll");

	// ���݂��Ȃ�
	if (!hJavaScriptModDll) {
		MessageBoxA(NULL, "JavaScript.mod.dll", "�G���[", NULL);
	}

	if (hJavaScriptModDll) {
		pMod_onCreateWindow = (PFNONCREATEWINDOW)GetProcAddress(hJavaScriptModDll, "onCreateWindow");
		if (pMod_onCreateWindow) {
			OutputDebugStream("callJSModCreateWindow");
			pMod_onCreateWindow(hWnd);
		}
		else {
			OutputDebugStream("NOT callJSModCreateWindow!!");
		}
	}
}

void callJSModDestroyWindow() {
	if (hJavaScriptModDll) {

		pMod_onDestroyWindow = (PFNONDESTROYWINDOW)GetProcAddress(hJavaScriptModDll, "onDestroyWindow");
		if (pMod_onDestroyWindow) {
			OutputDebugStream("callJSModDestoryWindow");
			pMod_onDestroyWindow();
		}
		else {
			OutputDebugStream("NOT callJSModDestoryWindow!!");
		}

		FreeLibrary(hJavaScriptModDll);
		hJavaScriptModDll = NULL;
	}
}