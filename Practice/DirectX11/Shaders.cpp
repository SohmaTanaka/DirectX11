#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"�V�F�[�_�\�̓ǂݍ��݂Ɏ��s: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	hr = device->CreateVertexShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"���_�V�F�[�_�[�̐����Ɏ��s: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	//�������Fdescription�̃|�C���^
	//�������F�v�f��
	//��O�����F���_�V�F�[�_�o�b�t�@�T�C�Y
	hr = device->CreateInputLayout(layoutDesc, numElements, this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), this->inputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "���̓��C�A�E�g�̐����Ɏ��s");
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

/*�ȉ��A�s�N�Z���V�F�[�_�\*/

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"�V�F�[�_�\�̓ǂݍ��݂Ɏ��s: ";
		errorMessage += shaderpath;
		ErrorLogger::Log(hr, errorMessage);

		return false;
	}

	hr = device->CreatePixelShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMessage = L"�s�N�Z���V�F�[�_�[�̐����Ɏ��s: ";
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
