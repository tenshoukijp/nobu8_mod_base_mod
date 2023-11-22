#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"

using PFNONCREATEWINDOW = void (WINAPI*)(HWND);
using PFNONDESTROYWINDOW = void (WINAPI*)();
using PFNONREQUESTFONT = char* (WINAPI*)();

PFNONCREATEWINDOW pMod_onCreateWindow = NULL;
PFNONDESTROYWINDOW pMod_onDestroyWindow = NULL;
PFNONREQUESTFONT pMod_onRequestFont = NULL;


// óÛïóì`Mod.dllÇÃì«Ç›çûÇ›
HMODULE hJavaScriptModDll = NULL;

void callJSModCreateWindow(HWND hWnd) {
	hJavaScriptModDll = LoadLibrary("JavaScript.mod.dll");

	// ë∂ç›ÇµÇ»Ç¢
	if (!hJavaScriptModDll) {
		MessageBoxA(NULL, "JavaScript.mod.dll", "ÉGÉâÅ[", NULL);
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

std::string bufRequestFont = "";
std::string callJSModRequestFont() {
	if (hJavaScriptModDll) {
		pMod_onRequestFont = (PFNONREQUESTFONT)GetProcAddress(hJavaScriptModDll, "onRequestFont");
		if (pMod_onRequestFont) {
			OutputDebugStream("callJSModRequestFont");
			bufRequestFont = pMod_onRequestFont();
			return bufRequestFont;
		}
		else {
			OutputDebugStream("NOT callJSModRequestFont!!");
		}
	}
	return bufRequestFont;
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