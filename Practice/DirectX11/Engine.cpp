#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	timer.Start();

	//エンジンはWindowContainerの継承クラスである
	if (!this->renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!gfx.Initialize(this->renderWindow.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessage()
{
	return this->renderWindow.ProcessMessage();
}

void Engine::Update()
{
	float dt = timer.GetMilliSecondsElapsed();
	timer.Restart();

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();

		/*std::string outMsg = "Char: ";
		outMsg += ch;
		outMsg += '\n';
		OutputDebugStringA(outMsg.c_str());*/
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent keyboardEvent = keyboard.ReadKey();

		/*unsigned char keycode = keyboardEvent.GetKeyCode();
		std::string outMsg = "";

		if (keyboardEvent.IsPress())
		{
			outMsg += "keyPress: ";
		}
		else if (keyboardEvent.IsRelease())
		{
			outMsg += "keyRelease: ";
		}

		outMsg += keycode;
		outMsg += '\n';
		OutputDebugStringA(outMsg.c_str());*/
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();

		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RawMove)
			{
				this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0.0f);
			}
		}

		/*if (me.GetType() == MouseEvent::EventType::WheelUp)
		{
			OutputDebugStringA("MouseWheelUp\n");
		}
		if (me.GetType() == MouseEvent::EventType::WheelDown)
		{
			OutputDebugStringA("MouseWheelDown\n");
		}*/

		/*std::string outMsg = "X: ";
		outMsg += std::to_string(me.GetPosX());
		outMsg += ", Y: ";
		outMsg += std::to_string(me.GetPosY());
		outMsg += '\n';
		OutputDebugStringA(outMsg.c_str());*/
	}

	const float cameraSpeed = 0.005f * dt;

	if (keyboard.KeyIsPressed('W'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		this->gfx.camera.AdjustPosition(0.0f, cameraSpeed, 0.0f);
	}
	if (keyboard.KeyIsPressed(VK_SHIFT))
	{
		this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed, 0.0f);
	}
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
