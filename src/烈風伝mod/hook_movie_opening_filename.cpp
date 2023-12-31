

#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_process.h"
#include "file_attribute.h"
#include "javascript_mod.h"
using namespace std;

#pragma unmanaged


/*
0085AFCC  D0 3B 52 00 50 40 52 00 B0 76 5E 00 D0 76 5E 00  Ð;R.P@R.°v^.Ðv^.
0085AFDC  A0 7D 5E 00 B0 40 52 00 D0 40 52 00 F0 40 52 00   }^.°@R.Ð@R.ð@R.
0085AFEC  6F 70 65 6E 69 6E 67 2E 61 76 69 00 F0 29 88 00  opening.avi.EE
0085AFFC  90 7F 5E 00 B0 3B 52 00 D0 3B 52 00 50 40 52 00  E^.°;R.Ð;R.P@R.

005E7795   6A 00            PUSH 0
005E7797   68 ECAF8500      PUSH Nobunaga.0085AFEC                                      ; ASCII "opening.avi"
005E779C   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005E779F   50               PUSH EAX

0084DC4C  90 40 52 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  @R.°@R.Ð@R.ð@R.
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
	iOpeningMovieFileNameCharPointer = 0x85AFEC; // "opening.avi" Æ¢¤¶ñÖÌ|C^[

	OutputDebugStream("MovieªÊèÜ·æ:");
	OutputDebugStream("l %s\n", iOpeningMovieFileNameCharPointer);

	ZeroMemory(bufOverrideMovieName, _countof(bufOverrideMovieName));

	if (iOpeningMovieFileNameCharPointer != NULL) {
		// OVERRIDEtH_ÉÎ·ét@Cª êÎA»êðÌp·é
		string overrideMovieName = string("OVERRIDE\\") + string((char*)iOpeningMovieFileNameCharPointer);
		if (isFileExists(overrideMovieName)) {
			OutputDebugStream("I[o[Cht@Cª¶Ý·éÌÅA»êðg¤\n");
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

int pSSRExeJumpFromToOnSSRExeOpeningMovieFileName = 0x5E7797; // ÖÍ±ÌAhX©çAOnSSRExeOpeningMovieFileNameÖÆWvµÄ­éB
int pSSRExeReturnLblFromOnSSRExeOpeningMovieFileName = 0x5E779C; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·é

#pragma warning(disable:4733)

__declspec(naked) void WINAPI OnSSRExeOpeningMovieFileName() {
	// X^bNÉ½ßÄ¨­
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

	// X^bNÉø«o·
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
#pragma warning(default: 4733) // [jOÌ}§ðð·é



char cmdOnSSRExeJumpFromOpeningMovieFileName[6] = "\xE9";
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªvToCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnSSRExeOpeningMovieFileName() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnSSRExeOpeningMovieFileName;
	int SubAddress = iAddress - (pSSRExeJumpFromToOnSSRExeOpeningMovieFileName + 5);
	// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnSSRExeJumpFromOpeningMovieFileName + 1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory(hCurrentProcess, (LPVOID)(pSSRExeJumpFromToOnSSRExeOpeningMovieFileName), cmdOnSSRExeJumpFromOpeningMovieFileName, 5, NULL);
}

#pragma managed
