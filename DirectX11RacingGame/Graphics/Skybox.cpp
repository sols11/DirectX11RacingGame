#include "Skybox.h"
#include <WICTextureLoader.h>

bool Skybox::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader, float radius)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;
    this->boxRadius = radius;

    // 设置6张贴图
    for (int i = 0; i < textureFile.size(); ++i)
    {
        HRESULT hr = CreateWICTextureFromFile(device, textureFile[i].c_str(), nullptr, texturePtr[i].GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
        this->SetTexture(texturePtr[i].Get(), i);
    }
    // 盒子24个顶点数据
    Vertex vertices[] =
    {
        ////前面的四个顶点
        Vertex(-boxRadius, -boxRadius,  boxRadius, 0.0f, 1.0f),
        Vertex(-boxRadius, boxRadius,   boxRadius, 0.0f, 0.0f),
        Vertex(boxRadius, -boxRadius,   boxRadius, 1.0f, 1.0f),
        Vertex(boxRadius, boxRadius,   boxRadius, 1.0f, 0.0f),

        //背面的四个顶点
        Vertex(boxRadius, -boxRadius,   -boxRadius, 0.0f, 1.0f),
        Vertex(boxRadius, boxRadius,  -boxRadius, 0.0f, 0.0f),
        Vertex(-boxRadius, -boxRadius,  -boxRadius, 1.0f, 1.0f),
        Vertex(-boxRadius, boxRadius,  -boxRadius, 1.0f, 0.0f),

        //左面的四个顶点
        Vertex(-boxRadius, -boxRadius, -boxRadius, 0.0f, 1.0f),
        Vertex(-boxRadius, boxRadius,  -boxRadius, 0.0f, 0.0f),
        Vertex(-boxRadius, -boxRadius, boxRadius, 1.0f, 1.0f),
        Vertex(-boxRadius, boxRadius,   boxRadius, 1.0f, 0.0f),

        //右面的四个顶点
        Vertex(boxRadius, -boxRadius,  boxRadius, 0.0f, 1.0f),
        Vertex(boxRadius, boxRadius,    boxRadius, 0.0f, 0.0f),
        Vertex(boxRadius, -boxRadius,  -boxRadius, 1.0f, 1.0f),
        Vertex(boxRadius, boxRadius,   -boxRadius, 1.0f, 0.0f),

        //上面的四个顶点
        Vertex(boxRadius, boxRadius,  -boxRadius, 1.0f, 0.0f),
        Vertex(boxRadius, boxRadius,   boxRadius, 1.0f, 1.0f),
        Vertex(-boxRadius, boxRadius,  -boxRadius, 0.0f, 0.0f),
        Vertex(-boxRadius, boxRadius,   boxRadius, 0.0f, 1.0f),

        //底面的四个顶点
        Vertex(boxRadius, -boxRadius,  -boxRadius, 1.0f, 0.0f),
        Vertex(boxRadius, -boxRadius,   boxRadius, 1.0f, 1.0f),
        Vertex(-boxRadius, -boxRadius,  -boxRadius, 0.0f, 0.0f),
        Vertex(-boxRadius, -boxRadius,   boxRadius, 0.0f, 1.0f),
    };
    HRESULT hr = this->vertexBuffer.Initialize(this->device, vertices, ARRAYSIZE(vertices));
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

    DWORD indices[] =
    {
        0, 1, 2,  3, 2, 1, //FRONT
        4, 5, 6,  7, 6, 5, //BACK 
        8, 9, 10,11,10, 9, //LEFT
        12,13,14,15,14,13, //RIGHT
        16,17,18,19,18,17, //TOP
        22,21,20,21,22,23, //DOWN
    };
    hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");

    return false;
}

void Skybox::SetTexture(ID3D11ShaderResourceView * texture, int index)
{
    this->texture[index] = texture;
}

void Skybox::Draw(const Camera& camera, const XMMATRIX & viewProjectionMatrix)
{
    // 根据相机移动
    XMFLOAT3 position = camera.GetPositionFloat3();
    this->worldMatrix = XMMatrixTranslation(position.x, position.y, position.z);
    this->cb_vs_vertexshader->data.matrix = this->worldMatrix * viewProjectionMatrix;
    this->cb_vs_vertexshader->data.matrix = XMMatrixTranspose(this->cb_vs_vertexshader->data.matrix);   // 转置
    this->cb_vs_vertexshader->ApplyChanges();
    this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

    UINT indexCount = this->indexBuffer.IndexCount() / 6;
    this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
    for (int i = 0; i < 6; ++i)
    {
        this->deviceContext->PSSetShaderResources(0, 1, &this->texture[i]); //Set Texture
        this->deviceContext->DrawIndexed(indexCount, indexCount*i, 0); //Draw
    }
}