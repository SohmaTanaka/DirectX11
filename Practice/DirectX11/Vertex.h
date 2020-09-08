#pragma once

#include <DirectXMath.h>

//’¸“_
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		:pos(x, y, z), texcoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	//DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 texcoord;
};