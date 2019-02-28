#pragma once
//#include <d3d11_1.h>
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v) : position(x, y, z), texCoord(u, v) {}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;
};