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


extern int nTargetKaoID;
BOOL Hook_ReadFileCustom_BushouKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {
    if (nTargetKaoID < 0) {
		return FALSE;
	}

    std::unique_ptr<BUSHOUKAO_PICTURE> picBushouKaoFileOrigin(new BUSHOUKAO_PICTURE);
    std::unique_ptr<BUSHOUKAO_PICTURE> picBushouKaoFlipSidePp(new BUSHOUKAO_PICTURE);

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
        memcpy(picBushouKaoFileOrigin.get(), buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAO_PIC_HIGHT; i++) {
            memcpy(&(picBushouKaoFlipSidePp->line[KAO_PIC_HIGHT -1 - i]), &(picBushouKaoFileOrigin->line[i]), KAO_PIC_WIDTH);
        }

        memcpy(lpBuffer, picBushouKaoFlipSidePp.get(), (KAO_PIC_WIDTH * KAO_PIC_HIGHT));
    }


    return TRUE;
}





struct HIMEKAO_PICLINE {
    BYTE at[KAO_PIC_WIDTH];
};
struct HIMEKAO_PICTURE {
    HIMEKAO_PICLINE line[KAO_PIC_HIGHT];
};


extern int nTargetHimeKaoID;
BOOL Hook_ReadFileCustom_HimeKao(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {
    if (nTargetHimeKaoID < 0) {
        return FALSE;
    }

    std::unique_ptr<HIMEKAO_PICTURE> picHimeKaoFileOrigin(new HIMEKAO_PICTURE);
    std::unique_ptr<HIMEKAO_PICTURE> picHimeKaoFlipSidePp(new HIMEKAO_PICTURE);

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
        memcpy(picHimeKaoFileOrigin.get(), buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAO_PIC_HIGHT; i++) {
            memcpy(&(picHimeKaoFlipSidePp->line[KAO_PIC_HIGHT - 1 - i]), &(picHimeKaoFileOrigin->line[i]), KAO_PIC_WIDTH);
        }

        memcpy(lpBuffer, picHimeKaoFlipSidePp.get(), (KAO_PIC_WIDTH * KAO_PIC_HIGHT));
    }


    return TRUE;
}








struct KAHOU_PICLINE {
    BYTE at[KAHOU_PIC_WIDTH];
};
struct KAHOU_PICTURE {
    KAHOU_PICLINE line[KAHOU_PIC_HIGHT];
};


extern int nTargetKahouGazouID;
BOOL Hook_ReadFileCustom_KahouPic(
    HANDLE hFile, // �t�@�C���̃n���h��
    LPVOID lpBuffer, // �f�[�^�̊i�[��
    DWORD nNumberOfBytesToRead, // �ǂݍ��ރo�C�g��
    LPDWORD lpNumberOfBytesRead, // ���ۂɓǂݍ��񂾃o�C�g��
    LPOVERLAPPED lpOverlapped // �I�[�o�[���b�v�\���̂̃|�C���^
) {

    std::unique_ptr<KAHOU_PICTURE> picItemFileOrigin(new KAHOU_PICTURE);
    std::unique_ptr<KAHOU_PICTURE> picItemFlipSidePp(new KAHOU_PICTURE);

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
        memcpy(picItemFileOrigin.get(), buffer.data(), buffer.size());

        // �㉺���]�������̂� picfileOrigin��picflipSidePp �ɃR�s�[
        for (int i = 0; i < KAHOU_PIC_HIGHT; i++) {
            memcpy(&(picItemFlipSidePp->line[KAHOU_PIC_HIGHT -1 - i]), &(picItemFileOrigin->line[i]), KAHOU_PIC_WIDTH);
        }

        memcpy(lpBuffer, picItemFlipSidePp.get(), (KAHOU_PIC_WIDTH * KAHOU_PIC_HIGHT));
    }


    return TRUE;
}
