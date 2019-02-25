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

//void Camera::UpdateViewMatrix() // 相机的移动和旋转由viewMatrix负责
//{
//	// 相机旋转矩阵
//	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
//	// 计算摄像机的朝向
//	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
//	// 朝向的实际位置
//	camTarget += this->posVector;
//	// 上方向
//	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
//	// 重建观察矩阵
//	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
//    // 计算相机的4个方向
//	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
//	this->vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
//	this->vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
//}

void Camera::UpdateWorldMatrix(Model* parent)
{
    Model::UpdateWorldMatrix(parent);

    this->positionVector = XMVector3TransformCoord(this->positionVector, this->worldMatrix);
    XMFLOAT3 worldRotation = this->rotation;
    if (parent != nullptr)
    {
        worldRotation.x += parent->GetRotationFloat3().x;
        worldRotation.y += parent->GetRotationFloat3().y;
        worldRotation.z += parent->GetRotationFloat3().z;
    }
    // 相机旋转矩阵
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(worldRotation.x, worldRotation.y, worldRotation.z);
    // 计算摄像机的朝向
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    // 朝向的实际位置
    camTarget += positionVector;
    // 上方向
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    // 重建观察矩阵
    this->viewMatrix = XMMatrixLookAtLH(positionVector, camTarget, upDir);

}