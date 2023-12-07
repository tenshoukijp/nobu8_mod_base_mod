#include <windows.h>
#include <string>
#include <wow64apiset.h>
#include "dllmain.h"
#include "process.h"
#include "dllextern.h"
#include "on_event.h"



void onInitialize();
void onFinalize();

extern HWND hNB8Wnd;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        {
            if (!IsWow64()) {
                MessageBox(NULL, "エラー", "「SysWow64」環境ではないようです。", NULL);
                return FALSE;
            }

            // C:\Windows\SysWow64のパスを取得。ほとんど全ての人はCドライブから変更していないとは思うが...
            char sysWow64Path[512] = "";
            UINT hasWow64Path = GetSystemWow64DirectoryA(sysWow64Path, sizeof(sysWow64Path));
            if (hasWow64Path == 0) {
                MessageBox(NULL, "エラー", "「SysWow64」のパスを取得できませんでした。", NULL);
                return FALSE;
            }
            strcat_s(sysWow64Path, "\\winmm.dll");

            hOriginalDll = LoadLibrary(sysWow64Path);
            if (hOriginalDll == NULL) {
                MessageBox(NULL, "エラー", "「SysWow64」内のwinmm.dllをロードできませんでした。", NULL);
                return FALSE;
            }

            setDllFuncAddress();
        }

        // プロセスのフルパスに「Nobunaga8WPK.exe」が含まれているか？
        std::string fullPathName = GetCurrentProcessFullPath();
        // 含まれているなら、烈風伝のexeが起動したということ。
        if (fullPathName.find("Nobunaga8WPK.exe") != std::string::npos) {
            // ここまで来たら「烈風伝本体」のexeが起動したということ。
            onInitialize();
        }

        break;
    }
    case DLL_THREAD_ATTACH:
    {
        break;
    }
    case DLL_THREAD_DETACH:
    {
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        if (hNB8Wnd) {
            onFinalize();
            hNB8Wnd = NULL;
        }

        if (hOriginalDll) {
            FreeLibrary(hOriginalDll);
            hOriginalDll = NULL;
        }

        break;
    }
    default:
    {
        break;
    }

    } // switch

    return TRUE;
}

