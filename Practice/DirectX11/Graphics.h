#pragma once

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "Timer.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();

	Camera camera;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;	//通信スマートポインタ。バッファの作成に必要
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;	//通信スマートポインタ。レンダリングで異なるリソースを利用する際に必要
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;	//通信スマートポインタ。一つのデバイスから複数のビューを表示することが可能。破棄された二つ目のフレームからバッファを取得し、描画に使用する（フリップ）（？）
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;	//通信スマートポインタ。レンダーターゲット（ウィンドウ）へのポインタ

	VertexShader vertxShader;	//頂点シェーダー
	PixelShader pixelShader;	//ピクセルシェーダー

	ConstantBuffer<CB_VS_vertexShader> cb_vs_vertexShader;	//頂点シェーダー定数バッファ
	ConstantBuffer<CB_PS_pixelShader> cb_ps_pixelShader;	//ピクセルシェーダー定数バッファ

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indicesBuffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> gembaTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> moonTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> jupiterTexture;

	int windowWidth = 0;
	int windowHeight = 0;

	Timer fpsTimer;

	/*	古い記述方法
	ID3D11Device* device;	//バッファの作成に必要
	ID3D11DeviceContext* deviceContext;	//レンダリングで異なるリソースを利用する際に必要
	IDXGISwapChain* swapchain;	//一つのデバイスから複数のビューを表示することが可能。破棄された二つ目のフレームからバッファを取得し、描画に使用する（フリップ）（？）
	ID3D11RenderTargetView* renderTargetView;	//レンダーターゲット（ウィンドウ）へのポインタ
	*/
};