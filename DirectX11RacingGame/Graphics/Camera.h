#pragma once
#include <DirectXMath.h>
#include "Model.h"
using namespace DirectX;

class Camera : public Model
{
public:
    // ���������û��ʵ��ģ�͵ģ���˲���Ҫ����Initialize��Draw�����Ϳ���ʹ��
	Camera();
	
    enum class Mode 
    { 
        FirstPerson,    // ֻ��WASD�����ƶ���ģʽ
        ThirdPerson,    // �������壬������������ת��ģʽ
        Free            // �����Զ�����ת��ģʽ
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
    // �����崹ֱ��ת(���ƽǶ�)
    void RotateX(float rad);
    // ������ˮƽ��ת
    void RotateY(float rad);
    // ��������
    void Approach(float dist);
    // ���ó�ʼ��X��Ļ���(���ƽǶ�)
    void SetRotationX(float rotX);
    // ���ó�ʼ��Y��Ļ���
    void SetRotationY(float rotY);
    // ���ó�ʼ����
    void SetDistance(float dist);
    // ������С����������
    void SetDistanceMinMax(float minDist, float maxDist);

private:
    Model* target = nullptr;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
    XMVECTOR vecLook;
    float distance;
    // ��С������룬����������
    float minDistance, maxDistance;
};
