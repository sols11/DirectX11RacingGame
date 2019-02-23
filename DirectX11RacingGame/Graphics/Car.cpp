#include "Car.h"
#include <WICTextureLoader.h>

bool Car::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader)
{
    //HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\sunset6.bmp", nullptr, bodyTexture.GetAddressOf());
    //COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
    // 调用父类
    auto bodyMeshData = Geometry::CreateBox(1.0f, 1.0f, 2.0f);
    HandDrawObject::Initialize(bodyMeshData.vertexVec, bodyMeshData.indexVec, device, deviceContext, bodyTexture.Get(), cb_vs_vertexshader);

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\pinksquare.jpg", nullptr, wheelTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

    auto wheelMeshData = Geometry::CreateCylinder(0.5f, 0.1f);
    leftFrontWheel.Initialize(wheelMeshData.vertexVec, wheelMeshData.indexVec, device, deviceContext, wheelTexture.Get(), cb_vs_vertexshader);
    leftFrontWheel.SetPosition(-0.55f, -0.5f, 1.0f);
    leftFrontWheel.SetRotation(0, 0, XM_PI / 2);  // 弧度旋转 1° = π/180

    return false;
}

void Car::Draw(const XMMATRIX& viewProjectionMatrix)
{
    HandDrawObject::Draw(viewProjectionMatrix);
    leftFrontWheel.Draw(viewProjectionMatrix);
}

void Car::UpdateWorldMatrix()
{
    this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vec_up = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
    this->vec_down = XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);

    leftFrontWheel.UpdateWorldMatrix(this->worldMatrix);
}
