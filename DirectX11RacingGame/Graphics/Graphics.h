#pragma once
#include "Adapter.h"
#include "Shader.h"
#include "Vertex.h"
#include "ConstantBuffer.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Camera.h"
#include "..\Basic\Timer.h"
#include "Model.h"
#include "Skybox.h"
#include "Car.h"

class Graphics
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
    Camera camera;
    Model model;
    Model plane;
    Skybox skybox;
    Car car;

private:
    bool InitializeDirectX(HWND hwnd);
    bool InitializeShaders();
    bool InitializeScene();

    // 设备，上下文，交换链，渲染目标视图，深度模板，光栅化，混合，采样器
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

    VertexShader vertexshader;
    PixelShader pixelshader;
    ConstantBuffer<CB_VS_VertexShader> cb_vs_vertexshader;
    ConstantBuffer<CB_PS_PixelShader> cb_ps_pixelshader;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> spriteFont;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

    int windowWidth = 0;
    int windowHeight = 0;
    Timer fpsTimer;
};
