#pragma once


enum class ゲーム画面ステータス {
	ウィンドウ無し = 0,
	起動画面 = 1,
	初期設定画面 = 2,
	戦略画面 = 3,
	野戦画面 = 4,
	籠城戦画面 = 5,
	不明 = 0xFFFF,
};

extern enum class ゲーム画面ステータス getゲーム画面ステータス();

extern void setゲーム画面ステータス(enum class ゲーム画面ステータス status);