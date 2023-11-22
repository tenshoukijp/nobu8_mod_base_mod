#pragma once

#include <windows.h>

BOOL loadNB8Mod();

void freeNB8Mod();

void Mod_onInitialize();

void Mod_onMmioOpenA(char * pszFileName, char *bufOverrideFileName);

