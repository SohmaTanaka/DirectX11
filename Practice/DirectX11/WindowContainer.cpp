#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool rawInput_Initialized = false;

	if (!rawInput_Initialized)
	{
		RAWINPUTDEVICE rid;

#pragma region //�f�o�C�X�̒�`
		//�|�C���^�[											rid.usUsagePage = 0x01;	rid.usUsage = 0x01;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_MOUSE
		//�}�E�X													rid.usUsagePage = 0x01;	rid.usUsage = 0x02;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_MOUSE
		//�W���C�X�e�B�b�N									rid.usUsagePage = 0x01;	rid.usUsage = 0x04;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_GAME
		//�Q�[���p�b�h										rid.usUsagePage = 0x01;	rid.usUsage = 0x05;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_GAME
		//�L�[�{�[�h											rid.usUsagePage = 0x01;	rid.usUsage = 0x06;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_KEYBOARD
		//�L�[�p�b�h											rid.usUsagePage = 0x01;	rid.usUsage = 0x07;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_KEYBOARD
		//�V�X�e���R���g���[��							rid.usUsagePage = 0x01;	rid.usUsage = 0x80;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_CONTROL
		//�R���V���[�}�[�I�[�f�B�I�R���g���[��	rid.usUsagePage = 0x0C;	rid.usUsage = 0x01;	//�n�[�h�E�F�AID		HID_DEVICE_SYSTEM_CONSUMER
#pragma endregion
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;

		//�����̓f�t�H���g�ł�0
		rid.dwFlags = 0;
		rid.hwndTarget = 0;

		if (RegisterRawInputDevices(&rid, 0x01, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "���̓f�o�C�X�̓o�^�Ɏ��s \"�ꏊ\": WindowContainer �R���X�g���N�^�� WM_NCCREATE");
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
		//�����J��Ԃ��̉\��������
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeysAutoRepeat())
		{
			keyboard.OnKeyPressed(keycode);
		}
		else
		{
			//�O�̃t���[���ŃL�[��������Ă�����0��Ԃ�
			const bool wasPressed = lParam & 0x40000000;

			//�O�̃t���[���ŃL�[��������Ă��Ȃ�������
			if (!wasPressed)
			{
				keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		//�����J��Ԃ��͋N���Ȃ�
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
			//�O�̃t���[���ŃL�[��������Ă�����0��Ԃ�
			const bool wasPressed = lParam & 0x40000000;

			//�O�̃t���[���ŃL�[��������Ă��Ȃ�������
			if (!wasPressed)
			{
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//MouseMessages
	//�E�B���h�E��̐�Βl
	case WM_MOUSEMOVE:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		//lParam���ł̃A�h���X�̉ӏ���ύX���A�󂯎����̂�ς���
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
	//�}�E�X�̈ړ��̓��͒l�i���Βl�j
	case WM_INPUT:
	{
		UINT dataSize;
		ZeroMemory(&dataSize, sizeof(UINT));

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			//���j�[�N�|�C���^
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

		//�N���[���A�b�v���K�v
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
