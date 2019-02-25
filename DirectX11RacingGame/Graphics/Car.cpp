#include "Car.h"
#include <WICTextureLoader.h>

bool Car::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader)
{
    //HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\sunset6.bmp", nullptr, bodyTexture.GetAddressOf());
    //COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
    // 调用父类
    auto bodyMeshData = Geometry::CreateBox(1.0f, 1.0f, 2.0f);
    Model::Initialize(bodyMeshData.vertexVec, bodyMeshData.indexVec, device, deviceContext, bodyTexture.Get(), cb_vs_vertexshader);

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\wheelBlack.jpg", nullptr, wheelTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

    auto wheelMeshData = Geometry::CreateCylinder(0.5f, 0.1f);
    wheels[0].Initialize(wheelMeshData.vertexVec, wheelMeshData.indexVec, device, deviceContext, wheelTexture.Get(), cb_vs_vertexshader);
    wheels[0].SetPosition(-0.55f, -0.5f, 1.0f);
    wheels[0].SetRotation(0, 0, XM_PI / 2);  // 弧度旋转 1° = π/180

    return false;
}

void Car::Draw(const XMMATRIX& viewProjectionMatrix)
{
    Model::Draw(viewProjectionMatrix);
    wheels[0].Draw(viewProjectionMatrix);
}

void Car::UpdateWorldMatrix(Model* parent)
{
    Model::UpdateWorldMatrix(parent);
    wheels[0].UpdateWorldMatrix(this);
}
