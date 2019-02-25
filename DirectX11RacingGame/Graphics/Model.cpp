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

bool Model::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->texture = texture;
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
    // 计算并保存World-View-Projection矩阵
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

void Model::UpdateWorldMatrix(Model* parent)
{
    this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
    if (parent != nullptr)
        this->worldMatrix *= parent->GetWorldMatrix();
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
    this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vecUp = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
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

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

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

const XMVECTOR & Model::GetPositionVector() const
{
    return this->positionVector;
}

const XMFLOAT3 & Model::GetPositionFloat3() const
{
    return this->position;
}

const XMVECTOR & Model::GetRotationVector() const
{
    return this->rotationVector;
}

const XMFLOAT3 & Model::GetRotationFloat3() const
{
    return this->rotation;
}

void Model::SetPosition(const XMVECTOR & position)
{
    XMStoreFloat3(&this->position, position);
    this->positionVector = position;
    this->UpdateWorldMatrix();
}

void Model::SetPosition(const XMFLOAT3 & position)
{
    this->position = position;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
    this->position = XMFLOAT3(x, y, z);
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMVECTOR & position)
{
    this->positionVector += position;
    XMStoreFloat3(&this->position, this->positionVector);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMFLOAT3 & position)
{
    this->position.x += position.x;
    this->position.y += position.y;
    this->position.z += position.z;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMVECTOR & rotation)
{
    this->rotationVector = rotation;
    XMStoreFloat3(&this->rotation, rotation);
    this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMFLOAT3 & rotation)
{
    this->rotation = rotation;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Model::SetRotation(float x, float y, float z)
{
    this->rotation = XMFLOAT3(x, y, z);
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMVECTOR & rotation)
{
    this->rotationVector += rotation;
    XMStoreFloat3(&this->rotation, this->rotationVector);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMFLOAT3 & rotation)
{
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Model::SetLookAtPos(XMFLOAT3 lookAtPos)
{
    // 检查pos是否与camera pos相同。它们不可能相同，因为这没有意义，会导致未定义的行为。
    if (lookAtPos.x == this->position.x && lookAtPos.y == this->position.y && lookAtPos.z == this->position.z)
        return;

    lookAtPos.x = this->position.x - lookAtPos.x;
    lookAtPos.y = this->position.y - lookAtPos.y;
    lookAtPos.z = this->position.z - lookAtPos.z;

    float pitch = 0.0f;
    if (lookAtPos.y != 0.0f)
    {
        const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
        pitch = atan(lookAtPos.y / distance);
    }

    float yaw = 0.0f;
    if (lookAtPos.x != 0.0f)
    {
        yaw = atan(lookAtPos.x / lookAtPos.z);
    }
    if (lookAtPos.z > 0)
        yaw += XM_PI;

    this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & Model::GetForwardVector()
{
    return this->vecForward;
}

const XMVECTOR & Model::GetRightVector()
{
    return this->vecRight;
}

const XMVECTOR & Model::GetUpVector()
{
    return this->vecUp;
}

const XMMATRIX & Model::GetWorldMatrix()
{
    return this->worldMatrix;
}

void XM_CALLCONV Model::SetWorldMatrix(XMMATRIX world)
{
    worldMatrix = world;
}
