#include "HandDrawObject.h"

bool HandDrawObject::Initialize(std::vector<Vertex>& vertexVector, std::vector<DWORD>& indexVector, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    return this->Initialize(vertexVector.data(), indexVector.data(), vertexVector.size(), indexVector.size(), device, deviceContext, texture, cb_vs_vertexshader);
}

bool HandDrawObject::Initialize(Vertex* v, DWORD* indices, UINT vertexNum, UINT indexNum, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_VertexShader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->texture = texture;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    try
    {
        //Load Vertex Data
        HRESULT hr = this->vertexBuffer.Initialize(this->device, v, vertexNum);
        COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

        //Load Index Data
        hr = this->indexBuffer.Initialize(this->device, indices, indexNum);
        COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
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

void HandDrawObject::SetTexture(ID3D11ShaderResourceView * texture)
{
    this->texture = texture;
}

void HandDrawObject::Draw(const XMMATRIX & viewProjectionMatrix)
{
    //Update Constant buffer with WVP Matrix
    this->cb_vs_vertexshader->data.matrix = this->worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
    this->cb_vs_vertexshader->data.matrix = XMMatrixTranspose(this->cb_vs_vertexshader->data.matrix);
    this->cb_vs_vertexshader->ApplyChanges();
    this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

    this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture
    this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
    this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0); //Draw
}

void HandDrawObject::UpdateWorldMatrix()
{
    this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vec_up = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
    this->vec_down = XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
}

void HandDrawObject::UpdateWorldMatrix(XMMATRIX & parentMatrix)
{
    UpdateWorldMatrix();
    this->worldMatrix = this->worldMatrix * parentMatrix;
}

const XMVECTOR & HandDrawObject::GetPositionVector() const
{
    return this->posVector;
}

const XMFLOAT3 & HandDrawObject::GetPositionFloat3() const
{
    return this->pos;
}

const XMVECTOR & HandDrawObject::GetRotationVector() const
{
    return this->rotVector;
}

const XMFLOAT3 & HandDrawObject::GetRotationFloat3() const
{
    return this->rot;
}

void HandDrawObject::SetPosition(const XMVECTOR & pos)
{
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetPosition(const XMFLOAT3 & pos)
{
    this->pos = pos;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetPosition(float x, float y, float z)
{
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustPosition(const XMVECTOR & pos)
{
    this->posVector += pos;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustPosition(const XMFLOAT3 & pos)
{
    this->pos.x += pos.y;
    this->pos.y += pos.y;
    this->pos.z += pos.z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetRotation(const XMVECTOR & rot)
{
    this->rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetRotation(const XMFLOAT3 & rot)
{
    this->rot = rot;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetRotation(float x, float y, float z)
{
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustRotation(const XMVECTOR & rot)
{
    this->rotVector += rot;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustRotation(const XMFLOAT3 & rot)
{
    this->rot.x += rot.x;
    this->rot.y += rot.y;
    this->rot.z += rot.z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void HandDrawObject::AdjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void HandDrawObject::SetLookAtPos(XMFLOAT3 lookAtPos)
{
    //Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
    if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
        return;

    lookAtPos.x = this->pos.x - lookAtPos.x;
    lookAtPos.y = this->pos.y - lookAtPos.y;
    lookAtPos.z = this->pos.z - lookAtPos.z;

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

const XMVECTOR & HandDrawObject::GetForwardVector()
{
    return this->vec_forward;
}

const XMVECTOR & HandDrawObject::GetRightVector()
{
    return this->vec_right;
}

const XMVECTOR & HandDrawObject::GetBackwardVector()
{
    return this->vec_backward;
}

const XMVECTOR & HandDrawObject::GetLeftVector()
{
    return this->vec_left;
}

const XMVECTOR & HandDrawObject::GetUpVector()
{
    return this->vec_up;
}

const XMVECTOR & HandDrawObject::GetDownVector()
{
    return this->vec_down;
}