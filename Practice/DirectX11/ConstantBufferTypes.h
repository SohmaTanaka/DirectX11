#pragma once

#include <DirectXMath.h>

//16byte�ȓ��Ɏ��߂�K�v������
struct CB_VS_vertexShader
{
	DirectX::XMMATRIX mat;
};

struct CB_PS_pixelShader
{
	float alpha = 1.0f;
};