#include <windows.h>
#include "output_debug_stream.h"

void loadAntiDebug() {
	// Steam�̃A���`�f�o�b�O������ɃA���`���A�u���C�N�|�C���g�n���ׂ����̂����Ȃ�͈̔͂Ŏ�����
	HMODULE hModule = LoadLibraryA("AntiDebug.mod.dll");
	if (hModule) {
		OutputDebugStream("AntiDebug.mod.dll�̃��[�h");
	}
}