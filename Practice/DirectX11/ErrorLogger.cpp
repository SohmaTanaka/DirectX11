#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
	std::string error_message = "Error: " + message;

	//String用
	//最初の引数は親ウィンドウもしくはそのハンドラ
	//三番目はウィンドウタイトル
	//最後のはボックスタイプ（今回はアイコンの種類）（オプション）
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	//LはWideStringを利用する宣言のようなもの
	std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
	
	//WideString用
	//最初の引数は親ウィンドウもしくはそのハンドラ
	//三番目はウィンドウタイトル
	//最後のはボックスタイプ（今回はアイコンの種類）（オプション）
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}
