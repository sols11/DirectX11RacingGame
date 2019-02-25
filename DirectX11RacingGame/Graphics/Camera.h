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

    // 获取与物体的距离
    float GetDistance() const;
    // 获取绕X轴的旋转方向
    float GetRotationX() const;
    // 获取绕Y轴的旋转方向
    float GetRotationY() const;
    // 绕物体垂直旋转(注意上下视野角度Phi限制在[pi/6, pi/2])
    void RotateX(float rad);
    // 绕物体水平旋转
    void RotateY(float rad);
    // 拉近物体
    void Approach(float dist);
    // 设置初始绕X轴的弧度(注意上下视野角度Phi限制在[pi/6, pi/2])
    void SetRotationX(float phi);
    // 设置初始绕Y轴的弧度
    void SetRotationY(float theta);
    // 设置初始距离
    void SetDistance(float dist);
    // 设置最小最大允许距离
    void SetDistanceMinMax(float minDist, float maxDist);

private:
    Model* target = nullptr;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
    XMVECTOR vecLook;
    float distance;
    // 最小允许距离，最大允许距离
    float minDistance, maxDistance;
    // 以世界坐标系为基准，当前的旋转角度
    float angleY;
    float angleX;
};
