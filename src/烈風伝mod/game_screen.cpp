#include "game_screen.h"
#include "game_window.h"

#include "output_debug_stream.h"

ゲーム画面ステータス lastゲーム画面ステータス = ゲーム画面ステータス::不明;

ゲーム画面ステータス getゲーム画面ステータス() {
	return lastゲーム画面ステータス;
}

void setゲーム画面ステータス(ゲーム画面ステータス const value) {
	lastゲーム画面ステータス = value;
}