#include <windows.h>
#include <winuser.h>

// dll���ǂݍ��܂ꂽ�^�C�~���O�Ńv���Z�X��x�[�X�A�h���X�͑�����Ă��܂��B��ԑ����^�C�~���O�Ŏg����悤�ɂ��Ă����B

HANDLE hCurrentProcess = ::GetCurrentProcess();

int nBaseAddress = (int)GetModuleHandle(NULL);
