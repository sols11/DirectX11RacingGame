#include "Car.h"

bool Car::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_VertexShader> & cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    auto wheelMeshData = Geometry::CreateCylinder();
    leftFrontWheel.Initialize(wheelMeshData.vertexVec)

    return false;
}
