#pragma once

#include <windows.h>
#include <string>

void OutputDebugStream(std::string strOutputString);

void OutputDebugStream(int number);

void OutputDebugStream(const char* format, ...);