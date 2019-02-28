#pragma once
#include <DirectXMath.h>

struct CB_VS_VertexShader
{
    DirectX::XMMATRIX matrix;
};

struct CB_PS_Light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength; 
};

struct CB_PS_PixelShader
{
    float alpha = 1.0f;
};