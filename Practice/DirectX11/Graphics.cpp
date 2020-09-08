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

	//ImGUI�̃Z�b�g�A�b�v
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
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	//��O�����F�[�x	��l�����F�X�e���V���̏�����

	this->deviceContext->IASetInputLayout(this->vertxShader.GetinputLayout());
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);	//�_�̕`��
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);		//���̕`��
	//this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);		//�_�����ԂɌq���������̕`��
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	//�O�p�`�̕`��

	this->deviceContext->RSSetState(this->rasterizerState.Get());

	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);

	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());	//PSSetSamplers(�J�n�X���b�g,�T���v���[�̐�,�T���v���[�X�e�[�g�̃|�C���^)

	this->deviceContext->VSSetShader(this->vertxShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	static float alpha = 1.0f;

	//GEMBA_NEKO�𓧂����������AJUPITER,GEMBA_NEKO,MOON�̏��Ԃ���JUPITER��������
//#pragma region JUPITER
//
//	//���[���h�}�g���b�N�X
//	XMMATRIX world = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(0.0f, 0.0f, 4.0f);
//	//�萔�o�b�t�@�̍X�V
//	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
//	//�ړ�
//	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
//	//��]�i�����v���j
//	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
//	//�g��k��
//	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
//
//	//�]�u�iOpenGL�W���̕�������DirectX�W���̕����ցj
//	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
//
//	if (!cb_vs_vertexShader.ApplyChanges())
//		return;
//
//	//���_�V�F�[�_�[�萔�o�b�t�@
//	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());
//
//	//�s�N�Z���V�F�[�_�[�萔�o�b�t�@
//	this->cb_ps_pixelShader.data.alpha = 1.0f;
//	this->cb_ps_pixelShader.ApplyChanges();
//	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());
//
//	//Square
//	this->deviceContext->PSSetShaderResources(0, 1, this->jupiterTexture.GetAddressOf());
//	//�������F�J�n�X���b�g
//	//�������F�o�b�t�@��
//	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
//	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//	//�������F���_�̐�
//	//�������F�J�n�_�̃I�t�Z�b�g
//	//��O�����F�C���f�b�N�X�̊J�n�_
//	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
//#pragma endregion
//
//#pragma region MOON
//
//	//static float translationOffset[3]{ };
//	//���[���h�}�g���b�N�X
//	world = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);
//	//�萔�o�b�t�@�̍X�V
//	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
//	//�ړ�
//	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
//	//��]�i�����v���j
//	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
//	//�g��k��
//	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
//
//	//�]�u�iOpenGL�W���̕�������DirectX�W���̕����ցj
//	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
//
//	if (!cb_vs_vertexShader.ApplyChanges())
//		return;
//
//	//���_�V�F�[�_�[�萔�o�b�t�@
//	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());
//
//	//�s�N�Z���V�F�[�_�[�萔�o�b�t�@
//	this->cb_ps_pixelShader.data.alpha = 1.0f;
//	this->cb_ps_pixelShader.ApplyChanges();
//	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());
//
//	//Square
//	this->deviceContext->PSSetShaderResources(0, 1, this->moonTexture.GetAddressOf());
//	//this->deviceContext->PSSetShaderResources(0, 1, this->gembaTexture.GetAddressOf());
//	//�������F�J�n�X���b�g
//	//�������F�o�b�t�@��
//	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
//	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//	//�������F���_�̐�
//	//�������F�J�n�_�̃I�t�Z�b�g
//	//��O�����F�C���f�b�N�X�̊J�n�_
//	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
//#pragma endregion

#pragma region GEMBA_NEKO

	/*�����_�ł�������ɂ���Ɠ��߂��Ȃ�*/

	static float translationOffset[3]{ 0.0f, 0.0f, -1.0f };

	//���[���h�}�g���b�N�X
	XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
	//�萔�o�b�t�@�̍X�V
	cb_vs_vertexShader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	//�ړ�
	//constantBuffer.data.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
	//��]�i�����v���j
	//constantBuffer.data.mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XM_PIDIV2);
	//�g��k��
	//constantBuffer.data.mat = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);

	//�]�u�iOpenGL�W���̕�������DirectX�W���̕����ցj
	cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);

	if (!cb_vs_vertexShader.ApplyChanges())
		return;

	//���_�V�F�[�_�[�萔�o�b�t�@
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader.GetAddressOf());

	//�s�N�Z���V�F�[�_�[�萔�o�b�t�@
	this->cb_ps_pixelShader.data.alpha = alpha;
	this->cb_ps_pixelShader.ApplyChanges();
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelShader.GetAddressOf());

	//Square
	//this->deviceContext->PSSetShaderResources(0, 1, this->moonTexture.GetAddressOf());
	this->deviceContext->PSSetShaderResources(0, 1, this->gembaTexture.GetAddressOf());
	//�������F�J�n�X���b�g
	//�������F�o�b�t�@��
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//�������F���_�̐�
	//�������F�J�n�_�̃I�t�Z�b�g
	//��O�����F�C���f�b�N�X�̊J�n�_
	this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);

#pragma endregion


	static int fpsCounter = 0;
	static std::string fpsString = "FPS : 0";
	fpsCounter += 1;

	//��b�o������
	if (fpsTimer.GetMilliSecondsElapsed() > 1000.0)
	{
		fpsString = "FPS : " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}

	//�e�L�X�g�`��J�n
	spriteBatch->Begin();

	//DrawString(�X�v���C�g�o�b�`�̃|�C���^,�\�����镶����,�ʒu���,�J���[���,��],�I���W�i��,�X�P�[�����O)
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::Aqua, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

	spriteBatch->End();
	//�e�L�X�g�`��I��

	static int counter;

	//ImGUI�p�̃t���[���̊J�n
	//���Ԃ����
	ImGui_ImplDX11_NewFrame();		//DirectX11�p
	ImGui_ImplWin32_NewFrame();	//Win32�p
	ImGui::NewFrame();
	//�e�X�g�E�B���h�E�̐���
	ImGui::Begin("Test");

	ImGui::Text("This is Test");
	if (ImGui::Button("CLICK ME !"))
		counter += 1;
	ImGui::SameLine();	//�O���ImGui�`��𓯗�ɕ��ׂ�
	std::string clickCount = "ClickCount : " + std::to_string(counter);
	ImGui::Text(clickCount.c_str());
	//Slider
	ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
	//Slider
	ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.0f, 1.0f);
	ImGui::End();
	//�`�揈��
	ImGui::Render();
	//�`��f�[�^����̕`��
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//�������F���������i0�ŃI�t�j
	//�������F�t���O
	this->swapchain->Present(0, NULL);
}

//�V�F�[�_�\�Ȃǂ������������ꏊ
bool Graphics::InitializeDirectX(HWND hwnd)
{
	//�O���t�B�b�N�X�J�[�h�̏��擾
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("�x���FDXGI�̓r�f�I�A�_�v�^�[�������邱�Ƃ��o���܂���ł���");

		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = this->windowWidth;
	scd.BufferDesc.Height = this->windowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;	//����
	scd.BufferDesc.RefreshRate.Denominator = 1;	//���q
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�o�b�t�@�̃t�H�[�}�b�g
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//�������̎w��
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	//�E�B���h�E�`�掞�̃X�P�[�����O�w��

	scd.SampleDesc.Count = 1;	//�T���v�����O��
	scd.SampleDesc.Quality = 0;	//�T���v�����O�̃N�I���e�B�i�O�`1�j

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�t�@�̎g�p���@�̎w��
	scd.BufferCount = 1;	//�o�b�t�@�̐�
	scd.OutputWindow = hwnd;	//�o�̓E�B���h�E�̃n���h��
	scd.Windowed = true;	//�E�B���h�E���[�h�̐ݒ�
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//�X���b�v�G�t�F�N�g�̎w��BDISCARD�͔j��
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//�X���b�v�`�F�C���̐ݒ�t���O�̎w��B�E�B���h�E���[�h�y�уt���X�N���[�����[�h�̈ڍs�����B

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter,	// IDXGIAdapter�̃|�C���^�BNULL�ŋK��
		D3D_DRIVER_TYPE_UNKNOWN,	//�h���C�o�[�^�C�v�BHARDWARE��Direct3D�̋@�\���n�[�h�E�F�A�Ɏ���
		NULL,	//�\�t�g�E�F�A�h���C�o�[�^�C�v�BD3D_DRIVER_TYPE��HARDWARE�Ȃ�K��NULL
		NULL,	//�L���ɂ��郉���^�C�����C���[
		NULL,	//�f�o�C�X�̋@�\���x���̏������w�肷��z��
		0,	//�f�o�C�X�̋@�\���x���̏������w�肷��z��̐�
		D3D11_SDK_VERSION,	//SDK�̃o�[�W����
		&scd,	//�X���b�v�`�F�C���̏������p�����[�^
		this->swapchain.GetAddressOf(),	//�쐬�����X���b�v�`�F�C���̃A�h���X
		this->device.GetAddressOf(),	//�쐬�����f�o�C�X�̃A�h���X
		NULL,	//�쐬���ꂽ�f�o�C�X�̋@�\���x��
		this->deviceContext.GetAddressOf()	//�쐬�����f�o�C�X�̃R���e�L�X�g�̃A�h���X
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�f�o�C�X�ƃX���b�v�`�F�C���̍쐬�Ɏ��s");
		return false;
	}

	//�o�b�N�o�b�t�@�̃X�}�[�g�|�C���^
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	//�X���b�v�`�F�C������擾
	//�������F�o�b�t�@�X���b�g
	//�������F�e�N�X�`��2D��ID
	//��O�����F�o�b�N�o�b�t�@�̃A�h���X
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�o�b�t�@�̎擾�Ɏ��s");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�����_���[�^�[�Q�b�g�r���[�̐����Ɏ��s");
		return false;
	}

	//�[�x�X�e���V���o�b�t�@�ƃX�e���V���r���[�̐���
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;	//�z��̑傫��
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//�t�H�[�}�b�g�B32�r�b�g��Z�o�b�t�@���B�[�x24�r�b�g�A8�r�b�g���n�܂�B
	depthStencilDesc.SampleDesc.Count = 1;	//�T���v�����O��
	depthStencilDesc.SampleDesc.Quality = 0;	//�T���v�����O�̃N�I���e�B�i�O�`1�j
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;		//�g�p���@�̎w��
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//�t���O�̎w��
	depthStencilDesc.CPUAccessFlags = 0;	//CPU�ւ̃A�N�Z�X�t���O
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�[�x�X�e���V���o�b�t�@�̐����Ɏ��s");
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�[�x�X�e���V���r���[�̐����Ɏ��s");
		return false;
	}

	//�[�x�X�e���V���X�e�[�g�̐���
	CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;	//�[�x�̓K�p
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;	//�[�x�}�X�N
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;	//�[�x�}�X�N�̏�ԁB���̏ꍇ�͒u�������B

	hr = this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�[�x�X�e���V���X�e�[�g�̐����Ɏ��s");
		return false;
	}

	//�������F�^�[�Q�b�g��
	//�������F�^�[�Q�b�g�r���[�̃A�h���X
	//��O�����F�[�x�X�e���V���|�C���^
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	//�r���[�|�[�g�̐���
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(CD3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)this->windowWidth;
	viewport.Height = (FLOAT)this->windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//�r���[�|�[�g�̃Z�b�g
	//�������F�r���[�|�[�g�̐�
	//�������F�r���[�|�[�g�̃A�h���X
	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerdisc;
	ZeroMemory(&rasterizerdisc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerdisc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;	//�h��Ԃ�	D3D11_FILL_WIREFRAME�Ń��C���[�t���[���\�������p�\
	rasterizerdisc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;	//�J�����O	BACK:������`�悵�Ȃ�	FRONT:�\����`�悵�Ȃ�	NONE:���ʕ`��
	//rasterizerdisc.FrontCounterClockwise = TRUE;	//�����v��菇�ɕ`��

	hr = this->device->CreateRasterizerState(&rasterizerdisc, this->rasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "���X�^���C�U�[�X�e�[�g�̐����Ɏ��s");
		return false;
	}

	//�u�����h�X�e�[�g�̐���
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	//�u�����h�̉�
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	/*�u�����h�̌���*/
	//SA = Source Alpha
	//SBF = Source Blend Factor
	//DA = Destination Alpha
	//DBF = Destination Blend Factor
	//BO = Blend Operation
	//FA = Final Alpha
	//
	//FA = SA�ESBF �} DA �E DBF�i����̏ꍇ��0�j

	/*�s�N�Z���Ƀu�����h*/
	//SP = Source Pixel
	//SBF = Source Blend Factor
	//DP = Destination Pixel
	//DBF = Destination Blend Factor
	//BO = Blend Operation
	//FC = Final Color
	//
	//FC = SP�ESBF �} DP�EDBF�i����̏ꍇ��0�j
	//��
	//SP = ( 0, 1.0, 0, 0.9)��0.9��SBF�Ɏg�p
	//DP = ( 1.0, 0, 0, 1.0)
	//1.0 - 0.9 = 0.1
	//FC = ( 0, 1.0, 0)�E0.9 + ( 1.0, 0, 0)�E0.1
	//	   = ( 0, 0.9, 0) + ( 0.1, 0, 0)
	//	   = ( 0.1, 0.9, 0)

	blendDesc.RenderTarget[0] = rtbd;

	hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�u�����h�X�e�[�g�̐����Ɏ��s");
		return false;
	}

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data/Fonts/comic_sans_16.spritefont");

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//�t�B���^�����O�I�v�V����
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//�e�N�X�`���̃A�h���X�̃��b�v�i�����J��Ԃ��j�BUV���W�B�e�N�X�`����x���W
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//�e�N�X�`���̃A�h���X�̃��b�v�i�����J��Ԃ��j�BUV���W�B�e�N�X�`����y���W
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	//3D�e�N�X�`�����W
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	//��r
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�T���v���[�X�e�[�g�̐����Ɏ��s");
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{
	std::wstring shaderFolder = L"";

#pragma region �V�F�[�_�[�p�X
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
		{"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0},	//�������̓Z�}���e�B�N�X�B��O�����̓f�[�^�t�H�[�}�b�g�B��l�����͓��̓X���b�g�B��܈����͐���o�C�g�I�t�Z�b�g�B��Z�����͒��_�f�[�^�������̓C���X�^���X
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

	//��Ȃ�_�̕`��
	Vertex v[] =
	{
		//PrimitiveTopology��Triangle�̎��ɒ����`��`���ꍇ
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
		ErrorLogger::Log(hr, "���_�o�b�t�@�̐����Ɏ��s");
		return false;
	}

	//�C���f�b�N�X
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
		ErrorLogger::Log(hr, "�C���f�b�N�X�o�b�t�@�̐����Ɏ��s");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/moon.jpg", nullptr, moonTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�e�N�X�`���̐����Ɏ��s");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/gemba.jpg", nullptr, gembaTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�e�N�X�`���̐����Ɏ��s");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/jupiter.jpg", nullptr, jupiterTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�e�N�X�`���̐����Ɏ��s");
		return false;
	}

	hr = this->cb_vs_vertexShader.Initialize(this->device.Get(), this->deviceContext.Get());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�萔�o�b�t�@�̏������Ɏ��s");
		return false;
	}

	hr = this->cb_ps_pixelShader.Initialize(this->device.Get(), this->deviceContext.Get());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "�萔�o�b�t�@�̏������Ɏ��s");
		return false;
	}

#pragma endregion

	camera.SetPosition(0.0f, 0.0f, -2.0f);

	//�A�X�y�N�g��i��`�������^�ł���̂ŃL���X�g����j
	float aspectRatio = static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight);

	camera.SetProjectionValues(90.0f, aspectRatio, 0.1f, 1000.0f);

	//#pragma region Triangle2�iGreen�j
	//
	//	//��Ȃ�_�̕`��
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
	//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v2);	//�o�b�t�@�T�C�Y���w��
	//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���_�o�b�t�@�Ƃ��ăo�C���h
	//	vertexBufferDesc.CPUAccessFlags = 0;	//CPU�ւ̃A�N�Z�X�t���O
	//	vertexBufferDesc.MiscFlags = 0;
	//
	//	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	vertexBufferData.pSysMem = v2;	//���_�ւ̃|�C���^
	//
	//	hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer2.GetAddressOf());
	//	if (FAILED(hr))
	//	{
	//		ErrorLogger::Log(hr, "���_�o�b�t�@�̐����Ɏ��s");
	//		return false;
	//	}
	//
	//#pragma endregion

	return true;
}
