#include "output_debug_stream.h"

void OutputDebugStream(std::string strOutputString) {
#ifdef SUPER_RELEASE
	return;
#endif
	OutputDebugString(strOutputString.c_str());
}

void OutputDebugStream(int number) {
#ifdef SUPER_RELEASE
	return;
#endif
	OutputDebugString(std::to_string(number).c_str());
}

char szBufOutputDebugStream[4096] = "";
void OutputDebugStream(const char* format, ...) {
#ifdef SUPER_RELEASE
	return;
#endif

	int length = sizeof(szBufOutputDebugStream) - 1;

	va_list arg;

	va_start(arg, format);
	_vsnprintf_s(szBufOutputDebugStream, length, _TRUNCATE, format, arg);
	va_end(arg);

	/*
	// �Ō�̕��������s�ł͂Ȃ��ꍇ
	if (szBufOutputDebugStream[strlen(szBufOutputDebugStream) - 1] != '\n') {
		// ���s�𑫂��B
		strcat(szBufOutputDebugStream, "\n");
	}
	*/

	szBufOutputDebugStream[length - 1] = '\0'; // �ԕ��B�ň��̏ꍇ�͍Ō�̉��s������ׂ��B

	// OutputDebugString�̕��ɏo���B
	OutputDebugString(szBufOutputDebugStream);
}