#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
#include "Mesh.h"
#include "Object.h"

using namespace DirectX;

class Model : public Object
{
public:
    // ��Ԥ�����vertex��index���ص�ģ��
    bool Initialize(std::vector<Vertex>& vertexVector, std::vector<DWORD>& indexVector, ID3D11Device* device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    // ���ļ��м��ص�ģ��
    bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader);
    void SetTexture(ID3D11ShaderResourceView* texture);
    void virtual Draw(const XMMATRIX & viewProjectionMatrix);

protected:
    bool LoadModel(const std::string & filePath);
    void ProcessNode(aiNode * node, const aiScene * scene);
    Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);

    std::vector<Mesh> meshes;
};