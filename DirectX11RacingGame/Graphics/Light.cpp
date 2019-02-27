#include "Light.h"

bool Light::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateWorldMatrix();

    return false;
}
