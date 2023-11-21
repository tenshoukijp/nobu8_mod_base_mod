#pragma once

#include <shellapi.h>
#include <string>

/*
MENUITEM "終了(&X)",  181

MENUITEM "武将エディタ(&B)",  313
MENUITEM "城エディタ(&C)",  314
*/

#define RESOURCE_MENU_ID_EXIT		57665   // 「終了」のメニューのリソースIDは57665 (Nobunaga8WPK.exe をリソースハッカーで見ろ)
#define RESOURCE_MENU_ID_BUSHOUEDIT	32954   // 「武将ｴﾃﾞｨﾀ」のメニューのリソースIDは32954 (Nobunaga8WPK.exe をリソースハッカーで見ろ)

#define ADDITIONAL_MENU_ID_NOTEPAD	    59000  // 59000　からの独自メニュースタート
#define ADDITIONAL_MENU_ID_MEMORYEDITOR	59001  

extern HMENU hNB8MenuCheckChange;

void addMenuItem(HMENU hTargetMenu, std::string menuname, int positionID, int menuID);

BOOL onMenuPushed(int iMenuID);

BOOL onSystemMenuPushed(int iMenuID);

void changeMenuItemString(HMENU hMenu, UINT iMenuID, const char* szNewString);

void changePopupString(HMENU hMenu, UINT iByPosition, const char* szNewString);
