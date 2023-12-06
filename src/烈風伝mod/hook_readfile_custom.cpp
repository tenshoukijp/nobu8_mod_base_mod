#include <Windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "hook_readfile_custom.h"
#include "output_debug_stream.h"
#include "javascript_mod.h"


// 顔の画像は幅が96, 高さが120。マジックナンバーになってしまうが、今後変更になったりは永久にしないため、そのまま埋め込む。(そっちの方が定数名使うよりわかりやすい)

struct KAO_PICLINE {
    BYTE at[KAO_PIC_WIDTH];
};
struct KAO_PICTURE {
    KAO_PICLINE line[KAO_PIC_HIGHT];
};

KAO_PICTURE picKaoFileOrigin = { 0 };
KAO_PICTURE picKaoFlipSidePp = { 0 };

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good(); // ファイルが存在すればtrueを返す
}
extern int nTargetKaoID;
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestBushouKaoID(nTargetKaoID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "KAODATA\\%04d.bmp", nTargetKaoID);
    }

    std::string filename = filenameBuf;
    if (!fileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("ファイル" + filename + "を開くことができませんでした。\n");
        return FALSE;
    }

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAO_PIC_WIDTH * KAO_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    // 元の画像をコピー
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(&picKaoFileOrigin, buffer.data(), buffer.size());

        // 上下反転したものを picfileOrigin→picflipSidePp にコピー
        for (int i = 0; i < KAO_PIC_HIGHT; i++) {
            memcpy(&(picKaoFlipSidePp.line[KAO_PIC_HIGHT -1 - i]), &(picKaoFileOrigin.line[i]), KAO_PIC_WIDTH);
        }

        memcpy(lpBuffer, &picKaoFlipSidePp, (KAO_PIC_WIDTH * KAO_PIC_HIGHT));
    }


    return TRUE;
}






struct KAHOU_PICLINE {
    BYTE at[KAHOU_PIC_WIDTH];
};
struct KAHOU_PICTURE {
    KAHOU_PICLINE line[KAHOU_PIC_HIGHT];
};

KAHOU_PICTURE picItemFileOrigin = { 0 };
KAHOU_PICTURE picItemFlipSidePp = { 0 };

extern int nTargetKahouGazouID;
BOOL Hook_ReadFileCustom_KahouPic(
    HANDLE hFile, // ファイルのハンドル
    LPVOID lpBuffer, // データの格納先
    DWORD nNumberOfBytesToRead, // 読み込むバイト数
    LPDWORD lpNumberOfBytesRead, // 実際に読み込んだバイト数
    LPOVERLAPPED lpOverlapped // オーバーラップ構造体のポインタ
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestKahouPicID(nTargetKahouGazouID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "ITEMCG\\%03d.bmp", nTargetKahouGazouID);
    }

    std::string filename = filenameBuf;
    if (!fileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("ファイル" + filename + "を開くことができませんでした。\n");
        return FALSE;
    }

    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("ファイル" + filename + "の読み込みに失敗しました。\n");
        return FALSE;
    }

    // 元の画像をコピー
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(&picItemFileOrigin, buffer.data(), buffer.size());

        // 上下反転したものを picfileOrigin→picflipSidePp にコピー
        for (int i = 0; i < KAHOU_PIC_HIGHT; i++) {
            memcpy(&(picItemFlipSidePp.line[KAHOU_PIC_HIGHT -1 - i]), &(picItemFileOrigin.line[i]), KAHOU_PIC_WIDTH);
        }

        memcpy(lpBuffer, &picItemFlipSidePp, (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT));
    }


    return TRUE;
}
