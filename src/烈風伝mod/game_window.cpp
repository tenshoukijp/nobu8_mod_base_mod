#include <windows.h>
#include "game_menu.h"
#include "game_font.h"
#include "output_debug_stream.h"

HWND hNB8Wnd = NULL;

HWND* referenceOfNB8Wnd = NULL;


BOOL doneCreateWindow = FALSE;
void onCreateWindow(HWND hWnd) {
	if (!doneCreateWindow) {
		// ウィンドウハンドルを保存
		hNB8Wnd = hWnd;

		OutputDebugStream("ウィンドウ生成\n");

		// 呼び出し元(WinMM.dll)に、「今回の起動ではちゃんとウィンドウが生成されたよ」と伝える。よってゲーム本体が起動された。
		*referenceOfNB8Wnd = hWnd;

		// メニューがころころ変わるので、メニューの変更を検知するため。最初にこれを代入。主にHook_SetMenuで、使われている。
		hNB8MenuCheckChange = GetMenu(hNB8Wnd);

		// メニューを追加した
		addMenuItem(GetSystemMenu(hNB8Wnd, FALSE), "メモ帳起動(&M)", RESOURCE_MENU_ID_EXIT, ADDITIONAL_MENU_ID_NOTEPAD);

		OutputDebugStream("メニューを追加した\n");

		OutputDebugStream("烈風伝の開始\n");
	}

	doneCreateWindow = TRUE;
}

BOOL doneDestroyWindow = FALSE;
void onDestroyWindow() {
	if (!doneDestroyWindow) {
		OutputDebugStream("ウィンドウ破棄\r\n");
	}

	doneDestroyWindow = TRUE;
}
