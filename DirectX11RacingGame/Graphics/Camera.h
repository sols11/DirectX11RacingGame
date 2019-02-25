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

    // ��ȡ������ľ���
    float GetDistance() const;
    // ��ȡ��X�����ת����
    float GetRotationX() const;
    // ��ȡ��Y�����ת����
    float GetRotationY() const;
    // �����崹ֱ��ת(ע��������Ұ�Ƕ�Phi������[pi/6, pi/2])
    void RotateX(float rad);
    // ������ˮƽ��ת
    void RotateY(float rad);
    // ��������
    void Approach(float dist);
    // ���ó�ʼ��X��Ļ���(ע��������Ұ�Ƕ�Phi������[pi/6, pi/2])
    void SetRotationX(float phi);
    // ���ó�ʼ��Y��Ļ���
    void SetRotationY(float theta);
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
    // ����������ϵΪ��׼����ǰ����ת�Ƕ�
    float angleY;
    float angleX;
};
