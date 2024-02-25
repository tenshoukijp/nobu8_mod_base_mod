#include <windows.h>

// dllが読み込まれたタイミングでプロセスやベースアドレスは代入してしまう。一番早いタイミングで使えるようにしておく。

HANDLE hCurrentProcess = ::GetCurrentProcess();

int nBaseAddress = (int)GetModuleHandle(NULL);
