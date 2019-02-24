#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;

	const XMVECTOR & GetPositionVector() const;
	const XMFLOAT3 & GetPosition() const;
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
	const XMVECTOR & GetBackwardVector();
	const XMVECTOR & GetLeftVector();
    void UpdateWorldMatrix(const XMMATRIX& parentMatrix);
private:
	void UpdateViewMatrix();
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vecForward;
	XMVECTOR vec_left;
	XMVECTOR vecRight;
	XMVECTOR vec_backward;
};
