#include <windows.h>
#include "game_menu.h"
#include "game_font.h"
#include "output_debug_stream.h"

HWND hNB8Wnd = NULL;

HWND* referenceOfNB8Wnd = NULL;


BOOL doneCreateWindow = FALSE;
void onCreateWindow(HWND hWnd) {
	if (!doneCreateWindow) {
		// �E�B���h�E�n���h����ۑ�
		hNB8Wnd = hWnd;

		OutputDebugStream("�E�B���h�E����\n");

		// �Ăяo����(WinMM.dll)�ɁA�u����̋N���ł͂����ƃE�B���h�E���������ꂽ��v�Ɠ`����B����ăQ�[���{�̂��N�����ꂽ�B
		*referenceOfNB8Wnd = hWnd;

		// ���j���[�����낱��ς��̂ŁA���j���[�̕ύX�����m���邽�߁B�ŏ��ɂ�������B���Hook_SetMenu�ŁA�g���Ă���B
		hNB8MenuCheckChange = GetMenu(hNB8Wnd);

		// ���j���[��ǉ�����
		addMenuItem(GetSystemMenu(hNB8Wnd, FALSE), "�������N��(&M)", RESOURCE_MENU_ID_EXIT, ADDITIONAL_MENU_ID_NOTEPAD);

		OutputDebugStream("���j���[��ǉ�����\n");

		OutputDebugStream("�󕗓`�̊J�n\n");
	}

	doneCreateWindow = TRUE;
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		OutputDebugStream("�E�B���h�E�j��\r\n");
	}

	doneDestroyWindow = TRUE;
}
