/*
 * Copyright (c) 2023-2024 天翔記.jp
 * under the MIT License
 */

#pragma once

using namespace System;

// dynamic型を利用しないもの。dynamicを呼び出すだけ。こうすることで、C++とリンクすることができる(メソッドの個数を合わせることが出来る)
public ref class IJavaScriptStaticLib
{
public:
	static void onCreateWindow(int hWnd);

	static void onDestroyWindow();
};