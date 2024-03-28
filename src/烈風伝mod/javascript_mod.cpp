#include <windows.h>

#include "javascript_mod.h"
#include "output_debug_stream.h"
#include "mng_������ϊ�.h"

void callJSModCreateWindow(HWND hWnd) {
	IJavaScriptMod::on���C���E�B���h�E������((int)hWnd);
}

std::string callJSModRequestFont() {
	System::String^ fontname = IJavaScriptMod::on�t�H���g�v����();
	if (System::String::IsNullOrEmpty(fontname)) { return ""; }
	return to_native_string(fontname);
}

void callJSModDestroyWindow() {
	IJavaScriptMod::on���C���E�B���h�E�j���O();
}

void callJSModMmioOpenA(char* pszFileName, char* bufOverrideFileName) {
	System::String^ filepath = gcnew System::String(pszFileName);
	if (filepath->ToLower()->StartsWith("bgm\\")) {
		System::String^ ret = IJavaScriptMod::on���y�v����(filepath->ToLower());
		if (!System::String::IsNullOrEmpty(ret)) {
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
	else {
		System::String^ ret = IJavaScriptMod::on���ʉ��v����(filepath->ToLower());
		if (!System::String::IsNullOrEmpty(ret)) {
			strcpy_s(bufOverrideFileName, 512, to_native_string(ret).c_str());
		}
	}
}

std::string callJSModRequestBushouKaoID(int iKaoID) {
	System::String^ filepath = IJavaScriptMod::on��摜�v����(iKaoID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}

std::string callJSModRequestHimeKaoID(int iKaoID) {
	System::String^ filepath = IJavaScriptMod::on�P��摜�v����(iKaoID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
	return "";
}


std::string callJSModRequestKahouPicID(int iPicID) {
	System::String^ filepath = IJavaScriptMod::on�ƕ�摜�v����(iPicID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}

std::string callJSModRequestKamonPicID(int iPicID) {
	System::String^ filepath = IJavaScriptMod::on�Ɩ�摜�v����(iPicID);
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}



std::string callJSModRequestFile(const char* pszFileName) {
	System::String^ filepath = IJavaScriptMod::on�t�@�C���v����(gcnew System::String(pszFileName));
	if (System::String::IsNullOrEmpty(filepath)) { return ""; }
	return to_native_string(filepath);
}
