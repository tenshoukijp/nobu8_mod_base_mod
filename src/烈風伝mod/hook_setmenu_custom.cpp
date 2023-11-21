#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_menu.h"
#include "game_screen.h"

int nCheckMenuCount = 0;

int prevMenuCount = 0;

extern �Q�[����ʃX�e�[�^�X last�Q�[����ʃX�e�[�^�X;

BOOL Hook_SetMenuCustom(HWND hWnd, HMENU hMenu) {
	if (hMenu == NULL) {
		return FALSE;
	}

	int count = GetMenuItemCount(hMenu);

	if (count == 8) {
		// �u�t�@�C���v��POPUP��S�p�ɕύX
		changePopupString(hMenu, 1, "�R�}���h(&C)");
		changePopupString(hMenu, 7, "�G�f�B�^(&E)");
		changeMenuItemString(hMenu, 32902, "�Z�[�u(&S)...");
		changeMenuItemString(hMenu, 32901, "���[�h(&L)...");
		changeMenuItemString(hMenu, 32928, "���j�b�g���(&U)");
		changeMenuItemString(hMenu, 32954, "�����ҏW(&B)");
		changeMenuItemString(hMenu, 32955, "�喼�ҏW(&D)");
		changeMenuItemString(hMenu, 32956, "��ҏW(&S)");
	}

	// �u�t�@�C���v��POPUP��S�p�ɕύX
	changePopupString(hMenu, 0, "�t�@�C��(&F)");

	int menu_count = GetMenuItemCount(hMenu);
	if (count == 1) {
		changeMenuItemString(hMenu, 57665, "�A�v���P�[�V�����̏I��(&X)"); // 226�̓��\�[�X�G�f�B�^�ł킩��u���j�b�g�v�̃��j���[ID
	}

	// ���j���[�̃C���X�^���X���ύX����Ă���
	if (hMenu != hNB8MenuCheckChange) {
		hNB8MenuCheckChange = hMenu;
	}

	if (prevMenuCount != menu_count) {
		prevMenuCount = menu_count;

	}

	return true;
}
