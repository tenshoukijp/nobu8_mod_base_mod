
#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged


/*
0085AFCC  D0 3B 52 00 50 40 52 00 B0 76 5E 00 D0 76 5E 00  �;R.P@R.�v^.�v^.
0085AFDC  A0 7D 5E 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  �}^.�@R.�@R.�@R.
0085AFEC  6F 70 65 6E 69 6E 67 2E 61 76 69 00 F0 29 88 00  opening.avi.�E�E
0085AFFC  90 7F 5E 00 B0 3B 52 00 D0 3B 52 00 50 40 52 00  �E^.�;R.�;R.P@R.

005E7795   6A 00            PUSH 0
005E7797   68 ECAF8500      PUSH Nobunaga.0085AFEC                                      ; ASCII "opening.avi"
005E779C   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005E779F   50               PUSH EAX

0084DC4C  90 40 52 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  �@R.�@R.�@R.�@R.
0084DC5C  65 6E 64 69 6E 67 2E 61 76 69 00 00 11 01 00 00  ending.avi....
0084DC6C  00 00 00 00 6E 14 00 00 6E 14 00 00 38 00 00 00  ....n..n..8...

0050339F   6A 01            PUSH 1
005033A1   68 5CDC8400      PUSH Nobunaga.0084DC5C                                      ; ASCII "ending.avi"
005033A6   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005033A9   50               PUSH EAX
*/


int iOpeningMovieFileNameCharPointer = NULL;

static char bufOverrideMovieName[512] = "";

void OnSSRExeOpeningMovieFileNameExecute() {
	iOpeningMovieFileNameCharPointer = 0x85AFEC; // "opening.avi" �Ƃ���������ւ̃|�C���^�[

	OutputDebugStream("������Movie���ʂ�܂���:");
	OutputDebugStream("�l %s\n", iOpeningMovieFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iOpeningMovieFileNameCharPointer != NULL) {
		// OVERRIDE�t�H���_�ɑΉ�����t�@�C��������΁A������̗p����
		string overrideMovieName = string("OVERRIDE\\") + string((char*)iOpeningMovieFileNameCharPointer);
		if (isFileExists(overrideMovieName)) {
			OutputDebugStream("�I�[�o�[���C�h�t�@�C�������݂���̂ŁA������g��\n");
			strcpy_s(bufOverrideMovieName, overrideMovieName.c_str());
			iOpeningMovieFileNameCharPointer = (int)bufOverrideMovieName;
		}
	}
}


/*
005E7795   6A 00            PUSH 0
005E7797   68 ECAF8500      PUSH Nobunaga.0085AFEC                                      ; ASCII "opening.avi"
005E779C   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005E779F   50               PUSH EAX
*/

int pSSRExeJumpFromToOnSSRExeOpeningMovieFileName = 0x5E7797; // �֐��͂��̃A�h���X����AOnSSRExeOpeningMovieFileName�ւƃW�����v���Ă���B
int pSSRExeReturnLblFromOnSSRExeOpeningMovieFileName = 0x5E779C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeOpeningMovieFileName() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnSSRExeOpeningMovieFileNameExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		push iOpeningMovieFileNameCharPointer

		jmp pSSRExeReturnLblFromOnSSRExeOpeningMovieFileName
	}
}
#pragma warning(default: 4733) // ���[�j���O�̗}������������



char cmdOnSSRExeJumpFromOpeningMovieFileName[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnSSRExeOpeningMovieFileName() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnSSRExeOpeningMovieFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeOpeningMovieFileName + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnSSRExeJumpFromOpeningMovieFileName + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeOpeningMovieFileName), cmdOnSSRExeJumpFromOpeningMovieFileName, 5, NULL);
}

#pragma managed
