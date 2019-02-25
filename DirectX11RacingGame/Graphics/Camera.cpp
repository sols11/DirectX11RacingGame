#include "Camera.h"

Camera::Camera()
{
    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);
    this->UpdateWorldMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void Camera::SetTarget(Model * target)
{
    this->target = target;
}

const XMMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

//void Camera::UpdateViewMatrix() // ������ƶ�����ת��viewMatrix����
//{
//	// �����ת����
//	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
//	// ����������ĳ���
//	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
//	// �����ʵ��λ��
//	camTarget += this->posVector;
//	// �Ϸ���
//	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
//	// �ؽ��۲����
//	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
//    // ���������4������
//	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
//	this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
//	this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
//}

void Camera::UpdateWorldMatrix(Model* parent)
{
    if (target != nullptr)
    {
        // ��������ϵ��Ч����XMMatrixLookAtLH����õ���ViewMatrix����
        position.x = target->GetPositionFloat3().x + distance * sinf(angleX) * cosf(angleY);
        position.z = target->GetPositionFloat3().z + distance * sinf(angleX) * sinf(angleY);
        position.y = target->GetPositionFloat3().y + distance * cosf(angleX);
        positionVector = XMLoadFloat3(&position);
        vecLook = XMVector3Normalize(XMLoadFloat3(&target->GetPositionFloat3()) - positionVector);
    }
    else
    {
        XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
        vecLook = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    }

    vecRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), vecLook));
    vecUp = XMVector3Cross(vecLook, vecRight);

    XMFLOAT3 look, right, up;
    // ��������
    XMStoreFloat3(&look, vecLook);
    XMStoreFloat3(&right, vecRight);
    XMStoreFloat3(&up, vecUp);

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
    return angleX;
}

float Camera::GetRotationY() const
{
    return angleY;
}

void Camera::RotateX(float rad)
{
    angleX -= rad;
    // ��������Ұ�Ƕ�Phi������[pi/6, pi/2]��
    // ������ֵ[0, cos(pi/6)]֮��
    if (angleX < XM_PI / 6)
        angleX = XM_PI / 6;
    else if (angleX > XM_PIDIV2)
        angleX = XM_PIDIV2;
}

void Camera::RotateY(float rad)
{
    angleY = XMScalarModAngle(angleY - rad);
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
    angleX = XMScalarModAngle(rotX);
    // ��������Ұ�Ƕ�Phi������[pi/6, pi/2]��
    // ������ֵ[0, cos(pi/6)]֮��
    if (angleX < XM_PI / 6)
        angleX = XM_PI / 6;
    else if (angleX > XM_PIDIV2)
        angleX = XM_PIDIV2;
}

void Camera::SetRotationY(float rotY)
{
    angleY = XMScalarModAngle(rotY);
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
