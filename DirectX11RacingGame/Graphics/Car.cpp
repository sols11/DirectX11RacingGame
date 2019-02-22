#include "Car.h"
#include <WICTextureLoader.h>

bool Car::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Data\\Textures\\pinksquare.jpg", nullptr, wheelTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

    auto wheelMeshData = Geometry::CreateCylinder();
    leftFrontWheel.Initialize(wheelMeshData.vertexVec.data(), wheelMeshData.indexVec.data(), wheelMeshData.vertexVec.size(), wheelMeshData.indexVec.size(), 
        device, deviceContext, wheelTexture.Get(), cb_vs_vertexshader);

    return false;
}

void Car::Draw(const XMMATRIX& viewProjectionMatrix)
{
    leftFrontWheel.Draw(viewProjectionMatrix);
}
