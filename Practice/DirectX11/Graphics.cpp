#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;

	this->fpsTimer.Start();

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	//ImGUIのセットアップ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f,0.0f,0.0f,1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	//第三引数：深度	第四引数：ステンシルの初期化

	this->deviceContext->IASetInputLayout(this->vertxShader.GetinputLayout());
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);	//点の描画
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);		//線の描画
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);		//点が順番に繋がった線の描画
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	//三角形の描画

	this->deviceContext->RSSetState(this->rasterizerState.Get());

	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);

	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());	//PSSetSamplers(開始スロット,サンプラーの数,サンプラーステートのポインタ)

	this->deviceContext->VSSetShader(this->vertxShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	static float alpha = 1.0f;

	//GEMBA_NEKOを透けさせた時、JUPITER,GEMBA_NEKO,MOONの順番だとJUPITERが透ける
//#pragma region JUPITER
//
//	//ワールドマトリックス
//	XMMATRIX world = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(0.0f, 0.0f, 4.0f);
//	//定数バッファの更新
//	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
//	//移動
//	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
//	//回転（反時計回り）
//	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
//	//拡大縮小
//	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
//
//	//転置（OpenGL標準の方式からDirectX標準の方式へ）
//	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
//
//	if (!cb_vs_vertexShader.ApplyChanges())
//		return;
//
//	//頂点シェーダー定数バッファ
//	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());
//
//	//ピクセルシェーダー定数バッファ
//	this->cb_ps_pixelShader.data.alpha = 1.0f;
//	this->cb_ps_pixelShader.ApplyChanges();
//	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());
//
//	//Square
//	this->deviceContext->PSSetShaderResources(0, 1, this->jupiterTexture.GetAddressOf());
//	//第一引数：開始スロット
//	//第二引数：バッファ数
//	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
//	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//	//第一引数：頂点の数
//	//第二引数：開始点のオフセット
//	//第三引数：インデックスの開始点
//	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
//#pragma endregion
//
//#pragma region MOON
//
//	//static float translationOffset[3]{ };
//	//ワールドマトリックス
//	world = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);
//	//定数バッファの更新
//	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
//	//移動
//	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
//	//回転（反時計回り）
//	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
//	//拡大縮小
//	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
//
//	//転置（OpenGL標準の方式からDirectX標準の方式へ）
//	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
//
//	if (!cb_vs_vertexShader.ApplyChanges())
//		return;
//
//	//頂点シェーダー定数バッファ
//	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());
//
//	//ピクセルシェーダー定数バッファ
//	this->cb_ps_pixelShader.data.alpha = 1.0f;
//	this->cb_ps_pixelShader.ApplyChanges();
//	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());
//
//	//Square
//	this->deviceContext->PSSetShaderResources(0, 1, this->moonTexture.GetAddressOf());
//	//this->deviceContext->PSSetShaderResources(0, 1, this->gembaTexture.GetAddressOf());
//	//第一引数：開始スロット
//	//第二引数：バッファ数
//	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
//	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//	//第一引数：頂点の数
//	//第二引数：開始点のオフセット
//	//第三引数：インデックスの開始点
//	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
//#pragma endregion

#pragma region GEMBA_NEKO

	/*現時点でこちらを先にすると透過しない*/

	static float translationOffset[3]{ 0.0f, 0.0f, -1.0f };

	//ワールドマトリックス
	XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
	//定数バッファの更新
	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	//移動
	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
	//回転（反時計回り）
	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
	//拡大縮小
	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);

	//転置（OpenGL標準の方式からDirectX標準の方式へ）
	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);

	if (!cb_vs_vertexShader.ApplyChanges())
		return;

	//頂点シェーダー定数バッファ
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());

	//ピクセルシェーダー定数バッファ
	this->cb_ps_pixelShader.data.alpha = alpha;
	this->cb_ps_pixelShader.ApplyChanges();
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());

	//Square
	//this->deviceContext->PSSetShaderResources(0, 1, this->moonTexture.GetAddressOf());
	this->deviceContext->PSSetShaderResources(0, 1, this->gembaTexture.GetAddressOf());
	//第一引数：開始スロット
	//第二引数：バッファ数
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//第一引数：頂点の数
	//第二引数：開始点のオフセット
	//第三引数：インデックスの開始点
	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);

#pragma endregion


	static int fpsCounter = 0;
	static std::string fpsString = "FPS : 0";
	fpsCounter += 1;

	//一秒経ったら
	if (fpsTimer.GetMilliSecondsElapsed() > 1000.0)
	{
		fpsString = "FPS : " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}

	//テキスト描画開始
	spriteBatch->Begin();

	//DrawString(スプライトバッチのポインタ,表示する文字列,位置情報,カラー情報,回転,オリジナル,スケーリング)
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::Aqua, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

	spriteBatch->End();
	//テキスト描画終了

	static int counter;

	//ImGUI用のフレームの開始
	//順番を守る
	ImGui_ImplDX11_NewFrame();		//DirectX11用
	ImGui_ImplWin32_NewFrame();	//Win32用
	ImGui::NewFrame();
	//テストウィンドウの生成
	ImGui::Begin("Test");

	ImGui::Text("This is Test");
	if (ImGui::Button("CLICK ME !"))
		counter += 1;
	ImGui::SameLine();	//前後のImGui描画を同列に並べる
	std::string clickCount = "ClickCount : " + std::to_string(counter);
	ImGui::Text(clickCount.c_str());
	//Slider
	ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
	//Slider
	ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.0f, 1.0f);
	ImGui::End();
	//描画処理
	ImGui::Render();
	//描画データからの描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//第一引数：垂直同期（0でオフ）
	//第二引数：フラグ
	this->swapchain->Present(0, NULL);
}

//シェーダ―などが初期化される場所
bool Graphics::InitializeDirectX(HWND hwnd)
{
	//グラフィックスカードの情報取得
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("警告：DXGIはビデオアダプターを見つけることが出来ませんでした");

		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = this->windowWidth;
	scd.BufferDesc.Height = this->windowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;	//分母
	scd.BufferDesc.RefreshRate.Denominator = 1;	//分子
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//バッファのフォーマット
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//走査線の指定
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	//ウィンドウ描画時のスケーリング指定

	scd.SampleDesc.Count = 1;	//サンプリング数
	scd.SampleDesc.Quality = 0;	//サンプリングのクオリティ（０～1）

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バッファの使用方法の指定
	scd.BufferCount = 1;	//バッファの数
	scd.OutputWindow = hwnd;	//出力ウィンドウのハンドル
	scd.Windowed = true;	//ウィンドウモードの設定
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//スワップエフェクトの指定。DISCARDは破棄
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//スワップチェインの設定フラグの指定。ウィンドウモード及びフルスクリーンモードの移行を許可。

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter,	// IDXGIAdapterのポインタ。NULLで規定
		D3D_DRIVER_TYPE_UNKNOWN,	//ドライバータイプ。HARDWAREでDirect3Dの機能をハードウェアに実装
		NULL,	//ソフトウェアドライバータイプ。D3D_DRIVER_TYPEがHARDWAREなら必ずNULL
		NULL,	//有効にするランタイムレイヤー
		NULL,	//デバイスの機能レベルの順序を指定する配列
		0,	//デバイスの機能レベルの順序を指定する配列の数
		D3D11_SDK_VERSION,	//SDKのバージョン
		&scd,	//スワップチェインの初期化パラメータ
		this->swapchain.GetAddressOf(),	//作成されるスワップチェインのアドレス
		this->device.GetAddressOf(),	//作成されるデバイスのアドレス
		NULL,	//作成されたデバイスの機能レベル
		this->deviceContext.GetAddressOf()	//作成されるデバイスのコンテキストのアドレス
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "デバイスとスワップチェインの作成に失敗");
		return false;
	}

	//バックバッファのスマートポインタ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	//スワップチェインから取得
	//第一引数：バッファスロット
	//第二引数：テクスチャ2DのID
	//第三引数：バックバッファのアドレス
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "バッファの取得に失敗");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "レンダラーターゲットビューの生成に失敗");
		return false;
	}

	//深度ステンシルバッファとステンシルビューの生成
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;	//配列の大きさ
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//フォーマット。32ビットのZバッファ式。深度24ビット、8ビットが始まり。
	depthStencilDesc.SampleDesc.Count = 1;	//サンプリング数
	depthStencilDesc.SampleDesc.Quality = 0;	//サンプリングのクオリティ（０～1）
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;		//使用方法の指定
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//フラグの指定
	depthStencilDesc.CPUAccessFlags = 0;	//CPUへのアクセスフラグ
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "深度ステンシルバッファの生成に失敗");
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "深度ステンシルビューの生成に失敗");
		return false;
	}

	//深度ステンシルステートの生成
	CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;	//深度の適用
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;	//深度マスク
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;	//深度マスクの状態。この場合は置き換え。

	hr = this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "深度ステンシルステートの生成に失敗");
		return false;
	}

	//第一引数：ターゲット数
	//第二引数：ターゲットビューのアドレス
	//第三引数：深度ステンシルポインタ
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	//ビューポートの生成
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(CD3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)this->windowWidth;
	viewport.Height = (FLOAT)this->windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//ビューポートのセット
	//第一引数：ビューポートの数
	//第二引数：ビューポートのアドレス
	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerdisc;
	ZeroMemory(&rasterizerdisc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerdisc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;	//塗りつぶし	D3D11_FILL_WIREFRAMEでワイヤーフレーム表示が利用可能
	rasterizerdisc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;	//カリング	BACK:裏側を描画しない	FRONT:表側を描画しない	NONE:両面描画
	//rasterizerdisc.FrontCounterClockwise = TRUE;	//反時計回り順に描画

	hr = this->device->CreateRasterizerState(&rasterizerdisc, this->rasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "ラスタライザーステートの生成に失敗");
		return false;
	}

	//ブレンドステートの生成
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	//ブレンドの可否
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	/*ブレンドの公式*/
	//SA = Source Alpha
	//SBF = Source Blend Factor
	//DA = Destination Alpha
	//DBF = Destination Blend Factor
	//BO = Blend Operation
	//FA = Final Alpha
	//
	//FA = SA・SBF ± DA ・ DBF（今回の場合は0）

	/*ピクセルにブレンド*/
	//SP = Source Pixel
	//SBF = Source Blend Factor
	//DP = Destination Pixel
	//DBF = Destination Blend Factor
	//BO = Blend Operation
	//FC = Final Color
	//
	//FC = SP・SBF ± DP・DBF（今回の場合は0）
	//例
	//SP = ( 0, 1.0, 0, 0.9)←0.9はSBFに使用
	//DP = ( 1.0, 0, 0, 1.0)
	//1.0 - 0.9 = 0.1
	//FC = ( 0, 1.0, 0)・0.9 + ( 1.0, 0, 0)・0.1
	//	   = ( 0, 0.9, 0) + ( 0.1, 0, 0)
	//	   = ( 0.1, 0.9, 0)

	blendDesc.RenderTarget[0] = rtbd;

	hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "ブレンドステートの生成に失敗");
		return false;
	}

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data/Fonts/comic_sans_16.spritefont");

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//フィルタリングオプション
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//テクスチャのアドレスのラップ（自動繰り返し）。UV座標。テクスチャのx座標
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//テクスチャのアドレスのラップ（自動繰り返し）。UV座標。テクスチャのy座標
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	//3Dテクスチャ座標
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	//比較
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "サンプラーステートの生成に失敗");
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{
	std::wstring shaderFolder = L"";

#pragma region シェーダーパス
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFolder = L"..//x64/Debug/";
#else	_WIN32
		shaderFolder = L"..//Debug/";
#endif
#else	_RELEASE
#ifdef _WIN64
		shaderFolder = L"..//x64/Release/";
#else	_WIN32
		shaderFolder = L"..//Release/";
#endif
#endif
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},	//第二引数はセマンティクス。第三引数はデータフォーマット。第四引数は入力スロット。第五引数は整列バイトオフセット。第六引数は頂点データもしくはインスタンス
		//{"COLOR",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertxShader.Initialize(this->device, shaderFolder + L"VertexShader.cso", layout, numElements))
		return false;

	if (!pixelShader.Initialize(this->device, shaderFolder + L"PixelShader.cso", layout, numElements))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
#pragma region Obj1

	//一つなら点の描画
	Vertex v[] =
	{
		//PrimitiveTopologyがTriangleの時に長方形を描く場合
		Vertex(-0.5f,-0.5f,-0.5f,0.0f,1.0f),	//FRONT LeftBottom		[0]
		Vertex(-0.5f,0.5f, -0.5f,0.0f,0.0f),	//FRONT LeftTop			[1]
		Vertex(0.5f,0.5f,-0.5f,1.0f,0.0f),		//FRONT RightTop			[2]
		Vertex(0.5f,-0.5f,-0.5f,1.0f,1.0f),		//FRONT RightBottom		[3]

		Vertex(-0.5f,-0.5f,0.5f,0.0f,1.0f),	//BACK LeftBottom			[4]
		Vertex(-0.5f,0.5f, 0.5f,0.0f,0.0f),	//BACK LeftTop				[5]
		Vertex(0.5f,0.5f,0.5f,1.0f,0.0f),		//BACK RightTop			[6]
		Vertex(0.5f,-0.5f,0.5f,1.0f,1.0f),	//BACK RightBottom		[7]
	};

	HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "頂点バッファの生成に失敗");
		return false;
	}

	//インデックス
	DWORD indices[] =
	{
		0,1,2,	//FRONT
		0,2,3,	//FRONT
		
		4,7,6,	//BACK
		4,6,5,	//BACK

		3,2,6,	//RIGHT
		3,6,7,	//RiGHT

		4,5,1,	//LEFT
		4,1,0,	//LEFT

		1,5,6,	//TOP
		1,6,2,	//TOP

		0,3,7,	//BOTTOM
		0,7,4,	//BOTTOM
	};

	hr = this->indicesBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "インデックスバッファの生成に失敗");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/moon.jpg", nullptr, moonTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "テクスチャの生成に失敗");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/gemba.jpg", nullptr, gembaTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "テクスチャの生成に失敗");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/jupiter.jpg", nullptr, jupiterTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "テクスチャの生成に失敗");
		return false;
	}

	hr = this->cb_vs_vertexShader.Initialize(this->device.Get(), this->deviceContext.Get());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "定数バッファの初期化に失敗");
		return false;
	}

	hr = this->cb_ps_pixelShader.Initialize(this->device.Get(), this->deviceContext.Get());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "定数バッファの初期化に失敗");
		return false;
	}

#pragma endregion

	camera.SetPosition(0.0f, 0.0f, -2.0f);

	//アスペクト比（定義が整数型であるのでキャストする）
	float aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);

	camera.SetProjectionValues(90.0f, aspectRatio, 0.1f, 1000.0f);

	//#pragma region Triangle2（Green）
	//
	//	//一つなら点の描画
	//	Vertex v2[] =
	//	{
	//		Vertex(-0.3f,-0.3f,0.0f,1.0f,0.0f,0.0f),	//Left	Red
	//		Vertex(0.0f,0.3f,0.0f,0.0f,1.0f,0.0f),	//Top	Green
	//		Vertex(0.3f,-0.3f,0.0f,0.0f,0.0f,1.0f),	//Right	Blue
	//		//Vertex(0.0f,0.0f),	//Center
	//		//Vertex(0.0f,-0.1f),	//Bottom
	//	};
	//
	//	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	//
	//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v2);	//バッファサイズを指定
	//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//頂点バッファとしてバインド
	//	vertexBufferDesc.CPUAccessFlags = 0;	//CPUへのアクセスフラグ
	//	vertexBufferDesc.MiscFlags = 0;
	//
	//	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	vertexBufferData.pSysMem = v2;	//頂点へのポインタ
	//
	//	hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer2.GetAddressOf());
	//	if (FAILED(hr))
	//	{
	//		ErrorLogger::Log(hr, "頂点バッファの生成に失敗");
	//		return false;
	//	}
	//
	//#pragma endregion

	return true;
}
