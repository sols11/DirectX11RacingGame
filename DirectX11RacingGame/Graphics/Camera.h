#pragma once
#include <DirectXMath.h>
#include "Model.h"
using namespace DirectX;

class Camera : public Model
{
public:
    // 相机可以是没有实际模型的，因此不需要调用Initialize和Draw方法就可以使用
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
    void SetTarget(Model* target);

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;
    void UpdateWorldMatrix(Model* parent = nullptr);    // camera只受target物体影响，这里parent只是为了保持签名一致

private:
    Model* target = nullptr;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

};
