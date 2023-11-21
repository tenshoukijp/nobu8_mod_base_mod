#include <windows.h>

#include "JavaScript.mod.h"

extern "C" __declspec(dllexport) void WINAPI onCreateWindow(HWND hWnd) {
	IJavaScriptStaticLib::onCreateWindow((int)hWnd);
}

extern "C" __declspec(dllexport) void WINAPI onDestroyWindow() {
	IJavaScriptStaticLib::onDestroyWindow();
}
