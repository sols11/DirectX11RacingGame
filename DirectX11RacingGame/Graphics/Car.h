#pragma once
#include "HandDrawObject.h"
#include "Geometry.h"

class Car
{
public:
    bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    void Draw();
private:
    ID3D11Device * device = nullptr;
    ID3D11DeviceContext * deviceContext = nullptr;
    ConstantBuffer<CB_VS_VertexShader> * cb_vs_vertexshader = nullptr;

    HandDrawObject leftFrontWheel;
};