#include "game_screen.h"
#include "game_window.h"

#include "output_debug_stream.h"

�Q�[����ʃX�e�[�^�X last�Q�[����ʃX�e�[�^�X = �Q�[����ʃX�e�[�^�X::�s��;

�Q�[����ʃX�e�[�^�X get�Q�[����ʃX�e�[�^�X() {
	return last�Q�[����ʃX�e�[�^�X;
}

void set�Q�[����ʃX�e�[�^�X(�Q�[����ʃX�e�[�^�X const value) {
	last�Q�[����ʃX�e�[�^�X = value;
}