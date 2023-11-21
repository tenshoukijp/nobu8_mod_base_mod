#pragma once

#include <windows.h>

extern HWND hNB8Wnd;

extern HWND* referenceOfNB8Wnd;

#define NB8_WINDOW_CLASS_NAME "KOEI_NOBU8WINDOW"

void onCreateWindow(HWND hWnd);

void onDestroyWindow();