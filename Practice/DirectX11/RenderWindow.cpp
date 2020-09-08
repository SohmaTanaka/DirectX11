#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	RECT wr;	//Windows Rectangle
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + this->width;
	wr.bottom = wr.top + this->height;

	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->handle = CreateWindowEx(
		0,
		this->window_class_wide.c_str(),	//�N���X��
		this->window_title_wide.c_str(),	//�^�C�g����
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,	//�E�B���h�E�X�^�C��
		wr.left,	//Window X Position
		wr.top,	//Window Y Position
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,	//�e�E�B���h�E�n���h��
		NULL,	//�q�E�B���h�E�������̓��j���[�n���h��
		this->hInstance,	//�n���h���̃C���X�^���X
		pWindowContainer);	//�E�B���h�E�����p�����[�^

	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEX�ɂ����ăE�B���h�E�̐����Ɏ��s:" + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessage()
{
	//�E�B���h�E���b�Z�[�W�̃n���h��
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, //���b�Z�[�W�̕ۑ���
		this->handle,	//���b�Z�[�W���̃E�B���h�E�̃n���h��
		0,	//�t�B���^�[���b�Z�[�W�̒l�̍ŏ�
		0,	//�t�B���^�[���b�Z�[�W�̒l�̍ő�
		PM_REMOVE))	//PeekMessage����ăL���v�`��������Ƀ��b�Z�[�W���폜����
	{
		TranslateMessage(&msg);	//���b�Z�[�W�̕ϊ��i�C�x���g�̊i�[�B�L�[�̓��͂��Ƃ����������̂ɖ��ɗ��j
		DispatchMessage(&msg);//���M����
	}

	//�E�B���h�E���������I�������ꍇ
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//���b�Z�[�W����A�j������
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		//�f�o�C�X����̃��b�Z�[�W��ǂݎ��Ԃ�
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

// �E�B���h�E�v���V�[�W���̃A�h���X��Ԃ��R�[���o�b�N�֐�
LRESULT CALLBACK HandlemessageSetUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// case�ȉ��ŏ��߂ďo�Ă���ϐ���{}�ň͂�Ȃ���Εϐ�������������Ȃ�
	switch (uMsg)
	{
		//case WM_CHAR:
		//{
		//	//�L�[�R�[�h��Ԃ��B���ۂɓ��͂��ꂽ�����̌`�ŕ\�L�����
		//	unsigned char letter = static_cast<unsigned char>(wParam);
		//	return 0;
		//}
		//case WM_KEYDOWN:
		//{
		//	//�L�[�R�[�h��Ԃ��B��ɑ啶���ŕ\�L�����
		//	unsigned char keycode = static_cast<unsigned char>(wParam);
		//	return 0;
		//}	
		//WM_CREATE�����O�ɌĂ΂��
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		if (pWindow == nullptr)
		{
			ErrorLogger::Log("�v���I�ȃG���[: WindowContainer�̃|�C���^��null \"�ꏊ\": RendererWindow HandlemessageSetUp�� WM_NCCREATE");
			exit(-1);
		}

		//���[�U�[�f�[�^��K�v�ɉ����ă|�C���^�ɃZ�b�g
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);

		//return DefWindowProc(hwnd, uMsg, wParam, IParam);
	}
	default:
		//�f�t�H���g�̃E�B���h�E�v���V�[�W���̃A�h���X
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;	//WindowClass
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//�E�B���h�E�̃X�^�C��
	wc.lpfnWndProc = HandlemessageSetUp;	// �E�B���h�E�v���V�[�W���̃A�h���X
	wc.cbClsExtra = 0;	//0�ɌŒ�
	wc.cbWndExtra = 0;	//0�ɌŒ�
	wc.hInstance = this->hInstance;	//�n���h���̃C���X�^���X
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���ݒ�
	wc.hbrBackground = NULL;	//�w�i�F
	wc.lpszMenuName = NULL;	//���j���[
	wc.lpszClassName = this->window_class_wide.c_str();	//�N���X��
	wc.cbSize = sizeof(WNDCLASSEX);	//�\���̃T�C�Y

	RegisterClassEx(&wc);	//�g���E�B���h�E�N���X�̕\��
}
