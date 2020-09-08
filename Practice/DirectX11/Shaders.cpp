#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"シェーダ―の読み込みに失敗: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	hr = device->CreateVertexShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"頂点シェーダーの生成に失敗: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	//第一引数：descriptionのポインタ
	//第二引数：要素数
	//第三引数：頂点シェーダバッファサイズ
	hr = device->CreateInputLayout(layoutDesc, numElements, this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), this->inputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "入力レイアウトの生成に失敗");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3DBlob* VertexShader::GetBuffer()
{
	return this->shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetinputLayout()
{
	return this->inputLayout.Get();
}

/*以下、ピクセルシェーダ―*/

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"シェーダ―の読み込みに失敗: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	hr = device->CreatePixelShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"ピクセルシェーダーの生成に失敗: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3DBlob* PixelShader::GetBuffer()
{
	return this->shaderBuffer.Get();
}
