#pragma once
#include <DirectXMath.h>

//struct CB_VS_VertexShader
//{
//	DirectX::XMMATRIX wvpMatrix;
//	DirectX::XMMATRIX worldMatrix;
//};

struct CB_VS_VertexShader
{
    DirectX::XMMATRIX matrix;
};

struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength; 

	DirectX::XMFLOAT3 dynamicLightColor; 
	float dynamicLightStrength; 
	DirectX::XMFLOAT3 dynamicLightPosition; 
	float dynamicLightAttenuation_a;
	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
};

struct CB_PS_PixelShader
{
    float alpha = 1.0f;
};