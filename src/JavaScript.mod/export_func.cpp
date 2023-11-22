#include <windows.h>
#include <string>
#include "JavaScript.mod.h"
#include "string_convert.h"


extern "C" __declspec(dllexport) void WINAPI onCreateWindow(HWND hWnd) {
	IJavaScriptStaticLib::onCreateWindow((int)hWnd);
}

std::string bufRequestFont = "";
extern "C" __declspec(dllexport) const char* WINAPI onRequestFont() {
	bufRequestFont.clear();
	System::String^ ret = IJavaScriptStaticLib::onRequestFont();
	if (ret == nullptr) { ret = ""; }
	System::Diagnostics::Trace::WriteLine("JS‚©‚ç•Ô‚Á‚Ä‚«‚½ƒtƒHƒ“ƒg" + ret);
	bufRequestFont = String_to_string(ret);
	return bufRequestFont.c_str();
}

extern "C" __declspec(dllexport) void WINAPI onDestroyWindow() {
	IJavaScriptStaticLib::onDestroyWindow();
}
