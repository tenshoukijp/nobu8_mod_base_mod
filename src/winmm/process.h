#pragma once

#include <string>

extern BOOL isTargetProcessing;

BOOL IsWow64();

std::string GetCurrentProcessFullPath();

// ���ݎ��s���̎����̃v���Z�X�̃E�B���h�E�n���h���𓾂�B
HWND GetCurrentWindowHandle();

// ���݂̃v���Z�X�̃��C���N���X�́A�w��̃N���X���ƈ�v���邩�H
HWND GetNB8WindowHandle(const char* programName);