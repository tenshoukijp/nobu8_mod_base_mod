#include <windows.h>
#include <winuser.h>
#include <stdio.h>

#include "game_window.h"
#include "process.h"



//------------------------------------------------
// �g���Ă��Ȃ��BHook_CreateFontACustom()�ŏ�������悤�ɂ����B
//------------------------------------------------
void onFontInitialize() {
}

//------------------------------------------------
// �㏑���p�̃t�H���g���B�����t�H���g�����\�B������JavaScript�������O���e�L�X�g����ύX�ł���悤�ɂ������B
//------------------------------------------------
char bufferCustomFontName[32] = "�� ����";
char* getNB8FontName() {
	return bufferCustomFontName;
}