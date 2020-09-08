#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool rawInput_Initialized = false;

	if (!rawInput_Initialized)
	{
		RAWINPUTDEVICE rid;

#pragma region //デバイスの定義
		//ポインター											rid.usUsagePage = 0x01;	rid.usUsage = 0x01;	//ハードウェアID		HID_DEVICE_SYSTEM_MOUSE
		//マウス													rid.usUsagePage = 0x01;	rid.usUsage = 0x02;	//ハードウェアID		HID_DEVICE_SYSTEM_MOUSE
		//ジョイスティック									rid.usUsagePage = 0x01;	rid.usUsage = 0x04;	//ハードウェアID		HID_DEVICE_SYSTEM_GAME
		//ゲームパッド										rid.usUsagePage = 0x01;	rid.usUsage = 0x05;	//ハードウェアID		HID_DEVICE_SYSTEM_GAME
		//キーボード											rid.usUsagePage = 0x01;	rid.usUsage = 0x06;	//ハードウェアID		HID_DEVICE_SYSTEM_KEYBOARD
		//キーパッド											rid.usUsagePage = 0x01;	rid.usUsage = 0x07;	//ハードウェアID		HID_DEVICE_SYSTEM_KEYBOARD
		//システムコントロール							rid.usUsagePage = 0x01;	rid.usUsage = 0x80;	//ハードウェアID		HID_DEVICE_SYSTEM_CONTROL
		//コンシューマーオーディオコントロール	rid.usUsagePage = 0x0C;	rid.usUsage = 0x01;	//ハードウェアID		HID_DEVICE_SYSTEM_CONSUMER
#pragma endregion
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;

		//これらはデフォルトでは0
		rid.dwFlags = 0;
		rid.hwndTarget = 0;

		if (RegisterRawInputDevices(&rid, 0x01, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "入力デバイスの登録に失敗 \"場所\": WindowContainer コンストラクタ内 WM_NCCREATE");
			exit(-1);
		}

		rawInput_Initialized = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
		//KeyboardMessages
	case WM_KEYDOWN:
	{
		//自動繰り返しの可能性がある
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeysAutoRepeat())
		{
			keyboard.OnKeyPressed(keycode);
		}
		else
		{
			//前のフレームでキーが押されていたら0を返す
			const bool wasPressed = lParam & 0x40000000;

			//前のフレームでキーが押されていなかったら
			if (!wasPressed)
			{
				keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		//自動繰り返しは起きない
		unsigned char keycode = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharsAutoRepeat())
		{
			keyboard.OnChar(ch);
		}
		else
		{
			//前のフレームでキーが押されていたら0を返す
			const bool wasPressed = lParam & 0x40000000;

			//前のフレームでキーが押されていなかったら
			if (!wasPressed)
			{
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//MouseMessages
	//ウィンドウ上の絶対値
	case WM_MOUSEMOVE:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		//lParam内でのアドレスの箇所を変更し、受け取るものを変える
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	//マウスの移動の入力値（相対値）
	case WM_INPUT:
	{
		UINT dataSize;
		ZeroMemory(&dataSize, sizeof(UINT));

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			//ユニークポインタ
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		//クリーンアップが必要
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
