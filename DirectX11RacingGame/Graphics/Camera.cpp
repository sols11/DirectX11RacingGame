#include "Camera.h"

Camera::Camera()
{
	this->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->position);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR & Camera::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & Camera::GetPosition() const
{
	return this->position;
}

const XMVECTOR & Camera::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & Camera::GetRotationFloat3() const
{
	return this->rotation;
}

void Camera::SetPosition(const XMVECTOR & position)
{
	XMStoreFloat3(&this->position, position);
	this->posVector = position;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(const XMFLOAT3 & position)
{
	this->position = position;
	this->posVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR & position)
{
	this->posVector += position;
	XMStoreFloat3(&this->position, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMFLOAT3 & position)
{
	this->position.x += position.y;
	this->position.y += position.y;
	this->position.z += position.z;
	this->posVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->posVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR & rotation)
{
	this->rotVector = rotation;
	XMStoreFloat3(&this->rotation, rotation);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMFLOAT3 & rotation)
{
	this->rotation = rotation;
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotation = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR & rotation)
{
	this->rotVector += rotation;
	XMStoreFloat3(&this->rotation, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMFLOAT3 & rotation)
{
	this->rotation.x += rotation.x;
	this->rotation.y += rotation.y;
	this->rotation.z += rotation.z;
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
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

const XMVECTOR & Camera::GetForwardVector()
{
	return this->vecForward;
}

const XMVECTOR & Camera::GetRightVector()
{
	return this->vecRight;
}

const XMVECTOR & Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR & Camera::GetLeftVector()
{
	return this->vec_left;
}

void Camera::UpdateViewMatrix() // 相机的移动和旋转由viewMatrix负责
{
	// 相机旋转矩阵
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
	// 计算摄像机的朝向
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	// 朝向的实际位置
	camTarget += this->posVector;
	// 上方向
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	// 重建观察矩阵
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
    // 计算相机的4个方向
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
	this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void Camera::UpdateWorldMatrix(const XMMATRIX& parentMatrix)
{
    // 相机旋转矩阵
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * parentMatrix;
    // 计算摄像机的朝向
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    // 朝向的实际位置
    camTarget += this->posVector;
    // 上方向
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    // 重建观察矩阵
    this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
    // 计算相机的4个方向
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
    this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}