#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
	std::string error_message = "Error: " + message;

	//String�p
	//�ŏ��̈����͐e�E�B���h�E�������͂��̃n���h��
	//�O�Ԗڂ̓E�B���h�E�^�C�g��
	//�Ō�̂̓{�b�N�X�^�C�v�i����̓A�C�R���̎�ށj�i�I�v�V�����j
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	//L��WideString�𗘗p����錾�̂悤�Ȃ���
	std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
	
	//WideString�p
	//�ŏ��̈����͐e�E�B���h�E�������͂��̃n���h��
	//�O�Ԗڂ̓E�B���h�E�^�C�g��
	//�Ō�̂̓{�b�N�X�^�C�v�i����̓A�C�R���̎�ށj�i�I�v�V�����j
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}
