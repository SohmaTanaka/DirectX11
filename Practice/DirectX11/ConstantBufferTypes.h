#pragma once

#include <DirectXMath.h>

//16byteˆÈ“à‚ÉŽû‚ß‚é•K—v‚ª‚ ‚é
struct CB_VS_vertexShader
{
	DirectX::XMMATRIX mat;
};

struct CB_PS_pixelShader
{
	float alpha = 1.0f;
};