
#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged


/*
0085AFCC  D0 3B 52 00 50 40 52 00 B0 76 5E 00 D0 76 5E 00  ﾐ;R.P@R.ｰv^.ﾐv^.
0085AFDC  A0 7D 5E 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  }^.ｰ@R.ﾐ@R.R.
0085AFEC  6F 70 65 6E 69 6E 67 2E 61 76 69 00 F0 29 88 00  opening.avi.・・
0085AFFC  90 7F 5E 00 B0 3B 52 00 D0 3B 52 00 50 40 52 00  ・^.ｰ;R.ﾐ;R.P@R.

005E7795   6A 00            PUSH 0
005E7797   68 ECAF8500      PUSH Nobunaga.0085AFEC                                      ; ASCII "opening.avi"
005E779C   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005E779F   50               PUSH EAX

0084DC4C  90 40 52 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  拭R.ｰ@R.ﾐ@R.R.
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
	iOpeningMovieFileNameCharPointer = 0x85AFEC; // "opening.avi" という文字列へのポインター

	OutputDebugStream("★★★Movieが通りますよ:");
	OutputDebugStream("値 %s\n", iOpeningMovieFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iOpeningMovieFileNameCharPointer != NULL) {
		// OVERRIDEフォルダに対応するファイルがあれば、それを採用する
		string overrideMovieName = string("OVERRIDE\\") + string((char*)iOpeningMovieFileNameCharPointer);
		if (isFileExists(overrideMovieName)) {
			OutputDebugStream("オーバーライドファイルが存在するので、それを使う\n");
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

int pSSRExeJumpFromToOnSSRExeOpeningMovieFileName = 0x5E7797; // 関数はこのアドレスから、OnSSRExeOpeningMovieFileNameへとジャンプしてくる。
int pSSRExeReturnLblFromOnSSRExeOpeningMovieFileName = 0x5E779C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプする

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeOpeningMovieFileName() {
	// スタックにためておく
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

	// スタックに引き出す
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
#pragma warning(default: 4733) // ワーニングの抑制を解除する



char cmdOnSSRExeJumpFromOpeningMovieFileName[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnSSRExeOpeningMovieFileName() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnSSRExeOpeningMovieFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeOpeningMovieFileName + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnSSRExeJumpFromOpeningMovieFileName + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeOpeningMovieFileName), cmdOnSSRExeJumpFromOpeningMovieFileName, 5, NULL);
}

#pragma managed
