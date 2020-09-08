#pragma once

#include "ErrorLogger.h"

//�z�ˑ�����Ɋ�Â��O���錾
class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * pWindowContainer,HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessage();	//true�̎��A�E�B���h�E�͕��Ă���
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL;	//���g�̃n���h��
	HINSTANCE hInstance = NULL; //�A�v���P�[�V�����̃C���X�^���X�ւ̃n���h��
	std::string window_title = "";
	std::wstring window_title_wide = L"";	//WideString�ŃE�B���h�E�^�C�g��
	std::string window_class = "";
	std::wstring window_class_wide = L"";	//WideString�ŃE�B���h�E�N���X

	int width = 0;
	int height = 0;
};