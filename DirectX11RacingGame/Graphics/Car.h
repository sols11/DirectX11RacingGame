#pragma once
#include "HandDrawObject.h"
#include "Geometry.h"

class Car:public HandDrawObject
{
public:
    bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    void Draw(const XMMATRIX& viewProjectionMatrix);
protected:
    void UpdateWorldMatrix();
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bodyTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> wheelTexture;

    HandDrawObject leftFrontWheel;
    HandDrawObject rightFrontWheel;
    HandDrawObject leftRearWheel;
    HandDrawObject rightRearWheel;
};