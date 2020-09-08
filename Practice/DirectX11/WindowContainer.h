#pragma once

#include "RenderWindow.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "Graphics.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow renderWindow;
	KeyboardClass keyboard;
	MouseClass mouse;
	Graphics gfx;
};