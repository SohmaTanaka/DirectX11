#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template <class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize = 0;

public:
	VertexBuffer() {}

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->bufferSize;
	}

	const UINT Stride() const
	{
		return *this->stride.get();
	}

	const UINT* StridePtr() const
	{
		return this->stride.get();
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices)
	{
		//メモリリーク防止
		if (buffer.Get() != nullptr)
			buffer.Reset();	//生成毎にメモリを解放

		this->bufferSize = numVertices;

		//メモリリーク防止
		//既に生成されていたら生成しない
		if (this->stride.get() == nullptr)
			this->stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(T) * numVertices;	//バッファサイズを指定
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//頂点バッファとしてバインド
		bufferDesc.CPUAccessFlags = 0;	//CPUへのアクセスフラグ
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;	//頂点へのポインタ

		HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
		return hr;
	}
};