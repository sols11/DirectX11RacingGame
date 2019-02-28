#pragma once
#include "Object.h"


class Light :public Object
{
public:
    bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
    void SetLightProperties(XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f), float strength = 1.0f);

private:
    // 暂时就只实现了一个全局光
    ConstantBuffer<CB_PS_Light> cb_vs_light;
};
