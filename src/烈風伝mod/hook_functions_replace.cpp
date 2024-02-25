#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "output_debug_stream.h"
#include "game_font.h"
#include "game_process.h"
#include "file_attribute.h"
#include "hook_readfile_custom.h"
#include "javascript_mod.h"



// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")

using namespace std;


// ひとつのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInOneMod(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew,
    HMODULE hmodCaller)
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while (pImportDesc->Name) {
        PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0)
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
        ((PBYTE)hmodCaller + pImportDesc->FirstThunk);

    while (pThunk->u1.Function) {
        PROC* ppfn = (PROC*)&pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                hCurrentProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// すべてのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInAllMods(
    PCSTR pszModuleName,
    PROC pfnCurrent,
    PROC pfnNew)
{
    // モジュールリストを取得
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // それぞれのモジュールに対してReplaceIATEntryInOneModを実行
    while (bModuleResult) {
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}


//---------------------------WindowProcA

// フックする関数のプロトタイプを定義
using PFNDEFWINDOWPROCA = LRESULT(WINAPI *)(HWND, UINT, WPARAM, LPARAM);

PROC pfnOrigDefWindowProcA = GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");

extern LRESULT Hook_DefWindowProcACustom(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Hook_DefWindowProcA(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
    Hook_DefWindowProcACustom(hWnd, Msg, wParam, lParam);

	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
	LRESULT LPResult = ((PFNDEFWINDOWPROCA)pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam);

	return LPResult;
}


//---------------------------TextOutA

using PFNTEXTOUTA = BOOL(WINAPI *)(HDC, int, int, LPCTSTR, int);

PROC pfnOrigTextOutA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");

BOOL WINAPI Hook_TextOutA(
    HDC hdc,           // デバイスコンテキストのハンドル
    int nXStart,       // 開始位置（基準点）の x 座標
    int nYStart,       // 開始位置（基準点）の y 座標
    LPCTSTR lpString,  // 文字列
    int cbString       // 文字数
) {

    // 先にカスタムの方を実行。
    BOOL nResult = ((PFNTEXTOUTA)pfnOrigTextOutA)(hdc, nXStart, nYStart, lpString, cbString);

    return nResult;
}



//---------------------------DrawTextA
/*
00425B20  /$ 55             PUSH EBP
00425B21  |. 8BEC           MOV EBP,ESP
00425B23  |. 83EC 44        SUB ESP,44
00425B26  |. 53             PUSH EBX
00425B27  |. 56             PUSH ESI
00425B28  |. 57             PUSH EDI
00425B29  |. 894D FC        MOV DWORD PTR SS:[EBP-4],ECX
00425B2C  |. 8B45 14        MOV EAX,DWORD PTR SS:[EBP+14]
00425B2F  |. 50             PUSH EAX                                 ; /Flags
00425B30  |. 8B4D 10        MOV ECX,DWORD PTR SS:[EBP+10]            ; |
00425B33  |. 51             PUSH ECX                                 ; |pRect
00425B34  |. 8B55 0C        MOV EDX,DWORD PTR SS:[EBP+C]             ; |
00425B37  |. 52             PUSH EDX                                 ; |Count
00425B38  |. 8B45 08        MOV EAX,DWORD PTR SS:[EBP+8]             ; |
00425B3B  |. 50             PUSH EAX                                 ; |Text
00425B3C  |. 8B4D FC        MOV ECX,DWORD PTR SS:[EBP-4]             ; |
00425B3F  |. 8B51 04        MOV EDX,DWORD PTR DS:[ECX+4]             ; |
00425B42  |. 52             PUSH EDX                                 ; |hDC
00425B43  |. FF15 64088400  CALL DWORD PTR DS:[<&USER32.DrawTextA>]  ; \DrawTextA
00425B49  |. 5F             POP EDI
00425B4A  |. 5E             POP ESI
00425B4B  |. 5B             POP EBX
00425B4C  |. 8BE5           MOV ESP,EBP
00425B4E  |. 5D             POP EBP
00425B4F  \. C2 1000        RETN 10
*/

using PFNDRAWTEXTA = int(WINAPI *)(HDC, LPCTSTR, int, LPRECT, UINT);

PROC pfnOrigDrawTextA = GetProcAddress(GetModuleHandleA("user32.dll"), "DrawTextA");

int WINAPI Hook_DrawTextA(
    HDC hdc,          // デバイスコンテキストのハンドル
    LPCTSTR lpchText, // 描画する文字列
    int cchText,      // 文字列の長さ
    LPRECT lprc,      // 文字列を描画する長方形の座標
    UINT format       // フォーマットオプション
) {

    OutputDebugStream("DrawTextA%s\n", lpchText);
    OutputDebugStream("%x\n", lpchText);
    /*
    if (std::string(lpchText).find("強国に従う") != string::npos) {
        return ((PFNDRAWTEXTA)pfnOrigDrawTextA)(hdc, "これあれなんか？\xA置き換えたんか？", strlen("これあれなんか？\xA置き換えたんか？"), lprc, format);
    }
    */

	// 先にカスタムの方を実行。
    int nResult = ((PFNDRAWTEXTA)pfnOrigDrawTextA)(hdc, lpchText, cchText, lprc, format);

	return nResult;
}






//---------------------------CreateFontA
using PFNCREATEFONTA = HFONT (WINAPI *)(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR);

PROC pfnOrigCreateFontA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");

HFONT WINAPI Hook_CreateFontA(
    int    cHeight,
    int    cWidth,
    int    cEscapement,
    int    cOrientation,
    int    cWeight,
    DWORD  bItalic,
    DWORD  bUnderline,
    DWORD  bStrikeOut,
    DWORD  iCharSet,
    DWORD  iOutPrecision,
    DWORD  iClipPrecision,
    DWORD  iQuality,
    DWORD  iPitchAndFamily,
    LPCSTR pszFaceName
) {
    // ここは通過しないみたい。
    HFONT hFont = ((PFNCREATEFONTA)pfnOrigCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
    // フォントファミリーを指定のもので上書きする
    return hFont;
}



//---------------------------CreateFontIndirectA

using PFNCREATEFONTINDIRECTA = HFONT(WINAPI *)(const LOGFONTA *);

PROC pfnOrigCreateFontIndirectA = GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontIndirectA");

HFONT WINAPI Hook_CreateFontIndirectA(
    const LOGFONTA* lplf   // フォントの論理的な特性を指定する LOGFONT 構造体へのポインタ
) {
	// OutputDebugStream("フォント名:%s\n", lplf->lfFaceName);
	HFONT hFont = NULL;
	// ＭＳ 明朝 なら 将星 明朝へ
    if (std::string(lplf->lfFaceName) == "ＭＳ 明朝") {
		// OutputDebugStream("フォントを上書きします\n");
        std::string strOverrideFontName = getNB8FontName();
        if (std::string(strOverrideFontName) != "") {
            strcpy_s((char*)lplf->lfFaceName, 31, strOverrideFontName.c_str()); // 
            // OutputDebugStream(strOverrideFontName);
            hFont = ((PFNCREATEFONTINDIRECTA)pfnOrigCreateFontIndirectA)(lplf);
            return hFont;
        }
	}

    hFont = ((PFNCREATEFONTINDIRECTA)pfnOrigCreateFontIndirectA)(lplf);
	
	// フォントファミリーを指定のもので上書きする
	return hFont;
}



//---------------------------SetMenu
using PFNSETMENU = BOOL(WINAPI*)(HWND, HMENU);

PROC pfnOrigSetMenu = GetProcAddress(GetModuleHandleA("user32.dll"), "SetMenu");

extern BOOL Hook_SetMenuCustom(HWND hWnd, HMENU hMenu);

BOOL WINAPI Hook_SetMenu(HWND hWnd, HMENU hMenu) {

    // 先にカスタムの方を実行。
    Hook_SetMenuCustom(hWnd, hMenu);

    // 元のものを呼び出す
    BOOL nResult = ((PFNSETMENU)pfnOrigSetMenu)(hWnd, hMenu);


    return nResult;
}




//---------------------------ReleaseDC

using PFNRELEASEDC = int(WINAPI *)(HWND, HDC);

PROC pfnOrigReleaseDC = GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");

int WINAPI Hook_ReleaseDC(
    HWND hWnd,  // ウィンドウのハンドル
    HDC hDC     // デバイスコンテキストのハンドル
) {

	// 元のものを呼び出す
	int nResult = ((PFNRELEASEDC)pfnOrigReleaseDC)(hWnd, hDC);

	return nResult;
}





//---------------------------CreateFileA

using PFNCREATEFILEA = HANDLE(WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

PROC pfnOrigCreateFileA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
int nTargetKaoID = -1;
int nTargetHimeKaoID = -1;
int nTargetKahouGazouID = -1;
// extern HANDLE Hook_CreateFileACustom(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE hFileKAODATA = NULL;
HANDLE hFileHIMEDATA = NULL;
HANDLE hFileITEMDATA = NULL;
HANDLE WINAPI Hook_CreateFileA(
    LPCSTR lpFileName, // ファイル名
    DWORD dwDesiredAccess, // アクセス方法
    DWORD dwShareMode, // 共有方法
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
    DWORD dwCreationDisposition, // 作成方法
    DWORD dwFlagsAndAttributes, // ファイル属性
    HANDLE hTemplateFile // テンプレートファイルのハンドル
) {
    // 先にカスタムの方を実行。
    // Hook_CreateFileACustom(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    nTargetKaoID = -1;
    nTargetKahouGazouID = -1;

    HANDLE nResult;

    // JS側からファイル名の変更要求があれば、ぞれ。
    // string jsOverrideFilePath = callJSModRequestFile(lpFileName);
    // 
    // デフォルトでチェックするオーバーライドファイル
    string dfOverrideFilePath = string("OVERRIDE\\") + lpFileName;
    /*
    if (jsOverrideFilePath.size() > 0) {
        OutputDebugStream("ファイル名を上書きします。%s\n", jsOverrideFilePath.c_str());
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(jsOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
    else*/ if (isFileExists(dfOverrideFilePath)) {
        // 元のもの
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(dfOverrideFilePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    } else {
        // 元のもの
        nResult = ((PFNCREATEFILEA)pfnOrigCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    string filename = string(lpFileName);
    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) { return std::toupper(c); });
    if (filename == "KAODATA.NB8") {
        OutputDebugStream("CreateFileA:" + std::string(lpFileName) + "\n");
        hFileKAODATA = nResult;
    }
    else if (filename == "HIMEDATA.NB8") {
        OutputDebugStream("CreateFileA:" + std::string(lpFileName) + "\n");
        hFileHIMEDATA = nResult;
    }
    else if (filename == "ITEMCG.NB8") {
        OutputDebugStream("CreateFileA:" + std::string(lpFileName) + "\n");
        hFileITEMDATA = nResult;
    }
    return nResult;
}


//---------------------------SetFilePointer

using PFNSETFILEPOINTER = DWORD(WINAPI*)(HANDLE, LONG, PLONG, DWORD);

PROC pfnOrigSetFilePointer = GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetFilePointer");
// extern DWORD Hook_SetFilePointerCustom(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
DWORD WINAPI Hook_SetFilePointer(
    HANDLE hFile, // ファイルのハンドル
    LONG lDistanceToMove, // 移動量
    PLONG lpDistanceToMoveHigh, // 移動量の上位 32 ビット
    DWORD dwMoveMethod // 移動方法
) {
    // 先にカスタムの方を実行。
    // Hook_SetFilePointerCustom(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);

    nTargetKaoID = -1;
    nTargetHimeKaoID = -1;
    nTargetKahouGazouID = -1;
    // 元のもの
    DWORD nResult = ((PFNSETFILEPOINTER)pfnOrigSetFilePointer)(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
    if (hFileKAODATA == hFile) {
        const int pic_data_size = (KAO_PIC_WIDTH * KAO_PIC_HIGHT) * 1677; // 1677個の顔画像が入っている
        const int file_org_size = 19345876; // KAODATA.NB8のファイルサイズ
        const int header_size = file_org_size - pic_data_size;
        nTargetKaoID = (lDistanceToMove - header_size) / (KAO_PIC_WIDTH * KAO_PIC_HIGHT);
        OutputDebugStream("顔SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
        OutputDebugStream("顔ID:%d\n", nTargetKaoID);
    }
    else if (hFileHIMEDATA == hFile) {
        const int pic_data_size = (KAO_PIC_WIDTH * KAO_PIC_HIGHT) * 64; // 1677個の顔画像が入っている
        const int file_org_size = 738308; // KAODATA.NB8のファイルサイズ
        const int header_size = file_org_size - pic_data_size;
        nTargetHimeKaoID = (lDistanceToMove - header_size) / (KAO_PIC_WIDTH * KAO_PIC_HIGHT);
        OutputDebugStream("姫顔SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
        OutputDebugStream("顔ID:%d\n", nTargetKaoID);
    }
    else if (hFileITEMDATA == hFile) {
        const int pic_data_size = (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT) * 51; // 51個の家宝画像が入っている
        const int file_org_size = 327220; // ITEMCG.NB8のファイルサイズ
        const int header_size = file_org_size - pic_data_size;
        nTargetKahouGazouID = (lDistanceToMove - header_size) / (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
        OutputDebugStream("家宝SetFilePointer:" + std::to_string(lDistanceToMove) + "\n");
        OutputDebugStream("顔ID:%d\n", nTargetKahouGazouID);
    }
    return nResult;
}


//---------------------------ReadFile

using PFNREADFILE = BOOL(WINAPI*)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

PROC pfnOrigReadFile = GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReadFile");

extern BOOL Hook_ReadFileCustom_BushouKao(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_HimeKao(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL Hook_ReadFileCustom_KahouPic(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

BOOL WINAPI Hook_ReadFile(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    // 元のもの
    BOOL nResult = ((PFNREADFILE)pfnOrigReadFile)(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

    if (hFileKAODATA == hFile) {
        OutputDebugStream("読み込むバイト数%d", nNumberOfBytesToRead);
        Hook_ReadFileCustom_BushouKao(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    }
    else if (hFileHIMEDATA == hFile) {
        OutputDebugStream("読み込むバイト数%d", nNumberOfBytesToRead);
        Hook_ReadFileCustom_HimeKao(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    }
    else if (hFileITEMDATA == hFile) {
        OutputDebugStream("読み込むバイト数%d", nNumberOfBytesToRead);
        Hook_ReadFileCustom_KahouPic(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    }

    nTargetKaoID = -1;
    nTargetHimeKaoID = -1;
    nTargetKahouGazouID = -1;

    return nResult;
}




//---------------------------IsDebuggerPresent

using PFNISDEBUGGERPRESENT = BOOL(WINAPI *)();

PROC pfnOrigIsDebuggerPresent = GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");

BOOL WINAPI Hook_IsDebuggerPresent() {

	// 元のもの
	BOOL nResult = ((PFNISDEBUGGERPRESENT)pfnOrigIsDebuggerPresent)();

	return FALSE;
}




/*----------------------------------------------------------------*
 HOOK系処理
 *----------------------------------------------------------------*/
bool isHookDefWindowProcA = false;
bool isHookTextOutA = false;
bool isHookDrawTextA = false;
bool isHookCreateFontA = false;
bool isHookCreateFontIndirectA = false;
bool isHookSetMenu = false;
bool isHookReleaseDC = false;
bool isHookCreateFileA = false;
bool isHookSetFilePointer = false;
bool isHookReadFile = false;
bool isHookIsDebuggerPresent = false;

void hookFunctionsReplace() {

    PROC pfnOrig;
    if (!isHookDefWindowProcA) {
        isHookDefWindowProcA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "DefWindowProcA");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
    }
    if (!isHookTextOutA) {
        isHookTextOutA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "TextOutA");
        ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_TextOutA);
    }
    if (!isHookDrawTextA) {
		isHookDrawTextA = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "DrawTextA");
		ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_DrawTextA);
	}
    if (!isHookCreateFontA) {
		isHookCreateFontA = true;
        OutputDebugStream("CreateFontAをフックします\n");
		pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontA");
		ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_CreateFontA);
    }
    if (!isHookCreateFontIndirectA) {
        isHookCreateFontIndirectA = true;
        OutputDebugStream("CreateFontIndirectAをフックします\n");
        pfnOrig = ::GetProcAddress(GetModuleHandleA("gdi32.dll"), "CreateFontIndirectA");
        ReplaceIATEntryInAllMods("gdi32.dll", pfnOrig, (PROC)Hook_CreateFontIndirectA);
    }
    if (!isHookSetMenu) {
        isHookSetMenu = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "SetMenu");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_SetMenu);
    }
    if (!isHookReleaseDC) {
        isHookReleaseDC = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("user32.dll"), "ReleaseDC");
        ReplaceIATEntryInAllMods("user32.dll", pfnOrig, (PROC)Hook_ReleaseDC);
    }
    if (!isHookCreateFileA) {
        isHookCreateFileA = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateFileA");
        ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_CreateFileA);
    }
    if (!isHookSetFilePointer) {
		isHookSetFilePointer = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetFilePointer");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_SetFilePointer);
	}
    if (!isHookReadFile) {
		isHookReadFile = true;
		pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReadFile");
		ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_ReadFile);
	}
    if (!isHookIsDebuggerPresent) {
        isHookIsDebuggerPresent = true;
        pfnOrig = ::GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsDebuggerPresent");
        ReplaceIATEntryInAllMods("kernel32.dll", pfnOrig, (PROC)Hook_IsDebuggerPresent);
    }

}