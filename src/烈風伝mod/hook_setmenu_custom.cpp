#include <windows.h>
#include <string>
#include "output_debug_stream.h"
#include "game_menu.h"
#include "game_screen.h"

int nCheckMenuCount = 0;

int prevMenuCount = 0;

extern ゲーム画面ステータス lastゲーム画面ステータス;

BOOL Hook_SetMenuCustom(HWND hWnd, HMENU hMenu) {
	if (hMenu == NULL) {
		return FALSE;
	}

	int count = GetMenuItemCount(hMenu);

	if (count == 8) {
		// 「ファイル」のPOPUPを全角に変更
		changePopupString(hMenu, 1, "コマンド(&C)");
		changePopupString(hMenu, 7, "エディタ(&E)");
		changeMenuItemString(hMenu, 32902, "セーブ(&S)...");
		changeMenuItemString(hMenu, 32901, "ロード(&L)...");
		changeMenuItemString(hMenu, 32928, "ユニット情報(&U)");
		changeMenuItemString(hMenu, 32954, "武将編集(&B)");
		changeMenuItemString(hMenu, 32955, "大名編集(&D)");
		changeMenuItemString(hMenu, 32956, "城編集(&S)");
	}

	// 「ファイル」のPOPUPを全角に変更
	changePopupString(hMenu, 0, "ファイル(&F)");

	int menu_count = GetMenuItemCount(hMenu);
	if (count == 1) {
		changeMenuItemString(hMenu, 57665, "アプリケーションの終了(&X)"); // 226はリソースエディタでわかる「ユニット」のメニューID
	}

	// メニューのインスタンスが変更されている
	if (hMenu != hNB8MenuCheckChange) {
		hNB8MenuCheckChange = hMenu;
	}

	if (prevMenuCount != menu_count) {
		prevMenuCount = menu_count;

	}

	return true;
}
