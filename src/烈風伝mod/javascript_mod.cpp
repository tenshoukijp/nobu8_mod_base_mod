#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"
#include "mng_•¶Žš—ñ•ÏŠ·.h"

void callJSModCreateWindow(HWND hWnd) {
	IJavaScriptMod::onCreateWindow((int)hWnd);
}

std::string callJSModRequestFont() {
	System::String^ fontname = IJavaScriptMod::onRequestFont();
	if (fontname == nullptr) { return ""; }
	return to_native_string(fontname);
}

void callJSModDestroyWindow() {
	IJavaScriptMod::onDestroyWindow();
}

void callJSModMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	System::String^ filepath = gcnew System::String(pszFileName);
	if (filepath->ToLower()->StartsWith("bgm\\")) {
		System::String^ ret = IJavaScriptMod::onRequestBGM(filepath);
		if (ret != nullptr) {
			OutputDebugString("JavaScript.mod‚ÌonMmioOpenA\n");
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
	else {
		System::String^ ret = IJavaScriptMod::onRequestSound(filepath);
		if (ret != nullptr) {
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
}