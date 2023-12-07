extern void WriteAsmJumperOnSSRExeOpeningMovieFileName();
extern void WriteAsmJumperOnSSRExeEndingMovieFileName();

bool iWriteAsmJumperOnSSRExeOpeningMovieFileName = false;
bool iWriteAsmJumperOnSSRExeEndingMovieFileName = false;

void hookFunctionsDirect() {
	if (!iWriteAsmJumperOnSSRExeOpeningMovieFileName) {
		WriteAsmJumperOnSSRExeOpeningMovieFileName();
		iWriteAsmJumperOnSSRExeOpeningMovieFileName = true;
	}
	if (!iWriteAsmJumperOnSSRExeEndingMovieFileName) {
		WriteAsmJumperOnSSRExeEndingMovieFileName();
		iWriteAsmJumperOnSSRExeEndingMovieFileName = true;
	}
}