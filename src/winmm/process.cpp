#include <windows.h>
#include <string>


BOOL isTargetProcessing = FALSE;

//------------------------------------------------
// 64bitOS��Wow64�œ��삵�Ă��邩�ǂ������`�F�b�N����B(32bitOS��Steam�ł𓮍삳���Ă���ꍇ�ɃG���[�ɂ��邽��)
//------------------------------------------------
BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    // IsWow64Process �́A�T�|�[�g����Ă��� Windows �̂��ׂẴo�[�W�����Ŏg�p�ł���킯�ł͂���܂���B
    // �֐����܂�DLL�ւ̃n���h�����擾����ɂ�GetModuleHandle���g�p���܂��B
    // GetProcAddress���g�p���āA�֐��ւ̃|�C���^���擾���܂��B

    using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);

    HMODULE hKernelModuleHandle = GetModuleHandle("kernel32");
    LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;
    if (hKernelModuleHandle) {
        fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hKernelModuleHandle, "IsWow64Process");
    }

    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
        {
            //handle error
        }
    }

    return bIsWow64;
}

//------------------------------------------------
// ���݂̃v���Z�X�̃t���p�X���擾����B
//------------------------------------------------
std::string GetCurrentProcessFullPath() {
    // ���݂̃v���Z�X���`�F�b�N����B�Ȃ��Ȃ� winmm�͕����̃v���Z�X����Ă΂�Ă��܂��ł��낤����B
    HANDLE hCurProc = GetCurrentProcess();
    char fullPathName[512] = "";
    GetModuleFileNameA(NULL, fullPathName, _countof(fullPathName));

    return fullPathName;
}


//------------------------------------------------
// �v���Z�XID �� HWND �ւ̕ϊ�
// ����:�v���Z�XID
// �߂�l: HWND �������� NULL
//------------------------------------------------
HWND GetWindowHandleFromProcessId(const DWORD TargetID) {
    HWND hWnd = GetTopWindow(NULL);
    do {
        if (GetWindowLong(hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible(hWnd)) {
            continue;
        }
        DWORD ProcessID;
        GetWindowThreadProcessId(hWnd, &ProcessID);
        if (TargetID == ProcessID) {
            return hWnd;
        }
    } while ((hWnd = GetNextWindow(hWnd, GW_HWNDNEXT)) != NULL);

    return NULL;
}


//------------------------------------------------
// ���݂̃E�B���h�E�n���h���𓾂�
//------------------------------------------------
HWND GetCurrentWindowHandle() {
    // ���s���̃v���Z�XID�𓾂�
    DWORD pID = GetCurrentProcessId();
    // �v���Z�XID�˃E�B���h�E�n���h��
    HWND hWnd = GetWindowHandleFromProcessId(pID);
    return hWnd;
}

//------------------------------------------------
// ���݂̃E�B���h�E�n���h�����A�m���ɓV�ċLHD�̂��̂��ǂ������`�F�b�N
//------------------------------------------------
HWND GetNB8WindowHandle(const char *pszClassName) {

    // �Q�[�������`���[����Q�[���{�̂��ǂ��炩�̃E�B���h�E������͂��B
    HWND hWndNB8HDHandle = NULL;

    if (!hWndNB8HDHandle) { hWndNB8HDHandle = ::FindWindow(pszClassName, NULL); }

    HWND hWndCurrentHandle = GetCurrentWindowHandle();
    if (hWndNB8HDHandle == hWndCurrentHandle) {
        return hWndNB8HDHandle;
    }
    else {
        return NULL;
    }
}

