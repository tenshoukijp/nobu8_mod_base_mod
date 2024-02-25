#include <windows.h>

#include "javascript_mod.h"

using namespace System;
using namespace Microsoft::Win32;
using namespace System::Drawing;
using namespace System::Windows::Forms;


BOOL fontExistCheck();
bool isFontExist = false;
void onFontInitialize() {
	isFontExist = fontExistCheck();
}

//------------------------------------------------
// �㏑���p�̃t�H���g���B�����t�H���g�����\�B������JavaScript�������O���e�L�X�g����ύX�ł���悤�ɂ������B
//------------------------------------------------
char bufferCustomFontName[32] = "�� ����";
char bufferDefaultFontName[32] = "";
std::string getNB8FontName() {
	std::string js_fontname = callJSModRequestFont();
	if (js_fontname != "") {
		// �t�H���g�����㏑������
		strcpy_s(bufferCustomFontName, js_fontname.c_str());
		return bufferCustomFontName;
	}

	else if (isFontExist) {
		return bufferCustomFontName;
	}
	// ���̂܂ܕԂ�
	return bufferDefaultFontName;
}

ref class FontGlobal {
public:
	static System::Drawing::Text::PrivateFontCollection^ pfc;
};

BOOL fontExistCheck() {

	// �V�X�e���ł͂Ȃ��A�v���C�x�[�g�t�H���g�Ƃ��āA���[�U�[��p�Ƃ��ăC���X�g�[�������t�H���g����������
	FontGlobal::pfc = gcnew System::Drawing::Text::PrivateFontCollection();
	RegistryKey^ key = nullptr;
	// ��r�Ώۂ̃t�H���g�B
	String^ targetFontName = gcnew String(bufferCustomFontName);
	try
	{
		key = Registry::CurrentUser->OpenSubKey(R"(Software\Microsoft\Windows NT\CurrentVersion\Fonts)");
		array<String^>^ valueNames = key == nullptr ? nullptr : key->GetValueNames();
		if (valueNames != nullptr)
		{
			for each (String ^ valueName in valueNames)
			{
				String^ path = key->GetValue(valueName)->ToString();
				if (valueName->Contains(targetFontName)) {
					// �t�H���g�����w��̃t�H���g�����܂�ł���ꍇ�́A�t�H���g��ǉ�����
					FontGlobal::pfc->AddFontFile(path);
				}
				FontGlobal::pfc->AddFontFile(path);
			}

			// �Ǝ��t�H���g�ꗗ�Ɏw��̃t�H���g�����݂��邩�ǂ������`�F�b�N����
			for each (System::Drawing::FontFamily ^ font in FontGlobal::pfc->Families) {
				if (font->Name == targetFontName) {
					isFontExist = true;
					return TRUE;
				}
			}
		}
	}
	catch (Exception^ /*ex1*/) {

	}
	finally
	{
		if (key != nullptr) {
			key->Close();
		}
	}

	try {
		// �Ǝ��t�H���g�ł͂Ȃ��ŏ�����E�B���h�E�Y�ɓ����Ă���t�H���g���w�肵�Ă��邩������Ȃ��B
		System::Drawing::Text::InstalledFontCollection^ fonts = gcnew System::Drawing::Text::InstalledFontCollection();

		for each (System::Drawing::FontFamily ^ font in fonts->Families) {
			if (font->Name == targetFontName) {
				isFontExist = true;
				return TRUE;
			}
		}
	}
	catch (Exception^ /*ex2*/) {

	}
	finally {
	}

	return FALSE;
}