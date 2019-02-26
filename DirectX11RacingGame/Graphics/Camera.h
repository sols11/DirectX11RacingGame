#pragma once
#include <DirectXMath.h>
#include "Model.h"
using namespace DirectX;

class Camera : public Model
{
public:
    // 相机可以是没有实际模型的，因此不需要调用Initialize和Draw方法就可以使用
	Camera();
	
    enum class Mode 
    { 
        FirstPerson,    // 只能WASD控制移动的模式
        ThirdPerson,    // 跟随物体，并可绕物体旋转的模式
        Free            // 自由自动和旋转的模式
    };
    Mode mode;

    void SetFrustum(float fovDegrees, float aspectRatio, float nearZ, float farZ);
    void SetTarget(Model* target);
    void RemoveTarget();
    void UpdateViewMatrix();

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;

    float GetDistance() const;
    float GetRotationX() const;
    float GetRotationY() const;
    // 绕物体垂直旋转(限制角度)
    void RotateX(float rad);
    // 绕物体水平旋转
    void RotateY(float rad);
    // 拉近物体
    void Approach(float dist);
    // 设置初始绕X轴的弧度(限制角度)
    void SetRotationX(float rotX);
    // 设置初始绕Y轴的弧度
    void SetRotationY(float rotY);
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
};
