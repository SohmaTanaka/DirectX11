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

	Microsoft::WRL::ComPtr<ID3D11Device> device;	//�ʐM�X�}�[�g�|�C���^�B�o�b�t�@�̍쐬�ɕK�v
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;	//�ʐM�X�}�[�g�|�C���^�B�����_�����O�ňقȂ郊�\�[�X�𗘗p����ۂɕK�v
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;	//�ʐM�X�}�[�g�|�C���^�B��̃f�o�C�X���畡���̃r���[��\�����邱�Ƃ��\�B�j�����ꂽ��ڂ̃t���[������o�b�t�@���擾���A�`��Ɏg�p����i�t���b�v�j�i�H�j
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;	//�ʐM�X�}�[�g�|�C���^�B�����_�[�^�[�Q�b�g�i�E�B���h�E�j�ւ̃|�C���^

	VertexShader vertxShader;	//���_�V�F�[�_�[
	PixelShader pixelShader;	//�s�N�Z���V�F�[�_�[

	ConstantBuffer<CB_VS_vertexShader> cb_vs_vertexShader;	//���_�V�F�[�_�[�萔�o�b�t�@
	ConstantBuffer<CB_PS_pixelShader> cb_ps_pixelShader;	//�s�N�Z���V�F�[�_�[�萔�o�b�t�@

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

	/*	�Â��L�q���@
	ID3D11Device* device;	//�o�b�t�@�̍쐬�ɕK�v
	ID3D11DeviceContext* deviceContext;	//�����_�����O�ňقȂ郊�\�[�X�𗘗p����ۂɕK�v
	IDXGISwapChain* swapchain;	//��̃f�o�C�X���畡���̃r���[��\�����邱�Ƃ��\�B�j�����ꂽ��ڂ̃t���[������o�b�t�@���擾���A�`��Ɏg�p����i�t���b�v�j�i�H�j
	ID3D11RenderTargetView* renderTargetView;	//�����_�[�^�[�Q�b�g�i�E�B���h�E�j�ւ̃|�C���^
	*/
};