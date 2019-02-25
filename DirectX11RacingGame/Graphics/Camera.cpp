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
    Model::UpdateWorldMatrix(parent);

    this->positionVector = XMVector3TransformCoord(this->positionVector, this->worldMatrix);
    XMFLOAT3 worldRotation = this->rotation;
    if (parent != nullptr)
    {
        worldRotation.x += parent->GetRotationFloat3().x;
        worldRotation.y += parent->GetRotationFloat3().y;
        worldRotation.z += parent->GetRotationFloat3().z;
    }
    // �����ת����
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(worldRotation.x, worldRotation.y, worldRotation.z);
    // ����������ĳ���
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    // �����ʵ��λ��
    camTarget += positionVector;
    // �Ϸ���
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    // �ؽ��۲����
    this->viewMatrix = XMMatrixLookAtLH(positionVector, camTarget, upDir);

}