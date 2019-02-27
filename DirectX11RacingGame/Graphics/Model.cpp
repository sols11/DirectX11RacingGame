#include "Model.h"

bool Model::Initialize(std::vector<Vertex>& vertexVector, std::vector<DWORD>& indexVector, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->texture = texture;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;
    meshes.push_back(Mesh(this->device, this->deviceContext, vertexVector, indexVector));

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateWorldMatrix();

    return true;
}

bool Model::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    try
    {
        if (!this->LoadModel(filePath))
            return false;
    }
    catch (COMException & exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateWorldMatrix();
    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView * texture)
{
    this->texture = texture;
}

void Model::Draw(const XMMATRIX & viewProjectionMatrix)
{
    // ¼ÆËã²¢±£´æWorld-View-Projection¾ØÕó
    this->cb_vs_vertexshader->data.matrix = this->worldMatrix * viewProjectionMatrix;
    this->cb_vs_vertexshader->data.matrix = XMMatrixTranspose(this->cb_vs_vertexshader->data.matrix);
    this->cb_vs_vertexshader->ApplyChanges();
    this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());
    this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture

    if (!meshes.empty())
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw();
        }
    }
    else
    {
        this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        UINT offset = 0;
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
        this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0); //Draw
    }
}

bool Model::LoadModel(const std::string & filePath)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filePath,
        aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    this->ProcessNode(pScene->mRootNode, pScene);
    return true;
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(this->ProcessMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
    // Data to fill
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    //Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // Get indices
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(this->device, this->deviceContext, vertices, indices);
}

