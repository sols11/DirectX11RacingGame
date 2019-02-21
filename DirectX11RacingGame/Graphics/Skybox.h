#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
using namespace DirectX;

class Skybox
{
public:
    bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader, float radius);   //初始化天空盒函数
    void Draw(const XMMATRIX& viewProjectionMatrix);
    void SetTexture(ID3D11ShaderResourceView* texture, int index);

private:
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext * deviceContext = nullptr;
    ID3D11ShaderResourceView* texture[6];

    ConstantBuffer<CB_VS_VertexShader> * cb_vs_vertexshader = nullptr;
    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indexBuffer;

    XMMATRIX worldMatrix = XMMatrixIdentity();
    float boxRadius;

    // 前后左右上下顺序
    std::vector<std::wstring> textureFile = {
    L"Data\\Textures\\sunset1.bmp", L"Data\\Textures\\sunset2.bmp",
    L"Data\\Textures\\sunset3.bmp", L"Data\\Textures\\sunset4.bmp",
    L"Data\\Textures\\sunset5.bmp", L"Data\\Textures\\sunset6.bmp", };

    //std::vector<std::wstring> textureFile = {
    //L"Data\\Textures\\1.jpg", L"Data\\Textures\\2.jpg",
    //L"Data\\Textures\\3.jpg", L"Data\\Textures\\4.jpg",
    //L"Data\\Textures\\5.jpg", L"Data\\Textures\\6.jpg", };
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texturePtr[6];
};
