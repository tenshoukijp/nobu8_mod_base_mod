/*
 * Copyright (c) 2023-2024 �V�ċL.jp
 * under the MIT License
 */

#pragma once

using namespace System;

// dynamic�^�𗘗p���Ȃ����́Bdynamic���Ăяo�������B�������邱�ƂŁAC++�ƃ����N���邱�Ƃ��ł���(���\�b�h�̌������킹�邱�Ƃ��o����)
public ref class IJavaScriptStaticLib
{
public:
	static void onCreateWindow(int hWnd);

	static String^ onRequestFont();

	static String^ onRequestBGM(String^ filepath);

	static String^ onRequestSound(String^ filepath);

	static void onDestroyWindow();
};