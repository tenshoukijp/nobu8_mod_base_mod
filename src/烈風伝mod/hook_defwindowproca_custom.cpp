#include <windows.h>
#include "output_debug_stream.h"
#include "game_window.h"
#include "game_menu.h"

using namespace std;

/*
WM_ACTIVATE                 �E�C���h�E���A�N�e�B�u�E��A�N�e�B�u�ɂȂ�Ƃ�
WM_ACTIVATEAPP              �ʂ̃A�v���P�[�V�����̃E�C���h�E���A�N�e�B�u�E��A�N�e�B�u�ɂȂ�Ƃ�
WM_ASKCBFORMATNAME          �N���b�v�{�[�h�̃t�H�[�}�b�g���𕶎���o�b�t�@�ɃR�s�[��v��
WM_CANCELMODEJOURNAL        �W���[�i���������L�����Z�����ꂽ
WM_CANCELMODE               ���b�Z�[�W�{�b�N�X���̃_�C�A���O�{�b�N�X���\������悤�Ƃ��Ă���
WM_CAPTURECHAINGED          �}�E�X�̃L���v�`�����ω�����
WM_CHANGECBCHAIN            �N���b�v�{�|�h�r���[���`�F�[�����ύX�����
WM_CHAR                     �L�[�{�[�h���當�����͂��󂯎����
WM_CHARTOITEM               WM_CHAR�ɉ����ăI�[�i�[�`�惊�X�g�{�b�N�X���󂯎��
WM_CHILDACTIVATE            MDI�R���g���[���̎q�E�C���h�E���A�N�e�B�u��
WM_CHOOSEFONT_GETLOGFONT    [�t�H���g�̎w��]�_�C�A���O�{�b�N�X�ɑ��郁�b�Z�[�W
WM_CLEAR                    �e�L�X�g�{�b�N�X�E�R���{�{�N�X�̑I���e�L�X�g���폜
WM_CLOSE                    �E�C���h�E���邢�̓A�v���P�[�V�������N���[�Y���ꂽ
WM_COMMAND                  ���j���[���I�����ꂽ���邢�̓R���g���[���ɃC�x���g����������
WM_COMPAREITEM              ���ёւ����w�肳�ꂽ�V�������ڂ̃��X�g�{�b�N�X�Ȃǂő��ΓI�Ȉʒu�����肷��B
WM_COMPACTING               �V�X�e�����������s���̏�Ԃɂ���
WM_COPY                     �e�L�X�g�{�b�N�X�E�R���{�{�b�N�X�̑I���e�L�X�g���N���b�v�{�[�h�ɃR�s�[�����Ƃ�
WM_COPYDATA                 �ʂ̃v���Z�X��̃E�B���h�E�Ƀf�[�^�𑗐M����
WM_CREATE                   �E�C���h�E���쐬��
WM_CTLCOLORBTN              �R�}���h�{�^�����`�悳��悤�Ƃ��Ă���
WM_CTLCOLORDLG              �_�C�A���O���`�悳��悤�Ƃ��Ă���
WM_CTLCOLOREDIT             �e�L�X�g�{�b�N�X���`�悳��悤�Ƃ��Ă���
WM_CTLCOLORLISTBOX          ���X�g�{�b�N�X���`�悳��悤�Ƃ��Ă���
WM_CTLCOLORMSGBOX           ���b�Z�[�W�{�b�N�X���`�悳��悤�Ƃ��Ă���
WM_CTLCOLORSCROLLBAR        �X�N���[���o�[���`�悳��悤�Ƃ��Ă���
WM_CTLCOLORSTATIC           ���x�����`�悳��悤�Ƃ��Ă���
WM_CUT                      �e�L�X�g�{�b�N�X�E�R���{�{�b�N�X�̑I���e�L�X�g���N���b�v�{�[�h�ɃR�s�[
WM_DEADCHAR                 �A�N�Z���g�L���̂悤�ȕ����Ƒg�������������󂯎����
WM_DELETEITEM               �I�[�i�`�惊�X�g�{�b�N�X�̍��ڂ��폜���ꂽ
WM_DESTROY                  �E�C���h�E���j�����ꂽ
WM_DESTROYCLIPBOARD         EmptyClipboard()�ɂ���ăN���b�v�{�[�h����ɂȂ���
WM_DEVICECHANGE             �n�[�h�E�F�A�̍\�����ύX���ꂽ
WM_DEVMODECHANGE            WIN.INI �̃f�t�H���g�̃f�o�C�X�����ύX���ꂽ
WM_DISPLAYCHANGE            �f�B�X�v���C�̉𑜓x���ύX���ꂽ
WM_DRAWCLIPBOARD            �N���b�v�{�[�h�̓��e���ς����
WM_DRAWITEM                 �I�[�i�[�`��R���g���[���̊O�ς��ς����
WM_DROPFILES                �t�@�C���}�l�[�W������� Drag & Drop
WM_ENABLE                   �E�C���h�E���L���E�����ɂȂ���
WM_ENDSESSION               Windows ���I������
WM_ENTERIDLE                �_�C�A���O�{�b�N�X�E���j���[�����͑҂�
WM_ENTERMENULOOP            ���[�_���ȃ��j���[���[�v�ɓ������Ƃ�
WM_ENTERSIZEMOVE            �E�B���h�E���ړ��E�T�C�Y�ύX����
WM_ERASEBKGND               �o�b�N�O���E���h������(�ăy�C���g)
WM_EXITMENULOOP             ���[�_���ȃ��j���[���甲���o����
WM_EXITSIZEMOVE             �E�B���h�E�̈ړ��E�T�C�Y�ύX�̏������I�������Ƃ�
WM_FONTCHANGE               �V�X�e���t�H���g���ύX���ꂽ
WM_GETDLGCODE               �R���g���[���Ɋ֘A�t����ꂽ�E�C���h�E�ɑ����郁�b�Z�[�W
WM_GETFONT                  �e�L�X�g�{�b�N�X���x���������ݎg���Ă���t�H���g�̃n���h����ݒ�E�擾����
WM_GETHOTKEY                �E�B���h�E�Ɋ֘A�t�����Ă���z�b�g�L�[���擾����
WM_GETICON                  �E�C���h�E�Ɋ֘A�t�����Ă���A�C�R���̃n���h�����擾
WM_GETMINMAXINFO            �t�H�[���̍ő�E�ŏ������ɉ\�ȃT�C�Y���擾
WM_GETTEXT                  �R���g���[���̃L���v�V�����E�e�L�X�g�{�b�N�X�̃e�L�X�g���o�b�t�@�ɃR�s�[
WM_GETTEXTLENGTH            �E�B���h�E�Ɋ֘A�t�����Ă���e�L�X�g�̃T�C�Y(�o�C�g��)���擾
WM_HELP                     [F1] or [?]�̉����ꂽ�ꍇ
WM_HOTKEY                   RegisterHotKey�֐��œo�^���ꂽ�z�b�g�L�[�������ꂽ�Ƃ�
WM_HSCROLL                  �����X�N���[���o�[�𒲐����Ă���
WM_HSCROLLCLIPBOARD         �N���b�v�{�[�h�r���[���Ő����X���N�[���o�[���g��ꂽ
WM_ICONERASEBKGND           �N���X�A�C�R�������鎞�A�A�C�R���̕`��O�ɒʒm
WM_IME_CHAR                 IME���ϊ���̕�����������
WM_IME_COMPOSITION          �L�[���͂ɂ����IME���m���̏�Ԃ�ύX����
WM_IME_COMPOSITIONFULL      �m���̕�����\������E�C���h�E�ɃX�y�[�X���Ȃ�
WM_IME_CONTROL              IME�ɃR�}���h�𑗂�
WM_IME_ENDCOMPOSITION       �������m�肵��
WM_IME_KEYDOWN              �L�[��������
WM_IME_KEYUP                �L�[�𗣂���
WM_IME_NOTIFY               IME�E�C���h�E�̕ω����A�v���P�[�V�����ɒm�点��
WM_IME_REQUEST              �V�X�e����IME��ύX���悤�Ƃ��Ă���
WM_IME_SELECT               �V�X�e����IME��ύX���悤�Ƃ��Ă���
WM_IME_SETCONTEXT           IME�̓��̓E�C���h�E���A�N�e�B�u�ɂȂ������A�v���P�[�V�����ɒʒm����
WM_IME_STARTCOMPOSITION     �m�肷��O�ɒʒm����
WM_INITDIALOG               �_�C�A���O�{�b�N�X���\�������
WM_INITMENU                 ���j���[���\������钼�O�ɒʒm
WM_INITMENUPOPUP            �|�b�v�A�b�v���j���[���\������钼�O�ɒʒm
WM_INPUTLANGCHANGE          ���ꃍ�P�[�����ύX���ꂽ�Ƃ��g�b�v���x���E�B���h�E�ɒʒm
WM_INPUTLANGCHANGEREQUEST   ���ꃍ�P�[�����ύX���ꂽ�Ƃ��A���̓t�H�[�J�X�����E�B���h�E�ɒʒm
WM_KEYDOWN                  �L�[�{�[�h�̃L�[�������ꂽ
WM_KEYUP                    �L�[��������ꂽ
WM_KILLFOCUS                �t�H�[�J�X���Ȃ��Ȃ钼�O�ɒʒm
WM_LBUTTONDBLCLK            ���̃}�E�X�{�^�����_�u���N���b�N����
WM_LBUTTONDOWN              ���̃}�E�X�{�^����������
WM_LBUTTONUP                ���̃}�E�X�{�^����������ꂽ
WM_MBUTTONDBLCLK            �����̃}�E�X�{�^�����_�u���N���b�N���ꂽ
WM_MBUTTONDOWN              �����̃}�E�X�{�^���������ꂽ
WM_MBUTTONUP                �����̃}�E�X�{�^����������ꂽ
WM_MDIACTIVATE              MDI�Ŏw�肵���q�E�C���h�E���A�N�e�B�u�ɂ���
WM_MDICASCADE               MDI�q�E�C���h�E���J�X�P�[�h�\���ɂ���
WM_MDICREATE                MDI�q�E�C���h�E���쐬����
WM_MDIDESTROY               MDI�q�E�C���h�E��j������
WM_MDIGETACTIVE             ���݃A�N�e�B�u��MDI�q�E�C���h�E�̃n���h�����擾
WM_MDIICONARRANGE           MDI�q�E�C���h�E�̃A�C�R���𐮗�
WM_MDIMAXIMIZE              MDI�q�E�C���h�E���ő剻
WM_MDINEXT                  ���̎q�E�C���h�E���A�N�e�B�u�ɂ���
WM_MDIRESTORE               �ő剻�E�ŏ�������Ă���q�E�C���h�E�����̃T�C�Y�ɖ߂�
WM_MDISETMENU               ���j���[��MDI�t���[���E�C���h�E�Ƀ����N
WM_MDITILE                  MDI�q�E�C���h�E���^�C������
WM_MEASUREITEM              �I�[�i�[�`��R���g���[�����쐬���ꂽ�Ƃ�
WM_MENUCHAR                 ���j���[�̃A�N�Z�X�L�[�̉p�����ƈ�v���Ȃ��������󂯎����
WM_MENUSELECT               ���j���[�������\���E�I�����ꂽ
WM_MOUSEACTIVATE            �}�E�X�N���b�N�ɂ��A�N�e�B�u�E�C���h�E���ڂ���
WM_MOUSEMOVE                �}�E�X���ړ�����
WM_MOUSEWHELL               �}�E�X�z�C�[������]����
WM_MOVE                     �E�C���h�E���ړ�����
WM_MOVING                   ���T�C�Y���ɃE�B���h�E�ɑ�����
WM_NCACTIVATE               ��N���C�A���g�̈悪�A�N�e�B�u�E��A�N�e�B�u�ł���
WM_NCCALCSIZE               �E�C���h�E�̃T�C�Y���Čv�Z����
WM_NCCREATE                 �E�C���h�E�̃X�^�C�����ύX���ꂽ���ɍŏ��ɑ����郁�b�Z�[�W
WM_NCDESTROY                ��N���C�A���g�̈悪�j��
WM_NCHITTEST                ��N���C�A���g�̈�̂ǂ̕����Ń}�E�X�����삳�ꂽ�����ׂ�
WM_NCLBUTTONDBCLK           ��N���C�A���g�̈�ō��{�^�����_�u���N���b�N
WM_NCLBUTTONDOWN            ��N���C�A���g�̈�ō��{�^��������
WM_NCLBUTTONUP              ��N���C�A���g�̈�ō��{�^�������
WM_NCMBUTTONDBCLK           ��N���C�A���g�̈�Œ����{�^�����_�u���N���b�N
WM_NCMBUTTONDOWN            ��N���C�A���g�̈�Œ����{�^��������
WM_NCMBUTTONUP              ��N���C�A���g�̈�Œ����{�^�������
WM_NCMOUSEMOVE              ��N���C�A���g�̈�Ń}�E�X���ړ�
WM_NCPAINT                  ��N���C�A���g�̈�ōăy�C���g���K�v
WM_NCRBUTTONDBCLK           ��N���C�A���g�̈�ŉE�{�^�����_�u���N���b�N
WM_NCRBUTTONDOWN            ��N���C�A���g�̈�ŉE�{�^��������
WM_NCRBUTTONUP              ��N���C�A���g�̈�ŉE�{�^�������
WM_NEXTDLGCTL               ���̓t�H�[�J�X��ʂ̎q�E�C���h�E�Ɉړ�
WM_NEXTMENU                 ���j���[�o�[�܂��̓V�X�e�����j���[�������E�̖��L�[���g�p���Ĉړ������Ƃ�
WM_NOTIFY                   �R���g���[���ɃC�x���g�������������A�܂��̓R���g���[�����f�[�^��v�����Ă���
WM_NULL                     ���ꎩ�͉̂������Ȃ������b�Z�[�W�𑀍삷��ꍇ�Ɏg��
WM_PAINT                    �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ�
WM_PAINTCLIPBOARD           �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ�
WM_PAINTICON                �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ�
WM_PALETTECHANGED           �V�X�e���p���b�g���ύX���ꂽ
WM_PALETTEISCHANGING        �V�X�e���p���b�g���ʂ̃E�C���h�E�ɂ���ĕύX����悤�Ƃ��Ă���
WM_PASTE                    �e�L�X�g�{�b�N�X�A�R���{�{�b�N�X�ɂ����ăN���b�v�{�[�h����R�s�[����Ƃ�����
WM_PARENTNOTIFY             ����C�x���g���q�E�C���h�E�Ŕ����������Ƃ�e�E�C���h�E�ɒʒm����
WM_POWER                    �V�X�e�����p���[�ߖ��Ԃɓ���E�o�邱�ƒʒm����
WM_POWERBROADCAST           �p���[�Ǘ��C�x���g�����o���邽�߂ɑ����Ă���
WM_PRINT                    �w��̃f�o�C�X�R���e�L�X�g�̍ĕ`���v��
WM_PRINTCLIENT              �w��̃f�o�C�X�R���e�L�X�g���̃E�B���h�E�̃N���C�A���g�̈�̕`���v��
WM_QUERYDRAGICON            �A�C�R����\�����悤�Ƃ��鎞�ɃN���X�A�C�R���������Ȃ��E�C���h�E�ɑ���
WM_QUERYENDSESSION          Windows���I�����鎞�ɑS�ẴE�C���h�E�ɖ⍇�킹��
WM_QUERYNEWPALETTE          ���̓t�H�[�J�X�����鎞�A�_���p���b�g�Œu�����邩�ǂ����⍇�킹��
WM_QUERYOPEN                �A�C�R�������ꂽ�E�C���h�E���������鎞�A���O�̈ʒu�ƃT�C�Y��v������Ă��邩�ǂ����⍇�킹��
WM_QUEUESYNC                ���[�U�[���̓��b�Z�[�W�ƃt�b�N�v���V�[�W������̃��b�Z�[�W����ʂ���
WM_QUIT                     �A�v���P�[�V��������������Ō�̃��b�Z�[�W
WM_RBUTTONDBCLK             �E�̃}�E�X�{�^�����_�u���N���b�N����
WM_RBUTTONDOWN              �E�}�E�X�{�^����������
WM_RBUTTONUP                �E�}�E�X�{�^�����������
WM_RENDERALLFORMATS         �f�B���C�h�����_�����O���Ă���Ƃ��A�N���b�v�{�[�h�̃I�[�i�[���j�������O�ɑ�����
WM_RENDERFORMAT             �f�B���C�h�����_�����O�����Ă���f�[�^���A�v���P�[�V�������v�������Ƃ�
WM_SETCURSOR                �}�E�X���L���v�`�����Ă���E�C���h�E���Ȃ����A�}�E�X������E�C���h�E�ɑ�����
WM_SETFOCUS                 �E�C���h�E�����̓t�H�[�J�X�𓾂�
WM_SETFONT                  �_���t�H���g��ݒ肷��
WM_SETHOTKEY                �z�b�g�L�[���E�B���h�E�Ɋ֘A�t����
WM_SETICON                  �A�C�R�����E�C���h�E�Ɋ֘A�t����
WM_SETREDRAW                �R���g���[���̍ĕ`�摀��𐧌䂷��
WM_SETTEXT                  �E�C���h�E(�R���g���[��)�̃^�C�g���E�e�L�X�g��ύX
WM_SETTINGCHANGE            �V�X�e���S�̂ɉe������ݒ�ύX�������Ƃ�
WM_SHOWWINDOW               �E�C���h�E���\���E��\���ɂȂ���
WM_SIZE                     �E�C���h�E�̃T�C�Y���ύX
WM_SIZECLIPBOARD            �N���b�v�{�[�h�r���[���̃N���C�A���g�̈�̃T�C�Y���ύX�����Ƃ�
WM_SIZING                   �E�B���h�E�̃T�C�Y���ύX��
WM_SPOOLERSTATUS            ����L���[�Ɉ���W���u���ǉ��E�폜���ꂽ
WM_STYLECHANGED             SetWindowLong�֐��ɂ���ăE�C���h�E�X�^�C����ύX����
WM_STYLECHANGING            SetWindowLong�֐��ɂ���ăE�C���h�E�X�^�C����ύX���ꂻ��
WM_SYNCPAINT                �ʂ̃X���b�h�ō쐬���ꂽ�E�B���h�E����ɏd�Ȃ����Ƃ�
WM_SYSCHAR                  [Alt]�L�[�Ƃ̑g�����ȊO��WM_CHAR�Ɠ���
WM_SYSCOLORCHANGE           �V�X�e���J���[���ύX���ꂽ
WM_SYSCOMMAND               �V�X�e�����j���[�����삳�ꂽ
WM_SYSDEADCHAR              �A�N�Z���g�L���̂悤�ȕʂ̕����Ƒg�������������͂��󂯎����
WM_SYSKEYDOWN               �V�X�e���������Ӗ�����L�[��������
WM_SYSKEYUP                 �V�X�e���������Ӗ�����L�[��������ꂽ
WM_TIMECHANGE               �V�X�e���^�C�}���ύX���ꂽ
WM_TIMER                    SetTimer()�Őݒ肵�����Ԃ��o��
WM_UNDO                     �e�L�X�g�{�b�N�X�� Undo ��������s����
WM_USER                     ���[�U�[����`�ł��郁�b�Z�[�W�̎g�p�̈��\�������ł��ꎩ�̂ɈӖ��͂Ȃ�
WM_USERCHANGED              ���O�I���E���O�I�t���ɑS�ẴE�B���h�E�ɑ�����
WM_VKEYTOITEM               ���X�g�{�b�N�X��WM_KEYDOWN�ɉ��������Ƃ�
WM_VSCROLL                  ���[�U�[�������X�N���[���o�[�𒲐�����
WM_VSCROLLCLIPBOARD         �N���b�v�{�[�h�r���[�����ŃX�N���[���C�x���g����
WM_WINDOWPOSCHANGED         SetWindowPos()��Ɉʒu�ƃT�C�Y��ύX�����E�C���h�E�ɑ�����
WM_WINDOWPOSCHANGING        SetWindowPos()�ňʒu�ƃT�C�Y��ύX���Ă���E�C���h�E�ɑ�����
WM_WININICHANGE             WIN.INI���ύX���ꂽ
*/

/*
���b�Z�[�W�͈̔�

���b�Z�[�W��ʂ��ĉ��炩�̒ʒm��v�����s�������ꍇ�A WM_COPYDATA�𗘗p����̂��K���������ʓI�Ƃ͌���܂���B
�ǂ̂悤�ȃ��b�Z�[�W�ł���A����𓖎��ҊԂ��ʐM�̍��}�ƔF���ł���΂悢�̂ł�����A
�Ǝ��ɒ�`�������l�����b�Z�[�W�Ƃ��ė��p����̂��܂��ꋻ�ł��B

�������A���̂悤�ȏꍇ�A���̒�`�������l�������̃��b�Z�[�W�Əd�����Ă͂Ȃ�܂��񂩂�A
�\�ߗ��p�\�ȃ��b�Z�[�W�͈̔͂�m���Ă����Ȃ���΂Ȃ�܂���B


���b�Z�[�W�͈̔�

�Ӗ�

0����WM_USER - 1    �V�X�e���ɂ���Ē�`����Ă��郁�b�Z�[�W�B
WM_USER����0x7FFF   �X�̃E�C���h�E�N���X�̂��߂̃��b�Z�[�W�B ���Ƃ��΁A�c�[���`�b�v�R���g���[�����A�N�e�B�u�ɂ���TTM_ACTIVATE�́A WM_USER + 1�Ƃ��Ē�`����Ă���B
WM_APP����0xBFFF    �A�v���P�[�V���������R�Ɏg���Ă悢�Ƃ����͈́B
0xC000����0xFFFF    RegisterWindowMessage���Ԃ��l�B
0xFFFF���傫���l  �����̂��߂ɗ\�񂳂�Ă���B

���ڂ��ׂ��Ƃ����WM_APP�ł��B ���̃��b�Z�[�W����0xBFFF�܂ł̒l�̓A�v���P�[�V���������R�Ɏg�����Ƃ�������Ă��邽�߁A
�I���W�i���ȃ��b�Z�[�W�Ƃ��ăA�v���P�[�V�����Ԃ̒ʐM�ɗ��p�ł��܂��B
WM_COPYDATA�̓f�[�^�̑��M��ړI�Ƃ������b�Z�[�W�ł����A�P���ɒʒm��v���Ȃǂ��s�������ꍇ��WM_APP�̂ق����K�؂ł��傤�B

#define WM_LAYOUTCHANGE WM_APP
�E
�E
PostMessage(hwndTarget, WM_LAYOUTCHANGE, 0, 0); // SendMessage�ł��悢


���̃R�[�h�ł́AWM_APP���Ӑ}�I��WM_LAYOUTCHANGE�Ƃ����`�Œ�`���Ă��܂����A
����̓��b�Z�[�W�̖����𖼑O���琄���ł���悤�ɂ��邽�߂ł��B
���Ƃ��΁A�ΏۂƂȂ�E�C���h�E�Ƀ��C�A�E�g�̕ύX�𑣂��̂ł���΁A
����ɑ��������O���g�����ق����R�[�h�͕�����₷���Ȃ�܂��B
wParam��lParam�͎��R�Ɏg�����Ƃ��ł��܂����A
�����܂ő����̂�4�o�C�g�̃f�[�^��2�܂łƂȂ�܂��B
�����̃I���W�i�����b�Z�[�W���K�v�ȏꍇ�́AWM_APP+n(n��0����0xBFFF)�Ƃ����`�Œ�`���܂��B
*/

WNDPROC wpOrigWndProc = NULL;

// Subclass procedure 
LRESULT APIENTRY NB8WndProcCustom(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam)
{

	if (Msg == WM_CLOSE) {
		OutputDebugStream("WM_CLOSE\n");
	}
	else if (Msg == WM_DESTROY) {
		OutputDebugStream("WM_DESTROY\n");
		onDestroyWindow();
	}
	else if (Msg == WM_COMMAND) {
		WORD menuID = LOWORD(wParam);
		onMenuPushed(menuID);
	}
	// �}�E�X�n�̏����S��
	else if (WM_MOUSEFIRST <= Msg && Msg <= WM_MOUSELAST) {
	}
	// �A�v������̃A�C�R�����E�N���������ɏo�Ă���V�X�e���R�}���h��I�񂾂Ƃ�
	else if (Msg == WM_SYSCOMMAND) {
		OutputDebugStream("WM_SYSCOMMAND\n");

		WORD menuID = LOWORD(wParam);
		onSystemMenuPushed(menuID);
	}
	else if (Msg == WM_NCMOUSEMOVE) {
	}
	else if (Msg == WM_MENUSELECT) {
	}
	else if (Msg == WM_INITMENUPOPUP) {
	}
	else if (WM_KEYFIRST <= Msg && Msg <= WM_KEYLAST) {
	}
	else if (Msg == WM_CONTEXTMENU) {
	}
	else if (Msg == WM_QUERYOPEN) {
	}
	else if (Msg == WM_PAINT) {
	}
	else if (Msg == WM_KILLFOCUS) {
	}
	else if (Msg == WM_SETFOCUS) {
	}
	else if (Msg == WM_COPYDATA) {
	}
	else if (WM_ACTIVATE) {
	}

	return CallWindowProc(wpOrigWndProc, hWnd, Msg, wParam, lParam);
}


// �摜�n�̃n�b�N
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	if (Msg == WM_CREATE) {
		char pszClassName[256] = { 0 };
		GetClassName(hWnd, pszClassName, _countof(pszClassName));
		if (string(pszClassName) == NB8_WINDOW_CLASS_NAME) {
			// �E�B���h�E�����̃^�C�~���O�ŁA�E�B���h�E�v���V�[�W��������Mod���̂��̂Ŏw���͂���
			wpOrigWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NB8WndProcCustom);

			onCreateWindow(hWnd);
		}
	}

	return 0;
}




