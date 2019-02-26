#include "Camera.h"

Camera::Camera()
{
    // �����˳Ƴ�ʼ�ӽ�
    mode = Mode::ThirdPerson;
    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(1.0f, -XM_PIDIV2, 0.0f);
    this->UpdateWorldMatrix();
}

void Camera::SetFrustum(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void Camera::SetTarget(Model * target)
{
    this->target = target;
}

void Camera::RemoveTarget()
{
    this->target = nullptr;
}

const XMMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    // �����˳�ģʽ
    if (target != nullptr && mode == Mode::ThirdPerson)
    {
        // ��������ϵ�������ʵ��λ�ã�Ч����XMMatrixLookAtLH����õ���ViewMatrix����
        position.x = target->GetPositionFloat3().x + distance * sinf(rotation.x) * cosf(rotation.y);
        position.z = target->GetPositionFloat3().z + distance * sinf(rotation.x) * sinf(rotation.y);
        position.y = target->GetPositionFloat3().y + distance * cosf(rotation.x);
        positionVector = XMLoadFloat3(&position);
        vecLook = XMVector3Normalize(XMLoadFloat3(&target->GetPositionFloat3()) - positionVector);
    }
    // ����ģʽ
    else
    {
        XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
        vecLook = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, rotationMatrix);
    }

    vecRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), vecLook));
    vecUp = XMVector3Cross(vecLook, vecRight);

    XMFLOAT3 look, right, up;
    // ��������
    XMStoreFloat3(&look, vecLook);
    XMStoreFloat3(&right, vecRight);
    XMStoreFloat3(&up, vecUp);
    // ����ǰ�����Ľ���õ��ӵ����
    this->viewMatrix = {
        right.x, up.x, look.x, 0.0f,
        right.y, up.y, look.y, 0.0f,
        right.z, up.z, look.z, 0.0f,
        -XMVectorGetX(XMVector3Dot(positionVector, vecRight)), -XMVectorGetX(XMVector3Dot(positionVector, vecUp)), -XMVectorGetX(XMVector3Dot(positionVector, vecLook)), 1.0f
    };
}

float Camera::GetDistance() const
{
    return distance;
}

float Camera::GetRotationX() const
{
    return rotation.x;
}

float Camera::GetRotationY() const
{
    return rotation.y;
}

void Camera::RotateX(float rad)
{
    rotation.x -= rad;
    if (rotation.x < XM_PI / 6)
        rotation.x = XM_PI / 6;
    else if (rotation.x > XM_PIDIV2)
        rotation.x = XM_PIDIV2;
}

void Camera::RotateY(float rad)
{
    rotation.y = XMScalarModAngle(rotation.y - rad);
}

void Camera::Approach(float dist)
{
    distance += dist;
    // ���ƾ�����[minDistance, maxDistance]֮��
    if (distance < minDistance)
        distance = minDistance;
    else if (distance > maxDistance)
        distance = maxDistance;
}

void Camera::SetRotationX(float rotX)
{
    rotation.x = XMScalarModAngle(rotX);
    // ��������Ұ�Ƕ�������[pi/6, pi/2]��������ֵ[0, cos(pi/6)]֮��
    if (rotation.x < XM_PI / 6)
        rotation.x = XM_PI / 6;
    else if (rotation.x > XM_PIDIV2)
        rotation.x = XM_PIDIV2;
}

void Camera::SetRotationY(float rotY)
{
    rotation.y = XMScalarModAngle(rotY);
}

void Camera::SetDistance(float dist)
{
    distance = dist;
}

void Camera::SetDistanceMinMax(float minDist, float maxDist)
{
    minDistance = minDist;
    maxDistance = maxDist;
}
