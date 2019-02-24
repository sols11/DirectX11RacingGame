#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
#include "Mesh.h"

using namespace DirectX;

class Model
{
public:
    // 从预定义的vertex和index加载的模型
    bool Initialize(std::vector<Vertex>& vertexVector, std::vector<DWORD>& indexVector, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    // 从文件中加载的模型
    bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    void SetTexture(ID3D11ShaderResourceView* texture);
    void virtual Draw(const XMMATRIX & viewProjectionMatrix);

    const XMVECTOR & GetPositionVector() const;
    const XMFLOAT3 & GetPositionFloat3() const;
    const XMVECTOR & GetRotationVector() const;
    const XMFLOAT3 & GetRotationFloat3() const;

    void SetPosition(const XMVECTOR & position);
    void SetPosition(const XMFLOAT3 & position);
    void SetPosition(float x, float y, float z);
    void AdjustPosition(const XMVECTOR & position);
    void AdjustPosition(const XMFLOAT3 & position);
    void AdjustPosition(float x, float y, float z);
    void SetRotation(const XMVECTOR & rotation);
    void SetRotation(const XMFLOAT3 & rotation);
    void SetRotation(float x, float y, float z);
    void AdjustRotation(const XMVECTOR & rotation);
    void AdjustRotation(const XMFLOAT3 & rotation);
    void AdjustRotation(float x, float y, float z);
    void SetLookAtPos(XMFLOAT3 lookAtPos);

    const XMVECTOR & GetForwardVector();
    const XMVECTOR & GetRightVector();
    const XMVECTOR & GetUpVector();
    const XMMATRIX& GetWorldMatrix();
    void XM_CALLCONV SetWorldMatrix(XMMATRIX world);
    void virtual UpdateWorldMatrix(XMMATRIX parentWorldMatrix = XMMatrixIdentity());

protected:
    bool LoadModel(const std::string & filePath);
    void ProcessNode(aiNode * node, const aiScene * scene);
    Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);

    ID3D11Device * device = nullptr;
    ID3D11DeviceContext * deviceContext = nullptr;
    ConstantBuffer<CB_VS_VertexShader> * cb_vs_vertexshader = nullptr;
    ID3D11ShaderResourceView * texture = nullptr;

    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indexBuffer;

    XMMATRIX worldMatrix = XMMatrixIdentity();

    XMVECTOR positionVector;
    XMVECTOR rotationVector;
    XMFLOAT3 position;
    XMFLOAT3 rotation;

    const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    const XMVECTOR DEFAULT_RIGHT_VECTOR =   XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMVECTOR vecForward;
    XMVECTOR vecRight;
    XMVECTOR vecUp;

    std::vector<Mesh> meshes;
};