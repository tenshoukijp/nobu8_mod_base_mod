#include "dllextern.h"
#include "process.h"
#include "loadmod.h"
#include "output_debug_stream.h"
#include "dllextern.h"
#include "loadmod.h"

void onInitialize() {

    setDllFuncAddress();

    loadNB8Mod();

    Mod_onInitialize();
}


void onMmioOpenA(char *pszFileName, char*bufOverrideFileName) {
    Mod_onMmioOpenA(pszFileName, bufOverrideFileName);
}

void onFinalize() {
    freeNB8Mod();

    OutputDebugStream("óÛïóì`ÇÃèIóπ\n");
}