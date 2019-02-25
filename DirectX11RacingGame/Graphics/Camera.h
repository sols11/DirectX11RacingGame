#pragma once
#include <DirectXMath.h>
#include "Model.h"
using namespace DirectX;

class Camera : public Model
{
public:
    // ���������û��ʵ��ģ�͵ģ���˲���Ҫ����Initialize��Draw�����Ϳ���ʹ��
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
    void SetTarget(Model* target);

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;
    void UpdateWorldMatrix(Model* parent = nullptr);    // cameraֻ��target����Ӱ�죬����parentֻ��Ϊ�˱���ǩ��һ��

private:
    Model* target = nullptr;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

};
