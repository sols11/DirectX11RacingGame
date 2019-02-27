#include "Object.h"

const XMVECTOR & Object::GetPositionVector() const
{
    return this->positionVector;
}

const XMFLOAT3 & Object::GetPositionFloat3() const
{
    return this->position;
}

const XMVECTOR & Object::GetRotationVector() const
{
    return this->rotationVector;
}

const XMFLOAT3 & Object::GetRotationFloat3() const
{
    return this->rotation;
}

void Object::SetPosition(const XMVECTOR & position)
{
    XMStoreFloat3(&this->position, position);
    this->positionVector = position;
    this->UpdateWorldMatrix();
}

void Object::SetPosition(const XMFLOAT3 & position)
{
    this->position = position;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Object::SetPosition(float x, float y, float z)
{
    this->position = XMFLOAT3(x, y, z);
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Object::AdjustPosition(const XMVECTOR & position)
{
    this->positionVector += position;
    XMStoreFloat3(&this->position, this->positionVector);
    this->UpdateWorldMatrix();
}

void Object::AdjustPosition(const XMFLOAT3 & position)
{
    this->position.x += position.x;
    this->position.y += position.y;
    this->position.z += position.z;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Object::AdjustPosition(float x, float y, float z)
{
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
    this->positionVector = XMLoadFloat3(&this->position);
    this->UpdateWorldMatrix();
}

void Object::SetRotation(const XMVECTOR & rotation)
{
    this->rotationVector = rotation;
    XMStoreFloat3(&this->rotation, rotation);
    this->UpdateWorldMatrix();
}

void Object::SetRotation(const XMFLOAT3 & rotation)
{
    this->rotation = rotation;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Object::SetRotation(float x, float y, float z)
{
    this->rotation = XMFLOAT3(x, y, z);
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Object::AdjustRotation(const XMVECTOR & rotation)
{
    this->rotationVector += rotation;
    XMStoreFloat3(&this->rotation, this->rotationVector);
    this->UpdateWorldMatrix();
}

void Object::AdjustRotation(const XMFLOAT3 & rotation)
{
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Object::AdjustRotation(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    this->rotationVector = XMLoadFloat3(&this->rotation);
    this->UpdateWorldMatrix();
}

void Object::SetLookAtPos(XMFLOAT3 lookAtPos)
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

const XMVECTOR & Object::GetForwardVector()
{
    return this->vecForward;
}

const XMVECTOR & Object::GetRightVector()
{
    return this->vecRight;
}

const XMVECTOR & Object::GetUpVector()
{
    return this->vecUp;
}

const XMMATRIX & Object::GetWorldMatrix()
{
    return this->worldMatrix;
}

void XM_CALLCONV Object::SetWorldMatrix(XMMATRIX world)
{
    worldMatrix = world;
}

void Object::UpdateWorldMatrix(Object* parent)
{
    this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
    if (parent != nullptr)
        this->worldMatrix *= parent->GetWorldMatrix();
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
    this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vecUp = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
}
