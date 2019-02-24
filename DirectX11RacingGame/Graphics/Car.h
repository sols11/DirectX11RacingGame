#pragma once
#include "Model.h"
#include "Geometry.h"

class Car:public Model
{
public:
    bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    void Draw(const XMMATRIX& viewProjectionMatrix);
protected:
    void UpdateWorldMatrix(XMMATRIX parentWorldMatrix = XMMatrixIdentity());
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bodyTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> wheelTexture;

    Model wheels[4];
    //Model leftFrontWheel;
    //Model rightFrontWheel;
    //Model leftRearWheel;
    //Model rightRearWheel;
};