#include "Mesh.h"

Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
    this->deviceContext = deviceContext;

    HRESULT hr = this->vertexBuffer.Initialize(device, vertices.data(), vertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

    hr = this->indexBuffer.Initialize(device, indices.data(), indices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

Mesh::Mesh(const Mesh & mesh)
{
    this->deviceContext = mesh.deviceContext;
    this->indexBuffer = mesh.indexBuffer;
    this->vertexBuffer = mesh.vertexBuffer;
}

void Mesh::Draw()
{
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
    this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0);
}