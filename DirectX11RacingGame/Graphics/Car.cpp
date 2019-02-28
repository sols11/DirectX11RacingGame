#include "Car.h"
#include <WICTextureLoader.h>

bool Car::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\red.png", nullptr, bodyTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
    auto bodyMeshData = Prefab::CreateBox(1.0f, 1.0f, 3.0f);
    // 调用父类
    Model::Initialize(bodyMeshData.vertexVec, bodyMeshData.indexVec, device, deviceContext, bodyTexture.Get(), cb_vs_vertexshader);

    hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\wheelBlack.jpg", nullptr, wheelTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

    auto wheelMeshData = Prefab::CreateCylinder(0.3f, 0.1f);
    for (int i = 0; i < 4; ++i)
    {
        wheels[i].Initialize(wheelMeshData.vertexVec, wheelMeshData.indexVec, device, deviceContext, wheelTexture.Get(), cb_vs_vertexshader);
        wheels[i].SetRotation(0, 0, XM_PI / 2);  // 弧度旋转 1° = π/180
    }
    wheels[0].SetPosition(-0.55f, -0.5f, 0.75f);
    wheels[1].SetPosition( 0.55f, -0.5f, 0.75f);
    wheels[2].SetPosition(-0.55f, -0.5f, -0.75f);
    wheels[3].SetPosition( 0.55f, -0.5f, -0.75f);

    return true;
}

void Car::Draw(const XMMATRIX& viewProjectionMatrix)
{
    if (dontDraw)
        return;
    Model::Draw(viewProjectionMatrix);
    for (int i = 0; i < 4; ++i)
    {
        wheels[i].Draw(viewProjectionMatrix);
    }
}

void Car::WheelRoll(float deltaTime, bool forward)
{
    if (forward)
    {
        for (int i = 0; i < 4; ++i)
        {
            wheels[i].AdjustRotation(wheels[i].GetRightVector() * deltaTime * rollSpeed);
        }
    }
}

void Car::UpdateWorldMatrix(Object* parent)
{
    Object::UpdateWorldMatrix(parent);
    for (int i = 0; i < 4; ++i)
    {
        wheels[i].UpdateWorldMatrix(this);
    }
}
