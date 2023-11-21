#include <windows.h>
#include <string>
#include <shellapi.h>
#include "output_debug_stream.h"
#include "game_menu.h"
#include "game_window.h"
#include "game_test.h"

#pragma comment(lib, "shell32.lib")

using namespace std;

HMENU hNB8MenuCheckChange = NULL;

// ���j���[(�ʏ�̃A�v���̃��j���[)�̃��j���[�A�C�e�������s�������A
// �ԍ��͎�Ƀ��\�[�X�G�f�B�^�Ŋm�F�ł���
BOOL onMenuPushed(int iMenuID) {
	switch (iMenuID) {
	case ADDITIONAL_MENU_ID_MEMORYEDITOR:
		ShellExecute(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOW);
		return TRUE;
	/*
	case ***:
		on********();
		return TRUE;
	*/
	}
	return FALSE;
}

// �V�X�e�����j���[(�A�v���̍�����E�N���b�N�Ȃǂŏo�Ă��郁�j���[)�̃��j���[�A�C�e�������s�������A
BOOL onSystemMenuPushed(int iMenuID) {
	OutputDebugStream(iMenuID);
	OutputDebugStream("\r\n");
	switch (iMenuID) {
	case ADDITIONAL_MENU_ID_NOTEPAD:
		// ShellExecute(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOW);

		doGameTest();


		return TRUE;
	}
	return FALSE;
}


// ���j���[�A�C�e���̒ǉ�
// menuname ���j���[�A�C�e���̕�����B"---"���ƃZ�p���[�^�ƂȂ�B
void addMenuItem(HMENU hTargetMenu, string menuname, int positionID, int menuID) {

	MENUITEMINFO info;

	// �쐬�������j���[���ڂ�ǉ�����
	ZeroMemory(&info, sizeof(info));
	info.cbSize = sizeof(info);

	// ID�ԍ��̎w�肪�����ꍇ
	if (menuID == -1) {
		info.fMask = MIIM_TYPE;
		// ID�ԍ��̎w�肪����ꍇ
	}
	else {
		info.fMask = MIIM_TYPE | MIIM_ID;
		info.wID = menuID;
	}
	if (menuname == "---") {
		info.fType = MFT_SEPARATOR;
	}
	else {
		info.fType = MFT_STRING;
	}
	info.dwTypeData = (LPSTR)menuname.c_str();
	InsertMenuItem(hTargetMenu, positionID, FALSE, &info);
}

// ���j���[�A�C�e���̒ǉ��B���h���[�t��(���j���[�A�C�e���͒ǉ����������ł͌����ڂ��ω����Ȃ��̂�)
void addMenuItemAndRedraw(HMENU hTargetMenu, string menuname, int positionID, int menuID) {
	addMenuItem(hTargetMenu, menuname, positionID, menuID);
	DrawMenuBar(hNB8Wnd);
}



// ���j���[�A�C�e���̕������ύX����֐�
void changeMenuItemString(HMENU hMenu, UINT iMenuID, const char* szNewString)
{
	MENUITEMINFO menuItemInfo;
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask = MIIM_STRING;
	menuItemInfo.dwTypeData = (LPSTR)szNewString;

	SetMenuItemInfoA(hMenu, iMenuID, FALSE, &menuItemInfo);
}

// ���j���[�A�C�e���̕������ύX����֐�
void changePopupString(HMENU hMenu, UINT iByPosition, const char* szNewString)
{
	MENUITEMINFO menuItemInfo;
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask = MIIM_STRING;
	menuItemInfo.dwTypeData = (LPSTR)szNewString;

	SetMenuItemInfoA(hMenu, iByPosition, TRUE, &menuItemInfo);
}

