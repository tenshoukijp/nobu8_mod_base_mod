#pragma once

#include <string>

void callJSModCreateWindow(HWND hWnd);

std::string callJSModRequestFont();

void callJSModMmioOpenA(char* pszFileName, char *bufOverrideFileName);

void callJSModDestroyWindow();

std::string callJSModRequestBushouKaoID(int iKaoID);

std::string callJSModRequestHimeKaoID(int iKaoID);

std::string callJSModRequestKahouPicID(int iPicID);

std::string callJSModRequestKamonPicID(int iPicID);

std::string callJSModRequestFile(const char* pszFileName);
