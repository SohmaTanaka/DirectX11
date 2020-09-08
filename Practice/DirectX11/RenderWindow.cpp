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
		this->window_class_wide.c_str(),	//クラス名
		this->window_title_wide.c_str(),	//タイトル名
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,	//ウィンドウスタイル
		wr.left,	//Window X Position
		wr.top,	//Window Y Position
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,	//親ウィンドウハンドラ
		NULL,	//子ウィンドウもしくはメニューハンドラ
		this->hInstance,	//ハンドラのインスタンス
		pWindowContainer);	//ウィンドウ生成パラメータ

	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEXにおいてウィンドウの生成に失敗:" + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessage()
{
	//ウィンドウメッセージのハンドル
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, //メッセージの保存先
		this->handle,	//メッセージ元のウィンドウのハンドル
		0,	//フィルターメッセージの値の最小
		0,	//フィルターメッセージの値の最大
		PM_REMOVE))	//PeekMessageを介してキャプチャした後にメッセージを削除する
	{
		TranslateMessage(&msg);	//メッセージの変換（イベントの格納。キーの入力だとかそういうのに役に立つ）
		DispatchMessage(&msg);//送信処理
	}

	//ウィンドウが正しく終了した場合
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
		//メッセージを閉じ、破棄する
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		//デバイスからのメッセージを読み取り返す
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

// ウィンドウプロシージャのアドレスを返すコールバック関数
LRESULT CALLBACK HandlemessageSetUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// case以下で初めて出てくる変数は{}で囲わなければ変数が初期化されない
	switch (uMsg)
	{
		//case WM_CHAR:
		//{
		//	//キーコードを返す。実際に入力された文字の形で表記される
		//	unsigned char letter = static_cast<unsigned char>(wParam);
		//	return 0;
		//}
		//case WM_KEYDOWN:
		//{
		//	//キーコードを返す。常に大文字で表記される
		//	unsigned char keycode = static_cast<unsigned char>(wParam);
		//	return 0;
		//}	
		//WM_CREATEよりも前に呼ばれる
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		if (pWindow == nullptr)
		{
			ErrorLogger::Log("致命的なエラー: WindowContainerのポインタがnull \"場所\": RendererWindow HandlemessageSetUp内 WM_NCCREATE");
			exit(-1);
		}

		//ユーザーデータを必要に応じてポインタにセット
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);

		//return DefWindowProc(hwnd, uMsg, wParam, IParam);
	}
	default:
		//デフォルトのウィンドウプロシージャのアドレス
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;	//WindowClass
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//ウィンドウのスタイル
	wc.lpfnWndProc = HandlemessageSetUp;	// ウィンドウプロシージャのアドレス
	wc.cbClsExtra = 0;	//0に固定
	wc.cbWndExtra = 0;	//0に固定
	wc.hInstance = this->hInstance;	//ハンドラのインスタンス
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル設定
	wc.hbrBackground = NULL;	//背景色
	wc.lpszMenuName = NULL;	//メニュー
	wc.lpszClassName = this->window_class_wide.c_str();	//クラス名
	wc.cbSize = sizeof(WNDCLASSEX);	//構造体サイズ

	RegisterClassEx(&wc);	//拡張ウィンドウクラスの表示
}
