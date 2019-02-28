#include "Light.h"

bool Light::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
    this->device = device;
    this->deviceContext = deviceContext;
    
    HRESULT hr = this->cb_vs_light.Initialize(device, deviceContext);
    COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

    return true;
}

void Light::SetLightProperties(XMFLOAT3 color, float strength)
{
    this->cb_vs_light.data.ambientLightColor = color;
    this->cb_vs_light.data.ambientLightStrength = strength;
    this->cb_vs_light.ApplyChanges();
    this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_vs_light.GetAddressOf());
}
