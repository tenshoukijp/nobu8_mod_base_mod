#include <windows.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "hook_readfile_custom.h"
#include "output_debug_stream.h"
#include "javascript_mod.h"
#include "file_attribute.h"


// ��̉摜�͕���96, ������120�B�}�W�b�N�i���o�[�ɂȂ��Ă��܂����A����ύX�ɂȂ�����͉i�v�ɂ��Ȃ����߁A���̂܂ܖ��ߍ��ށB(�������̕����萔���g�����킩��₷��)

struct BUSHOUKAO_PICLINE {
    BYTE at[KAO_PIC_WIDTH];
};
struct BUSHOUKAO_PICTURE {
    BUSHOUKAO_PICLINE line[KAO_PIC_HIGHT];
};

BUSHOUKAO_PICTURE picBushouKaoFileOrigin = { 0 };
BUSHOUKAO_PICTURE picBushouKaoFlipSidePp = { 0 };

extern int nTargetKaoID;
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestBushouKaoID(nTargetKaoID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\KAODATA\\%04d.bmp", nTargetKaoID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAO_PIC_WIDTH * KAO_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // ���̉摜���R�s�[
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(&picBushouKaoFileOrigin, buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAO_PIC_HIGHT; i++) {
            memcpy(&(picBushouKaoFlipSidePp.line[KAO_PIC_HIGHT -1 - i]), &(picBushouKaoFileOrigin.line[i]), KAO_PIC_WIDTH);
        }

        memcpy(lpBuffer, &picBushouKaoFlipSidePp, (KAO_PIC_WIDTH * KAO_PIC_HIGHT));
    }


    return TRUE;
}





struct HIMEKAO_PICLINE {
    BYTE at[KAO_PIC_WIDTH];
};
struct HIMEKAO_PICTURE {
    HIMEKAO_PICLINE line[KAO_PIC_HIGHT];
};

HIMEKAO_PICTURE picHimeKaoFileOrigin = { 0 };
HIMEKAO_PICTURE picHimeKaoFlipSidePp = { 0 };

extern int nTargetHimeKaoID;
BOOL Hook_ReadFileCustom_HimeKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestHimeKaoID(nTargetHimeKaoID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\HIMEDATA\\%04d.bmp", nTargetHimeKaoID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAO_PIC_WIDTH * KAO_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAO_PIC_WIDTH * KAO_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // ���̉摜���R�s�[
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(&picHimeKaoFileOrigin, buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAO_PIC_HIGHT; i++) {
            memcpy(&(picHimeKaoFlipSidePp.line[KAO_PIC_HIGHT - 1 - i]), &(picHimeKaoFileOrigin.line[i]), KAO_PIC_WIDTH);
        }

        memcpy(lpBuffer, &picHimeKaoFlipSidePp, (KAO_PIC_WIDTH * KAO_PIC_HIGHT));
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
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    char filenameBuf[512] = "";
    std::string jsOverridePath = callJSModRequestKahouPicID(nTargetKahouGazouID);
    if (jsOverridePath != "") {
        strcpy_s(filenameBuf, jsOverridePath.c_str());
    }
    else {
        sprintf_s(filenameBuf, "OVERRIDE\\ITEMCG\\%03d.bmp", nTargetKahouGazouID);
    }

    std::string filename = filenameBuf;
    if (!isFileExists(filename)) {
        return FALSE;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        OutputDebugStream("�t�@�C��" + filename + "���J�����Ƃ��ł��܂���ł����B\n");
        return FALSE;
    }

    file.seekg(-(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT), std::ios::end);

    std::vector<char> buffer(KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT);
    file.read(buffer.data(), buffer.size());

    if (file.fail()) {
        OutputDebugStream("�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂����B\n");
        return FALSE;
    }

    // ���̉摜���R�s�[
    if (nNumberOfBytesToRead == buffer.size()) {
        memcpy(&picItemFileOrigin, buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAHOU_PIC_HIGHT; i++) {
            memcpy(&(picItemFlipSidePp.line[KAHOU_PIC_HIGHT -1 - i]), &(picItemFileOrigin.line[i]), KAHOU_PIC_WIDTH);
        }

        memcpy(lpBuffer, &picItemFlipSidePp, (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT));
    }


    return TRUE;
}
