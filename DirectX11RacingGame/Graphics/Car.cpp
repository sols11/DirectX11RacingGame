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

void Car::UpdateWorldMatrix(XMMATRIX parentWorldMatrix)
{
    this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z) *parentWorldMatrix;
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
    this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vecUp = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);

    wheels[0].UpdateWorldMatrix(this->worldMatrix);
}
