#pragma once

#include <shellapi.h>
#include <string>

/*
MENUITEM "�I��(&X)",  181

MENUITEM "�����G�f�B�^(&B)",  313
MENUITEM "��G�f�B�^(&C)",  314
*/

#define RESOURCE_MENU_ID_EXIT		57665   // �u�I���v�̃��j���[�̃��\�[�XID��57665 (Nobunaga8WPK.exe �����\�[�X�n�b�J�[�Ō���)
#define RESOURCE_MENU_ID_BUSHOUEDIT	32954   // �u������ި��v�̃��j���[�̃��\�[�XID��32954 (Nobunaga8WPK.exe �����\�[�X�n�b�J�[�Ō���)

#define ADDITIONAL_MENU_ID_NOTEPAD	    59000  // 59000�@����̓Ǝ����j���[�X�^�[�g
#define ADDITIONAL_MENU_ID_MEMORYEDITOR	59001  

extern HMENU hNB8MenuCheckChange;

void addMenuItem(HMENU hTargetMenu, std::string menuname, int positionID, int menuID);

BOOL onMenuPushed(int iMenuID);

BOOL onSystemMenuPushed(int iMenuID);

void changeMenuItemString(HMENU hMenu, UINT iMenuID, const char* szNewString);

void changePopupString(HMENU hMenu, UINT iByPosition, const char* szNewString);
