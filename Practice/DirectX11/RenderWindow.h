#pragma once

#include "ErrorLogger.h"

//循環依存回避に基づく前方宣言
class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * pWindowContainer,HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessage();	//trueの時、ウィンドウは閉じている
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL;	//自身のハンドル
	HINSTANCE hInstance = NULL; //アプリケーションのインスタンスへのハンドル
	std::string window_title = "";
	std::wstring window_title_wide = L"";	//WideString版ウィンドウタイトル
	std::string window_class = "";
	std::wstring window_class_wide = L"";	//WideString版ウィンドウクラス

	int width = 0;
	int height = 0;
};